#include <sstream>
#include <string>
#include "Exceptions.hpp"
#include "Node.hpp"

LeafNotFoundException::LeafNotFoundException(KeyType aKey) : fKey{aKey} {}

const char* LeafNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Chave nao encontra nas folhas:  ";
    ss << fKey;
    static std::string message;
    message = ss.str();
    return message.c_str();
}


NodeNotFoundException::NodeNotFoundException(std::string aSearchedNode,
                                             std::string aContainingNode) :
fSearchedNode{aSearchedNode}, fContainingNode{aContainingNode}
{}

const char* NodeNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "No |" << fSearchedNode << "| nao encontrado";
    ss << " como filho no";
    ss << fContainingNode;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

RecordNotFoundException::RecordNotFoundException(KeyType aKey) : fKey{aKey} {}

const char* RecordNotFoundException::what() const noexcept
{
    std::ostringstream ss;
    ss << "Registro nao encontrado com a chave:  " << fKey;
    static std::string message;
    message = ss.str();
    return message.c_str();
}

