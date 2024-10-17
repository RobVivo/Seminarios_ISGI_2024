/***************************************************
*	Transformaciones.cpp
*
*	Codigo ejemplo de composicion de transformaciones
*   en OpenGL
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Transformaciones"

#include <iostream>	
#include <codebase.h>

using namespace cb;

GLuint petalo, corola, flor;
static const int npetalos = 12;

// Funcion de inicializacion propia
void init()
{
	// Geometria
	petalo = glGenLists(1);
	glNewList(petalo, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	glScalef(0.3, 1, 0.3);
	glColor3fv(BLANCO);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();
	glPopAttrib();
	glEndList();

	corola = glGenLists(1);
	glNewList(corola, GL_COMPILE);
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	for (int i = 0; i < npetalos; i++) {
		glPushMatrix();
		glRotatef(360.0 / npetalos * i, 0, 0, 1);
		glTranslatef(0, 0.5, 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(petalo);
		glPopMatrix();
	}

	glPushMatrix();
	glColor3f(1, 1, 0);
	glScalef(0.5, 0.5, 0.5);
	glutSolidSphere(1, 20, 20);
	glPopMatrix();

	glPopAttrib();
	glEndList();

	flor = glGenLists(1);
	glNewList(flor, GL_COMPILE);

	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(0, -0.25, 0);
	glScalef(0.02, 0.5, 0.05);
	glutSolidCube(2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.2, 0);
	glScalef(0.25, 0.25, 0.25);
	glCallList(corola);
	glPopMatrix();

	glEndList();

	// Configurar el motor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

// Funcion de atencion al evento de dibujo
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Seleccionar la matriz vista-modelo
	glMatrixMode(GL_MODELVIEW);
	// Cargar en el tope de la pila la identidad
	glLoadIdentity();

	for (int i = 0; i < 50; i++) {
		glPushMatrix();
		glTranslatef(random(-0.9, 0.9), random(-0.5, 0.5), 0);
		glScalef(0.5, 0.5, 0.5);
		glCallList(flor);
		glPopMatrix();
	}
	glFlush();
}

// Funcion de atencion al redimensionamiento
void reshape(GLint w, GLint h)
{
}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	// Inicializacion propia
	init();

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}
