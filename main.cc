#include <QApplication>

#include "home_view.hh"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  HomeView homeView;
  homeView.show();

  return app.exec();
}
