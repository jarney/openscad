#include "OpenSCADGraphModel.hpp"

OpenSCADGraphModel::OpenSCADGraphModel(std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry)
    : QtNodes::DataFlowGraphModel(registry)
{}

OpenSCADGraphModel::~OpenSCADGraphModel()
{}
    
bool
OpenSCADGraphModel::dataTypeConnectionAllowed(const QtNodes::NodeDataType & outType, const QtNodes::NodeDataType & inType) const
{
#if 0
    // This whole thing isn't really well thought out yet, so maybe get rid of it?
    if (inType.id == DATA_VECTOR_OR_STRING.id) {
	return outType.id == DATA_VECTOR.id ||
	    outType.id == DATA_STRING.id ||
	    outType.id == DATA_VECTOR_OR_STRING.id;
    }
#endif
    return outType.id == inType.id;
}
