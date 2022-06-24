/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include "tiny_dnn/layers/layers.h"

namespace tiny_dnn {

template <typename T>
void register_layers(T* h) {
  h->template register_layer<average_pooling_layer>("avepool");
  h->template register_layer<convolutional_layer>("conv");
  h->template register_layer<fully_connected_layer>("fully_connected");

  h->template register_layer<softmax_layer>("softmax");
  h->template register_layer<leaky_relu_layer>("leaky_relu");
}

}  // namespace tiny_dnn
