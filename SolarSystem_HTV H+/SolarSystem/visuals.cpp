// Thư viện Opengl
#include <stdio.h>  
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <GL/freeglut.h>
#include <GL/glut.h>   
#include "visuals.h"   // Tệp cho các chức năng OpenGL
#include "imageloader.h"

static float worldX = 15.0;
static float worldY = 0.0;

static float scaleFactor = 3;

static float first = 1;
static float second = 1;
point stars[500];

int globalW, globalH;

GLuint x = 0;
GLuint y = 0;

int isAnimate = 0;
int bigOrbitActive = 1;
int smallOrbitActive = 1;
int moonsActive = 1;
int logoScene = 1;
int changeCamera = 0;
int zoom = 50;



using namespace std;

class Planet {
public:
	float radius, distance, orbit, orbitSpeed, axisTilt, axisAni;
	Planet(float _radius, float _distance, float _orbit, float _orbitSpeed, float _axisTilt, float _axisAni) {
		radius = _radius; //bán kính
		distance = _distance; //khoảng cách
		orbit = _orbit; //Quỹ Đạo
		orbitSpeed = _orbitSpeed; //vận tốc quỹ đạo
		axisTilt = _axisTilt; //trục nghiêng
		axisAni = _axisAni;//trục động
	}

	void drawSmallOrbit(void) {
		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		glutWireTorus(0.001, distance, 100.0, 100.0);
		glPopMatrix();
	}

	void drawMoon(void) {
		GLUquadricObj* quadric;
		quadric = gluNewQuadric();

		glPushMatrix();
		glColor3ub(255, 255, 255);
		glRotatef(orbit, 0.0, 1.0, 0.0);
		glTranslatef(distance, 0.0, 0.0);
		gluSphere(quadric, radius, 20.0, 20.0);
		glPopMatrix();
	}

};

// set thông số cho từng ngôi sao
Planet sun(5.0, 0, 0, 6.42, 7.25, 0);				//mặt trời
Planet mer(1.0, 7, 0, 4.74, 02.11, 0);		//sao thủy 
Planet ven(1.5, 11, 0, 3.50, 177.0, 0);		//sao kim
Planet ear(2.0, 16, 0, 2.98, 23.44, 0);		//trái đất
Planet mar(1.2, 21, 0, 2.41, 25.00, 0);		//sao hỏa
Planet jup(3.5, 28, 0, 1.31, 03.13, 0);		//soa mộc
Planet sat(3.0, 37, 0, 0.97, 26.70, 0);		//sao thổ
Planet ura(2.5, 45.5, 0, 0.68, 97.77, 0);	//sao thiên vương
Planet nep(2.3, 53.6, 0, 0.54, 28.32, 0);	//sao hải vương
Planet plu(0.3, 59, 0, 0.47, 119.6, 0);		//sao diêm vương
Planet lun(.40, 3, 0, 5.40, 0, 0);			//Luna   (Earth) mặt trăng của trái đất 
Planet pho(.20, 1.8, 0, 2.30, 0, 0);		//Phobos   (Mars) mặt trăng của sao hỏa
Planet dei(.24, 2.4, 0, 3.60, 0, 0);		//Deimos   (Mars) mặt trăng của sao hỏa
Planet eur(.24, 4, 0, 4.40, 0, 0);			//Europa   (Jupiter) mặt trăng của sao mộc
Planet gan(.24, 4.7, 0, 5.00, 0, 0);		//Ganymede (Jupiter) mặt trăng của sao mộc
Planet cal(.24, 5.3, 0, 2.30, 0, 0);		//Callisto (Jupiter) mặt trăng của sao mộc
Planet tit(.75, 3.7, 0, 2.40, 0, 0);		//Titan	   (Saturn) mặt trăng của sao thổ
Planet nix(.10, 1.5, 0, 5.00, 0, 0);		//Nix	   (Pluto) mặt trăng của Sao Diêm Vương
Planet puc(.26, 2.9, 0, 7.00, 0, 0);		//Puck	   (Uranus) mặt trăng của Sao Thiên Vương
Planet tri(.36, 3.2, 0, 3.40, 0, 0);		//Triton   (Neptune) mặt trăng của sao Hải vương

// ham ve chu
void keimeno(const char* str, float size)
{

	glPushMatrix();
		glScalef(size, size, size);

		for (int i = 0; i < strlen(str); i++)
			glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	glPopMatrix();

}

GLuint loadTexture(Image* image) {

	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}

GLuint sunTexture, merTexture, venTexture, earTexture, marTexture, jupTexture, satTexture, uraTexture, nepTexture, pluTexture, staTexture, logTexture;

void writeBitmapString(void* font, char* string)
{
	char* c;
	for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}

void load_map(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);

	//TEXUTRING SETUP
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	Image* sta = loadBMP("stars.bmp");		staTexture = loadTexture(sta);		delete sta;
	Image* sun = loadBMP("2k_sun.bmp");		sunTexture = loadTexture(sun);		delete sun;
	Image* mer = loadBMP("mercury.bmp");	merTexture = loadTexture(mer);		delete mer;
	Image* ven = loadBMP("venus.bmp");		venTexture = loadTexture(ven);		delete ven;
	Image* ear = loadBMP("earth.bmp");		earTexture = loadTexture(ear);		delete ear;
	Image* mar = loadBMP("mars.bmp");		marTexture = loadTexture(mar);		delete mar;
	Image* jup = loadBMP("jupiter.bmp");	jupTexture = loadTexture(jup);		delete jup;
	Image* sat = loadBMP("saturn.bmp");		satTexture = loadTexture(sat);		delete sat;
	Image* ura = loadBMP("uranus.bmp");		uraTexture = loadTexture(ura);		delete ura;
	Image* nep = loadBMP("neptune.bmp");	nepTexture = loadTexture(nep);		delete nep;
	Image* plu = loadBMP("pluto.bmp");		pluTexture = loadTexture(plu);		delete plu;
	Image* log = loadBMP("htv.bmp");		logTexture = loadTexture(log);		delete log;

}

void orbitalTrails(void) {
	glPushMatrix();

	glColor3ub(255, 255, 255); // doi lai he mau RGB
	glTranslatef(0.0, 0.0, 0.0); //tinh tien ve goc toa do 
	glRotatef(90.0, 1.0, 0.0, 0.0); //quay 90 do xung quang truc x

	glutWireTorus(0.001, mer.distance, 100.0, 100.0); //ve hinh donut co chieu day 0.001, ban kinh la ban kinh quy dao cua mercury cur.distance
	glutWireTorus(0.001, ven.distance, 100.0, 100.0);
	glutWireTorus(0.001, ear.distance, 100.0, 100.0);
	glutWireTorus(0.001, mar.distance, 100.0, 100.0);
	glutWireTorus(0.001, jup.distance, 100.0, 100.0);
	glutWireTorus(0.001, sat.distance, 100.0, 100.0);
	glutWireTorus(0.001, ura.distance, 100.0, 100.0);
	glutWireTorus(0.001, nep.distance, 100.0, 100.0);
	glutWireTorus(0.001, plu.distance, 100.0, 100.0);

	glPopMatrix();
}

void Render(void)
{
	//CLEARS FRAME BUFFER ie COLOR BUFFER& DEPTH BUFFER (1.0)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clean up the colour of the window
	// and the depth buffer
	glMatrixMode(GL_MODELVIEW);
	
	
	glLoadIdentity();
	

	if (changeCamera == 0)gluLookAt(0.0, zoom, 120, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	if (changeCamera == 1)gluLookAt(0.0, 0.0, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	
	

	glTranslatef(0, 0, -100);
	
	if (!isAnimate) {
		glPushMatrix();
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glColor3f(1, 1, 1); 
		glTranslatef(-8, 50, 0.0);
		keimeno("Pause", 0.05f);
		glPopMatrix();
	}

	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glColor3f(1, 1, 1);
	glTranslatef(-100, 50.0, 0.0);
	keimeno("Nhom HTV H+", 0.03f);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();



	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluDeleteQuadric(quadric);

	glScalef(scaleFactor, scaleFactor, scaleFactor);

	glRotatef(worldX, 1, 0, 0);
	glRotatef(worldY, 0, 1, 0);

	glDisable(GL_LIGHTING);
	
	//mặt trời

	glPushMatrix();
	glRotatef(sun.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sun.distance, 0.0, 0.0);

	glColor3f(0.8, 0.498039, 0.196078);
	gluSphere(quadric, 0, 0, 0);
	glColor3f(1, 1, 0);

	glEnable(GL_BLEND);
	glBlendFunc(first, second);

	glEnable(GL_TEXTURE_2D);
	glRotatef(sun.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sun.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glBindTexture(GL_TEXTURE_2D, sunTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	//gluSphere(quadric, 5, 36, 18);
	gluSphere(quadric, sun.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glPopMatrix();

	glEnable(GL_LIGHTING);
	GLfloat light_position[] = { 0.0, 0.0, 0.0,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	GLfloat light_diff[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
	GLfloat light_amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);

	


	//Mercury
	glPushMatrix();
	glRotatef(mer.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mer.distance, 0.0, 0.0);

	glPushMatrix();
	
	glRotatef(mer.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mer.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, merTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mer.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();

	
	if (bigOrbitActive == 1) orbitalTrails();
	
	glPushMatrix();
	glColor3f(1, 1, 1); //fix mau
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, staTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_POLYGON);
	glTexCoord2f(-1.0, 0.0); glVertex3f(-200, -200, -100);
	glTexCoord2f(2.0, 0.0); glVertex3f(200, -200, -100);
	glTexCoord2f(2.0, 2.0); glVertex3f(200, 200, -100);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-200, 200, -100);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, staTexture);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-200, -83, 200);
	glTexCoord2f(8.0, 0.0); glVertex3f(200, -83, 200);
	glTexCoord2f(8.0, 8.0); glVertex3f(200, -83, -200);
	glTexCoord2f(0.0, 8.0); glVertex3f(-200, -83, -200);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//Venus
	glPushMatrix();
	glRotatef(ven.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ven.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(ven.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ven.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, venTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ven.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glPopMatrix();

	//Earth, Orbit, Moon
	glPushMatrix();
	glRotatef(ear.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ear.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(ear.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ear.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, earTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	//glColor3f(1, 1, 1);
	gluSphere(quadric, ear.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	if (smallOrbitActive == 1) {
		lun.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		lun.drawMoon();
	}
	glPopMatrix();

	//Mars, Orbits, Moons
	glPushMatrix();
	glRotatef(mar.orbit, 0.0, 1.0, 0.0);
	glTranslatef(mar.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(mar.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(mar.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, marTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, mar.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		pho.drawSmallOrbit();
		dei.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		pho.drawMoon();
		dei.drawMoon();
	}
	glPopMatrix();

	//Jupiter, Orbits, Moons
	glPushMatrix();
	glRotatef(jup.orbit, 0.0, 1.0, 0.0);
	glTranslatef(jup.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(jup.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(jup.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, jupTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, jup.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		eur.drawSmallOrbit();
		gan.drawSmallOrbit();
		cal.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		eur.drawMoon();
		gan.drawMoon();
		cal.drawMoon();
	}
	glPopMatrix();

	//Saturn, Orbit, Moon
	glPushMatrix();
	glRotatef(sat.orbit, 0.0, 1.0, 0.0);
	glTranslatef(sat.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(sat.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(sat.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, satTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, sat.radius, 20.0, 20.0);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glColor3ub(158, 145, 137);
	glRotatef(-63.0, 1.0, 0.0, 0.0);
	glutWireTorus(0.2, 6.0, 30.0, 30.0);
	glutWireTorus(0.4, 5.0, 30.0, 30.0);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		tit.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		tit.drawMoon();
	}
	glPopMatrix();

	glColor3ub(255, 255, 255);		//FIXES SHADING ISSUE

	//Uranus, Orbit, Moon
	glPushMatrix();
	glRotatef(ura.orbit, 0.0, 1.0, 0.0);
	glTranslatef(ura.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(ura.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(ura.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, uraTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, ura.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		puc.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		puc.drawMoon();
	}
	glPopMatrix();

	//Neptune, Orbit, Moon
	glPushMatrix();
	glRotatef(nep.orbit, 0.0, 1.0, 0.0);
	glTranslatef(nep.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(nep.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(nep.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, nepTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, nep.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		tri.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		tri.drawMoon();
	}
	glPopMatrix();

	//Pluto, Orbit, Moon
	glPushMatrix();
	glRotatef(plu.orbit, 0.0, 1.0, 0.0);
	glTranslatef(plu.distance, 0.0, 0.0);

	glPushMatrix();
	glRotatef(plu.axisTilt, 1.0, 0.0, 0.0);
	glRotatef(plu.axisAni, 0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pluTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	gluQuadricTexture(quadric, 1);
	gluSphere(quadric, plu.radius, 20.0, 20.0);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	if (smallOrbitActive == 1) {
		nix.drawSmallOrbit();
	}
	if (moonsActive == 1) {
		nix.drawMoon();
	}
	glPopMatrix();

	DrawStars();

	glutSwapBuffers();             // All drawing commands applied to the 
	// hidden buffer, so now, bring forward
	// the hidden buffer and hide the visible one
}
void drawLogoScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, logTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex3f(-100, -100, -100);
	glTexCoord2f(1.0, 0.0); glVertex3f(100, -100, -100);
	glTexCoord2f(1.0, 1.0); glVertex3f(100, 100, -100);
	glTexCoord2f(0.0, 1.0); glVertex3f(-100, 100, -100);
	glEnd();

	glutSwapBuffers();
}

void drawScenesInOrder(void) {
	if (logoScene == 1) {
		drawLogoScene();
	}
	else {
		Render();
	}
}
//-----------------------------------------------------------

void Resize(int w, int h)
{
	// define the visible area of the window ( in pixels )
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	globalH = h;
	globalW = w;

	// Setup viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, (float)w / (float)h, 1.0, 1000.0);
}

void animate(int n)
{
	if (isAnimate) {
		sun.orbit += sun.orbitSpeed;
		mer.orbit += mer.orbitSpeed;
		ven.orbit += ven.orbitSpeed;
		ear.orbit += ear.orbitSpeed;
		mar.orbit += mar.orbitSpeed;
		jup.orbit += jup.orbitSpeed;
		sat.orbit += sat.orbitSpeed;
		ura.orbit += ura.orbitSpeed;
		nep.orbit += nep.orbitSpeed;
		plu.orbit += plu.orbitSpeed;
		lun.orbit += lun.orbitSpeed;
		pho.orbit += pho.orbitSpeed;
		dei.orbit += dei.orbitSpeed;
		eur.orbit += eur.orbitSpeed;
		gan.orbit += gan.orbitSpeed;
		cal.orbit += cal.orbitSpeed;
		tit.orbit += tit.orbitSpeed;
		nix.orbit += nix.orbitSpeed;
		puc.orbit += puc.orbitSpeed;
		tri.orbit += tri.orbitSpeed;
		if (sun, mer, ven, ear, mar, jup, sat, ura, nep, plu, lun, pho, dei, eur, gan, cal, tit, nix, puc, tri.orbit > 360.0) {
			sun,mer, ven, ear, mar, jup, sat, ura, nep, plu, lun, pho, dei, eur, gan, cal, tit, nix, puc, tri.orbit -= 360.0;
		}
		sun.axisAni += 10.0;
		mer.axisAni += 10.0;
		ven.axisAni += 10.0;
		ear.axisAni += 10.0;
		mar.axisAni += 10.0;
		jup.axisAni += 10.0;
		sat.axisAni += 10.0;
		ura.axisAni += 10.0;
		nep.axisAni += 10.0;
		plu.axisAni += 10.0;
		if (sun,mer, ven, ear, mar, jup, sat, ura, nep, plu.axisAni > 360.0) {
			sun,mer, ven, ear, mar, jup, sat, ura, nep, plu.axisAni -= 360.0;
		}
		glutPostRedisplay();
		glutTimerFunc(30, animate, 1);
	}
}



void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q': exit(0);
		break;
	case 'w': if (isAnimate) worldX -= 1.0f;
		break;
	case 's': if (isAnimate) worldX += 1.0f;
		break;
	case 'a': if (isAnimate) worldY -= 1.0f;
		break;
	case 'd': if (isAnimate)  worldY += 1.0f;
		break;
	case ' ': if (isAnimate) isAnimate = 0;
			else { isAnimate = 1; animate(1); }
			break;
	case 'O': if (bigOrbitActive) bigOrbitActive = 0; else bigOrbitActive = 1; glutPostRedisplay(); break;
	case 'o': if (smallOrbitActive) smallOrbitActive = 0; else smallOrbitActive = 1; glutPostRedisplay(); break;
	case '1': changeCamera = 0; glutPostRedisplay(); break;
	case '2': changeCamera = 1; glutPostRedisplay(); break;
	case 'm': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;
	case 'M': if (moonsActive) moonsActive = 0; else moonsActive = 1; glutPostRedisplay(); break;

	default: break;
	}
	//glutPostRedisplay();

}

void MouseWheel(int button, int dir, int x, int y)
{
	if (isAnimate) {
		if (dir > 0) {
			scaleFactor += 0.1;
			glutPostRedisplay();
		}
		else {
			if (scaleFactor > 0.08) {
				scaleFactor -= 0.1;
				glutPostRedisplay();
			}
		}
	}
}

void mouseControl(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		if (logoScene) logoScene = 0;

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) exit(0);
	glutPostRedisplay();
}


void Setup()  // TOUCH IT !! 
{

	//get random cordinates for the stars
	for (int i = 0; i < 500; i++)
		RandomCoordinates(&stars[i]);
	srand(time(0));
	//Parameter handling

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	// polygon rendering mode
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RandomCoordinates(point* star) //random các ngôi sao xung quanh
{

	int lowest = -1000, highest = 1000;
	int range = (highest - lowest) + 1;
	star->x = lowest + int(range * rand() / (RAND_MAX + 1.0));
	star->y = lowest + int(range * rand() / (RAND_MAX + 1.0));
	star->z = lowest + int(range * rand() / (RAND_MAX + 1.0));

}

void DrawStars() // vẽ các ngôi sao nhỏ
{
	GLUquadricObj* quadric;
	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluDeleteQuadric(quadric);
	for (int i = 0; i < 500; i++) {
		glPushMatrix();
		glTranslatef(stars[i].x, stars[i].y, stars[i].z);
		glColor3f(1, 1, 1);
		gluSphere(quadric, 1.5, 36, 18);
		glPopMatrix();
	}
}

void intructions(void) {
	cout << "Click chuot trai de bat dau" << endl;
	cout << "an SPACE de tiep tuc hoac dung lai" << endl;
	cout << "an Q de ket thuc " << endl;
	cout << "O de hien thi ban kinh quy dao hanh tinh." << endl;
	cout << "o de hien thi ban kinh quy dao ve tinh" << endl;
	cout << "M/m de hien thi hoac tat ngoi sao." << endl;
	cout << "1, 2 de chuyen doi goc nhin." << endl;
	cout << "Lan chuot de zoom" << endl;
}
