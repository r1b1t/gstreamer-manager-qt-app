cmake_minimum_required(VERSION 3.19)

# Proje adını ve dillerini tanımla
project(GStreamerGUI LANGUAGES CXX)

# Qt6 Bul
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

# =========================================================
# 1. KÜTÜPHANE BAĞIMLILIKLARI (ÖNEMLİ)
# =========================================================
# Senin kütüphanen (GStreamerCppLibrary) içinde PkgConfig kullanıyor.
# Ana projede bunu bulmazsan kütüphanen düzgün yapılandırılmaz.
find_package(PkgConfig REQUIRED)

# MSYS2 kullanıyorsan GStreamer pathlerini yine de manuel vermek gerekebilir,
# ama PkgConfig'i mutlaka başlatmalısın.
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-app-1.0 gstreamer-rtsp-server-1.0)

# =========================================================
# 2. FETCH CONTENT İLE KÜTÜPHANEYİ ÇEK
# =========================================================
include(FetchContent)

FetchContent_Declare(
    GStreamerCppLibrary
    GIT_REPOSITORY https://github.com/r1b1t/gstreamer-cpp-library.git
    GIT_TAG main
)

# Kütüphaneyi kullanılabilir hale getir (Bu işlem add_subdirectory yapar)
FetchContent_MakeAvailable(GStreamerCppLibrary)

# =========================================================
# 3. KAYNAK DOSYALAR
# =========================================================
set(PROJECT_SOURCES
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

add_executable(GStreamerGUI
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

# =========================================================
# 4. LİNKLEME VE INCLUDE
# =========================================================

# MSYS2 Ortam Ayarları (Kendi pathlerin)
target_include_directories(GStreamerGUI PRIVATE
    C:/msys64/mingw64/include/gstreamer-1.0
    C:/msys64/mingw64/include/glib-2.0
    C:/msys64/mingw64/lib/glib-2.0/include
    # Buraya kütüphanenin include yolunu MANUEL EKLEMENE GEREK YOK.
    # GStreamerCppLibrary::GStreamerCppLibrary'i linkleyince otomatik gelecek.
)

target_link_directories(GStreamerGUI PRIVATE C:/msys64/mingw64/lib)

# Ana Linkleme
target_link_libraries(GStreamerGUI
    PRIVATE
    Qt::Core
    Qt::Widgets
    # GStreamer kütüphaneleri (PkgConfig'den gelen değişkenleri kullanmak daha sağlıklıdır)
    ${GSTREAMER_LIBRARIES}
    gobject-2.0
    glib-2.0
    # Senin Kütüphanen
    GStreamerCppLibrary::GStreamerCppLibrary
)

# =========================================================
# 5. İPUCU: IDE'de HPP Dosyalarını Görmek İçin
# =========================================================
# FetchContent dosyaları normalde proje ağacında görünmez.
# Eğer görmek istiyorsan, indirilen dizindeki headerları
# dummy bir target'a ekleyebilirsin (Bu sadece görsel amaçlıdır).
file(GLOB_RECURSE LIB_HEADERS
     "${gstreamercpplibrary_SOURCE_DIR}/include/*.hpp"
)
add_custom_target(LibHeaders SOURCES ${LIB_HEADERS})

# =========================================================
# 6. INSTALL
# =========================================================
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
