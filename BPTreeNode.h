//
// Created by trann on 6/11/2022.
//

#include <vector>
#include <string>
using namespace std;

#ifndef BPTREENODE_H
#define BPTREENODE_H

class BPTreeNode {
protected:
	BPTreeNode* parent;
	vector<int>* keySet;
public:
	//constructors
	BPTreeNode();
	//destructors
	virtual ~BPTreeNode();

	//getters
	vector<int>* getKeySet();
	BPTreeNode* getParentPtr();
	virtual vector<string>* getValueSet();
	virtual BPTreeNode* getNextLeafPtr();
	virtual vector<BPTreeNode*>* getNodePointerSet();

	//setters
	void setParent(BPTreeNode* p);
	virtual void setNextLeafPointer(BPTreeNode* p);

	//other methods
	void printKeys();
	virtual void printValues();
	virtual bool isLeaf() = 0;
};

//////////////////////////////////////////////////////////////////////////////
class LeafNode : public BPTreeNode {
	vector<string>* valueSet;
	BPTreeNode* nextLeafPtr;
public:
	//constructors
	LeafNode();
	//destructors
	~LeafNode();

	//getters
	vector<string>* getValueSet() override;
	BPTreeNode* getNextLeafPtr() override;

	//setters
	void setNextLeafPointer(BPTreeNode* p) override;

	//others
	void printValues() override;
	bool isLeaf() override;
};

//////////////////////////////////////////////////////////////////////////////

class InteriorNode : public BPTreeNode {
	vector<BPTreeNode*>* nodePointerSet;
public:
	InteriorNode();
	~InteriorNode();

	//getters
	vector<BPTreeNode*>* getNodePointerSet() override;
	bool isLeaf() override;
};



#endif //BPTREENODE_H
