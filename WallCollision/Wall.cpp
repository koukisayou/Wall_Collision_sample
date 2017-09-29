#include "Wall.hpp"

Wall::Wall(){
	m_pattern = 0;
}

int Wall::WallCollision(double x, double z){
	if(z_minus > z){
		return 1;//x軸上0~180
	}else if(x_plus > x){
		return 2;//y軸左90~270
	}else if(z_plus < z){
		return 3;//x軸下180~360
	}else if(x_minus < x){
		return 4;//y軸右270~90
	}else return 0;
}

//テスト用壁のラインを描画
void Wall::Drawing(){
	glColor3f(0.0,0.0,1.0);
	glBegin(GL_LINES);
	glVertex3f(x_plus,1.0,z_plus);
	glVertex3f(x_plus,1.0,z_minus);
	glVertex3f(x_minus,1.0,z_minus);
	glVertex3f(x_minus,1.0,z_plus);
	glEnd();
}