/*
 * File:   BinarySearchTree.cpp
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on November 27, 2017, 6:24 AM
 */

#include <iostream>
#include <iomanip>
#include "BinarySearchTree.h"

// default constructor
BinarySearchTree::BinarySearchTree()
{
    rootPtr = NULL; // tree initially empty
    ptrToRootPtr = &rootPtr;
    columnCounter = NULL;
    lastLevelIndex = 0;
    numLevels = 1;
    numNodes = 0;
    numElemsPerRow = 1;
    dataMask = NULL;
    dataMap = NULL;
}// end default constructor


// move constructor
BinarySearchTree::BinarySearchTree(BinarySearchTree&& orig)
{
    std::cout << "\nI am about moving in to a new object!" << std::endl;
    // steal tree pointer
    rootPtr = orig.rootPtr;
    ptrToRootPtr = &rootPtr;
    orig.rootPtr = NULL; // condemn

    // steal columnCounter
    columnCounter = orig.columnCounter;
    orig.columnCounter = NULL; // condemn

    // steal lastLevelIndex
    lastLevelIndex = orig.lastLevelIndex;
    orig.lastLevelIndex = 0; // condemn

    // steal numLevels
    numLevels = orig.numLevels;
    orig.numLevels = 0; // condemn

    // steal numNodes
    numNodes = orig.numNodes;
    orig.numNodes = 0; // condemn

    // steal numElemsPerRow
    numElemsPerRow = orig.numElemsPerRow;
    orig.numElemsPerRow = 0; // condemn

     // steal dataMask
    dataMask = orig.dataMask;
    orig.dataMask = NULL; // condemn

    // steal dataMap
    dataMap = orig.dataMap;
    orig.dataMap = NULL; // condemn

    std::cout << "Oh! I am through with moving!!!!!" << std::endl;
}// end move constructor

void BinarySearchTree::insertNode(TreeNode** ptrToTreePtr, int value)
{
    if(*ptrToTreePtr == NULL)
    {// tree is empty
      *ptrToTreePtr = new TreeNode; // allocate memory

      if(*ptrToTreePtr != NULL)
      {// memory was allocated, then assign data
         (*ptrToTreePtr)->data = value;
         (*ptrToTreePtr)->leftPtr = NULL;
         (*ptrToTreePtr)->rightPtr = NULL;
      } // end if
      else
      {// memory was not allocated
         std::cout << value << " not inserted. No memory available." << std::endl;
      } // end else
   } // end if
   else // tree is not empty
   {
      if(value < (*ptrToTreePtr)->data)
      { // data to insert is less than data in current node
         insertNode(&((*ptrToTreePtr)->leftPtr), value);
      } // end if
      else if(value > (*ptrToTreePtr)->data)
      {// data to insert is greater than data in current node
         insertNode(&((*ptrToTreePtr)->rightPtr), value);
      } // end else if
      else
      { // duplicate data value ignored
         //std::cout << "duplicate value: ignored";
         return;
      } // end else
   } // end else
}// end insertNode()

int BinarySearchTree::highestLevelIndex(TreeNode* treePtr, int zeroLevelIndex)
{
    // if tree is not empty then traverse
   if(treePtr != NULL)
   {
      //std::cout << "\nWe are in Level: " << zeroLevelIndex+1 << std::endl;
      int m = highestLevelIndex(treePtr->leftPtr, zeroLevelIndex+1);
      int n = highestLevelIndex(treePtr->rightPtr, zeroLevelIndex+1);

      return (n >= m) ? n : m;
   } // end if

   return zeroLevelIndex;
}// end highestLevelIndex()

void BinarySearchTree::countNodes(TreeNode* treePtr, size_t & counter)
{
    // if tree is not empty then traverse
   if(treePtr != NULL)
   {
      counter++;
      countNodes(treePtr->leftPtr, counter);
      countNodes(treePtr->rightPtr, counter);
   } // end if
}// end countNodes()

void BinarySearchTree::populateMask(TreeNode* treePtr, const int & rowCounter)
{
    int i = rowCounter + 1; 

    if(i <= lastLevelIndex)
    {
       columnCounter[i] += 1; 

       if(treePtr != NULL)  // if tree is not empty then traverse
       {
          //std::cout << "\nWe are in Level: " << i << " of !NULL branch" << std::endl;
          int j = columnCounter[i];

          *(dataMask + i*numElemsPerRow + j) = 1; // update mask matrix at this position
          *(dataMap + i*numElemsPerRow + j) = treePtr->data; // update mask matrix at this position

          populateMask(treePtr->leftPtr, i);
          populateMask(treePtr->rightPtr, i);
       }
       else /* if(treePtr == NULL) */
       {/* propagate downwards to update the i and columnCounter[i] for NULL node including its not-assigned descendant nodes*/
            //std::cout << "\nWe are in Level: " << i << " of NULL branch" << std::endl;
            populateMask(NULL, i);
            populateMask(NULL, i);
       } /* end if_else */
    }/* end if */
}// populateMask()

int BinarySearchTree::getNodesfromValue(TreeNode* treePtr, const int & value, TreeNode*  resultPtrToTreePtr[], int & parentDataValue, const int & rowCounter)
{
    int i = rowCounter + 1; 

    if(i <= lastLevelIndex) /* if_A */
    {
       columnCounter[i] += 1; 

       if(treePtr != NULL) /* if_else_A */  /* if tree is not empty then traverse */
       {
            //std::cout << "\nWe are in Level: " << i << " of !NULL branch" << std::endl;
            if(value == treePtr->data) // if_C
            {
                resultPtrToTreePtr[0] = treePtr; // node of the found value
                resultPtrToTreePtr[1] = treePtr->leftPtr; // save left node of the found value
                resultPtrToTreePtr[2] = treePtr->rightPtr; // save right node of the found value

                int j = columnCounter[i];

                if(!(i == 0 && j == 0)) // if_else_B
                {
                    int ip = i-1; /* ip = parent node row counter; i = child node row counter */
                    int jp = j/2; /* jp = parent node column counter; j = child node column counter */
                    parentDataValue = *(dataMap + ip*numElemsPerRow + jp);
                    //std::cout << "Parent's data value is " << *parentDataValue << std::endl;

                    return (j%2 == 0) ? 0 : 1; /* success: 0 signifying that value found is at the left node of its parent node */
                                                /* success: 1 signifying that value found is at the right node of its parent node */
                }
                else //if(i == 0 && j == 0)
                {/* the case for the root node; d/4 it has no parent node */
                    resultPtrToTreePtr[3] = NULL;
                    return 2; /* success: 2 signifying that value was found but it is the root node data value */
                }/* end if_else_B */
            }/* end if_C */

            int n = getNodesfromValue(treePtr->leftPtr, value, resultPtrToTreePtr, parentDataValue, i);
            int m = getNodesfromValue(treePtr->rightPtr, value, resultPtrToTreePtr, parentDataValue, i);

            /* bcos 0, 1 and 2 can only be returned once as this is a binary tree with no duplicate values */
            if(n == -1 && m != -1) return m; /* if_else_C */
            else if(m == -1 && n != -1) return n;
            else /* if(m == -1 && n == -1) */ return -1;
            /* end if_else_C */
        }/* end if_else_A_contd.*/
        else /* if(treePtr == NULL) */
        {/* propagate downwards to update the i and columnCounter[i] for NULL node including its not-assigned descendant nodes*/
            //std::cout << "\nWe are in Level: " << i << " of NULL branch" << std::endl;
            getNodesfromValue(NULL, value, resultPtrToTreePtr, parentDataValue, i);
            getNodesfromValue(NULL, value, resultPtrToTreePtr, parentDataValue, i);
            //return -1; /* failure, value not found */
        }/* end if_else_A */
    }/* end if_A*/
    return -1; /* failure, value not found */
}// end getNodesfromValue()

void BinarySearchTree::getNodefromValue(TreeNode* treePtr, const int & value, TreeNode**  resultPtrToTreePtr)
{
    if(treePtr != NULL) /* if begins*/
    {/* tree is not empty */
        if(value == treePtr->data) /* if_else */
        {
            *resultPtrToTreePtr = treePtr;
            return;
        }
        else if(value < treePtr->data)
        {
            getNodefromValue(treePtr->leftPtr, value, resultPtrToTreePtr);
        }
        else /* if(value > treePtr->data) */
        {
            getNodefromValue(treePtr->rightPtr, value, resultPtrToTreePtr);
        }/* end if_else */
    } /* end if */
}// end getNodefromValue()

void BinarySearchTree::resultOfgetNodesfromValue01(const int & result)
{
    switch(result)
    {
        case -1:
            std::cout << "\nThe searched value was not found in any node in the tree" << std::endl;
            break;
        case 0:
            std::cout << "\nThe searched value was found at the left node of its parent node" << std::endl;
            break;
        case 1:
            std::cout << "\nThe searched value was found at the right node of its parent node" << std::endl;
            break;
        case 2:
            std::cout << "\nThe searched value was found at the root node, and thus has no parent node" << std::endl;
            break;
        default:
            std::cout << "\nPlease supply a return value from the getNodesfromValue() function" << std::endl;
            break;
    }/* end switch */
    std::cout << "\n" << std::endl;
}// end resultOfgetNodesfromValue01()

void BinarySearchTree::resultOfgetNodesfromValue02(TreeNode* populatedResultPtrToTreePtr[])
{
    int k;
    for(k=0; k<4; k++)
    {
        switch(k)
        {
            case 0:
               if(populatedResultPtrToTreePtr[k] != NULL)
                    std::cout << "\nThe searched out node's data value is " << populatedResultPtrToTreePtr[k]->data << std::endl;
                else
                    std::cout << "\nThe searched out value is not in the tree." << std::endl;
                break;
            case 1:
                if(populatedResultPtrToTreePtr[k] != NULL)
                    std::cout << "\nThe searched out node's left data value is " << populatedResultPtrToTreePtr[k]->data << std::endl;
                else
                    std::cout << "\nThe searched out node's left node is NULL." << std::endl;
                break;
            case 2:
                if(populatedResultPtrToTreePtr[k] != NULL)
                    std::cout << "\nThe searched out node's right data value is " << populatedResultPtrToTreePtr[k]->data << std::endl;
                else
                    std::cout << "\nThe searched out node's right node is NULL." << std::endl;
                break;
            case 3:
                if(populatedResultPtrToTreePtr[k] != NULL)
                    std::cout << "\nThe searched out parent's data value is " << populatedResultPtrToTreePtr[k]->data << std::endl;
                else
                    std::cout << "\nThe searched out node has NO parent." << std::endl;
                break;
        }/* end switch */
        std::cout << std::endl;
    }/* end for */
    std::cout << std::endl;
}// end resultOfgetNodesfromValue02()

void BinarySearchTree::resultOfgetNodesfromValue(TreeNode* populatedResultPtrToTreePtr[], const int & result)
{
    resultOfgetNodesfromValue02(populatedResultPtrToTreePtr);
    resultOfgetNodesfromValue01(result);
}// end resultOfgetNodesfromValue()

int BinarySearchTree::insertSubTreeNode(TreeNode** ptrToTreePtr, TreeNode* subTreePtr)
{
    if(subTreePtr == NULL) return 0; /* true if provided sub-tree node is empty */

    if (*ptrToTreePtr == NULL)
    {/* tree is empty */
        *ptrToTreePtr = subTreePtr; /* provided sub-tree node becomes the node */
    } /* end if */
    else /* tree is not empty */
    { /* tree is not empty */
        if(subTreePtr->data < (*ptrToTreePtr)->data)
        {/* subTreePtr data to insert is less than data in current node */
            insertSubTreeNode(&((*ptrToTreePtr)->leftPtr), subTreePtr);
        } /* end if */
        else if(subTreePtr->data > (*ptrToTreePtr)->data)
        {/* subTreePtr data to insert is greater than data in current node */
            insertSubTreeNode(&((*ptrToTreePtr)->rightPtr), subTreePtr);
        } /* end else if */
        else
        { /* duplicate data value ignored */
            //std::cout << "duplicate value: ignored";
            return 0;
        } /* end else */
    } /* end else */
    return 1;
}// end insertSubTreeNode()

int BinarySearchTree::deleteNodeAndInsertSubTrees(TreeNode** ptrToTreePtr, TreeNode*  populatedResultPtrToTreePtr[], const int & result)
{
    /* show results */
    resultOfgetNodesfromValue(populatedResultPtrToTreePtr, result);

    if(populatedResultPtrToTreePtr[3] != NULL) /* if_A */
    {/* The parent node should be detached before the searched-out data value node is deleted */
        if(0 == result)
            populatedResultPtrToTreePtr[3]->leftPtr = NULL;
        else if(1 == result)
            populatedResultPtrToTreePtr[3]->rightPtr = NULL;
    }/* end if_A */

    /* process result */
    if((populatedResultPtrToTreePtr[0] == NULL) && (populatedResultPtrToTreePtr[1] == NULL) && (populatedResultPtrToTreePtr[2] == NULL)) /* if_else_A_begins */
    {/* Case: 000, and note that Cases 001, 010 and 011 are not possible states as there cannot be child nodes without parent nodes*/
        std::cout << "0: Nothing was found (or deleted) and therefore, no left and right nodes are there for re-insertion." << std::endl;
        return 0; /* nothing was deleted and no other sub-tree nodes re-inserted */
    }/* if_else_A_contd. */
    else /* if_else_A_contd.: Cases: 1XX */
    { /* Cases: 1XX */
        if(*ptrToTreePtr == populatedResultPtrToTreePtr[0]) /* if_else_B_begins */
        {/* Cases: 1XX for the tree root node = populatedResultPtrToTreePtr[0] */

            delete *ptrToTreePtr; /* free the node whose data value was found */

            if((populatedResultPtrToTreePtr[1] == NULL) && (populatedResultPtrToTreePtr[2] == NULL)) /* if_else_C_begins */
            {/* Cases 100 */
                std::cout << "1: Node deleted was the root node and a leaf node as no other sub-tree nodes were re-inserted; therefore tree is now empty." << std::endl;
                return 1; /* node deleted was the root node and a leaf node as no other sub-tree nodes were re-inserted; therefore tree is now empty  */
            }/* if_else_C_contd_0 */
            else if((populatedResultPtrToTreePtr[1] == NULL) && (populatedResultPtrToTreePtr[2] != NULL))
            {/* Cases 101 */
                *ptrToTreePtr = populatedResultPtrToTreePtr[2]; /* the right sub-tree node replaces the root node and the left sub-tree node is NUL */
                std::cout << "2: Node deleted was the root node and the right sub-tree node replaced it as there is no left node." << std::endl;
                return 2; /* node deleted was the root node and only the right sub-tree node was re-inserted  */
            }/* if_else_C_contd_1 */
            else if((populatedResultPtrToTreePtr[1] != NULL) && (populatedResultPtrToTreePtr[2] == NULL))
            {/* Cases 110 */
                *ptrToTreePtr = populatedResultPtrToTreePtr[1]; /* the left sub-tree node replaces the root node and the right sub-tree node is NUL */
                std::cout << "3: Node deleted was the root node and the left sub-tree node replaced it as there is no right node." << std::endl;
                return 3; /* node deleted was the root node and only the left sub-tree node was re-inserted  */
            }/* if_else_C_contd_2 */
            else if((populatedResultPtrToTreePtr[1] != NULL) && (populatedResultPtrToTreePtr[2] != NULL))
            {/* Cases 111 */
                *ptrToTreePtr = populatedResultPtrToTreePtr[2]; /* the right sub-tree node replaces the and this is done firstly as its data value is always higher */
                insertSubTreeNode(ptrToTreePtr, populatedResultPtrToTreePtr[1]); /* then re-insert the left sub-tree node */
                std::cout << "4: Node deleted was the root node: the right sub-tree node replaced it and the left sub-tree node was re-inserted." << std::endl;
                return 4; /* node deleted was the root node and both the left and right sub-tree nodes was re-inserted  */
            }/* end if_else_C */
        } /* if_else_B_contd. */
        else  /* if_else_B_contd: if(*ptrToTreePtr != populatedResultPtrToTreePtr[0]) */
        {/* Cases: 1XX for the tree root node != populatedResultPtrToTreePtr[0] */

            delete populatedResultPtrToTreePtr[0]; /* free the node whose data value was found */

            if((populatedResultPtrToTreePtr[1] == NULL) && (populatedResultPtrToTreePtr[2] == NULL)) /* if_else_D_begins */
            {/* Cases 100 */
                std::cout << "5: Node deleted was a leaf node as no other sub-tree nodes were re-inserted." << std::endl;
                return 5; /* node deleted was a leaf node as no other sub-tree nodes were re-inserted.  */
            }/* if_else_D_contd_0 */
            if((populatedResultPtrToTreePtr[1] == NULL) && (populatedResultPtrToTreePtr[2] != NULL))
            {/* Cases 101 */
                insertSubTreeNode(ptrToTreePtr, populatedResultPtrToTreePtr[2]); /* re-insert only the right sub-tree node as the left sub-tree node is NUL */
                std::cout << "6: Node was deleted and the right sub-tree node was re-inserted as there is no left node." << std::endl;
                return 6; /* node was deleted and only the right sub-tree node was re-inserted  */
            }/* if_else_D_contd_1 */
            else if((populatedResultPtrToTreePtr[1] != NULL) && (populatedResultPtrToTreePtr[2] == NULL))
            {/* Cases 110 */
                insertSubTreeNode(ptrToTreePtr, populatedResultPtrToTreePtr[1]); /* re-insert only the left sub-tree node as the right sub-tree node is NUL */
                std::cout << "7: Node was deleted and the left sub-tree node was re-inserted as there is no right node." << std::endl;
                return 7; /* node was deleted and only the left sub-tree node was re-inserted  */
            }/* if_else_D_contd_2 */
            else if((populatedResultPtrToTreePtr[1] != NULL) && (populatedResultPtrToTreePtr[2] != NULL))
            {/* Cases 111 */
                insertSubTreeNode(ptrToTreePtr, populatedResultPtrToTreePtr[2]); /* re-insert the right sub-tree node firstly as its data value is always higher, and this tends to replace the place of the deleted node */
                insertSubTreeNode(ptrToTreePtr, populatedResultPtrToTreePtr[1]); /* then re-insert the left sub-tree node */
                std::cout << "8: Node was deleted and the left sub-tree node was re-inserted after right sub-tree node was re-inserted." << std::endl;
                return 8; /* node was deleted and both the left and right sub-tree nodes was re-inserted  */
            }/* end if_else_D */
        }/* end if_else_B */
    }/* end if_else_A */
    return -1; /* Error as control is NOT expected to reach here */
}// end deleteNodeAndInsertSubTrees()

int BinarySearchTree::balTreeHighestLevelIndex(TreeNode* treePtr)
{
    int log2Mantissa = 0;
    size_t quotient = numNodes;
    while(quotient >= 2)
    {
        quotient /= 2;
        log2Mantissa++;
    }// end while

    return log2Mantissa;
}// end balTreeHighestLevelIndex()

void BinarySearchTree::balTreeInitializeArrIdxCounter(int idxCounter[], const int & btNumLevel)
{
    int m;
    for(m = 0; m < btNumLevel; m++)
    {
        size_t maxNumElemAtLevel = 1 << m; // pow(2, m)
        int startingIdxAtLevel = maxNumElemAtLevel - 1;
        idxCounter[m] = startingIdxAtLevel - 1; // = initializedIdxAtLevel
    }// end for
}// end balTreeInitializeArrIdxCounter()

void BinarySearchTree::balTreePopulateIdxArr(const int & rowCounter, int idxCounter[], const int & arrBegIdx, const int & arrEndIdx, int result[])
{
    if(arrEndIdx < arrBegIdx) return;

    int i = rowCounter + 1; // go to next level(or row) since rowCounter was -1 initially
    idxCounter[i] += 1; //increment the array index counter (as they were all -1 initially) for this level in preparation for the next level
    int j = idxCounter[i];
    int midIdx = (arrBegIdx + arrEndIdx)/2;
    result[j] = midIdx;
    int arrSize = arrEndIdx - arrBegIdx + 1;

    if(arrSize > 2) // Begin if_else
    {
        balTreePopulateIdxArr(i, idxCounter, arrBegIdx, midIdx-1, result); // firstly, call the left side of the divide for proper indexing
        balTreePopulateIdxArr(i, idxCounter, midIdx+1, arrEndIdx, result); // then, call the right side of the divide
    }
    else if(arrSize == 2)
    {
        balTreePopulateIdxArr(i, idxCounter, arrEndIdx, arrEndIdx, result);
    }
    else if(arrSize == 1)
    {
        // DO NOTHING
    }// end if_else
}// end balTreePopulateIdxArr()

void BinarySearchTree::inOrderRetrieval(TreeNode* treePtr, int arrContainer[], int & arrIdxCounter)
{
    // if tree is not empty then traverse
   if(treePtr != NULL)
   {
        inOrderRetrieval(treePtr->leftPtr, arrContainer, arrIdxCounter);
        int i = ++arrIdxCounter;
        arrContainer[i] = treePtr->data;
	   std::cout << "\n i and arrContainer[i] are: " << i << " and " << arrContainer[i] << std::endl;
        inOrderRetrieval(treePtr->rightPtr, arrContainer, arrIdxCounter);
   } // end if
}// end inOrderRetrieval()

void BinarySearchTree::updateData(TreeNode* treePtr)
{
    //ptrToRootPtr = &rootPtr; // Not necessary
    lastLevelIndex = highestLevelIndex(treePtr, COUNTER_INITIAL_VALUE);
    numLevels = lastLevelIndex + 1;
    numElemsPerRow = 1 << lastLevelIndex; //pow(2, lastLevelIndex);

    numNodes = MASK_MAP_INITIALIZER;
    countNodes(treePtr, numNodes);

    delete [] columnCounter;
    columnCounter = new int[numLevels];
    initializeArrayInt(columnCounter, numLevels, COUNTER_INITIAL_VALUE);

    delete [] dataMask;
    dataMask = new int[numLevels*numElemsPerRow];
    initializeArrayInt(dataMask, numLevels*numElemsPerRow, MASK_MAP_INITIALIZER);

    delete [] dataMap;
    dataMap = new int[numLevels*numElemsPerRow];
    initializeArrayInt(dataMap, numLevels*numElemsPerRow, MASK_MAP_INITIALIZER);

    populateMask(treePtr, COUNTER_INITIAL_VALUE); // populate positional mask  and map matrices

    // always initialize columnCounter in readiness for the next method that will use it
    initializeArrayInt(columnCounter, numLevels, COUNTER_INITIAL_VALUE);

}// end updateData()

// move assignment operator
BinarySearchTree & BinarySearchTree::operator=(BinarySearchTree && orig)
{
    std::cout << "\nI am about moving in to a pre-existing object!" << std::endl;
    if(this == &orig) return *this;

    // steal tree pointer
    rootPtr = orig.rootPtr;
    ptrToRootPtr = &rootPtr;
    orig.rootPtr = NULL; // condemn

    // steal lastLevelIndex
    lastLevelIndex = orig.lastLevelIndex;
    orig.lastLevelIndex = 0; // condemn

    // steal numLevels
    numLevels = orig.numLevels;
    orig.numLevels = 0; // condemn

    // steal numNodes
    numNodes = orig.numNodes;
    orig.numNodes = 0; // condemn

    // steal numElemsPerRow
    numElemsPerRow = orig.numElemsPerRow;
    orig.numElemsPerRow = 0; // condemn

    delete [] columnCounter; // free data
    columnCounter = orig.columnCounter; // steal columnCounter
    orig.columnCounter = NULL; // condemn

    delete [] dataMask; // free data
    dataMask = orig.dataMask; // steal dataMask
    orig.dataMask = NULL; // condemn

    delete [] dataMap; // free data
    dataMap = orig.dataMap; // steal dataMap
    orig.dataMap = NULL; // condemn

    std::cout << "Oh! I am through with moving!!!!!" << std::endl;
    return *this;
}// move assignment operator=()

void BinarySearchTree::inOrder(TreeNode* treePtr)
{
    // if tree is not empty then traverse
    if(treePtr != NULL)
    {
        inOrder(treePtr->leftPtr);
        std::cout.width(4); std::cout << treePtr->data;
        inOrder(treePtr->rightPtr);
    } // end if
}// end inOrder()

void BinarySearchTree::preOrder(TreeNode* treePtr)
{
    // if tree is not empty then traverse
   if(treePtr != NULL)
   {
      std::cout.width(4); std::cout << treePtr->data;
      preOrder(treePtr->leftPtr);
      preOrder(treePtr->rightPtr);
   } // end if
}// end preOrder()

void BinarySearchTree::postOrder(TreeNode* treePtr)
{
    // if tree is not empty then traverse
    if(treePtr != NULL )
    {
        postOrder(treePtr->leftPtr);
        postOrder(treePtr->rightPtr);
        std::cout.width(4); std::cout << treePtr->data;
   } // end if
}// end postOrder()

void BinarySearchTree::destroyTree(TreeNode* treePtr)
{
    // if tree is not empty then traverse
    if(treePtr != NULL)
    {
        destroyTree(treePtr->leftPtr);
        destroyTree(treePtr->rightPtr);
        std::cout << "Node of " << treePtr->data << " freed!" << std::endl;
        delete treePtr;
    } /* end if */
}// end destroyTree()

void BinarySearchTree::insertNodeAndUpdate(TreeNode** ptrToTreePtr, int value)
{
    insertNode(ptrToTreePtr, value);
    updateData(*ptrToTreePtr);
}// end insertNodeAndUpdate()

int BinarySearchTree::getNodeDeleteAndInsertSubTrees(TreeNode** ptrToTreePtr, const int & value, const int & rowCounter)
{
    TreeNode* resultPtrToTreePtr[4] = {NULL, NULL, NULL, NULL}; /* scratch array to be populated by getNodesfromValue() and getNodefromValue() */
    int parentValue; /* The search value parent's node data value will be to be populated by getNodesfromValue() */
    int searchResult; /* scratch variable to hold the return value from getNodesfromValue() */

    searchResult = getNodesfromValue(*ptrToTreePtr, value, resultPtrToTreePtr, parentValue, rowCounter); /* populate array elements resPtrToTreePtr[0 to 2] and parentValue */
    getNodefromValue(*ptrToTreePtr, parentValue, &resultPtrToTreePtr[3]); /* populate resultPtrToTreePtr[3] */
    deleteNodeAndInsertSubTrees(ptrToTreePtr, resultPtrToTreePtr, searchResult); /* show results  and relocate detached */
    updateData(*ptrToTreePtr);

    return parentValue;
}// end getNodeDeleteAndInsertSubTrees()

void BinarySearchTree::balanceTree(TreeNode** ptrToTreePtr)
{
    int llv = balTreeHighestLevelIndex(*ptrToTreePtr);

    std::cout << "\n\n numNodes, llv are: " << numNodes << ", " << llv << "\n" << std::endl;

    int* temp = new int[numNodes]; //int temp[numNodes];
    int nodeCounter = COUNTER_INITIAL_VALUE;
    // populate temp array with tree elements already sorted in ascending order
    inOrderRetrieval(*ptrToTreePtr, temp, nodeCounter);

     std::cout << "\nAt the end, nodeCounter is: " << nodeCounter << "\n" << std::endl;

     int mySize = llv + 1;
	 int* idxCter = new int[mySize]; //int idxCter[mySize];
     balTreeInitializeArrIdxCounter(idxCter, mySize); // initialize idxCter[]
     std::cout << "\n\n Show initialized idxCter[mySize] array:\n";
     for(int i = 0; i < mySize; i++)
            std::cout << idxCter[i] << std::endl;

     int rCounter = COUNTER_INITIAL_VALUE;
	 int* res = new int[numNodes]; //int res[numNodes]; // will hold indices that will ensure a balanced tree
     balTreePopulateIdxArr(rCounter, idxCter, 0, numNodes-1, res);// populate res[] with those indices

     //In order to use memory efficiently, free up the old tree b/4 creating a new one
    destroyTree(*ptrToTreePtr);

    //MAKE TREE AFRESH WITH THE OLD ELEMENTS VALUES
    TreeNode* root2Ptr = NULL; /* tree initially empty */
    TreeNode** ptrToRoot2Ptr = &root2Ptr;

    size_t i;
    for(i = 0; i < numNodes; i++)
    {
        int arrIdx = res[i];
        insertNode(ptrToRoot2Ptr, temp[arrIdx]);
        std::cout << "\n arrIdx and temp[arrIdx] are: " << arrIdx << " and " << temp[arrIdx] << std::endl;
    }// end for

    // point to the new tree
    *ptrToTreePtr = root2Ptr;

    // update class members
    updateData(*ptrToTreePtr);

	delete [] temp; delete [] idxCter; delete [] res;
}// end balanceTree()

// destructor
BinarySearchTree::~BinarySearchTree()
{
    TreeNode* treePtr = rootPtr; // assign member bcos it will be used recursively

    std::cout << "\nDestroying Tree.....!" << std::endl;
    destroyTree(treePtr);

    // free dynamically allocated memory
    std::cout << "Freeing..... columnCounter!" << std::endl;
    delete [] columnCounter; // free data

    std::cout << "Freeing..... dataMask!" << std::endl;
    delete [] dataMask; // free data

    std::cout << "Freeing..... dataMap!" << std::endl;
    delete [] dataMap; // free data

    std::cout << "All freed!!!!!!\n" << std::endl;
}// end destructor

// friends
void initializeArrayInt(int* arr, int arrSize, int value)
{
    int i;
    for(i=0; i<arrSize; i++)
    {
        *(arr + i) = value;
    }/*end for*/
}// end initializeArrayInt()

void print2dArrayInt(int* arrRow0Address, int numRows, int numCols)
{
    int i, j;
    for(i=0; i<numRows; i++)
    {
        for(j=0; j<numCols; j++)
        {
            std::cout << std::setw(4) << *(arrRow0Address + i*numCols + j);
        }/*end for*/
        std::cout << std::endl;
    }/*end for*/
    std::cout << std::endl;
}// print2dArrayInt()
