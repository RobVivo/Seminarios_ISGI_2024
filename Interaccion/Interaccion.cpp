/***************************************************
*	Interaccion.cpp
*
*	Codigo ejemplo para la captura de eventos de usuario
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Interaccion"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;

static enum { ALAMBRICO, SOLIDO, DOBLE} modo;
//static float girox = 0, giroy = 0;
static int xanterior, yanterior;
static float giroxTB = 0, giroyTB = 0, giroxTA = 0, giroyTA = 0;
static float giroxCam = 0, giroyCam = 0;
static bool dragging = false;
static GLubyte esrojo, esverde;

// Funcion de atencion al menu
void onMenu(int opcion)
{
	switch (opcion)
	{
	case 0:
		modo = ALAMBRICO;
		break;
	case 1:
		modo = SOLIDO;
		break;
	case 2:
		modo = DOBLE;
		break;
	}
	glutPostRedisplay();
}

void init()
{
	// Geometria

	// Menu
	glutCreateMenu(onMenu);
	glutAddMenuEntry("Alambrico", 0);
	glutAddMenuEntry("Solido", 1);
	glutAddMenuEntry("Doble", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);


	// Configuracion del motor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

// Funcion de atencion al evento de dibujo
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Posicion y orientacion de la camara
	Vec3 ojo = { 0,0,4 };
	ojo = ojo.rotX(rad(giroxCam));
	ojo = ojo.rotY(rad(giroyCam));

	static Vec3 poi = { 0,0,0 };
	static Vec3 up = { 0,1,0 };

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);

	ejes();

	//glRotatef(girox, 1, 0, 0);
    //glRotatef(giroy, 0, 1, 0);

	// Tetera blanca
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	ejes();
	if(modo != SOLIDO) {
		glColor3f(0.7,0.7,0.7);
	    glutWireTeapot(0.51);
	}
	if(modo != ALAMBRICO) {
		glColor3f(1,1,1);
	    glutSolidTeapot(0.5);
	}
	glPopMatrix();

	// Tetera amarilla
	glTranslatef(-0.7, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	ejes();
	if (modo != SOLIDO) {
		glColor3f(0.7, 0.7, 0);
		glutWireTeapot(0.51);
	}
	if (modo != ALAMBRICO) {
		glColor3f(1, 1, 0);
		glutSolidTeapot(0.5);
	}

	glutSwapBuffers();
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

// Funcion de atencion al evento de teclado
void onKey(unsigned char tecla, int x, int y)
{
	switch (tecla)
	{
	case 27: // Escape
		exit(0);
		break;
	case 'a':
		modo = ALAMBRICO;
		break;
	case 's':
		modo = SOLIDO;
		break;
	case 'd':
		modo = DOBLE;
		break;
	}
	glutPostRedisplay();
}

// Funcion que dibuja los objetos seleccionables de la escena
// en solido y con colores unicos sin mostrarlos en el frontbuffer
void select()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Posicion y orientacion de la camara
	Vec3 ojo = { 0,0,4 };
	ojo.rotX(rad(giroxCam)).rotY(rad(giroyCam));

	static Vec3 poi = { 0,0,0 };
	static Vec3 up = { 0,1,0 };

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);

	// Tetera blanca - se dibuja toda roja
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	glColor3f(1, 0, 0);
	glutSolidTeapot(0.5);
	glPopMatrix();

	// Tetera amarilla - se dibuja toda verde
	glTranslatef(-0.7, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(0, 1, 0);
	glutSolidTeapot(0.5);

}

// Funcion de atencion al evento de raton
void onClick(int boton, int estado, int x, int y)
{
	if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN)
	{
		xanterior = x;
		yanterior = y;
		dragging = true;
	}
	else if(estado == GLUT_UP)
	{
		dragging = false;
	}

	// ¿que mide el viewport?
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	// cambio a left-bottom
	int yflip = viewport[3] - y;

	// dibujar en el backbuffer
	select();

	// leer el pixel x,yflip
	glReadPixels(x, yflip, 1, 1, GL_RED, GL_UNSIGNED_BYTE, &esrojo);
	glReadPixels(x, yflip, 1, 1, GL_GREEN, GL_UNSIGNED_BYTE, &esverde);
}

// Funcion de atencion al evento de arrastre
void onDrag(int x, int y)
{
	static const float gradosXpixel = 0.5f;

	if(!dragging) return;

	// Dependiendo del canal de color del pixel seleccionado
	// adivino la tetera que se esta moviendo
	if(esverde == 0xFF) {
		giroyTA += (x - xanterior) * gradosXpixel;
		giroxTA += (y - yanterior) * gradosXpixel;
	}
	else if(esrojo==0xFF) {
		giroyTB += (x - xanterior) * gradosXpixel;
		giroxTB += (y - yanterior) * gradosXpixel;
	}
	else
	{
		giroyCam += (x - xanterior) * gradosXpixel;
		giroxCam += (y - yanterior) * gradosXpixel;
		// Limitar el giro en x -90,90
		if( fabs(giroxCam) > 89 ) giroxCam = signo(giroxCam)*89.0f;
	}

	/*
	// Al mover el cursor hacia la derecha la x aumenta y el giro
	// es en torno al eje y positivo
	giroy += (x - xanterior) * gradosXpixel;
	// Al mover el cursor hacia abajo la y aumenta y el giro
	// es en torno al eje x positivo
	girox += (y - yanterior) * gradosXpixel;
	*/

	xanterior = x;
	yanterior = y;
		
	glutPostRedisplay();
	
}




int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	init();

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(onKey);
	glutMouseFunc(onClick);
	glutMotionFunc(onDrag);

	// Bucle de atencion a eventos
	glutMainLoop();
}
