# umgebung / library / OpenCV

install OpenCV `@version(4.10.0)` with homebrew `brew install opencv` ( to test installation run `opencv_version` in terminal )

include library in application CMake file with:

```CMake
set(OPENCV_UMGEBUNG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../umgebung-libraries/opencv") # path to this library

add_subdirectory(${OPENCV_UMGEBUNG_PATH}
        ${CMAKE_BINARY_DIR}/umgebung-opencv-${PROJECT_NAME} # can be any path to a directory
)

target_link_libraries(${PROJECT_NAME} PRIVATE umgebung-library-opencv) # link library to project
```

## optionally embed *Info.plist* on macOS to allow camera access

add the following lines to CMake script:

```CMake
if (APPLE)
    # Specify the path to your Info.plist file
    set(INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

    # Ensure the Info.plist file exists
    if (EXISTS "${INFO_PLIST}")
        target_link_options(${PROJECT_NAME} PRIVATE "SHELL:-sectcreate __TEXT __info_plist ${INFO_PLIST}")
    else ()
        message(FATAL_ERROR "Info.plist not found at ${INFO_PLIST}")
    endif ()
endif ()
```

and add `Info.plist` file with the following content:

```XML
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>NSCameraUsageDescription</key>
    <string>This app requires access to the camera.</string>
    <key>NSCameraUseContinuityCameraDeviceType</key>
    <true/>
</dict>
</plist>
```