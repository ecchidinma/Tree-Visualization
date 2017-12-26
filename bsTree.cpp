/*
 * File:   bsTree.cpp : Defines the entry point for the application.
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on November 27, 2017, 2:42 PM
 */

//#include <gl/glut.h> // included in render.h
//#include <iostream> // included in render.h
//#include <cstdlib> // included in render.h
//#include <ctime> // included in render.h
//#include "BinarySearchTree.h" // included in render.h
#include "render.h"

int width = 1200, height = 300;
const size_t NUMNODES = 80;

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGB display mode, depth buffer.
 *
 *  Press 'v' or 'V' to balance the tree and re-display
 *
 */

void populateTree(); // Tree object populating function

BinarySearchTree bsTree; // global declaration: also re-declared in render.cpp

int main(int argc, char* argv[])
{
	populateTree();

	glutInit(&argc, argv);	        //initialize toolkit
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB ); //set display mode
	glutInitWindowSize(width, height);		//set window size on screen
	glutInitWindowPosition( 100, 50 ); 	//set window position on screen
	glutCreateWindow(argv[0]);		//open screen widow
	init();                               //customized initialization
	glutDisplayFunc (display);		//points to display function
	glutKeyboardFunc(keyboard);
	glutMainLoop();			//go into perpetual loop
	return 0;
}

void populateTree()
{
	// populate bsTree object
	int i; /* loop counter */
	int item[NUMNODES]; /* array to hold random values */

	//BinarySearchTree::TreeNode* treePtr = bsTree.getRootPtr();
	BinarySearchTree::TreeNode** ptrToTreePtr = bsTree.getPtrToRootPtr();

	//srand(time(NULL)); /* comment out if the same set of numbers are desired to be generated every time*/
	std::cout << "The numbers being placed in the tree are:\n" << std::endl;
	// insert random values 
	for (i = 0; i < NUMNODES; i++)
	{
		item[i] = (rand() % 130) - 30; /* generate from -30 to 99 */
		std::cout << std::setw(4) << item[i];
		bsTree.insertNodeAndUpdate(ptrToTreePtr, item[i]);
	} /* end for */
	std::cout << std::endl;
}// end populateTree()






