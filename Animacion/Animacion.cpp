/***************************************************
*	Animacion.cpp
*
*	Codigo ejemplo para animacion en OpenGL con GLUT
*
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Animacion"

#include <iostream>	
#include <codebase.h>
#include <sstream>
#include <codebase.h>

using namespace std;
using namespace cb;

// Variables globales
static GLuint tetera;
static const int FPS = 40;

// Variables dependientes del tiempo
static float gradosTetera = 0;
static float gradosCamara = 0;
static float radioCamara = 5;
static Vec3 ojo = { 5,0,0 };


void init()
{
	cout << "GL version " << glGetString(GL_VERSION) << endl;

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

// Funcion de muestra de FPS
void muestraFPS()
{
	static int tiempoAnterior = glutGet(GLUT_ELAPSED_TIME);;
	static int frames = 0;
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempoTranscurrido = (ahora - antes);

	int tiempoActual = glutGet(GLUT_ELAPSED_TIME);
	frames++;
	if (tiempoTranscurrido > 1000)
	{
		stringstream titulo;
		titulo << PROYECTO << " (FPS: " << frames << ")";
		glutSetWindowTitle(titulo.str().c_str());
		frames = 0;
		antes = ahora;
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
	glRotatef(gradosTetera, 0, 1, 0);
	glColor3fv(AZUL);
	glCallList(tetera);
	glPopMatrix();

	glColor3fv(VERDE);
	glTranslatef(0, -0.5, 0);
	glRotatef(gradosTetera / 2, 0, 1, 0);
	glCallList(tetera);

	glutSwapBuffers();

	muestraFPS();

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

// Funcion de actualizacion
void update()
{
	// Sin coherencia temporal
	// gradosTetera += 1;

	// Con coherencia temporal
	static const float velAngTetera = 360; // grados por segundo
	static const float velAngCamara = 24; // grados por segundo

	// Calculo del tiempo transcurrido
	static int antes = glutGet(GLUT_ELAPSED_TIME);
	int ahora = glutGet(GLUT_ELAPSED_TIME);
	float tiempoTranscurrido = (ahora - antes) / 1000.0f;

	// Actualizacion de variables dependientes del tiempo
	gradosTetera += velAngTetera * tiempoTranscurrido;
	gradosCamara += velAngCamara * tiempoTranscurrido;
	ojo = Vec3(radioCamara * cos(rad(gradosCamara)),
		0,
		-radioCamara * sin(rad(gradosCamara)));

	// Actualizacion de la marca de tiempo
	antes = ahora;

	glutPostRedisplay();    // Encola un evento de redibujo
}

void onTimer(int tiempo)
{
	// Generar un evento periodico cada 'tiempo' milisegundos
	glutTimerFunc(tiempo, onTimer, tiempo);

	// Actualizar la escena
	update();
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
	// glutIdleFunc(update);
	glutTimerFunc(1000 / FPS, onTimer, 1000 / FPS);

	// Bucle de atencion a eventos
	glutMainLoop();
}
