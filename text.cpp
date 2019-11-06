// C program to demonstrate
// drawing a circle using
// OpenGL
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "BMP.h"

#include "loadTexture.h"

#define pi 3.142857

GLuint moonTex, earthTex;
GLUquadric *earth, *moon;

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;

unsigned char moonTextureData[256][256][3];
unsigned char earthTextureData[256][256][3];

// https://solarianprogrammer.com/2018/11/19/cpp-reading-writing-bmp-images/

void make_tex(void) {
  // Textura lua
  loadTextureIntoArray(moonTextureData, "moon.bmp");

  // Pega o array de BMP e faz a textura
  glGenTextures(1, &moonTex);
  glBindTexture(GL_TEXTURE_2D, moonTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 255, 255, 0, GL_RGB, GL_UNSIGNED_BYTE,
               (const GLvoid*)moonTextureData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Textura terra
  loadTextureIntoArray(earthTextureData, "terra.bmp");

  // Pega o array de BMP e faz a textura
  glGenTextures(1, &earthTex);
  glBindTexture(GL_TEXTURE_2D, earthTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 255, 255, 0, GL_RGB, GL_UNSIGNED_BYTE,
               (const GLvoid*)earthTextureData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

struct point {
  float x;
  float y;
  float z;
};

void drawSphere(GLUquadric* object, GLuint textureCode, float rotation = 0,
                point Position = {x : 0, y : 0, z : 0}) {
  glTranslatef(Position.x, Position.y, Position.z);
  glColor3f(2.0, 1.0, 1.0);
  gluQuadricDrawStyle(object, GLU_FILL);
  glBindTexture(GL_TEXTURE_2D, textureCode);
  glRotatef(rotation, 0, 1, 1);
  gluQuadricTexture(object, GL_TRUE);
  gluQuadricNormals(object, GLU_SMOOTH);
  gluSphere(object, 1.0, 32, 16);
  glPopMatrix();
}

void draw(void) {
  glClearColor(0.5, 0.5, 1.0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  drawSphere(earth, earthTex, zRotated, (point){x : 0, y : 0, z : -2});
  drawSphere(moon, moonTex, zRotated * 90, (point){x : 10, y : 0, z : -2});

  glutSwapBuffers();
}

void resize(int w, int h) {
  if (!h) h = 1;
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.0, 1.0 * w / h, 0.1, 100.0);
}

void init(void) {
  glEnable(GL_DEPTH_TEST);

  make_tex();

  earth = gluNewQuadric();
  moon = gluNewQuadric();
  glEnable(GL_TEXTURE_2D);
}

void idleFunc(void) {
  zRotated += 0.3;
  glutPostRedisplay();
}

unsigned char* readBMP(char* filename) {
  int i;
  FILE* f = fopen(filename, "rb");
  unsigned char info[54];
  fread(info, sizeof(unsigned char), 54, f);  // read the 54-byte header

  // extract image height and width from header
  int width = *(int*)&info[18];
  int height = *(int*)&info[22];

  int size = 3 * width * height;
  unsigned char* data = new unsigned char[size];  // allocate 3 bytes per pixel
  fread(data, sizeof(unsigned char), size,
        f);  // read the rest of the data at once
  fclose(f);

  for (i = 0; i < size; i += 3) {
    unsigned char tmp = data[i];
    data[i] = data[i + 2];
    data[i + 2] = tmp;
  }

  return data;
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(640, 512);

  glutCreateWindow("Test");

  glutDisplayFunc(draw);
  glutReshapeFunc(resize);
  glutIdleFunc(idleFunc);

  init();

  glutMainLoop();
}
