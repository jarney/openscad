#pragma once

#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramModelRegistry.hpp"

class NodeProgramGraphModel;

class OpenSCADBuiltinFactory : public NodeDelegateFactory {
public:
    OpenSCADBuiltinFactory(
	std::unique_ptr<NodeModelType> type
    );
    virtual ~OpenSCADBuiltinFactory() = default;
    
    std::string getName() const override;
    std::string getDescription() const override;
    std::string getCategory() const override;
    std::unique_ptr<QtNodes::NodeDelegateModel> create(NodeProgramGraphModel & model) const;
    std::string getIcon(void) const override;
private:
    std::unique_ptr<NodeModelType> _type;
    std::string _icon;
};
