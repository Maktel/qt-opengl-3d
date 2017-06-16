#include "myglwidget.h"

void MyGLWidget::drawCube() {
  glBegin(GL_QUADS);

  glColor3f(1.0, 1.0, 1.0);
  glVertex3f(0.5, -0.5, 0.5);
  glVertex3f(0.5, 0.5, 0.5);
  glVertex3f(-0.5, 0.5, 0.5);
  glVertex3f(-0.5, -0.5, 0.5);

  glColor3f(1.0, 0.0, 1.0);
  glVertex3f(0.5, -0.5, -0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glVertex3f(0.5, 0.5, 0.5);
  glVertex3f(0.5, -0.5, 0.5);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(-0.5, -0.5, 0.5);
  glVertex3f(-0.5, 0.5, 0.5);
  glVertex3f(-0.5, 0.5, -0.5);
  glVertex3f(-0.5, -0.5, -0.5);

  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.5, 0.5, 0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glVertex3f(-0.5, 0.5, -0.5);
  glVertex3f(-0.5, 0.5, 0.5);

  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(0.5, -0.5, -0.5);
  glVertex3f(0.5, -0.5, 0.5);
  glVertex3f(-0.5, -0.5, 0.5);
  glVertex3f(-0.5, -0.5, -0.5);

  glColor3f(1.0, 1.0, 0.0);
  glVertex3f(0.5, -0.5, -0.5);
  glVertex3f(0.5, 0.5, -0.5);
  glVertex3f(-0.5, 0.5, -0.5);
  glVertex3f(-0.5, -0.5, -0.5);

  glEnd();
}

void MyGLWidget::drawPlanes() {
  glBegin(GL_LINES);

  //  glColor3f(1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX,
  //            1.0 * rand() / RAND_MAX);
  glColor3f(0.0, 1.0, 1.0);
  for (int i = -25; i < 25; ++i) {
    glVertex3f(1.0 * i, -50, 0);
    glVertex3f(1.0 * i, 50, 0);
  }

  //  glColor3f(1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX,
  //            1.0 * rand() / RAND_MAX);
  glColor3f(1.0, 0.0, 1.0);
  for (int i = -25; i < 25; ++i) {
    glVertex3f(0, 1.0 * i, -50);
    glVertex3f(0, 1.0 * i, 50);
  }

  //  glColor3f(1.0 * rand() / RAND_MAX, 1.0 * rand() / RAND_MAX,
  //            1.0 * rand() / RAND_MAX);
  glColor3f(1.0, 1.0, 0.0);
  for (int i = -25; i < 25; ++i) {
    glVertex3f(-50, 0, 1.0 * i);
    glVertex3f(50, 0, 1.0 * i);
  }

  glEnd();
}
