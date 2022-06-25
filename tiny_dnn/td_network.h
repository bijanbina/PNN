#ifndef TD_NETWORK_H
#define TD_NETWORK_H

#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
#include <QString>
#include <QDir>
#include <QDebug>

#include "tiny_dnn/nodes.h"

#include <cereal/types/tuple.hpp>
#include <cereal/types/utility.hpp>

#include "tiny_dnn/layers/layer.h"
#include "tiny_dnn/util/util.h"

namespace cereal
{
template <typename Archive>
void load(Archive &ar, std::vector<std::shared_ptr<tiny_dnn::layer>> &v)
{
    cereal::size_type size;
    ar(cereal::make_size_tag(size));

    for (size_t i = 0; i < size; i++)
    {
        v.emplace_back(tiny_dnn::layer::load_layer(ar));
    }
}  // namespace cereal
}

class TdNetwork
{
public:
    TdNetwork(QString word);
    ~TdNetwork();

    template <typename InputArchive>
    void load(InputArchive &ia);

    tiny_dnn::vec_t predict(float *data, int len);
    void load_connections();

    bool model_loaded;
    QString m_name;

private:

    std::vector<std::shared_ptr<tiny_dnn::layer>> buf_nodes;
    /* List of all nodes which includes own_nodes */
    std::vector<tiny_dnn::layer *> layers;
};

template <typename InputArchive>
void TdNetwork::load(InputArchive &ia)
{
    buf_nodes.clear();
    layers.clear();

    ia(cereal::make_nvp("nodes", buf_nodes));

    for (auto &n : buf_nodes)
    {
        layers.push_back(&*n);
    }

    load_connections();

    // load weights
    for( int i=0 ; i<layers.size() ; i++ )
    {
        tiny_dnn::layer *n = layers[i];
        ia(*n);
    }
}

#endif // TD_NETWORK_H
