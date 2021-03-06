
#pragma once

#include <queue>
#include <vector>
#include "Definitions.hpp"
#include "Node.hpp"

class InternalNode : public Node
{
public:
    explicit InternalNode(int aOrder);
    explicit InternalNode(int aOrder, Node* aParent);
    ~InternalNode() override;
    using MappingType = std::pair<KeyType, Node*>;
    bool isLeaf() const override;
    int size() const override;
    int minSize() const override;
    int maxSize() const override;
    KeyType keyAt(int aIndex) const;
    void setKeyAt(int aIndex, KeyType aKey);
    Node* firstChild() const;
    void populateNewRoot(Node* aOldNode, KeyType aNewKey, Node* aNewNode);
    int insertNodeAfter(Node* aOldNode, KeyType aNewKey, Node* aNewNode);
    void remove(int aIndex);
    Node* removeAndReturnOnlyChild();
    KeyType replaceAndReturnFirstKey();
    void moveHalfTo(InternalNode* aRecipient);
    void moveAllTo(InternalNode* aRecipient, int aIndexInParent);
    void moveFirstToEndOf(InternalNode* aRecipient);
    void moveLastToFrontOf(InternalNode* aRecipient, int aParentIndex);
    Node* lookup(KeyType aKey) const;
    int nodeIndex(Node* aNode) const;
    Node* neighbor(int aIndex) const;
    std::string toString(bool aVerbose = false) const override;
    void queueUpChildren(std::queue<Node*>* aQueue);
private:
    void copyHalfFrom(std::vector<MappingType>& aMappings);
    void copyAllFrom(std::vector<MappingType>& aMappings);
    void copyLastFrom(MappingType aPair);
    void copyFirstFrom(MappingType aPair, int aParentIndex);
    std::vector<MappingType> fMappings;
};
