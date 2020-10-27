// -*- coding: utf-8; mode: C++ -*-
#ifndef DLP_ONCE__DLP_CONFIG
#define DLP_ONCE__DLP_CONFIG
#pragma once

// #define DLP_WIN32
// #define DLP_USE_DLLEXPORT
// #define DLP_USE_DLLIMPORT
// #define DLP_USER_NAMESPACE  // <Name>
#define SPECIAL_DLP_VERSION
#define DLP_CHAR_TYPE wchar_t
#define DLP_WINT_TYPE wint_t
#define DLP_VERSION_PATCH(Major, Minor, Patch)                                 \
  (Major * 10000UL + Minor * 100UL + Patch)
#define DLP_VERSION DLP_VERSION_PATCH(1, 0, 0)

#endif // DLP_ONCE__DLP_CONFIG
