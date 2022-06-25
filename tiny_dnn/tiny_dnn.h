/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include "tiny_dnn/config.h"
#include "tiny_dnn/network.h"
#include "tiny_dnn/nodes.h"
#include "tiny_dnn/td_network.h"

#include <cereal/types/tuple.hpp>
#include <cereal/types/utility.hpp>

#include "tiny_dnn/layers/layer.h"
#include "tiny_dnn/util/util.h"

#include "tiny_dnn/core/framework/tensor.h"

#include "tiny_dnn/core/framework/device.h"
#include "tiny_dnn/core/framework/program_manager.h"

#include "tiny_dnn/activations/leaky_relu_layer.h"
#include "tiny_dnn/activations/softmax_layer.h"
#include "tiny_dnn/layers/average_pooling_layer.h"
#include "tiny_dnn/layers/convolutional_layer.h"
#include "tiny_dnn/layers/fully_connected_layer.h"

#include "tiny_dnn/util/deform.h"
#include "tiny_dnn/util/product.h"
#include "tiny_dnn/util/weight_init.h"
#include "tiny_dnn/util/nms.h"

#include "tiny_dnn/util/deserialization_helper.h"
#include "tiny_dnn/util/serialization_helper.h"
// to allow upcasting
CEREAL_REGISTER_TYPE(tiny_dnn::leaky_relu_layer)
CEREAL_REGISTER_TYPE(tiny_dnn::softmax_layer)
