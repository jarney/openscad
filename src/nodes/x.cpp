SCADModels::RegistryItemPtr SCADModels::f_math_asin() {
    auto model = std::make_unique<BaseSCADModel>("asin", "Arc Sine");
    model->addInputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT), "x");
    model->addOutputPort(std::make_unique<BaseSCADPort>(DATA_FLOAT), "out");
    model->setProcessor(f_math_asin_process);
    return model;
}

void SCADModels::f_math_asin_process( const BaseSCADModel & model, const PortFunctionData & "x", PortFunctionData & _output )
{
    _output["out"] = (std::string("asin(") + FIND(input, "x", "0") + std::string(")"));
}
