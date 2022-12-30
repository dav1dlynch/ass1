//OpenGL Empty Project

#include <stdio.h>     // - Just for some ASCII messages
#include <iostream>
#include "gl/glut.h"   // - An interface and windows 
#include <vector>
#include <array>
#include <cmath>


using namespace std;



bool clearScreen = true, polygonDraw = false, pSelect = false, pMove = false, pRotate = false;//menu choices
float rl = 0.0, gl = 0.0, bl = 0.0;//rgb values for line color
float rf = 1.0, gf = 1.0, bf = 1.0;//rgb values for fill color
float sr = 0.67843137, sg = 0.8470588, sb = 0.9019608;//rgb values for selection color

int winw, winh; // window width and height
double mousex, mousey, motionx, motiony;//mouse coordinates
double distx = 0, disty = 0;//distance to move
bool mouseleftpressed = false, mouserightpressed = false, enterpressed = false;//mouse statuses
double angle = 0.0; 
bool selectionstate = false;//whether a polygon is selected
int selected;//the selcted polygon

// Keyboard
const int ENTERKEY = 13;

// window 
const int const1 = 600;
const int const2 = 500;

struct Points
{
	double px, py;


};

Points clickedPoint; //used for rotation

vector<Points> polygon;		//the individual polygon the user is in the process of drawing

vector<vector<Points>> listOfPolygons;	//all the polygons drawn in the scene

vector<array<array<double, 3>, 2>> listOfColors;		//all the colors used for the polygons drawn in the scene



void MenuSelect(int choice)
{

	switch (choice)
	{
	case 1:						//Create Polygon
		polygonDraw = true;
		pSelect = false;
		pMove = false;
		pRotate = false;
		mouseleftpressed = false;
		enterpressed = false;
		break;
	case 2:						//Select Polygon
		polygonDraw = false;
		pSelect = true;
		pMove = false;
		pRotate = false;
		mouseleftpressed = false;
		enterpressed = false;
		break;
	case 3:						//Move Polygon
		polygonDraw = false;
		pSelect = false;
		pMove = true;
		pRotate = false;
		mouseleftpressed = false;
		enterpressed = false;
		break;
	case 4:						//Rotate Polygon
		polygonDraw = false;
		pSelect = false;
		pMove = false;
		pRotate = true;
		mouseleftpressed = false;
		enterpressed = false;
		break;
	case 5:						//Clear Screen
		clearScreen = true;
		polygonDraw = false;
		pSelect = false;
		pMove = false;
		pRotate = false;
		mouseleftpressed = false;
		enterpressed = false;
		break;
	case 6:						//Exit
		exit(0);
	case 7:						//Line Color Black
		rl = 0.0;
		gl = 0.0;
		bl = 0.0;
		break;
	case 8:						//Line Color White
		rl = 1.0;
		gl = 1.0;
		bl = 1.0;
		break;
	case 9:						//Line Color Blue
		rl = 0.0;
		gl = 0.0;
		bl = 1.0;
		break;
	case 10:					//Line Color Red
		rl = 1.0;
		gl = 0.0;
		bl = 0.0;
		break;
	case 11:					//Line Color Green
		rl = 0.0;
		gl = 1.0;
		bl = 0.0;
		break;
	case 12:					//Line Color Yellow
		rl = 1.0;
		gl = 1.0;
		bl = 0.0;
		break;
	case 13:					//Line Color Purple
		rl = 1.0;
		gl = 0.0;
		bl = 1.0;
		break;
	case 14:					//Line Color Grey
		rl = 0.5;
		gl = 0.5;
		bl = 0.5;
		break;
	case 15:					//Fill Color Black
		rf = 0.0;
		gf = 0.0;
		bf = 0.0;
		break;
	case 16:					//Fill Color White
		rf = 1.0;
		gf = 1.0;
		bf = 1.0;
		break;
	case 17:					//Fill Color Blue
		rf = 0.0;
		gf = 0.0;
		bf = 1.0;
		break;
	case 18:					//Fill Color Red
		rf = 1.0;
		gf = 0.0;
		bf = 0.0;
		break;
	case 19:					//Fill Color Green
		rf = 0.0;
		gf = 1.0;
		bf = 0.0;
		break;
	case 20:					//Fill Color Yellow
		rf = 1.0;
		gf = 1.0;
		bf = 0.0;
		break;
	case 21:					//Fill Color Purple
		rf = 1.0;
		gf = 0.0;
		bf = 1.0;
		break;
	case 22:					//Fill Color Grey
		rf = 0.5;
		gf = 0.5;
		bf = 0.5;
		break;

	}

	glutPostRedisplay();

}
void createMenu()
{

	int linecolor = glutCreateMenu(MenuSelect);
	glutAddMenuEntry("Black", 7);
	glutAddMenuEntry("White", 8);
	glutAddMenuEntry("Blue", 9);
	glutAddMenuEntry("Red", 10);
	glutAddMenuEntry("Green", 11);
	glutAddMenuEntry("Yellow", 12);
	glutAddMenuEntry("Purple", 13);
	glutAddMenuEntry("Grey", 14);

	int fillcolor = glutCreateMenu(MenuSelect);
	glutAddMenuEntry("Black", 15);
	glutAddMenuEntry("White", 16);
	glutAddMenuEntry("Blue", 17);
	glutAddMenuEntry("Red", 18);
	glutAddMenuEntry("Green", 19);
	glutAddMenuEntry("Yellow", 20);
	glutAddMenuEntry("Purple", 21);
	glutAddMenuEntry("Grey", 22);

	glutCreateMenu(MenuSelect);
	glutAddMenuEntry("Polygon", 1);
	glutAddSubMenu("LineColor", linecolor);
	glutAddSubMenu("FillColor", fillcolor);
	glutAddMenuEntry("PolygonSelect", 2);
	glutAddMenuEntry("Move", 3);
	glutAddMenuEntry("Rotate", 4);
	glutAddMenuEntry("Clear", 5);
	glutAddMenuEntry("Exit", 6);

	// attach the menu to the right button
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void Mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{

		if (pMove && state == GLUT_DOWN) {			//drag and drop for moving
			motionx = double(x) / winw;
			motiony = 1 - double(y) / winh;
			distx = 0;
			disty = 0;
		}
		else if (pMove && state == GLUT_UP) {
			distx = motionx - (double(x) / winw);
			disty = motiony - (1 - double(y) / winh);

		}
		
		if (pRotate && state == GLUT_DOWN) { // rotation mode
			clickedPoint.px = x;
			clickedPoint.py = y;
		}

		mouseleftpressed = (state == GLUT_UP);
		glutPostRedisplay();
	}
	else if (button == GLUT_RIGHT_BUTTON)
	{
		mouserightpressed = (state == GLUT_UP);
		glutPostRedisplay();
	}

	//converting and saving the mouse position
	mousex = double(x) / winw;
	mousey = 1 - double(y) / winh;

}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case ENTERKEY:
		enterpressed = true;
		break;
		/*	case 'x':
				exit(0);*/
	}
	glutPostRedisplay();
}


bool validPointCalculation(Points a, Points b, Points c, Points d)
{

	if ((b.px - a.px) * (d.py - c.py) == (b.py - a.py) * (d.px - c.px)) {		//checking if line segments are parallel
		if ((a.px - b.px) * (a.py - c.py) == (a.py - b.py) * (a.px - c.px)) {	//checking if they are collinear

			if (pSelect) {
				return false;//because the polygons are not selfintesecting or concave a point cannot be inside the polygon if collinear with one of the sides
			}
			else {
				//checking if they overlap
				return (min(a.px, b.px) <= max(c.px, d.px) &&
					max(a.px, b.px) >= min(c.px, d.px) &&
					min(a.py, b.py) <= max(c.py, d.py) &&
					max(a.py, b.py) >= min(c.py, d.py));
			}
		}
		else {				// If the line segments are parallel but not collinear, they do not intersect
			return false;
		}
	}
	else {			//finding intersection point
		double t = ((a.px - c.px) * (c.py - d.py) - (a.py - c.py) * (c.px - d.px)) / ((a.px - b.px) * (c.py - d.py) - (a.py - b.py) * (c.px - d.px));
		double u = -((a.px - b.px) * (a.py - c.py) - (a.py - b.py) * (a.px - c.px)) / ((a.px - b.px) * (c.py - d.py) - (a.py - b.py) * (c.px - d.px));

		if (pSelect) {
			if ((0 < t && t < 1) && (0 < u && u < 1)) {
				return true;
			}
			return false;
		}
		else if ((0 < t && t < 1) || (0 < u && u < 1)) {		//checking if intersection is on the line segments 
			return true;
		}
		else {
			return false;
		}

	}



}


void drawListOfPolygons() {

	double currentcol[2][3] = { {rl,gl,bl},{rf,gf,bf} };
	int count = 0;
	if (selectionstate) {
		for (auto polygon : listOfPolygons) {

			if (count != selected) {
				glColor3f(listOfColors[count][1][0], listOfColors[count][1][1], listOfColors[count][1][2]);
			}
			else {//selection color
				glColor3f(sr, sg, sb);
			}
			glBegin(GL_POLYGON);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();
			if (count != selected) {
				glColor3f(listOfColors[count][0][0], listOfColors[count][0][1], listOfColors[count][0][2]);
			}
			else {//selection color
				glColor3f(sr, sg, sb);
			}
			glBegin(GL_LINE_LOOP);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();

			count++;
		}
	}
	else {
		for (auto polygon : listOfPolygons) {

			glColor3f(listOfColors[count][1][0], listOfColors[count][1][1], listOfColors[count][1][2]);

			glBegin(GL_POLYGON);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();

			glColor3f(listOfColors[count][0][0], listOfColors[count][0][1], listOfColors[count][0][2]);

			glBegin(GL_LINE_LOOP);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();

			count++;
		}
	}


}


void createPolygon() {

	//glClear(GL_COLOR_BUFFER_BIT);

	if (mouseleftpressed) {

		Points p;
		p.px = mousex;
		p.py = mousey;

		bool validPoint = true;

		if (polygon.size() > 2) {

			for (int i = 0; i < polygon.size() - 1; i++) {


				if (validPointCalculation(polygon[i], polygon[i + 1], polygon[polygon.size() - 1], p)) {

					validPoint = false;
				}

			}
		}

		if (validPoint) {

			polygon.push_back(p);

			glColor3f(rl, gl, bl);
			/*glBegin(GL_POINTS);
			glVertex2d(p.px, p.py);
			glEnd();*/


			glBegin(GL_LINE_STRIP);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();
		}
		else {

			polygon.clear();

			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			drawListOfPolygons();

		}


		mouseleftpressed = false;
	}
	if (enterpressed) {

		bool validPoly = true;

		for (int i = 1; i < polygon.size() - 2; i++) {


			if (validPointCalculation(polygon[i], polygon[i + 1], polygon.back(), polygon[0])) {

				validPoly = false;
			}

		}

		if (validPoly) {

			glColor3f(rf, gf, bf);

			glBegin(GL_POLYGON);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();

			glColor3f(rl, gl, bl);

			glBegin(GL_LINE_LOOP);
			for (auto point : polygon) {

				glVertex2d(point.px, point.py);

			}
			glEnd();

			array<array<double, 3>, 2> col = { { {rl,gl,bl},{rf,gf,bf} } };

			listOfColors.push_back(col);
			listOfPolygons.push_back(polygon);
			polygon.clear();

		}
		else {
			polygon.clear();

			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			drawListOfPolygons();
		}



		enterpressed = false;
	}



}

void Rotate() {
	if (selectionstate) {
		float angle;
		angle = atan2(clickedPoint.py, clickedPoint.px);		//angle of rotation
		angle = angle * 3.14 / 180.0;
		vector<Points> rpolygon;								// new rotated polygon 

		for (auto point : polygon) {
			Points newPoint;									// rotated point 

			// the known transform for rotation
			newPoint.px = point.px * cos(angle) - point.py * sin(angle);
			newPoint.py = point.px * sin(angle) + point.py * cos(angle);


			rpolygon.push_back(newPoint);
		}
		
		
		listOfPolygons.push_back(rpolygon);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		
	}

	/*if (selectionstate) {
		Points newpoint;
		for (int i = 0; i < listOfPolygons[selected].size(); i++) {
			//angle += angleIn;
			
			listOfPolygons[selected][i].px = listOfPolygons[selected][i].px * cos(angle) - listOfPolygons[selected][i].py * sin(angle);
			listOfPolygons[selected][i].py = listOfPolygons[selected][i].px * sin(angle) + listOfPolygons[selected][i].py * cos(angle);
		//	polygon.push_back(newpoint);
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		drawListOfPolygons();
	}*/
}


void movePolygon() {

	if (selectionstate) {
		for (int i = 0; i < listOfPolygons[selected].size(); i++) {
			listOfPolygons[selected][i].px = listOfPolygons[selected][i].px - distx;
			listOfPolygons[selected][i].py = listOfPolygons[selected][i].py - disty;
		}

		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		drawListOfPolygons();
	}
}


void selectPolygon() {
	if (mouseleftpressed) {

		Points mouseposition, p;//points for creating a semi straight from mousepoint
		mouseposition.px = mousex;
		mouseposition.py = mousey;
		p.px = mousex + 100;
		p.py = mousey;
		int counter = 0;

		if (!selectionstate) {//checking if a polygon is already selected

			for (int i = 0; i < listOfPolygons.size(); i++) {
				for (int j = 0; j < listOfPolygons[i].size(); j++) {
					if ((j < listOfPolygons[i].size() - 1) && validPointCalculation(mouseposition, p, listOfPolygons[i][j], listOfPolygons[i][j + 1])) {
						counter++;
					}
					else if ((j == listOfPolygons[i].size() - 1) && validPointCalculation(mouseposition, p, listOfPolygons[i][j], listOfPolygons[i][0])) {
						counter++;
					}
				}

				if (counter % 2 != 0) {
					selectionstate = true;
					selected = i;
					drawListOfPolygons();
					break;
				}
				else {
					counter = 0;
				}

			}

		}
		else {
			for (int j = 0; j < listOfPolygons[selected].size(); j++) {
				if ((j < listOfPolygons[selected].size() - 1) && validPointCalculation(mouseposition, p, listOfPolygons[selected][j], listOfPolygons[selected][j + 1])) {
					counter++;
				}
				else if ((j == listOfPolygons[selected].size() - 1) && validPointCalculation(mouseposition, p, listOfPolygons[selected][j], listOfPolygons[selected][0])) {
					counter++;
				}
			}

			if (counter % 2 == 0) {
				selectionstate = false;
				drawListOfPolygons();
			}
		}

		mouseleftpressed = false;
	}

}


void Render()
{
	if (clearScreen)
	{
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		polygon.clear();
		listOfPolygons.clear();
		listOfColors.clear();

		clearScreen = false;
	}

	if (polygonDraw) {
		createPolygon();
	}

	if (pMove) {
		movePolygon();
	}

	if (pSelect) {
		selectPolygon();
	}

	if (pRotate) {
		Rotate();
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glFlush();
	//	glutSwapBuffers();             // All drawing commands applied to the 
									   // hidden buffer, so now, bring forward
									 // the hidden buffer and hide the visible one           

}

//-----------------------------------------------------------


void Resize(int w, int h)
{
	// define the visible area of the window ( in pixels )
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);

	// Save the window size
	winw = w;
	winh = h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(0.0, 1.0, 0.0, 1.0);

	//         L	   R 	  B 	T	 N	   F
	//glOrtho(-100.0, 100.0, -100.0, 100.0, 100.0, -100.0);
	glutReshapeWindow(const1, const2);
}

void Idle()
{

}

//-----------------------------------------------------------

void Setup()  // DON'T TOUCH IT 
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}


/////////////// Main Program ///////////////////////////

int main(int argc, char* argv[])
{

	glutInit(&argc, argv);


	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);


	glutInitWindowSize(const1, const2);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Assignment 01");


	Setup();
	createMenu();

	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);

	glutDisplayFunc(Render);
	glutReshapeFunc(Resize);

	//glutIdleFunc(Idle);



	glutMainLoop();
	return 0;


}
