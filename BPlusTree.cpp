//
// Created by trann on 6/11/2022.
//

#include "BPlusTree.h"
#include <iostream>
#include <cmath>
using namespace std;

//constructors
BPlusTree::BPlusTree(int keyLimit) {
	this->root = nullptr;
	this->n = keyLimit;
}

//assignment operator
//copy constructor
//destructor

bool BPlusTree::insert(int key, string value) {
	//check special case: if tree is currently empty
	if(this->root == nullptr){
		BPTreeNode* newRoot = new LeafNode();
		newRoot->getKeySet()->push_back(key);
		newRoot->getValueSet()->push_back(value);
		this->root = newRoot;
		return true;
	}

	//check first if the key is in the tree already
	if(!find(key).empty()){
		return false;
	}

	//now, we know the key is not in the tree, we start inserting it
	//1. get the leaf node where the key needs to be inserted into:
	BPTreeNode* targetNode = findNodeStoringThisKey(key);

	//2. insert the key into the correct position in this leaf node
	int i = 0;
	for(; i < targetNode->getKeySet()->size(); i++){
		if(key < targetNode->getKeySet()->at(i)) {
			break;
		}
	}
	//3. insert key at index i
	targetNode->getKeySet()->insert(targetNode->getKeySet()->begin() + i, key);
	//4. insert the value at index i
	targetNode->getValueSet()->insert(targetNode->getValueSet()->begin() + i, value);

	//5. if node is overflowed, we need to split
	if(isNodeOverflow(targetNode)) {
		//split leaf node:
		BPTreeNode* leftNode = targetNode;
		BPTreeNode* rightNode = new LeafNode();
		int breakIndex = ceil((double)(this->n + 1) / 2) - 1;

		//copy into right node
		rightNode->setParent(targetNode->getParentPtr());
		for(int i=breakIndex+1; i < targetNode->getKeySet()->size(); i++) {
			rightNode->getKeySet()->push_back(targetNode->getKeySet()->at(i));
			rightNode->getValueSet()->push_back(targetNode->getValueSet()->at(i));
		}
		rightNode->setNextLeafPointer(targetNode->getNextLeafPtr());

		//now, remove things from left node
		leftNode->getKeySet()->erase(leftNode->getKeySet()->begin()+(breakIndex + 1), leftNode->getKeySet()->end());
		leftNode->getValueSet()->erase(leftNode->getValueSet()->begin()+(breakIndex + 1), leftNode->getValueSet()->end());
		leftNode->setNextLeafPointer(rightNode);

		//done splitting => now need to insert the key and pointers into parent node
		int upKey = rightNode->getKeySet()->front();
		insertInterior(leftNode->getParentPtr(), upKey, leftNode, rightNode);
	}
	return true;
}

bool BPlusTree::remove(int key) {
	//special case: tree is empty
	if(this->root == nullptr) {
		return false;
	}

	//tree is not empty, but the key does not exist in the tree
	if(find(key).empty()){
		return false;
	}

	//by this point, we know the key exists in this tree => so we start removing it from the leaf
	BPTreeNode* targetNode = findNodeStoringThisKey(key);
	//1. find the correct spot index i to remove this key
	vector<int>* targetNodeKeySet = targetNode->getKeySet();
	int i=0;
	for(; i < targetNodeKeySet->size(); i++){
		if(key == targetNodeKeySet->at(i)){
			break;
		}
	}
	//2. remove key and value at index i
	targetNode->getKeySet()->erase(targetNode->getKeySet()->begin()+i);
	targetNode->getValueSet()->erase(targetNode->getValueSet()->begin()+i);

	//special case: if this is the root node
	if(targetNode == this->root) {
		if(targetNode->getKeySet()->empty()) {
			cleanUp(targetNode);
			this->root = nullptr;
		}
		return true;
	}

	//3.check half full condition
	if (!isLeafNodeStillHalfFull(targetNode)) {
		//need to decide: redistribute or coalesce
		//go up to the parent to check:
		int targetNodePositionWithinParent = getPointerIndexFromParentThatPointsToThisNode(targetNode);
		if(possibleToRedistributeFromLeft(targetNode->getParentPtr(), targetNodePositionWithinParent)){
			BPTreeNode* leftSibling = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePositionWithinParent-1);
			int transferKey = leftSibling->getKeySet()->back();
			string transferValue = leftSibling->getValueSet()->back();
			//insert the key and value into target node
			targetNode->getKeySet()->insert(targetNode->getKeySet()->begin(), transferKey);
			targetNode->getValueSet()->insert(targetNode->getValueSet()->begin(), transferValue);

			//remove transfer key and value from left sibling
			leftSibling->getKeySet()->pop_back();
			leftSibling->getValueSet()->pop_back();

			//update the key in the parent
			int updateKeyPos = targetNodePositionWithinParent - 1;
			(*(targetNode->getParentPtr()->getKeySet()))[updateKeyPos] = transferKey;
			//done!

		} else if (possibleToRedistributeFromRight(targetNode->getParentPtr(), targetNodePositionWithinParent)) {
			BPTreeNode* rightSibling = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePositionWithinParent+1);
			int transferKey = rightSibling->getKeySet()->front();
			string transferValue = rightSibling->getValueSet()->front();

			//insert key and value into current target node
			targetNode->getKeySet()->push_back(transferKey);
			targetNode->getValueSet()->push_back(transferValue);

			//remove key and value from right sibling
			rightSibling->getKeySet()->erase(rightSibling->getKeySet()->begin());
			rightSibling->getValueSet()->erase(rightSibling->getValueSet()->begin());

			//update key in the parent
			int updateKeyPos = targetNodePositionWithinParent;
			int updateKey = rightSibling->getKeySet()->front();
			(*(targetNode->getParentPtr()->getKeySet()))[updateKeyPos] = updateKey;
			//done!

		} else if (possibleToCoalesceWithLeft(targetNodePositionWithinParent)) {
			//push things from current target node into the left node
			BPTreeNode* leftSibling = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePositionWithinParent-1);
			//push keys & values into left sibling
			for(int j=0; j < targetNode->getKeySet()->size(); j++) {
				leftSibling->getKeySet()->push_back(targetNode->getKeySet()->at(j));
				leftSibling->getValueSet()->push_back(targetNode->getValueSet()->at(j));
			}

			//adjust next leaf pointer
			leftSibling->setNextLeafPointer(targetNode->getNextLeafPtr());

			//delete the current target node
			cleanUp(targetNode);

			//delete from parent
			int keyIndexToRemove = targetNodePositionWithinParent - 1;
			int childIndexToRemove = targetNodePositionWithinParent;
			removeFromInterior(leftSibling->getParentPtr(), keyIndexToRemove, childIndexToRemove);

		} else {
			//coalesce with right sibling
			//push things from right sibling into this current target node
			BPTreeNode* rightSibling = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePositionWithinParent+1);
			//push keys and values into the current target node
			for(int j=0; j < rightSibling->getKeySet()->size(); j++) {
				targetNode->getKeySet()->push_back(rightSibling->getKeySet()->at(j));
				targetNode->getValueSet()->push_back(rightSibling->getValueSet()->at(j));
			}
			targetNode->setNextLeafPointer(rightSibling->getNextLeafPtr());

			cleanUp(rightSibling);

			int keyIndexToRemove = targetNodePositionWithinParent;
			int childIndexToRemove = targetNodePositionWithinParent+1;
			removeFromInterior(targetNode->getParentPtr(), keyIndexToRemove, childIndexToRemove);
		}
	}

	return true;
}

string BPlusTree::find(int key) {
	//special case: empty tree
	if (this->root == nullptr){
		return "";
	}

	//otherwise:
	string result;
	BPTreeNode* traverser = findNodeStoringThisKey(key);
	//now we reach the leaf
	vector<int>* keyList = traverser->getKeySet();
	for(int i=0; i < keyList->size(); i++) {
		int current = keyList->at(i);
		if(key == current) {
			result = traverser->getValueSet()->at(i);
			break;
		}
		if(key < current) {
			break;
		}
	}

	return result;
}


void BPlusTree::printKeys() {
	BPTreeNode* traverser = this->root;
	//handle tree empty
	if (traverser == nullptr){
		cout << "Your tree is empty!" << endl;
		return;
	}

	//print root
	traverser->printKeys();
	cout << endl;

	if(traverser->isLeaf()){
		return;
	}

	vector<BPTreeNode*> parentLevel;
	parentLevel.push_back(traverser);
	vector<BPTreeNode*> nextLevel;

	while(!parentLevel.empty()){
		//for each parent node -> print its children -> add the children into the next level list
		for(int i=0; i < parentLevel.size(); i++) {
			BPTreeNode* parent = parentLevel.at(i);
			for(int j=0; j < parent->getNodePointerSet()->size(); j++) {
				BPTreeNode *child = parent->getNodePointerSet()->at(j);
				child->printKeys();
				if(!child->isLeaf()) {
					nextLevel.push_back(child);
				}
			}
		}
		//done with one level
		cout << endl;
		parentLevel = nextLevel;
		nextLevel.clear();
	}
}

void BPlusTree::printValues() {
	//reach the left most leaf, then start printing across the leaves
	BPTreeNode* traverser = this->root;

	if(traverser == nullptr) {
		cout << "Your tree is empty!" << endl;
		return;
	}

	while(!traverser->isLeaf()){
		traverser = traverser->getNodePointerSet()->front();
	}

	//now we are at the left most leaf
	while(traverser != nullptr){
		//print values in this current leaf
		traverser->printValues();
		traverser = traverser->getNextLeafPtr();
	}
}


//////////////////////////// PRIVATE SUPPORT FUNCTIONS /////////////////////////////////
BPTreeNode* BPlusTree::findNodeStoringThisKey(int key) {
	BPTreeNode* traverser = this->root;
	while (!traverser->isLeaf()) {
		if(key < traverser->getKeySet()->front()){
			traverser = traverser->getNodePointerSet()->front();
		} else if (key >= traverser->getKeySet()->back()) {
			traverser = traverser->getNodePointerSet()->back();
		} else {
			//scan thru and find a pointer to jump to
			vector<int>* keyList = traverser->getKeySet();
			for(int i=1; i < keyList->size(); i++) {
				if(key < keyList->at(i)) {
					traverser = traverser->getNodePointerSet()->at(i);
					break;
				}
			}
		}
	}
	return traverser;
}


bool BPlusTree::isNodeOverflow(BPTreeNode* node) {
	if (node->getKeySet()->size() > n) {
		return true;
	}
	return false;
}

void BPlusTree::insertInterior(BPTreeNode* targetNode, int key, BPTreeNode* leftPtr, BPTreeNode* rightPtr) {
	//special base case: when we need to create new root
	if(targetNode == nullptr) {
		BPTreeNode* newRoot = new InteriorNode();
		newRoot->getKeySet()->push_back(key);
		newRoot->getNodePointerSet()->push_back(leftPtr);
		newRoot->getNodePointerSet()->push_back(rightPtr);
		this->root = newRoot;

		leftPtr->setParent(newRoot);
		rightPtr->setParent(newRoot);
		return;
	}

	//1. find the correct spot index i for insert
	int i=0;
	for(; i < targetNode->getKeySet()->size(); i++) {
		if(key < targetNode->getKeySet()->at(i)){
			break;
		}
	}
	//2. insert key into index i
	targetNode->getKeySet()->insert(targetNode->getKeySet()->begin()+i, key);

	//3. no need to adjust the child pointer index i -> left node

	//4. add the child pointer index i+1 -> right node
	targetNode->getNodePointerSet()->insert(targetNode->getNodePointerSet()->begin()+(i+1), rightPtr);

	//5. handle overflow
	if(isNodeOverflow(targetNode)) {
		//split interior node
		int breakIndex = ceil((double)this->n / 2) - 1;
		int upKey = targetNode->getKeySet()->at(breakIndex+1);

		BPTreeNode* newLeftNode = targetNode;
		BPTreeNode* newRightNode = new InteriorNode();

		//copy things into right node:
		newRightNode->setParent(targetNode->getParentPtr());
		for(int j=breakIndex+2; j < targetNode->getKeySet()->size(); j++) {
			newRightNode->getKeySet()->push_back(targetNode->getKeySet()->at(j));
		}
		for(int j=breakIndex+2; j < targetNode->getNodePointerSet()->size(); j++) {
			newRightNode->getNodePointerSet()->push_back(targetNode->getNodePointerSet()->at(j));
			//make sure children points to me as their parent
			targetNode->getNodePointerSet()->at(j)->setParent(newRightNode);
		}

		//delete things from left node:
		newLeftNode->getKeySet()->erase(newLeftNode->getKeySet()->begin()+(breakIndex+1), newLeftNode->getKeySet()->end());
		newLeftNode->getNodePointerSet()->erase(newLeftNode->getNodePointerSet()->begin()+(breakIndex+2), newLeftNode->getNodePointerSet()->end());

		//insert again into the parent node
		insertInterior(newLeftNode->getParentPtr(), upKey, newLeftNode, newRightNode);
	}
}

bool BPlusTree::isLeafNodeStillHalfFull(BPTreeNode* node) {
	int threshold = floor(double(this->n + 1) / 2);
	if(node->getKeySet()->size() < threshold){
		return false;
	}
	return true;
}

bool BPlusTree::isInteriorNodeStillHalfFull(BPTreeNode* node) {
	int threshold = ceil(double(this->n + 1) / 2);
	if(node->getNodePointerSet()->size() < threshold) {
		return false;
	}
	return true;
}

int BPlusTree::getPointerIndexFromParentThatPointsToThisNode(BPTreeNode* node) {
	BPTreeNode* parent = node->getParentPtr();
	int i=0;
	for(; i < parent->getNodePointerSet()->size(); i++) {
		if(node == parent->getNodePointerSet()->at(i)) {
			break;
		}
	}
	return i;
}

bool BPlusTree::possibleToRedistributeFromLeft(BPTreeNode* parentNode, int meIndex) {
	//if im the left most child already, then return false
	if(meIndex == 0) {
		return false;
	}

	BPTreeNode* leftSibling = parentNode->getNodePointerSet()->at(meIndex-1);
	if(readyToGiveUpOneKey(leftSibling)){
		return true;
	}
	return false;
}

bool BPlusTree::possibleToRedistributeFromRight(BPTreeNode* parentNode, int meIndex) {
	//if im the right most child already, return false
	if(meIndex == parentNode->getNodePointerSet()->size()-1) {
		return false;
	}

	BPTreeNode* rightSibling = parentNode->getNodePointerSet()->at(meIndex+1);
	if(readyToGiveUpOneKey(rightSibling)){
		return true;
	}
	return false;
}

bool BPlusTree::possibleToCoalesceWithLeft(int meIndex){
	//if im the left most child already
	if(meIndex == 0) {
		return false;
	}
	return true;
}

bool BPlusTree::readyToGiveUpOneKey(BPTreeNode* node) {
	//two cases: leaf node versus interior node
	if(node->isLeaf()) {
		int threshold = floor(double(this->n + 1) / 2);
		if(node->getKeySet()->size() > threshold) {
			return true;
		}
		return false;
	}
	else {
		int threshold = ceil(double(this->n + 1) / 2);
		if(node->getNodePointerSet()->size() > threshold) {
			return true;
		}
		return false;
	}
}

void BPlusTree::cleanUp(BPTreeNode* node) {
	if(node == nullptr) {
		return;
	}
	delete node;
}

void BPlusTree::removeFromInterior(BPTreeNode* targetNode, int keyIndexToRemove, int childIndexToRemove) {
	//1. just remove things as requested first
	targetNode->getKeySet()->erase(targetNode->getKeySet()->begin()+keyIndexToRemove);
	targetNode->getNodePointerSet()->erase(targetNode->getNodePointerSet()->begin()+childIndexToRemove);

	//2. special case: targetNode is the root
	if(targetNode == this->root) {
		if(targetNode->getKeySet()->empty()) {
			this->root = targetNode->getNodePointerSet()->front();
			cleanUp(targetNode);
		}
		return;
	}

	//3. otherwise: if no problem we return
	if(isInteriorNodeStillHalfFull(targetNode)) {
		return;
	}

	//4. by this point we know that the node is less than half full
	//find my position in my parent's children list
	int targetNodePosInParent = getPointerIndexFromParentThatPointsToThisNode(targetNode);
	if(possibleToRedistributeFromLeft(targetNode->getParentPtr(), targetNodePosInParent)) {
		BPTreeNode* leftSib = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePosInParent-1);
		int keyFromParent = targetNode->getParentPtr()->getKeySet()->at(targetNodePosInParent-1);
		int keyFromLeftSib = leftSib->getKeySet()->back();
		BPTreeNode* childFromLeftSib = leftSib->getNodePointerSet()->back();

		//start inserting
		targetNode->getKeySet()->insert(targetNode->getKeySet()->begin(), keyFromParent);
		targetNode->getNodePointerSet()->insert(targetNode->getNodePointerSet()->begin(), childFromLeftSib);
		childFromLeftSib->setParent(targetNode);

		(*(targetNode->getParentPtr()->getKeySet()))[targetNodePosInParent-1] = keyFromLeftSib;

		//remove from left sibling
		leftSib->getKeySet()->pop_back();
		leftSib->getNodePointerSet()->pop_back();

		//done!
	}
	else if (possibleToRedistributeFromRight(targetNode->getParentPtr(), targetNodePosInParent)) {
		BPTreeNode* rightSib = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePosInParent+1);

		int keyFromParent = targetNode->getParentPtr()->getKeySet()->at(targetNodePosInParent);
		int keyFromRightSib = rightSib->getKeySet()->front();
		BPTreeNode* childFromRightSib = rightSib->getNodePointerSet()->front();

		//start inserting
		targetNode->getKeySet()->push_back(keyFromParent);
		targetNode->getNodePointerSet()->push_back(childFromRightSib);
		childFromRightSib->setParent(targetNode);

		(*(targetNode->getParentPtr()->getKeySet()))[targetNodePosInParent] = keyFromRightSib;

		//remove from right sibling
		rightSib->getKeySet()->erase(rightSib->getKeySet()->begin());
		rightSib->getNodePointerSet()->erase(rightSib->getNodePointerSet()->begin());

		//done!
	}
	else if(possibleToCoalesceWithLeft(targetNodePosInParent)) {
		BPTreeNode* leftSib = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePosInParent-1);
		int keyFromParent = targetNode->getParentPtr()->getKeySet()->at(targetNodePosInParent-1);

		//insert all keys into left sibling
		leftSib->getKeySet()->push_back(keyFromParent);
		for(int j=0; j < targetNode->getKeySet()->size(); j++) {
			leftSib->getKeySet()->push_back(targetNode->getKeySet()->at(j));
		}

		//insert all children pointers into left sibling
		for(int j=0; j < targetNode->getNodePointerSet()->size(); j++) {
			BPTreeNode* child = targetNode->getNodePointerSet()->at(j);
			leftSib->getNodePointerSet()->push_back(child);
			child->setParent(leftSib);
		}

		//delete current target node
		cleanUp(targetNode);

		//remove from parent:
		int keyIndexParentShouldRemove = targetNodePosInParent - 1;
		int childIndexParentShouldRemove = targetNodePosInParent;
		removeFromInterior(leftSib->getParentPtr(), keyIndexParentShouldRemove, childIndexParentShouldRemove);
	}
	else {
		//coalesce with the right sibling
		BPTreeNode* rightSib = targetNode->getParentPtr()->getNodePointerSet()->at(targetNodePosInParent+1);
		int keyFromParent = targetNode->getParentPtr()->getKeySet()->at(targetNodePosInParent);

		//insert keys into current target node:
		targetNode->getKeySet()->push_back(keyFromParent);
		for(int j=0; j < rightSib->getKeySet()->size(); j++) {
			targetNode->getKeySet()->push_back(rightSib->getKeySet()->at(j));
		}

		//insert all children pointers into target node
		for(int j=0; j < rightSib->getNodePointerSet()->size(); j++) {
			BPTreeNode* child = rightSib->getNodePointerSet()->at(j);
			targetNode->getNodePointerSet()->push_back(child);
			child->setParent(targetNode);
		}
		cleanUp(rightSib);

		//remove from parent
		int keyIndexParentShouldRemove = targetNodePosInParent;
		int childIndexParentShouldRemove = targetNodePosInParent + 1;
		removeFromInterior(targetNode->getParentPtr(), keyIndexParentShouldRemove, childIndexParentShouldRemove);
	}
}