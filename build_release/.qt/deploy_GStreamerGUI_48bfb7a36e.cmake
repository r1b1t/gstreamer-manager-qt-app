include("C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/GStreamerGUI-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "C:/Users/nberk/Desktop/Projeler/QT/GStreamerGUI/build_release/GStreamerGUI.exe"
    GENERATE_QT_CONF
)
