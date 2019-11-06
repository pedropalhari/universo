// C program to demonstrate
// drawing a circle using
// OpenGL
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "BMP.h"
#define pi 3.142857

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;

//------------------------------  reshapeFunc  ---------------------------------

void reshapeFunc(int x, int y) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(40.0, (GLdouble)x / (GLdouble)y, 0.5, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glViewport(0, 0, x, y);
}

//------------------------------  Draw_Spheres   -------------------------------

void Draw_Spheres(void) {
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -15.0);

  glColor3f(0.8, 0.2, 0.1);  // Red ball displaced to left.
  glPushMatrix();
  glTranslatef(-1.5, 0.0, 0.0);
  glRotatef(60.0, 1, 0, 0);
  glRotatef(zRotated * 2.0, 0, 0,
            1);  // Red ball rotates at twice the rate of blue ball.
  glutSolidSphere(1.0, 20, 50);
  glPopMatrix();

  glColor3f(0.1, 0.2, 0.8);  // Blue ball displaced to right.
  glPushMatrix();
  glTranslatef(1.5, 0.0, 0.0);
  glRotatef(60.0, 1, 0, 0);
  glRotatef(zRotated, 0, 0, 1);
  glutSolidSphere(1.0, 20, 50);
  glPopMatrix();

  glutSwapBuffers();
}

//--------------------------------  idleFunc  ----------------------------------

void idleFunc(void) {
  zRotated += 0.3;
  glutPostRedisplay();
}

//----------------------------------  main  ------------------------------------

void main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(800, 700);
  glutInitWindowPosition(700, 200);
  glutCreateWindow("Sphere Rotating Animation");
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glClearColor(0.0, 0.0, 0.0, 0.0);

  glutDisplayFunc(Draw_Spheres);
  glutReshapeFunc(reshapeFunc);
  glutIdleFunc(idleFunc);

  glutMainLoop();
}