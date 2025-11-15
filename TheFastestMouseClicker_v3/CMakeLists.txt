cmake_minimum_required(VERSION 3.16)



project(TheFastestMouseClickerv3)



set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



# Set MSVC runtime for static linking (Windows only)
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()



# Qt5 static setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)



# Find static Qt5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)



if (WIN32 OR WIN64)
    set(UIOHOOK_SOURCE_DIR "windows")
elseif (APPLE)
    set(UIOHOOK_SOURCE_DIR "darwin")
else()
    set(UIOHOOK_SOURCE_DIR "x11")
endif()

add_library(uiohook STATIC
    "libuiohook/src/logger.c"
    "libuiohook/src/${UIOHOOK_SOURCE_DIR}/input_helper.c"
    "libuiohook/src/${UIOHOOK_SOURCE_DIR}/input_hook.c"
    "libuiohook/src/${UIOHOOK_SOURCE_DIR}/post_event.c"
    "libuiohook/src/${UIOHOOK_SOURCE_DIR}/system_properties.c"
)

# Custom include path relative to CMakeLists.txt location
target_include_directories(uiohook PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libuiohook/include
    ${CMAKE_CURRENT_SOURCE_DIR}/libuiohook/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libuiohook/src/${UIOHOOK_SOURCE_DIR}
)



# Add executable
add_executable(${PROJECT_NAME}
    main.cpp
    # Add other source files here
    global_atomic.cpp
    global_atomic.h
    TheFastestMouseClicker.cpp
    TheFastestMouseClicker.h
    MainWindow.ui
    Application.cpp
    Application.h
    HookThread.cpp
    HookThread.h
    PostThread.cpp
    PostThread.h
    MyCustomEvent.h
    myapp.rc
    myapp.qrc
)

# Custom include path relative to CMakeLists.txt location
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libuiohook/include
)

target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE)

# Link Qt5 static libraries
target_link_libraries(${PROJECT_NAME}
    uiohook
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

# Platform-specific static libraries
if(WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE /Od /Ob0 /Oi- /GL-) #/Zc:threadSafeInit-
    target_link_options(${PROJECT_NAME} PRIVATE)
    target_link_libraries(${PROJECT_NAME}
        Qt5::WinMain
        imm32
        winmm
        dwmapi
        version
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        "/SUBSYSTEM:WINDOWS"
        "/MANIFEST:NO"
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME}
        "-framework AppKit"
        "-framework ApplicationServices"
        "-framework Carbon"
        "-framework Security"
    )
else()
    target_link_libraries(${PROJECT_NAME}
        X11
        GL
        fontconfig
        freetype
    )
endif()

# Static build definition
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_STATIC
    QT_NO_IMAGEFORMAT_TGA
    QT_NO_IMAGEFORMAT_TIFF
    QT_NO_IMAGEFORMAT_WBMP
    QT_NO_IMAGEFORMAT_WEBP
    QT_NO_IMAGEFORMAT_JPEG
)
