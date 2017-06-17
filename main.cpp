#include <QApplication>
#include "myglwidget.h"

int main(int argc, char* argv[]) {
  srand((unsigned int)time(0));
  glutInit(&argc, argv);
  QApplication a(argc, argv);
  MyGLWidget w;
  //  w.setFixedSize(800, 800);
  w.show();

  return a.exec();
}
