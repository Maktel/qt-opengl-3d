#include "myglwidget.h"

void MyGLWidget::resizeGL() {
  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glViewport(0, 0, this->width(), this->height());
  gluPerspective(60, 1.0 * this->width() / this->height(), 0.01, 100.0);

  // switch back to modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void MyGLWidget::timerEvent(QTimerEvent* e) {
  //    qDebug() << ++number_of_updates;
  updateGL();
}

void MyGLWidget::resizeEvent(QResizeEvent* e) { resizeGL(); }
