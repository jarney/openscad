#pragma once

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>
#include <QtNodes/NodeData>
#include "nodes/OpenSCADDataTypes.hpp"

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
    
    // Math
#define FUNCTION_PROCESSOR_PROTOTYPE const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output
#define FUNCTION_DECL(prefix, name)                                          \
    static RegistryItemPtr f_##prefix##_##name();                            \
    static void f_##prefix##_##name##_process(FUNCTION_PROCESSOR_PROTOTYPE);

    FUNCTION_DECL(math, asin);
    FUNCTION_DECL(math, sin);
    FUNCTION_DECL(math, acos);
    FUNCTION_DECL(math, cos);
    FUNCTION_DECL(math, abs);
    FUNCTION_DECL(math, atan);
    FUNCTION_DECL(math, atan2);
    FUNCTION_DECL(math, tan);
    FUNCTION_DECL(math, sign);
    FUNCTION_DECL(math, ceil);
    FUNCTION_DECL(math, floor);
    FUNCTION_DECL(math, round);
    FUNCTION_DECL(math, ln);
    FUNCTION_DECL(math, log);
    FUNCTION_DECL(math, exp);
    FUNCTION_DECL(math, pow);
    FUNCTION_DECL(math, sqrt);
    FUNCTION_DECL(math, min);
    FUNCTION_DECL(math, max);
    FUNCTION_DECL(math, concat);
    FUNCTION_DECL(math, norm);
    FUNCTION_DECL(math, len);
    FUNCTION_DECL(math, str);
    FUNCTION_DECL(math, chr);
    FUNCTION_DECL(math, ord);
    FUNCTION_DECL(math, cross);

    FUNCTION_DECL(math, add);
    FUNCTION_DECL(math, subtract);
    FUNCTION_DECL(math, multiply);
    FUNCTION_DECL(math, divide);
    FUNCTION_DECL(math, modulo);
    FUNCTION_DECL(math, exponentiate);
    FUNCTION_DECL(math, lt);
    FUNCTION_DECL(math, leq);
    FUNCTION_DECL(math, eq);
    FUNCTION_DECL(math, geq);
    FUNCTION_DECL(math, gt);
    FUNCTION_DECL(math, and);
    FUNCTION_DECL(math, or);
    FUNCTION_DECL(math, not);
    
    FUNCTION_DECL(math, is_bool);
    FUNCTION_DECL(math, is_string);
    FUNCTION_DECL(math, is_num);
    FUNCTION_DECL(math, is_function);
    FUNCTION_DECL(math, is_list);
    FUNCTION_DECL(math, is_undef);

    // Literal Constants
    FUNCTION_DECL(const, true);
    FUNCTION_DECL(const, false);
    FUNCTION_DECL(const, int);
    FUNCTION_DECL(const, float);
    FUNCTION_DECL(const, string);
    FUNCTION_DECL(const, undef);
    FUNCTION_DECL(const, version);
    FUNCTION_DECL(const, version_num);
    FUNCTION_DECL(const, pi);

    FUNCTION_DECL(math, rands);
    FUNCTION_DECL(math, lookup);
    FUNCTION_DECL(math, search);
    FUNCTION_DECL(math, parent_module);
    
    FUNCTION_DECL(import, dxf_dim);
    FUNCTION_DECL(import, dxf_cross);

#undef FUNCTION_DECL
#undef FUNCTION_PROCESSOR_PROTOTYPE

    // Outputs
    static RegistryItemPtr f_output();
    static void f_output_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
};
