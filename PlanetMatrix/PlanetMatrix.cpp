// PlanetMatrix.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <math.h>
#include <glut.h>

static int titik = 0;
static float tick = 0;
static float moonTick = 0;

typedef struct {
	float x;
	float y;
} point2D_t;

typedef struct {
	float v[3];
} vector2D_t;

typedef struct {
	float m[3][3];
} matrix2D_t;

typedef struct {
	float r;
	float g;
	float b;
} color_t;

vector2D_t point2vector(point2D_t pnt) {
	vector2D_t vec;
	vec.v[0] = pnt.x;
	vec.v[1] = pnt.y;
	vec.v[2] = 1.;
	return vec;
}

point2D_t vector2point(vector2D_t vec) {
	point2D_t pnt;
	pnt.x = vec.v[0];
	pnt.y = vec.v[1];
	return pnt;
}

matrix2D_t createIdentity() {
	matrix2D_t mat;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mat.m[i][j] = 0.;
		}
		mat.m[i][i] = 1.;
	}
	return mat;
}

matrix2D_t translationMTX(float dx, float dy) {
	matrix2D_t trans = createIdentity();
	trans.m[0][2] = dx;
	trans.m[1][2] = dy;
	return trans;
}

matrix2D_t scalingMTX(float mx, float my) {
	matrix2D_t scale = createIdentity();
	scale.m[0][0] = mx;
	scale.m[1][1] = my;
	return scale;
}

matrix2D_t rotationMTX(float theta) {
	matrix2D_t rotate = createIdentity();
	float cs = cos(theta);
	float sn = sin(theta);
	rotate.m[0][0] = cs;
	rotate.m[0][1] = -sn;
	rotate.m[1][0] = sn;
	rotate.m[1][1] = cs;
	return rotate;
}

matrix2D_t operator *(matrix2D_t a, matrix2D_t b) {
	matrix2D_t c;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			c.m[i][j] = 0;
			for (int k = 0; k < 3; k++)
			{
				c.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return c;
}

vector2D_t operator *(matrix2D_t a, vector2D_t b) {
	vector2D_t c;
	for (int i = 0; i < 3; i++)
	{
		c.v[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			c.v[i] += a.m[i][j] * b.v[j];
		}
	}
	return c;
}

void setColor(color_t col) {
	glColor3f(col.r, col.g, col.b);
}

void drawPolygon(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<n; i++) {
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolygon(point2D_t pnt[], int n, color_t c) {
	int i;
	setColor(c);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i<n; i++) {
		glVertex2i(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPolyline(point2D_t pnt[], int n) {
	int i;
	glBegin(GL_LINE_STRIP);
	for (i = 0; i<n; i++) {
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void fillPolygon(point2D_t pnt[], int n, color_t color) {
	int i;
	setColor(color);
	glBegin(GL_POLYGON);
	for (i = 0; i < n; i++)
	{
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawTriangleFan(point2D_t pnt[], int n, color_t color) {
	int i;
	setColor(color);
	glBegin(GL_TRIANGLE_FAN);
	for (i = 0; i < n; i++)
	{
		glVertex2f(pnt[i].x, pnt[i].y);
	}
	glEnd();
}

void drawPlanet(point2D_t *p, point2D_t dot, float r, int n, color_t c) {
	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = dot.x + r*cos(i*a);
		p[i].y = dot.y + r*sin(i*a);
	}

	fillPolygon(p, n, c);
}

void drawPlanetNew(point2D_t *p, point2D_t trans, float tck,  float r, int n, color_t c) {
	vector2D_t vec = point2vector({ 0,0 });
	matrix2D_t matRotasi = rotationMTX(tck);
	matrix2D_t matTranslasi = translationMTX(trans.x, trans.y);
	//matrix2D_t matFinal = operator*(matRotasi, matTranslasi);
	vec = operator*(matTranslasi, vec);
	vec = operator*(matRotasi, vec);
	point2D_t pnt = vector2point(vec);

	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = pnt.x + r*cos(i*a);
		p[i].y = pnt.y + r*sin(i*a);
	}

	fillPolygon(p, n, c);
}

void drawMoon(point2D_t *p, point2D_t dot, point2D_t trans, float tck, float r, int n, color_t c) {
	vector2D_t vec = point2vector({ 5,0 });
	
	matrix2D_t matRotasi = rotationMTX(tck*5);
	matrix2D_t matTranslasi = translationMTX(trans.x, trans.y);
	//matrix2D_t matFinal = operator*(matRotasi, matTranslasi);
	/*vec = operator*(matTranslasi, vec);
	vec = operator*(matRotasi, vec);*/
	point2D_t pnt = vector2point(vec);

	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = dot.x + pnt.x + r*cos(i*a);
		p[i].y = dot.y + pnt.y + r*sin(i*a);
	}

	fillPolygon(p, n, c);
}

void drawStar(point2D_t *p, float tck, int n, color_t c) {
	vector2D_t vec[12];
	for (int i = 0; i < 12; i++)
	{
		vec[i] = point2vector(p[i]);
	}

	matrix2D_t matRotasi = rotationMTX(tck*2);
	for (int i = 0; i < 12; i++)
	{
		vec[i] = operator*(matRotasi, vec[i]);
	}
	for (int i = 0; i < 12; i++)
	{
		p[i] = vector2point(vec[i]);
	}

	drawTriangleFan(p, 12, c);
}

void drawOrbitNew(point2D_t *p, point2D_t trans, float tck, float r, int n, color_t c) {
	vector2D_t vec = point2vector({ 0,0 });
	matrix2D_t matRotasi = rotationMTX(tck);
	matrix2D_t matTranslasi = translationMTX(trans.x, trans.y);
	//matrix2D_t matFinal = operator*(matRotasi, matTranslasi);
	vec = operator*(matTranslasi, vec);
	vec = operator*(matRotasi, vec);
	point2D_t pnt = vector2point(vec);

	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = pnt.x + r*cos(i*a);
		p[i].y = pnt.y + r*sin(i*a);
	}

	drawPolygon(p, n, c);
}

void drawOrbit(point2D_t *p, point2D_t dot, float r, int n, color_t c) {
	float a = 6.28 / n;
	for (int i = 0; i < n; i++)
	{
		p[i].x = dot.x + r*cos(i*a);
		p[i].y = dot.y + r*sin(i*a);
	}

	drawPolygon(p, n, c);
}


void userdraw() {
	color_t orbitColor = { 0.0f, 0.0f, 0.0f };
	color_t sunColor = { 1.0f, 0.7f, 0.0f };
	color_t earthColor = { 0.0f, 0.3f, 0.0f };
	color_t moonColor = { 0.8f,0.8f,0.8f };

	//// Sun
	/*point2D_t p1[360];
	drawPlanetNew(p1, { 0, 0 }, 0, 50, 360, sunColor);*/
	/*drawSunTriangle({60, 0}, tick);
	drawSunTriangle({ 0, 60 }, tick);*/

	point2D_t p1[12] = {
		{0, 0},
		{0, 65},
		{20, 25},
		{65, 25},
		{30, -10}, 
		{50, -60},
		{0, -32}, 
		{-50, -60},
		{-30, -10},
		{-65, 25},
		{-20, 25},
		{0, 65}
	};

	//drawTriangleFan(p1, 12, sunColor);
	drawStar(p1, tick, 12, sunColor);

	/*glColor3f(0, 0, 0);
	glPointSize(2);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	glEnd();*/

	//// Orbit
	point2D_t p2[360];
	drawOrbit(p2, { 0, 0 }, 150, 360, orbitColor);

	//// Earth
	point2D_t p3[360];
	drawPlanetNew(p3, { 150, 0 }, tick, 20, 360, earthColor);

	//// Orbit Moon
	point2D_t p4[360];
	drawOrbitNew(p4, { 150, 0 }, tick, 50, 360, orbitColor);

	//// Moon
	point2D_t p5[360];
	drawMoon(p5, p4[titik], { 50, 0 }, tick, 10, 360, moonColor);

	tick += 0.01;
	moonTick += 0.01;

	titik++;

	if (titik >= 360)
	{
		titik = 0;
	}
}

void display(void) { //clear screen 
	glClear(GL_COLOR_BUFFER_BIT);
	userdraw();
	glutSwapBuffers();
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void initialize() {
	glClearColor(1, 1, 1, 0);
	glLoadIdentity();
	// Ortho biasa
	gluOrtho2D(-300., 300., -300., 300.);
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	//Inisialisasi Toolkit 
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("My Amazing Sky");
	initialize();
	//glutIdleFunc(display);
	glutDisplayFunc(display);
	glutTimerFunc(1, timer, 0);
	glutMainLoop();
	return 0;
}
