// -*- coding: utf-8; mode: C++ -*-
//////////////////////////////////////////////////////////////////////////
// Author:   mjhw<371432590@qq.com>
// License:  GNU GPL
//////////////////////////////////////////////////////////////////////////
#ifndef ZYH_ONCE__DLP_FUNCTIONS
#define ZYH_ONCE__DLP_FUNCTIONS
#pragma once

#include "dlp_defines.h"

#ifdef DLP_WIN32
#include <atlstr.h>
#else
#include <wctype.h>
#endif

#ifndef _T
#define _T(s) DLP_L(s)
#endif

#define STRICMP StrCmpI
#define STRISTR StrStrI
#define STRLEN wcslen
#define STRCMP wcscmp
#define STRSTR wcsstr
#define STRTOF wcstof
#define TOLOWER towlower
#define TOUPPER towupper
#define ISCNTRL iswcntrl
#define ISPUNCT iswpunct
#define ISSPACE iswspace
#define ISDIGIT iswdigit
#define ISXDIGIT iswxdigit

#ifndef DLP_WIN32
using namespace DLP_NAMESPACE::win32_data_types;
#endif

inline bool IS_EMPTY(LPCTSTR s) {
  // ASSERT(s != NULL);
  return s[0] == _T('\0');
}

inline bool IS_SPACE(LPCTSTR s) {
  // ASSERT(s != NULL);
  int i = 0;
  while (ISSPACE(s[i]))
    ++i;
  return i > 0;
}

LPCTSTR STRESTR(LPCTSTR s, LPCTSTR regex);
LPCTSTR STREISTR(LPCTSTR s, LPCTSTR regex);
int STRECMP(LPCTSTR s, LPCTSTR regex);
int STREICMP(LPCTSTR s, LPCTSTR regex);

#ifndef DLP_WIN32
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L
#define StrCmpI wcscasecmp
#else
int StrCmpI(LPCTSTR s1, LPCTSTR s2);
#endif
#endif

#ifndef DLP_WIN32
LPCTSTR StrStrI(LPCTSTR haystack, LPCTSTR needle);
#endif

#endif // ZYH_ONCE__DLP_FUNCTIONS
