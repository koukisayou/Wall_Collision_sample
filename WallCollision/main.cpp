/*
	球を運動させるプログラム
	任意ベクトルに等加速度運動をさせる
*/

#include <stdlib.h>
#include <GLUT/glut.h>
#include <iostream>
#include <cmath>
#include "Ball.hpp"
#include "Wall.hpp"
#include "Collision.hpp"
using namespace std;

#define KEY_ESC 27
#define PI 3.141592f//円周率
#define G 9.800665f//重力加速度

//Color
static const GLfloat red[] = {1.0f, 0.0f, 0.0f};
static const GLfloat green[] = {0.0f, 1.0f, 0.0f};


//Global ball class.
Ball mainBall(0.0, 0.0, 0.0);//手球
Ball subBall(20.0,0.0,20.0);//的球
Wall wall;
Collision coll;

//デバッグ用メッセージ出力
void TestMessage(){
	cout << "speed: " << mainBall.m_v << endl;
	cout << "accele: " << mainBall.m_a << endl;
	cout << "force: " << mainBall.m_n << endl;
	cout << "x: " << mainBall.m_x << endl;
	cout << "z: " << mainBall.m_z << endl;
	cout << "angle: " << mainBall.m_angle << endl;
	cout << endl;
}

//テスト用グリッド描画
void TestGrid(double r, double g, double b, double min_x, double max_x, double min_z, double max_z){
	glColor3f(r,g,b);
	glBegin(GL_LINES);//地面のグリッド
	for(int i = min_x; i < max_x; i += 2){
		glVertex3f((float)i,0,min_z);
		glVertex3f((float)i , 0, max_z);
	}
	for(int i = min_x; i < max_x; i += 2){
		glVertex3f(min_x,0,(float)i);
		glVertex3f(max_x , 0, (float)i);
	}
	glEnd();
}

//反射x,zのチェック
void checkQuadrant(Ball& ball){
	//x,zの真理チェック
	switch(mainBall.m_quadrant){
		case 1://第１象限
			coll.b_reflection = coll.Reflection(wall.x_plus, wall.z_plus);
			ball.Reflection(coll.b_Quadrant, coll.b_reflection,wall.x_plus, wall.z_plus);
			break;
		case 2://第２象限
			coll.b_reflection = coll.Reflection(wall.x_minus, wall.z_plus);
			ball.Reflection(coll.b_Quadrant, coll.b_reflection,wall.x_minus, wall.z_plus);
			break;
		case 3://第３象限
			coll.b_reflection = coll.Reflection(wall.x_minus, wall.z_minus);
			ball.Reflection(coll.b_Quadrant, coll.b_reflection,wall.x_minus, wall.z_minus);
			break;
		case 4://第４象限
			coll.b_reflection = coll.Reflection(wall.x_plus, wall.z_minus);
			ball.Reflection(coll.b_Quadrant, coll.b_reflection,wall.x_plus, wall.z_minus);
			break;
		default:
			break;
	}
}

//Drawing display.
void display(void){
	mainBall.Moving();//玉の運動計算
	mainBall.IncAngle();//角度更新
	mainBall.m_quadrant = coll.checkQuadrant(mainBall.m_x, mainBall.m_z);//玉の象限位置確認
	checkQuadrant(mainBall);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(mainBall.m_x, 3.0, 20.0 + mainBall.m_z, mainBall.m_x, 0.0, 1.0 + mainBall.m_z, 0.0, 1.0, 0.0);//Camera
	glEnable(GL_DEPTH_TEST);//Start Z buffer.
	glPushMatrix();
	mainBall.Drawing();
	mainBall.DrawVectorLine();//進行方向に線を描画

	TestGrid(1.0,0.5,0.5,-100,0,-100,0);
	TestGrid(0.5,1.0,0.5,-100,0,0,100);
	TestGrid(0.5,0.5,1.0,0,100,-100,0);
	TestGrid(0.5,0.5,0.5,0,100,0,100);
	/*glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);//地面のグリッド
	for(int i = -35; i < 36; i += 2){
		glVertex3f((float)i,0,-35.0);
		glVertex3f((float)i , 0,35.0);
		glVertex3f(-50.0 ,0,(float)i);
		glVertex3f(50.0 ,0,(float)i);
	}
	glEnd();*/
	wall.Drawing();
	glColor3fv(green);//描画色の指定
	glutSolidCube(1.0);//箱の描画
	
	glPopMatrix();//座標系の復元
	if(mainBall.motion)TestMessage();
	glDisable(GL_DEPTH_TEST);//End Z buffer.
	glutSwapBuffers();
}

//タイマー用コールバック関数
void timer(int value){
	//番号1のタイマーであるか
	if(value == 1){
		glutTimerFunc(30, timer, 1);//タイマーの再起動
		glutPostRedisplay();//画面の更新
	}
	//番号2のタイマーであるか
	if(value == 2){
		TestMessage();//デバッグ用テストメッセージの出力
		glutTimerFunc(500, timer, 2);//タイマーの再起動
	}
}

//keyboard event
void myKbd(unsigned char key, int x, int y){
	switch(key){
		//End program.
		case 'q':
		case 'Q':
		case '\033': //'\033'はESCのASCIIコード
			exit(0);//プログラムの強制終了
			break;
		//球の運動をスタートさせる
		case 'w':
		case '\040': //'\040'はspaceのASCIIコード(8進数表現)
			mainBall.move = true;
			break;
		default:
			break;
	}
}

//window resize.
void resize(int w, int h){
	glViewport(0,0,w,h);

	//透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)w / (double)h, 1.0, 1000.0);

	//モデルビュー変換行列の設定
	glMatrixMode(GL_MODELVIEW);
}

//initialize
void myInit(char *progname){
	int width = 640, height = 480;
	float aspect = (float)width / (float)height;//アス比

	glutInitWindowPosition(0,0);//
	glutInitWindowSize(width, height);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow(progname);
	glClearColor(0.0,0.0,0.0,1.0);
	glutKeyboardFunc(myKbd);

	glMatrixMode(GL_PROJECTION);
	//投影変換行列スタックを操作対象とする
	glLoadIdentity();//行列スタックをクリア
	gluPerspective(60.0, aspect, 1.0,1000.0);
	glMatrixMode(GL_MODELVIEW);//幾何変換行列スタックを操作対象とする

	gluLookAt(0.0, 3.0, 20.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0);//Camera
}

//main
int main(int argc, char** argv){
	glutInit(&argc,argv);//GLUT initialize.
	myInit(argv[0]);//OpenGL initialize.
	glutDisplayFunc(display);//Display drawing.
	glutReshapeFunc(resize);//Window resize.
	glutTimerFunc(30, timer, 1);//Timer start.
	//glutTimerFunc(500, timer, 2);//Test message timer start.
	glutMainLoop();//Loop
	return 0;
}