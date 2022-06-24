/*
    Copyright (c) 2013, Taiga Nomi and the respective contributors
    All rights reserved.

    Use of this source code is governed by a BSD-style license that can be found
    in the LICENSE file.
*/
#pragma once

#include <string>
#include <utility>
#include <vector>

#include <cereal/access.hpp>  // For LoadAndConstruct
#include "tiny_dnn/tiny_dnn.h"

namespace detail {

/**
 * size of layer, model, data etc.
 * change to smaller type if memory footprint is severe
 **/
typedef std::uint32_t serial_size_t;

typedef tiny_dnn::index3d<serial_size_t> shape3d_serial;

template <class T>
static inline cereal::NameValuePair<T> make_nvp(const char *name, T &&value) {
  return cereal::make_nvp(name, value);
}

template <typename T>
struct is_binary_input_archive {
  static const bool value = false;
};
template <typename T>
struct is_binary_output_archive {
  static const bool value = false;
};
template <>
struct is_binary_input_archive<cereal::BinaryInputArchive> {
  static const bool value = true;
};
template <>
struct is_binary_output_archive<cereal::BinaryOutputArchive> {
  static const bool value = true;
};

template <class Archive, typename dummy = Archive>
struct ArchiveWrapper {
  explicit ArchiveWrapper(Archive &ar) : ar(ar) {}
  template <typename T>
  void operator()(T &arg) {
    ar(arg);
  }
  Archive &ar;
};

template <typename Archive>
struct ArchiveWrapper<
  Archive,
  typename std::enable_if<is_binary_input_archive<Archive>::value,
                          Archive>::type> {
  explicit ArchiveWrapper(Archive &ar) : ar(ar) {}
  template <typename T>
  void operator()(T &arg) {
    ar(arg);
  }
  void operator()(cereal::NameValuePair<size_t &> &arg) {
    cereal::NameValuePair<serial_size_t> arg2(arg.name, 0);
    ar(arg2);
    arg.value = arg2.value;
  }
  Archive &ar;
};

template <typename Archive>
struct ArchiveWrapper<
  Archive,
  typename std::enable_if<is_binary_output_archive<Archive>::value,
                          Archive>::type> {
  explicit ArchiveWrapper(Archive &ar) : ar(ar) {}
  template <typename T>
  void operator()(T &arg) {
    ar(arg);
  }
  void operator()(cereal::NameValuePair<size_t &> &arg) {
    cereal::NameValuePair<serial_size_t> arg2(arg.name, 0);
    arg2.value = static_cast<serial_size_t>(arg.value);
    ar(arg2);
  }
  Archive &ar;
};

template <class Archive, typename T>
void arc(Archive &ar, T &&arg) {
  ArchiveWrapper<Archive> wa(ar);
  wa(arg);
}

template <class Archive>
inline void arc(Archive &ar) {}

template <class Archive, class Type, class Type2>
inline void arc(Archive &ar, Type &&arg, Type2 &&arg2) {
  arc(ar, std::forward<Type>(arg));
  arc(ar, std::forward<Type2>(arg2));
}

template <class Archive, class Type, class... Types>
inline void arc(Archive &ar, Type &&arg, Types &&... args) {
  arc(ar, std::forward<Type>(arg));
  arc(ar, std::forward<Types>(args)...);
}

}  // namespace detail

namespace cereal {

template <>
struct LoadAndConstruct<tiny_dnn::average_pooling_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::average_pooling_layer> &construct) {
    tiny_dnn::shape3d in;
    size_t stride_x, stride_y, pool_size_x, pool_size_y;
    bool ceil_mode;
    tiny_dnn::padding pad_type;

    ::detail::arc(ar, ::detail::make_nvp("in_size", in),
                  ::detail::make_nvp("pool_size_x", pool_size_x),
                  ::detail::make_nvp("pool_size_y", pool_size_y),
                  ::detail::make_nvp("stride_x", stride_x),
                  ::detail::make_nvp("stride_y", stride_y),
                  ::detail::make_nvp("ceil_mode", ceil_mode),
                  ::detail::make_nvp("pad_type", pad_type));
    construct(in.width_, in.height_, in.depth_, pool_size_x, pool_size_y,
              stride_x, stride_y, ceil_mode, pad_type);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::convolutional_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::convolutional_layer> &construct) {
    size_t w_width, w_height, out_ch, w_stride, h_stride, w_dilation,
      h_dilation;
    bool has_bias;
    tiny_dnn::shape3d in;
    tiny_dnn::padding pad_type;
    tiny_dnn::core::connection_table tbl;

    ::detail::arc(ar, ::detail::make_nvp("in_size", in),
                  ::detail::make_nvp("window_width", w_width),
                  ::detail::make_nvp("window_height", w_height),
                  ::detail::make_nvp("out_channels", out_ch),
                  ::detail::make_nvp("connection_table", tbl),
                  ::detail::make_nvp("pad_type", pad_type),
                  ::detail::make_nvp("has_bias", has_bias),
                  ::detail::make_nvp("w_stride", w_stride),
                  ::detail::make_nvp("h_stride", h_stride),
                  ::detail::make_nvp("w_dilation", w_dilation),
                  ::detail::make_nvp("h_dilation", h_dilation));

    construct(in.width_, in.height_, w_width, w_height, in.depth_, out_ch, tbl,
              pad_type, has_bias, w_stride, h_stride, w_dilation, h_dilation);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::fully_connected_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar,
    cereal::construct<tiny_dnn::fully_connected_layer> &construct) {
    size_t in_dim, out_dim;
    bool has_bias;

    ::detail::arc(ar, ::detail::make_nvp("in_size", in_dim),
                  ::detail::make_nvp("out_size", out_dim),
                  ::detail::make_nvp("has_bias", has_bias));
    construct(in_dim, out_dim, has_bias);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::softmax_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::softmax_layer> &construct) {
    tiny_dnn::shape3d in_shape;

    ::detail::arc(ar, ::detail::make_nvp("in_size", in_shape));
    construct(in_shape);
  }
};

template <>
struct LoadAndConstruct<tiny_dnn::leaky_relu_layer> {
  template <class Archive>
  static void load_and_construct(
    Archive &ar, cereal::construct<tiny_dnn::leaky_relu_layer> &construct) {
    tiny_dnn::shape3d in_shape;
    tiny_dnn::float_t epsilon;

    ::detail::arc(ar, ::detail::make_nvp("in_size", in_shape),
                  ::detail::make_nvp("epsilon", epsilon));

    construct(in_shape, epsilon);
  }
};

}  // namespace cereal

namespace tiny_dnn {

struct serialization_buddy {
#ifndef CNN_NO_SERIALIZATION

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::layer &layer) {
    auto all_weights = layer.weights();
    for (auto weight : all_weights) {
      ar(*weight);
    }
    layer.initialized_ = true;
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::average_pooling_layer &layer) {
    ::detail::arc(ar, ::detail::make_nvp("in_size", layer.in_),
                  ::detail::make_nvp("pool_size_x", layer.pool_size_x_),
                  ::detail::make_nvp("pool_size_y", layer.pool_size_y_),
                  ::detail::make_nvp("stride_x", layer.stride_x_),
                  ::detail::make_nvp("stride_y", layer.stride_y_),
                  ::detail::make_nvp("ceil_mode", layer.ceil_mode_),
                  ::detail::make_nvp("pad_type", layer.pad_type_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::convolutional_layer &layer) {
    auto &params_ = layer.params_;
    ::detail::arc(ar, ::detail::make_nvp("in_size", params_.in),
                  ::detail::make_nvp("window_width", params_.weight.width_),
                  ::detail::make_nvp("window_height", params_.weight.height_),
                  ::detail::make_nvp("out_channels", params_.out.depth_),
                  ::detail::make_nvp("connection_table", params_.tbl),
                  ::detail::make_nvp("pad_type", params_.pad_type),
                  ::detail::make_nvp("has_bias", params_.has_bias),
                  ::detail::make_nvp("w_stride", params_.w_stride),
                  ::detail::make_nvp("h_stride", params_.w_stride),
                  ::detail::make_nvp("w_dilation", params_.w_dilation),
                  ::detail::make_nvp("h_dilation", params_.h_dilation));
  }

  template <class Archive>
  static inline void serialize(Archive &ar,
                               tiny_dnn::fully_connected_layer &layer) {
    auto &params_ = layer.params_;
    ::detail::arc(ar, ::detail::make_nvp("in_size", params_.in_size_),
                  ::detail::make_nvp("out_size", params_.out_size_),
                  ::detail::make_nvp("has_bias", params_.has_bias_));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::softmax_layer &layer) {
    ::detail::arc(ar, ::detail::make_nvp("in_size", layer.in_shape()[0]));
  }

  template <class Archive>
  static inline void serialize(Archive &ar, tiny_dnn::leaky_relu_layer &layer) {
    ::detail::arc(ar, ::detail::make_nvp("in_size", layer.in_shape()[0]),
                  ::detail::make_nvp("epsilon", layer.epsilon_));
  }

#endif  // #ifndef CNN_NO_SERIALIZATION
};      // struct serialization_buddy

template <class Archive, typename T>
typename std::enable_if<std::is_base_of<tiny_dnn::layer, T>::value>::type
serialize(Archive &ar, T &layer) {
  auto &inconstant_layer =
    const_cast<typename std::remove_const<T>::type &>(layer);
  inconstant_layer.serialize_prolog(ar);
  serialization_buddy::serialize(ar, inconstant_layer);
}

template <class Archive, typename T>
void serialize(Archive &ar, tiny_dnn::index3d<T> &idx) {
  ::detail::arc(ar, ::detail::make_nvp("width", idx.width_),
                ::detail::make_nvp("height", idx.height_),
                ::detail::make_nvp("depth", idx.depth_));
}

namespace core {

template <class Archive>
void serialize(Archive &ar, tiny_dnn::core::connection_table &tbl) {
  ::detail::arc(ar, ::detail::make_nvp("rows", tbl.rows_),
                ::detail::make_nvp("cols", tbl.cols_));
  if (tbl.is_empty()) {
    std::string all("all");
    ::detail::arc(ar, ::detail::make_nvp("connection", all));
  } else {
    ::detail::arc(ar, ::detail::make_nvp("connection", tbl.connected_));
  }
}

}  // namespace core

}  // namespace tiny_dnn
