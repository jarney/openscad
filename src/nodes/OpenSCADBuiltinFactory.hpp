#pragma once

#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramModelRegistry.hpp"

class NodeProgramGraphModel;

class OpenSCADBuiltinFactory : public NodeDelegateFactory {
public:
    OpenSCADBuiltinFactory(
	std::unique_ptr<NodeModelType> type
    );
    std::string getName() const override;
    std::string getCategory() const override;
    std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeProgramGraphModel & model) const;
private:
    std::unique_ptr<NodeModelType> _type;
};
