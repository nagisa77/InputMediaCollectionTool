
#ifndef PREVIEWVIEW_H
#define PREVIEWVIEW_H
#include "camera_capture.hh"
#include "screen_capture.hh"
#include <QImage>
#include <QWidget>

class PreviewView :
public QWidget,
public ScreenCaptureListener,
public CameraCaptureListener {
  Q_OBJECT

public:
  PreviewView(QWidget* parent = nullptr);
  virtual ~PreviewView();

  virtual void OnCameraFrame(AVFRAME frame) override;
  virtual void OnScreenFrame(AVFRAME frame) override;
  void paintEvent(QPaintEvent* event) override;
  void closeEvent(QCloseEvent* event) override;

  void renderFrame(QImage frame);

signals:
  void frameReady(const QImage& image);

private:
  QImage convertToQImage(AVFRAME frame);

  QImage current_frame_;
};

#endif
