
#include "camera_capture.hh"
#include <AVFoundation/AVFoundation.h>
#include <spdlog/spdlog.h>
#include "frame.hh"

@interface CameraCaptureDelegate : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate>
@end

@implementation CameraCaptureDelegate
- (void)captureOutput:(AVCaptureOutput *)output
    didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection *)connection {
  AVFRAME frame = CMSampleBufferRefToAVFRAME(sampleBuffer); 
}
@end

CameraCapture *CameraCapture::getInstance() {
  static CameraCapture c;
  return &c;
}

// CameraCapture 构造函数和析构函数
CameraCapture::CameraCapture() {}

CameraCapture::~CameraCapture() {
  // 清理资源
}

void CameraCapture::Start(bool enable) {
  if (enable) {
    StartCapture();
  } else {
    StopCapture();
  }
}

void CameraCapture::Register(CameraCaptureListener *listener) { listeners_.insert(listener); }

void CameraCapture::UnRegister(CameraCaptureListener *listener) { listeners_.erase(listener); }

void CameraCapture::StartCapture() {
  if (capture_running_) {
    spdlog::info("capture is running");
    return;
  }
  capture_session_ = [[AVCaptureSession alloc] init];
  capture_running_ = true;
  [(AVCaptureSession *)capture_session_ setSessionPreset:AVCaptureSessionPresetHigh];

  AVCaptureDevice *videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
  NSError *error;
  AVCaptureDeviceInput *videoInput = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice
                                                                           error:&error];

  if ([(AVCaptureSession *)capture_session_ canAddInput:videoInput]) {
    [(AVCaptureSession *)capture_session_ addInput:videoInput];
  }

  CameraCaptureDelegate *delegate = [[CameraCaptureDelegate alloc] init];
  dispatch_queue_t queue = dispatch_queue_create("cameraCaptureQueue", NULL);

  AVCaptureVideoDataOutput *output = [[AVCaptureVideoDataOutput alloc] init];
  [output setSampleBufferDelegate:delegate queue:queue];

  if ([(AVCaptureSession *)capture_session_ canAddOutput:output]) {
    [(AVCaptureSession *)capture_session_ addOutput:output];
  }

  [(AVCaptureSession *)capture_session_ startRunning];
}

void CameraCapture::StopCapture() {
  [(AVCaptureSession *)capture_session_ stopRunning];
  capture_session_ = nullptr;
  capture_running_ = false;
}

// void CameraCapture::CaptureOutput(/* 参数 */) {
//   AVFrame* frame = av_frame_alloc();
//   // 填充 frame 数据
//
//   for (auto listener : listeners_) {
//     listener->OnCameraFrame(frame);
//   }
//
//   av_frame_free(&frame);
// }
