// -*- mode: c++; coding: utf-8 -*-
//////////////////////////////////////////////////////////////////////////
// Author:   mjhw<371432590@qq.com>
// License:  GNU GPL
//////////////////////////////////////////////////////////////////////////
#ifndef DLP_ONCE__DLP_DENYLIST
#define DLP_ONCE__DLP_DENYLIST
#pragma once

#include "dlp_defines.h"

//////////////////////////////////////////////////////////////////////////
DLP_BEGIN_NAMESPACE
//////////////////////////////////////////////////////////////////////////

#ifndef DLP_WIN32
using namespace win32_data_types;
#endif

enum DenyListOptions {
  DenyList_Default = 0x0,
  DenyList_CaseInsensitive = 0x1,
  DenyList_Regex = 0x1 << 1,
  DenyList_Modversion_Hard = 0x1 << 2,
  DenyList_Modversion_Soft = 0x1 << 3,
  DenyList_Username_Hard = 0x1 << 5,
  DenyList_Username_Soft = 0x1 << 6,
};

typedef struct DenyList {
  const LPCTSTR *data;
  size_t size;
  size_t options;
  LPCTSTR (*check)(LPCTSTR, LPCTSTR);
} DenyList;

DenyList *GetDenyList(DenyList *list);

//////////////////////////////////////////////////////////////////////////
DLP_END_NAMESPACE
//////////////////////////////////////////////////////////////////////////

#endif // DLP_ONCE__DLP_DENYLIST
