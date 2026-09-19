#include <stdio.h>
#include <QString>
#include <QtNodes/Definitions>

#include "nodes/Node.hpp"
#include "nodes/openscad/NodeProgramSerializerOpenSCAD.hpp"

#include "openscad.h"

using namespace JNodes::core;
using namespace JNodes::openscad;

NodeProgramSerializerOpenSCAD::NodeProgramSerializerOpenSCAD()
{}

const NodeProgramSerializer &
NodeProgramSerializerOpenSCAD::instance()
{
    static NodeProgramSerializerOpenSCAD instance;
    return instance;
}
    

// This is a bad place to put the
// code, but we don't have a better plan
// right now.
#include "Processor.h"
#include "Parser.h"

/**
 * Writes the given node program to
 * the given stream using the serialization
 * method of JSON output.
 */
void
NodeProgramSerializerOpenSCAD::write(const NodeProgram &program, std::ostream & output_stream) const
{
    const NodeGraph *graph = program.getGraph("main");
    if (!graph) {
	fprintf(stderr, "Cannot find main\n");
	return;
    }
    std::string scadOutput = toString(*graph);
    output_stream << scadOutput;
}

/**
 * Reads the input stream and fills in the (assumed empty)
 * node program based on the file content.
 */
int
NodeProgramSerializerOpenSCAD::read(NodeProgram & program, std::istream & input_stream) const
{
    std::string fulltext(std::istreambuf_iterator<char>(input_stream), {});

    SourceFile *sourceFile;
    std::string fname("none");
    
    sourceFile = parse(
	sourceFile,
	fulltext,
	fname,
	fname,
	false) ? sourceFile : nullptr;
    if (!sourceFile) {
	fprintf(stderr, "Unsuccessful parse\n");
	return -1;
    }
    fprintf(stderr, "Got a valid parse tree\n");

    EvaluationSession session{sourceFile->getFullpath()};
    ContextHandle<BuiltinContext> builtin_context{Context::create<BuiltinContext>(&session)};
    
    processSourceFile(program, sourceFile, *builtin_context);

    return 0;
}

#include "Parser.cpp"
