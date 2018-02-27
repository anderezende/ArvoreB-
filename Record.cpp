#include <string>
#include <sstream>
#include "Definitions.hpp"
#include "Record.hpp"

Record::Record(ValueType aValue)
: fValue(aValue)
{}

ValueType Record::value() const
{
    return fValue;
}

std::string Record::toString() const
{
    std::ostringstream oss;
    oss << fValue;
    return oss.str();
}
