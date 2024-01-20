//
// Created by trann on 6/11/2022.
//

#include "BPTreeNode.h"
#include <iostream>
using namespace std;

////////// IMPLEMENT BPTREENODE ///////////////////
//constructors
BPTreeNode::BPTreeNode() {
	parent = nullptr;
	keySet = new vector<int>();
}

BPTreeNode::~BPTreeNode() {};

//getters
vector<int>* BPTreeNode::getKeySet() {
	return this->keySet;
}

BPTreeNode* BPTreeNode::getParentPtr() {
	return this->parent;
}

vector<string>* BPTreeNode::getValueSet() {
	return nullptr;
}

BPTreeNode* BPTreeNode::getNextLeafPtr() {
	return nullptr;
}

vector<BPTreeNode*>* BPTreeNode::getNodePointerSet() {
	return nullptr;
}

//setters
void BPTreeNode::setParent(BPTreeNode* p) {
	this->parent = p;
}

void BPTreeNode::setNextLeafPointer(BPTreeNode* p) {};

void BPTreeNode::printKeys() {
	//print all keys in this node as: [k1, k2, k3,...]
	cout << "[";
	for(int i=0; i < keySet->size() - 1; i++) {
		cout << keySet->at(i) << ", ";
	}
	//print the last key
	cout << keySet->back() << "] ";
}

void BPTreeNode::printValues() {};

/////////// IMPLEMENT LEAF NODE /////////////////////

//constructors
LeafNode::LeafNode() {
	//automatically call the super class constructor
	//then execute this constructor:
	valueSet = new vector<string>();
	nextLeafPtr = nullptr;
}
//destructors
LeafNode::~LeafNode() {
	delete this->keySet;
	delete this->valueSet;
	//delete this;
}

//getters
vector<string>* LeafNode::getValueSet() {
	return this->valueSet;
}

BPTreeNode* LeafNode::getNextLeafPtr() {
	return this->nextLeafPtr;
}

//setters
void LeafNode::setNextLeafPointer(BPTreeNode* p) {
	this->nextLeafPtr = p;
}

void LeafNode::printValues() {
	for(int i=0; i < valueSet->size(); i++){
		cout << valueSet->at(i) << endl;
	}
}

bool LeafNode::isLeaf() {
	return true;
}

////////////////// IMPLEMENT INTERIOR NODE ///////////////////////

InteriorNode::InteriorNode() {
	//automatically call the super class constructor
	//then execute this constructor:
	nodePointerSet = new vector<BPTreeNode*>();
}

InteriorNode::~InteriorNode() {
	delete this->keySet;
	delete this->nodePointerSet;
}

//getters
vector<BPTreeNode*>* InteriorNode::getNodePointerSet() {
	return this->nodePointerSet;
}

bool InteriorNode::isLeaf() {
	return false;
}
