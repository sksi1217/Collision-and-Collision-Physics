#pragma once
#include <typeinfo>

class Component {
public:
    virtual ~Component() = default;
    virtual const std::type_info& GetStoredType() const {
        return typeid(*this);
    }
};