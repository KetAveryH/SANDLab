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
    Conv2D<int16_t> l3;
    

public:
    Conv2D<int16_t> l4;

    /**
     * @brief Initialize layers in constructor function
     * 
     */

    MNIST() : l3(Conv2D<int16_t>(-11, get_fused_gemm_1_filter(), get_fused_gemm_1_bias(), get_fused_gemm_1_activation(), PADDING_SAME_END, {}, 1, 1, "l2")),
              l4(Conv2D<int16_t>(-9, get_fused_gemm_2_filter(), get_fused_gemm_2_bias(), NULL, PADDING_SAME_END, {}, 1, 1, "l3")){}
    /**
     * @brief call each layers' build(...) function in sequence
     * 
     * @param input 
     */
    void build(Tensor<int16_t> &input)
    {
        this->l3.build(input);
        this->l4.build(this->l3.get_output());
    }

    /**
     * @brief call each layers' call(...) function in sequence
     * 
     * @param input 
     */
    void call(Tensor<int16_t> &input)
    {
        this->l3.call(input);
        input.free_element();

        this->l4.call(this->l3.get_output());
        this->l3.get_output().free_element();
    }
};