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

static const QtNodes::NodeDataType DATA_FLOAT{"float", "Float"};
static const QtNodes::NodeDataType DATA_INT{"integer", "Integer"};
static const QtNodes::NodeDataType DATA_BOOL{"bool", "Boolean"};
static const QtNodes::NodeDataType DATA_STRING{"string", "String"};
static const QtNodes::NodeDataType DATA_VECTOR2{"vector2", "Vector 2d"};
static const QtNodes::NodeDataType DATA_VECTOR3{"vector3", "Vector 3d"};
static const QtNodes::NodeDataType DATA_LIST{"list", "List"};
static const QtNodes::NodeDataType DATA_SOLID_GEOMETRY{"geometry", "Solid Geometry"};
static const QtNodes::NodeDataType DATA_PLANE_GEOMETRY{"plane", "Plane Geometry"};

std::string FIND(const PortFunctionData & input, std::string key, std::string def)
{
    const auto it = input.find(key);
    if (it == input.end()) {
	fprintf(stderr, "Could not find key %s returning default\n", key.c_str());
	return def;
    }
    return it->second;
}

// Primitives
SCADModels::RegistryItemPtr
SCADModels::f_prim_sphere()
{
    auto model = std::make_unique<BaseSCADModel>("prim_sphere", "Sphere");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT, "r"), "r");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_BOOL, "is_diameter"), "is_diameter");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_SOLID_GEOMETRY, "Geometry"), "Geometry");
    model->setProcessor(f_prim_sphere_process);
    return model;
}

void
SCADModels::f_prim_sphere_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["Geometry"] = std::string("        sphere(") + std::string("r=") + FIND(input, "r", "15.0") + std::string(");\n");
}


SCADModels::RegistryItemPtr
SCADModels::f_prim_cube()
{
    auto model = std::make_unique<BaseSCADModel>("prim_cube", "Cube");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT, "x"), "x");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT, "y"), "y");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT, "z"), "z");
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
    auto model = std::make_unique<BaseSCADModel>("prim_cylinder", "Cylinder");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT, "r"), "r");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT, "h"), "h");
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
    auto model = std::make_unique<BaseSCADModel>("op_union", "Union");
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
SCADModels::f_op_difference()
{
    auto model = std::make_unique<BaseSCADModel>("op_difference", "Difference");
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
    auto model = std::make_unique<BaseSCADModel>("op_intersection", "Intersection");
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

// Transformations
//    static RegistryItemPtr f_xform_translate();
//    static RegistryItemPtr f_xform_rotate_euler();
//    static RegistryItemPtr f_xform_rotate_axis_angle();
//    static RegistryItemPtr f_xform_rotate_quat();
    
    // Numerics
//    static RegistryItemPtr f_math_modulus();
//    static RegistryItemPtr f_math_add();
//    static RegistryItemPtr f_math_sub();
//    static RegistryItemPtr f_math_mul();
//    static RegistryItemPtr f_math_div();
//    static RegistryItemPtr f_math_sin();
//    static RegistryItemPtr f_math_cos();
//    static RegistryItemPtr f_math_asin();
//    static RegistryItemPtr f_math_acos();

    // Vector3
//    static RegistryItemPtr f_vector3_dot();
//    static RegistryItemPtr f_vector3_cross();
//    static RegistryItemPtr f_vector3_scale();
//    static RegistryItemPtr f_vector3_length();
//    static RegistryItemPtr f_vector3_decompose();
//    static RegistryItemPtr f_vector3_compose();


// Boolean Constants
SCADModels::RegistryItemPtr
SCADModels::f_const_true()
{
    auto model = std::make_unique<BaseSCADModel>("const_true", "True");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_BOOL), "value");
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
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_BOOL), "value");
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
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_INT), "value");
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
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT), "value");
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
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_STRING), "value");
    model->setProcessor(f_const_string_process);
    return model;
}
void
SCADModels::f_const_string_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output)
{
    output["value"] = std::string("\"constant string\"");
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

    ret->registerModel(SCADModels::f_const_true, "Constants");
    ret->registerModel(SCADModels::f_const_false, "Constants");
    ret->registerModel(SCADModels::f_const_int, "Constants");
    ret->registerModel(SCADModels::f_const_float, "Constants");
    ret->registerModel(SCADModels::f_const_string, "Constants");

    ret->registerModel(SCADModels::f_output, "Output");
    
    return ret;
}
