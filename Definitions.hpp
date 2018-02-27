#pragma once



const int DEFAULT_ORDER{5}; // 5 * 4bytes = 20 bytes
//colocar como cin

// A ordem mínima é necessariamente 3. Pode-se definir até 20
// por padrao deixamos ordem 4.
const int MIN_ORDER{DEFAULT_ORDER - 1};
const int MAX_ORDER{20};

using KeyType = int64_t;
using ValueType = std::string;

