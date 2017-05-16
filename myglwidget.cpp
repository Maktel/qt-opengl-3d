#include "myglwidget.h"

MyGLWidget::MyGLWidget(QGLWidget* parent) : QGLWidget(parent) {}

MyGLWidget::~MyGLWidget() {}

void MyGLWidget::initializeGL() {
  // set background (void) color
  glClearColor(0.5, 0.0, 0.5, 1);

  // change observer
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // default viewport seems to be too small (100, 100) ?
  // if window size was not a square, image would be disproportional
  glViewport(0, 0, this->width(), this->height());
  gluPerspective(60, 1.0 * this->width() / this->height(), 0.01, 100.0);

  // switch back to modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  qDebug() << doubleBuffer();  // should be enabled by default
  //  startTimer(0);               // simulates glutIdleFunc() to animate scene
}

void MyGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glRotatef(rotation_y_, 0.0, 1.0, 0.0);
  glTranslatef(-pos_x_, -pos_y_, pos_z_);

  glPushMatrix();

  glTranslatef(0, 0, -5);
  glBegin(GL_QUADS);
  drawCubeWalls();
  glEnd();

  glPopMatrix();
  swapBuffers();  // maybe unnecessary
  glFlush();
}

void MyGLWidget::resizeGL() {}

void MyGLWidget::timerEvent(QTimerEvent* e) {
  //    qDebug() << ++number_of_updates;
  updateGL();
}

void MyGLWidget::keyPressEvent(QKeyEvent* e) {
  qDebug() << char(e->key());

  switch (e->key()) {
    case Qt::Key_W: {
      moveViewmodel(FORWARD_SPEED, FORWARD);
    } break;

    case Qt::Key_S: {
      moveViewmodel(-FORWARD_SPEED, FORWARD);
    } break;

    case Qt::Key_A: {
      moveViewmodel(SIDEWAYS_SPEED, SIDEWAYS);
    } break;

    case Qt::Key_D: {
      moveViewmodel(-SIDEWAYS_SPEED, SIDEWAYS);
    } break;

    case Qt::Key_Q: {
      rotateY(ROTATION_LEFT);
    } break;

    case Qt::Key_E: {
      rotateY(ROTATION_RIGHT);
    }
  }

  updateGL();
}

void MyGLWidget::moveViewmodel(float speed, int direction) {
  float radians = (rotation_y_ + 90 * direction) * M_PI / 180;
  pos_z_ += speed * sin(radians);
  pos_x_ -= speed * cos(radians);
  //  if (direction != 0) {
  //    pos_y_ -= speed * sin(rotation_x_ * M_PI / 180);
  //  }
  qDebug() << pos_x_ << pos_y_ << pos_z_;
}

void MyGLWidget::rotateY(int direction) {
  rotation_y_ += direction * ROTATION_ANGLE_Y;
}


void MyGLWidget::drawCubeWalls() {
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
}
