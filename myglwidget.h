#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <QApplication>
#include <QDebug>
#include <QGLWidget>
#include <QKeyEvent>
#include <QTimer>

typedef GLfloat vec3f[3];

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
  void mouseMoveEvent(QMouseEvent* e);
  void mousePressEvent(QMouseEvent* e);
  void resizeEvent(QResizeEvent* e);

 private:
  // mouse position for camera movement
  QPointF mouse_pos_;

  // function called when camera ought to move
  void moveViewmodel(float speed, int direction);

  // rotation helper functions
  void rotateY(int direction);
  void rotateX(int direction);

  // rotation and camera movement members
  float pos_x_ = 0;
  float pos_y_ = 0;
  float pos_z_ = 0;
  float rotation_x_ = 0;
  float rotation_y_ = 0;

  // configuration constants
  const float FORWARD_SPEED = 0.1;
  const int FORWARD = 1;
  const float SIDEWAYS_SPEED = 0.15;
  const int SIDEWAYS = 0;
  const float ROTATION_ANGLE_Y = 5.0;
  const float ROTATION_ANGLE_X = 5.0;

  // enum-like camera movement constants -- do not change
  const int ROTATION_LEFT = -1;
  const int ROTATION_UP = -1;
  const int ROTATION_RIGHT = 1;
  const int ROTATION_DOWN = 1;


  // drawing functions
  void drawCube();
  void drawPlanes();


  // image scene generation is based on
  QImage terrain_map_;
  const QString map_path_{
      "/home/maktel/Dropbox/Studia/programming/QtProjects/qtOpengl/"
      "terrain_map_1.jpg"};
  int terrain_height_;
  int terrain_width_;

  GLuint* vertices_buffer_;
  GLuint* colors_buffer_;
  GLuint* indices_buffer_;
  GLuint* normals_buffer_;
  int buffer_size_;

  void generateTerrain();
  // terrain generation constants
  const float TERRAIN_SPREAD = 50;
  const float HEIGHT_SPREAD = 2;
  const float SEA_LEVEL = 0;

  void drawTerrain();

  // lighting
  const GLfloat LIGHT_STEPS = 10;
  void calcNormal(vec3f p1, vec3f p2, vec3f p3, vec3f out);
  void calcVersor(vec3f vec);
  GLfloat light_pos_[4]{0, 0, 0, 0};
  int light_dir_ = 1;
  void moveLightPosition();
};

#endif  // MYGLWIDGET_H
