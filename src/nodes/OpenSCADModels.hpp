#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtNodes/NodeData>

class BaseSCADPort {
public:
    BaseSCADPort(QtNodes::NodeDataType type, QString portCaption);
    BaseSCADPort(QtNodes::NodeDataType type);
    virtual ~BaseSCADPort();

    QtNodes::NodeDataType nodeDataType() const;
    QWidget *portWidget();
    void setData(std::shared_ptr<QtNodes::NodeData> data);
    std::shared_ptr<QtNodes::NodeData> getData(void) const;
    
    QString portCaption() const;
    bool portCaptionVisible() const;
private:
    QString _caption;
    QtNodes::NodeDataType _type;
    std::shared_ptr<QtNodes::NodeData> _data;
};

typedef std::map<std::string, std::string> PortFunctionData;

class BaseSCADModel;

typedef std::function<void(const BaseSCADModel & model, const PortFunctionData &, PortFunctionData & )> NodeProcessor;

/// The model dictates the number of inputs and outputs for the Node.
/// In this example it has no logic.
class BaseSCADModel : public QtNodes::NodeDelegateModel {
public:
    BaseSCADModel(std::string name, std::string caption);

    unsigned int nPorts(QtNodes::PortType portType) const override;

    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;

    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex port) override;

    void setInData(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex portIndex) override;

    QWidget *embeddedWidget() override { return nullptr; }

    QString name() const override;
    QString caption() const override;
//    void compute() override;
    void addInputPort(std::unique_ptr<BaseSCADPort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<BaseSCADPort> outputPort, std::string outputPortName);
    std::string inputPortName(QtNodes::PortIndex portIndex) const;
    std::string outputPortName(QtNodes::PortIndex portIndex) const;
    
    void setProcessor(NodeProcessor processor);

    void process(const PortFunctionData & input, PortFunctionData & output) const;

    static void default_processor(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);

protected:
    std::string _name;
    std::string _caption;
    std::vector<std::unique_ptr<BaseSCADPort>> _inputPorts;
    std::vector<std::unique_ptr<BaseSCADPort>> _outputPorts;
    std::map<QtNodes::PortIndex, std::string> _inputPortNames;
    std::map<QtNodes::PortIndex, std::string> _outputPortNames;
    NodeProcessor _processor;
    
//    std::weak_ptr<DecimalData> _number1;
//    std::weak_ptr<DecimalData> _number2;
//    std::shared_ptr<DecimalData> _result;
};

class SCADModels {
public:
    using RegistryItemPtr = QtNodes::NodeDelegateModelRegistry::RegistryItemPtr;

    static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();
    std::string f_default_process(const PortFunctionData & input, PortFunctionData & output);
    
    // Primitives
    static RegistryItemPtr f_prim_sphere();
    static void f_prim_sphere_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    
    static RegistryItemPtr f_prim_cube();
    static void f_prim_cube_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    
    static RegistryItemPtr f_prim_cylinder();
    static void f_prim_cylinder_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);

    // Boolean operations
    static RegistryItemPtr f_op_union();
    static void f_op_union_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static RegistryItemPtr f_op_difference();
    static void f_op_difference_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static RegistryItemPtr f_op_intersection();
    static void f_op_intersection_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);

    // Transformations
    static RegistryItemPtr f_xform_translate();
    static RegistryItemPtr f_xform_rotate_euler();
    static RegistryItemPtr f_xform_rotate_axis_angle();
    static RegistryItemPtr f_xform_rotate_quat();
    
    // Numerics
    static RegistryItemPtr f_math_modulus();
    static RegistryItemPtr f_math_add();
    static RegistryItemPtr f_math_sub();
    static RegistryItemPtr f_math_mul();
    static RegistryItemPtr f_math_div();
    static RegistryItemPtr f_math_sin();
    static RegistryItemPtr f_math_cos();
    static RegistryItemPtr f_math_asin();
    static RegistryItemPtr f_math_acos();

    // Vector3
    static RegistryItemPtr f_vector3_dot();
    static RegistryItemPtr f_vector3_cross();
    static RegistryItemPtr f_vector3_scale();
    static RegistryItemPtr f_vector3_length();
    static RegistryItemPtr f_vector3_decompose();
    static RegistryItemPtr f_vector3_compose();

    // Literal Constants
    static RegistryItemPtr f_const_true();
    static void f_const_true_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static RegistryItemPtr f_const_false();
    static void f_const_false_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static RegistryItemPtr f_const_int();
    static void f_const_int_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static RegistryItemPtr f_const_float();
    static void f_const_float_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static RegistryItemPtr f_const_string();
    static void f_const_string_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);

    // Outputs
    static RegistryItemPtr f_output();
    static void f_output_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
};
