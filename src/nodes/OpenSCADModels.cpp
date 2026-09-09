#include "OpenSCADModels.hpp"

BaseSCADModel::BaseSCADModel(std::string name, std::string caption)
    : _name(name)
    , _caption(caption)
    , _processor(default_processor)
{}

QString
BaseSCADModel::name() const
{ return QString::fromStdString(_name); }

QString
BaseSCADModel::caption() const
{ return QString::fromStdString(_caption); }

//void
//BaseSCADModel::compute()
//{}

unsigned int BaseSCADModel::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
	return _inputPorts.size();
    }
    else {
	return _outputPorts.size();
    }
}

QtNodes::NodeDataType
BaseSCADModel::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _inputPorts.at(portIndex)->nodeDataType();
    }
    else {
	return _outputPorts.at(portIndex)->nodeDataType();
    }
}

QString
BaseSCADModel::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _inputPorts.at(portIndex)->portCaption();
    }
    else {
	return _outputPorts.at(portIndex)->portCaption();
    }
}

bool
BaseSCADModel::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
	return _inputPorts.at(portIndex)->portCaptionVisible();
    }
    else {
	return _outputPorts.at(portIndex)->portCaptionVisible();
    }
}

std::shared_ptr<QtNodes::NodeData> BaseSCADModel::outData(QtNodes::PortIndex portIndex)
{
    return _outputPorts.at(portIndex)->getData();
}

void BaseSCADModel::setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex)
{
    if (!data) {
        Q_EMIT dataInvalidated(0);
    }

    _inputPorts.at(portIndex)->setData(data);
}


std::string
BaseSCADModel::inputPortName(QtNodes::PortIndex portIndex) const
{
    return _inputPortNames.at(portIndex);
}
std::string
BaseSCADModel::outputPortName(QtNodes::PortIndex portIndex) const
{
    return _outputPortNames.at(portIndex);
}

void
BaseSCADModel::addInputPort(std::unique_ptr<BaseSCADPort> inputPort, std::string inputPortName)
{
    _inputPortNames[_inputPorts.size()] = inputPortName;
    _inputPorts.push_back(std::move(inputPort));
}

void
BaseSCADModel::addOutputPort(std::unique_ptr<BaseSCADPort> outputPort, std::string outputPortName)
{
    _outputPortNames[_outputPorts.size()] = outputPortName;
    _outputPorts.push_back(std::move(outputPort));
}

void
BaseSCADModel::setProcessor(NodeProcessor processor)
{
    _processor = processor;
}

void
BaseSCADModel::process(const PortFunctionData & input, PortFunctionData & output) const
{
    _processor(*this, input, output);
}

void
BaseSCADModel::default_processor(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
}

/*********************************************/
BaseSCADPort::BaseSCADPort(QtNodes::NodeDataType type, QString caption)
    : _type(type)
    , _caption(caption)
{}

BaseSCADPort::BaseSCADPort(QtNodes::NodeDataType type)
    : _type(type)
    , _caption("")
{}

BaseSCADPort::~BaseSCADPort()
{}

QtNodes::NodeDataType
BaseSCADPort::nodeDataType() const
{ return _type; }

QWidget *
BaseSCADPort::portWidget()
{ return nullptr; }

QString
BaseSCADPort::portCaption() const
{ return _caption; }

bool
BaseSCADPort::portCaptionVisible() const
{ return _caption.length() > 0; }

void
BaseSCADPort::setData(std::shared_ptr<QtNodes::NodeData> data)
{ _data = data; }

std::shared_ptr<QtNodes::NodeData>
BaseSCADPort::getData() const
{ return _data; }

/*********************************************/

static std::string FIND(const PortFunctionData & input, std::string key, std::string def)
{
    const auto it = input.find(key);
    if (it == input.end()) {
	fprintf(stderr, "Could not find key %s returning default\n", key.c_str());
	return def;
    }
    return it->second;
}
static bool KEY_EXISTS(const PortFunctionData & input, std::string key)
{
    const auto it = input.find(key);
    if (it == input.end()) {
	return false;
    }
    return true;
    
}

// Primitives
SCADModels::RegistryItemPtr
SCADModels::f_prim_sphere()
{
    auto model = std::make_unique<BaseSCADModel>("sphere", "Sphere");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "d"), "d");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_sphere_process);
    return model;
}

void
SCADModels::f_prim_sphere_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("sphere(");
    if (KEY_EXISTS(input, "r")) {
	out += std::string("r=") + FIND(input, "r", "15.0");
    }
    else if (KEY_EXISTS(input, "d")) {
	out += std::string("d=") + FIND(input, "d", "30.0");
    }
    out += std::string(");\n");
    output["Geometry"] = out;
    
}

SCADModels::RegistryItemPtr
SCADModels::f_util_color()
{
    auto model = std::make_unique<BaseSCADModel>("color", "Color");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "c"), "color");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "alpha"), "alpha");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_util_color_process);
    return model;
}

void
SCADModels::f_util_color_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("color(");
    if (KEY_EXISTS(input, "color")) {
	out += std::string("c=") + FIND(input, "color", "15.0") + ", ";
    }
    else if (KEY_EXISTS(input, "alpha")) {
	out += std::string("alpha=") + FIND(input, "alpha", "30.0");
    }
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}


SCADModels::RegistryItemPtr
SCADModels::f_xform_translate()
{
    auto model = std::make_unique<BaseSCADModel>("translate", "Translate");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_translate_process);
    return model;
}
void
SCADModels::f_xform_translate_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("translate(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}
SCADModels::RegistryItemPtr
SCADModels::f_xform_mirror()
{
    auto model = std::make_unique<BaseSCADModel>("mirror", "Mirror");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_mirror_process);
    return model;
}
void
SCADModels::f_xform_mirror_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("mirror(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}
SCADModels::RegistryItemPtr
SCADModels::f_xform_scale()
{
    auto model = std::make_unique<BaseSCADModel>("scale", "Scale");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_scale_process);
    return model;
}
void
SCADModels::f_xform_scale_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("scale(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}

SCADModels::RegistryItemPtr
SCADModels::f_xform_resize()
{
    auto model = std::make_unique<BaseSCADModel>("resize", "Resize");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "vector"), "vector");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_xform_resize_process);
    return model;
}
void
SCADModels::f_xform_resize_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("resize(");
    out += std::string("v=") + FIND(input, "vector", "[0,0,0]");
    out += std::string(") {\n");
    out += FIND(input, "Geometry", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}

SCADModels::RegistryItemPtr
SCADModels::f_flow_if()
{
    auto model = std::make_unique<BaseSCADModel>("if", "If");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "condition"), "condition");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_flow_if_process);
    return model;
}
void
SCADModels::f_flow_if_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string out = std::string();
    out += std::string("if(");
    out += FIND(input, "condition", "true");
    out += std::string(") {\n");
    out += FIND(input, "a", "");
    out += std::string("} else {\n");
    out += FIND(input, "b", "");
    out += std::string("}\n");
    output["Geometry"] = out;
}



SCADModels::RegistryItemPtr
SCADModels::f_prim_cube()
{
    auto model = std::make_unique<BaseSCADModel>("cube", "Cube");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "x"), "x");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "y"), "y");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "z"), "z");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_cube_process);
    return model;
}

void
SCADModels::f_prim_cube_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("        cube([") +
	FIND(input, "x", "15.0") + std::string(", ") +
	FIND(input, "y", "15.0") + std::string(", ") +
	FIND(input, "z", "15.0") +
	std::string("]);\n");
}

SCADModels::RegistryItemPtr
SCADModels::f_prim_cylinder()
{
    auto model = std::make_unique<BaseSCADModel>("cylinder", "Cylinder");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "r"), "r");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "h"), "h");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_cylinder_process);
    return model;
}
void
SCADModels::f_prim_cylinder_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("cylinder(") +
	FIND(input, "r", "15") + std::string(", ") +
	FIND(input, "h", "15") +
	std::string(");");
}

// Boolean operations
SCADModels::RegistryItemPtr
SCADModels::f_op_union()
{
    auto model = std::make_unique<BaseSCADModel>("union", "Union");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_union_process);
    return model;
}
void
SCADModels::f_op_union_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("union() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}

SCADModels::RegistryItemPtr
SCADModels::f_op_hull()
{
    auto model = std::make_unique<BaseSCADModel>("hull", "Convex Hull");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_hull_process);
    return model;
}
void
SCADModels::f_op_hull_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("hull() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}

SCADModels::RegistryItemPtr
SCADModels::f_op_minkowski()
{
    auto model = std::make_unique<BaseSCADModel>("minkowski", "Minkowski");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_minkowski_process);
    return model;
}
void
SCADModels::f_op_minkowski_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("minkowski() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}

SCADModels::RegistryItemPtr
SCADModels::f_op_difference()
{
    auto model = std::make_unique<BaseSCADModel>("difference", "Difference");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_difference_process);
    return model;
}
void
SCADModels::f_op_difference_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("difference() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}


SCADModels::RegistryItemPtr
SCADModels::f_op_intersection()
{
    auto model = std::make_unique<BaseSCADModel>("intersection", "Intersection");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "a"), "a");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "b"), "b");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_op_intersection_process);
    return model;
}
void
SCADModels::f_op_intersection_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    fprintf(stderr, "Processing intersection\n");
    output["Geometry"] = std::string("intersection() {\n") +
	std::string("    {\n") + 
        FIND(input, "a", "{}") +
	std::string("    }") + 
	std::string("    {\n") +
        FIND(input, "b", "{}") +
	std::string("    }") + 
	std::string("};");
}


SCADModels::RegistryItemPtr
SCADModels::f_math_rands()
{
    auto model = std::make_unique<BaseSCADModel>("rands", "Random Vector");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "min"), "max");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "min"), "max");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "n"), "n");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "seed"), "seed");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_math_rands_process);
    return model;
}
void
SCADModels::f_math_rands_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    auto args = std::string();

    args += FIND(input, "min", "0");
    args += std::string(", ");
    args += FIND(input, "max", "100");
    args += std::string(", ");
    args += FIND(input, "n", "1");
    if (KEY_EXISTS(input, "seed")) {
	args += std::string(", ");
	args += FIND(input, "seed", "0");
    }
    output["Geometry"] = std::string("rands(") + args + std::string(")");
}

SCADModels::RegistryItemPtr
SCADModels::f_import_dxf_dim()
{
    auto model = std::make_unique<BaseSCADModel>("dxf_dim", "Read DXF Dimension");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "name"), "name");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "origin"), "origin");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "scale"), "scale");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_import_dxf_dim_process);
    return model;
}
void
SCADModels::f_import_dxf_dim_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["out"] = std::string("dxf_dim(") +
	std::string("file=") + FIND(input, "file", "0") + "," +
	std::string("name=") + FIND(input, "name", "0") + "," +
	std::string("layer=") + FIND(input, "layer", "0") + "," +
	std::string("origin=") + FIND(input, "origin", "0") + "," +
	std::string("scale=") + FIND(input, "scale", "0") +
	std::string(")");

}

SCADModels::RegistryItemPtr
SCADModels::f_import_dxf_cross()
{
    auto model = std::make_unique<BaseSCADModel>("dxf_cross", "Read DXF Origin");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "file"), "file");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "layer"), "layer");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "origin"), "origin");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "scale"), "scale");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE, "out"), "out");
    model->setProcessor(f_import_dxf_cross_process);
    return model;
}
void
SCADModels::f_import_dxf_cross_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["out"] = std::string("dxf_cross(") +
	std::string("file=") + FIND(input, "file", "0") + "," +
	std::string("layer=") + FIND(input, "layer", "0") + "," +
	std::string("origin=") + FIND(input, "origin", "0") + "," +
	std::string("scale=") + FIND(input, "scale", "0") +
	std::string(")");
}

// Boolean Constants
SCADModels::RegistryItemPtr
SCADModels::f_const_true()
{
    auto model = std::make_unique<BaseSCADModel>("const_true", "True");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_true_process);
    return model;
}
void
SCADModels::f_const_true_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("true");
}


SCADModels::RegistryItemPtr
SCADModels::f_const_false()
{
    auto model = std::make_unique<BaseSCADModel>("const_false", "False");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_false_process);
    return model;
}
void
SCADModels::f_const_false_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("false");
}

// Other Constants
SCADModels::RegistryItemPtr
SCADModels::f_const_int()
{
    auto model = std::make_unique<BaseSCADModel>("const_int", "Integer");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_int_process);
    return model;
}
void
SCADModels::f_const_int_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("15");
}


SCADModels::RegistryItemPtr
SCADModels::f_const_float()
{
    auto model = std::make_unique<BaseSCADModel>("const_float", "Float");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_float_process);
    return model;
}
void
SCADModels::f_const_float_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("15.0");
}


SCADModels::RegistryItemPtr
SCADModels::f_const_string()
{
    auto model = std::make_unique<BaseSCADModel>("const_string", "String");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_string_process);
    return model;
}
void
SCADModels::f_const_string_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("\"constant string\"");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_undef()
{
    auto model = std::make_unique<BaseSCADModel>("const_undef", "Undefined");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_undef_process);
    return model;
}
void
SCADModels::f_const_undef_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("undef");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_pi()
{
    auto model = std::make_unique<BaseSCADModel>("PI", "PI");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_pi_process);
    return model;
}
void
SCADModels::f_const_pi_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("PI");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_version()
{
    auto model = std::make_unique<BaseSCADModel>("version", "Version");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_version_process);
    return model;
}
void
SCADModels::f_const_version_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("version()");
}
SCADModels::RegistryItemPtr
SCADModels::f_const_version_num()
{
    auto model = std::make_unique<BaseSCADModel>("version_num", "Version Number");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_VARIABLE), "value");
    model->setProcessor(f_const_pi_process);
    return model;
}
void
SCADModels::f_const_version_num_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("version_num()");
}

SCADModels::RegistryItemPtr
SCADModels::f_output()
{
    auto model = std::make_unique<BaseSCADModel>("output", "Output");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY), "out");
    model->setProcessor(f_output_process);
    return model;
}
void
SCADModels::f_output_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    std::string s = FIND(input, "out", "//No Geometry Output\n");
    fprintf(stderr, "Output geometry %s\n", s.c_str());
    for (const auto & it : input) {
	fprintf(stderr, "Output map is %s -> %s\n", it.first.c_str(), it.second.c_str());
    }
    output["out"] = s;
}

#define UNARY_FUNCTION_BODY(prefix, name, description, input, atype, output, outtype, body) \
SCADModels::RegistryItemPtr                                                     \
SCADModels::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<BaseSCADModel>(#name, description);           \
    model->addInputPort(std::make_unique<BaseSCADPort>(atype), input);          \
    model->addOutputPort(std::make_unique<BaseSCADPort>(outtype), output);      \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
SCADModels::f_##prefix##_##name##_process(                                      \
    const BaseSCADModel & model,                                                \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                \
{                                                                               \
    _output[output] = body;                                                     \
}

#define BINARY_FUNCTION_BODY(prefix, name, description, a, atype, b, btype, output, outtype, body) \
SCADModels::RegistryItemPtr                                                     \
SCADModels::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<BaseSCADModel>(#name, description);           \
    model->addInputPort(std::make_unique<BaseSCADPort>(atype), a);              \
    model->addInputPort(std::make_unique<BaseSCADPort>(btype), b);              \
    model->addOutputPort(std::make_unique<BaseSCADPort>(outtype), output);      \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
SCADModels::f_##prefix##_##name##_process(                                      \
    const BaseSCADModel & model,                                                \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                \
{                                                                               \
    _output[output] = body;                                                     \
}

UNARY_FUNCTION_BODY(math, asin, "Arc Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("asin(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sin, "Sine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sin(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, acos, "Arc Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("acos(") + FIND(_input, "x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, cos, "Cosine", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("cos(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, abs, "Absolute Value", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("abs(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, atan, "Arc Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("atan(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, tan, "Tangent", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("tan(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sign, "Sign", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sign(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ceil, "Ceiling", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ceil(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, floor, "Floor", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("floor(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, round, "Round", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("round(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ln, "Natural Log", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ln(") + FIND(_input, "x", "2.718") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, log, "Log Base 10", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("log(") + FIND(_input, "x", "10") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, exp, "Exponential(e)", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("exp(") + FIND(_input, "x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, sqrt, "Square Root", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("sqrt(") + FIND(_input, "x", "1") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, norm, "Norm", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("norm(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, len, "Length", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("len(") + FIND(_input, "x", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_bool, "Is Boolean", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_bool(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_string, "Is String", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_string(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_num, "Is Number", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_num(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_function, "Is Function", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_function") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_list, "Is List", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_list") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, is_undef, "Is Undefined", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("is_undef") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, str, "To String", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("str(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, chr, "To Char", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("chr(") + FIND(_input, "x", "true") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, ord, "To Ordinal", "x", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("ord(") + FIND(_input, "x", "true") + std::string(")"))
    )


BINARY_FUNCTION_BODY(math, max, "Maximum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("max(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, pow, "Power", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("pow(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, min, "Minimum", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("min(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, concat, "Concatenate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("concat(") + FIND(_input, "a", "0") + "," + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, atan2, "Arc Tan(2)", "x", DATA_VARIABLE, "y", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("atan2(") + FIND(_input, "x", "0") + "," + FIND(_input, "y", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, cross, "Cross Product", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
     (std::string("cross(") + FIND(_input, "a", "[1,0,0]") + "," + FIND(_input, "b", "[0,1,0]") + std::string(")"))
    )

BINARY_FUNCTION_BODY(math, add, "Add", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "+" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, subtract, "Subtract", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "-" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, multiply, "Multiply", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "*" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, divide, "Divide", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "/" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, modulo, "Modulo", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "%" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, exponentiate, "Exponentiate", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "^" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, lt, "Less Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "<" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, leq, "Less Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "<=" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, eq, "Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "==" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, geq, "Greater Than or Equal", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + ">=" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, gt, "Greater Than", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + ">" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, and, "And", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "&&" + FIND(_input, "b", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, or, "Or", "a", DATA_VARIABLE, "b", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(") + FIND(_input, "a", "0") + "||" + FIND(_input, "b", "0") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, not, "Greater Than", "a", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("(!") + FIND(_input, "a", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, lookup, "Lookup", "value", DATA_VARIABLE, "table", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("lookup(") + FIND(_input, "value", "0") + ", " + FIND(_input, "table", "0") + std::string(")"))
    )
BINARY_FUNCTION_BODY(math, search, "Search for value", "needle", DATA_VARIABLE, "haystack", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("search(") + FIND(_input, "needle", "0") + ", " + FIND(_input, "haystack", "[]") + std::string(")"))
    )
UNARY_FUNCTION_BODY(math, parent_module, "Parent Module Name", "index", DATA_VARIABLE, "out", DATA_VARIABLE,
    (std::string("parent_module(") + FIND(_input, "needle", "0") + std::string(")"))
    )

//    ret->registerModel(SCADModels::f_math_rands, "Math");


std::shared_ptr<QtNodes::NodeDelegateModelRegistry>
SCADModels::registerDataModels()
{
    auto ret = std::make_shared<QtNodes::NodeDelegateModelRegistry>();
    
    ret->registerModel(SCADModels::f_prim_sphere, "Primitives");
    ret->registerModel(SCADModels::f_prim_cube, "Primitives");
    ret->registerModel(SCADModels::f_prim_cylinder, "Primitives");
    
    ret->registerModel(SCADModels::f_op_union, "Boolean Operators");
    ret->registerModel(SCADModels::f_op_difference, "Boolean Operators");
    ret->registerModel(SCADModels::f_op_intersection, "Boolean Operators");
    ret->registerModel(SCADModels::f_op_hull, "Boolean Operators");
    ret->registerModel(SCADModels::f_op_minkowski, "Boolean Operators");

    ret->registerModel(SCADModels::f_util_color, "Utility");
    
    ret->registerModel(SCADModels::f_const_true, "Constants");
    ret->registerModel(SCADModels::f_const_false, "Constants");
    ret->registerModel(SCADModels::f_const_int, "Constants");
    ret->registerModel(SCADModels::f_const_float, "Constants");
    ret->registerModel(SCADModels::f_const_string, "Constants");
    ret->registerModel(SCADModels::f_const_undef, "Constants");
    ret->registerModel(SCADModels::f_const_pi, "Constants");
    ret->registerModel(SCADModels::f_const_version, "Constants");
    ret->registerModel(SCADModels::f_const_version_num, "Constants");

    ret->registerModel(SCADModels::f_output, "Output");

    // Flow control
    //ret->registerModel(SCADModels::f_flow_for);
    ret->registerModel(SCADModels::f_flow_if, "Flow Control");
    //ret->registerModel(SCADModels::f_flow_let);
    //ret->registerModel(SCADModels::f_flow_group);

    // Transformations
    ret->registerModel(SCADModels::f_xform_translate, "Transform");
    //ret->registerModel(SCADModels::f_xform_offset, "Transform");
    ret->registerModel(SCADModels::f_xform_scale, "Transform");
    //ret->registerModel(SCADModels::f_xform_rotate, "Transform");
    ret->registerModel(SCADModels::f_xform_mirror, "Transform");
    ret->registerModel(SCADModels::f_xform_resize, "Transform");
    //ret->registerModel(SCADModels::f_xform_multmatrix, "Transform");

    // Math functions:
    
    ret->registerModel(SCADModels::f_math_asin, "Math");
    ret->registerModel(SCADModels::f_math_sin, "Math");
    ret->registerModel(SCADModels::f_math_acos, "Math");
    ret->registerModel(SCADModels::f_math_abs, "Math");
    ret->registerModel(SCADModels::f_math_atan, "Math");
    ret->registerModel(SCADModels::f_math_atan2, "Math");
    ret->registerModel(SCADModels::f_math_cos, "Math");
    ret->registerModel(SCADModels::f_math_tan, "Math");
    ret->registerModel(SCADModels::f_math_sign, "Math");
    ret->registerModel(SCADModels::f_math_ceil, "Math");
    ret->registerModel(SCADModels::f_math_floor, "Math");
    ret->registerModel(SCADModels::f_math_round, "Math");
    ret->registerModel(SCADModels::f_math_ln, "Math");
    ret->registerModel(SCADModels::f_math_len, "Math");
    ret->registerModel(SCADModels::f_math_log, "Math");
    ret->registerModel(SCADModels::f_math_exp, "Math");
    ret->registerModel(SCADModels::f_math_sqrt, "Math");
    ret->registerModel(SCADModels::f_math_concat, "Math");
    ret->registerModel(SCADModels::f_math_min, "Math");
    ret->registerModel(SCADModels::f_math_pow, "Math");
    ret->registerModel(SCADModels::f_math_max, "Math");
    ret->registerModel(SCADModels::f_math_norm, "Math");
    ret->registerModel(SCADModels::f_math_str, "Math");
    ret->registerModel(SCADModels::f_math_chr, "Math");
    ret->registerModel(SCADModels::f_math_ord, "Math");
    ret->registerModel(SCADModels::f_math_cross, "Math");

    ret->registerModel(SCADModels::f_math_add, "Math");
    ret->registerModel(SCADModels::f_math_subtract, "Math");
    ret->registerModel(SCADModels::f_math_multiply, "Math");
    ret->registerModel(SCADModels::f_math_divide, "Math");
    ret->registerModel(SCADModels::f_math_modulo, "Math");
    ret->registerModel(SCADModels::f_math_exponentiate, "Math");
    ret->registerModel(SCADModels::f_math_lt, "Math");
    ret->registerModel(SCADModels::f_math_leq, "Math");
    ret->registerModel(SCADModels::f_math_eq, "Math");
    ret->registerModel(SCADModels::f_math_geq, "Math");
    ret->registerModel(SCADModels::f_math_gt, "Math");
    ret->registerModel(SCADModels::f_math_and, "Math");
    ret->registerModel(SCADModels::f_math_or, "Math");
    ret->registerModel(SCADModels::f_math_not, "Math");
    ret->registerModel(SCADModels::f_math_lookup, "Math");
    ret->registerModel(SCADModels::f_math_search, "Math");
    ret->registerModel(SCADModels::f_math_parent_module, "Math");
    
    ret->registerModel(SCADModels::f_math_is_bool, "Math");
    ret->registerModel(SCADModels::f_math_is_string, "Math");
    ret->registerModel(SCADModels::f_math_is_num, "Math");
    ret->registerModel(SCADModels::f_math_is_function, "Math");
    ret->registerModel(SCADModels::f_math_is_list, "Math");
    ret->registerModel(SCADModels::f_math_is_undef, "Math");

    ret->registerModel(SCADModels::f_math_rands, "Math");

    ret->registerModel(SCADModels::f_import_dxf_dim);
    ret->registerModel(SCADModels::f_import_dxf_cross);
    
    return ret;
}
