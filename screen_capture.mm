
#include "screen_capture.hh"
#import <ScreenCaptureKit/ScreenCaptureKit.h>
#include <spdlog/spdlog.h>

@interface ScreenCaptureDelegate : NSObject<SCStreamOutput>

- (void)stream:(SCStream *)stream didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer ofType:(SCStreamOutputType)type;

@property (assign) SCStream *scstream;

@end

@implementation ScreenCaptureDelegate

- (void)stream:(SCStream *)stream didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer ofType:(SCStreamOutputType)type {
  AVFRAME frame = CMSampleBufferRefToAVFRAME(sampleBuffer);
  ScreenCapture::getInstance()->InjectFrame(frame);
}

@end

ScreenCapture* ScreenCapture::getInstance() {
  static ScreenCapture c;
  return &c;
}

// ScreenCapture 构造函数和析构函数
ScreenCapture::ScreenCapture() {}

ScreenCapture::~ScreenCapture() {}

void ScreenCapture::Start(bool enable) {
  if (enable) {
    StartCapture();
  } else {
    StopCapture();
  }
}

void ScreenCapture::Register(ScreenCaptureListener* listener) {
  listeners_.insert(listener);
}

void ScreenCapture::UnRegister(ScreenCaptureListener* listener) {
  listeners_.erase(listener);
}

void ScreenCapture::InjectFrame(AVFRAME frame) {
  for (auto listener : listeners_) {
    listener->OnScreenFrame(frame);
  }
  ReleaseAVFRAME(frame);
}

void ScreenCapture::StartCapture() {
  [SCShareableContent getShareableContentExcludingDesktopWindows:NO
                                             onScreenWindowsOnly:YES
                                               completionHandler:^(SCShareableContent *shareableContent, NSError *error) {
    if (error) {
      spdlog::info("getShareableContentExcludingDesktopWindows error: {}", [[error description] UTF8String]);
    } else {
      // prepare display
      NSArray<SCDisplay *> *displays = [shareableContent displays];
      for (SCDisplay *display in displays) {
        spdlog::info("display id: {}", display.displayID);
      }
      SCDisplay *display = displays[0];
      
      // prepare filter
      NSArray *emptyArray = [[NSArray alloc] init];
      SCContentFilter *filter = [[SCContentFilter alloc] initWithDisplay:display excludingWindows:emptyArray];
      
      // prepare scstream config
      SCStreamConfiguration *conf = [[SCStreamConfiguration alloc] init];
      [conf setWidth:display.width];
      [conf setHeight:display.height];
      [conf setScalesToFit:YES];
      [conf setDestinationRect:display.frame];
      [conf setShowsCursor:YES];
      [conf setQueueDepth:8];
      [conf setColorSpaceName:kCGColorSpaceDisplayP3];
      [conf setPixelFormat:'BGRA'];
      
      // prepare stream
      stream_ = [[SCStream alloc] initWithFilter:filter configuration:conf delegate:nil];
      
      ScreenCaptureDelegate* delegate = [[ScreenCaptureDelegate alloc] init];
      
      // set output
      NSError *err;
      [(SCStream*)stream_ addStreamOutput:delegate type:SCStreamOutputTypeScreen sampleHandlerQueue:nil error: &err];
      
      // start stream
      [(SCStream*)stream_ startCaptureWithCompletionHandler:^(NSError *_Nullable error) {
        if (error) {
          spdlog::info("err: {}", [[error description] UTF8String]);
        }
      }];
    }
  }];
}

void ScreenCapture::StopCapture() {
  [(SCStream*)stream_ stopCaptureWithCompletionHandler:^(NSError * _Nullable error) {
    spdlog::info("err: {}", [[error description] UTF8String]);
  }];
  capture_running_ = false;
}
