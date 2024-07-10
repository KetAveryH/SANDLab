#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <chrono>
#include <string>
#include <cstdio>

#include "NimBLEDevice.h"

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"

#include "dl_tool.hpp"
#include "../model/mnist_model.hpp"
#include "church.h"

// UART Variables
static const int RX_BUF_SIZE = 516;
#define TXD_PIN (GPIO_NUM_21)
#define RXD_PIN (GPIO_NUM_47)
#define SIGNAL_PIN GPIO_NUM_4

// Size of MTU
#define SIZEOFMTU 516

using namespace std::chrono;
std::map<int, steady_clock::time_point> timers;
int timer_id_counter = 0; // Global counter for generating unique IDs

#define connectTo "BLE SERVER 2"

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

static bool doConnect = false;
static bool connected = false;
static bool doScan = false;
static BLERemoteCharacteristic *pRemoteCharacteristic;
static BLEAdvertisedDevice *myDevice;

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

// Notify Callbacks
static void notifyCallback(
    BLERemoteCharacteristic *pBLERemoteCharacteristic,
    uint8_t *pData,
    size_t length,
    bool isNotify)
{
    printf("Notify callback for characteristic %s of data length %d data: %s\n",
           pBLERemoteCharacteristic->getUUID().toString().c_str(),
           length,
           (char *)pData);

    // Extract the timer ID from the received data
    sendData("UART", "Received Notification");
    int timer_id;
    memcpy(&timer_id, pData, sizeof(timer_id));

    if (timers.find(timer_id) != timers.end())
    {
        auto start_time = timers[timer_id];
        auto end_time = steady_clock::now();
        auto duration = duration_cast<milliseconds>(end_time - start_time).count();

        printf("Time taken for timer ID %d: %lld ms\n", timer_id, duration);

        timers.erase(timer_id); // Remove the timer for this chunk
    }
}

// Client Callbacks
class MyClientCallback : public BLEClientCallbacks
{
  void onConnect(BLEClient *pClient)
  {
    sendData("UART", "Client connected to a server");
    printf("Client connected to a server!");
    int rssi = pClient->getRssi();
    printf("RSSI value: %d\n", rssi);
  }
  void onDisconnect(BLEClient *pclient, int reason)
  {
    connected = false;
    sendData("UART", "Client disconnected from a server");
    printf("Client disconnected from a server!");
  }
};

// Process to connect to server
bool connectToServer()
{
  printf("Forming a connection to %s\n", myDevice->getAddress().toString().c_str());

  BLEClient *pClient = BLEDevice::createClient();
  printf(" - Created client\n");

  pClient->setClientCallbacks(new MyClientCallback());
  NimBLEDevice::setMTU(SIZEOFMTU);

  pClient->connect(myDevice);
  printf(" - Connected to server\n");

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService *pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr)
  {
    printf("Failed to find our service UUID: %s\n", serviceUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  printf(" - Found our service\n");

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr)
  {
    printf("Failed to find our characteristic UUID: %s\n", charUUID.toString().c_str());
    pClient->disconnect();
    return false;
  }
  printf(" - Found our characteristic\n");

  // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead())
  {
    std::string value = pRemoteCharacteristic->readValue();
    printf("The characteristic value was: %s\n", value.c_str());
  }

  // Set up notifications
  if (pRemoteCharacteristic->canNotify())
  {
    printf("Attempting to subscribe to notifications...\n");
    bool success = pRemoteCharacteristic->subscribe(true, notifyCallback, true);
    if (success)
    {
      printf("Successfully subscribed to notifications.\n");
    }
    else
    {
      printf("Failed to subscribe to notifications.\n");
    }
  }

  connected = true;
  return true;
}

// Client Scan Found Advertising Device
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
  void onResult(BLEAdvertisedDevice *advertisedDevice)
  {
    // Check if the device has a name before printing
    if (!advertisedDevice->getName().empty())
    {
      printf("BLE Advertised Device found: %s\n", advertisedDevice->toString().c_str());

      // Check if the device has the service UUID we are looking for
      if (advertisedDevice->haveServiceUUID() && advertisedDevice->isAdvertisingService(serviceUUID))
      {
        // Check if the device name is "BLE SERVER 2"
        if (advertisedDevice->getName() == connectTo)
        {
          BLEDevice::getScan()->stop();
          myDevice = advertisedDevice;
          doConnect = true;
          doScan = true;
        }
      }
    }
  }
};

// Send client data to server
void sendDataChunks(int16_t *intermediates, size_t num_values)
{
    sendData("UART", "Starting to send data");
    
    auto start_time = std::chrono::high_resolution_clock::now();
    size_t chunk_size = (NimBLEDevice::getMTU() - 3 - sizeof(int64_t)) / 2; // Adjust based on actual MTU size

    size_t before_send_free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    printf("Free heap memory before sending: %d bytes\n", before_send_free_heap);

    timer_id_counter = 0;

    //Set to low
    gpio_set_level(SIGNAL_PIN, 0);      
    printf("GPIO SET LEVEL BACK TO 0\n");
    // vTaskDelay(10 / portTICK_PERIOD_MS); // TODO Unsure if this 25 milliseconds is too small
    printf("GPIO SET LEVEL BACK TO 1\n");
    gpio_set_level(SIGNAL_PIN, 1);
    for (size_t i = 0; i < num_values; i += chunk_size)
    {
        size_t remaining = num_values - i;
        size_t this_chunk_size = remaining < chunk_size ? remaining : chunk_size;

        uint8_t values[this_chunk_size * 2 + sizeof(int)]; // Include space for the timer ID
        int timer_id = timer_id_counter++;

        // Copy the timer ID to the beginning of the data
        memcpy(values, &timer_id, sizeof(timer_id));

        for (size_t j = 0; j < this_chunk_size; j++)
        {
            values[sizeof(timer_id) + j * 2] = intermediates[i + j] & 0xFF;
            values[sizeof(timer_id) + j * 2 + 1] = (intermediates[i + j] >> 8) & 0xFF;
        }

        // Start timer for this chunk
        timers[timer_id] = steady_clock::now();

        bool success = pRemoteCharacteristic->writeValue(values, sizeof(values), false);
        sendData("UART", "Sent data");
        if (!success)
        {
            printf("Failed to write characteristic\n");
            return;
        }
        vTaskDelay(80 / portTICK_PERIOD_MS);
    }

    const char *buf = "TRAIN";
    pRemoteCharacteristic->writeValue((uint8_t *)buf, strlen(buf), false);
    sendData("UART", "Sent TRAIN");

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "Time to send: " << duration.count() << " ms" << std::endl;

    size_t after_send_free_heap = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    printf("Free heap memory after sending: %d bytes\n", after_send_free_heap);

    size_t memory_used_for_sending = before_send_free_heap - after_send_free_heap;
    printf("Memory used for sending: %d bytes\n", memory_used_for_sending);

    printf("Sent this many bytes: %zu\n", sizeOfTensor);
}

// Connect to server
void connectTask(void *parameter)
{
  for (;;)
  {
    if (doConnect == true)
    {
      if (connectToServer())
      {
        printf("We are now connected to the BLE Server.\n");
        // Send the data in chunks
        sendDataChunks(nums_to_send, sizeOfTensor);
        
      }
      else
      {
        printf("We have failed to connect to the server; there is nothin more we will do.\n");
      }
      doConnect = false;
    }

    // If we are connected to a peer BLE Server, update the characteristic each time we are reached
    // with the current time since boot.
    if (connected)
    {
      //Loop
    }
    else if (doScan)
    {
      BLEDevice::getScan()->start(0);
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }

  vTaskDelete(NULL);
}

void clientSetUp(void)
{
  printf("Client Start\n");
  sendData("UART", "Client Start");
  auto start_time = std::chrono::high_resolution_clock::now();

  BLEDevice::init("");
  BLEScan *pBLEScan = BLEDevice::getScan();
  pBLEScan->setScanCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);

  xTaskCreate(connectTask, "connectTask", 5000, NULL, 1, NULL);
  pBLEScan->start(0);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  std::cout << "Client start time: " << duration.count() << " ms" << std::endl;

  printf("Client End\n");
  sendData("UART", "Client End");
}

void modelSetUp(void)
{
  printf("Model Start\n");
  sendData("UART", "Model Start");

  auto start_time = std::chrono::high_resolution_clock::now();

  // Mount SPIFFS
  esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
  };

  esp_err_t ret = esp_vfs_spiffs_register(&conf);
  if (ret != ESP_OK) {
      if (ret == ESP_FAIL) {
          printf("Failed to mount or format filesystem\n");
      } else if (ret == ESP_ERR_NOT_FOUND) {
          printf("Failed to find SPIFFS partition\n");
      } else {
          printf("Failed to initialize SPIFFS (%s)\n", esp_err_to_name(ret));
      }
      return;
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(NULL, &total, &used);
  if (ret != ESP_OK) {
      printf("Failed to get SPIFFS partition information (%s)\n", esp_err_to_name(ret));
  } else {
      printf("Partition size: total: %d, used: %d\n", total, used);
  }

  // Allocate and initialize the example_element array
  allocate_example_element();

  // Input
  int input_height = 224;
  int input_width = 224;
  int input_channel = 3;
  int input_exponent = -15;
  int total_elements = input_height * input_width * input_channel;
  int16_t *model_input = (int16_t *)dl::tool::malloc_aligned_prefer(total_elements, sizeof(int16_t));

  if (model_input == NULL) {
      printf("Failed to allocate memory for model input\n");
      return;
  }

  for (int i = 0; i < total_elements; i++) {
      float normalized_input = (example_element[i] / 127.5) - 1;
      model_input[i] = (int16_t)DL_CLIP(normalized_input * (1 << -input_exponent), -32768, 32767);
  }

  Tensor<int16_t> input;
  input.set_element((int16_t *)model_input).set_exponent(input_exponent).set_shape({224, 224, 3}).set_auto_free(false);

  MNIST model;

  dl::tool::Latency latency;

  // Model forward
  latency.start();
  //model.build(input);
  model.forward(input);
  latency.end();
  latency.print("MobileNetV2", "forward");

  // Parse
  Tensor<int16_t> &intermediates = model.l23.get_output();
  
  sizeOfTensor = intermediates.get_size();
  printf("Size of Tensor: %zu\n", sizeOfTensor);
  // my_tensor.print_all();
  nums_to_send = intermediates.all_list();

  // Free the allocated memory for example_element
  heap_caps_free(example_element);
  free(model_input);

  // Unmount SPIFFS
  esp_vfs_spiffs_unregister(NULL);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
  std::cout << "Time to train: " << duration.count() << " ms" << std::endl;

  printf("Model End\n");
  sendData("UART", "Model End");
}

void app_main(void)
{
  // CONFIGURE UART PINS
  initUART();

  // CONFIGURE SIGNAL PIN
  configure_gpio();
  gpio_set_level(SIGNAL_PIN, 0);

  // MODEL
  modelSetUp();

  // CLIENT
  clientSetUp();
}