
#define UNARY_FUNCTION_BODY(prefix, name, description, input, atype, output, outtype, body) \
OpenSCADBuiltins::RegistryItemPtr                                                     \
OpenSCADBuiltins::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, "Math");   \
    model->addInputPort(std::make_unique<NodeModelPort>(atype), input);         \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), output);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
OpenSCADBuiltins::f_##prefix##_##name##_process(                                      \
    const OpenSCADBuiltinModel & model,                                                 \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                \
{                                                                               \
    _output.setValue(output, body);					\
}

#define BINARY_FUNCTION_BODY(prefix, name, description, a, atype, b, btype, output, outtype, body) \
OpenSCADBuiltins::RegistryItemPtr                                                     \
OpenSCADBuiltins::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, "Math");   \
    model->addInputPort(std::make_unique<NodeModelPort>(atype), a);             \
    model->addInputPort(std::make_unique<NodeModelPort>(btype), b);             \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), output);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
OpenSCADBuiltins::f_##prefix##_##name##_process(                                      \
    const OpenSCADBuiltinModel & model,                                                 \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                \
{                                                                               \
    _output.setValue(output, body);					\
}

#define NONARY_NODE(prefix, name, description, category, output, outtype) \
OpenSCADBuiltins::RegistryItemPtr                                                     \
OpenSCADBuiltins::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, category);   \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), output);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
OpenSCADBuiltins::f_##prefix##_##name##_process(                                      \
    const OpenSCADBuiltinModel & model,                                                 \
    const PortFunctionData & _input,                                            \
    PortFunctionData & _output                                                  \
    )						                                

