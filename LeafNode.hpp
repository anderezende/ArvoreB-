#pragma once

#include <tuple>
#include <utility>
#include <vector>
#include "Node.hpp"
#include "Record.hpp"

class LeafNode : public Node
{
public:
    explicit LeafNode(int aOrder);
    explicit LeafNode(int aOrder, Node* aParent);
    ~LeafNode() override;
    using MappingType = std::pair<KeyType, Record*>;
    using EntryType = std::tuple<KeyType, ValueType, LeafNode*>;
    bool isLeaf() const override;
    LeafNode* next() const;
    void setNext(LeafNode* aNext);
    int size() const override;
    int minSize() const override;
    int maxSize() const override;
    int createAndInsertRecord(KeyType aKey, ValueType aValue);
    void insert(KeyType aKey, Record* aRecord);
    Record* lookup(KeyType aKey) const;
    int removeAndDeleteRecord(KeyType aKey);
    KeyType firstKey() const;
    void moveHalfTo(LeafNode* aRecipient);
    void moveAllTo(LeafNode* aRecipient, int /* Unused */);
    void moveFirstToEndOf(LeafNode* aRecipient);
    void moveLastToFrontOf(LeafNode* aRecipient, int aParentIndex);
    void copyRangeStartingFrom(KeyType aKey, std::vector<EntryType>& aVector);
    void copyRangeUntil(KeyType aKey, std::vector<EntryType>& aVector);
    void copyRange(std::vector<EntryType>& aVector);
    std::string toString(bool aVerbose = false) const override;
private:
    void copyHalfFrom(std::vector<MappingType>& aMappings);
    void copyAllFrom(std::vector<MappingType>& aMappings);
    void copyLastFrom(MappingType aPair);
    void copyFirstFrom(MappingType aPair, int aParentIndex);
    std::vector<MappingType> fMappings;
    LeafNode* fNext;
};
