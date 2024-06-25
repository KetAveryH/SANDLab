#pragma once
#include "dl_layer_model.hpp"
#include "dl_layer_reshape.hpp"
#include "dl_layer_conv2d.hpp"
#include "dl_layer_max_pool2d.hpp"
#include "dl_layer_flatten.hpp"
#include "dl_layer_fullyconnected.hpp"
#include "dl_layer_transpose.hpp"
#include "mnist_coefficient.hpp"
#include <stdint.h>
#include <iostream>

using namespace dl;
using namespace layer;
using namespace mnist_coefficient;

/***
Quantized model info:
model input name: input, exponent: -15

Transpose layer name: sequential/conv2d/BiasAdd__6, output_exponent: -15

Conv layer name: sequential/conv2d/BiasAdd, output_exponent: -14

MaxPool layer name: sequential/max_pooling2d/MaxPool, output_exponent: -14

Conv layer name: sequential/conv2d_1/BiasAdd, output_exponent: -13

MaxPool layer name: sequential/max_pooling2d_1/MaxPool, output_exponent: -13

Conv layer name: sequential/conv2d_2/BiasAdd, output_exponent: -12

Transpose layer name: sequential/conv2d_2/BiasAdd__24, output_exponent: -12

Reshape layer name: sequential/flatten/Reshape, output_exponent: -12

Gemm layer name: fused_gemm_0, output_exponent: -11

Gemm layer name: fused_gemm_1, output_exponent: -10
 * 
*/

class MNIST : public Model<int16_t> // Derive the Model class in "dl_layer_model.hpp"
{
private:

    Conv2D<int16_t> l1;
    MaxPool2D<int16_t> l2;
    Conv2D<int16_t> l3;
    MaxPool2D<int16_t> l4;
    Conv2D<int16_t> l5;

    Flatten<int16_t> l6;

    FullyConnected<int16_t> l7;


public:
    
    FullyConnected<int16_t> l8;

    /**
     * @brief Initialize layers in constructor function
     * 
     */

    // Unsure about MaxPool "l2" connections t o previous layer as a parameter.

    MNIST() : l1(Conv2D<int16_t>(-15, get_sequential_conv2d_biasadd_filter(), get_sequential_conv2d_biasadd_bias(), get_sequential_conv2d_biasadd_activation(), PADDING_VALID, {}, 1, 1)),
              l2(MaxPool2D<int16_t>({2, 2}, PADDING_VALID, {}, 2, 2, "l1")), 
              l3(Conv2D<int16_t>(-13, get_sequential_conv2d_1_biasadd_filter(), get_sequential_conv2d_1_biasadd_bias(), get_sequential_conv2d_1_biasadd_activation(), PADDING_VALID, {}, 1, 1, "l2")),
              l4(MaxPool2D<int16_t>({2,2}, PADDING_VALID, {}, 2, 2, "l3")), 
              l5(Conv2D<int16_t>(-12, get_sequential_conv2d_2_biasadd_filter(), get_sequential_conv2d_2_biasadd_bias(), get_sequential_conv2d_2_biasadd_activation(), PADDING_VALID, {}, 1, 1, "l4")),
              l6(Flatten<int16_t>("l5", false)),  
              l7(FullyConnected<int16_t>(-11, get_fused_gemm_0_filter(), get_fused_gemm_0_bias(), get_fused_gemm_0_activation(), true, "l6")),
              l8(FullyConnected<int16_t>(-10, get_fused_gemm_1_filter(), get_fused_gemm_1_bias(), NULL, true, "l7")){}
    
    /**
     * @brief call each layers' build(...) function in sequence
     * 
     * @param input 
     */
       void build(Tensor<int16_t> &input)
    {
        std::vector<int> shape = input.shape;
        std::cout << "Input shape: (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l1.build(input);
        shape = this->l1.get_output().shape;
        std::cout << "After l1 (Transpose): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l2.build(this->l1.get_output());
        shape = this->l2.get_output().shape;
        std::cout << "After l2 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l3.build(this->l2.get_output());
        shape = this->l3.get_output().shape;
        std::cout << "After l3 (MaxPool2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l4.build(this->l3.get_output());
        shape = this->l4.get_output().shape;
        std::cout << "After l4 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l5.build(this->l4.get_output());
        shape = this->l5.get_output().shape;
        std::cout << "After l5 (MaxPool2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l6.build(this->l5.get_output());
        shape = this->l6.get_output().shape;
        std::cout << "After l6 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l7.build(this->l6.get_output());
        shape = this->l7.get_output().shape;
        std::cout << "After l7 (Transpose): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        this->l8.build(this->l7.get_output());
        shape = this->l8.get_output().shape;
        std::cout << "After l8 (Flatten): (" << shape[0] << ", " << shape[1] << ")\n";

        // this->l9.build(this->l8.get_output());
        // shape = this->l9.get_output().shape;
        // std::cout << "After l9 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

        // this->l10.build(this->l9.get_output());
        // shape = this->l10.get_output().shape;
        // std::cout << "After l10 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";
    }

    /**
     * @brief call each layers' call(...) function in sequence
     * 
     * @param input 
     */
    void call(Tensor<int16_t> &input)
    {
        this->l1.call(input);
        input.free_element();

        this->l2.call(this->l1.get_output());
        this->l1.get_output().free_element();

        this->l3.call(this->l2.get_output());
        this->l2.get_output().free_element();

        this->l4.call(this->l3.get_output());
        this->l3.get_output().free_element();

        this->l5.call(this->l4.get_output());
        this->l4.get_output().free_element();

        this->l6.call(this->l5.get_output());
        this->l5.get_output().free_element();

        this->l7.call(this->l6.get_output());
        this->l6.get_output().free_element();

        this->l8.call(this->l7.get_output());
        this->l7.get_output().free_element();

        // this->l9.call(this->l8.get_output());
        // this->l8.get_output().free_element();

        // this->l10.call(this->l9.get_output());
        // this->l9.get_output().free_element();
    }
};