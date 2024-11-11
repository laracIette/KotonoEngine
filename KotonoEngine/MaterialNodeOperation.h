#pragma once
#include "MaterialNode.h"
template <const char __operator>
class MaterialNodeOperation :
    public MaterialNode
{
public:
    MaterialNodeOperation();

    const std::string GetDefaultValue() const;
    const std::string GetShaderCode() const override;
    
    void AddInput();

private:
    std::string _type;
};

typedef MaterialNodeOperation<'+'> MaterialNodeOperationAdd;
typedef MaterialNodeOperation<'-'> MaterialNodeOperationSubstract;
typedef MaterialNodeOperation<'*'> MaterialNodeOperationMultiply;
typedef MaterialNodeOperation<'/'> MaterialNodeOperationDivide;
typedef MaterialNodeOperation<'%'> MaterialNodeOperationModulo;