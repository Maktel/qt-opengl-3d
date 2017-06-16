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
