/*
 * File:   render.h
 * Author: Emmanuel Chidinma
 * emmanuel.c.chidinma@gmail.com
 * Created on November 27, 2017, 2:42 PM
 */

#include "render.h"
//#include <cmath>

const unsigned short NUMDP = 6; // used by numToCstring()

extern BinarySearchTree bsTree; // re-declaration to give bsTree (used in draw()) an external linkage 
// bsTree is initially declared and populated in bsTree.cpp

float xCoordStart(int lastLevelIndex, int rowCounter)
{
    if(rowCounter > lastLevelIndex)
    {
        return -1.0f; /*error*/
    }
    else if(rowCounter == lastLevelIndex)
    {
        return 0.0f;
    }
    else
    {
        float m = xCoordStart(lastLevelIndex, rowCounter+1); // the starting X coordinate of the next higher level
        float n = m + xCoordOffset(lastLevelIndex, rowCounter+1, 1); // the X coord following the starting X coordinate for the next higher level: or simply start X + offset
        return (m + n)/2.0f;
    }/*end else_if*/
}/* end xCoordStart()*/

float xCoordOffset(int lastLevelIndex, int rowCounter, int columnCounter)
{
     return (float) (1 << (lastLevelIndex-rowCounter))*columnCounter; //return pow(2, (lastLevelIndex - rowCounter))*columnCounter;
}/* end xCoordOffset()*/

float xCoord(int lastLevelIndex, int rowCounter, int columnCounter)
{
    return xCoordStart(lastLevelIndex, rowCounter) + xCoordOffset(lastLevelIndex, rowCounter, columnCounter);
}/* end xCoord()*/

float yCoord(int lastLevelIndex, int rowCounter)
{
    return (float) lastLevelIndex - rowCounter;
}/* end yCoord()*/

void populateCoords(float* yCoordinates, float* xCoordsRow0Address, int numRows, int numCols)
{
    int i, j;
    for(i=0; i<numRows; i++)
    {
        *(yCoordinates + i) = yCoord(numRows-1, i); // deduce the y coordinate
        int colLimit = 1 << i; //pow(2, i);
        for(j=0; j<colLimit; j++)
        {
            *(xCoordsRow0Address + i*numCols + j) = xCoord(numRows-1, i, j); // deduce the x coordinate
        }/*end for*/
    }/*end for*/
}/* end populateCoords()*/

void initializeArrayFloat(float* arr, int arrSize, float value)
{
    int i;
    for(i=0; i<arrSize; i++)
    {
        *(arr + i) = value;
    }/*end for*/
}/* end initializeArrayFloat()*/

void print2dArrayFloat(float* arrRow0Address, int numRows, int numCols)
{
    int i, j;
    for(i=0; i<numRows; i++)
    {
        for(j=0; j<numCols; j++)
        {
            std::cout.setf(std::ios_base::fixed, std::ios_base::floatfield);
            std::cout << std::setw(5) << std::setprecision(1) << *(arrRow0Address + i*numCols + j);
        }/*end for*/
        std::cout << std::endl;
    }/*end for*/
    std::cout << std::endl;;
}/* print2dArrayFloat() */

char* numToCstring(double num /*, size_t* strLength*/)
{
    int signOfNum = (num < 0) ? -1 : 1;

	double absNum = num * signOfNum; // work with a +ve number: the abs(num)

    // separate absNum in to its whole and decimal parts.
    long wholePart = (long)absNum;
    double decimalPart = absNum - (double)wholePart;

    int isNumAfloat = (decimalPart > 0) ? 1 : 0;

    std::deque<char> coll;

    // 1. Thaw each of the digits (starting from the LSB) of wholePart and save in coll
    short remainder;
	long quotient = wholePart;
    do
	{
		remainder = quotient % 10;
		quotient = quotient / 10;
		coll.push_back((char)(remainder + 48)); // 48 is the ascii number of character 0
	}while(quotient != 0);// end do-while

    // 2. put the -ve character at the back if num is a -ve number
    if(signOfNum == -1)coll.push_back('-');

    // 3. Thaw each of the digits (starting from the MSB) of decimalPart and save in coll

     if(isNumAfloat == 1) // true if num is a floating point number and not an integer number
     {
        coll.push_front('.'); // put the . character at the front if num is a floating number
        size_t idxOfPoint = 0; // index of the the . character in coll

        short mantissa;
        double product = decimalPart;
        do
        {
            product = product * 10; // push the MSB of the decimal part in to the whole part
            mantissa = (short)product; // extract the integer (whole) part
            product = product - (double)mantissa; // remove the extracted integer part and re-assign
            coll.push_front((char)(mantissa + 48)); // 48 is the ascii number of character 0
            idxOfPoint++; // update index of the the . character in coll
        }while(product > 0.0);// end do-while // comparison with floating point zero value here introduces approx. errors.

        // use Artifical Intelligence to round up the decimal part in an attempt to get the exact num value and not an approximation
        floatNumRoundUpAI(coll, idxOfPoint);

     }//end if

     // 4. populate this function argument
     size_t length = coll.size();

     char* res = new char[sizeof(char)*length + 1]; // the result C String: allocate it dynamically

     // 5. Populate the result C String
     size_t i = 0; // index
     while(!coll.empty())
     {
        char lastElement = coll.back(); //read element from back of coll
        res[i++] = lastElement;
        coll.pop_back();// remove the read-element from the back of container
     }// end while
     res[i] = '\0'; // terminate C String

     return res;

}// end numToCstring()

void floatNumRoundUpAI(std::deque<char> & cont, size_t indexOfPoint)
{
    size_t i, j;
    bool wasRounded = false; // round up flag

    // check for occurrence of ".....999X..." where X is any other number xter apart from 9
    // NB: THIS AI IS HEAVILY BIASEDIN TAKING CARE OF DOUBLE TYPES
    for(i=0; indexOfPoint>=4 && i<=indexOfPoint-4; i++) // for_A // indexOfPoint>=4 bcos we are testing the 4 xters before the . xter
    {
        if(cont[i] == '9' && cont[i+1] == '9' && cont[i+2] == '9' && cont[i+3] != '9') // if_A
        {
            cont[i+3] += 1; // round up this character
            // discard all xters before index i+3
            for(j=0; j<=i+2; j++) // for_B
            {
                cont.pop_front();// remove the first i+2 element from the container
            }// end for_B
            wasRounded = true;
            break;
        }// end if_A
    }// end for_A

    // WITH NUMDP CHOSEN AS 6, THIS SECTION OF THE FUNCTION TAKES CARE OF FLOAT TYPES
    // AS THE MAXIMIM ALLOWABLE NUMBER OF DECIMAL PLACES OF FLOAT TYPE IS 6
    if(!wasRounded) // if_B: true if round up did not take place
    {
        // discard all xters before 6 (NUMDP) places to the . character
        for(j=0; j<=indexOfPoint-(NUMDP+1); j++) // for_C
        {
            cont.pop_front();
        }// end for_C

        //float types internally have the tendency for the 6th decimal place to be 1 when the
        // actually specified test number is not up to 6 decimal places
        if(!cont.empty() && (('1' == cont[0]) && ('0' == cont[1]))) // if_C
        {
            cont.pop_front();
        }// end if_C

        // remove all remaining initial '0' xters if testNum's number of decimal places is less than 6 (NUMDP)
        while(!cont.empty() && ('0' == cont.front())) // cont.front() OR cont[0]
        {
            cont.pop_front();
        }// end while
    }// end if_B
}// end floatNumRoundUpAI()

// OpenGL functions
void writeText(char str[])
{
	int numElemsPerRow = bsTree.getNumElemsPerRow();
	void* font;

	if (numElemsPerRow <= 4) font = GLUT_BITMAP_TIMES_ROMAN_24; // llv <= 2
	else if (numElemsPerRow == 8) font = GLUT_BITMAP_HELVETICA_18; // llv = 3
	else if (numElemsPerRow == 16) font = GLUT_BITMAP_HELVETICA_12; // llv = 4
	else if (numElemsPerRow == 32) font = GLUT_BITMAP_9_BY_15; // llv = 5
	else /*if (numElemsPerRow > 32) */ font = GLUT_BITMAP_8_BY_13; // llv > 5

	char* p;
	for (p = str; *p; p++)
	{
		glutBitmapCharacter(font, *p);
	}// end for
}// end writeText()

void drawAxes(void)
{
   // draw the +ve axes for clarity
   float xr = 20.8f, yr = 20.8f, zr = 20.8f;
   glLineWidth (1);
   glColor3f(1.0f, 1.0f, 1.0f);           // set line colour
   glBegin( GL_LINES );
      glVertex3f(0.0f, 0.0f, 0.0f); 
      glVertex3f((xr-0.3f), 0.0f, 0.0f);
      glVertex3f(0.0f, 0.0f, 0.0f); 
      glVertex3f(0.0f, (yr-0.3f), 0.0f); 	  
      glVertex3f(0.0f, 0.0f, 0.0f); 
      glVertex3f(0.0f, 0.0f, (xr-0.3f));
    glEnd();

    // label the axes
   glColor3f(0.0f, 0.0f, 0.0f);            // set line colour
   glRasterPos3f (xr, 0.0f, 0.0f);
   writeText( "x" );
   glRasterPos3f (0.0f, yr, 0.0f);
   writeText( "y" );
   glRasterPos3f (0.0f, 0.0f, zr);
   writeText( "z" );
}

//initialization
void init()
{
  //glClearColor( 1.0, 1.0, 1.0, 0.0 );	//get white background color
  glClearColor( 0.0, 0.0, 0.0, 0.0 );	//get black background color
  glColor3f( 0.0f, 0.0f, 0.0f );	//set default drawing color to black
}//  end init()

void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D (-2.0f, 2.0f, -2.0f, 2.0f);
	glMatrixMode (GL_MODELVIEW);
}// end reshape()

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	int numLevels = bsTree.getNumLevels();
	int numElemsPerRow = bsTree.getNumElemsPerRow();
	gluOrtho2D(-1.0, numElemsPerRow, -1.0, numLevels);  

	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();            

	glPushMatrix();
 
	// DRAW AXES 
	//drawAxes();

	// DRAW Objects 
	draw();

	glPopMatrix();
   
	// RENDER TO THE FRAME BUFFER 
	glFlush (); // send to framebuffer to display drawing
}// end  display()

void draw()
{
	int llv = bsTree.getLastLevelIndex();
	int numLevels = bsTree.getNumLevels();
	int numElemsPerRow = bsTree.getNumElemsPerRow();
	int* colCounter = bsTree.getColumnCounter();
	int* mask2dArr = bsTree.getDataMask();
	int* map2dArr = bsTree.getDataMap();

	// draw title
	if(numElemsPerRow >= 8)
	{
		glColor3f(1.0f, 1.0f, 1.0f); // set color white
		glRasterPos2f(((numElemsPerRow/2) - 3), numLevels - 0.5f);
		writeText("BINARY TREE VISUALIZATION");
	}// end if

	//ptAtI[2] is the coordinate point at level i (child's level) while ptAtIp[2] is the coordinate point at level i-i (parent's level)
	int i, j; float ptAtI[2];  float ptAtIp[2]; const float delFactor = 0.2f;

	for(i=0; i<numLevels; i++) // for_i
    {
        int colLimit = 1 << i; //pow(2, i);
        for(j=0; j<colLimit; j++) // for_j
		{
			if(1 == *(mask2dArr + i*numElemsPerRow + j))  // begin if_A
			{
				ptAtI[0] = xCoord(llv, i, j);
				ptAtI[1] = yCoord(llv, i);

				// draw a point at the coordinate
				glColor3f(0.0f, 1.0f, 0.0f); // set color green
				glPointSize( 4.0 );			//a dot is 4x4 pixels
				glEnable(GL_POINT_SMOOTH); // round points desired and not square ones
				glBegin(GL_POINTS);
					glVertex2fv(ptAtI);
				glEnd();

				// write the node data value at the coordinate position
				//glColor3f(1.0f, 0.0f, 0.0f); // set color red
				glColor3f(1.0f, 1.0f, 1.0f); // set color white
				int iNodeValue = *(map2dArr + i*numElemsPerRow + j); // get node data value
				char* strNodeValue = numToCstring(iNodeValue);
				glRasterPos2f(ptAtI[0], ptAtI[1]);
				writeText(strNodeValue);

				if(!(i==0 && j==0)) // begin if_B
				{
					float offsetI = xCoordOffset(llv, i, 1);
					int delSign = (j%2 == 0) ? 1 : -1;

					ptAtIp[0] = ptAtI[0] + (delSign * offsetI)/2;
					ptAtIp[1] = yCoord(llv, i-1);

					enhanceCoords(ptAtI, ptAtIp, delSign, delFactor);

					// draw line from child to parent
					//glColor3f(0.0f, 0.0f, 1.0f); // set color blue
					glColor3f(1.0f, 1.0f, 0.0f); // set color yellow
					glLineWidth(2); //set drawing line width
					glBegin( GL_LINES );			
						glVertex2fv(ptAtI);		
						glVertex2fv(ptAtIp);
					glEnd();
				}// end if_B
				delete [] strNodeValue; // free up memory cos strNodeValue was allocated dynamically within numToCstring()
			}// end if_A
		}// end for_j
	}// end for_i

}// end draw()

void enhanceCoords(float ptChild[], float ptParent[], const int & deltaSign, const float & deltaFactor)
{
	if(deltaFactor < 0.0f || deltaFactor > 0.4f) return; // do not enhance coordinates if true

	float absXrange = (ptParent[0] - ptChild[0]) * deltaSign;
	float delta = absXrange * deltaSign * deltaFactor;
	float m = (ptParent[1] - ptChild[1])/(ptParent[0] - ptChild[0]); // slope
	float c = ptChild[1] - m*ptChild[0]; // intercept on y-axis

	// deduce enhanced X coordinates
	ptChild[0] = ptChild[0] + delta;
	ptParent[0] = ptParent[0] - delta;

	// deduce enhanced Y coordinates
	ptChild[1] = m*ptChild[0] + c;
	ptParent[1] = m*ptParent[0] + c; 
}// end enhanceCoords()


void balanceTheTree()
{
	BinarySearchTree::TreeNode** ptrToTreePtr = bsTree.getPtrToRootPtr();
	bsTree.balanceTree(ptrToTreePtr);
	glutPostRedisplay();
}// end balanceTheTree()

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27: // escape key
			exit(0);
			break;
		case 'v':
		case 'V':
			balanceTheTree();
			break;
		default:
			break;
	}// end switch
}// end keyboard()





