#include <stdio.h>
#include <unistd.h>

#include <QtCore/QFileInfo>
#include "OpenSCADModels.hpp"
#include "OpenSCADEvaluator.hpp"
#include "OpenSCADSerializer.hpp"

int main(int argc, char *argv[])
{
//    QApplication app(argc, argv);
    
    if (argc != 2) {
	fprintf(stderr, "Usage: processor filename\n");
	return 1;
    }
    
    if (!QFileInfo::exists(argv[1])) {
	fprintf(stderr, "Cannot process file, it does not exist\n");
	return 2;
    }

    std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registry = SCADModels::registerDataModels();
    QtNodes::DataFlowGraphModel dataFlowGraphModel(registry);

    QJsonObject object = oscd_loadJson(argv[1]);
    dataFlowGraphModel.load(object);
    
    evaluateToSCAD(dataFlowGraphModel);
    return 0;
}
