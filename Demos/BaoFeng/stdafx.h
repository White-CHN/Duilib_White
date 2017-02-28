// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>



// TODO: 在此处引用程序需要的其他头文件
#include "..\..\DuiLib\DuiLib.h"

using namespace DuiLib;

#ifdef _WIN64
#ifdef _DEBUG
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64_d.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64A_d.lib")
    #endif
#else
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibx64A.lib")
    #endif
#endif
#else
#ifdef _DEBUG
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLib_d.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibA_d.lib")
    #endif
#else
    #ifdef _UNICODE
        #pragma comment(lib, "..\\..\\Lib\\DuiLib.lib")
    #else
        #pragma comment(lib, "..\\..\\Lib\\DuiLibA.lib")
    #endif
#endif
#endif