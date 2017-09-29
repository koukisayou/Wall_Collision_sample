#ifndef _WALL_CLASS
#define _WALL_CLASS

#include <GLUT/glut.h>

///////////////////////////////////////////////////////////////////////////////////////
//Wallクラス
///////////////////////////////////////////////////////////////////////////////////////
class Wall{
public:
	const double x_plus = 100.0;
	const double x_minus = -100.0;
	const double z_plus = 100.0;
	const double z_minus = -100.0;
	int m_pattern;
public:
	Wall();
	int WallCollision(double x, double z);
	void Drawing();//テスト用壁のラインを描画
};


#endif