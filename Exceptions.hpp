
#pragma once

#include <exception>
#include <string>
#include "Definitions.hpp"

class LeafNotFoundException : public std::exception
{
public:
    explicit LeafNotFoundException(KeyType);
    const char* what() const noexcept override;
private:
    KeyType fKey;
};

class NodeNotFoundException : public std::exception
{
public:
    explicit NodeNotFoundException(std::string, std::string);
    const char* what() const noexcept override;
private:
    std::string fSearchedNode;
    std::string fContainingNode;
};

class RecordNotFoundException : public std::exception
{
public:
    explicit RecordNotFoundException(KeyType);
    const char* what() const noexcept override;
private:
    KeyType fKey;
};
