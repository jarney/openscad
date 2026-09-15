#pragma once

#include "nodes/NodeModelType.hpp"
#include "nodes/NodeProgramModelRegistry.hpp"

class OpenSCADBuiltinFactory : public NodeDelegateFactory {
public:
    OpenSCADBuiltinFactory(
	std::unique_ptr<NodeModelType> type
    );
    std::string getName() const override;
    std::string getCategory() const override;
    std::unique_ptr<QtNodes::NodeDelegateModel> create() const;
private:
    std::unique_ptr<NodeModelType> _type;
};


