cmake_minimum_required(VERSION 3.10)
project(InputMediaCollectionTool)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -x objective-c++")

cmake_policy(SET CMP0100 NEW)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

find_package(spdlog REQUIRED)

find_library(COCOA_LIBRARY Cocoa)

find_library(SCREEN_CAPTURE_KIT_LIBRARY ScreenCaptureKit)

find_library(AVFOUNDATION_LIBRARY AVFoundation)

find_path(FFMPEG_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)
find_library(AVCODEC_LIBRARY avcodec)
find_library(AVUTIL_LIBRARY avutil)
find_library(SWSCALE_LIBRARY swscale)
find_library(SWRESAMPLE_LIBRARY swresample)

find_library(COREMEDIA_LIBRARY CoreMedia)
find_library(COREVIDEO_LIBRARY CoreVideo)

file(GLOB PROJECT_SOURCES "*.cc" "*.hh" "*.mm")

file(GLOB_RECURSE OBJCPP_SOURCES "*.mm")
set_source_files_properties(${OBJCPP_SOURCES} PROPERTIES LANGUAGE CXX)

add_executable(InputMediaCollectionTool
  ${PROJECT_SOURCES}
)

target_link_libraries(InputMediaCollectionTool
  Qt6::Core
  Qt6::Widgets
  spdlog::spdlog
  ${COCOA_LIBRARY}
  ${SCREEN_CAPTURE_KIT_LIBRARY}
  ${AVFOUNDATION_LIBRARY}
  ${AVFORMAT_LIBRARY}
  ${AVCODEC_LIBRARY}
  ${AVUTIL_LIBRARY}
  ${SWSCALE_LIBRARY}
  ${COREMEDIA_LIBRARY}
  ${COREVIDEO_LIBRARY}
)
