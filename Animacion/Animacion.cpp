/***************************************************
*	Animacion.cpp
*
*	Codigo ejemplo para manejo del tiempo y animacion
*   en OpenGL con GL
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Camara"

#include <iostream>	
#include <codebase.h>
#include <sstream>

using namespace std;
using namespace cb;

// Variables globales
static GLuint tetera;
static float radioCamara = 5;
static const int FPS = 60;

// Variables dependientes del tiempo
static float gradosTeteras = 0;
static float gradosCamara = 0;
static Vec3 ojo = { 5,0,0 };


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

// Funcion de cálculo de los FPS y muestra
void muestraFPS()
{
	static int tiempoAnterior = glutGet(GLUT_ELAPSED_TIME);;
	static int frames = 0;

	int tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	frames++;

	if (tiempoActual - tiempoAnterior > 1000)
	{
		stringstream ss;
		ss << "FPS: " << frames;
		glutSetWindowTitle(ss.str().c_str());
		frames = 0;
		tiempoAnterior = tiempoActual;
	}
}

// Funcion de atencion al evento de dibujo
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Posicion y orientacion de la camara

	static Vec3 poi = { 0,0,0 };
	static Vec3 up = { 0,1,0 };

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);

	//glRotatef(30, 1, 0, 0);
	//glRotatef(-30, 0, 1, 0);

	ejes();

	glPushMatrix();
	glRotatef(gradosTeteras, 0, 1, 0);
	glColor3fv(AZUL);
	glCallList(tetera);
	glPopMatrix();

	glColor3fv(VERDE);
	glTranslatef(0, -0.5, 0);
	glRotatef(gradosTeteras/2, 0, 1, 0);
	glCallList(tetera);

	glutSwapBuffers();

	muestraFPS();
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

// Funcion de actualizacion de la escena
void update()
{
	// Actualizar la posicion de las teteras sin control de tiempo
	// gradosTeteras += 0.1;

	// Actualizar controlando el tiempo transcurrido
	static const float velAngTeteras = 360; // grados por segundo
	static const float velAngCamara = 24; // grados por segundo

	static int tiempoAnterior = glutGet(GLUT_ELAPSED_TIME);

	int tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	float dt = (tiempoActual - tiempoAnterior) / 1000.0;

	gradosTeteras += velAngTeteras * dt;
	gradosCamara += velAngCamara * dt;

	ojo.x = radioCamara * cos(rad(gradosCamara));
	ojo.z = radioCamara * sin(rad(-gradosCamara));  // - para que gire en sentido antihorario (positivo)

	//--- forma alternativa de mover el ojo alrededor de cualquier eje que pase por el origen
	// float deltaAngCamara = velAngCamara * dt;
	// ojo.rotate(rad(deltaAngCamara),Vec3(0,1,1));
	//--- 

	tiempoAnterior = tiempoActual;

	glutPostRedisplay();
}

// Funcion de atencion al evento de cuenta atras
// Genera eventos periodicos al encolarse a si misma
void onTimer(int value)
{
	glutTimerFunc(value, onTimer, value);
	update();
}

int main(int argc, char** argv)
{
	// Inicializaciones
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(50, 50);

	// Crear ventana
	glutCreateWindow(PROYECTO);

	init();

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(update);			// Sin control de disparo
	glutTimerFunc(1000/FPS, onTimer, 1000/FPS);	// Con control de disparo

	// Bucle de atencion a eventos
	glutMainLoop();
}
