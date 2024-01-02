
#ifndef VIEWCONTROLLER_H
#define VIEWCONTROLLER_H

#include "home_view.hh"
#include "preview_view.hh"
#include <QWidget>

class ViewController : public QObject {
  Q_OBJECT

public:
  static ViewController* getInstance();

  void ShowHomeView(bool show);
  void ShowPreviewView(bool show);

private:
  ViewController(QObject* parent = nullptr);
  virtual ~ViewController();
  HomeView* home_view_ = nullptr;
  PreviewView* preview_view_ = nullptr;
};

#endif // VIEWCONTROLLER_H
