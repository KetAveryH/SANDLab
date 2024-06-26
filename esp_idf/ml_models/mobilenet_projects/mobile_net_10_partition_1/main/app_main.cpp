#include <stdio.h>
#include <stdlib.h>

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "dl_tool.hpp"
#include "mnist_model.hpp"

__attribute__((aligned(16))) int16_t example_element[] = {};

extern "C" void app_main(void)
{
    // input

    int input_height = 244; //changed
    int input_width = 244; //changed
    int input_channel = 3; //changed
    int input_exponent = -15; // to change and below
    int16_t *model_input = (int16_t *)dl::tool::malloc_aligned_prefer(input_height*input_width*input_channel, sizeof(int16_t *));
    for(int i=0 ;i<input_height*input_width*input_channel; i++){
        float normalized_input = (example_element[i] / 127.5) - 1;
        model_input[i] = (int16_t)DL_CLIP(normalized_input * (1 << -input_exponent), -32768, 32767);
    } 

    Tensor<int16_t> input;
    input.set_element((int16_t *)model_input).set_exponent(input_exponent).set_shape({224, 224, 3}).set_auto_free(false);

    MNIST model;

    dl::tool::Latency latency;

    // model forward
    latency.start();
    // model.build(input)
    model.forward(input);
    latency.end();
    latency.print("MNIST", "forward");

    // parse
    model.l23.get_output().print_all();
    // esp32s3
    // MNIST::forward: 7243 us
    // -5523, 5300, 756, -4854, -4545, -3133, -1132, -1813, -2432, -7306, 
    // Prediction Result: 1
}