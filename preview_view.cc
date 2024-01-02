
#include "preview_view.hh"
extern "C" {
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
}
#include "camera_capture.hh"
#include "view_controller.hh"
#include <QPainter>

PreviewView::PreviewView(QWidget* parent) : QWidget(parent) {
  connect(this, &PreviewView::frameReady, this, &PreviewView::renderFrame);

  CameraCapture::getInstance()->Register(this);
}

PreviewView::~PreviewView() {
  CameraCapture::getInstance()->UnRegister(this);
  CameraCapture::getInstance()->Start(false);
}

void PreviewView::closeEvent(QCloseEvent* event) {
  ViewController::getInstance()->ShowPreviewView(false); 
  QWidget::closeEvent(event);
}

void PreviewView::OnCameraFrame(AVFRAME frame) {
  if (frame) {
    QImage image = convertToQImage(frame);
    emit frameReady(image);
  }
}

QImage PreviewView::convertToQImage(AVFRAME f) {
  AVFrame* frame = (AVFrame*)f;
  if (!frame) {
    return QImage();
  }

  static SwsContext* sws_ctx = nullptr;
  AVPixelFormat src_pix_fmt = (AVPixelFormat)frame->format; // 设置源格式为 YUYV422
  AVPixelFormat dst_pix_fmt = AV_PIX_FMT_RGBA;    // 目标格式为 ARGB

  // 检查是否需要重新创建转换上下文
  if (!sws_ctx || frame->format != src_pix_fmt) {
    if (sws_ctx) {
      sws_freeContext(sws_ctx);
    }
    sws_ctx = sws_getContext(frame->width, frame->height, src_pix_fmt,
                             frame->width, frame->height, dst_pix_fmt,
                             SWS_BILINEAR, nullptr, nullptr, nullptr);
    if (!sws_ctx) {
      return QImage();
    }
  }

  uint8_t* dest[4] = {nullptr};
  int dest_linesize[4] = {0};
  if (av_image_alloc(dest, dest_linesize, frame->width, frame->height,
                     dst_pix_fmt, 1) < 0) {
    return QImage();
  }

  sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, dest,
            dest_linesize);

  QImage img(dest[0], frame->width, frame->height, dest_linesize[0],
             QImage::Format_RGBA8888);

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
    QSize windowSize = size();
    QImage scaledFrame = current_frame_.scaled(windowSize, Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
    int startX = (windowSize.width() - scaledFrame.width()) / 2;
    int startY = (windowSize.height() - scaledFrame.height()) / 2;
    painter.drawImage(startX, startY, scaledFrame);
  }
}
