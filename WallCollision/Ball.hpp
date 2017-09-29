#ifndef _BALL_CLASS
#define _BALL_CLASS

#define PI 3.141592f//円周率
#define G 9.800665f//重力加速度

#include <iostream>
#include <GLUT/glut.h>
#include <cmath>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////
//Ballクラス
///////////////////////////////////////////////////////////////////////////////////////
class Ball{
//private:
public://テスト状態なのでメンバ変数のアクセス権限はpublic
	double size = 2.0;//ボールの半径
	double m_x, m_y, m_z;//ローカル座標
	const double m_m = 0.17;//ビリヤード球の質量
	double m_a;//加速度
	double m_v;//速度
	const double max_v = 10.0;//最大速度
	double m_n;//与えられたニュートン
	bool move;//運動のスイッチ
	bool motion;//true=moving、false=stopping
	int m_t;//運動が始まってからの経過フレームカウンタ
	double m_angle;//進行ベクトルデグリー角
	const double max_f = 0.03;//最大静止摩擦係数
	const double mov_f = 0.02;//動摩擦係数
	double s_f;//静止摩擦力
	double d_f;//動摩擦力
	int m_quadrant;//象限位置
public:
	Ball(double x, double y, double z);//Constructor
	void Reflection(int quad_patter, int ref_pattern, double wall_x, double wall_z);//反射
	void Moving();//移動し、現在座標を求める。引数はニュートン
	bool checkFriction(double n);//最大静止摩擦力の計算による運動開始の許可
	void Drawing();//描画
	void IncAngle();//角度の更新
	void DrawVectorLine();//進行方向ベクトルに線を描画する
};

#endif