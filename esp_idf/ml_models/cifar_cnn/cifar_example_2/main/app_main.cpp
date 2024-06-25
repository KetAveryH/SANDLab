#include <stdio.h>
#include <stdlib.h>

#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h" // Include the ESP log library
#include "dl_tool.hpp"
#include "mnist_model.hpp"

static const char* TAG = "TensorShape"; // Define the TAG variable

__attribute__((aligned(16))) int16_t example_element[] = {

    1234, 0, 18, 0, 0, 436, 0, 0, 0, 0, 589, 0, 278, 0, 0, 0, 0, 0, 0, 0, 61, 0, 0, 3342, 1033, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1979, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 264, 0, 340, 0, 0, 0, 0, 713, 0, 0, 2216, 0, 0, 0, 0, 1326, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 251, 0, 0, 0, 0, 92, 653, 0, 0, 0, 1633, 0, 0, 27, 0, 0, 0, 0, 0, 728, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1060, 0, 325, 0, 0, 0, 0, 1432, 0, 3106, 0, 0, 301, 295, 0, 0, 0, 0, 0, 0, 226, 0, 0, 0, 0, 0, 0, 0, 513, 145, 0, 0, 0, 2020, 807, 0, 0, 0, 0, 0, 0, 1358, 1946, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1708, 0, 3011, 0, 0, 0, 0, 2039, 0, 395, 1486, 573, 0, 0, 0, 0, 0, 0, 0, 125, 0, 222, 1035, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2360, 0, 0, 0, 0, 0, 0, 0, 360, 1662, 0, 0, 0, 0, 0, 0, 0, 0, 0, 412, 2148, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1950, 0, 409, 0, 0, 0, 0, 1664, 0, 0, 0, 1616, 863, 1059, 0, 0, 0, 0, 0, 0, 2642, 0, 0, 0, 0, 0, 0, 0, 0, 0, 158, 0, 0, 2290, 252, 0, 0, 0, 0, 0, 2561, 0, 0, 0, 0, 0, 0, 1049, 0, 0, 0, 0, 0, 0, 0, 207, 0, 0, 0, 0, 667, 354, 0, 0, 3236, 0, 0, 4284, 0, 0, 0, 558, 3299, 0, 2471, 3926, 0, 998, 320, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 428, 0, 0, 0, 135, 1499, 0, 0, 0, 0, 0, 0, 970, 672, 0, 0, 20, 0, 792, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 119, 0, 0, 2729, 0, 0, 0, 0, 0, 0, 2241, 3214, 2462, 0, 1538, 0, 1208, 0, 0, 0, 0, 0, 0, 911, 0, 0, 0, 0, 0, 0, 493, 1072, 0, 0, 0, 0, 647, 296, 0, 0, 0, 0, 0, 0, 1874, 0, 0, 0, 0, 0, 867, 0, 0, 487, 0, 0, 415, 0, 0, 0, 0, 0, 0, 167, 0, 0, 0, 1913, 0, 301, 1655, 0, 0, 0, 3776, 2551, 0, 12, 2465, 272, 0, 0, 0, 0, 0, 0, 827, 0, 0, 0, 0, 0, 0, 0, 0, 115, 0, 0, 0, 0, 394, 308, 0, 0, 610, 0, 0, 1224, 0, 1313, 0, 0, 0, 0, 46, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1738, 0, 0, 0, 0, 0, 0, 1826, 2314, 0, 1185, 0, 167, 256, 0, 1806, 0, 0, 0, 0, 333, 0, 0, 0, 0, 0, 0, 0, 3751, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 280, 0, 0, 0, 0, 0, 0, 0, 469, 0, 0, 1376, 0, 0, 2362, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1719, 0, 0, 1575, 338, 0, 749, 0, 2624, 0, 0, 0, 0, 0, 0, 0, 0, 417, 0, 0, 0, 2878, 0, 190, 0, 0, 609, 0, 0, 0, 0, 2659, 0, 0, 1016, 0, 0, 0, 0, 0, 648, 0, 0, 0, 0, 0, 1565, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 522, 0, 882, 127, 252, 0, 0, 471, 530, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1853, 0, 0, 0, 0, 856, 1431, 0, 0, 0, 0, 0, 0, 1745, 0, 0, 0, 0, 0, 0, 0, 0, 646, 0, 0, 1014, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 280, 0, 227, 0, 0, 0, 0, 2294, 0, 1158, 1847, 0, 0, 0, 0, 1369, 0, 0, 0, 0, 0, 0, 599, 0, 0, 0, 0, 0, 2688, 0, 0, 0, 0, 1641, 2132, 0, 0, 0, 35, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 355, 0, 61, 3312, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1558, 0, 0, 0, 0, 1608, 0, 932, 1248, 0, 670, 801, 0, 2357, 0, 0, 0, 556, 115, 1296, 277, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 318, 0, 0, 0, 0, 0, 0, 0, 0, 1740, 0, 0, 0, 0, 1134, 0, 0, 0, 0, 0, 2375, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 789, 0, 726, 0, 0, 0, 0, 1353, 0, 0, 264, 969, 0, 0, 1542, 2397, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 70, 0, 0, 0, 326, 1340, 0, 0, 0, 1081, 0, 0, 0, 0, 0, 0, 0, 0, 855, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1372, 0, 385, 0, 0, 0, 0, 1353, 0, 2233, 0, 354, 159, 435, 1093, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 764, 343, 0, 0, 0, 0, 0, 128, 0, 0, 0, 0, 0, 0, 538, 0, 0, 0, 0, 0, 703, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2498, 0, 2895, 0, 0, 0, 0, 2541, 0, 560, 0, 151, 0, 0, 0, 0, 0, 0, 0, 903, 1021, 885, 944, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1397, 100, 0, 0, 0, 0, 0, 1889, 0, 51, 0, 0, 0, 0, 1153, 0, 0, 0, 0, 0, 1765, 0, 170, 0, 0, 0, 0, 382, 0, 0, 0, 1962, 0, 2636, 464, 0, 0, 0, 2559, 0, 0

};

extern "C" void app_main(void)
{
    // input

    int input_height = 1;
    int input_width = 1024;
    int input_channel = 1; // 3 channels for RGB
    int input_exponent = -15;

    int total_elements = input_height * input_width * input_channel;

    int16_t *model_input = (int16_t *)dl::tool::malloc_aligned_prefer(input_height*input_width*input_channel, sizeof(int16_t *));
    for(int i=0 ;i<input_height*input_width*input_channel; i++){
        float normalized_input = example_element[i];
        model_input[i] = (int16_t)DL_CLIP(normalized_input * (1 << -input_exponent), -32768, 32767);
    } 

    size_t model_input_size = total_elements * sizeof(int16_t);
    ESP_LOGI(TAG, "Size of model_input: %zu bytes", model_input_size);

    // Print the elements of model_input
    // printf("Model Input Elements:\n");
    // for (int i = 0; i < total_elements; i++) {
    //     printf("%d ", model_input[i]);
    // }
    // printf("\n");

    Tensor<int16_t> input;
    input.set_element((int16_t *)model_input).set_exponent(input_exponent).set_shape({1, 1024, 1}).set_auto_free(false); 

    // Verify input shape
    std::vector<int> shape = input.shape;
    std::cout << "Input shape: (";
    for (size_t i = 0; i < shape.size(); ++i) {
        std::cout << shape[i];
        if (i < shape.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")\n";

    MNIST model;

    dl::tool::Latency latency;

    // model forward
    latency.start();
    model.build(input);   // Uncomment if you need to build before forwarding
    
    model.forward(input);
    latency.end();
    latency.print("MNIST", "forward");

    // parse
    int16_t *score = model.l8.get_output().get_element_ptr(); // assuming l10 is the final layer
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