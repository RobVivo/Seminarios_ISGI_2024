/***************************************************
*	Geometria.cpp
*
*	Ejemplo de geometria predefinida y propia en
*   glut y opengl
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Geometria"

#include <iostream>	
#include <codebase.h>
#include <vector>

using namespace cb;
static GLuint rueda;

//Funcion de muestreo de una circunferencia
vector<Vec3> puntosCircunferencia(float radio, int numPuntos, float desfase = 0)
{
	vector<Vec3> puntos;
	for (float angulo = desfase; angulo < 2 * PI + desfase; angulo += 2 * PI / numPuntos)
		puntos.push_back(Vec3(radio * cos(angulo), radio * sin(angulo), 0));
	return puntos;
}

void init()
{
	// Objeto rueda
	static const float radio = 1;
	static const int nPuntos = 16;
	static const float desfase = PI / 2;
	vector<Vec3> pExterior = puntosCircunferencia(radio, nPuntos, desfase);
	vector<Vec3> pInterior = puntosCircunferencia(radio/2, nPuntos, desfase);

	rueda = glGenLists(1);
	glNewList(rueda, GL_COMPILE);

	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 0; i <= nPuntos; i++) {
		glVertex3fv(pInterior[i % nPuntos]);
		glVertex3fv(pExterior[i % nPuntos]);
	}

	glEnd();
	glEndList();
}

// Funcion de atencion al evento de dibujo
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Formas preconstruidas en glut
	glColor3f(0, 0, 1);
	glutSolidSphere(0.49, 20, 20);
	glColor3f(1, 1, 1);
	glutWireSphere(0.5,20,20);
	*/

	/* Cuadrado como primitiva POLYGON
	glColor3f(1, 0, 0);
	glLineWidth(4);
	glPointSize(10);
	glPolygonMode(GL_FRONT, GL_FILL);
	//Un cuadrado de lado 1
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex3f(-0.5, -0.5, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0.5, -0.5, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0.5, 0.5, 0);
	glColor3f(1, 1, 0);
	glVertex3f(-0.5, 0.5, 0);
	glEnd();
	*/

	// Uso de display lists
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3fv(ROJO);
	glCallList(rueda);

	glPolygonMode(GL_FRONT, GL_LINE);
	glColor3fv(BLANCO);
	glLineWidth(4);
	glCallList(rueda);


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

	// Funcion de inicializacion propia
	init();

	// Registrar callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);

	// Bucle de atencion a eventos
	glutMainLoop();
}
