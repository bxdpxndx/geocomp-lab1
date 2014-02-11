#include "input.h"

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

input::input(){
	// screen size 
	height = 600;
	width = 800;

	// This is hardcoded..... (we should control this by rotation around (0,0,0)
	direction = vector3f(0, 0.5, 1);

	// init params scale factor (Borrar si al final no es fan servir tots ! )
	sf_trlz[0] = 1.0f;      // Translate and dolly
	sf_trlz[1] = 5.0f;      // Rotate (degrees)
	sf_trlz[2] = 1.0f;      // Zoom

	reset();
}

void input::reset(){
	// init view
	dolly = 10.0;
	updateView();

	// init proj
	zoom = 40.0;
	updateProjection();

	// init model
	model = glm::mat4(1.0);
	updateModel();
}

void input::onReshape(int w, int h){
	width = w;
	height = h;
	glViewport(0, 0, width, height);
	updateProjection();
}

void input::MyKeyboardFunc(unsigned char Key, int x, int y){
	switch (Key){
		case '+': SetZoom(-1);				break; // Zoom IN 
		case '-': SetZoom(1);				break; // Zoom OUT
		case ',': SetDolly(-1);				break; // Dolly IN 
		case '.': SetDolly(1);				break; // Dolly OUT
		case 'q': MoveObj( 1, 0, 0);		break; // Move X + OBJ
		case 'a': MoveObj(-1, 0, 0);		break; // Move X - OBJ          
		case 'w': MoveObj(0,  1, 0);		break; // Move Y + OBJ
		case 's': MoveObj(0, -1, 0);		break; // Move Y - OBJ
		case 'e': MoveObj(0, 0,  1);		break; // Move Z + OBJ
		case 'd': MoveObj(0, 0, -1);		break; // Move Z + OBJ  
		case 'r': RotObj(1, 0, 0,  1);		break; // Rotate X + OBJ 
		case 'f': RotObj(1, 0, 0, -1);		break; // Rotate X - OBJ        
		case 't': RotObj(0, 1, 0,  1);		break; // Rotate Y + OBJ
		case 'g': RotObj(0, 1, 0, -1);		break; // Rotate Y - OBJ 
		case 'y': RotObj(0, 0, 1,  1);		break; // Rotate Z + OBJ
		case 'h': RotObj(0, 0, 1, -1);		break; // Rotate Z + OBJ  
		case 'Z': case 'z': reset();        break; // Reset to defaults
		case '1': case '2': case  '3': case  '4': case  '5': case  '6': case  '7': case  '8': 
			o.getChildren(Key);
			break;
		case 'p':
			o.getParent();
			break;
		case 'o':
			o.getRood();
			break;
	};
}

void input::MyKeyboardFunc(int Key, int x, int y){
	// futer worck with arrows keys
}

void input::updateProjection(){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, width / (float)height, 0.1f, 1000.0f);
	Projection = glm::perspective(zoom, width / (float)height, 0.1f, 1000.0f);
}

void input::updateView(){
	glMatrixMode(GL_PROJECTION);
	gluLookAt(direction[0] * dolly, direction[1] * dolly, direction[2] * dolly,
		0.f, 0.f, 0.f, 
		0, 1, 0);
	View = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0, 1, 0));
}

void input::updateModel(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	matrix4x4f view;
	view.identity();

	view.m[0] = model[0][0];
	view.m[1] = model[0][1];
	view.m[2] = model[0][2];
	view.m[3] = model[0][3];

	view.m[4] = model[1][0];
	view.m[5] = model[1][1];
	view.m[6] = model[1][2];
	view.m[7] = model[1][3];

	view.m[8] = model[2][0];
	view.m[9] = model[2][1];
	view.m[10] = model[2][2];
	view.m[11] = model[2][3];

	view.m[12] = model[3][0];
	view.m[13] = model[3][1];
	view.m[14] = model[3][2];
	view.m[15] = model[3][3];

	glMultMatrixf(view.m);
}

void input::SetZoom(float in_zoom){
	zoom = zoom + in_zoom * sf_trlz[2];
	zoom = MIN(zoom, 150);
	zoom = MAX(zoom, 0.01);
	updateProjection();
}

void input::SetDolly(float in_dolly){
	dolly = dolly + in_dolly * sf_trlz[0];
	dolly = MAX(dolly, 0.01);
	updateView();
}

void input::MoveObj(float in_x, float in_y, float in_z){
	glm::vec3 tr = glm::vec3(sf_trlz[0] * in_x, sf_trlz[0] * in_y, sf_trlz[0] * in_z);
	model = glm::translate(model, tr);
}


void input::RotObj(float in_x, float in_y, float in_z, float dir){
	model = glm::rotate(model, sf_trlz[1] * dir, glm::vec3(in_x, in_y, in_z));
}

input::~input()
{
}
