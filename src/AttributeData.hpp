#pragma once

#include "VertexFormat.hpp"

//------------------------------------------------------------------------

class AttributeData {
public:
    virtual VertexFormat getVertexFormat() const noexcept = 0;

protected:
    AttributeData() = default;
};

//------------------------------------------------------------------------
