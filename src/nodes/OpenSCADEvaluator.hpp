#pragma once

#include <QtNodes/DataFlowGraphModel>
#include "nodes/OpenSCADDataTypes.hpp"

std::string evaluateToSCAD(const QtNodes::DataFlowGraphModel & model);
