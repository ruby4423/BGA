#include<glut.h>
#include<stdlib.h>
#include<stdio.h>
#include"block.h"
#define MAX 10

double pX = 0;
double pZ = 0;
double speed = 0.001;

Block boxes[MAX];

void initBlock()
{
	for (int i = 0; i < MAX; i++)
	{
		boxes[i] = Block();
		boxes[i].reset();
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glLoadIdentity();

	//glTranslatef(0, 0, 0);
	gluLookAt(0 + pX, 3, -5 + pZ, 0 + pX, 3, 1 + pZ, 0, 1, 0);
	//gluLookAt(0 + pX, 10, 0 + pZ, 0 + pX, 0, 0 + pZ, 0, 0, 1);

	glPushMatrix();
	glColor3f(0, 255, 0);
	glTranslatef(0 + pX, 0, 0 + pZ);
	//glutWireCube(1);
	glutSolidCube(1);
	glPopMatrix();

	glColor3f(255, 0, 0);
	//glPushMatrix();
	//glTranslatef(0, 0, 0);
	//glutWireCube(1);
	
	glPopMatrix();

	for (int i = 0; i < MAX; i++)
	{
		boxes[i].make();
		boxes[i].collision(pX, pZ);
	}

	glFlush();
}
void reShape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2, 2, -2, 2, 2, 15);
	glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char key, int x, int y)
{
	if (key == 'a')
		pX += 0.5;
	else if (key == 'd')
		pX -= 0.5;
	else if (key == 'w')
		pZ += 0.5;
	else if (key == 's')
		pZ -= 0.5;
	else if (key == '+')
		speed *= 2;
	else if (key == '-')
		speed /= 2;
}
void idle()
{
	for (int i = 0; i < MAX; i++)
	{
		boxes[i].z -= speed;
	}
	display();
}


int main()
{
	initBlock();
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Hello World");
	glutDisplayFunc(display);
	glutReshapeFunc(reShape);
	glutKeyboardUpFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}