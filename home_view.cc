#include "home_view.hh"

#include <spdlog/spdlog.h>

#include "camera_capture.hh"

HomeView::HomeView(QWidget* parent) : QWidget(parent) {
  capture_audio_button_ = new QPushButton("Capture Audio");
  capture_camera_button_ = new QPushButton("Capture Camera");
  capture_screen_button_ = new QPushButton("Capture Screen");

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(capture_audio_button_);
  layout->addWidget(capture_camera_button_);
  layout->addWidget(capture_screen_button_);

  setLayout(layout);

  // 连接按钮的点击事件到槽函数
  connect(capture_audio_button_, &QPushButton::clicked, this,
          &HomeView::onCaptureAudioClicked);
  connect(capture_camera_button_, &QPushButton::clicked, this,
          &HomeView::onCaptureCameraClicked);
  connect(capture_screen_button_, &QPushButton::clicked, this,
          &HomeView::onCaptureScreenClicked);
}

void HomeView::onCaptureAudioClicked() {
  spdlog::info("Capture Audio clicked");
}

void HomeView::onCaptureCameraClicked() {
  spdlog::info("Capture Camera clicked");
  CameraCapture::getInstance()->Start(true);
}

void HomeView::onCaptureScreenClicked() {
  spdlog::info("Capture Screen clicked");
}
