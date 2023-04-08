#This CMakeLists.txt is created by HDTool V1
#project name
project(COMObjects)

#cmake version
cmake_minimum_required (VERSION 3.8)
#system environment


MACRO_PROJECT_ENVIRONMENT()
MACRO_PROJECT_DLL()
MACRO_PROJECT_DENPENDENCY(${PROJECT_NAME} io)