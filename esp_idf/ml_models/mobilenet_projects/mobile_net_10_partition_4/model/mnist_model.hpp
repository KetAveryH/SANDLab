#pragma once
#include "dl_layer_model.hpp"
#include "dl_layer_reshape.hpp"
#include "dl_layer_conv2d.hpp"
#include "dl_layer_depthwise_conv2d.hpp"
#include "dl_layer_add2d.hpp"
#include "dl_layer_avg_pool2d.hpp"
#include "dl_layer_squeeze.hpp"
#include "dl_layer_flatten.hpp"
#include "dl_layer_fullyconnected.hpp"
#include "dl_layer_global_avg_pool2d.hpp"
#include "mobilenetv2_coefficient_12.hpp"
#include <stdint.h>

using namespace dl;
using namespace layer;
using namespace mobilenetv2_coefficient_12;

class MNIST : public Model<int16_t> // Derive the Model class in "dl_layer_model.hpp"
{
private:
    // // Layer definitions
    // Conv2D<int16_t> l1;  // model_2/conv2d_72/Conv2D__6, output_exponent: -15
    // DepthwiseConv2D<int16_t> l2;  // model_2/depthwise_conv2d_34/depthwise, output_exponent: -10
    // Conv2D<int16_t> l3;  // model_2/conv2d_73/Conv2D, output_exponent: -11
    // Conv2D<int16_t> l4;  // model_2/conv2d_74/Conv2D, output_exponent: -11

    // DepthwiseConv2D<int16_t> l5;  // model_2/depthwise_conv2d_35/depthwise, output_exponent: -10
    // Conv2D<int16_t> l6;  // model_2/conv2d_75/Conv2D, output_exponent: -10
    // Conv2D<int16_t> l7;  // model_2/conv2d_76/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l8;  // model_2/depthwise_conv2d_36/depthwise, output_exponent: -10
    // Conv2D<int16_t> l9;  // model_2/conv2d_77/Conv2D, output_exponent: -11

    // Add2D<int16_t> l10;  // model_2/add_20/add, output_exponent: -10

    // Conv2D<int16_t> l11;  // model_2/conv2d_78/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l12;  // model_2/depthwise_conv2d_37/depthwise, output_exponent: -10
    // Conv2D<int16_t> l13;  // model_2/conv2d_79/Conv2D, output_exponent: -11
    // Conv2D<int16_t> l14;  // model_2/conv2d_80/Conv2D, output_exponent: -11
    // DepthwiseConv2D<int16_t> l15;  // model_2/depthwise_conv2d_38/depthwise, output_exponent: -10
    // Conv2D<int16_t> l16;  // model_2/conv2d_81/Conv2D, output_exponent: -11
    
    // Add2D<int16_t> l17;  // model_2/add_21/add, output_exponent: -10

    // Conv2D<int16_t> l18;  // model_2/conv2d_82/Conv2D, output_exponent: -11
    // DepthwiseConv2D<int16_t> l19;  // model_2/depthwise_conv2d_39/depthwise, output_exponent: -10
    // Conv2D<int16_t> l20;  // model_2/conv2d_83/Conv2D, output_exponent: -11

    // Add2D<int16_t> l21;  // model_2/add_22/add, output_exponent: -10

    // Conv2D<int16_t> l22;  // model_2/conv2d_84/Conv2D, output_exponent: -11
    // DepthwiseConv2D<int16_t> l23;  // model_2/depthwise_conv2d_40/depthwise, output_exponent: -10
    // Conv2D<int16_t> l24;  // model_2/conv2d_85/Conv2D, output_exponent: -11
    // Conv2D<int16_t> l25;  // model_2/conv2d_86/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l26;  // model_2/depthwise_conv2d_41/depthwise, output_exponent: -10
    // Conv2D<int16_t> l27;  // model_2/conv2d_87/Conv2D, output_exponent: -10

    // Add2D<int16_t> l28;  // model_2/add_23/add, output_exponent: -10

    // Conv2D<int16_t> l29;  // model_2/conv2d_88/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l30;  // model_2/depthwise_conv2d_42/depthwise, output_exponent: -10
    // Conv2D<int16_t> l31;  // model_2/conv2d_89/Conv2D, output_exponent: -10

    // Add2D<int16_t> l32;  // model_2/add_24/add, output_exponent: -9

    // Conv2D<int16_t> l33;  // model_2/conv2d_90/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l34;  // model_2/depthwise_conv2d_43/depthwise, output_exponent: -9
    // Conv2D<int16_t> l35;  // model_2/conv2d_91/Conv2D, output_exponent: -9

    // Add2D<int16_t> l36;  // model_2/add_25/add, output_exponent: -9
    
    // Conv2D<int16_t> l37;  // model_2/conv2d_92/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l38;  // model_2/depthwise_conv2d_44/depthwise, output_exponent: -10
    // Conv2D<int16_t> l39;  // model_2/conv2d_93/Conv2D, output_exponent: -10
    // Conv2D<int16_t> l40;  // model_2/conv2d_94/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l41;  // model_2/depthwise_conv2d_45/depthwise, output_exponent: -9
    // Conv2D<int16_t> l42;  // model_2/conv2d_95/Conv2D, output_exponent: -9
    
    // Add2D<int16_t> l43;  // model_2/add_26/add, output_exponent: -9

    // Conv2D<int16_t> l44;  // model_2/conv2d_96/Conv2D, output_exponent: -10
    // DepthwiseConv2D<int16_t> l45;  // model_2/depthwise_conv2d_46/depthwise, output_exponent: -9
    // Conv2D<int16_t> l46;  // model_2/conv2d_97/Conv2D, output_exponent: -9

    // Add2D<int16_t> l47;  // model_2/add_27/add, output_exponent: -8
    
    // Conv2D<int16_t> l48;  // model_2/conv2d_98/Conv2D, output_exponent: -9
    // DepthwiseConv2D<int16_t> l49;  // model_2/depthwise_conv2d_47/depthwise, output_exponent: -9
    Conv2D<int16_t> l50;  // model_2/conv2d_99/Conv2D, output_exponent: -10
    Conv2D<int16_t> l51;  // model_2/conv2d_100/Conv2D, output_exponent: -9
    DepthwiseConv2D<int16_t> l52;  // model_2/depthwise_conv2d_48/depthwise, output_exponent: -9
    Conv2D<int16_t> l53;  // model_2/conv2d_101/Conv2D, output_exponent: -9
    
    Add2D<int16_t> l54;  // model_2/add_28/add, output_exponent: -9

    Conv2D<int16_t> l55;  // model_2/conv2d_102/Conv2D, output_exponent: -9
    DepthwiseConv2D<int16_t> l56;  // model_2/depthwise_conv2d_49/depthwise, output_exponent: -9
    Conv2D<int16_t> l57;  // model_2/conv2d_103/Conv2D, output_exponent: -9

    Add2D<int16_t> l58;  // model_2/add_29/add, output_exponent: -8

    // Conv2D<int16_t> l59;  // model_2/conv2d_104/Conv2D, output_exponent: -9
    // DepthwiseConv2D<int16_t> l60;  // model_2/depthwise_conv2d_50/depthwise, output_exponent: -8
    // Conv2D<int16_t> l61;  // model_2/conv2d_105/Conv2D, output_exponent: -8
    // Conv2D<int16_t> l62;  // model_2/conv2d_106/Conv2D, output_exponent: -8

    // GlobalAveragePool2D<int16_t> l63;  // model_2/global_average_pooling2d_2/Mean, output_exponent: -8
    // Reshape<int16_t> l64;  // model_2/global_average_pooling2d_2/Mean_Squeeze__559, output_exponent: -8
    // Conv2D<int16_t> l65;  // model_2/conv2d_107/BiasAdd, output_exponent: -9

    // Flatten<int16_t> l66;  // model_2/flatten_2/Reshape, output_exponent: -9
    





    




public:
    Conv2D<int16_t> l59;  // fused_gemm_0, output_exponent: -10

    /**
     * @brief Initialize layers in constructor function
     * 
     */

    MNIST() :
        // l1(Conv2D<int16_t>(-15, get_model_2_conv2d_72_conv2d_filter(), get_model_2_conv2d_72_conv2d_bias(), get_model_2_conv2d_72_conv2d_activation())),
        // l2(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_34_depthwise_filter(), get_model_2_depthwise_conv2d_34_depthwise_bias(), get_model_2_depthwise_conv2d_34_depthwise_activation())),
        // l3(Conv2D<int16_t>(-11, get_model_2_conv2d_73_conv2d_filter(), get_model_2_conv2d_73_conv2d_bias())),
        // l4(Conv2D<int16_t>(-11, get_model_2_conv2d_74_conv2d_filter(), get_model_2_conv2d_74_conv2d_bias(), get_model_2_conv2d_74_conv2d_activation())),

        // l5(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_35_depthwise_filter(), get_model_2_depthwise_conv2d_35_depthwise_bias(), get_model_2_depthwise_conv2d_35_depthwise_activation())),
        // l6(Conv2D<int16_t>(-10, get_model_2_conv2d_75_conv2d_filter(), get_model_2_conv2d_75_conv2d_bias())),
        // l7(Conv2D<int16_t>(-10, get_model_2_conv2d_76_conv2d_filter(), get_model_2_conv2d_76_conv2d_bias(), get_model_2_conv2d_76_conv2d_activation())),
        // l8(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_36_depthwise_filter(), get_model_2_depthwise_conv2d_36_depthwise_bias(), get_model_2_depthwise_conv2d_36_depthwise_activation())),
        // l9(Conv2D<int16_t>(-11, get_model_2_conv2d_77_conv2d_filter(), get_model_2_conv2d_77_conv2d_bias())),

        // l10(Add2D<int16_t>(-10, NULL)),

        // l11(Conv2D<int16_t>(-10, get_model_2_conv2d_78_conv2d_filter(), get_model_2_conv2d_78_conv2d_bias(), get_model_2_conv2d_78_conv2d_activation())),
        // l12(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_37_depthwise_filter(), get_model_2_depthwise_conv2d_37_depthwise_bias(), get_model_2_depthwise_conv2d_37_depthwise_activation())),
        // l13(Conv2D<int16_t>(-11, get_model_2_conv2d_79_conv2d_filter(), get_model_2_conv2d_79_conv2d_bias())),
        // l14(Conv2D<int16_t>(-11, get_model_2_conv2d_80_conv2d_filter(), get_model_2_conv2d_80_conv2d_bias(), get_model_2_conv2d_80_conv2d_activation())),
        // l15(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_38_depthwise_filter(), get_model_2_depthwise_conv2d_38_depthwise_bias(), get_model_2_depthwise_conv2d_38_depthwise_activation())),
        // l16(Conv2D<int16_t>(-11, get_model_2_conv2d_81_conv2d_filter(), get_model_2_conv2d_81_conv2d_bias())),
        
        // l17(Add2D<int16_t>(-10, NULL)),

        // l18(Conv2D<int16_t>(-11, get_model_2_conv2d_82_conv2d_filter(), get_model_2_conv2d_82_conv2d_bias(), get_model_2_conv2d_82_conv2d_activation())),
        // l19(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_39_depthwise_filter(), get_model_2_depthwise_conv2d_39_depthwise_bias(), get_model_2_depthwise_conv2d_39_depthwise_activation())),
        // l20(Conv2D<int16_t>(-11, get_model_2_conv2d_83_conv2d_filter(), get_model_2_conv2d_83_conv2d_bias())),

        // l21(Add2D<int16_t>(-10, NULL)),

        // l22(Conv2D<int16_t>(-11, get_model_2_conv2d_84_conv2d_filter(), get_model_2_conv2d_84_conv2d_bias(), get_model_2_conv2d_84_conv2d_activation())),
        // l23(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_40_depthwise_filter(), get_model_2_depthwise_conv2d_40_depthwise_bias(), get_model_2_depthwise_conv2d_40_depthwise_activation())),
        // l24(Conv2D<int16_t>(-11, get_model_2_conv2d_85_conv2d_filter(), get_model_2_conv2d_85_conv2d_bias())),
        // l25(Conv2D<int16_t>(-10, get_model_2_conv2d_86_conv2d_filter(), get_model_2_conv2d_86_conv2d_bias(), get_model_2_conv2d_86_conv2d_activation())),
        // l26(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_41_depthwise_filter(), get_model_2_depthwise_conv2d_41_depthwise_bias(), get_model_2_depthwise_conv2d_41_depthwise_activation())),
        // l27(Conv2D<int16_t>(-10, get_model_2_conv2d_87_conv2d_filter(), get_model_2_conv2d_87_conv2d_bias())),

        // l28(Add2D<int16_t>(-10, NULL)),

        // l29(Conv2D<int16_t>(-10, get_model_2_conv2d_88_conv2d_filter(), get_model_2_conv2d_88_conv2d_bias(), get_model_2_conv2d_88_conv2d_activation())),
        // l30(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_42_depthwise_filter(), get_model_2_depthwise_conv2d_42_depthwise_bias(), get_model_2_depthwise_conv2d_42_depthwise_activation())),
        // l31(Conv2D<int16_t>(-10, get_model_2_conv2d_89_conv2d_filter(), get_model_2_conv2d_89_conv2d_bias())),

        // l32(Add2D<int16_t>(-9, NULL)),

        // l33(Conv2D<int16_t>(-10, get_model_2_conv2d_90_conv2d_filter(), get_model_2_conv2d_90_conv2d_bias(), get_model_2_conv2d_90_conv2d_activation())),
        // l34(DepthwiseConv2D<int16_t>(-9, get_model_2_depthwise_conv2d_43_depthwise_filter(), get_model_2_depthwise_conv2d_43_depthwise_bias(), get_model_2_depthwise_conv2d_43_depthwise_activation())),
        // l35(Conv2D<int16_t>(-9, get_model_2_conv2d_91_conv2d_filter(), get_model_2_conv2d_91_conv2d_bias())),

        // l36(Add2D<int16_t>(-9, NULL)),
        
        // l37(Conv2D<int16_t>(-10, get_model_2_conv2d_92_conv2d_filter(), get_model_2_conv2d_92_conv2d_bias(), get_model_2_conv2d_92_conv2d_activation())),
        // l38(DepthwiseConv2D<int16_t>(-10, get_model_2_depthwise_conv2d_44_depthwise_filter(), get_model_2_depthwise_conv2d_44_depthwise_bias(), get_model_2_depthwise_conv2d_44_depthwise_activation())),
        // l39(Conv2D<int16_t>(-10, get_model_2_conv2d_93_conv2d_filter(), get_model_2_conv2d_93_conv2d_bias())),
        // l40(Conv2D<int16_t>(-10, get_model_2_conv2d_94_conv2d_filter(), get_model_2_conv2d_94_conv2d_bias(), get_model_2_conv2d_94_conv2d_activation())),
        // l41(DepthwiseConv2D<int16_t>(-9, get_model_2_depthwise_conv2d_45_depthwise_filter(), get_model_2_depthwise_conv2d_45_depthwise_bias(), get_model_2_depthwise_conv2d_45_depthwise_activation())),
        // l42(Conv2D<int16_t>(-9, get_model_2_conv2d_95_conv2d_filter(), get_model_2_conv2d_95_conv2d_bias())),
        
        // l43(Add2D<int16_t>(-9, NULL)),

        // l44(Conv2D<int16_t>(-10, get_model_2_conv2d_96_conv2d_filter(), get_model_2_conv2d_96_conv2d_bias(), get_model_2_conv2d_96_conv2d_activation())),
        // l45(DepthwiseConv2D<int16_t>(-9, get_model_2_depthwise_conv2d_46_depthwise_filter(), get_model_2_depthwise_conv2d_46_depthwise_bias(), get_model_2_depthwise_conv2d_46_depthwise_activation())),
        // l46(Conv2D<int16_t>(-9, get_model_2_conv2d_97_conv2d_filter(), get_model_2_conv2d_97_conv2d_bias())),

        // l47(Add2D<int16_t>(-8, NULL)),
        
        // l48(Conv2D<int16_t>(-9, get_model_2_conv2d_98_conv2d_filter(), get_model_2_conv2d_98_conv2d_bias(), get_model_2_conv2d_98_conv2d_activation())),
        // l49(DepthwiseConv2D<int16_t>(-9, get_model_2_depthwise_conv2d_47_depthwise_filter(), get_model_2_depthwise_conv2d_47_depthwise_bias(), get_model_2_depthwise_conv2d_47_depthwise_activation())),
        l50(Conv2D<int16_t>(-10, get_model_2_conv2d_99_conv2d_filter(), get_model_2_conv2d_99_conv2d_bias(), NULL, PADDING_SAME_END, {0,0,0,0}, 1,1)),
        l51(Conv2D<int16_t>(-9, get_model_2_conv2d_100_conv2d_filter(), get_model_2_conv2d_100_conv2d_bias(), get_model_2_conv2d_100_conv2d_activation(), PADDING_SAME_END, {0,0,0,0}, 1,1)),
        l52(DepthwiseConv2D<int16_t>(-9, get_model_2_depthwise_conv2d_48_depthwise_filter(), get_model_2_depthwise_conv2d_48_depthwise_bias(), get_model_2_depthwise_conv2d_48_depthwise_activation(), PADDING_SAME_END, {1,1,1,1}, 1,1)),
        l53(Conv2D<int16_t>(-9, get_model_2_conv2d_101_conv2d_filter(), get_model_2_conv2d_101_conv2d_bias(), NULL,PADDING_SAME_END, {0,0,0,0}, 1,1)),
        
        l54(Add2D<int16_t>(-9, NULL)),

        l55(Conv2D<int16_t>(-9, get_model_2_conv2d_102_conv2d_filter(), get_model_2_conv2d_102_conv2d_bias(), get_model_2_conv2d_102_conv2d_activation(), PADDING_SAME_END, {0,0,0,0}, 1,1)),
        l56(DepthwiseConv2D<int16_t>(-9, get_model_2_depthwise_conv2d_49_depthwise_filter(), get_model_2_depthwise_conv2d_49_depthwise_bias(), get_model_2_depthwise_conv2d_49_depthwise_activation(), PADDING_SAME_END, {1,1,1,1}, 1,1)),
        l57(Conv2D<int16_t>(-9, get_model_2_conv2d_103_conv2d_filter(), get_model_2_conv2d_103_conv2d_bias(), NULL ,PADDING_SAME_END, {0,0,0,0}, 1,1)),

        l58(Add2D<int16_t>(-8, NULL)),

        l59(Conv2D<int16_t>(-9, get_model_2_conv2d_104_conv2d_filter(), get_model_2_conv2d_104_conv2d_bias(), get_model_2_conv2d_104_conv2d_activation(), PADDING_SAME_END, {0,0,0,0}, 1,1)){}
        // l60(DepthwiseConv2D<int16_t>(-8, get_model_2_depthwise_conv2d_50_depthwise_filter(), get_model_2_depthwise_conv2d_50_depthwise_bias(), get_model_2_depthwise_conv2d_50_depthwise_activation(), PADDING_SAME_END, {1,1,1,1}, 1,1)),
        // l61(Conv2D<int16_t>(-8, get_model_2_conv2d_105_conv2d_filter(), get_model_2_conv2d_105_conv2d_bias(), NULL, PADDING_SAME_END, {0,0,0,0}, 1,1)),
        // l62(Conv2D<int16_t>(-8, get_model_2_conv2d_106_conv2d_filter(), get_model_2_conv2d_106_conv2d_bias(), get_model_2_conv2d_106_conv2d_activation(), PADDING_SAME_END, {} ,1,1)),

        // l63(GlobalAveragePool2D<int16_t>(-8)),  // GlobalAveragePool
        // l64(Reshape<int16_t>({1, 1, 1280})),  // Squeeze  // Squeeze: remove all singleton dimensions
        // l65(Conv2D<int16_t>(-9, get_model_2_conv2d_107_biasadd_filter(), get_model_2_conv2d_107_biasadd_bias(), NULL, PADDING_SAME_END, {0,0,0,0}, 1,1)),
        // l66(Flatten<int16_t>()),  // Flatten

        // l67(FullyConnected<int16_t>(-10, get_fused_gemm_0_filter(), get_fused_gemm_0_bias(), NULL))   // Fully Connected
    

/**
 * @brief call each layers' build(...) function in sequence
 * 
 * @param input 
 */
void build(Tensor<int16_t> &input)
{
    std::vector<int> shape = input.shape;
    std::cout << "Input shape: (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";



    this->l50.build(input);
    shape = this->l50.get_output().shape;
    std::cout << "After l50 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l2.build(this->l1.get_output());
    // shape = this->l2.get_output().shape;
    // std::cout << "After l2 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l3.build(this->l2.get_output());
    // shape = this->l3.get_output().shape;
    // std::cout << "After l3 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l4.build(this->l3.get_output());
    // shape = this->l4.get_output().shape;
    // std::cout << "After l4 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l5.build(this->l4.get_output());
    // shape = this->l5.get_output().shape;
    // std::cout << "After l5 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l6.build(this->l5.get_output());
    // shape = this->l6.get_output().shape;
    // std::cout << "After l6 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l7.build(this->l6.get_output());
    // shape = this->l7.get_output().shape;
    // std::cout << "After l7 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l8.build(this->l7.get_output());
    // shape = this->l8.get_output().shape;
    // std::cout << "After l8 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l9.build(this->l8.get_output());
    // shape = this->l9.get_output().shape;
    // std::cout << "After l9 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l10.build(this->l9.get_output(), this->l6.get_output());
    // shape = this->l10.get_output().shape;
    // std::cout << "After l10 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l11.build(this->l10.get_output());
    // shape = this->l11.get_output().shape;
    // std::cout << "After l11 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l12.build(this->l11.get_output());
    // shape = this->l12.get_output().shape;
    // std::cout << "After l12 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l13.build(this->l12.get_output());
    // shape = this->l13.get_output().shape;
    // std::cout << "After l13 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l14.build(this->l13.get_output());
    // shape = this->l14.get_output().shape;
    // std::cout << "After l14 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l15.build(this->l14.get_output());
    // shape = this->l15.get_output().shape;
    // std::cout << "After l15 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l16.build(this->l15.get_output());
    // shape = this->l16.get_output().shape;
    // std::cout << "After l16 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l17.build(this->l16.get_output(), this->l13.get_output());
    // shape = this->l17.get_output().shape;
    // std::cout << "After l17 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l18.build(this->l17.get_output());
    // shape = this->l18.get_output().shape;
    // std::cout << "After l18 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l19.build(this->l18.get_output());
    // shape = this->l19.get_output().shape;
    // std::cout << "After l19 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l20.build(this->l19.get_output());
    // shape = this->l20.get_output().shape;
    // std::cout << "After l20 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l21.build(this->l20.get_output(), this->l17.get_output());
    // shape = this->l21.get_output().shape;
    // std::cout << "After l21 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l22.build(this->l21.get_output());
    // shape = this->l22.get_output().shape;
    // std::cout << "After l22 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l23.build(this->l22.get_output());
    // shape = this->l23.get_output().shape;
    // std::cout << "After l23 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l24.build(this->l23.get_output());
    // shape = this->l24.get_output().shape;
    // std::cout << "After l24 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l25.build(this->l24.get_output());
    // shape = this->l25.get_output().shape;
    // std::cout << "After l25 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l26.build(this->l25.get_output());
    // shape = this->l26.get_output().shape;
    // std::cout << "After l26 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l27.build(this->l26.get_output());
    // shape = this->l27.get_output().shape;
    // std::cout << "After l27 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l28.build(this->l27.get_output(), this->l24.get_output());
    // shape = this->l28.get_output().shape;
    // std::cout << "After l28 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l29.build(this->l28.get_output());
    // shape = this->l29.get_output().shape;
    // std::cout << "After l29 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l30.build(this->l29.get_output());
    // shape = this->l30.get_output().shape;
    // std::cout << "After l30 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l31.build(this->l30.get_output());
    // shape = this->l31.get_output().shape;
    // std::cout << "After l31 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l32.build(this->l31.get_output(), this->l28.get_output());
    // shape = this->l32.get_output().shape;
    // std::cout << "After l32 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l33.build(this->l32.get_output());
    // shape = this->l33.get_output().shape;
    // std::cout << "After l33 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l34.build(this->l33.get_output());
    // shape = this->l34.get_output().shape;
    // std::cout << "After l34 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l35.build(this->l34.get_output());
    // shape = this->l35.get_output().shape;
    // std::cout << "After l35 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l36.build(this->l35.get_output(), this->l32.get_output()); ////
    // shape = this->l36.get_output().shape;
    // std::cout << "After l36 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l37.build(this->l36.get_output());
    // shape = this->l37.get_output().shape;
    // std::cout << "After l37 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l38.build(this->l37.get_output());
    // shape = this->l38.get_output().shape;
    // std::cout << "After l38 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l39.build(this->l38.get_output());
    // shape = this->l39.get_output().shape;
    // std::cout << "After l39 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l40.build(this->l39.get_output());
    // shape = this->l40.get_output().shape;
    // std::cout << "After l40 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l41.build(this->l40.get_output());
    // shape = this->l41.get_output().shape;
    // std::cout << "After l41 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l42.build(this->l41.get_output());
    // shape = this->l42.get_output().shape;
    // std::cout << "After l42 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l43.build(this->l42.get_output(), this->l3.get_output());
    // shape = this->l43.get_output().shape;
    // std::cout << "After l43 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l44.build(this->l43.get_output());
    // shape = this->l44.get_output().shape;
    // std::cout << "After l44 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l45.build(this->l44.get_output());
    // shape = this->l45.get_output().shape;
    // std::cout << "After l45 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l46.build(this->l45.get_output());
    // shape = this->l46.get_output().shape;
    // std::cout << "After l46 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l47.build(this->l46.get_output(), this->l43.get_output());
    // shape = this->l47.get_output().shape;
    // std::cout << "After l47 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l48.build(this->l47.get_output());
    // shape = this->l48.get_output().shape;
    // std::cout << "After l48 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l49.build(this->l48.get_output());
    // shape = this->l49.get_output().shape;
    // std::cout << "After l49 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l50.build(this->l49.get_output());
    // shape = this->l50.get_output().shape;
    // std::cout << "After l50 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l51.build(this->l50.get_output());
    shape = this->l51.get_output().shape;
    std::cout << "After l51 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l52.build(this->l51.get_output());
    shape = this->l52.get_output().shape;
    std::cout << "After l52 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l53.build(this->l52.get_output());
    shape = this->l53.get_output().shape;
    std::cout << "After l53 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l54.build(this->l53.get_output(), this->l50.get_output());
    shape = this->l54.get_output().shape;
    std::cout << "After l54 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l55.build(this->l54.get_output());
    shape = this->l55.get_output().shape;
    std::cout << "After l55 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l56.build(this->l55.get_output());
    shape = this->l56.get_output().shape;
    std::cout << "After l56 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l57.build(this->l56.get_output());
    shape = this->l57.get_output().shape;
    std::cout << "After l57 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l58.build(this->l57.get_output(), this->l54.get_output());
    shape = this->l58.get_output().shape;
    std::cout << "After l58 (Add2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    this->l59.build(this->l58.get_output());
    shape = this->l59.get_output().shape;
    std::cout << "After l59 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l60.build(this->l59.get_output());
    // shape = this->l60.get_output().shape;
    // std::cout << "After l60 (DepthwiseConv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l61.build(this->l60.get_output());
    // shape = this->l61.get_output().shape;
    // std::cout << "After l61 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l62.build(this->l61.get_output());
    // shape = this->l62.get_output().shape;
    // std::cout << "After l62 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l63.build(this->l62.get_output());
    // shape = this->l63.get_output().shape;
    // std::cout << "After l63 (GlobalAveragePool): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l64.build(this->l63.get_output());
    // shape = this->l64.get_output().shape;
    // std::cout << "After l64 (Squeeze): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l65.build(this->l64.get_output());
    // shape = this->l65.get_output().shape;
    // std::cout << "After l65 (Conv2D): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l66.build(this->l65.get_output());
    // shape = this->l66.get_output().shape;
    // std::cout << "After l66 (Flatten): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";

    // this->l67.build(this->l66.get_output());
    // shape = this->l67.get_output().shape;
    // std::cout << "After l67 (FullyConnected): (" << shape[0] << ", " << shape[1] << ", " << shape[2] << ", " << shape[3] << ")\n";
}


/**
 * @brief call each layers' call(...) function in sequence
 * 
 * @param input 
 */
void call(Tensor<int16_t> &input)
{
    this->l50.call(input);
    input.free_element();

    // this->l2.call(this->l1.get_output());
    // this->l1.get_output().free_element();

    // this->l3.call(this->l2.get_output());
    // this->l2.get_output().free_element();

    // this->l4.call(this->l3.get_output());
    // this->l3.get_output().free_element();

    // this->l5.call(this->l4.get_output());
    // this->l4.get_output().free_element();

    // this->l6.call(this->l5.get_output());
    // this->l5.get_output().free_element();

    // this->l7.call(this->l6.get_output());
    // this->l6.get_output().free_element();

    // this->l8.call(this->l7.get_output());
    // this->l7.get_output().free_element();

    // this->l9.call(this->l8.get_output());
    // this->l8.get_output().free_element();

    // this->l10.call(this->l9.get_output(), this->l6.get_output());
    // this->l9.get_output().free_element();

    // this->l11.call(this->l10.get_output());
    // this->l10.get_output().free_element();

    // this->l12.call(this->l11.get_output());
    // this->l11.get_output().free_element();

    // this->l13.call(this->l12.get_output());
    // this->l12.get_output().free_element();

    // this->l14.call(this->l13.get_output());
    // this->l13.get_output().free_element();

    // this->l15.call(this->l14.get_output());
    // this->l14.get_output().free_element();

    // this->l16.call(this->l15.get_output());
    // this->l15.get_output().free_element();

    // this->l17.call(this->l16.get_output(), this->l13.get_output());
    // this->l16.get_output().free_element();

    // this->l18.call(this->l17.get_output());
    // this->l17.get_output().free_element();

    // this->l19.call(this->l18.get_output());
    // this->l18.get_output().free_element();

    // this->l20.call(this->l19.get_output());
    // this->l19.get_output().free_element();

    // this->l21.call(this->l20.get_output(), this->l17.get_output());
    // this->l20.get_output().free_element();

    // this->l22.call(this->l21.get_output());
    // this->l21.get_output().free_element();

    // this->l23.call(this->l22.get_output());
    // this->l22.get_output().free_element();

    // this->l24.call(this->l23.get_output());
    // this->l23.get_output().free_element();

    // this->l25.call(this->l24.get_output());
    // this->l24.get_output().free_element();

    // this->l26.call(this->l25.get_output());
    // this->l25.get_output().free_element();

    // this->l27.call(this->l26.get_output());
    // this->l26.get_output().free_element();

    // this->l28.call(this->l27.get_output(), this->l24.get_output());
    // this->l27.get_output().free_element();

    // this->l29.call(this->l28.get_output());
    // this->l28.get_output().free_element();

    // this->l30.call(this->l29.get_output());
    // this->l29.get_output().free_element();

    // this->l31.call(this->l30.get_output());
    // this->l30.get_output().free_element();

    // this->l32.call(this->l31.get_output(), this->l28.get_output());
    // this->l31.get_output().free_element();

    // this->l33.call(this->l32.get_output());
    // this->l32.get_output().free_element();

    // this->l34.call(this->l33.get_output());
    // this->l33.get_output().free_element();

    // this->l35.call(this->l34.get_output());
    // this->l34.get_output().free_element();

    // this->l36.call(this->l35.get_output(), this->l32.get_output());
    // this->l35.get_output().free_element();

    // this->l37.call(this->l36.get_output());
    // this->l36.get_output().free_element();

    // this->l38.call(this->l37.get_output());
    // this->l37.get_output().free_element();

    // this->l39.call(this->l38.get_output());
    // this->l38.get_output().free_element();

    // this->l40.call(this->l39.get_output());
    // this->l39.get_output().free_element();

    // this->l41.call(this->l40.get_output());
    // this->l40.get_output().free_element();

    // this->l42.call(this->l41.get_output());
    // this->l41.get_output().free_element();

    // this->l43.call(this->l42.get_output(), this->l3.get_output());
    // this->l42.get_output().free_element();

    // this->l44.call(this->l43.get_output());
    // this->l43.get_output().free_element();

    // this->l45.call(this->l44.get_output());
    // this->l44.get_output().free_element();

    // this->l46.call(this->l45.get_output());
    // this->l45.get_output().free_element();

    // this->l47.call(this->l46.get_output(), this->l43.get_output());
    // this->l46.get_output().free_element();

    // this->l48.call(this->l47.get_output());
    // this->l47.get_output().free_element();

    // this->l49.call(this->l48.get_output());
    // this->l48.get_output().free_element();

    // this->l50.call(this->l49.get_output());
    // this->l49.get_output().free_element();

    this->l51.call(this->l50.get_output());
    // this->l50.get_output().free_element();

    this->l52.call(this->l51.get_output());
    this->l51.get_output().free_element();

    this->l53.call(this->l52.get_output());
    this->l52.get_output().free_element();

    this->l54.call(this->l53.get_output(), this->l50.get_output());
    this->l50.get_output().free_element();
    this->l53.get_output().free_element();

    this->l55.call(this->l54.get_output());
    // this->l54.get_output().free_element();

    this->l56.call(this->l55.get_output());
    this->l55.get_output().free_element();

    this->l57.call(this->l56.get_output());
    this->l56.get_output().free_element();

    this->l58.call(this->l57.get_output(), this->l54.get_output());
    this->l54.get_output().free_element();
    this->l57.get_output().free_element();

    this->l59.call(this->l58.get_output());
    this->l58.get_output().free_element();

    // this->l60.call(this->l59.get_output());
    // this->l59.get_output().free_element();

    // this->l61.call(this->l60.get_output());
    // this->l60.get_output().free_element();

    // this->l62.call(this->l61.get_output());
    // this->l61.get_output().free_element();

    // this->l63.call(this->l62.get_output());
    // this->l62.get_output().free_element();

    // this->l64.call(this->l63.get_output());
    // this->l63.get_output().free_element();

    // this->l65.call(this->l64.get_output());
    // this->l64.get_output().free_element();

    // this->l66.call(this->l65.get_output());
    // this->l65.get_output().free_element();

    // this->l67.call(this->l66.get_output());
    // this->l66.get_output().free_element();
}

};