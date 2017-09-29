#ifndef _COLLISION_CLASS
#define _COLLISION_CLASS
#include "Ball.hpp"

class Collision{
public:
	int b_Quadrant;//玉の象限の一時保管用
	int b_reflection;//壁との衝突判定の一時保管用
	double b_x, b_z;//玉の座標一時保管用
public:
	int checkQuadrant(double x, double z);//玉がどの象限上に存在するかチェック
	int Reflection(double x, double z);//壁との衝突判定
	bool BallCollision(double x1, double z1, double x2, double z2);//球と球の衝突判定
	bool BallCollision(Ball& ball1, Ball& ball2);//球と球の衝突判定
};

#endif