#project name
project(COMAssistant)
#cmake version
cmake_minimum_required (VERSION 3.8)
#include(environment.cmake)

#string(REGEX MATCH "[a-zA-z]:/[^ \t\r\n]*/lib"  GIT_ROOT "${CMAKE_CURRENT_LIST_DIR}")#路径不能有空格
get_filename_component(GIT_ROOT "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)#也可以用这句


set(REPOSITORY_PATH "${GIT_ROOT}/YQuick/inc")
get_filename_component(PROJECT_FILE_NAME "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)


set (SLN_NAME "COMAssistant")
set (TARGET_MACHINE "x64")
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

INCLUDE(ySetupProject)
set(Y_BUILD_SLN CACHE STATIC INTERNAL FORCE)

#find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui  Network Qml Quick Widgets Sql Core5Compat QuickControls2 WebEngineWidgets REQUIRED QUIET)
AddProject("${GIT_ROOT}/COMAssistant" COMObjects "Machine")
if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/test.cmake")
	include(test.cmake)
endif ()
MACRO_UPDATE_DENPENDENCY()


