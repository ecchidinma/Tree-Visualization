/*
 * File:   render.h
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on November 27, 2017, 2:42 PM
 */

#ifndef RENDER_H
#define	RENDER_H

#include <GL/glut.h>
#include <iostream>
#include <iomanip>
#include <cstdlib> // for rand()
#include <ctime> // for time() 
#include <deque> // used in numToCstring() and floatNumRoundUpAI()
#include "BinarySearchTree.h"

// OpenGL helper rendering functions
float xCoordStart(int lastLevelIndex, int rowCounter);
float xCoordOffset(int lastLevelIndex, int rowCounter, int columnCounter);
float xCoord(int lastLevelIndex, int rowCounter, int columnCounter);
float yCoord(int lastLevelIndex, int rowCounter);
void populateCoords(float* yCoordinates, float* xCoordsRow0Address, int numRows, int numCols);
void initializeArrayInt(int* arr, int arrSize, int value);
void initializeArrayFloat(float* arr, int arrSize, float value);
void print2dArrayInt(int* arrRow0Address, int numRows, int numCols);
void print2dArrayFloat(float* arrRow0Address, int numRows, int numCols);
char* numToCstring(double num  /*, size_t* strLength*/);
void floatNumRoundUpAI(std::deque<char> & cont, size_t indexOfPoint);

// OpenGL core functions
void writeText(char str[]);
void drawAxes(void);
void init();
void reshape(int w, int h);
void display();
void draw();
void enhanceCoords(float ptChild[], float ptParent[], const int & deltaSign, const float & deltaFactor);
void balanceTheTree();
void keyboard(unsigned char key, int x, int y);


#endif	/* RENDER_H */

