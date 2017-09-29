#include "Collision.hpp"

/****************************
*玉がどの象限上に存在するかチェック
*引数:玉の座標(x,z平面)
*戻り値: 象限(1~4)
		0 = 原点
		-1 = NULL
*TODO:NULLの場合にエラーをスローして方がいいかも
*****************************/
int Collision::checkQuadrant(double x, double z){
	b_x = x;
	b_z = z;
	if(x > 0.0 && z > 0.0){
		b_Quadrant = 1;
		return 1;//第１象限
	}else if(x < 0.0 && z > 0.0){
		b_Quadrant = 2;
		return 2;//第２象限
	}else if(x < 0.0 && z < 0.0){
		b_Quadrant = 3;
		return 3;//第３象限
	}else if(x > 0.0 && z < 0.0){
		b_Quadrant = 4;
		return 4;//第４象限
	}else if(x > 0.0 && z < 0.0){
		b_Quadrant = 0;
		return 0;//原点
	}else return -1;//NULL
}

/****************************
*壁との衝突判定
*checkQuadrantで解析された象限と、引数の座標に合わせて、
*x,zの衝突判定を行う
*引数:壁の座標(変域)、象限に合わせて適切な値を与えること
*戻り値: 1 = 両成分反転
*		2 = x反転
*		3 = z反転
*		4 = 当たってない
*TODO:戻り値をビットフラグにしたらマジイケメン
*****************************/
int Collision::Reflection(double x, double z){
	switch(b_Quadrant){
		case 1://第1象限
			if(b_x > x && b_z > z)return 1;
			else if(b_x > x && b_z <= z)return 2;
			else if(b_x <= x && b_z > z)return 3;
			else if(b_x <= x && b_z <= z)return 4;
			break;
		case 2://第2象限
			if(b_x < x && b_z > z)return 1;
			else if(b_x < x && b_z <= z)return 2;
			else if(b_x >= x && b_z > z)return 3;
			else if(b_x >= x && b_z <= z)return 4;
			break;
		case 3://第3象限
			if(b_x < x && b_z < z)return 1;
			else if(b_x < x && b_z >= z)return 2;
			else if(b_x >= x && b_z < z)return 3;
			else if(b_x >= x && b_z >= z)return 4;
			break;
		case 4://第4象限
			if(b_x > x && b_z < z)return 1;
			else if(b_x > x && b_z >= z)return 2;
			else if(b_x <= x && b_z < z)return 3;
			else if(b_x <= x && b_z >= z)return 4;
			break;
	}
	return 4;
}

/*****************************
*
*****************************/
bool Collision::BallCollision(double x1, double z1, double x2, double z2){

}

/*****************************
*円と円の衝突式は
+(x1 - x2) ^ 2 + (y1 - y2) ^ 2 <= (r1 + r2) ^2
*****************************/
bool Collision::BallCollision(Ball& ball1, Ball& ball2){
	double result = (ball1.m_x - ball2.m_x) * (ball1.m_x - ball2.m_x) + (ball1.m_z - ball2.m_z) * (ball1.m_x - ball2.m_x);
	if(result <= (ball1.size + ball2.size) * (ball1.size + ball2.size))return true;//衝突している
	return false;//非衝突である
}