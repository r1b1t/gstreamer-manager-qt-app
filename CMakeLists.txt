cmake_minimum_required(VERSION 3.19)

project(GStreamerGUI LANGUAGES CXX)

# --- QT AYARLARI ---
find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/form)
qt_standard_project_setup()

# --- PAKETLERİ BUL (PKG-CONFIG) ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0 gstreamer-app-1.0 gstreamer-rtsp-server-1.0)

# --- FETCHCONTENT İLE KÜTÜPHANE ÇEKME ---
include(FetchContent)

# İsmi 'gstreamer_lib' olarak basitleştirdim ki değişkenlerde karışıklık çıkmasın
FetchContent_Declare(
    gstreamer_lib
    GIT_REPOSITORY https://github.com/r1b1t/gstreamer-cpp-library.git
    GIT_TAG main
)

# Kütüphaneyi kullanılabilir hale getir
FetchContent_MakeAvailable(gstreamer_lib)

# Kütüphanenin yolunu garantiye alalım (FetchContent_GetProperties kullanımı daha güvenlidir)
FetchContent_GetProperties(gstreamer_lib)
if(NOT gstreamer_lib_POPULATED)
    FetchContent_Populate(gstreamer_lib)
endif()

message(STATUS ">>>> KUTUPHANE KAYNAK YOLU: ${gstreamer_lib_SOURCE_DIR}")

# --- PROJE KAYNAKLARI ---
set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    form/mainwindow.ui
    include/LogRedirector.h
    src/LogRedirector.cpp
    include/ManualPipelineExecutor.h
    src/ManualPipelineExecutor.cpp
    src/PlayerController.cpp
    include/PlayerController.h
    include/PresetPipelineExecutor.h
    src/PresetPipelineExecutor.cpp
    src/UdpWatcherExecutor.cpp
    include/UdpWatcherExecutor.h
)

qt_add_executable(GStreamerGUI MANUAL_FINALIZATION ${PROJECT_SOURCES})

# --- INCLUDE DİZİNLERİ ---
target_include_directories(GStreamerGUI PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    # GStreamer ve GLib yollarını PKG-CONFIG'den almak daha sağlıklıdır (Hardcoded yollar yerine)
    ${GSTREAMER_INCLUDE_DIRS}

    # İndirilen kütüphanenin include yolu
    "${gstreamer_lib_SOURCE_DIR}/include"
)

# Eğer MSYS2 kullanıyorsan ve pkg-config bulamazsa diye hardcoded yolları YEDEK olarak buraya ekliyorum:
if(WIN32)
    target_include_directories(GStreamerGUI PRIVATE
        C:/msys64/mingw64/include/gstreamer-1.0
        C:/msys64/mingw64/include/glib-2.0
        C:/msys64/mingw64/lib/glib-2.0/include
    )
endif()

# --- LIBRARIES LINKING ---
# Kütüphane adını kontrol et: Genelde "FetchContent" ile çekilen projeler
# "Namespace::Target" şeklinde değil, direkt "TargetName" olarak gelir.
# Senin kütüphanenin CMakeLists.txt dosyasında "project(GStreamerCppLibrary)" yazdığı için
# hedef muhtemelen "GStreamerCppLibrary"dir.

target_link_libraries(GStreamerGUI PRIVATE
    Qt::Core
    Qt::Widgets
    ${GSTREAMER_LIBRARIES}
    gobject-2.0
    glib-2.0
    # BURASI DÜZELTİLDİ: Namespace (::) kaldırıldı.
    GStreamerCppLibrary
)

# --- DEPLOYMENT ---
include(GNUInstallDirs)
install(TARGETS GStreamerGUI BUNDLE DESTINATION . RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
qt_generate_deploy_app_script(TARGET GStreamerGUI OUTPUT_SCRIPT deploy_script NO_UNSUPPORTED_PLATFORM_ERROR)
install(SCRIPT ${deploy_script})



