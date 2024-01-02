
#include "view_controller.hh"

ViewController::ViewController(QObject* parent) : QObject(parent) {}

ViewController::~ViewController() {}

ViewController* ViewController::getInstance() {
  static ViewController vc;
  return &vc;
}

void ViewController::ShowHomeView(bool show) {
  if (show) {
    if (!home_view_) {
      home_view_ = new HomeView();
      home_view_->show();
    }
  } else {
    if (home_view_) {
      home_view_->deleteLater();
      home_view_ = nullptr;
    }
  }
}

void ViewController::ShowPreviewView(bool show) {
  if (show) {
    if (!preview_view_) {
      preview_view_ = new PreviewView();
      preview_view_->show();
    }
  } else {
    if (preview_view_) {
      preview_view_->deleteLater();
      preview_view_ = nullptr;
    }
  }
}
