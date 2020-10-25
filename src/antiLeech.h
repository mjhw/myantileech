// -*- mode: c++; coding: utf-8 -*-
#ifndef DLP_ONCE__ANTILEECH
#define DLP_ONCE__ANTILEECH
#pragma once

#include "dlp_defines.h"

#ifndef DLP_WIN32
#include <CString_wx.h>
#endif

#ifdef DLP_WIN32
#define DLP_XX_VIRTUAL
#else
#define DLP_XX_VIRTUAL virtual
#endif

////////////////////////////////////////////////////////////////////////
#ifndef DLP_WIN32
using namespace DLP_NAMESPACE::win32_data_types;
#endif
////////////////////////////////////////////////////////////////////////

class CantiLeech {
public:
#ifndef DLP_WIN32
  DLP_XX_VIRTUAL ~CantiLeech(){};
#endif

#ifdef DLP_WIN32
  BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD, LPVOID);
  void DLP_API TestFunc();
#endif

  DLP_XX_VIRTUAL DWORD GetDLPVersion();
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckModstring_Hard(LPCTSTR modversion,
                                                        LPCTSTR clientversion);
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckModstring_Soft(LPCTSTR modversion,
                                                        LPCTSTR clientversion);
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckUsername_Hard(LPCTSTR username);
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckUsername_Soft(LPCTSTR username);

#ifdef DLP_WIN32
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckNameAndHashAndMod(CString username,
                                                           CString &userhash,
                                                           CString &modversion);
#else
  DLP_XX_VIRTUAL LPCTSTR DLP_API
  DLPCheckNameAndHashAndMod(const CString &username, const CString &userhash,
                            const CString &modversion);
#endif

  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckMessageSpam(LPCTSTR messagetext);
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckUserhash(const PBYTE userhash);
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckHelloTag(UINT tagnumber);
  DLP_XX_VIRTUAL LPCTSTR DLP_API DLPCheckInfoTag(UINT tagnumber);
};

#ifndef DLP_WIN32
typedef CantiLeech IantiLeech;
// Bill Lee: never call delete on IantiLeech, use destoryAntiLeechInstat
// instead.
extern "C" IantiLeech *createAntiLeechInstant();
extern "C" int destoryAntiLeechInstant(IantiLeech *);
#endif

#undef DLP_XX_VIRTUAL

#endif // DLP_ONCE__ANTILEECH
