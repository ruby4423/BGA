#include <glut.h>
#include <math.h>

double year = 0;
double month = 0;
double day = 0;
double yearme = 0;
double yearvi = 0;
double yearma = 0;
double yearju = 0;
double yearsa = 0;
double yearur = 0;
double yearne = 0;

//수금지화목

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius) {
	int i;
	int lineAmount = 100; //# of triangles used to draw circle

	GLfloat twicePi = 2.0f * 3.14;
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= lineAmount; i++) {
		glVertex2f(
			x + (radius * cos(i *  twicePi / lineAmount)),
			y + (radius * sin(i * twicePi / lineAmount))
		);
	}
	glEnd();
}

void Display()
{//z버퍼
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //메모리 버퍼 초기화 , 멀리있는거 나중에 그림
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);

	glLoadIdentity();
	glColor3f(1, 0, 0);
	gluLookAt(0, -20, 10, 0, 0, -1, 0, 1, 0); //카메라 속성(카메라 위치, 바라보는 위치, 위쪽방향)
											  //(0,0,15,0,0,-1,0,1,0)

	glPushMatrix();
	glRotatef(year, 0, 0, 1);
	glutWireSphere(1, 10, 10); //태양
	glPopMatrix();

	glPushMatrix();//mercury
	glRotatef(yearme, 0, 0, 1); //공전
	glTranslatef(1.5, 0, 0); //
	glRotatef(yearme * 10, 0, 0, 1);
	glColor3f(1, 1, 1);
	glutWireSphere(0.1, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 1.5);

	glPushMatrix();//vinus
	glRotatef(yearvi, 0, 0, 1); //공전
	glTranslatef(2.2, 0, 0); //이동
	glRotatef(yearvi * 10, 0, 0, 1);
	glColor3f(1, 0, 1);
	glutWireSphere(0.15, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 2.2);

	glPushMatrix();//earth
	{
		glRotatef(year, 0, 0, 1); //공전
		glTranslatef(3, 0, 0); //이동
		glRotatef(year * 10, 0, 0, 1);
		glColor3f(0, 1, 1);
		glutWireSphere(0.2, 10, 10);

		glPushMatrix();//moon
		{
			glRotated(month, 0, 0, 1); //공전
			glTranslatef(0.3, 0, 0);
			glRotatef(month, 0, 0, 1);//자전
			glColor3f(1, 1, 0);
			glutWireSphere(0.05, 10, 10);
		}
		glPopMatrix();
		drawHollowCircle(0, 0, 0.3);
	}
	glPopMatrix();
	drawHollowCircle(0, 0, 3);

	glPushMatrix();//mars
	glRotatef(yearma, 0, 0, 1); //공전
	glTranslatef(4, 0, 0); //이동
	glRotatef(yearma * 5, 0, 0, 1);
	glColor3f(0, 0, 1);
	glutWireSphere(0.2, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 4);

	glPushMatrix();//jupiter
	glRotatef(yearju, 0, 0, 1); //공전
	glTranslatef(5.2, 0, 0); //이동
	glRotatef(yearju * 5, 0, 0, 1);
	glColor3f(5, 0.5, 0);
	glutWireSphere(0.4, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 5.2);

	glPushMatrix();//saturn
	glRotatef(yearsa, 0, 0, 1); //공전
	glTranslatef(6.4, 0, 0); //이동
	glRotatef(yearsa * 5, 0, 0, 1); //공전
	glColor3f(5, 0.1, 1);
	glutWireSphere(0.4, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 6.4);

	glPushMatrix();//uranus
	glRotatef(yearur, 0, 0, 1); //공전
	glTranslatef(7.6, 0, 0); //이동
	glRotatef(yearur * 5, 0, 0, 1);
	glColor3f(0, 0.5, 1);
	glutWireSphere(0.2, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 7.6);

	glPushMatrix();//neptune
	glRotatef(yearne, 0, 0, 1); //공전
	glTranslatef(8.6, 0, 0); //이동
	glRotatef(yearne * 5, 0, 0, 1);
	glColor3f(0, 0.5, 0.5);
	glutWireSphere(0.2, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 8.6);

	//glBegin(GL_POLYGON); //
	//glVertex3f(-0.5, -0.5, 0); //3개 먼저 상대좌표
	//glVertex3f(0.5, -0.5, 0); 
	//glVertex3f(0.5, 0.5, 0); //두번째 3개
	//glVertex3f(-0.5, 0.5, 0);
	//glutWireSphere(1, 50, 50);
	//glutSolidCube(0.5);
	//glutWireTeapot(50);
	//glEnd(); //

	glFlush(); //화면에 뿌림, 메모리 주소 교환
}

void idle()
{
	year += 0.01;
	month += 0.02;
	day += 0.08;
	yearme += 0.05;
	yearvi += 0.03;
	yearma += 0.008;
	yearju += 0.005;
	yearsa += 0.002;
	yearur += 0.001;
	yearne += 0.0008;
	Display();
}

void reShape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1.5, 30);//최대시야거리 30
	glMatrixMode(GL_MODELVIEW);

}

void main()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glShadeModel(GL_SMOOTH);

	//glutInitWindowSize(1000, 1000);
	glutCreateWindow("Hello World");
	glutDisplayFunc(Display);
	glutReshapeFunc(reShape);
	glutIdleFunc(idle);
	glutMainLoop(); //루프마다 콜백
}