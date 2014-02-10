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

//int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate)
//{
//	//Transformation matrices
//	float m[16], A[16];
//	float in[4], out[4];
//	//Calculation for inverting a matrix, compute projection x modelview
//	//and store in A[16]
//	MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
//	//Now compute the inverse of matrix A
//	if (glhInvertMatrixf2(A, m) == 0)
//		return 0;
//	//Transformation of normalized coordinates between -1 and 1
//	in[0] = (winx - (float)viewport[0]) / (float)viewport[2] * 2.0 - 1.0;
//	in[1] = (winy - (float)viewport[1]) / (float)viewport[3] * 2.0 - 1.0;
//	in[2] = 2.0*winz - 1.0;
//	in[3] = 1.0;
//	//Objects coordinates
//	MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
//	if (out[3] == 0.0)
//		return 0;
//	out[3] = 1.0 / out[3];
//	objectCoordinate[0] = out[0] * out[3];
//	objectCoordinate[1] = out[1] * out[3];
//	objectCoordinate[2] = out[2] * out[3];
//	return 1;

vector3f input::UnProjection(int  x, int  y, int  z){
	vector3f newPos;
	glm::vec4 cords;
	glm::vec3 d;
	GLint viewport [4];

	////////////
	GLdouble ox = 0.0, oy = 0.0, oz = 0.0;
	GLint viewport[4];
	GLdouble mview[16], proj[16];
	GLfloat wx = x, wy, wz;

	glGetIntegerv(GL_VIEWPORT, viewport);
	y = viewport[3] - y;
	wy = y;
	glGetDoublev(GL_MODELVIEW_MATRIX, mview);
	glGetDoublev(GL_PROJECTION_MATRIX, proj);
	glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);
	gluUnProject(wx, wy, wz, mview, proj, viewport, &ox, &oy, &oz);
	glutPostRedisplay();
	////////////
	glm::mat4 m;
	//glGetFloatv (GL_PROJECTION_MATRIX, proj);
	//glGetFloatv(GL_MODELVIEW_MATRIX, mod);

	m = model * Projection * View;
	glm::mat4 inverseProjection = m._inverse();
	glGetIntegerv(GL_VIEWPORT, viewport);

	//cords[0] = 2 * (x - direction[0]) / direction[2] - 1;
	//cords[1] = 2 * (y - direction[1]) / direction[3] - 1;
	//cords[2] = 2 * (z)-1;
	//cords[3] = 1;

	cords[0] = 2 * (x - (float)viewport[0]) / (float)viewport[2] - 1;
	cords[1] = 2 * (y - (float)viewport[1]) / (float)viewport[3] - 1;
	cords[2] = 2 * (z)-1;
	cords[3] = 1;

	d = glm::vec3(inverseProjection * cords);
	newPos.set(d[0], d[1], d[2]);

	cout << "mouse window          " << "_" << x << "_" << y << "_" << z << endl;
	cout << "mouse world           " << "_" << d[0] << "_" << d[1] << "_" << d[2] << endl;
	cout << "view port             " << "_" << viewport[0] << "_" << viewport[1] << "_" << viewport[2] << "_" << viewport[3] << endl;
	return newPos;
}


vector3f input::GetVecPoint(int x, int y, int z, vector3f *v){
	vector3f near;
	vector3f far;
	vector3f t;
	cout << "near  ";
	near = input::UnProjection(x, y, z);
	cout << "far  ";
	far = input::UnProjection(x, y, z+1);
	t = far - near;
	cout << "Vector no normalized" << "_" << t[0] << "_" << t[1] << "_" << t[2] << endl;
	*v = far - near;
	v->normalize();
	cout << "Vector normalized" << "_" << v->x << "_" << v->y << "_" << v->z << endl;


	return near;

}


//////////////////////////////////////////////////////////
// TEST ! 

int glhProjectf(float objx, float objy, float objz, float *modelview, float *projection, int *viewport, float *windowCoordinate)
{
	//Transformation vectors
	float fTempo[8];
	//Modelview transform
	fTempo[0] = modelview[0] * objx + modelview[4] * objy + modelview[8] * objz + modelview[12];  //w is always 1
	fTempo[1] = modelview[1] * objx + modelview[5] * objy + modelview[9] * objz + modelview[13];
	fTempo[2] = modelview[2] * objx + modelview[6] * objy + modelview[10] * objz + modelview[14];
	fTempo[3] = modelview[3] * objx + modelview[7] * objy + modelview[11] * objz + modelview[15];
	//Projection transform, the final row of projection matrix is always [0 0 -1 0]
	//so we optimize for that.
	fTempo[4] = projection[0] * fTempo[0] + projection[4] * fTempo[1] + projection[8] * fTempo[2] + projection[12] * fTempo[3];
	fTempo[5] = projection[1] * fTempo[0] + projection[5] * fTempo[1] + projection[9] * fTempo[2] + projection[13] * fTempo[3];
	fTempo[6] = projection[2] * fTempo[0] + projection[6] * fTempo[1] + projection[10] * fTempo[2] + projection[14] * fTempo[3];
	fTempo[7] = -fTempo[2];
	//The result normalizes between -1 and 1
	if (fTempo[7] == 0.0)        //The w value
		return 0;
	fTempo[7] = 1.0 / fTempo[7];
	//Perspective division
	fTempo[4] *= fTempo[7];
	fTempo[5] *= fTempo[7];
	fTempo[6] *= fTempo[7];
	//Window coordinates
	//Map x, y to range 0-1
	windowCoordinate[0] = (fTempo[4] * 0.5 + 0.5)*viewport[2] + viewport[0];
	windowCoordinate[1] = (fTempo[5] * 0.5 + 0.5)*viewport[3] + viewport[1];
	//This is only correct when glDepthRange(0.0, 1.0)
	windowCoordinate[2] = (1.0 + fTempo[6])*0.5;  //Between 0 and 1
	return 1;
}



void MultiplyMatrices4by4OpenGL_FLOAT(float *result, float *matrix1, float *matrix2)
{
	result[0] = matrix1[0] * matrix2[0] +
		matrix1[4] * matrix2[1] +
		matrix1[8] * matrix2[2] +
		matrix1[12] * matrix2[3];
	result[4] = matrix1[0] * matrix2[4] +
		matrix1[4] * matrix2[5] +
		matrix1[8] * matrix2[6] +
		matrix1[12] * matrix2[7];
	result[8] = matrix1[0] * matrix2[8] +
		matrix1[4] * matrix2[9] +
		matrix1[8] * matrix2[10] +
		matrix1[12] * matrix2[11];
	result[12] = matrix1[0] * matrix2[12] +
		matrix1[4] * matrix2[13] +
		matrix1[8] * matrix2[14] +
		matrix1[12] * matrix2[15];
	result[1] = matrix1[1] * matrix2[0] +
		matrix1[5] * matrix2[1] +
		matrix1[9] * matrix2[2] +
		matrix1[13] * matrix2[3];
	result[5] = matrix1[1] * matrix2[4] +
		matrix1[5] * matrix2[5] +
		matrix1[9] * matrix2[6] +
		matrix1[13] * matrix2[7];
	result[9] = matrix1[1] * matrix2[8] +
		matrix1[5] * matrix2[9] +
		matrix1[9] * matrix2[10] +
		matrix1[13] * matrix2[11];
	result[13] = matrix1[1] * matrix2[12] +
		matrix1[5] * matrix2[13] +
		matrix1[9] * matrix2[14] +
		matrix1[13] * matrix2[15];
	result[2] = matrix1[2] * matrix2[0] +
		matrix1[6] * matrix2[1] +
		matrix1[10] * matrix2[2] +
		matrix1[14] * matrix2[3];
	result[6] = matrix1[2] * matrix2[4] +
		matrix1[6] * matrix2[5] +
		matrix1[10] * matrix2[6] +
		matrix1[14] * matrix2[7];
	result[10] = matrix1[2] * matrix2[8] +
		matrix1[6] * matrix2[9] +
		matrix1[10] * matrix2[10] +
		matrix1[14] * matrix2[11];
	result[14] = matrix1[2] * matrix2[12] +
		matrix1[6] * matrix2[13] +
		matrix1[10] * matrix2[14] +
		matrix1[14] * matrix2[15];
	result[3] = matrix1[3] * matrix2[0] +
		matrix1[7] * matrix2[1] +
		matrix1[11] * matrix2[2] +
		matrix1[15] * matrix2[3];
	result[7] = matrix1[3] * matrix2[4] +
		matrix1[7] * matrix2[5] +
		matrix1[11] * matrix2[6] +
		matrix1[15] * matrix2[7];
	result[11] = matrix1[3] * matrix2[8] +
		matrix1[7] * matrix2[9] +
		matrix1[11] * matrix2[10] +
		matrix1[15] * matrix2[11];
	result[15] = matrix1[3] * matrix2[12] +
		matrix1[7] * matrix2[13] +
		matrix1[11] * matrix2[14] +
		matrix1[15] * matrix2[15];
}

void MultiplyMatrixByVector4by4OpenGL_FLOAT(float *resultvector, const float *matrix, const float *pvector)
{
	resultvector[0] = matrix[0] * pvector[0] + matrix[4] * pvector[1] + matrix[8] * pvector[2] + matrix[12] * pvector[3];
	resultvector[1] = matrix[1] * pvector[0] + matrix[5] * pvector[1] + matrix[9] * pvector[2] + matrix[13] * pvector[3];
	resultvector[2] = matrix[2] * pvector[0] + matrix[6] * pvector[1] + matrix[10] * pvector[2] + matrix[14] * pvector[3];
	resultvector[3] = matrix[3] * pvector[0] + matrix[7] * pvector[1] + matrix[11] * pvector[2] + matrix[15] * pvector[3];
}

int glhUnProjectf(float winx, float winy, float winz, float *modelview, float *projection, int *viewport, float *objectCoordinate)
{
	//Transformation matrices
	float m[16], A[16];
	float in[4], out[4];
	//Calculation for inverting a matrix, compute projection x modelview
	//and store in A[16]
	MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
	//Now compute the inverse of matrix A
	if (glhInvertMatrixf2(A, m) == 0)
		return 0;
	//Transformation of normalized coordinates between -1 and 1
	in[0] = (winx - (float)viewport[0]) / (float)viewport[2] * 2.0 - 1.0;
	in[1] = (winy - (float)viewport[1]) / (float)viewport[3] * 2.0 - 1.0;
	in[2] = 2.0*winz - 1.0;
	in[3] = 1.0;
	//Objects coordinates
	MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
	if (out[3] == 0.0)
		return 0;
	out[3] = 1.0 / out[3];
	objectCoordinate[0] = out[0] * out[3];
	objectCoordinate[1] = out[1] * out[3];
	objectCoordinate[2] = out[2] * out[3];
	return 1;
}
#define SWAP_ROWS_DOUBLE(a, b) { double *_tmp = a; (a) = (b); (b) = _tmp; }
#define SWAP_ROWS_FLOAT(a, b) { float *_tmp = a; (a) = (b); (b) = _tmp; }
#define MAT(m, r, c) (m)[(c)* 4 + (r)]

//This code comes directly from GLU except that it is for float
int glhInvertMatrixf2(float *m, float *out)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;
	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];
	r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
		r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
		r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
		r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
		r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
		r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
		r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
		r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
		r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
		r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
		r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
		r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;
	/* choose pivot - or die */
	if (fabsf(r3[0]) > fabsf(r2[0]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[0]) > fabsf(r1[0]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (fabsf(r1[0]) > fabsf(r0[0]))
		SWAP_ROWS_FLOAT(r1, r0);
	if (0.0 == r0[0])
		return 0;
	/* eliminate first variable     */
	m1 = r1[0] / r0[0];
	m2 = r2[0] / r0[0];
	m3 = r3[0] / r0[0];
	s = r0[1];
	r1[1] -= m1 * s;
	r2[1] -= m2 * s;
	r3[1] -= m3 * s;
	s = r0[2];
	r1[2] -= m1 * s;
	r2[2] -= m2 * s;
	r3[2] -= m3 * s;
	s = r0[3];
	r1[3] -= m1 * s;
	r2[3] -= m2 * s;
	r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) {
		r1[4] -= m1 * s;
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r0[5];
	if (s != 0.0) {
		r1[5] -= m1 * s;
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r0[6];
	if (s != 0.0) {
		r1[6] -= m1 * s;
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r0[7];
	if (s != 0.0) {
		r1[7] -= m1 * s;
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[1]) > fabsf(r2[1]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (fabsf(r2[1]) > fabsf(r1[1]))
		SWAP_ROWS_FLOAT(r2, r1);
	if (0.0 == r1[1])
		return 0;
	/* eliminate second variable */
	m2 = r2[1] / r1[1];
	m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2];
	r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3];
	r3[3] -= m3 * r1[3];
	s = r1[4];
	if (0.0 != s) {
		r2[4] -= m2 * s;
		r3[4] -= m3 * s;
	}
	s = r1[5];
	if (0.0 != s) {
		r2[5] -= m2 * s;
		r3[5] -= m3 * s;
	}
	s = r1[6];
	if (0.0 != s) {
		r2[6] -= m2 * s;
		r3[6] -= m3 * s;
	}
	s = r1[7];
	if (0.0 != s) {
		r2[7] -= m2 * s;
		r3[7] -= m3 * s;
	}
	/* choose pivot - or die */
	if (fabsf(r3[2]) > fabsf(r2[2]))
		SWAP_ROWS_FLOAT(r3, r2);
	if (0.0 == r2[2])
		return 0;
	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
		r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];
	/* last check */
	if (0.0 == r3[3])
		return 0;
	s = 1.0 / r3[3];             /* now back substitute row 3 */
	r3[4] *= s;
	r3[5] *= s;
	r3[6] *= s;
	r3[7] *= s;
	m2 = r2[3];                  /* now back substitute row 2 */
	s = 1.0 / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
		r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
		r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
		r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;
	m1 = r1[2];                  /* now back substitute row 1 */
	s = 1.0 / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
		r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
		r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;
	m0 = r0[1];                  /* now back substitute row 0 */
	s = 1.0 / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
		r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);
	MAT(out, 0, 0) = r0[4];
	MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
	MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
	MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
	MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
	MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
	MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
	MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
	MAT(out, 3, 3) = r3[7];
	return 1;
}

input::~input()
{
}
