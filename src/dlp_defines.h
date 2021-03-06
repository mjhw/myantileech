// -*- coding: utf-8; mode: C++ -*-
//////////////////////////////////////////////////////////////////////////
// Author:   mjhw<371432590@qq.com>
// License:  GNU GPL
//////////////////////////////////////////////////////////////////////////
#ifndef DLP_ONCE__DLP_DEFINES
#define DLP_ONCE__DLP_DEFINES
#pragma once

//////////////////////////////////////////////////////////////////////////
#include "dlp_config.h"
//////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <wchar.h>

#include <iostream>

#if defined(DLP_USE_DLLEXPORT) && defined(DLP_USE_DLLIMPORT)
#error ""
#endif

#if __cplusplus >= 201103L
#define DLP_CXX11
#endif
#if __cplusplus >= 201402L
#define DLP_CXX14
#endif
#if __cplusplus >= 201703L
#define DLP_CXX17
#endif

#ifdef DLP_WIN32
#define DLP_DLLEXPORT __declspec(dllexport)
#define DLP_DLLIMPORT __declspec(dllimport)
#else
#define DLP_DLLEXPORT
#define DLP_DLLIMPORT
#endif

#if defined(DLP_USE_DLLEXPORT)
#define DLP_API DLP_DLLEXPORT
#elif defined(DLP_USE_DLLIMPORT)
#define DLP_API DLP_DLLIMPORT
#else
#define DLP_API
#endif

#ifdef DLP_USER_NAMESPACE
#define DLP_NAMESPACE DLP_USER_NAMESPACE
#define DLP_BEGIN_NAMESPACE namespace DLP_NAMESPACE {
#define DLP_END_NAMESPACE }
#define DLP_USING_NAMESPACE using namespace DLP_NAMESPACE;
#else
#define DLP_NAMESPACE
#define DLP_BEGIN_NAMESPACE
#define DLP_END_NAMESPACE
#define DLP_USING_NAMESPACE
#endif

#define DLP_L(s) L##s
#define DLP_C(s) // Comment
#define DLP_COUT std::wcout
#define DLP_CERR std::wcerr
#define DLP_CDBG                                                               \
  if (::dlp_use_cdbg)                                                          \
  DLP_CERR << DLP_L("[DLP_DEBUG] ") << __FILE__ << DLP_L(':') << __LINE__      \
           << DLP_L(": ")

#ifdef DLP_USE_CDBG
enum { dlp_use_cdbg = 1 };
#else
enum { dlp_use_cdbg = 0 };
#endif

#ifdef DLP_USE_ASSERT
#define DLP_ASSERT(bX) assert(bX)
#else
#define DLP_ASSERT(bX) static_cast<void>(0)
#endif

////////////////////////////////////////////////////////////////////////
DLP_BEGIN_NAMESPACE
////////////////////////////////////////////////////////////////////////

#ifndef DLP_WIN32
namespace win32_data_types {
typedef DLP_WINT_TYPE _TINT;
typedef unsigned int UINT;
typedef int BOOL;
typedef size_t DWORD;
typedef DLP_CHAR_TYPE TCHAR;
typedef const DLP_CHAR_TYPE *LPCTSTR;
typedef unsigned char *PBYTE;
typedef void *LPVOID;
typedef void *HINSTANCE;
} // namespace win32_data_types
#endif

////////////////////////////////////////////////////////////////////////
DLP_END_NAMESPACE
////////////////////////////////////////////////////////////////////////

#endif // DLP_ONCE__DLP_DEFINES
