#pragma once
#if defined(COMObjects_EXPORTS)
#if defined(_MSC_VER)
#define COMOBJECTS_API __declspec(dllexport)
#else
#define COMOBJECTS_API
#endif
#else
#if defined(_MSC_VER)
#define COMOBJECTS_API __declspec(dllimport)
#else
#define COMOBJECTS_API
#endif
#endif


//HDTOOL::AddImportProject
#ifndef COMObjects_EXPORTS
#ifdef _DEBUG
#   ifdef _UNICODE
#		ifdef _WIN64 
//HDTOOL::AddImportLibUnicodeDebugx64
#pragma comment(lib,"COMObjects/x64/UnicodeDebug/COMObjects.lib")
#		else
//HDTOOL::AddImportLibUnicodeDebugWin32
#pragma comment(lib,"COMObjects/Win32/UnicodeDebug/COMObjects.lib")
#		endif		
#   else
#		ifdef _WIN64 
//HDTOOL::AddImportLibDebugx64
#pragma comment(lib,"COMObjects/x64/Debug/COMObjects.lib")
#		else
//HDTOOL::AddImportLibDebugWin32
#pragma comment(lib,"COMObjects/Win32/Debug/COMObjects.lib")
#		endif
#   endif
#else
#   ifdef _UNICODE
#		ifdef _WIN64 
//HDTOOL::AddImportLibUnicodeReleasex64
#pragma comment(lib,"COMObjects/x64/UnicodeRelease/COMObjects.lib")
#		else
//HDTOOL::AddImportLibUnicodeReleaseWin32
#pragma comment(lib,"COMObjects/Win32/UnicodeRelease/COMObjects.lib")
#		endif	
#   else
#		ifdef _WIN64 
//HDTOOL::AddImportLibReleasex64
#pragma comment(lib,"COMObjects/x64/Release/COMObjects.lib")
#		else
//HDTOOL::AddImportLibReleaseWin32
#pragma comment(lib,"COMObjects/Win32/Release/COMObjects.lib")
#		endif
#   endif
#endif
#endif
//HDTOOL::AddExportClass


#include "io/io.h"


#include "COMCommunication.h"
