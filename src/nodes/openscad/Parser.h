#include "core/SourceFile.h"
#include "core/LocalScope.h"
#include "core/ModuleInstantiation.h"
#include "core/Assignment.h"
#include "core/Expression.h"
#include "core/Parameters.h"
#include "core/Context.h"
#include "core/BuiltinContext.h"
#include "core/ScopeContext.h"

void
processAssignment(
    NodeProgram & program,
    NodeGraph *currentGraph,
    const std::shared_ptr<Assignment> & assignment,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void
processExpression(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    const std::shared_ptr<Expression> & expression,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void processSourceFile(
    NodeProgram & program,
    SourceFile *sourceFile,
    const std::shared_ptr<const Context>& context
    );

void processLocalScope(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<LocalScope> localScope,
    const std::shared_ptr<const Context>& context,
    int depth
    );

void processModuleInstantiation(
    NodeProgram & program,
    NodeGraph *currentGraph,
    QtNodes::NodeId parentNode,
    std::shared_ptr<ModuleInstantiation> moduleInstantiation,
    const std::shared_ptr<const Context>& context,
    int depth,
    int i
    );

