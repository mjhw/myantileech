// -*- coding: utf-8; mode: C++ -*-
//////////////////////////////////////////////////////////////////////////
// Author:   mjhw<371432590@qq.com>
// License:  GNU GPL
//////////////////////////////////////////////////////////////////////////
#include "dlp_functions.h"

#include <stdio.h>

#include <regex>

#define ARRAY_COUNT(a) int(sizeof(a) / sizeof(a[0]))

LPCTSTR STRESTR(LPCTSTR s, LPCTSTR regex) {
  try {
    std::basic_regex<TCHAR> re(regex, std::regex::ECMAScript);
    std::match_results<LPCTSTR> m;
    if (std::regex_search(s, m, re)) {
      return m[0].first;
    }
  } catch (std::exception& e) {
    // TODO
    fprintf(stderr, "[DLP_DEBUG] %s:%u: %s\n", __FILE__, __LINE__, e.what());
    fflush(stderr);
  }
  return NULL;
}

LPCTSTR STREISTR(LPCTSTR s, LPCTSTR regex) {
  try {
    std::basic_regex<TCHAR> re(regex,
                               std::regex::ECMAScript | std::regex::icase);
    std::match_results<LPCTSTR> m;
    if (std::regex_search(s, m, re)) {
      return m[0].first;
    }
  } catch (std::exception& e) {
    // TODO
    fprintf(stderr, "[DLP_DEBUG] %s:%u: %s\n", __FILE__, __LINE__, e.what());
    fflush(stderr);
  }
  return NULL;
}

int STRECMP(LPCTSTR s, LPCTSTR regex) {
  try {
    std::basic_regex<TCHAR> re(regex, std::regex::ECMAScript);
    if (std::regex_match(s, re)) {
      return 0;
    }
  } catch (std::exception& e) {
    // TODO
    fprintf(stderr, "[DLP_DEBUG] %s:%u: %s\n", __FILE__, __LINE__, e.what());
    fflush(stderr);
  }
  return 1;
}

int STREICMP(LPCTSTR s, LPCTSTR regex) {
  try {
    std::basic_regex<TCHAR> re(regex,
                               std::regex::ECMAScript | std::regex::icase);
    if (std::regex_match(s, re)) {
      return 0;
    }
  } catch (std::exception& e) {
    // TODO
    fprintf(stderr, "[DLP_DEBUG] %s:%u: %s\n", __FILE__, __LINE__, e.what());
    fflush(stderr);
  }
  return 1;
}

#ifndef DLP_WIN32
#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 200809L
// #define StrCmpI wcscasecmp
#else
int StrCmpI(LPCTSTR s1, LPCTSTR s2) {
  TCHAR c1, c2;
  while (true) {
    c1 = TOLOWER(*s1);
    c2 = TOLOWER(*s2);
    if (c1 != c2)
      break;
    if (c1 == _T('\0'))
      return 0;
    ++s1;
    ++s2;
  }
  return c1 > c2 ? 1 : -1;
}
#endif
#endif

#ifndef DLP_WIN32
// Author:	greensea <gs@bbxy.net>
// Bug fixed by Orzogc Lee
LPCTSTR StrStrI(LPCTSTR haystack, LPCTSTR needle) {
  LPCTSTR ret;
  // Bill Lee: allocate wchar array on the stack
  TCHAR haystack_lower[512];
  TCHAR needle_lower[512];
  int i = 0;
  do {
    haystack_lower[i] = TOLOWER(haystack[i]);
    if (haystack_lower[i] == _T('\0'))
      break;
  } while (++i < ARRAY_COUNT(haystack_lower));
  i = 0;
  do {
    needle_lower[i] = TOLOWER(needle[i]);
    if (needle_lower[i] == _T('\0'))
      break;
  } while (++i < ARRAY_COUNT(needle_lower));

  ret = STRSTR(haystack_lower, needle_lower);
  if (ret != NULL) {
    ret = ret - haystack_lower + haystack;
  }
  return ret;
}
#endif
