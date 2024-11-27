/***************************************************
*	Iluminacion.cpp
*
*	Codigo ejemplo para el uso de luces y materiales
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Iluminacion"

#include <iostream>	
#include <codebase.h>

using namespace std;
using namespace cb;


static int xanterior, yanterior;
static float giroxTB = 0, giroyTB = 0, giroxTA = 0, giroyTA = 0;
static float giroxCam = 0, giroyCam = 0;
static bool dragging = false;
static GLubyte esrojo, esverde;
static bool L0on = true, L1on = true;
static GLuint oro, plata;

void init()
{
	// Geometria

	// Luces
	GLfloat luzAmbiente[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT0, GL_SPECULAR, GRISOSCURO);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT1, GL_DIFFUSE, GRISCLARO);
	glLightfv(GL_LIGHT1, GL_SPECULAR, GRISCLARO);
	glEnable(GL_LIGHT1);

	// Materiales
	oro = glGenLists(1);
	glNewList(oro, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BRONCE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ORO);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
	glEndList();

	plata = glGenLists(1);
	glNewList(plata, GL_COMPILE);
	GLfloat plataDifuso[] = { 0.51f, 0.51f, 0.51f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, plataDifuso);
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO);
	glMaterialf(GL_FRONT, GL_SHININESS, 90);
	glEndList();

	// Configuracion del motor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
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

	// Luces solidarias con la camara
	GLfloat posicionL0[] = { 0, 1, 0, 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, posicionL0);

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);

	ejes();

	// Luces fijas
	GLfloat posicionL1[] = { -1, 1, -1, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, posicionL1);

	// Tetera blanca
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	ejes();
	    glShadeModel(GL_SMOOTH);
		glCallList(oro);
		glColor3f(1, 1, 1);
		glutSolidTeapot(0.5);
	glPopMatrix();

	// Tetera amarilla
	glTranslatef(-0.7, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	ejes();
		glShadeModel(GL_FLAT);
		glCallList(plata);
		glColor3f(1, 1, 0);
		glutSolidTeapot(0.5);

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
	case '0':
		L0on = !L0on;
		if (L0on) glEnable(GL_LIGHT0);
		else glDisable(GL_LIGHT0);
		break;
	case '1':
		L1on = !L1on;
		if (L1on) glEnable(GL_LIGHT1);
		else glDisable(GL_LIGHT1);
		break;
	}
	glutPostRedisplay();
}

// Funcion que dibuja los objetos seleccionables de la escena
// en solido y con colores unicos sin mostrarlos en el frontbuffer
void select()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);

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

	glPopAttrib();

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
	else if (estado == GLUT_UP)
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

	if (!dragging) return;

	// Dependiendo del canal de color del pixel seleccionado
	// adivino la tetera que se esta moviendo
	if (esverde == 0xFF) {
		giroyTA += (x - xanterior) * gradosXpixel;
		giroxTA += (y - yanterior) * gradosXpixel;
	}
	else if (esrojo == 0xFF) {
		giroyTB += (x - xanterior) * gradosXpixel;
		giroxTB += (y - yanterior) * gradosXpixel;
	}
	else
	{
		giroyCam += (x - xanterior) * gradosXpixel;
		giroxCam += (y - yanterior) * gradosXpixel;
		// Limitar el giro en x -90,90
		if (fabs(giroxCam) > 89) giroxCam = signo(giroxCam) * 89.0f;
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
