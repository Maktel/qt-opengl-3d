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
  glClearColor(0.5, 0.0, 0.5, 1);

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

  // load and set up terrain map
  terrain_map_ = QImage(map_path_).convertToFormat(QImage::Format_Grayscale8);
  terrain_height_ = terrain_map_.height();
  terrain_width_ = terrain_map_.width();

  // buffers will be of length equal to number of scanLines() in map image (- 1)
  buffer_size_ = terrain_height_ - 1;

  // generate buffer ids
  vertices_buffer_ = new GLuint[buffer_size_];
  colors_buffer_ = new GLuint[buffer_size_];
  indices_buffer_ = new GLuint[buffer_size_];
  glGenBuffers(buffer_size_, vertices_buffer_);
  glGenBuffers(buffer_size_, colors_buffer_);
  glGenBuffers(buffer_size_, indices_buffer_);

  generateTerrain();
}

void MyGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // set observer transformations
  glRotatef(rotation_x_, 1.0, 0.0, 0.0);
  glRotatef(rotation_y_, 0.0, 1.0, 0.0);
  glTranslatef(-pos_x_, -pos_y_, pos_z_);


  glPushMatrix();
  drawTerrain();
  glPopMatrix();


  glPushMatrix();
  glTranslatef(0, 0, -5);
  drawCube();
  glPopMatrix();


  // draw orientation lines align to a terrain
  glPushMatrix();
  glBegin(GL_LINES);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, (terrain_height_ - 1) / TERRAIN_SPREAD);

  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f((terrain_width_ - 1) / TERRAIN_SPREAD, 0.0, 0.0);
  glEnd();
  glPopMatrix();


  // don't use it -- slows application dooooooown so much...
  //  swapBuffers();

  // force drawing
  glFlush();
}

#define GET_HEIGHT(h) (SEA_LEVEL + (((h) / 255.0f) * HEIGHT_SPREAD))
void MyGLWidget::generateTerrain() {
  // create temporary arrays for buffers -- they are loaded line by line
  const int vertices_size = (terrain_width_ - 1) * 4;
  vec3f* vertices = new vec3f[vertices_size];

  const int colors_size = (terrain_width_ - 1) * 4;
  vec3f* colors = new vec3f[colors_size];

  const int indices_size = (terrain_width_ - 1) * 6;
  uint* indices = new uint[indices_size];

  for (int y = 0; y < terrain_height_ - 1; ++y) {
    const unsigned char* height_first_row = terrain_map_.constScanLine(y);
    const unsigned char* height_second_row = terrain_map_.constScanLine(y + 1);

    // 0 1
    // 2 3
    for (int x = 0; x < terrain_width_ - 1; ++x) {
      // use local coordinates and indices for now, it is easier to calculate
      // normals that way

      const auto TS = TERRAIN_SPREAD;
      vec3f loc_vertices[]{
          {x / TS, GET_HEIGHT(height_first_row[x]), y / TS},
          {(x + 1) / TS, GET_HEIGHT(height_first_row[x + 1]), y / TS},
          {x / TS, GET_HEIGHT(height_second_row[x]), (y + 1) / TS},
          {(x + 1) / TS, GET_HEIGHT(height_second_row[x + 1]), (y + 1) / TS}};

      uint loc_indices[]{0, 1, 3, 0, 3, 2};

      vec3f loc_colors[]{{height_first_row[x] / 255.0, 0, 0},
                         {height_first_row[x + 1] / 255.0, 0, 0},
                         {height_second_row[x] / 255.0, 0, 0},
                         {height_second_row[x + 1] / 255.0, 0, 0}};

      // copy locals over to "global" arrays
      // TODO find quicker way to copy elements
      const int vec_offset = x * 4;
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
          vertices[vec_offset + i][j] = loc_vertices[i][j];
          colors[vec_offset + i][j] = loc_colors[i][j];
        }
      }

      const int ind_offset = x * 6;
      for (int i = 0; i < 6; ++i) {
        // indices index is calculated as x * 6 + offset
        // value is calculated as vertex_index + local_index_offset
        indices[ind_offset + i] = vec_offset + loc_indices[i];
      }
    }

    // insert data from this line into buffers
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_[y]);
    glBufferData(GL_ARRAY_BUFFER, vertices_size * sizeof(vec3f), vertices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_[y]);
    glBufferData(GL_ARRAY_BUFFER, colors_size * sizeof(vec3f), colors,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_[y]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint), indices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  delete vertices;
  delete colors;
  delete indices;
}
#undef GET_HEIGHT

void MyGLWidget::drawTerrain() {
  for (int y = 0; y < buffer_size_; ++y) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_[y]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_[y]);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_[y]);
    glDrawElements(GL_TRIANGLES, 6 * (terrain_width_ - 1), GL_UNSIGNED_INT, 0);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
  }
}
