# Project Name
InputMediaCollectionTool

## Introduction

This project is a multimedia capture application based on Qt and FFmpeg, supporting audio, camera video, and screen capture functionalities. Combining Qt's powerful GUI capabilities with FFmpeg's multimedia processing, the application provides a streamlined interface for various media capture operations.

## Compilation

### Prerequisites

- Qt 6
- FFmpeg development libraries
- macOS environment (due to the use of Screen Capture Kit and other macOS-specific APIs)

### Compilation Steps

1. Clone the repository:
   ```sh
   git clone https://github.com/nagisa77/InputMediaCollectionTool
   cd InputMediaCollectionTool
   ```

2. Open the project with Qt Creator or use the command line:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```

## Execution

After compilation, run the application as follows:

```sh
./InputMediaCollectionTool
```

## Principle

The application leverages the Qt framework for building the user interface and processes audio and video using FFmpeg. Specifically:

- The **CameraCapture** class is responsible for capturing video through the camera.
- The **ScreenCapture** class uses macOS's Screen Capture Kit API to capture screen content.
- The **ViewController** class manages the transition and lifecycle between different views, such as the main view and preview view.
- The **PreviewView** class displays captured media content and converts FFmpeg's AVFrame into QImage for display.

## APIs Used & Introduction

- **Qt**: A cross-platform application framework for developing software with graphical user interfaces.
- **FFmpeg**: A multimedia framework capable of decoding, encoding, transcode, mux, demux, stream, filter, and play almost any multimedia content.
- **Screen Capture Kit**: An API provided by macOS for capturing screen content.
- **AVFoundation**: Apple's native multimedia framework for capturing, processing, and playing audio and video on macOS and iOS.

### ScreenCaptureKit

`ScreenCaptureKit` is a macOS framework for capturing screen content, including windows, the entire screen, or parts of the screen. It is part of macOS 10.15 Catalina and later versions, offering developers an efficient way to capture screen videos and audio.

#### Key Features

- **Efficient Screen Capture**: Allows applications to efficiently capture screen content, including video and audio.
- **Selective Capture**: Options to capture the entire screen, specific windows, or specific areas of the screen.
- **Support for Various Output Formats**: Supports multiple video and audio formats, suitable for different application scenarios.

#### ScreenCaptureKit in the Project

The project primarily utilizes the following interfaces of `ScreenCaptureKit`:

1. **SCShareableContent**: Retrieves a list of shareable content, such as screens or windows.

2. **SCStreamConfiguration**: Configures properties of a screen stream, such as size, color space, and pixel format.

3. **SCStream**: Represents a screen capture stream, the main interface for capturing screen content.

4. **SCStreamOutput**: A protocol defining methods for handling output data from a screen capture stream.

#### Usage Example

`ScreenCaptureKit` is used to implement screen capture functionality. By creating an `SCStream` object and configuring its properties and output processing, the application can capture screen video streams for further processing.

#### Considerations

- `ScreenCaptureKit` is only available on macOS 10.15 Catalina and later versions.
- Requires appropriate permissions and privacy settings to allow the application to capture screen content.
- Consider user privacy and data security in applications involving screen capture.

### AVFoundation

`AVFoundation` is used for camera capture on macOS. It provides access to multimedia hardware like cameras and microphones and supports capturing video and audio in various formats and resolutions.

#### Key Features

- **Device Access and Management**: Allows access to and management of multimedia hardware.
- **Media Capture**: Captures video and audio data in multiple formats and resolutions.
- **Real-Time Processing**: Supports real-time video and audio processing.

#### AVFoundation in the Project

The project utilizes `AVFoundation` for camera video capture, managing the flow of data from input devices to output.

#### Usage Example

`AVFoundation` captures video data from the camera. By creating an `AVCaptureSession` and adding the camera as an input source, the application receives a video data stream. The video frames are processed and converted for further use or display.

#### Considerations

- Requires appropriate permissions to access the camera.
- `AVFoundation` is available only on Apple platforms like iOS and macOS.
- Consider user privacy and data security in applications involving camera capture.

## Additional Information

- Ensure the application runs on macOS, as it utilizes macOS-specific APIs.
- Performance and stability may vary based on hardware and system resources.
- Suitable for educational and research purposes. Consult copyright and usage permissions for commercial use.