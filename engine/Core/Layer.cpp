#include "pch.h"
#include "Layer.h"

namespace Circe {

Layer::Layer(const std::string& name)
    : m_Name(name) {}

Layer::~Layer() = default;

}