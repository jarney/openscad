#pragma once

#include <memory>

#include "core/SourceFile.h"
#include "core/LocalScope.h"
#include "core/ModuleInstantiation.h"
#include "core/Assignment.h"
#include "core/Expression.h"
#include "core/Parameters.h"
#include "core/Context.h"
#include "core/BuiltinContext.h"
#include "core/ScopeContext.h"

#include "node--js/NodeModule.hpp"
#include "node--js/NodeGraph.hpp"
#include "node--js/NodeType.hpp"
#include "node--js/NodePort.hpp"

void
processAssignment(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentAssignments,
    const std::shared_ptr<Assignment> & assignment,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void
processExpression(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentPort,
    const std::shared_ptr<Expression> & expression,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void
processExpressionUnaryOp(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentPort,
    const UnaryOp *operation,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void
processExpressionBinaryOp(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentPort,
    const BinaryOp *operation,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void
processExpressionFunctionCall(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentPort,
    const FunctionCall *functionCall,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void
processExpressionBuiltinFunctionCall(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentPort,
    const BuiltinFunction *functionCall,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void processSourceFile(
    NodeJS::core::NodeModule & program,
    SourceFile *sourceFile,
    const std::shared_ptr<const Context>& context
    );

void processLocalScope(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentAssignments,
    NodeJS::core::PortId parentFunctionDefinitions,
    NodeJS::core::PortId parentModuleDefinitions,
    NodeJS::core::PortId parentModuleInstantiations,
    std::shared_ptr<LocalScope> localScope,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void processModuleInstantiation(
    NodeJS::core::NodeModule & program,
    NodeJS::core::NodeGraph *currentGraph,
    NodeJS::core::NodeId parentNode,
    NodeJS::core::PortId parentAssignments,
    NodeJS::core::PortId parentFunctionDefinitions,
    NodeJS::core::PortId parentModuleDefinitions,
    NodeJS::core::PortId parentModuleInstantiations,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    const std::shared_ptr<const Context>& context,
    int depth,
    int i
    );

