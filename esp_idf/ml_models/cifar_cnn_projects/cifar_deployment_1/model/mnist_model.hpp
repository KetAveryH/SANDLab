#pragma once
#include "dl_layer_model.hpp"
#include "dl_layer_reshape.hpp"
#include "dl_layer_conv2d.hpp"
#include "mnist_coefficient.hpp"
#include <stdint.h>

using namespace dl;
using namespace layer;
using namespace mnist_coefficient;

class MNIST : public Model<int16_t> // Derive the Model class in "dl_layer_model.hpp"
{
private:
    // Declare layers as member variables
    Reshape<int16_t> l1;
    

public:
    Conv2D<int16_t> l2;

    /**
     * @brief Initialize layers in constructor function
     * 
     */

    MNIST() : l1(Reshape<int16_t>({1,1,784})),
              l2(Conv2D<int16_t>(-11, get_fused_gemm_0_filter(), get_fused_gemm_0_bias(), get_fused_gemm_0_activation(), PADDING_SAME_END, {}, 1, 1, "l1")){}
    /**
     * @brief call each layers' build(...) function in sequence
     * 
     * @param input 
     */
    void build(Tensor<int16_t> &input)
    {
        printf("Shape: (%d, %d, %d, %d)\n", input.shape[0], input.shape[1], input.shape[2], input.shape[3]);
        this->l1.build(input);
        printf("Shape: (%d, %d, %d, %d)\n", input.shape[0], input.shape[1], input.shape[2], input.shape[3]);
        this->l2.build(this->l1.get_output());
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
    }
};