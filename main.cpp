#include <iostream>
#include <cstring>
// global includes
#include "GL/glut.h"
#include <iostream>

// local includes
#include "ase.h"
#include "vector3f.h"
#include "input.h"
#include "box3f.h"
#include "octree.h"
#include "matrix4x4f.h" 
// global libs






#ifdef _WIN32
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glut32.lib")
#endif


// global variables
int g_buttons[3];
float g_mouse_x,g_mouse_y;
int g_width, g_height;
input inputinstance;
std::vector<const triangle*> selected;


// the mesh model
CASEModel g_model;
box3f box;
Octree *o;

// local defines
#define LEFTMOUSE 0
#define MIDDLEMOUSE 1
#define RIGHTMOUSE 2

//2k8

void drawString(int x, int y, const char* string){
	int i, len;
	glRasterPos2f(x, y);
	glColor3f(1,1,1);
	len = strlen (string);
	for (i=0;i<len;i++)	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,string[i]);
	}
}

void help(){
	glDisable(GL_LIGHTING);
	// prepare 2d viewport
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1000,0,1000);

	// REESCRIURE
	drawString(60,10," MOVE --->  RIGHT:q LEFT:a UP:w DOWN:s BACK:e FORWARDS:d");
	drawString(60,40," X Axis rotation (+/-): r/f, Y Axis rotation (+/-): t/g Z Axis rotation (+/-): y/h");
}

void drawAxis() {
    // XYZ correspond to RGB. 
    // Negative coordinates are the inverse color.
    
    glBegin(GL_LINES);
        glColor3f(1,0,0);
        glVertex3f(2,0,0);
        glColor3f(0,1,1);
        glVertex3f(-2,0,0);
        glColor3f(0,1,0);
        glVertex3f(0,2,0);
        glColor3f(1,0,1);
        glVertex3f(0,-2,0);
        glColor3f(0,0,1);
        glVertex3f(0,0,2);
        glColor3f(1,1,0);
        glVertex3f(0,0,-2);
    glEnd();
}

void drawSelT(){
	int i;
	for (i = 0; i<selected.size(); i++)	{
		const triangle *t = selected[i];

		glPolygonMode(GL_BACK, GL_FILL);
		glColor3f(1.f, 0.f, 0.f);
		glBegin(GL_TRIANGLES);
		glVertex3fv(o->get_vertex(t->a)); 
		glVertex3fv(o->get_vertex(t->b));
		glVertex3fv(o->get_vertex(t->c));

		glPolygonMode(GL_BACK, GL_LINE);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(-1.f, -1.f);
		glColor3f(1.f, 0.f, 0.f);
		glBegin(GL_TRIANGLES);
		glVertex3fv(o->get_vertex(t->a)); 
		glVertex3fv(o->get_vertex(t->b));
		glVertex3fv(o->get_vertex(t->c));

	}
}
void init(void){


}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glShadeModel(GL_FLAT);
	// setup camera
	inputinstance.updateProjection();
	inputinstance.updateView();
	inputinstance.updateModel();

	//render here

	//draw  a square centered at 0,0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);
	glVertex3f(-3,0,3);
	glVertex3f(3,0,3);
	glVertex3f(3, 0, -3);
	glVertex3f(-3, 0, -3);
	glEnd();

    // draw axis centered at 0,0,0
	drawAxis();

	// draw model
	g_model.render();

	// draw octre
	o->render();

	//glDisable(GL_LIGHTING);
	help();
	

	glFlush();
	glutSwapBuffers();
}

void onReshape(int w, int h){
	inputinstance.onReshape(w, h);
}

void onKeyboard(unsigned char k, int x, int y){
	inputinstance.MyKeyboardFunc(k, x, y);
}

void onKeyboard(int k, int x, int y){
	inputinstance.MyKeyboardFunc(k, x, y);
}


void idle(){
	display();
}

void mouse(int button, int state, int x, int y)
{
	//g_mouse_x = x / (float)inputinstance.height;
	//g_mouse_y = y / (float) inputinstance.width;
	g_mouse_x = x ;
	g_mouse_y = y ;
	vector3f v;
	vector3f p;
	
    switch (button) {
    case GLUT_LEFT_BUTTON:
        {
            p = inputinstance.GetVecPoint(g_mouse_x, g_mouse_y, 0, &v);
            const triangle *t = o->get_intersecting_triangle(p, v, NULL);
			//gluUnProject(g_mouse_x, g_mouse_y, 0.f, inputinstance.model, inputinstance.Projection, inputinstance.direction, q, w, e);

			//cout << "mouse REAL world" << "_" << d[0] << "_" << d[1] << "_" << d[2] << endl;

			cout << "point" << p.x << p.y << p.z << endl;
			cout << "vector" << v.x << v.y << v.z << endl;
			if (t!=NULL){
                selected.push_back(t);
				cout << "New triangle !" << endl;
            }
        break;
        }
    case GLUT_MIDDLE_BUTTON:
        g_buttons[MIDDLEMOUSE] = (state == GLUT_DOWN);
        break;
    case GLUT_RIGHT_BUTTON:
        g_buttons[RIGHTMOUSE] = (state == GLUT_DOWN);
        break;
    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(200, 0);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Practica 1. Creacio d un octree a partir d un mesh");
	glutDisplayFunc(display);
	glutKeyboardFunc(onKeyboard);
	glutSpecialFunc(onKeyboard);
	glutReshapeFunc(onReshape);
	glutIdleFunc(idle);
	glutMouseFunc(mouse);

	// load a model
	//g_model.load("data/teapot.ase");
	g_model.load("data/knot.ase");
	//g_model.load("data\\terrain.ase");
    o = new Octree(g_model);
	
	glutSwapBuffers();
	glutMainLoop();
	glutReshapeFunc(onReshape);
	return 0;
}


#undef LEFTMOUSE
#undef MIDDLEMOUSE
#undef RIGHTMOUSE
