#include "trackball.h"
#include <math.h>
#include <stdlib.h>

	GLfloat vertices[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
	{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

	GLfloat normals[][3] = {{-1.0,-1.0,-1.0},{1.0,-1.0,-1.0},
	{1.0,1.0,-1.0}, {-1.0,1.0,-1.0}, {-1.0,-1.0,1.0}, 
	{1.0,-1.0,1.0}, {1.0,1.0,1.0}, {-1.0,1.0,1.0}};

	GLfloat colors[][3] = {{0.0,0.0,0.0},{1.0,0.0,0.0},
	{1.0,1.0,0.0}, {0.0,1.0,0.0}, {0.0,0.0,1.0}, 
	{1.0,0.0,1.0}, {1.0,1.0,1.0}, {0.0,1.0,1.0}};

	GLfloat mouseX_, mouseY_; //posicao inicial do cursor
	bool rotacionando2 = true;
	GLfloat m2 [] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

void polygon(int a, int b, int c , int d)
{

/* draw a polygon via list of vertices */

 	glBegin(GL_POLYGON);
		glColor3fv(colors[a]);
		//glNormal3fv(normals[a]);
		glVertex3fv(vertices[a]);
		glColor3fv(colors[b]);
		//glNormal3fv(normals[b]);
		glVertex3fv(vertices[b]);
		glColor3fv(colors[c]);
		//glNormal3fv(normals[c]);
		glVertex3fv(vertices[c]);
		glColor3fv(colors[d]);
		//glNormal3fv(normals[d]);
		glVertex3fv(vertices[d]);
	glEnd();
}

void colorcube(void)
{
	/*polygon(0,3,2,1);
	polygon(2,3,7,6);
	polygon(0,4,7,3);
	polygon(1,2,6,5);
	polygon(4,5,6,7);
	polygon(0,1,5,4);*/
	glutWireTeapot(1.0);
}

static GLfloat theta[] = {0.0,0.0,0.0};
static GLint axis = 2;

void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
 colorcube();

 glFlush();
 glutSwapBuffers();
}

void spinCube()
{
	theta[axis] += 0.1;
	if( theta[axis] > 360.0 ) theta[axis] -= 360.0;
	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y)
{
	/*Not in use since we are using a trackball*/
	//if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN) axis = 0;
	//if(btn==GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) axis = 1;
	//if(btn==GLUT_RIGHT_BUTTON && state == GLUT_DOWN) axis = 2;
}

void myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.0, 2.0, -2.0 * (GLfloat) h / (GLfloat) w,
            2.0 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.0 * (GLfloat) w / (GLfloat) h,
            2.0 * (GLfloat) w / (GLfloat) h, -2.0, 2.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouseAndandoCB2(int PX, int PY){
	//mouse = last position
	//P = current position

	printf("px %d", PX);

	if (rotacionando2){
		GLint W = glutGet(GLUT_WINDOW_WIDTH);
		GLint H = glutGet(GLUT_WINDOW_HEIGHT);

		GLfloat raio; //for a while, we don't need the radius square root
		if (W>H) raio=W*W/2; 
		else raio=H*H/2;

		mouseX_ -= W/2;         //
		GLfloat px = PX - W/2;   // uses the screen center as origin
		mouseY_ = H/2 - mouseY_; // 
		GLfloat py = H/2 - PY;   //

		//Finds the Z coordinate of the sphere centered at the origin
		GLfloat mouseX_Y2 = mouseX_*mouseX_ + mouseY_*mouseY_;
		GLfloat pXY2 = px*px+py*py;

		if (mouseX_Y2 < raio && pXY2 < raio){ //this trackball only works for points inside the sphere

			GLfloat mouseZ = sqrt(raio - mouseX_Y2);
			GLfloat pz = sqrt(raio - pXY2); 
			

			GLfloat eixoRotacao[3]; //vector product "mouse X P"
			eixoRotacao[0] = -mouseZ*py + mouseY_*pz;
			eixoRotacao[1] = mouseZ*px - mouseX_*pz;
			eixoRotacao[2] = -mouseY_*px + mouseX_*py;

			GLfloat anguloRotacao = atan(sqrt(eixoRotacao[0]*eixoRotacao[0] + eixoRotacao[1]*eixoRotacao[1] + eixoRotacao[2]*eixoRotacao[2]));

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();
			glRotatef(anguloRotacao,eixoRotacao[0],eixoRotacao[1],eixoRotacao[2]);
			glMultMatrixf(m2);
			glGetFloatv(GL_MODELVIEW_MATRIX, m2);
		}
	} 
	mouseX_ = PX;
	mouseY_ = PY;
	glutPostRedisplay();
}

void main(int argc, char **argv)
{
    glutInit(&argc, argv);

/* need both double buffering and z buffer */

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("colorcube");
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
	  // glutIdleFunc(spinCube);
	  //glutMouseFunc(mouse);
	glutMotionFunc(mouseAndandoCB2);
	glEnable(GL_DEPTH_TEST); /* Enable hidden--surface--removal */
    glutMainLoop();
}
