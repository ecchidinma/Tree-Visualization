/*
 * File:   BinarySearchTree.h
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on November 27, 2017, 6:24 AM
 */

#ifndef BINARYSEARCHTREE_H
#define	BINARYSEARCHTREE_H

class BinarySearchTree
{
    public:
        // self-referential nested structure
        typedef struct treeNode
        {
           struct treeNode *leftPtr;  // pointer to left subtree
           int data; // node value
           struct treeNode *rightPtr; // pointer to right subtree
        }TreeNode; // end structure treeNode

    private: // private data members
        TreeNode* rootPtr; // tree root pointer
        TreeNode** ptrToRootPtr; //pointer to tree root pointer
        int* columnCounter; // a 1-dimensional array of numLevels elements each initially -1
        int lastLevelIndex; // highest ("bottomest") level number in the tree, the top being level 0
        int numLevels; // lastLevelIndex+1;
        size_t numNodes; // holds the number of nodes in the tree
        int numElemsPerRow; // pow(2, lastLevelIndex) => highest possible number of elements at highest level
        int* dataMask; // a 2-dimensional array of numLevels by numElemsPerRow elements
        int* dataMap; // a 2-dimensional array of numLevels by numElemsPerRow elements

    public: // public data members
        enum{COUNTER_INITIAL_VALUE = -1, MASK_MAP_INITIALIZER = 0};

    private: // copying of this class object is not necessary
        BinarySearchTree(const BinarySearchTree& orig){} // empty copy constructor defined inline
        BinarySearchTree & operator=(const BinarySearchTree & orig){ return *this; } // empty copy assignment constructor defined inline

        // core private methods
        void insertNode(TreeNode** ptrToTreePtr, int value);
        int highestLevelIndex(TreeNode* treePtr, int zeroLevelIndex);
        void countNodes(TreeNode* treePtr, size_t & counter);
        void populateMask(TreeNode* treePtr, const int & rowCounter);
        int getNodesfromValue(TreeNode* treePtr, const int & value, TreeNode*  resultPtrToTreePtr[], int & parentDataValue, const int & rowCounter);
        void getNodefromValue(TreeNode* treePtr, const int & value, TreeNode**  resultPtrToTreePtr);
        void resultOfgetNodesfromValue01(const int & result);
        void resultOfgetNodesfromValue02(TreeNode* populatedResultPtrToTreePtr[]);
        void resultOfgetNodesfromValue(TreeNode* populatedResultPtrToTreePtr[], const int & result);
        int insertSubTreeNode(TreeNode** ptrToTreePtr, TreeNode* subTreePtr);
        int deleteNodeAndInsertSubTrees(TreeNode** ptrToTreePtr, TreeNode*  populatedResultPtrToTreePtr[], const int & result);
        int balTreeHighestLevelIndex(TreeNode* treePtr);
        void balTreeInitializeArrIdxCounter(int idxCounter[], const int & btNumLevel);
        void balTreePopulateIdxArr(const int & rowCounter, int idxCounter[], const int & arrBegIdx, const int & arrEndIdx, int result[]);
        void inOrderRetrieval(TreeNode* treePtr, int arrContainer[], int & arrIdxCounter);
        void updateData(TreeNode* treePtr);
        void destroyTree(TreeNode* treePtr);

    public:
        BinarySearchTree(); // default constructor
        BinarySearchTree(BinarySearchTree&& orig); // move constructor
        BinarySearchTree & operator=(BinarySearchTree && orig); // move assignment operator

        // accessor methods
        TreeNode* getRootPtr(){ return rootPtr; } // tree root pointer getter defined inline
        TreeNode** getPtrToRootPtr(){ return ptrToRootPtr; } //pointer to tree root pointer getter defined inline
        int* getColumnCounter() { return columnCounter; }  // columnCounter getter defined inline
        int getLastLevelIndex() { return lastLevelIndex; } // lastLevelIndex getter defined inline
        int getNumLevels() { return numLevels; } // numLevels getter defined inline
        size_t getNumNodes() { return numNodes; } // numNodes getter defined inline
        int getNumElemsPerRow() { return numElemsPerRow; } // numElemsPerRow getter defined inline
        int* getDataMask() { return dataMask; }  // dataMask getter defined inline
        int* getDataMap() { return dataMap; }  // dataMap getter defined inline

        // core public methods
        void inOrder(TreeNode* treePtr);
        void preOrder(TreeNode* treePtr);
        void postOrder(TreeNode* treePtr);
        void insertNodeAndUpdate(TreeNode** ptrToTreePtr, int value);
        int getNodeDeleteAndInsertSubTrees(TreeNode** ptrToTreePtr, const int & value, const int & rowCounter);
        void balanceTree(TreeNode** ptrToTreePtr);

        virtual ~BinarySearchTree();

        friend void initializeArrayInt(int* arr, int arrSize, int value);
        friend void print2dArrayInt(int* arrRow0Address, int numRows, int numCols);

}; // end class BinarySearchTree


#endif	/* BINARYSEARCHTREE_H */

