#include <glut.h>
#include <math.h>
#include <windows.h>

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
double speed = 0.01;
bool addKey[2] = { 0, };
bool subKey[2] = { 0, };

//������ȭ��

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
	int i;
	int lineAmount = 100; //# of triangles used to draw circle

	GLfloat twicePi = 2.0f * 3.14;
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	for (i = 0; i <= lineAmount; i++) {
		glVertex3f(
			x + (radius * cos(i *  twicePi / lineAmount)), 0,
			z + (radius * sin(i * twicePi / lineAmount))
		);
	}
	glEnd();
}

void Display()
{
	glClearColor(0, 0, 0, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);//���� �� �ݿ�
	GLfloat ambient[] = { 0.1, 0.1, 0.1, 0.1 }; //�ֺ���
	GLfloat diffuse[] = { 5 ,5 ,5 ,0 }; //�л籤
	GLfloat position[] = { 0, 0, 0, 1 };//��ġ(4��° ���� 1-��ġ, 0-���⺤��(���౤))
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	GLfloat position1[] = { 1, 1, 1, 1 };//��ġ
	//glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);
	//glEnable(GL_LIGHT1);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�޸� ���� �ʱ�ȭ , �ָ��ִ°� ���߿� �׸�
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0f);
	
	glLoadIdentity();
	gluLookAt(0, 5, -15, 0, 0, -1, 0, 1, 0); //ī�޶� �Ӽ�(ī�޶� ��ġ, �ٶ󺸴� ��ġ, ���ʹ���)
	//(0,0,15,0,0,-1,0,1,0)
	GLfloat temp[] = { 1, 0, 0, 0 };
	glPushMatrix();
	glColor3f(1, 0, 0);
	//glMaterialf(GL_FRONT,GL_EMISSION , 1);
	//glMaterialf(GL_FRONT, GL_SHININESS, 60.0f);
	glRotatef(year, 0, 1, 0);//����
	glutSolidSphere(1, 10, 10); //�¾�
	glPopMatrix();
	
	glPushMatrix();//mercury
	glRotatef(yearme, 0, 1, 0); //����
	glTranslatef(1.5, 0, 0); //
	glRotatef(yearme*10, 0, 1, 0);
	glColor3f(1, 1, 1);
	glutSolidSphere(0.1, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 1.5);
	

	glPushMatrix();//vinus
	glRotatef(yearvi, 0, 1, 0); //����
	glTranslatef(2.2, 0, 0); //�̵�
	glRotatef(yearvi*10, 0, 0, 1);
	glColor3f(1, 0, 1);
	glutSolidSphere(0.15, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 2.2);

	glPushMatrix();//earth
	{
		glRotatef(year, 0, 1, 0); //����
		glTranslatef(3, 0, 0); //�̵�
		glRotatef(year*10, 0, 1, 0);
		glColor3f(0, 1, 1);
		glutSolidSphere(0.2, 10, 10);

		glPushMatrix();//moon
		{
			glRotated(month, 0, 1, 0); //����
			glTranslatef(0.3, 0, 0);
			glRotatef(month, 0, 1, 0);//����
			glColor3f(1, 1, 0);
			glutSolidSphere(0.05, 10, 10);
		}
		glPopMatrix();
		drawHollowCircle(0, 0, 0, 0.3);
	}
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 3);

	glPushMatrix();//mars
	glRotatef(yearma, 0, 1, 0); //����
	glTranslatef(4, 0, 0); //�̵�
	glRotatef(yearma*5, 0, 1, 0);
	glColor3f(0, 0, 1);
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 4);

	glPushMatrix();//jupiter
	glRotatef(yearju, 0, 1, 0); //����
	glTranslatef(5.2, 0, 0); //�̵�
	glRotatef(yearju*5, 0, 1, 0);
	glColor3f(0.8, 0.2, 0);
	glutSolidSphere(0.4, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 5.2);

	glPushMatrix();//saturn
	glRotatef(yearsa, 0, 1, 0); //����
	glTranslatef(6.4, 0, 0); //�̵�
	glRotatef(yearsa*5, 0, 1, 0);
	glColor3f(0.5, 0.3, 0);
	glutSolidSphere(0.4, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 6.4);

	glPushMatrix();//uranus
	glRotatef(yearur, 0, 1, 0); //����
	glTranslatef(7.6, 0, 0); //�̵�
	glRotatef(yearur*5, 0, 1, 0);
	glColor3f(0, 0.5, 0.2);
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 7.6);

	glPushMatrix();//neptune
	glRotatef(yearne, 0, 1, 0); //����
	glTranslatef(8.6, 0, 0); //�̵�
	glRotatef(yearne*5, 0, 1, 0);
	glColor3f(0, 0.5, 0.5);
	glutSolidSphere(0.2, 10, 10);
	glPopMatrix();
	drawHollowCircle(0, 0, 0, 8.6);

	//glBegin(GL_POLYGON); //
	//glVertex3f(-0.5, -0.5, 0); //3�� ���� �����ǥ
	//glVertex3f(0.5, -0.5, 0); 
	//glVertex3f(0.5, 0.5, 0); //�ι�° 3��
	//glVertex3f(-0.5, 0.5, 0);
	//glutWireSphere(1, 50, 50);
	//glutSolidCube(0.5);
	//glutWireTeapot(50);
	//glEnd(); //

	glFlush(); //ȭ�鿡 �Ѹ�, �޸� �ּ� ��ȯ
}

void keyboard(unsigned char key, int x, int y)
{	//     down   release     up
	//pre    x		o			o
	//now    o		o			x
	
	if (key == '+')
		speed += 0.01f;
	else if (key == '-')
		speed -= 0.01f;
}

void idle()
{
	if (GetAsyncKeyState(VK_ADD))
		addKey[1] = true;
	else
		addKey[1] = false;
	if(addKey[0] == true && addKey[1] == false)
		speed += 0.01f;

	subKey[1] = GetAsyncKeyState(VK_SUBTRACT) ? true : false;
	if (subKey[0] == true && subKey[1] == false)
		speed -= 0.01f;
	
	addKey[0] = addKey[1];
	subKey[0] = subKey[1];

	year += speed;
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
	glFrustum(-1, 1, -1, 1, 1.5, 30);//�ִ�þ߰Ÿ� 30
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
	//glutKeyboardUpFunc(keyboard);//Ű�����Է�
	//glutSpecialFunc//Ư��Ű
	glutIdleFunc(idle);
	glutMainLoop(); //�������� �ݹ�
}