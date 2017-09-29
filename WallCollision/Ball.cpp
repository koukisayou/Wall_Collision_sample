#include "Ball.hpp"

Ball::Ball(double x, double y, double z):m_x(x), m_y(y), m_z(z){
	m_a = 0.0;
	//m_n = 0.017;//ひとまず0.017に設定
	move = false;
	m_t = 0;
	m_angle = 0.0;
	s_f = max_f * m_m * G;//静止摩擦力の計算
	d_f = mov_f * m_m * G;//動摩擦力の計算
}

/*************************
*x,z軸の反射をさせる
*引数のパターンによってベクトルの向き(角度)を変換する
*引数は両方ともCollisionクラスによる結果を用いる
*
*反射による角度変換の法則は以下 (θ = 現在デグリー角度)
*Z軸に対する反転
*A.右下 -> 左下 540 - θ B.左下 -> 右下 540 - θ
*C.右上 -> 左上 180 - θ D.左上 -> 右上 180 - θ
*X軸に対する反転
*E.左下 -> 左上 360 - θ F.右下 -> 右上 360 - θ
*G.右上 -> 右下 360 - θ H.左上 -> 左下 360 - θ
*
*引数:象限数, x,zの反転判定, 壁の座標(x,z)
*戻り値: なし
*************************/
void Ball::Reflection(int quad_pattern, int ref_pattern, double wall_x, double wall_z){
	//象限の場合分け
	switch(quad_pattern){
		case 1://第１象限
			//x,zの反転判定の場合分け
			switch(ref_pattern){
				case 1://両方反転 A.C.G.H.
					m_angle = 180 - m_angle;//C.
					if(m_angle < 0.0)m_angle += 360.0;//A.
					m_angle = 360 - m_angle;//G.H.
					m_x -= m_x - wall_x;//余剰座標を反射方向へ加算
					m_z -= m_z - wall_z;//余剰座標を反射方向へ加算
					break;
				case 2://xのみ A.C.
					m_angle = 180 - m_angle;//C.
					if(m_angle < 0.0)m_angle += 360.0;//A.
					m_x -= m_x - wall_x;//余剰座標を反射方向へ加算
					break;
				case 3://zのみ G.H.
					m_angle = 360 - m_angle;//G.H.
					m_z -= m_z - wall_z;//余剰座標を反射方向へ加算
					break;
				case 4://反転しない
					break;
			}
			break;
		case 2://第２象限
			//x,zの反転判定の場合分け
			switch(ref_pattern){
				case 1://両方反転 B.D.G.H
					m_angle = 180 - m_angle;//D.
					if(m_angle < 0.0)m_angle += 360.0;//B.
					m_angle = 360 - m_angle;//G.H.
					m_x = 2 * wall_x - m_x;
					m_z -= m_z - wall_z;//余剰座標を反射方向へ加算
					break;
				case 2://xのみ B.D.
					m_angle = 180 - m_angle;//D.
					if(m_angle < 0.0)m_angle += 360.0;//B.
					m_x = 2 * wall_x - m_x;
					break;
				case 3://zのみ G.H.
					m_angle = 360 - m_angle;//G.H.
					m_z -= m_z - wall_z;//余剰座標を反射方向へ加算
					break;
				case 4://反転しない
					break;
			}
			break;
		case 3://第３象限
			//x,zの反転判定の場合分け
			switch(ref_pattern){
				case 1://両方反転 B.D.E.F.
					m_angle = 180 - m_angle;//D.
					if(m_angle < 0.0)m_angle += 360.0;//B.
					m_angle = 360 - m_angle;//E.F.
					m_x = 2 * wall_x - m_x;
					m_z = 2 * wall_z - m_z;
					break;
				case 2://xのみ B.D.
					m_angle = 180 - m_angle;//D.
					if(m_angle < 0.0)m_angle += 360.0;//B.
					m_x = 2 * wall_x - m_x;
					break;
				case 3://zのみ E.F.
					m_angle = 360 - m_angle;//E.F.
					m_z = 2 * wall_z - m_z;
					break;
				case 4://反転しない
					break;
			}
			break;
		case 4://第４象限
			//x,zの反転判定の場合分け
			switch(ref_pattern){
				case 1://両方反転 A.C.E.F.
					m_angle = 180 - m_angle;//C.
					if(m_angle < 0.0)m_angle += 360.0;//A.
					m_angle = 360 - m_angle;//E.F.
					m_x -= m_x - wall_x;//余剰座標を反射方向へ加算
					m_z = 2 * wall_z - m_z;
					break;
				case 2://xのみ A.C.
					m_angle = 180 - m_angle;//C.
					if(m_angle < 0.0)m_angle += 360.0;//A.
					m_x -= m_x - wall_x;//余剰座標を反射方向へ加算
					break;
				case 3://zのみ E.F.
					m_angle = 360 - m_angle;//E.F.
					m_z = 2 * wall_z - m_z;
					break;
				case 4://反転しない
					break;
			}
			break;
	}

	m_quadrant = -1;//象限位置の初期化
	if(m_angle > 360){m_angle -= 360;}
	else if(m_angle < 0){m_angle += 360;}
}

void Ball::Moving(){
	if(move)motion = checkFriction(0.3);//最大摩擦係数のチェック
	if(motion){
		m_t++;//フレームカウンタの更新
		//加速度の計算
		m_n -= d_f;//動摩擦係数を引き、現在ボールにかかっている力を計算
		m_a = (m_n * m_t) / m_m;//加速度の計算
		m_v = m_a + m_v;//現在速度の更新

		if(m_v < 0.0){
			motion = false;
			return;//現在速度が0以下になったので計算終了
		}

		m_x = m_x + m_v * cos(m_angle * PI / 180.0);//xの次回座標
		m_z = m_z + m_v * sin(m_angle * PI / 180.0);//zの次回座標
	}
}

bool Ball::checkFriction(double newton){
	cout << "//////////////////////////////////////////" << endl
		 << "//////// Move Start! /////////////////////" << endl
		 << "//////////////////////////////////////////" << endl;
	move = false;
	//与えられたnewtonが静止摩擦力を越えていれば
	if(d_f < newton){
		m_n = newton - d_f;//与えられた[N]から動摩擦力を引き代入する
		m_t = 0;//運動を開始するため タイマーの初期化
		m_v = 0.0;//速度の初期化
		return true;
	}
	return false;
}

void Ball::Drawing(){
	//Color
	static const GLfloat red[] = {1.0f, 0.0f, 0.0f};
	static const GLfloat green[] = {0.0f, 1.0f, 0.0f};
	glPushMatrix();
	glTranslatef(m_x,m_y,m_z);//ローカル座標へ移動

	glColor3fv(red);//描画色の指定
	glutWireSphere(size,15,15);//球の描画
	//glutSolidCube(1.0);//箱の描画
	glPopMatrix();//座標系の復元
}

/*************************
*角度をインクリメントして更新していく
*************************/
void Ball::IncAngle(){
	if(motion) return;//球が動いていれば実行しない
	m_angle += 1.0;//角度をインクリメント
	if(m_angle >= 360)m_angle = 0.0;//角度の初期化
}

/*************************
*進行方向のベクトルラインを描画
*************************/
void Ball::DrawVectorLine(){
	if(motion) return;//球が動いていれば実行しない
	double xLine = 2 * cos(m_angle * PI / 180.0);
	double zLine = 2 * sin(m_angle * PI / 180.0);
	glPushMatrix();
	glTranslatef(m_x,m_y,m_z);//ローカル座標へ移動
	glColor3f(0.5,0.5,0.5);
	glLineWidth(5.0);
	glBegin(GL_LINES);
	glVertex3f((float)xLine,1.0,(float)zLine);
	glVertex3f((float)xLine * 10.0,1.0,(float)zLine * 10.0);
	glEnd();
	glLineWidth(1.0);
	glPopMatrix();//座標系の復元
}