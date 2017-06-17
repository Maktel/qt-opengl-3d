#include "myglwidget.h"

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

    case Qt::Key_L: {
      moveLightPosition();
    } break;
  }

  updateGL();
}

void MyGLWidget::mouseMoveEvent(QMouseEvent* e) {
  if (e->buttons() & Qt::LeftButton) {
    rotation_x_ += (e->localPos().y() - mouse_pos_.y()) / 2;
    rotation_y_ += (e->localPos().x() - mouse_pos_.x()) / 2;

    mouse_pos_ = e->localPos();
    qDebug() << "rotation x y" << rotation_x_ << rotation_y_;

    updateGL();
  }
}

void MyGLWidget::mousePressEvent(QMouseEvent* e) {
  if (e->button() == Qt::LeftButton) {
    mouse_pos_ = e->localPos();
  }
}

void MyGLWidget::moveViewmodel(float speed, int direction) {
  float radians = (rotation_y_ + 90 * direction) * M_PI / 180;
  pos_z_ += speed * sin(radians);
  pos_x_ -= speed * cos(radians);
  if (direction != 0) {
    pos_y_ -= speed * sin(rotation_x_ * M_PI / 180);
  }
  qDebug() << pos_x_ << pos_y_ << pos_z_;
}

void MyGLWidget::rotateY(int direction) {
  rotation_y_ += direction * ROTATION_ANGLE_Y;
}

void MyGLWidget::rotateX(int direction) {
  rotation_x_ += direction * ROTATION_ANGLE_X;
}

void MyGLWidget::moveLightPosition() {
  light_pos_[0] = ((terrain_width_ - 1) / TERRAIN_SPREAD) / 3;
  light_pos_[1] = -10.0;

  const GLfloat light_step =
      ((terrain_height_ - 1) / TERRAIN_SPREAD) / LIGHT_STEPS;
  light_pos_[2] += light_dir_ * light_step;

  if (light_pos_[2] < -1 * (terrain_height_ - 1) / TERRAIN_SPREAD ||
      light_pos_[2] > 2 * (terrain_height_ - 1) / TERRAIN_SPREAD) {
    light_dir_ = -light_dir_;
  }

  qDebug() << "light pos: " << light_pos_[0] << light_pos_[1] << light_pos_[2];

  // glLightfv with GL_POSITION accepts 4 element array
  glLightfv(GL_LIGHT0, GL_POSITION, light_pos_);
}

void MyGLWidget::setLighting() {
  float light0_ambient[] = {0.3, 0.3, 0.3};
  float light0_diffuse[] = {0.8, 0.8, 0.8};
  float light0_specular[] = {0.9, 0.9, 0.9};
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);


  float light1_ambient[] = {1.0, 1.0, 1.0, 1.0};
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
}
