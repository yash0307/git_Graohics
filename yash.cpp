/***
 *****YASH PATEL*****
 ***/

#include<iostream>
#include<GL/glut.h>
#include<stdlib.h>
#include<cmath>
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
using namespace std;

class coin
{
	public : 
		double x_coordinate;
		double y_coordinate;
		double x_velocity;
		double y_velocity;
		int  coin_type; // 0 for white, 1 for black, 2 for red.
};
//Declaring initial data//
coin basic_coins[11];
coin striker;
GLdouble mouse_x=0, mouse_y=0, mouse_z=0;
int flag_striker = 0;
float angle_striker = 90;
float striker_power = 0;
//data for initial//
class Patches
{
	public : 
		double x_coordinate;
		double y_coordinate;
		double radius;
		int patch_number;
};
Patches holes[4];
void date_for_holes()
{
	holes[0].x_coordinate = 1.63f;
	holes[0].y_coordinate = 1.63f;
	holes[0].radius = 0.1f;
	holes[0].patch_number = 1;
	holes[1].x_coordinate = -1.63f;
	holes[1].y_coordinate = -1.63f;
	holes[1].radius = 0.1f;
	holes[1].patch_number = 2;
	holes[2].x_coordinate = 1.63f;
	holes[2].y_coordinate = -1.63f;
	holes[2].radius = 0.1f;
	holes[2].patch_number = 3;
	holes[3].x_coordinate = -1.63f;
	holes[3].y_coordinate = 1.63f;
	holes[3].radius = 0.1f;
	holes[3].patch_number = 4;
}
void initialize_data_for_coins()
{
	int i;
	for(i=0;i<11;i++)
	{
		if(i==10)
		{
			basic_coins[i].x_coordinate = 0.0;
			basic_coins[i].y_coordinate = 0.0;
			basic_coins[i].x_velocity = 0.0;
			basic_coins[i].y_velocity = 0.0;
			basic_coins[i].coin_type = 2; //red coin.
		}
		else
		{
			basic_coins[i].x_coordinate = 0.3*sin(2*3.14159*i/10);
			basic_coins[i].y_coordinate = 0.3*cos(2*3.14159*i/10);
			basic_coins[i].x_velocity = 0.0;
			basic_coins[i].y_velocity = 0.0;
			basic_coins[i].coin_type = i%2;
		}
	}
}
void handleKeypress(unsigned char key, int x, int y)
{
	if(key == 27)
	{
		exit(0);
	}
	else if(key == 97)
	{
		angle_striker++;
	}
	else if(key==99)
	{
		angle_striker--;
	}
	else if(key == 32)
	{
		striker.x_velocity = (striker_power * cos(3.14159*angle_striker/180))/10;
		striker.y_velocity = (striker_power * sin(3.14159*angle_striker/180))/10;
	}
}
void initRendering()
{
	glEnable(GL_DEPTH_TEST);
}
void handleResize(int w, int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,(double)w/(double)h,1.0,200.0);
}
void DrawCircle(float cx, float cy, float rad) 
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex2f( cx + rad * cos(DEG2RAD(i)), cy + rad * sin(DEG2RAD(i)));
	}
	glEnd();
}
void CentralCircle(float rad)
{
	glBegin(GL_POINTS);
	for(int i=0;i<1000;++i)
	{
		glVertex2f(rad * cos(2*3.14159*i/1000.0),rad * sin(2*3.14159*i/1000.0));
	}
	glEnd();
}
void DrawArch(float x, float y, float r)
{
	glBegin(GL_POINTS);
	for(int i=325; i<925; i++)
	{
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	glEnd();
}
void DrawArch2(float x, float y, float r)
{
	glBegin(GL_POINTS);
	for(int i=825; i<1000; i++)
	{
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	for(int i = 0 ; i < 425 ; i++)
	{	
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	glEnd();
}
void DrawArch3(float x, float y, float r)
{
	glBegin(GL_POINTS);
	for(int i=575; i < 1000; i++)
	{
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	for(int i = 0 ; i < 175 ; i++)
	{
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	glEnd();
}
void DrawArch4(float x, float y, float r)
{
	glBegin(GL_POINTS);
	for(int i = 0 ; i < 175 ; i++)
	{
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	for(int i = 575; i < 1000 ; i++)
	{
		glVertex2f(x + r * cos(2*3.14159*i/1000.0), y + r * sin(2*3.14159*i/1000.0));
	}
	glEnd();
}
void DrawStriker(float cx, float cy, float rad) 
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex2f( cx + rad * cos(DEG2RAD(i)), cy + rad * sin(DEG2RAD(i)));
	}
	glEnd();
}
void DrawCoins(float cx, float cy, float rad) 
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex2f( cx + rad * cos(DEG2RAD(i)), cy + rad * sin(DEG2RAD(i)));
	}
	glEnd();
}
void draw_initial_coins()
{
	int i;
	for(i=0;i<10;i++)
	{
		if(i%2==0)
		{
			glColor3f(1.0f, 1.0f,1.0f);
			DrawCoins(basic_coins[i].x_coordinate,basic_coins[i].y_coordinate,0.050);
		}
		else if(i%2==1)
		{
			glColor3f(0.2f,0.2f,0.2f);
			DrawCoins(basic_coins[i].x_coordinate,basic_coins[i].y_coordinate,0.050);
		}
	}
	glColor3f(1.0f, 0.3f, 0.3f);
	DrawCoins(basic_coins[10].x_coordinate, basic_coins[10].y_coordinate, 0.050);
}
void draw_initial_striker()
{
	glPushMatrix();
	if(flag_striker == 0)
	{
		flag_striker = 1;
		striker.x_coordinate = 0.0;
		striker.y_coordinate = -1.165;
		striker.x_velocity = 0;
		striker.y_velocity = 0;
		striker.coin_type = 3;
	}
	glColor3f(0.0f, 0.3f, 0.7f);
	DrawStriker(striker.x_coordinate, striker.y_coordinate, 0.070);
	glPopMatrix();
}
void drawPatches(float cx, float cy, float rad) 
{
	glBegin(GL_TRIANGLE_FAN);
	for(int i=0 ; i<360 ; i++) 
	{
		glVertex2f( cx + rad * cos(DEG2RAD(i)), cy + rad * sin(DEG2RAD(i)));
	}
	glEnd();
}
void drawHoles()
{
	glColor3f(0.0f, 0.0f, 0.0f);
	date_for_holes();
	int i;
	for(i=0; i<4; i++)
	{
		drawPatches(holes[i].x_coordinate, holes[i].y_coordinate, holes[i].radius);
	}
}
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -5.0f);
	draw_initial_striker();
	initialize_data_for_coins();
	draw_initial_coins();
	glColor3f(0.2f, 0.7f, 0.2f);
	glBegin(GL_LINES);
	glVertex2f(striker.x_coordinate, striker.y_coordinate);
	glVertex2f(striker.x_coordinate + 1.0*cos(2*3.14159*angle_striker/360), striker.y_coordinate + 1.0*sin(2*3.14159*angle_striker/360));
	glEnd();
	drawHoles();
	glLineWidth(3.0);
	glColor3f(0.2f, 0.7f, 0.2f);
	glBegin(GL_LINES);
	glVertex2f(2.0f, 0.0f);
	glVertex2f(2.0f, striker_power);
	glEnd();
	glColor3f(0.1f, 0.4f, 0.5f);
	DrawCircle(1.16f, 1.16f, 0.05f);
	DrawCircle(-1.16f, -1.16f, 0.05f);
	DrawCircle(-1.16f, 1.16f, 0.05f);
	DrawCircle(1.16f, -1.16f, 0.05f);
	glPointSize(3.0);
	CentralCircle(0.60f);
	glPointSize(1.0);
	glColor3f(0.0f,0.0f,0.0f);
	CentralCircle(0.4f);
	glLineWidth(2.0);
	DrawArch(0.77f,0.77f,0.1f);
	DrawArch2(-0.77f, -0.77f, 0.1f);
	DrawArch3(0.77f, -0.77f, -0.1f);
	DrawArch4(-0.77f, 0.77f, 0.1f);
	glBegin(GL_LINES);
	glVertex2f(-1.125f, -1.125f);
	glVertex2f(1.125f, -1.125f);
	glVertex2f(1.125f, -1.125f);
	glVertex2f(1.125f, 1.125f);
	glVertex2f(1.125f, 1.125f);
	glVertex2f(-1.125f, 1.125f);
	glVertex2f(-1.125f, 1.125f);
	glVertex2f(-1.125f, -1.125f);
	glVertex2f(-1.3f, -1.3f);
	glVertex2f(-0.7f, -0.7f);
	glVertex2f(1.3f, 1.3f);
	glVertex2f(0.7f, 0.7f);
	glVertex2f(-1.3f, 1.3f);
	glVertex2f(-0.7f, 0.7f);
	glVertex2f(1.3f, -1.3f);
	glVertex2f(0.7f, -0.7f);
	glEnd();
	GLfloat thickness;
	thickness = 5.0;
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-1.205f, -1.205f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(1.205f, -1.205f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(1.205f, 1.205f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(-1.205f, 1.205f);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex2f(-1.205f, 1.205f);
	glColor3f(1.0f,0.0f,0.0f);
	glVertex2f(-1.205f,-1.205f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(1.205f,1.205f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(1.205f, -1.205f);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1.0f,0.86f,0.6f);
	glVertex2f(-1.7f, -1.7f);
	glColor3f(1.0f,0.86f,0.6f);
	glVertex2f(1.7f, -1.7f);
	glColor3f(1.0f,0.86f,0.6f);
	glVertex2f(1.7f, 1.7f);
	glColor3f(1.0f,0.86f,0.6f);
	glVertex2f(-1.7f, 1.7f);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.54f, 0.35f, -5.0f);
	glVertex2f(-1.9f,-1.9f);
	glColor3f(0.54f, 0.35f, -5.0f);
	glVertex2f(1.9f, -1.9f);
	glColor3f(0.54f, 0.35f, -5.0f);
	glVertex2f(1.9f, 1.9f);
	glColor3f(0.54f, 0.35f, -5.0f);
	glVertex2f(-1.9f, 1.9f);
	glEnd();
	DrawCircle(1.16f, 1.16f, 0.05f);
	DrawCircle(-1.16f, -1.16f, 0.05f);
	DrawCircle(-1.16f, 1.16f, 0.05f);
	DrawCircle(1.16f, -1.16f, 0.05f);
	glPointSize(3.0);
	CentralCircle(0.60f);
	glPointSize(1.0);
	glColor3f(0.0f,0.0f,0.0f);
	CentralCircle(0.4f);
	glLineWidth(2.0);
	DrawArch(0.77f,0.77f,0.1f);
	DrawArch2(-0.77f, -0.77f, 0.1f);
	DrawArch3(0.77f, -0.77f, -0.1f);
	DrawArch4(-0.77f, 0.77f, 0.1f);
	initialize_data_for_coins();
	draw_initial_coins();
	glPopMatrix();
	glutSwapBuffers();
}
void update(int value)
{
	int i;
	striker.x_coordinate = striker.x_coordinate + striker.x_velocity;
	striker.y_coordinate = striker.y_coordinate + striker.y_velocity;
	for(i=0;i<11;i++)
	{
		basic_coins[i].x_coordinate += basic_coins[i].x_velocity;
		basic_coins[i].y_coordinate += basic_coins[i].y_velocity;
		if(basic_coins[i].x_coordinate + 0.05 > 1.7 || basic_coins[i].x_coordinate - 0.05 < -1.7)
		{
			basic_coins[i].x_velocity *= -1;
		}
		if(basic_coins[i].y_coordinate + 0.05 > 1.7 || basic_coins[i].y_coordinate - 0.05 < -1.7)
		{
			basic_coins[i].y_velocity *= -1;
		}
		float magnitude_velocity_coin = sqrt((basic_coins[i].x_velocity*basic_coins[i].x_velocity)+(basic_coins[i].y_velocity*basic_coins[i].y_velocity));
		if(basic_coins[i].x_velocity!=0 && magnitude_velocity_coin!=0)
		{
			basic_coins[i].x_velocity -= 0.0007*basic_coins[i].x_velocity/magnitude_velocity_coin;
		}
		if(basic_coins[i].y_velocity!=0 && magnitude_velocity_coin!=0)
		{
			basic_coins[i].y_velocity = basic_coins[i].y_velocity - 0.0007*basic_coins[i].y_velocity/magnitude_velocity_coin;
		}
		if(fabs(basic_coins[i].x_velocity) <= 0.0007)
		{
			basic_coins[i].x_velocity = 0;
		}
		if(fabs(basic_coins[i].y_velocity) <= 0.0007)
		{
			basic_coins[i].y_velocity = 0;
		}

	}
	if(striker.x_coordinate + 0.07 > 1.7 || striker.x_coordinate - 0.07 < -1.7)
	{
		striker.x_velocity *= -1;
	}
	if(striker.y_coordinate + 0.07 > 1.7 || striker.y_coordinate - 0.07 < -1.7)
	{
		striker.y_velocity *= -1;
	}
	float magnitude_velocity_striker = sqrt((striker.x_velocity*striker.x_velocity)+(striker.y_velocity*striker.y_velocity));
	if(striker.x_velocity!=0 && magnitude_velocity_striker!=0)
	{
		striker.x_velocity = striker.x_velocity - 0.0007*striker.x_velocity/magnitude_velocity_striker;
	}
	if(striker.y_velocity!=0 && magnitude_velocity_striker!=0)
	{
		striker.y_velocity = striker.y_velocity - 0.0007*striker.y_velocity/magnitude_velocity_striker;
	}
	if(fabs(striker.x_velocity) <= 0.0007)
	{
		striker.x_velocity = 0;
	}
	if(fabs(striker.y_velocity) <= 0.0007)
	{
		striker.y_velocity = 0;
	}
	//striker and coin collision.
	glutTimerFunc(10,update, 0);
}
void handleKeypress2(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		if(striker.x_coordinate > -1.07f )
		{
			striker.x_coordinate -= 0.1;
		}
		else
		{
			striker.x_coordinate -= 0.0;
		}
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if(striker.x_coordinate < 1.07f)
		{
			striker.x_coordinate += 0.1;
		}
		else
		{
			striker.x_coordinate += 0.0;
		}
	}
	if(key == GLUT_KEY_UP)
	{
		if(striker_power >= 1.0)
		{
			striker_power = 1.0;
		}
		else
		{
			striker_power = striker_power + 0.1f;
		}
	}
	if(key == GLUT_KEY_DOWN)
	{
		if(striker_power <= 0)
		{
			striker_power = 0;
		}
		else
		{
			striker_power = striker_power - 0.1f;
		}
	}
}
void handleMouseClick(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN)
	{
		if(button == GLUT_LEFT_BUTTON)
			exit(0);
		if(button == GLUT_RIGHT_BUTTON)
			exit(0);
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutCreateWindow("Game");
	initRendering();
	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleKeypress2);
	glutMouseFunc(handleMouseClick);
	glutReshapeFunc(handleResize);
	glutTimerFunc(10, update, 0);
	glutMainLoop();
	return 0;
}
