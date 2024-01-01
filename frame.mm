#include "frame.hh"
extern "C" {
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
}
#import <CoreMedia/CoreMedia.h>

AVFRAME CMSampleBufferRefToAVFRAME(void *ref) {
  CMSampleBufferRef sampleBuffer = (CMSampleBufferRef)ref;
  if (!sampleBuffer) {
    return nullptr;
  }

  // 获取像素缓冲区
  CVImageBufferRef imageBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
  if (!imageBuffer) {
    return nullptr;
  }

  // 锁定像素缓冲区的基地址
  CVPixelBufferLockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
  void *baseAddress = CVPixelBufferGetBaseAddress(imageBuffer);

  // 获取图像的维度和行字节
  size_t width = CVPixelBufferGetWidth(imageBuffer);
  size_t height = CVPixelBufferGetHeight(imageBuffer);
  size_t stride = CVPixelBufferGetBytesPerRow(imageBuffer);

  // 创建一个 AVFrame
  AVFrame *frame = av_frame_alloc();
  if (!frame) {
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    return nullptr;
  }

  frame->format = AV_PIX_FMT_NV12;  // 假设样本缓冲区是 NV12 格式
  frame->width = static_cast<int>(width);
  frame->height = static_cast<int>(height);

  // 分配图像缓冲区
  if (av_image_alloc(frame->data, frame->linesize, frame->width, frame->height,
                     (AVPixelFormat)frame->format, 32) < 0) {
    av_frame_free(&frame);
    CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
    return nullptr;
  }

  // 将原始数据复制到 AVFrame 缓冲区
  if (frame->format == AV_PIX_FMT_NV12) {
    // NV12 需要特殊处理，因为它是一个半平面格式
    uint8_t *yDest = frame->data[0];
    uint8_t *uvDest = frame->data[1];
    uint8_t *ySrc = static_cast<uint8_t *>(baseAddress);
    uint8_t *uvSrc = ySrc + (height * stride);

    for (int i = 0; i < height; i++) {
      memcpy(yDest, ySrc, width);
      yDest += frame->linesize[0];
      ySrc += stride;
    }
    for (int i = 0; i < height / 2; i++) {
      memcpy(uvDest, uvSrc, width);
      uvDest += frame->linesize[1];
      uvSrc += stride;
    }
  } else {
    // 对于其他格式，可能需要不同的处理方式
  }

  CVPixelBufferUnlockBaseAddress(imageBuffer, kCVPixelBufferLock_ReadOnly);
  return frame;
}
