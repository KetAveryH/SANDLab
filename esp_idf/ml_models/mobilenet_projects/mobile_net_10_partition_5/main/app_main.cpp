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

    int input_height = 7;
    int input_width = 7;
    int input_channel = 960;
    int input_exponent = -8;
    int16_t *model_input = (int16_t *)dl::tool::malloc_aligned_prefer(input_height*input_width*input_channel, sizeof(int16_t *));
    for(int i=0 ;i<input_height*input_width*input_channel; i++){
        float normalized_input = example_element[i];
        model_input[i] = (int16_t)DL_CLIP(normalized_input * (1 << -input_exponent), -32768, 32767);
    } 

    Tensor<int16_t> input;
    input.set_element((int16_t *)model_input).set_exponent(input_exponent).set_shape({7, 7, 960}).set_auto_free(false);

    MNIST model;

    dl::tool::Latency latency;

    // Model build
    latency.start();
    model.build(input);  // build method likely returns void
    latency.end();
    latency.print("MobileNetV2", "build");

    // Model forward
    latency.start();
    model.forward(input);  // forward method likely returns void
    latency.end();
    latency.print("MobileNetV2", "forward");
    // model.l67.get_output().print_all();
    free(model_input);

    // parse
        // parse
    int16_t *score = model.l67.get_output().get_element_ptr(); // assuming l10 is the final layer
    int16_t max_score = score[0];
    int max_index = 0;
    printf("%d, ", max_score);

    for (size_t i = 1; i < 10; i++)
    {
        printf("%d, ", score[i]);
        if (score[i] > max_score)
        {
            max_score = score[i];
            max_index = i;
        }
    }
    printf("\nPrediction Result: %d\n", max_index);


    // esp32s3
    // MNIST::forward: 7243 us
    // -5523, 5300, 756, -4854, -4545, -3133, -1132, -1813, -2432, -7306, 
    // Prediction Result: 1
}

