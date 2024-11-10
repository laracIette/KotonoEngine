#pragma once
#include "MaterialNode.h"
template <const char __operator>
class MaterialNodeOperation :
    public MaterialNode
{
public:
    MaterialNodeOperation();

    std::string GetDefaultValue() const;
    std::string GetShaderCode() const override;
    
    void AddInput();

private:
    std::string _type;
};

using MaterialNodeOperationAdd = MaterialNodeOperation<'+'>;
using MaterialNodeOperationSubstract = MaterialNodeOperation<'-'>;
using MaterialNodeOperationMultiply = MaterialNodeOperation<'*'>;
using MaterialNodeOperationDivide = MaterialNodeOperation<'/'>;
using MaterialNodeOperationModulo = MaterialNodeOperation<'%'>;
