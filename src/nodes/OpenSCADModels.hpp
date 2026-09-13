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

class Receiver : public QObject {
    Q_OBJECT
public:
//    Receiver();
//    ~Receiver();
Q_SIGNALS:
    void somethingWasSaid(QtNodes::NodeId nodeId, std::string message);
};

class BaseSCADModel;

typedef std::function<void(const BaseSCADModel & model, const PortFunctionData &, PortFunctionData & )> NodeProcessor;
typedef std::function<QWidget*(BaseSCADModel *)> WidgetFactory;

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

    QString name() const override;
    QString caption() const override;

    QWidget *embeddedWidget() override;

    void addInputPort(std::unique_ptr<BaseSCADPort> inputPort, std::string inputPortName);
    void addOutputPort(std::unique_ptr<BaseSCADPort> outputPort, std::string outputPortName);
    std::string inputPortName(QtNodes::PortIndex portIndex) const;
    std::string outputPortName(QtNodes::PortIndex portIndex) const;
    
    void setProcessor(NodeProcessor processor);
    void setWidgetFactory(WidgetFactory widgetFactory);

    void process(const PortFunctionData & input, PortFunctionData & output) const;

    static void default_processor(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
    static QWidget *default_widget_factory(BaseSCADModel *model);

    void setReceiver(Receiver *receiver);
    Receiver *getReceiver() const;

    void somethingWasSaid(QtNodes::NodeId nodeId, std::string message);
    
protected:
    std::string _name;
    std::string _caption;
    std::vector<std::unique_ptr<BaseSCADPort>> _inputPorts;
    std::vector<std::unique_ptr<BaseSCADPort>> _outputPorts;
    std::map<QtNodes::PortIndex, std::string> _inputPortNames;
    std::map<QtNodes::PortIndex, std::string> _outputPortNames;
    NodeProcessor _processor;
    WidgetFactory _widgetFactory;
    QWidget *_widget;
    Receiver *_receiver;
    
//    std::weak_ptr<DecimalData> _number1;
//    std::weak_ptr<DecimalData> _number2;
//    std::shared_ptr<DecimalData> _result;
};

class SCADModels {
public:
    using RegistryItemPtr = QtNodes::NodeDelegateModelRegistry::RegistryItemPtr;

    static std::shared_ptr<QtNodes::NodeDelegateModelRegistry> registerDataModels();
    std::string f_default_process(const PortFunctionData & input, PortFunctionData & output);
    
    // Math
#define _OPENSCAD_NODE_PROCESSOR_PROTOTYPE const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output
#define _OPENSCAD_NODE_DECL(name)                                    \
    static RegistryItemPtr f_##name();                               \
    static void f_##name##_process(_OPENSCAD_NODE_PROCESSOR_PROTOTYPE);

    // 3d Primitives
    _OPENSCAD_NODE_DECL(prim_sphere);
    _OPENSCAD_NODE_DECL(prim_cube);
    _OPENSCAD_NODE_DECL(prim_cylinder);
    _OPENSCAD_NODE_DECL(prim_polyhedron);
    _OPENSCAD_NODE_DECL(prim_linear_extrude);
    _OPENSCAD_NODE_DECL(prim_rotate_extrude);
    _OPENSCAD_NODE_DECL(prim_fill);

    // 2d primitives
    _OPENSCAD_NODE_DECL(prim_circle);
    _OPENSCAD_NODE_DECL(prim_polygon);
    _OPENSCAD_NODE_DECL(prim_surface);
    _OPENSCAD_NODE_DECL(prim_square);
    
    // Boolean operations
    _OPENSCAD_NODE_DECL(op_union);
    _OPENSCAD_NODE_DECL(op_difference);
    _OPENSCAD_NODE_DECL(op_intersection);
    _OPENSCAD_NODE_DECL(op_hull);
    _OPENSCAD_NODE_DECL(op_minkowski);

    // Special module operations
    _OPENSCAD_NODE_DECL(mod_children);

    // Debug/utility
    _OPENSCAD_NODE_DECL(util_echo);
    _OPENSCAD_NODE_DECL(util_assert);
    _OPENSCAD_NODE_DECL(util_render);
    _OPENSCAD_NODE_DECL(util_color);

    // Flow control
    _OPENSCAD_NODE_DECL(flow_for);
    static QWidget* f_flow_for_widget(BaseSCADModel *model);
    
    _OPENSCAD_NODE_DECL(flow_if);
    _OPENSCAD_NODE_DECL(flow_let);
    _OPENSCAD_NODE_DECL(flow_group);

    // Transformations
    _OPENSCAD_NODE_DECL(xform_translate);
    _OPENSCAD_NODE_DECL(xform_offset);
    _OPENSCAD_NODE_DECL(xform_scale);
    _OPENSCAD_NODE_DECL(xform_rotate);
    _OPENSCAD_NODE_DECL(xform_mirror);
    _OPENSCAD_NODE_DECL(xform_resize);
    _OPENSCAD_NODE_DECL(xform_multmatrix);
    
    _OPENSCAD_NODE_DECL(math_asin);
    _OPENSCAD_NODE_DECL(math_sin);
    _OPENSCAD_NODE_DECL(math_acos);
    _OPENSCAD_NODE_DECL(math_cos);
    _OPENSCAD_NODE_DECL(math_abs);
    _OPENSCAD_NODE_DECL(math_atan);
    _OPENSCAD_NODE_DECL(math_atan2);
    _OPENSCAD_NODE_DECL(math_tan);
    _OPENSCAD_NODE_DECL(math_sign);
    _OPENSCAD_NODE_DECL(math_ceil);
    _OPENSCAD_NODE_DECL(math_floor);
    _OPENSCAD_NODE_DECL(math_round);
    _OPENSCAD_NODE_DECL(math_ln);
    _OPENSCAD_NODE_DECL(math_log);
    _OPENSCAD_NODE_DECL(math_exp);
    _OPENSCAD_NODE_DECL(math_pow);
    _OPENSCAD_NODE_DECL(math_sqrt);
    _OPENSCAD_NODE_DECL(math_min);
    _OPENSCAD_NODE_DECL(math_max);
    _OPENSCAD_NODE_DECL(math_concat);
    _OPENSCAD_NODE_DECL(math_norm);
    _OPENSCAD_NODE_DECL(math_len);
    _OPENSCAD_NODE_DECL(math_str);
    _OPENSCAD_NODE_DECL(math_chr);
    _OPENSCAD_NODE_DECL(math_ord);
    _OPENSCAD_NODE_DECL(math_cross);

    _OPENSCAD_NODE_DECL(math_add);
    _OPENSCAD_NODE_DECL(math_subtract);
    _OPENSCAD_NODE_DECL(math_multiply);
    _OPENSCAD_NODE_DECL(math_divide);
    _OPENSCAD_NODE_DECL(math_modulo);
    _OPENSCAD_NODE_DECL(math_exponentiate);
    _OPENSCAD_NODE_DECL(math_lt);
    _OPENSCAD_NODE_DECL(math_leq);
    _OPENSCAD_NODE_DECL(math_eq);
    _OPENSCAD_NODE_DECL(math_geq);
    _OPENSCAD_NODE_DECL(math_gt);
    _OPENSCAD_NODE_DECL(math_and);
    _OPENSCAD_NODE_DECL(math_or);
    _OPENSCAD_NODE_DECL(math_not);
    
    _OPENSCAD_NODE_DECL(math_is_bool);
    _OPENSCAD_NODE_DECL(math_is_string);
    _OPENSCAD_NODE_DECL(math_is_num);
    _OPENSCAD_NODE_DECL(math_is_function);
    _OPENSCAD_NODE_DECL(math_is_list);
    _OPENSCAD_NODE_DECL(math_is_undef);

    // Literal Constants
    _OPENSCAD_NODE_DECL(const_true);
    _OPENSCAD_NODE_DECL(const_false);
    _OPENSCAD_NODE_DECL(const_int);
    _OPENSCAD_NODE_DECL(const_float);
    _OPENSCAD_NODE_DECL(const_string);
    _OPENSCAD_NODE_DECL(const_undef);
    _OPENSCAD_NODE_DECL(const_version);
    _OPENSCAD_NODE_DECL(const_version_num);
    _OPENSCAD_NODE_DECL(const_pi);

    _OPENSCAD_NODE_DECL(math_rands);
    _OPENSCAD_NODE_DECL(math_lookup);
    _OPENSCAD_NODE_DECL(math_search);
    _OPENSCAD_NODE_DECL(math_parent_module);
    
    _OPENSCAD_NODE_DECL(import_dxf_dim);
    _OPENSCAD_NODE_DECL(import_dxf_cross);

#undef _OPENSCAD_NODE_DECL
#undef _OPENSCAD_NODE_PROCESSOR_PROTOTYPE

    // Outputs
    static RegistryItemPtr f_output();
    static void f_output_process(const BaseSCADModel & model, const PortFunctionData & input, PortFunctionData & output);
};
