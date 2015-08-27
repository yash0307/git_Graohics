/***
  Yash Patel
  201301134,CSE
 ***/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "target.cpp"
#include "imageloader.h"
#include "vec3f.h"
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

Target tr;

Vec3f g(0.0, -1.0, 0.0);
float dott;
Vec3f gt;

int speed_given = 1;
float angle = 0.0f;
float angle2 = -30.0f;
float angle3 =  0.0f;
float angle4 = 0.0f;
int score = 0;
//class for the spin
class Spin
{
	public:
		float latto_x, latto_z, latto_velocityx, latto_velocityz, latto_height1, laty;
		float latto_radius1, latto_radius2, latto_height2, latto_n1, latto_n2;
		float speed;
		int score;
		Spin()
		{
			latto_x = 0.0f;
			latto_z = 127.0f;
			latto_velocityx = 0.0f;
			latto_velocityz = 0.0f;
			latto_radius1 = 1.2f;
			latto_radius2 = 1.0f;			
			latto_n1 = 0.1;
			latto_n2 = 0.3;
			latto_height1 = 2.5f;
			latto_height2 = 2.0f;
			speed = 0.0; 
			score = 0;
		}
		void uplats(float s)
		{
			this->speed = s;
		}
		void uplatx(float x)
		{
			this->latto_x = x;
		}
		void uplatz(float z)
		{
			this->latto_z = z;
		}
		void uplatvx(float vx)
		{
			this->latto_velocityx = vx;
		}
		void uplatvz(float vz)
		{
			this->latto_velocityz = vz;
		}
		void uplaty(float y)
		{
			this->laty = y;
		}
};
Spin latto;
//Represents a terrain, by storing a set of heights and normals at 2D locations
class Terrain {
	private:
		int w; //Width
		int l; //Length
		float** hs; //Heights
		Vec3f** normals;
		bool computedNormals; //Whether normals is up-to-date
	public:
		Terrain(int w2, int l2) {
			w = w2;
			l = l2;

			hs = new float*[l];
			for(int i = 0; i < l; i++) {
				hs[i] = new float[w];
			}

			normals = new Vec3f*[l];
			for(int i = 0; i < l; i++) {
				normals[i] = new Vec3f[w];
			}

			computedNormals = false;
		}

		~Terrain() {
			for(int i = 0; i < l; i++) {
				delete[] hs[i];
			}
			delete[] hs;

			for(int i = 0; i < l; i++) {
				delete[] normals[i];
			}
			delete[] normals;
		}

		int width() {
			return w;
		}

		int length() {
			return l;
		}

		//Sets the height at (x, z) to y
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
			computedNormals = false;
		}

		//Returns the height at (x, z)
		float getHeight(int x, int z) {
			return hs[z][x];
		}

		//Computes the normals, if they haven't been computed yet
		void computeNormals() {
			if (computedNormals) {
				return;
			}

			//Compute the rough version of the normals
			Vec3f** normals2 = new Vec3f*[l];
			for(int i = 0; i < l; i++) {
				normals2[i] = new Vec3f[w];
			}

			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum(0.0f, 0.0f, 0.0f);

					Vec3f out;
					if (z > 0) {
						out = Vec3f(0.0f, hs[z - 1][x] - hs[z][x], -1.0f);
					}
					Vec3f in;
					if (z < l - 1) {
						in = Vec3f(0.0f, hs[z + 1][x] - hs[z][x], 1.0f);
					}
					Vec3f left;
					if (x > 0) {
						left = Vec3f(-1.0f, hs[z][x - 1] - hs[z][x], 0.0f);
					}
					Vec3f right;
					if (x < w - 1) {
						right = Vec3f(1.0f, hs[z][x + 1] - hs[z][x], 0.0f);
					}

					if (x > 0 && z > 0) {
						sum += out.cross(left).normalize();
					}
					if (x > 0 && z < l - 1) {
						sum += left.cross(in).normalize();
					}
					if (x < w - 1 && z < l - 1) {
						sum += in.cross(right).normalize();
					}
					if (x < w - 1 && z > 0) {
						sum += right.cross(out).normalize();
					}

					normals2[z][x] = sum;
				}
			}

			//Smooth out the normals
			const float FALLOUT_RATIO = 0.5f;
			for(int z = 0; z < l; z++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum = normals2[z][x];

					if (x > 0) {
						sum += normals2[z][x - 1] * FALLOUT_RATIO;
					}
					if (x < w - 1) {
						sum += normals2[z][x + 1] * FALLOUT_RATIO;
					}
					if (z > 0) {
						sum += normals2[z - 1][x] * FALLOUT_RATIO;
					}
					if (z < l - 1) {
						sum += normals2[z + 1][x] * FALLOUT_RATIO;
					}

					if (sum.magnitude() == 0) {
						sum = Vec3f(0.0f, 1.0f, 0.0f);
					}
					normals[z][x] = sum;
				}
			}

			for(int i = 0; i < l; i++) {
				delete[] normals2[i];
			}
			delete[] normals2;

			computedNormals = true;
		}

		//Returns the normal at (x, z)
		Vec3f getNormal(int x, int z) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[z][x];
		}
};
//Loads a terrain from a heightmap.  The heights of the terrain range from
//-height / 2 to height / 2.
Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}

	delete image;
	t->computeNormals();
	return t;
}


Terrain* ter;

void cleanup() {
	delete ter;
}
/*************************************Handle KEYS ************************************************/
void handleKeypress2_speical(int key, int x, int y)
{
	float speed_at_this_point;
	speed_at_this_point = sqrt((latto.latto_velocityx*latto.latto_velocityx) + (latto.latto_velocityz*latto.latto_velocityz));
	if(speed_at_this_point == 0)
	{
		if (key == GLUT_KEY_UP)
		{
			if((speed_given == 10) && (speed_given < 11))
			{
				speed_given = 0; // round it if exceeds.
			}
			else if((speed_given!=10) && (speed_given < 11))
			{
				speed_given = speed_given + 1;
			}
		}
		else if(key == GLUT_KEY_DOWN)
		{
			if((speed_given == 0) && (speed_given > -1))
			{
				speed_given = 10; // round it if decreases less then zero.
			}
			else if((speed_given!=0) && (speed_given > -1))
			{
				speed_given = speed_given - 1;
			}
		}
		else if(key == GLUT_KEY_LEFT)
		{
			if(latto.latto_x!=0.0)
				latto.latto_x = latto.latto_x - 1.0;
		}
		else if(key == GLUT_KEY_RIGHT)
		{
			if(latto.latto_x!=127.0)
				latto.latto_x = latto.latto_x + 1.0;
		}
	}
}

void handleKeypress(unsigned char key, int x, int y)
{
	float speed_at_this_point;
	speed_at_this_point = sqrt((latto.latto_velocityx*latto.latto_velocityx) + (latto.latto_velocityz*latto.latto_velocityz));
	if(speed_at_this_point == 0)
	{
		if(key == 27)
		{
			cleanup();
			exit(0);
		}
		if (key == 'a')
		{
			angle2 = -30.0;
			angle = angle + 1.0f;
			//condition for after complete roataion.
			if( angle > 360)
			{
				angle = angle - 360;
			}
		}
		if(key == 'd')
		{
			angle2 = -30.0;
			angle  = angle - 1.0f;
			// opposite complete rotation.
			if(angle < 0)
			{
				angle = angle + 360;
			}
		}
		if(key == 'w')
		{
			angle2 = -90.0f;
		}
		if(key == 's')
		{
			angle2 = 90.0f;
		}
		if (key == 'x')
		{
			if(angle3!=-90)
			{
				angle3 = angle3 - 5;
			}
		}
		if(key == 'z')
		{
			if(angle3!=90)
			{
				angle3 = angle3 + 5;
			}
		}
		//speed and power stuff goes here :D
		if (key == 32)//for space ascii is 32.
		{
			latto.speed = speed_given*0.1;
			latto.latto_velocityz = latto.speed*cos(DEG2RAD(-angle3));
			latto.latto_velocityx = latto.speed*sin(DEG2RAD(-angle3));
		}
		if(key == 'r')
		{
			tr.tarx = rand()%128;
			tr.tarz = rand()%128;
			if(tr.tarz >= 64)
				tr.tarz -= 50;
		}
		else
		{
			//do nothing.
		}
	}
}
/***********************************done with keys*************************************************/

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}
string text;
void get_text()
{
	string temp = "PLAYER. SCORE :";
	ostringstream oss;
	oss << score;
	text = temp + oss.str();

}
int check_collision()
{
	float distance;
	distance = sqrt(((latto.latto_x - tr.tarx)*(latto.latto_x - tr.tarx)) + ((latto.latto_z - tr.tarz)*(latto.latto_z - tr.tarz)));
	if(distance < 0.03f)
	{
		score++;
		return 1;
	}
	return 0;
}

int check_boundary()
{
	if(latto.latto_x >= 127 ||  latto.latto_z <= 0 || latto.latto_x <=0)
		return 1;
	return 0;
}

void drawtarget()
{
	glPushMatrix();
	glTranslatef(tr.tarx, ter->getHeight(tr.tarx, tr.tarz)+tr.tarr1+1, tr.tarz);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidTorus(1, tr.tarr2, 10, 10);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidTorus(1, tr.tarr1, 10, 10);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidSphere(tr.tarr1-1, 32, 32);
	glPopMatrix();
}

void drawarr()
{
	glPushMatrix();
	glColor3f(160.0/256.0, 160.0/256.0, 160.0/256.0);
	GLUquadricObj *quadratic3;
	quadratic3 = gluNewQuadric();
	glTranslatef(-10.0, 20.0, 127.0 - 20.0);
	glTranslatef(0.0, 0.0, 20.0);
	glRotatef(angle3, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -20.0);
	gluCylinder(quadratic3, 1.0, 1.0, 15.0, 32, 32);
	glPushMatrix();
	glRotatef(45.0, 0.0, 1.0, 0.0);
	gluCylinder(quadratic3, 1.0, 1.0, 5.0, 32, 32);
	glPopMatrix();
	glPushMatrix();
	glRotatef(-45.0, 0.0, 1.0, 0.0);
	gluCylinder(quadratic3, 1.0, 1.0, 5.0, 32, 32);
	glPopMatrix();
	glPopMatrix();
}

void drawcube(float z, int i)
{
	glPushMatrix();
	glTranslatef(135.0, 5.0, z-4.0 );
	glColor3f((i*28.4)/256.0, ((9-i)*28.4)/256.0, 0.0);
	glutSolidSphere(2.0, 32, 32);
	
	//glutSolidCube(8.0);
	glPopMatrix();
}

void drawtop(Vec3f nor)
{
	glPushMatrix();
	//lower tip

	glTranslatef(latto.latto_x, ter->getHeight(latto.latto_x,latto.latto_z)+latto.latto_height2, latto.latto_z);
	glRotatef(90.0, nor[0], nor[1], nor[2]);
	glRotatef(90.0f, 1.0, 0.0, 0.0);
	glRotatef(angle4, 0.0, 0.0, -1.0);
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	glColor3f(139/256.0, 197/256.0, 137/256.0);
	gluCylinder(quadratic, latto.latto_n2, latto.latto_n1, latto.latto_height2, 32, 32);

	//middle tip

	glTranslatef(0.0, 0.0, -latto.latto_height1);
	glColor3f(205.0/256.0, 51.0/256.0, 51.0/256.0);
	GLUquadricObj *quadratic2;
	quadratic2 = gluNewQuadric();
	gluCylinder(quadratic2, latto.latto_radius1, latto.latto_n2, latto.latto_height1, 32, 32);

	//upper torus
	glColor3f(176.0/256.0, 23.0/256.0, 31.0/256.0);
	glutWireTorus(latto.latto_radius2, latto.latto_radius1, 10, 10);
	glPopMatrix();

}

void drawText(const char *text, int length, int x, int y)
{
	glMatrixMode(GL_PROJECTION);
	double *matrix = new double[16];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix);
	glLoadIdentity();
	glOrtho(0,800,0,600,-5,5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRasterPos2i(x,y);
	for(int i=0; i< length ; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (int)text[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matrix);
	glMatrixMode(GL_MODELVIEW);
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -7.8f);
	//glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(-angle2, 1.0f, 0.0f, 0.0f);
	glRotatef(-angle, 0.0f, 1.0f, 0.0f);

	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	GLfloat lightColor0[] = {1.0f, 0.8f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	float scale = 6.0f / max(ter->width() - 1, ter->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(ter->width() - 1) / 2,
			0.0f,
			-(float)(ter->length() - 1) / 2);

	glColor3f(122.0/244, 118.0/244, 160.0/244);
	for(int z = 0; z < ter->length() - 1; z++)
	{
		//Makes OpenGL draw a triangle at every three consecutive vertices
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < ter->width(); x++) {
			Vec3f normal = ter->getNormal(x, z);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, ter->getHeight(x, z), z);
			normal = ter->getNormal(x, z + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glVertex3f(x, ter->getHeight(x, z + 1), z + 1);
		}
		glEnd();
	}
	Vec3f nor = ter->getNormal(latto.latto_x, latto.latto_z);
	//Vec3f g(0.0, 1.0, 0.0);
	dott = nor.dot(g);
	gt = g + nor*dott;
	drawtop(nor);
	drawarr();
	for (int i = 0; i < speed_given; ++i)
	{
		drawcube(127.0 - i*10.0, i);
	}
	drawtarget();
	get_text();
	drawText(text.data(), text.size(), 0, 0);
	glutSwapBuffers();
}

void update(int value) {

	glutPostRedisplay();
	angle4+=10;

	if(check_collision())
	{
		latto.score++;
		latto.latto_x = 64.0f;
		latto.latto_z = 127.0f;
		latto.latto_velocityx = 0.0f;
		latto.latto_velocityz = 0.0f;
		tr.tarx = rand()%128;
		tr.tarz = rand()%128;
		if(tr.tarz >= 64)
			tr.tarz -= 50;
	}
	if(check_boundary())
	{
		latto.latto_x = 64.0f;
		latto.latto_z = 127.0f;
		latto.latto_velocityx = 0.0f;
		latto.latto_velocityz = 0.0f;
		tr.tarx = rand()%128;
		tr.tarz = rand()%128;
		if(tr.tarz >= 64)
			tr.tarz -= 50;
	}
	latto.latto_x+=latto.latto_velocityx;
	latto.latto_z-=latto.latto_velocityz;
	glutTimerFunc(25, update, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	int windowWidth = glutGet(GLUT_SCREEN_WIDTH);
	int windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);

	glutCreateWindow("Terrain");
	initRendering();
	tr.tarx = rand()%128;
	tr.tarz = rand()%128;
	if(tr.tarz >= 64)
		tr.tarz -= 50;
	ter = loadTerrain("Sample.bmp", 20);
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleKeypress2_speical);
	glutReshapeFunc(handleResize);

	glutTimerFunc(25, update, 0);

	glutMainLoop();
	return 0;
}
