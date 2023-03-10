#include <math.h>
#include <GL/glut.h> 
#include <stdio.h>
#include <string.h>
#include <GL/freeglut.h>
#include "visuals.h" // Header file for our OpenGL functions
#include "imageloader.h"
int main(int argc, char* argv[])
{
	intructions();
	// initialize GLUT library state
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);


	// Define the main window size and initial position 

	glutInitWindowSize(800, 800);
	glutInitWindowPosition(500, 0);

	// Create and label the main window
	glutCreateWindow("Solar system");

	// Configure various properties of the OpenGL rendering context
	Setup();

	// Callbacks for the GL and GLUT events:

	// The rendering function 
	glutDisplayFunc(Render);

	glutReshapeFunc(Resize);
	glutIdleFunc(drawScenesInOrder);
	
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(mouseControl);
	glutMouseWheelFunc(MouseWheel);

	load_map();

	//Enter main event handling loop
	glutMainLoop();
	return 0;
}