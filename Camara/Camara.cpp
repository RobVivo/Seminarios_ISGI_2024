/***************************************************
*	Camara.cpp
*
*	Codigo ejemplo para el uso y manejo de la camara
*   en OpenGL
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Camara"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

static GLuint tetera;

void init()
{
	// Geometria
	tetera = glGenLists(1);
	glNewList(tetera, GL_COMPILE);
	glutSolidTeapot(0.5);
	glPushAttrib(GL_CURRENT_BIT);
	glColor3fv(BLANCO);
	glutWireTeapot(0.51);
	glPopAttrib();
	glEndList();

	// Configuracion del motor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

// Funcion de atencion al evento de dibujo
void display()
{

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Posicion y orientacion de la camara
	static Vec3 ojo = { 1,2,3 };
	static Vec3 poi = { 0,0,0 };
	static Vec3 up = { 0,1,0 };

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);

	//glRotatef(30, 1, 0, 0);
	//glRotatef(-30, 0, 1, 0);

	ejes();

	glColor3fv(AZUL);
	glCallList(tetera);

	glColor3fv(VERDE);
	glTranslatef(0, -0.5, 0);
	glCallList(tetera);

	glFlush();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
	glViewport(0, 0, w, h);

	float ra = (float)w / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/*
	// Camara ortografica con isometria
	if(w>h)
		glOrtho(-1*ra, 1*ra, -1, 1, -1, 1);
	else
		glOrtho(-1, 1, -1/ra, 1/ra, -1, 1);
	*/

	// Camara perspectiva con isometria
	gluPerspective(30, ra, 0.1, 10);
}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	init();

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}
