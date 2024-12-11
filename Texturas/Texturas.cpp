/***************************************************
*	Texturas.cpp
*
*	Codigo ejemplo para el uso de texturas y mezcla
*
*	@author	R.Vivo' <rvivo@upv.es>
*
***************************************************/
#define PROYECTO "Texturas"

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
static GLuint oro, piedra, cemento, cristal;
static GLuint txsuelo, txtetera, txnubes, txventana, txpiedra;

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

	// Texturas
	glGenTextures(1, &txtetera);
	glBindTexture(GL_TEXTURE_2D, txtetera);
	loadImageFile((char*)"oxido.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &txnubes);
	glBindTexture(GL_TEXTURE_2D, txnubes);
	loadImageFile((char*)"nubes.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &txsuelo);
	glBindTexture(GL_TEXTURE_2D, txsuelo);
	loadImageFile((char*)"r.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &txpiedra);
	glBindTexture(GL_TEXTURE_2D, txpiedra);
	loadImageFile((char*)"piedra.jpg");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenTextures(1, &txventana);
	glBindTexture(GL_TEXTURE_2D, txventana);
	loadImageFile((char*)"ventana.png");
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Materiales
	oro = glGenLists(1);
	glNewList(oro, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BRONCE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ORO);
	glMaterialf(GL_FRONT, GL_SHININESS, 10);
	glEndList();

	piedra = glGenLists(1);
	glNewList(piedra, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, BLANCO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO);
	glMaterialf(GL_FRONT, GL_SHININESS, 90);
	glBindTexture(GL_TEXTURE_2D, txpiedra);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	// Textura de superposicion por generacion automatica
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	static const GLfloat planoS[] = { 2,0,0,0 };
	static const GLfloat planoT[] = { 0,2,0,0 };
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_S, GL_OBJECT_PLANE, planoS);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, planoT);
	glShadeModel(GL_SMOOTH);
	glEndList();

	cemento = glGenLists(1);
	glNewList(cemento, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, GRISCLARO);
	glMaterialfv(GL_FRONT, GL_SPECULAR, NEGRO);
	glBindTexture(GL_TEXTURE_2D, txsuelo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glEndList();

	cristal = glGenLists(1);
	glNewList(cristal, GL_COMPILE);
	static const GLfloat cristalDiffuse[] = { 1,1,1,0.4 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cristalDiffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, BLANCO);
	glMaterialf(GL_FRONT, GL_SHININESS, 90);
	glBindTexture(GL_TEXTURE_2D, txnubes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
	// Textura de entorno por generacion automatica
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glShadeModel(GL_FLAT);
	glEndList();



	// Configuracion del motor
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);	// Zbuffer
	glEnable(GL_LIGHTING);		// Iluminacion
	glEnable(GL_TEXTURE_2D);    // Textura
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);         // Mezcla
	glEnable(GL_CULL_FACE);     // Culling
	glEnable(GL_NORMALIZE);     // Normales normalizadas
	glFogfv(GL_FOG_COLOR, NEGRO);
	glFogf(GL_FOG_START, 1);
	glFogf(GL_FOG_END, 4.5);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glEnable(GL_FOG);			// Medio
	
}

// Funcion de muestra de objetos solidarios a la camara
void displayHud()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	// Quitar iluminacion y lo que se necesite
	glDisable(GL_LIGHTING);
	/// Cambiar a ortografica
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	/// Reinicializar la modelview
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	/// Ventana
	glBindTexture(GL_TEXTURE_2D,txventana);
	static  GLfloat v0[] = { -1,-1,0 };
	static  GLfloat v1[] = { 1,-1,0 };
	static  GLfloat v2[] = { 1,1,0 };
	static  GLfloat v3[] = { -1,1,0 };
	quadtex(v0, v1, v2, v3, 0, 1, 0, 1);
	/// Recuperar la proyeccion
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	/// Recuperar la modelview
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

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

	// OBJETOS OPACOS PRIMERO --------------------------------
	
	// Suelo
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glCallList(cemento);
	// Se puede hacer con quadtex(v0,v1,v2,v3,smin,smax,tmin,tmax,divx,divy);
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -0.5, 1);
	glTexCoord2f(2, 0);
	glVertex3f(1, -0.5, 1);
	glTexCoord2f(2, 2);
	glVertex3f(1, -0.5, -1);
	glTexCoord2f(0, 2);
	glVertex3f(-1, -0.5, -1);
	glEnd();
	glPopAttrib();

	// Tetera oxidada
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glBindTexture(GL_TEXTURE_2D, txtetera);
	glDisable(GL_CULL_FACE);
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	ejes();
	glShadeModel(GL_SMOOTH);
	glCallList(oro);
	glFrontFace(GL_CW);
	glutSolidTeapot(0.5);
	glPopMatrix();
	glPopAttrib();

	// OBJETOS TRASLUCIDOS DESPUES -------------------------------
	
	// Donut de cristal
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glCallList(cristal);
	//glCallList(piedra);
	glTranslatef(-0.7, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	ejes();
	glutSolidTorus(0.2,0.4,40,40);
	glPopAttrib();

	// Objetos solidarios a la camara

	displayHud();

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
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_FOG);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Posicion y orientacion de la camara
	Vec3 ojo = { 0,0,4 };
	ojo.rotX(rad(giroxCam)).rotY(rad(giroyCam));

	static Vec3 poi = { 0,0,0 };
	static Vec3 up = { 0,1,0 };

	gluLookAt(ojo.x, ojo.y, ojo.z, poi.x, poi.y, poi.z, up.x, up.y, up.z);

	// Tetera - se dibuja toda roja
	glPushMatrix();
	glTranslatef(0.7, 0, 0);
	glRotatef(giroxTB, 1, 0, 0);
	glRotatef(giroyTB, 0, 1, 0);
	glColor3f(1, 0, 0);
	glutSolidTeapot(0.5);
	glPopMatrix();

	// Donut - se dibuja toda verde
	glTranslatef(-0.7, 0, 0);
	glRotatef(giroxTA, 1, 0, 0);
	glRotatef(giroyTA, 0, 1, 0);
	glColor3f(0, 1, 0);
	glutSolidTorus(0.2, 0.4, 40, 40);

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
	FreeImage_Initialise();
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
	FreeImage_DeInitialise();
}
