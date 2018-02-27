#pragma once

#include "Definitions.hpp"

class Record
{
public:
    explicit Record(ValueType aValue);
    ValueType value() const;
    std::string toString() const;
private:
    Record() : fValue(0) {}
    ValueType fValue;
};
