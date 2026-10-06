#include "nodes/gui/GraphModelAdapter.hpp"

#include <unordered_set>

using namespace NodeJS::core;
using namespace NodeJS::gui;

GraphModelAdapter::GraphModelAdapter(NodeJS::core::NodeGraph & aGraph)
    : mGraph(aGraph)
{}
