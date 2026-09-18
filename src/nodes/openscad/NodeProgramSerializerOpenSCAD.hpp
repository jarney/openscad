#include "nodes/NodeProgramSerializer.hpp"

namespace JNodes {
    namespace openscad {

/**
 * Singleton implementation of a serializer
 * that serializes the node program to and
 * from OpenSCAD
 */
class NodeProgramSerializerOpenSCAD : public JNodes::core::NodeProgramSerializer
{
private:
    /**
     * Constructor is private because we are a singleton.
     */
    NodeProgramSerializerOpenSCAD();
    ~NodeProgramSerializerOpenSCAD() = default;
public:
    /**
     * No copy because we are a singleton.
     */
    NodeProgramSerializerOpenSCAD(NodeProgramSerializerOpenSCAD const &other) = delete;
    /**
     * No copy because we are a singleton.
     */
    void operator=(NodeProgramSerializerOpenSCAD const &other)  = delete;

    /**
     * Public method to return the serializer.
     */
    static const NodeProgramSerializer & instance();
    /**
     * Writes the given node program to
     * the given stream using the serialization
     * method of JSON output.
     */
    void write(const JNodes::core::NodeProgram &program, std::ostream & output_stream) const override;
    /**
     * Reads the input stream and fills in the (assumed empty)
     * node program based on the file content.
     */
    void read(JNodes::core::NodeProgram & program, std::istream & input_stream) const override;

    static std::string toString(const JNodes::core::NodeGraph & model);
    
};

    } // End openscad
} // End JNodes
