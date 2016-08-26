#pragma once

#include<glut.h>
#include<stdlib.h>

class Block {
public:
	double x;
	double z;
	double r;
	double g;
	double b;
	Block() {};
	void make()//���׸���
	{
		glPushMatrix();
		glTranslatef(x, 0, z);
		//glutWireCube(1);
		glutSolidCube(1);
		glPopMatrix();
		if (z < -1)// ������ ���
			reset();
	}
	void collision(double pX, double pZ)//�浹
	{
		
		bool row = (x-1<pX&&x>pX-1) ? true : false;
		bool col = z <= pZ + 1 ? true : false;
		if (row&&col)
			reset();

	}
	void reset()//����ġ �ʱ�ȭ
	{
		x = rand() % 20 - 10;
		z = rand() % 15 + 10;
	}
};

