#define NONARY_NODE(prefix, name, category, description, outname, outtype) \
Builtins::RegistryItemPtr                                                     \
Builtins::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, category);   \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), outname);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
Builtins::f_##prefix##_##name##_process(                                      \
    const OpenSCADBuiltinModel & model,                                                 \
    const PortFunctionData & input,                                            \
    PortFunctionData & output                                                  \
    )						                                

#define UNARY_NODE(prefix, name, category, description, inname, atype, outname, outtype) \
Builtins::RegistryItemPtr                                                     \
Builtins::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, category);   \
    model->addInputPort(std::make_unique<NodeModelPort>(atype), inname);         \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), outname);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
Builtins::f_##prefix##_##name##_process(                                      \
    const OpenSCADBuiltinModel & model,                                                 \
    const PortFunctionData & input,                                            \
    PortFunctionData & output                                                  \
    )

#define BINARY_NODE(prefix, name, category, description, a, atype, b, btype, outname, outtype) \
Builtins::RegistryItemPtr                                                     \
Builtins::f_##prefix##_##name() {                                             \
    auto model = std::make_unique<NodeModelType>(#name, description, category);   \
    model->addInputPort(std::make_unique<NodeModelPort>(atype), a);             \
    model->addInputPort(std::make_unique<NodeModelPort>(btype), b);             \
    model->addOutputPort(std::make_unique<NodeModelPort>(outtype), outname);     \
    model->setProcessor(f_##prefix##_##name##_process);                         \
    return model;                                                               \
}                                                                               \
void									        \
Builtins::f_##prefix##_##name##_process(                                      \
    const OpenSCADBuiltinModel & model,                                                 \
    const PortFunctionData & input,                                            \
    PortFunctionData & output                                                  \
    )

