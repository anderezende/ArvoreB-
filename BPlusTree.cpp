#include <iostream>
#include <fstream>
#include <string>
#include "BPlusTree.hpp"
#include "Exceptions.hpp"
#include "InternalNode.hpp"
#include "LeafNode.hpp"
#include "Node.hpp"

BPlusTree::BPlusTree(int aOrder) : fOrder{aOrder}, fRoot{nullptr} {}

//Verifica se a arvore esta vazia
bool BPlusTree::isEmpty() const
{
    return !fRoot;
}

// INSERCAO
//inserir um key de um valor qualquer
void BPlusTree::insert(KeyType aKey, ValueType aValue)
{
    if (isEmpty()) {
        startNewTree(aKey, aValue);
        //se for vazio comeca uma arvore vazia, onde a raiz é a propria folha
    } else {
        insertIntoLeaf(aKey, aValue);
        //se a arvore nao estiver vazia insere elementos na folha
    }
}
//funcao que comeca a arvore quando esta vazia no insert acima
void BPlusTree::startNewTree(KeyType aKey, ValueType aValue) {
    //Aloca um ponteiro como um objeto
    LeafNode* newLeafNode = new LeafNode(fOrder);
    //o ponteiro manda para o metodo criar e inserir registro
    newLeafNode->createAndInsertRecord(aKey, aValue);
    //a raiz tem um mesmo valor da folha EX: se tiver 5 na raiz, tera também na folha
    fRoot = newLeafNode;
}
//funcao para inserir na folha do insert acima caso a arvore ja tenha algum elemento.
void BPlusTree::insertIntoLeaf(KeyType aKey, ValueType aValue)
{
    //aloca um ponteiro recebendo uma funcao que encontra a folha do nodo recebendo uma chave
    LeafNode* leafNode = findLeafNode(aKey);
    //se nao for folha dispara uma excessao que fala que nao é folha
    if (!leafNode) {
        throw LeafNotFoundException(aKey);
    }
    //aloca um inteiro recebendo um ponteiro que aponta para a funcao de criar e inserir um registro
    int newSize = leafNode->createAndInsertRecord(aKey, aValue);
    //testa se o novo tamanho é > que o maxsize definido
	//aloca um ponteiro novo como nova folha e atribui o resultado de slip ja setando os valores para nova folha
	if (newSize > leafNode->maxSize()) {

        LeafNode* newLeaf = split(leafNode);
        newLeaf->setNext(leafNode->next());
        leafNode->setNext(newLeaf);
        KeyType newKey = newLeaf->firstKey();
		//insere no pai a chave, a nova chave e a nova folha
        insertIntoParent(leafNode, newKey, newLeaf);
    }
}
//insere no pai
void BPlusTree::insertIntoParent(Node *aOldNode, KeyType aKey, Node *aNewNode)
{
	//forca um ponteiro com o antigo pai ser um o pai atual.
    InternalNode* parent = static_cast<InternalNode*>(aOldNode->parent());
    //nullptr = Tipo constante do ponteiro nulo nullptr.
    if (parent == nullptr) {
		//novo no interno, raiz
        fRoot = new InternalNode(fOrder);
        //recebe um objeto estatico convertido
        parent = static_cast<InternalNode*>(fRoot);
        aOldNode->setParent(parent); //seta os valores antigos
        aNewNode->setParent(parent); //seta os valores novos
        parent->populateNewRoot(aOldNode, aKey, aNewNode); // popula a tupla do pai com o no antigo, a chave e o novo no
    } else {
        int newSize = parent->insertNodeAfter(aOldNode, aKey, aNewNode);
        if (newSize > parent->maxSize()) {
            InternalNode* newNode = split(parent); //template de split
            KeyType newKey = newNode->replaceAndReturnFirstKey(); //fatia e retorna a primeira chave
            insertIntoParent(parent, newKey, newNode); // insere no pai o proprio pai, a chave e o novo no
        }
    }
}

template <typename T>
T* BPlusTree::split(T* aNode)
{
    T* newNode = new T(fOrder, aNode->parent());
    aNode->moveHalfTo(newNode); // move o meio para o novo no
    return newNode;
}


// REMOVER


void BPlusTree::remove(KeyType aKey)
{
    if (isEmpty()) {
        return;
    } else {
        removeFromLeaf(aKey);
    }
}

void BPlusTree::removeFromLeaf(KeyType aKey)
{
    LeafNode* leafNode = findLeafNode(aKey);
    if (!leafNode) {
        return;
    }
    if (!leafNode->lookup(aKey)) {
        return;
    }
    int newSize = leafNode->removeAndDeleteRecord(aKey);
    if (newSize < leafNode->minSize()) {
        coalesceOrRedistribute(leafNode);
    }
}


template <typename N>
void BPlusTree::coalesceOrRedistribute(N* aNode)
{
    if (aNode->isRoot()) {
        adjustRoot();
        return;
    }
    //static_cast = Converte entre tipos usando uma combinação de conversões implícitas e definidas pelo usuário.
    // auto
    auto parent = static_cast<InternalNode*>(aNode->parent());
    int indexOfNodeInParent = parent->nodeIndex(aNode);
    int neighborIndex = (indexOfNodeInParent == 0) ? 1 : indexOfNodeInParent - 1;
    N* neighborNode = static_cast<N*>(parent->neighbor(neighborIndex));
    if (aNode->size() + neighborNode->size() <= neighborNode->maxSize()) {
        coalesce(neighborNode, aNode, parent, indexOfNodeInParent);
    } else {
        redistribute(neighborNode, aNode, parent, indexOfNodeInParent);
    }
}

template <typename N>
void BPlusTree::coalesce(N* aNeighborNode, N* aNode, InternalNode* aParent, int aIndex)
{
    if (aIndex == 0) {
        std::swap(aNode, aNeighborNode);
        //swap Troca os valores de a e b .
        aIndex = 1;
    }
    aNode->moveAllTo(aNeighborNode, aIndex);
    aParent->remove(aIndex);
    if (aParent->size() < aParent->minSize()) {
        coalesceOrRedistribute(aParent);
    }
    delete aNode;
}

template <typename N>
void BPlusTree::redistribute(N* aNeighborNode, N* aNode, InternalNode* aParent, int aIndex)
{
    if (aIndex == 0) {
        aNeighborNode->moveFirstToEndOf(aNode);
    } else {
        aNeighborNode->moveLastToFrontOf(aNode, aIndex);
    }
}

void BPlusTree::adjustRoot()
{
    if (!fRoot->isLeaf() && fRoot->size() == 1) {
        auto discardedNode = static_cast<InternalNode*>(fRoot);
        fRoot = static_cast<InternalNode*>(fRoot)->removeAndReturnOnlyChild();
        fRoot->setParent(nullptr);
        delete discardedNode;
    } else if (!fRoot->size()){
        delete fRoot;
        fRoot = nullptr;
    }
}


// UTILITIES AND PRINTING

LeafNode* BPlusTree::findLeafNode(KeyType aKey, bool aPrinting, bool aVerbose)
{
    if (isEmpty()) {
        if (aPrinting) {
            std::cout << "Arvore Vazia" << std::endl;
        }
        return nullptr;
    }
    auto node = fRoot;
    if (aPrinting) {
        std::cout << "Raiz: ";
        if (fRoot->isLeaf()) {
            std::cout << "\t" << static_cast<LeafNode*>(fRoot)->toString(aVerbose);
        } else {
            std::cout << "\t" << static_cast<InternalNode*>(fRoot)->toString(aVerbose);
        }
        std::cout << std::endl;
    }
    while (!node->isLeaf()) {
        auto internalNode = static_cast<InternalNode*>(node);
        if (aPrinting && node != fRoot) {
            std::cout << "\tNo: " << internalNode->toString(aVerbose) << std::endl;
        }
        node = internalNode->lookup(aKey);
    }
    return static_cast<LeafNode*>(node);
}



void BPlusTree::print(bool aVerbose)
{
    fPrinter.setVerbose(aVerbose);
    fPrinter.printTree(fRoot);
}

void BPlusTree::printLeaves(bool aVerbose)
{
    fPrinter.setVerbose(aVerbose);
    fPrinter.printLeaves(fRoot);
}

void BPlusTree::destroyTree()
{
    if (fRoot->isLeaf()) {
        delete static_cast<LeafNode*>(fRoot);
    } else {
        delete static_cast<InternalNode*>(fRoot);
    }
    fRoot = nullptr;
}

void BPlusTree::printValue(KeyType aKey, bool aVerbose)
{
    printValue(aKey, false, aVerbose);
}
//imprimi a chave, o caminho e o endereco
void BPlusTree::printValue(KeyType aKey, bool aPrintPath, bool aVerbose)
{
    LeafNode* leaf = findLeafNode(aKey, aPrintPath, aVerbose);
    if (!leaf) {
        std::cout << "Folha não encontrada com a chave " << aKey << "." << std::endl;
        return;
    }
    if (aPrintPath) {
        std::cout << "\t";
    }
    std::cout << "Folha: " << leaf->toString(aVerbose) << std::endl;
    Record* record = leaf->lookup(aKey);
    if (!record) {
        std::cout << "Registro nao encontrada com a chave " << aKey << "." << std::endl;
        return;
    }
    if (aPrintPath) {
        std::cout << "\t";
    }
    std::cout << "Registro encontrado com a localizacao " << std::hex << record << std::dec << ":" << std::endl;
    std::cout << "\tChave: " << aKey << "   Valor: " << record->value() << std::endl;
}
//imprimi o caminho da arvore ate uma chave
void BPlusTree::printPathTo(KeyType aKey, bool aVerbose)
{
    printValue(aKey, true, aVerbose);
}

void BPlusTree::printRange(KeyType aStart, KeyType aEnd)
{
    auto rangeVector = range(aStart, aEnd);
    for (auto entry : rangeVector) {
        std::cout << "Chave: " << std::get<0>(entry);
        std::cout << " Valor: " << std::get<1>(entry);
        std::cout << " Folha: " << std::hex << std::get<2>(entry) << std::dec << std::endl;
    }
}

std::vector<BPlusTree::EntryType> BPlusTree::range(KeyType aStart, KeyType aEnd)
{
    auto startLeaf = findLeafNode(aStart);
    auto endLeaf = findLeafNode(aEnd);
    std::vector<std::tuple<KeyType, ValueType, LeafNode*>> entries;
    if (!startLeaf || !endLeaf) {
        return entries;
    }
    //recebe uma chave e um vetor de entradas
    startLeaf->copyRangeStartingFrom(aStart, entries);
    startLeaf = startLeaf->next();
    while (startLeaf != endLeaf) {
        startLeaf->copyRange(entries);
        startLeaf = startLeaf->next();
    }
    startLeaf->copyRangeUntil(aEnd, entries);
    return entries;
}
