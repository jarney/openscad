#pragma once

#include "NodeProgram.hpp"

namespace JNodes {
    namespace core {

/**
 * This is the base class for serializing node programs
 * to various formats.
 */
class NodeProgramSerializer {
public:
    virtual ~NodeProgramSerializer() = default;
    virtual void write(const NodeProgram &program, std::ostream & output_stream) const = 0;
    virtual void read(NodeProgram & program, std::istream & input_stream) const = 0;
};

/**
 * Singleton implementation of a serializer
 * that serializes the node program to and
 * from JSON.
 */
class NodeProgramSerializerJSON : public NodeProgramSerializer
{
private:
    /**
     * Constructor is private because we are a singleton.
     */
    NodeProgramSerializerJSON();
    ~NodeProgramSerializerJSON() = default;
public:
    /**
     * No copy because we are a singleton.
     */
    NodeProgramSerializerJSON(NodeProgramSerializerJSON const &other) = delete;
    /**
     * No copy because we are a singleton.
     */
    void operator=(NodeProgramSerializerJSON const &other)  = delete;

    /**
     * Public method to return the serializer.
     */
    static const NodeProgramSerializerJSON & instance();
    /**
     * Writes the given node program to
     * the given stream using the serialization
     * method of JSON output.
     */
    void write(const NodeProgram &program, std::ostream & output_stream) const override;
    /**
     * Reads the input stream and fills in the (assumed empty)
     * node program based on the file content.
     */
    void read(NodeProgram & program, std::istream & input_stream) const override;
};

    } // End core
} // End JNodes
