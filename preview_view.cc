
#include "preview_view.hh"
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}
#include "camera_capture.hh"
#include <QPainter>

PreviewView::PreviewView(QWidget* parent) : QWidget(parent) {
  connect(this, &PreviewView::frameReady, this, &PreviewView::renderFrame);

  CameraCapture::getInstance()->Register(this);
}

PreviewView::~PreviewView() { CameraCapture::getInstance()->UnRegister(this); }

void PreviewView::OnCameraFrame(AVFRAME frame) {
  if (frame) {
    QImage image = convertToQImage(frame);
    emit frameReady(image);
  }
}

QImage PreviewView::convertToQImage(AVFRAME f) {
  AVFrame* frame = (AVFrame*)f;
//  if (frame->format != AV_PIX_FMT_YUV420P &&
//      frame->format != AV_PIX_FMT_YUVJ420P) {
//    return QImage();
//  }

  static SwsContext* sws_ctx = nullptr;
  if (!sws_ctx /* || 检查帧格式或大小是否改变 */) {
    if (sws_ctx) {
      sws_freeContext(sws_ctx);
    }
    sws_ctx = sws_getContext(frame->width, frame->height,
                             static_cast<AVPixelFormat>(frame->format),
                             frame->width, frame->height, AV_PIX_FMT_RGB32,
                             SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws_ctx) {
      return QImage();
    }
  }

  uint8_t* dest[4] = {nullptr};
  int dest_linesize[4] = {0};
  av_image_alloc(dest, dest_linesize, frame->width, frame->height,
                 AV_PIX_FMT_RGB32, 1);

  sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, dest,
            dest_linesize);

  QImage img(dest[0], frame->width, frame->height, dest_linesize[0],
             QImage::Format_RGB32);

  av_freep(&dest[0]);

  return img;
}

void PreviewView::renderFrame(QImage frame) {
  current_frame_ = frame;
  update();
}

void PreviewView::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  if (!current_frame_.isNull()) {
    QSize windowSize = this->size();
    QImage scaledFrame = current_frame_.scaled(windowSize, Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
    int startX = (windowSize.width() - scaledFrame.width()) / 2;
    int startY = (windowSize.height() - scaledFrame.height()) / 2;
    painter.drawImage(startX, startY, scaledFrame);
  }
}
