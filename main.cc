#include <QApplication>

#include "view_controller.hh"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  
  ViewController::getInstance()->ShowHomeView(true);
  
  return app.exec();
}
