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
	void make()//블럭그리기
	{
		glPushMatrix();
		glTranslatef(x, 0, z);
		//glutWireCube(1);
		glutSolidCube(1);
		glPopMatrix();
		if (z < -1)// 기준점 통과
			reset();
	}
	void collision(double pX, double pZ)//충돌
	{
		
		bool row = (x-1<pX&&x>pX-1) ? true : false;
		bool col = z <= pZ + 1 ? true : false;
		if (row&&col)
			reset();

	}
	void reset()//블럭위치 초기화
	{
		x = rand() % 20 - 10;
		z = rand() % 15 + 10;
	}
};

