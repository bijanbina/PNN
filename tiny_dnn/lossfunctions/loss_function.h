/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <vector>
#include <QDebug>
#include "tiny_dnn/util/util.h"

namespace tiny_dnn {

// mean-squared-error loss function for regression
class mse {
public:
    static float_t f(const vec_t &y, const vec_t &t) {
        assert(y.size() == t.size());
        float_t d{0.0};

        for (size_t i = 0; i < y.size(); ++i) d += (y[i] - t[i]) * (y[i] - t[i]);

        return d / static_cast<float_t>(y.size());
    }

    static vec_t df(const vec_t &y, const vec_t &t)
    {
        assert(y.size() == t.size());
        vec_t d(t.size());
        float_t factor = float_t(2) / static_cast<float_t>(t.size());

        for(size_t i = 0; i < y.size(); ++i)
        {
            d[i] = factor * (y[i] - t[i]);
        }

        return d;
    }
};

// absolute loss function for regression
class absolute {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0};

    for (size_t i = 0; i < y.size(); ++i) d += std::abs(y[i] - t[i]);

    return d / static_cast<float_t>(y.size());
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());
    float_t factor = float_t(1) / static_cast<float_t>(t.size());

    for (size_t i = 0; i < y.size(); ++i) {
      float_t sign = y[i] - t[i];
      if (sign < float_t{0.f})
        d[i] = -factor;
      else if (sign > float_t{0.f})
        d[i] = factor;
      else
        d[i] = {0};
    }

    return d;
  }
};

// absolute loss with epsilon range for regression
// epsilon range [-eps, eps] with eps = 1./fraction
template <int fraction>
class absolute_eps {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0};
    const float_t eps = float_t(1) / fraction;

    for (size_t i = 0; i < y.size(); ++i) {
      float_t diff = std::abs(y[i] - t[i]);
      if (diff > eps) d += diff;
    }
    return d / static_cast<float_t>(y.size());
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());
    const float_t factor = float_t(1) / static_cast<float_t>(t.size());
    const float_t eps    = float_t(1) / fraction;

    for (size_t i = 0; i < y.size(); ++i) {
      float_t sign = y[i] - t[i];
      if (sign < -eps)
        d[i] = -factor;
      else if (sign > eps)
        d[i] = factor;
      else
        d[i] = 0.f;
    }
    return d;
  }
};

// cross-entropy loss function for (multiple independent) binary classifications
class cross_entropy {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0};

    for (size_t i = 0; i < y.size(); ++i)
      d += -t[i] * std::log(y[i]) -
           (float_t(1) - t[i]) * std::log(float_t(1) - y[i]);

    return d;
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());

    for (size_t i = 0; i < y.size(); ++i)
      d[i]        = (y[i] - t[i]) / (y[i] * (float_t(1) - y[i]));

    return d;
  }
};

// cross-entropy loss function for multi-class classification
class cross_entropy_multiclass {
 public:
  static float_t f(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    float_t d{0.0};

    for (size_t i = 0; i < y.size(); ++i) d += -t[i] * std::log(y[i]);

    return d;
  }

  static vec_t df(const vec_t &y, const vec_t &t) {
    assert(y.size() == t.size());
    vec_t d(t.size());

    for (size_t i = 0; i < y.size(); ++i) d[i] = -t[i] / y[i];

    return d;
  }
};

inline void apply_cost_if_defined(tensor_t &gradient, const vec_t &cost)
{
    int channel_count = gradient.size();
    for( int channel=0 ; channel<channel_count ; channel++ )
    {
        // @todo optimize? (use AVX or so)
        gradient[channel][0] *= cost[channel];
    }
}

// gradient for a minibatch
template <typename E>
std::vector<tensor_t> gradient(const std::vector<tensor_t> &y,
                               const std::vector<tensor_t> &t,
                               const tensor_t &t_cost,
                               int s_index, int e_index)
{
    const size_t sample_count  = y.size();
    std::vector<tensor_t> gradients(sample_count);

    for(int sample=s_index; sample < e_index; ++sample)
    {
        gradients[sample].resize(y[sample].size());
        for(size_t i = 0; i < y[sample].size(); i++)
        {
            gradients[sample][i] = E::df(y[sample][i],
                                         t[sample][i]);
        }

        if( sample<t_cost.size() )
        {
            apply_cost_if_defined(gradients[sample], t_cost[sample]);
        }
    }

    return gradients;
}

}  // namespace tiny_dnn
