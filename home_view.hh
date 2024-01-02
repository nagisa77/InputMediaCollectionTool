#ifndef HOME_VIEW_H
#define HOME_VIEW_H

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class HomeView : public QWidget {
  Q_OBJECT

public:
  HomeView(QWidget* parent = nullptr);

private slots: // 槽函数声明
  void onCaptureAudioClicked();
  void onCaptureCameraClicked();
  void onCaptureScreenClicked();

private:
  QPushButton* capture_audio_button_;
  QPushButton* capture_camera_button_;
  QPushButton* capture_screen_button_;
};

#endif // HOME_VIEW_H
