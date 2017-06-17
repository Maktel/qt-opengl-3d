#include "myglwidget.h"

MyGLWidget::MyGLWidget(QGLWidget* parent) : QGLWidget(parent) {}

MyGLWidget::~MyGLWidget() {
  delete vertices_buffer_;
  delete colors_buffer_;
  delete indices_buffer_;
}

void MyGLWidget::initializeGL() {
  // initialize glew, used for all the buffers
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    qDebug() << glewGetErrorString(err);
    QApplication::quit();
  }

  // set background (void) color
  glClearColor(93.0 / 255, 123.0 / 255, 160.0 / 255, 1);

  // change camera configuration
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // default viewport seems to be too small, so enlarge it
  glViewport(0, 0, this->width(), this->height());
  // set proper camera angle and viewport
  gluPerspective(60, 1.0 * this->width() / this->height(), 0.01, 100.0);

  // switch back to modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // doubleBuffer should be enabled by default

  // required for correct depth
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);

  glEnable(GL_LIGHTING);

  // nicer perspective calculations
  //  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  // load and set up terrain map
  terrain_map_ = QImage(kMapPath).convertToFormat(QImage::Format_Grayscale8);
  terrain_height_ = terrain_map_.height();
  terrain_width_ = terrain_map_.width();

  // buffers will be of length equal to number of scanLines() in map image (- 1)
  buffer_size_ = terrain_height_ - 1;

  // generate buffer ids
  vertices_buffer_ = new GLuint[buffer_size_];
  colors_buffer_ = new GLuint[buffer_size_];
  indices_buffer_ = new GLuint[buffer_size_];
  normals_buffer_ = new GLuint[buffer_size_];
  glGenBuffers(buffer_size_, vertices_buffer_);
  glGenBuffers(buffer_size_, colors_buffer_);
  glGenBuffers(buffer_size_, indices_buffer_);
  glGenBuffers(buffer_size_, normals_buffer_);

  // load and set up terrain textures
  glGenTextures(1, &surface_texture_handle_);

  surface_texture_img_ = QGLWidget::convertToGLFormat(QImage(kTexturePath));
  glBindTexture(GL_TEXTURE_2D, surface_texture_handle_);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, surface_texture_img_.width(),
               surface_texture_img_.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
               surface_texture_img_.bits());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, surface_texture_img_.width(),
                    surface_texture_img_.height(), GL_RGBA, GL_UNSIGNED_BYTE,
                    surface_texture_img_.bits());
  glBindTexture(GL_TEXTURE_2D, 0);


  // load and set up skybox textures
  glEnable(GL_TEXTURE_CUBE_MAP);

  glGenTextures(1, &skybox_texture_handle_);
  glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture_handle_);
  {
    QImage skybox_xp = QGLWidget::convertToGLFormat(QImage(kSkyboxXPPath));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, skybox_xp.width(),
                 skybox_xp.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 skybox_xp.bits());
  }
  {
    QImage skybox_xn = QGLWidget::convertToGLFormat(QImage(kSkyboxXNPath));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, skybox_xn.width(),
                 skybox_xn.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 skybox_xn.bits());
  }
  {
    QImage skybox_yp = QGLWidget::convertToGLFormat(QImage(kSkyboxYPPath));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, skybox_yp.width(),
                 skybox_yp.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 skybox_yp.bits());
  }
  {
    QImage skybox_yn = QGLWidget::convertToGLFormat(QImage(kSkyboxYNPath));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, skybox_yn.width(),
                 skybox_yn.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 skybox_yn.bits());
  }
  {
    QImage skybox_zp = QGLWidget::convertToGLFormat(QImage(kSkyboxZPPath));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, skybox_zp.width(),
                 skybox_zp.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 skybox_zp.bits());
  }
  {
    QImage skybox_zn = QGLWidget::convertToGLFormat(QImage(kSkyboxZNPath));
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, skybox_zn.width(),
                 skybox_zn.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 skybox_zn.bits());
  }
  // some parameters for cube map
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  glDisable(GL_TEXTURE_CUBE_MAP);

  glGenBuffers(1, &skybox_buffer_);

  prepareSkybox();
  generateTerrain();
  moveLightPosition();
  setLighting();
}

void MyGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //  moveLightPosition();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // set observer transformations
  glRotatef(rotation_x_, 1.0, 0.0, 0.0);
  glRotatef(rotation_y_, 0.0, 1.0, 0.0);

  glPushMatrix();
  glTranslatef(-pos_x_, -pos_y_, pos_z_);

  glEnable(GL_LIGHT0);
  glPushMatrix();
  drawTerrain();
  glPopMatrix();

  glPushMatrix();
  glTranslatef(5, 5, 15);
  glPushMatrix();
  drawTeapot();
  glPopMatrix();
  glPopMatrix();


  // draw orientation lines align to a terrain
  //  glPushMatrix();
  //  glBegin(GL_LINES);
  //  glColor3f(0.0, 0.0, 1.0);
  //  glVertex3f(0.0, SEA_LEVEL, 0.0);
  //  glVertex3f(0.0, SEA_LEVEL, (terrain_height_ - 1) / TERRAIN_SPREAD);

  //  glColor3f(0.0, 1.0, 0.0);
  //  glVertex3f(0.0, SEA_LEVEL, 0.0);
  //  glVertex3f((terrain_width_ - 1) / TERRAIN_SPREAD, SEA_LEVEL, 0.0);
  //  glEnd();
  //  glPopMatrix();

  glDisable(GL_LIGHT0);


  // draw skybox without any scene translations
  glPopMatrix();
  glEnable(GL_LIGHT1);
  drawSkybox();
  glDisable(GL_LIGHT1);

  // don't use it -- slows application dooooooown so much...
  //  swapBuffers();

  // force drawing
  glFlush();
}
