#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spiffs.h"
#include "dl_tool.hpp"
#include "mnist_model.hpp"
#include "church.h"

extern "C" void app_main(void) {
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
    model.build(input);
    model.forward(input);
    latency.end();
    latency.print("MobileNetV2", "forward");

    // Parse
    Tensor<int16_t> &intermediates = model.l23.get_output();
    // intermediates.print_all();
    int *list = intermediates.all_list();

    // Free the allocated memory for example_element
    heap_caps_free(example_element);
    free(model_input);

    // Unmount SPIFFS
    esp_vfs_spiffs_unregister(NULL);
}
