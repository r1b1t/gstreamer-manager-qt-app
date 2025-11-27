cmake_minimum_required(VERSION 3.19)
project(GStreamerGUI LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(GStreamerGUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    LogRedirector.h
    LogRedirector.cpp
    ManualPipelineExecutor.h
    ManualPipelineExecutor.cpp
    PlayerController.cpp
    PlayerController.h
    PresetPipelineExecutor.h
    PresetPipelineExecutor.cpp
    UdpWatcherExecutor.cpp
    UdpWatcherExecutor.h
)


# --- 1. PkgConfig'i aktif et ve Video Kütüphanesini Bul ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
# ----------------------------------------------------------

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets OpenGLWidgets)

target_link_libraries(GStreamerGUI
    PRIVATE
    Qt::Core
    Qt::Widgets
    Qt::OpenGLWidgets
    ${GST_VIDEO_LIBRARIES}  # <--- BU SATIRI EKLEMEN ŞART
    GStreamerCppLibrary # Senin kütüphanenin target ismi
)

# --- (Opsiyonel) Windows/MSYS2 kullanıyorsan ve header bulunamadı hatası alırsan ---
# Senin README dosyasındaki gibi include yollarını eklemen gerekebilir:
if(WIN32)
    include_directories(
        C:/msys64/mingw64/include/gstreamer-1.0
        C:/msys64/mingw64/include/glib-2.0
        C:/msys64/mingw64/lib/glib-2.0/include
        C:/msys64/mingw64/lib/GStreamerCppLibrary
        # Diğer gerekli yollar...
    )
endif()

include(GNUInstallDirs)

install(TARGETS GStreamerGUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET GStreamerGUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
