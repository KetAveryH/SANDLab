#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <chrono>
#include <string>
#include <cstdio>
#include <iostream>

#include "NimBLEDevice.h"

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#include "dl_tool.hpp"
#include "../model/mnist_model.hpp"

// UART Variables
static const int RX_BUF_SIZE = 1024;
#define TXD_PIN (GPIO_NUM_21)
#define RXD_PIN (GPIO_NUM_47)
#define SIGNAL_PIN GPIO_NUM_4  // OUTPUT DATA SEND/RECEIVED PIN

// Size of MTU
#define SIZEOFMTU 516

// More timer setup
using namespace std::chrono;
std::map<int, steady_clock::time_point> timers;
int timer_id_counter = 0; // Global counter for generating unique IDs

#define serverName "BLE SERVER 2"

extern "C"
{
  void app_main(void);
}

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

// The remote service we wish to connect to.
static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");
// The characteristic of the remote service we are interested in.
static BLEUUID charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;
std::vector<uint16_t> values;
int16_t *nums_to_send;
int sizeOfTensor;

void configure_gpio() 
{
  gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << SIGNAL_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

// UART Setup
void initUART(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

// Send data using UART
int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

__attribute__((aligned(16))) int16_t example_element[] = {};

// onWrite callbacks
class MyCallbacks : public NimBLECharacteristicCallbacks
{
public:
  int64_t start_time;
  bool measuring;

  MyCallbacks() : start_time(0), measuring(false) {}
  void onWrite(NimBLECharacteristic *pCharacteristic, NimBLEConnInfo &connInfo) override
  {
    sendData("UART", "Got Data");
    pCharacteristic->notify();
    size_t before_receive_free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    printf("Free heap memory before receiving: %d bytes\n", before_receive_free_heap);

    if (!measuring)
    {
      start_time = esp_timer_get_time();
      measuring = true;
    }
    std::string value = pCharacteristic->getValue();

    if (!value.empty() && value != "TRAIN")
    {
      if (measuring)
      {
        int64_t end_time = esp_timer_get_time();
        printf("Data collection took %lld microseconds\n", end_time - start_time);
        measuring = false;
      }

      // Convert the remaining bytes back into 16-bit integers
      for (size_t i = 4; i < value.size(); i += 2)
      {
        uint16_t val = (uint16_t)value[i] | ((uint16_t)value[i + 1] << 8);
        values.push_back(val);
      }
    }
    else if (value == "TRAIN")
    {
      gpio_set_level(SIGNAL_PIN, 0);  
      printf("GPIO SET LEVEL BACK TO 0\n");    
      vTaskDelay(10 / portTICK_PERIOD_MS); // TODO Unsure if this 25 milliseconds is too small
      printf("GPIO SET LEVEL BACK TO 1\n");
      gpio_set_level(SIGNAL_PIN, 1);

      for (const auto &val : values)
      {
        printf("%d ", val);
      }
      printf("\n");

      auto start_time = std::chrono::high_resolution_clock::now();

      //PARTITION 5
      int input_height = 7;
      int input_width = 7;
      int input_channel = 160;
      int input_exponent = -9;
      int16_t *model_input = (int16_t *)dl::tool::malloc_aligned_prefer(input_height*input_width*input_channel, sizeof(int16_t *));
      for(int i=0 ;i<input_height*input_width*input_channel; i++){
          float normalized_input = example_element[i];
          model_input[i] = (int16_t)DL_CLIP(normalized_input * (1 << -input_exponent), -32768, 32767);
      } 

      Tensor<int16_t> input;
      input.set_element((int16_t *)model_input).set_exponent(input_exponent).set_shape({7, 7, 160}).set_auto_free(false);

      MNIST model;

      dl::tool::Latency latency;

      // model forward
      latency.start();
      
      model.forward(input);
      // model.build(input);
      Tensor<int16_t> &my_tensor = model.l67.get_output();
      auto sizeOfTensor = my_tensor.get_size();
      // my_tensor.print_all();
      auto nums_to_send = my_tensor.all_list();
      latency.end();
      latency.print("MobileNetV2", "forward");

      // // parse
      // int16_t *score = model.l4.get_output().get_element_ptr();
      // int16_t max_score = score[0];
      // int max_index = 0;
      // printf("%d, ", max_score);

      // for (size_t i = 1; i < 10; i++)
      // {
      //   printf("%d, ", score[i]);
      //   if (score[i] > max_score)
      //   {
      //     max_score = score[i];
      //     max_index = i;
      //   }
      // }
      // printf("\nPrediction Result: %d\n", max_index);

      auto end_time = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
      std::cout << "Execution time: " << duration.count() << " ms" << std::endl;

      values.clear();
    }

    size_t after_receive_free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    printf("Free heap memory after receiving: %d bytes\n", after_receive_free_heap);

    size_t memory_used_for_receiving = before_receive_free_heap - after_receive_free_heap;
    printf("Memory used for receiving: %d bytes\n", memory_used_for_receiving);
  }
};

// Server Callbacks
class MyServerCallbacks : public NimBLEServerCallbacks
{
  void onConnect(NimBLEServer *pServer, NimBLEConnInfo& connInfo)
  {
    sendData("UART", "A client has connected to our server");
    printf("A client has connected to our server!\n");
  }

  void onDisconnect(NimBLEServer *pServer, NimBLEConnInfo& connInfo, int reason)
  {
    sendData("UART", "A client has disconnected from our server");
    printf("A client has disconnected from our server!\n");
  }
};

void serverSetUp(void)
{
  auto start_time = std::chrono::high_resolution_clock::now();
  BLEDevice::init(serverName);
  BLEDevice::setMTU(SIZEOFMTU);
  sendData("UART", "Starting Server");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    NIMBLE_PROPERTY::READ |
    NIMBLE_PROPERTY::WRITE |
    NIMBLE_PROPERTY::NOTIFY
);

  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);

  sendData("UART", "Starting Advertising");
  BLEDevice::startAdvertising();
  printf("Characteristic defined! Now you can read it in your phone!\n");
  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  std::cout << "Server start time: " << duration.count() << " ms" << std::endl;
}

void app_main(void)
{
  // CONFIGURE UART PINS
  initUART();

  // CONFIGURE SIGNAL PIN
  configure_gpio();
  gpio_set_level(SIGNAL_PIN, 1);

  // SERVER
  serverSetUp();
}