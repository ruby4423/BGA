#include<glut.h>
#include<stdlib.h>
#include<math.h>

GLfloat vertiecs[8][3] = {
	{0, 0, 0}, {0.5, 0, 0}, 
	{0.5, 0.5, 0}, {0, 0.5, 0}, 
	{ 0, 0, 0.5 },{ 0.5, 0, 0.5 },
	{ 0.5, 0.5, 0.5 },{ 0, 0.5, 0.5 }
};
GLfloat angleX = 0;
GLfloat angleY = 0;
GLfloat z = 1;

void display()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	glLoadIdentity();
	
	
	gluLookAt(0, 0, z, 0, 0, 0, 0, 1, 0);
	glRotatef(angleX, 1, 0, 0);
	glRotatef(angleY, 0, 1, 0);

	//x,y,z축 그리기 (직선 그리기)
	glPushMatrix();
	glBegin(GL_LINES);
	//glLineWidth(3.0f);  //확인 못해봄
	glColor3f(1.0, 0.0, 0.0);  //RGB 색 지정해 주고,
	glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.5, 0.0, 0.0); /* X axis      */
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.5, 0.0); /* Y axis      */
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.0, 0.5); /* Z axis    */
	glEnd();
	glPopMatrix();

	
	glPushMatrix();
	glTranslatef(0, 0, -1);
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1, 0, 0);
	glVertex3fv(vertiecs[0]);
	glVertex3fv(vertiecs[1]);
	glVertex3fv(vertiecs[3]);
	glColor3f(0, 1, 0);
	glVertex3fv(vertiecs[2]);
	glVertex3fv(vertiecs[6]);
	glVertex3fv(vertiecs[1]);
	glColor3f(0, 0, 1);
	glVertex3fv(vertiecs[5]);
	glVertex3fv(vertiecs[0]);
	glVertex3fv(vertiecs[4]);
	glColor3f(1, 1, 0);
	glVertex3fv(vertiecs[3]);
	glVertex3fv(vertiecs[7]);
	glVertex3fv(vertiecs[6]);
	glColor3f(1, 0, 1);
	glVertex3fv(vertiecs[4]);
	glVertex3fv(vertiecs[5]);
	glEnd();
	glPopMatrix();


	glPushMatrix();
	//glTranslatef(1, 1, -1);
	glColor3f(1, 1, 1);
	//glBegin(GL_LINE_STRIP);
	//float pi = acos(-1);
	//int max = 12;
	////구면 좌표계(i = theta, j = phi)
	//for (float i = 0; i <= max; i++)
	//{
	//	for (float j = 0; j <= max; j++)
	//	{
	//		glVertex3f(sinf(i*pi*2/max)*cosf(j*pi*2/max), sinf(i*pi * 2 / max)*sinf(j*pi * 2 / max), cosf(i*pi*2/max));
	//	}
	//}
	//glEnd();
	glBegin(GL_TRIANGLES);
	float pi = acos(-1);
	int max = 12;
	//구면 좌표계(i = theta, j = phi)
	for (float i = 0; i <= max; i++)
	{
		for (float j = 0; j <= max; j++)
		{
			glVertex3f(cosf(j*pi*2/max)*sinf(i*pi * 2 / max), sinf(j*pi * 2 / max)*sinf(i*pi * 2 / max), cosf(i*pi*2/max));
		}
	}
	glEnd();



	glPopMatrix();

	glFlush();
}


void reShape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, 1, 20);
	//glFrustum(-1, 1, -1, 1, 1, 20);//최대시야거리 30
	glMatrixMode(GL_MODELVIEW);

}

void keyboard(unsigned char key, int x, int y)
{
	if (key == '4')
		angleY++;
	else if (key == '6')
		angleY--;
	else if (key == '2')
		angleX++;
	else if (key == '8')
		angleX--;
	else if (key == '+')
		z -= 0.5;
	else if (key == '-')
		z += 0.5;
}
void idle()
{
	display();
}

void main()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	//glShadeModel(GL_FLAT);
	glutInitWindowPosition(300, 300);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OGL TEST");

	glutDisplayFunc(display);
	glutReshapeFunc(reShape);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
}