#pragma once

#include <tuple>
#include <vector>
#include "Definitions.hpp"
#include "Printer.hpp"

class InternalNode;
class LeafNode;
class Node;


/// Classe principal que fornece a API para a Árvore Interativa B +.
class BPlusTree
{
public:
    /// Único construtor. Aceita uma ordem opcional para a Árvore B +
    /// estrutura de dados e suas operações.
    explicit BPlusTree(int aOrder = DEFAULT_ORDER);

   /// O tipo usado na API para inserir um novo par de valores-chave na arvore. O terceiro item e o tipo do no em qual a chave sera inserida.
    using EntryType = std::tuple<KeyType, ValueType, LeafNode*>;

    /// Retorna verdadeiro se esta arvore B + nao possui chaves ou valores.
    bool isEmpty() const;

  /// Insira um par de valores-chave nesta arvore B +.
    void insert(KeyType aKey, ValueType aValue);

    /// Remova uma chave e seu valor desta arvore B +.
    void remove(KeyType aKey);

    ///Imprima esta arvore B + para stdout usando uma linha de comando simples esquema grafico ASCII @param [in] aVerbose Determina se a impressão deve incluir endereços
    void print(bool aVerbose = false);

    /* Imprima o ponto inferior desta árvore B +, que consiste em suas folhas.
      Isso mostra todas as chaves na árvore B + em ordem ordenada.
      @param [in] aVerbose Determina se a impressão deve incluir endereços. */
    void printLeaves(bool aVerbose = false);

    /* Imprimir o valor associado a uma determinada chave, juntamente com o endereço
      em que a árvore armazena esse valor.
     @param [in] aVerbose Determina se a impressão deve incluir endereços. */
    void printValue(KeyType aKey, bool aVerbose = false);

    /* Imprima o caminho da chave de roda para a roda de folha aKey.
      @param [in] aVerbose Determina se a impressão deve incluir endereços.*/
    void printPathTo(KeyType aKey, bool aVerbose = false);

    /* Imprimir a chave, o valor e o endereço para cada item no intervalo /// de Iniciar para e, incluindo ambos.*/
    void printRange(KeyType aStart, KeyType aEnd);

    /* Remover todos os elementos da árvore B +. Você pode então construir
     novamente, inserindo novos elementos nela.*/
    void destroyTree();

    /* Leia os elementos a serem inseridos na árvore B + a partir de um arquivo de texto.
      Cada novo elemento deve consistir de um único inteiro em uma linha por si só.
      Esta árvore B + trata cada entrada como um novo valor e a chave
      sob o qual armazená-lo.*/
void readInputFromFile(std::string aFileName);
private:
    void startNewTree(KeyType aKey, ValueType aValue);
    void insertIntoLeaf(KeyType aKey, ValueType aValue);
    void insertIntoParent(Node* aOldNode, KeyType aKey, Node* aNewNode);
    template <typename T> T* split(T* aNode);
    void removeFromLeaf(KeyType aKey);
    template <typename N> void coalesceOrRedistribute(N* aNode);
    template <typename N> void coalesce(N* aNeighborNode, N* aNode, InternalNode* aParent, int aIndex);
    template <typename N> void redistribute(N* aNeighborNode, N* aNode, InternalNode* aParent, int aIndex);
    void adjustRoot();
    LeafNode* findLeafNode(KeyType aKey, bool aPrinting = false, bool aVerbose = false);
    void printValue(KeyType aKey, bool aPrintPath, bool aVerbose);
    std::vector<EntryType> range(KeyType aStart, KeyType aEnd);
    const int fOrder;
    Node* fRoot;
    Printer fPrinter;
};
