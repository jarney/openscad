#include "NodeProgramSerializer.hpp"

NodeProgramSerializerJSON::NodeProgramSerializerJSON()
{}

const NodeProgramSerializerJSON &
NodeProgramSerializerJSON::instance()
{
    static NodeProgramSerializerJSON instance;
    return instance;
}
    

/**
 * Writes the given node program to
 * the given stream using the serialization
 * method of JSON output.
 */
void
NodeProgramSerializerJSON::write(const NodeProgram &program, std::ostream & output_stream) const
{
    QJsonObject main;
    for (const NodeProgram::GraphId & id : program.getGraphs()) {
	const OpenSCADGraphModel *dataFlowGraphModel = program.getGraph(id);
	QJsonObject object = dataFlowGraphModel->save();
	main[QString::fromStdString(id)] = object;
    }
    QJsonDocument main_document;
    main_document.setObject(main);
    QByteArray byte_array = main_document.toJson();
    output_stream << byte_array.toStdString();
}
/**
 * Reads the input stream and fills in the (assumed empty)
 * node program based on the file content.
 */
void
NodeProgramSerializerJSON::read(NodeProgram & program, std::istream & input_stream) const
{
    std::string json_string(std::istreambuf_iterator<char>(input_stream), {});
    QJsonDocument document = QJsonDocument().fromJson(QByteArray::fromStdString(json_string));
    QJsonObject document_object = document.object();

    // For each key in the json object...
    for (const auto key : document_object.keys()) {
	QJsonObject object = document_object[key].toObject();
	
	// The new ID may not match the old one????
	// This could be a problem for referencing graphs to one another.
	OpenSCADGraphModel *dataFlowGraphModel = program.newGraph(key.toStdString());
	dataFlowGraphModel->load(object);
    }
}
