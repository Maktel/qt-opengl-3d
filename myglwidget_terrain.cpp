#include "myglwidget.h"

#define GET_HEIGHT(h) (SEA_LEVEL + (((h) / 255.0f) * HEIGHT_SPREAD))
void MyGLWidget::generateTerrain() {
  // create temporary arrays for buffers -- they are loaded line by line
  const int vertices_size = (terrain_width_ - 1) * 4;
  vec3f* vertices = new vec3f[vertices_size];

  const int colors_size = (terrain_width_ - 1) * 4;
  vec3f* colors = new vec3f[colors_size];

  const int normals_size = (terrain_width_ - 1) * 4;
  vec3f* normals = new vec3f[normals_size];

  const int prev_normals_size = (terrain_width_ - 1) * 4;
  vec3f* prev_normals = new vec3f[prev_normals_size];

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

      vec3f loc_colors[]{{1.0, 0, 0}, {1.0, 0, 0}, {1.0, 0, 0}, {1.0, 0, 0}};
      //      vec3f loc_colors[]{{height_first_row[x] / 255.0f, 0, 0},
      //                         {height_first_row[x + 1] / 255.0f, 0, 0},
      //                         {height_second_row[x] / 255.0f, 0, 0},
      //                         {height_second_row[x + 1] / 255.0f, 0, 0}};

      vec3f right_triangle_norm, left_triangle_norm;
      calcNormal(loc_vertices[loc_indices[0]], loc_vertices[loc_indices[1]],
                 loc_vertices[loc_indices[2]], right_triangle_norm);
      calcNormal(loc_vertices[loc_indices[3]], loc_vertices[loc_indices[4]],
                 loc_vertices[loc_indices[5]], left_triangle_norm);

      // point is top left corner
      vec3f point_norm{0, 0, 0};
      if (x == 0) {
        if (y == 0) {
          for (int i = 0; i < 3; ++i) {
            point_norm[i] = right_triangle_norm[i] + left_triangle_norm[i];
            point_norm[i] /= 2;
          }

        } else {
          vec3f c_tri_norm{prev_normals[x * 4 + 2][0],
                           prev_normals[x * 4 + 2][1],
                           prev_normals[x * 4 + 2][2]};
          for (int i = 0; i < 3; ++i) {
            point_norm[i] =
                right_triangle_norm[i] + left_triangle_norm[i] + c_tri_norm[i];
            point_norm[i] /= 3;
          }
        }

      } else {
        vec3f d_tri_norm{normals[(x - 1) * 4 + 0][0],
                         normals[(x - 1) * 4 + 0][1],
                         normals[(x - 1) * 4 + 0][2]};
        vec3f c_tri_norm{prev_normals[x * 4 + 2][0], prev_normals[x * 4 + 2][1],
                         prev_normals[x * 4 + 2][2]};
        vec3f b_tri_norm{prev_normals[(x - 1) * 4 + 0][0],
                         prev_normals[(x - 1) * 4 + 0][1],
                         prev_normals[(x - 1) * 4 + 0][2]};
        vec3f a_tri_norm{prev_normals[(x - 1) * 4 + 2][0],
                         prev_normals[(x - 1) * 4 + 2][1],
                         prev_normals[(x - 1) * 4 + 2][2]};

        for (int i = 0; i < 3; ++i) {
          point_norm[i] = right_triangle_norm[i] + left_triangle_norm[i] +
                          d_tri_norm[i] + c_tri_norm[i] + b_tri_norm[i] +
                          a_tri_norm[i];
          point_norm[i] /= 6;
        }
      }

      vec3f loc_normals[]{
          {right_triangle_norm[0], right_triangle_norm[1],
           right_triangle_norm[2]},
          {point_norm[0], point_norm[1], point_norm[2]},
          {left_triangle_norm[0], left_triangle_norm[1], left_triangle_norm[2]},
          {point_norm[0], point_norm[1], point_norm[2]}};


      // copy locals over to "global" arrays
      // TODO find quicker way to copy elements
      const int vec_offset = x * 4;
      for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 3; ++j) {
          vertices[vec_offset + i][j] = loc_vertices[i][j];
          colors[vec_offset + i][j] = loc_colors[i][j];
          normals[vec_offset + i][j] = loc_normals[i][j];
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

    glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_[y]);
    glBufferData(GL_ARRAY_BUFFER, normals_size * sizeof(vec3f), normals,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_[y]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size * sizeof(uint), indices,
                 GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // save current line of normals
    for (int i = 0; i < normals_size; ++i) {
      for (int j = 0; j < 3; ++j) {
        prev_normals[i][j] = normals[i][j];
      }
    }
  }

  delete vertices;
  delete colors;
  delete indices;
  delete normals;
  delete prev_normals;
}
#undef GET_HEIGHT

// https://www.khronos.org/opengl/wiki/Calculating_a_Surface_Normal
void MyGLWidget::calcNormal(vec3f p1, vec3f p2, vec3f p3, vec3f out) {
  vec3f u{p2[0] - p1[0], p2[1] - p1[1], p2[2] - p1[2]};
  vec3f v{p3[0] - p1[0], p3[1] - p1[1], p3[2] - p1[2]};

  out[0] = (u[1] * v[2]) - (u[2] * v[1]);
  out[1] = (u[2] * v[0]) - (u[0] * v[2]);
  out[2] = (u[0] * v[1]) - (u[1] * v[0]);

  calcVersor(out);
}

#include <cmath>
void MyGLWidget::calcVersor(vec3f vec) {
  GLfloat len =
      sqrtf((vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]));

  if (len == 0.0f) return;

  for (int i = 0; i < 3; ++i) vec[i] /= len;
}

void MyGLWidget::drawTerrain() {
  glPushAttrib(GL_LIGHTING_BIT);
  float amb[] = {0.2, 0.2, 0.0, 1.0};
  float diff[] = {1.0, 1.0, 0.1, 1.0};
  float spec[] = {1.0, 1.0, 1.0, 1.0};
  glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
  glMateriali(GL_FRONT, GL_SHININESS, 20);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, surface_texture_handle_);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_T);
  glEnable(GL_TEXTURE_GEN_R);

  glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
  GLfloat xPlane[] = {1.0, 0.0, 0.0, 0.0};
  GLfloat zPlane[] = {0.0, 0.0, 1.0, 0.0};
  glTexGenfv(GL_S, GL_OBJECT_PLANE, xPlane);
  glTexGenfv(GL_T, GL_OBJECT_PLANE, zPlane);

  for (int y = 0; y < buffer_size_; ++y) {
    glBindBuffer(GL_ARRAY_BUFFER, vertices_buffer_[y]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, colors_buffer_[y]);
    glColorPointer(3, GL_FLOAT, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_[y]);
    glNormalPointer(GL_FLOAT, 0, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer_[y]);
    glDrawElements(GL_TRIANGLES, 6 * (terrain_width_ - 1), GL_UNSIGNED_INT, 0);
  }

  glDisable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_GEN_S);
  glDisable(GL_TEXTURE_GEN_T);
  glDisable(GL_TEXTURE_GEN_R);
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glPopAttrib();
}
