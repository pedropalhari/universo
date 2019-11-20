// C program to demonstrate
// drawing a circle using
// OpenGL
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include "BMP.h"

#include "loadTexture.h"

#define pi 3.142857

GLuint moonTex, earthTex, sunTex;
GLUquadric *earth, *moon, *sun;

float xRotated = 90.0, yRotated = 0.0, zRotated = 0.0;

// Posicao da luz
const GLfloat sunPosition[] = {0.0, 0.0, 0.0, 1.0};  // Centro -> Sol

unsigned char moonTextureData[256][256][3];
unsigned char earthTextureData[256][256][3];
unsigned char sunTextureData[256][256][3];

float moon_radius = 0.30, earth_radius = 0.43, sun_radius = 0.9;

struct CameraPos {
  float cameraX;
  float cameraY;
  float cameraZ;

  float directionX;
  float directionY;
  float directionZ;
};

struct CameraPos cameraPos = {.cameraX = 0,
                              .cameraY = 0,
                              .cameraZ = 0,

                              .directionX = 10,
                              .directionY = 10,
                              .directionZ = 10};

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

  // Textura sol
  loadTextureIntoArray(sunTextureData, "sun.bmp");

  // Pega o array de BMP e faz a textura
  glGenTextures(1, &sunTex);
  glBindTexture(GL_TEXTURE_2D, sunTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 255, 255, 0, GL_RGB, GL_UNSIGNED_BYTE,
               (const GLvoid*)sunTextureData);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

struct point {
  float x;
  float y;
  float z;
};

GLfloat no_mat[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_ambient[] = {0.7, 0.7, 0.7, 1.0};
GLfloat mat_ambient_color[] = {0.8, 0.8, 0.2, 1.0};
GLfloat mat_diffuse[] = {1, 1, 1, 1.0};
GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

void drawSphere(GLUquadric* object, GLuint textureCode, float radius,
                float rotation = 0, point Position = {x : 0, y : 0, z : 0},
                bool isSun = false) {
  glTranslatef(Position.x, Position.y, Position.z);
  glColor3f(2.0, 1.0, 1.0);

  if (isSun) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
    glMaterialfv(
        GL_FRONT, GL_DIFFUSE,
        mat_diffuse);  // Cor que eu jogo difusamente para os outros objs
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_EMISSION,
                 mat_emission);  // Propriedade que indica que ele emite
  } else {
    // Se for planeta normal
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
  }

  gluQuadricDrawStyle(object, GLU_FILL);
  glBindTexture(GL_TEXTURE_2D, textureCode);
  glRotatef(rotation, 0, 1, 1);
  gluQuadricTexture(object, GL_TRUE);
  gluQuadricNormals(object, GLU_SMOOTH);
  gluSphere(object, radius, 32, 16);

  glPopMatrix();
}

void draw(void) {
  glClearColor(0.0, 0.0, 0.0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  gluLookAt(cameraPos.cameraX, cameraPos.cameraY, cameraPos.cameraZ,

            cameraPos.cameraX + cameraPos.directionX,
            cameraPos.cameraY + cameraPos.directionY,
            cameraPos.cameraZ + cameraPos.directionZ, 0, 0.1, 0);

  drawSphere(
      sun, sunTex, sun_radius, 0,
      (point){x : sunPosition[0], y : sunPosition[1], z : sunPosition[2]},
      true);
  glLightfv(GL_LIGHT0, GL_POSITION,
            sunPosition);  // Aplicacao de luz puntiforme

  drawSphere(earth, earthTex, earth_radius, zRotated,
             (point){x : 5, y : 0, z : 0});
  drawSphere(moon, moonTex, moon_radius, zRotated * 90,
             (point){x : 2, y : 0, z : 0});

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
  sun = gluNewQuadric();

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

float angles_x, angles_y;

void motion(int x, int y) {
  static bool wrap = false;

  int dx, dy;

  if (!wrap) {
    int ww = glutGet(GLUT_WINDOW_WIDTH);
    int wh = glutGet(GLUT_WINDOW_HEIGHT);

    dx = x - ww / 2;
    dy = y - wh / 2;

    wrap = true;
    glutWarpPointer(ww / 2, wh / 2);
  } else {
    wrap = false;
    return;
  }

  std::cout << "(" << dx << "," << dy << ")"
            << "  (" << x << "," << y << ")\n";

  const float mousespeed = 0.001;

  angles_x += dx * mousespeed;
  angles_y += dy * mousespeed;

  cameraPos.directionX = -sin(angles_x) * cos(angles_y);
  cameraPos.directionY = sin(angles_y);
  cameraPos.directionZ = cos(angles_x) * cos(angles_y);
}

void MyKeyboardFunc(unsigned char key, int x, int y) {
  if (key == 'd') cameraPos.cameraX += 0.1;

  if (key == 's') cameraPos.cameraY += 0.1;

  if (key == 'a') cameraPos.cameraX -= 0.1;

  if (key == 'w') cameraPos.cameraY -= 0.1;

  if (key == 'q') cameraPos.cameraZ += 0.1;

  if (key == 'e') cameraPos.cameraZ -= 0.1;
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

  glutSetCursor(GLUT_CURSOR_NONE);
  glutPassiveMotionFunc(motion);
  glutKeyboardFunc(MyKeyboardFunc);

  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glutMainLoop();
}
