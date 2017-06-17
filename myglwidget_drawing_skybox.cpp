#include "myglwidget.h"

void MyGLWidget::prepareSkybox() {}

void MyGLWidget::drawCube() {
  glBegin(GL_QUADS);


  glColor3f(1.0, 1.0, 1.0);
  glVertex3f(sb_scl_, -sb_scl_, sb_scl_);
  glVertex3f(sb_scl_, sb_scl_, sb_scl_);
  glVertex3f(-sb_scl_, sb_scl_, sb_scl_);
  glVertex3f(-sb_scl_, -sb_scl_, sb_scl_);

  glVertex3f(sb_scl_, -sb_scl_, -sb_scl_);
  glVertex3f(sb_scl_, sb_scl_, -sb_scl_);
  glVertex3f(sb_scl_, sb_scl_, sb_scl_);
  glVertex3f(sb_scl_, -sb_scl_, sb_scl_);

  glVertex3f(-sb_scl_, -sb_scl_, sb_scl_);
  glVertex3f(-sb_scl_, sb_scl_, sb_scl_);
  glVertex3f(-sb_scl_, sb_scl_, -sb_scl_);
  glVertex3f(-sb_scl_, -sb_scl_, -sb_scl_);

  glVertex3f(sb_scl_, sb_scl_, sb_scl_);
  glVertex3f(sb_scl_, sb_scl_, -sb_scl_);
  glVertex3f(-sb_scl_, sb_scl_, -sb_scl_);
  glVertex3f(-sb_scl_, sb_scl_, sb_scl_);

  glVertex3f(sb_scl_, -sb_scl_, -sb_scl_);
  glVertex3f(sb_scl_, -sb_scl_, sb_scl_);
  glVertex3f(-sb_scl_, -sb_scl_, sb_scl_);
  glVertex3f(-sb_scl_, -sb_scl_, -sb_scl_);

  glVertex3f(sb_scl_, -sb_scl_, -sb_scl_);
  glVertex3f(sb_scl_, sb_scl_, -sb_scl_);
  glVertex3f(-sb_scl_, sb_scl_, -sb_scl_);
  glVertex3f(-sb_scl_, -sb_scl_, -sb_scl_);

  glEnd();

  //  glEnableClientState(GL_VERTEX_ARRAY);
  //  glEnableClientState(GL_COLOR_ARRAY);

  //  GLfloat skybox_vertices[]{
  //      sb_scl_,  -sb_scl_, sb_scl_,  sb_scl_,  sb_scl_,  sb_scl_,
  //      -sb_scl_, sb_scl_,  sb_scl_,  -sb_scl_, -sb_scl_, sb_scl_,

  //      sb_scl_,  -sb_scl_, -sb_scl_, sb_scl_,  sb_scl_,  -sb_scl_,
  //      sb_scl_,  sb_scl_,  sb_scl_,  sb_scl_,  -sb_scl_, sb_scl_,

  //      -sb_scl_, -sb_scl_, sb_scl_,  -sb_scl_, sb_scl_,  sb_scl_,
  //      -sb_scl_, sb_scl_,  -sb_scl_, -sb_scl_, -sb_scl_, -sb_scl_,

  //      sb_scl_,  sb_scl_,  sb_scl_,  sb_scl_,  sb_scl_,  -sb_scl_,
  //      -sb_scl_, sb_scl_,  -sb_scl_, -sb_scl_, sb_scl_,  sb_scl_,

  //      sb_scl_,  -sb_scl_, -sb_scl_, sb_scl_,  -sb_scl_, sb_scl_,
  //      -sb_scl_, -sb_scl_, sb_scl_,  -sb_scl_, -sb_scl_, -sb_scl_,

  //      sb_scl_,  -sb_scl_, -sb_scl_, sb_scl_,  sb_scl_,  -sb_scl_,
  //      -sb_scl_, sb_scl_,  -sb_scl_, -sb_scl_, -sb_scl_, -sb_scl_};

  //  glVertexPointer(3, GL_FLOAT, 0, skybox_vertices);
  //  glColorPointer(3, GL_FLOAT, 0, skybox_vertices);
  //  glDrawArrays(GL_TRIANGLES, 0, 6);

  //  glDisableClientState(GL_VERTEX_ARRAY);
  //  glDisableClientState(GL_COLOR_ARRAY);
}

void MyGLWidget::drawSkybox() {
  glDepthMask(GL_FALSE);

  glEnable(GL_TEXTURE_CUBE_MAP);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture_handle_);

  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);
  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  GLfloat xPlane[]{1.0, 0.0, 0.0, 0.0};
  GLfloat yPlane[]{0.0, 1.0, 0.0, 0.0};
  GLfloat zPlane[]{0.0, 0.0, 1.0, 0.0};
  glTexGenfv(GL_S, GL_OBJECT_PLANE, xPlane);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, yPlane);
  glTexGenfv(GL_R, GL_OBJECT_PLANE, zPlane);

  glTexCoord3f(pos_x_, -pos_y_, pos_z_);

  glPushAttrib(GL_LIGHTING_BIT);

  float ambient[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

  drawCube();

  glPopAttrib();

  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDisable(GL_TEXTURE_CUBE_MAP);
  glDepthMask(GL_TRUE);
}

void MyGLWidget::drawTeapot() {
  glEnable(GL_TEXTURE_CUBE_MAP);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture_handle_);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);
  glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP);

  glutSolidTeapot(1);

  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDisable(GL_TEXTURE_CUBE_MAP);
}
