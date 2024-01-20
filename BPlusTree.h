//
// Created by trann on 6/11/2022.
//

#include "BPTreeNode.h"
#ifndef BPLUSTREE_H
#define BPLUSTREE_H

class BPlusTree {
	BPTreeNode* root;
	int n; 		//max number for keys in a node
public:
	//constructors
	BPlusTree(int keyLimit);
	//assignment operator
	//copy constructor

	bool insert(int key, string value);
	bool remove(int key);
	string find(int key);

	void printKeys();
	void printValues();

	//destructor

private:
	BPTreeNode* findNodeStoringThisKey(int key);
	bool isNodeOverflow(BPTreeNode* node);
	void insertInterior(BPTreeNode* targetNode, int key, BPTreeNode* leftPtr, BPTreeNode* rightPtr);
	bool isLeafNodeStillHalfFull(BPTreeNode* node);
	bool isInteriorNodeStillHalfFull(BPTreeNode* node);
	int getPointerIndexFromParentThatPointsToThisNode(BPTreeNode* node);
	bool possibleToRedistributeFromLeft(BPTreeNode* parentNode, int meIndex);
	bool possibleToRedistributeFromRight(BPTreeNode* parentNode, int meIndex);
	bool possibleToCoalesceWithLeft(int meIndex);
	bool readyToGiveUpOneKey(BPTreeNode* node);
	void cleanUp(BPTreeNode* node);
	void removeFromInterior(BPTreeNode* targetNode, int keyIndexToRemove, int childIndexToRemove);

};


#endif //ASSIGNMENT2_BPLUSTREE_H
