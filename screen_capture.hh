
#ifndef SCREEN_CAPTURE_HH
#define SCREEN_CAPTURE_HH
#include "frame.hh"
#include <set>

class ScreenCaptureListener {
public:
  virtual ~ScreenCaptureListener() {}
  virtual void OnScreenFrame(AVFRAME frame) = 0;
};

class ScreenCapture {
public:
  void Start(bool enable);
  void Register(ScreenCaptureListener* listener);
  void UnRegister(ScreenCaptureListener* listener);
  static ScreenCapture* getInstance();
  void InjectFrame(AVFRAME frame);

private:
  ScreenCapture();
  ~ScreenCapture();

  void StartCapture();
  void StopCapture();

  void* stream_; 
  std::set<ScreenCaptureListener*> listeners_;
  bool capture_running_ = false;
};

#endif // SCREEN_CAPTURE_HH
