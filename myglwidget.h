#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <GL/glu.h>

#include <QtOpenGL>
#include <QtWidgets>

class MyGLWidget : public QGLWidget {
  Q_OBJECT

 public:
  MyGLWidget(QGLWidget* parent = 0);
  ~MyGLWidget();

 protected:
  void initializeGL();
  void paintGL();
  void resizeGL();

  void timerEvent(QTimerEvent* e);
  void keyPressEvent(QKeyEvent* e);

 private:
  float pos_x_ = 0;
  float pos_y_ = 0;
  float pos_z_ = 0;
  float rotation_x_ = 0;
  float rotation_y_ = 0;

  const float FORWARD_SPEED = 0.3;
  const int FORWARD = 1;
  const float SIDEWAYS_SPEED = 0.15;
  const int SIDEWAYS = 0;

  const float ROTATION_ANGLE_Y = 10.0;
  const int ROTATION_LEFT = -1;
  const int ROTATION_RIGHT = 1;

  long long number_of_updates = 0;
  int variable = 0;

  void drawCubeWalls();
  void moveViewmodel(float speed, int direction);
  void rotateY(int direction);
};

#endif  // MYGLWIDGET_H
