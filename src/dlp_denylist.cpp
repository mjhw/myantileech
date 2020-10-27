//////////////////////////////////////////////////////////////////////////
// Author:   mjhw<371432590@qq.com>
// License:  GNU GPL
//////////////////////////////////////////////////////////////////////////
#include "dlp_denylist.h"

#if defined(DLP_CXX11)
#include <mutex>
#elif defined(DLP_WIN32)
#include <synchapi.h>
#else
#include <pthread.h>
#endif

#include "dlp_functions.h"

#define DLP_PRIVATE__DLP_DENYLIST
#include "dlp__denylist.h"

#define BEGIN_PRIVATE_NAMESPACE                                                \
  namespace {                                                                  \
  namespace p__ {
#define END_PRIVATE_NAMESPACE                                                  \
  }                                                                            \
  }
#define ASSERT(x) // assert(x)
#define ARRAY_COUNT(a) int(sizeof(a) / sizeof(a[0]))
#define XX_GENERATE_CODES(XX)                                                  \
  XX(DLP_MODVERSION_CASE_INSENSITIVE_HARD)                                     \
  XX(DLP_MODVERSION_CASE_INSENSITIVE_SOFT)                                     \
  XX(DLP_USERNAME_CASE_INSENSITIVE_HARD)                                       \
  /* END */

////////////////////////////////////////////////////////////////////////
DLP_BEGIN_NAMESPACE
////////////////////////////////////////////////////////////////////////

BEGIN_PRIVATE_NAMESPACE

#define XX(dl) const LPCTSTR *dl##_pptr;
XX_GENERATE_CODES(XX)
#undef XX

void InitGlobalDenyList(void) {
#define XX(dl) dl##_pptr = dl;
  XX_GENERATE_CODES(XX)
#undef XX
}

END_PRIVATE_NAMESPACE

void InitOnceGlobalDenyList() {
#if defined(DLP_CXX11)
  static std::once_flag once_flag;
  try {
    std::call_once(once_flag, p__::InitGlobalDenyList);
  } catch (...) {
    // TODO
  }
#elif defined(DLP_WIN32)
  static ::INIT_ONCE once_flag = INIT_ONCE_STATIC_INIT;
  BOOL status =
      ::InitOnceExecuteOnce(&once_flag, p__::InitGlobalDenyList, NULL, NULL);
  // TODO
  (void)status;
#else
  static ::pthread_once_t once_flag = PTHREAD_ONCE_INIT;
  ::pthread_once(&once_flag, p__::InitGlobalDenyList); // always returns 0.
#endif
}

DenyList *GetDenyList(DenyList *list) {
  InitOnceGlobalDenyList();
  size_t opts = list->options & ~(DenyList_CaseInsensitive | DenyList_Regex);

#define XX(dl)                                                                 \
  list->data = p__::dl##_pptr;                                                 \
  list->size = ARRAY_COUNT(dl)
  switch (opts) {
  case DenyList_Modversion_Hard:
    XX(DLP_MODVERSION_CASE_INSENSITIVE_HARD);
    break;
  case DenyList_Modversion_Soft:
    XX(DLP_MODVERSION_CASE_INSENSITIVE_HARD);
    break;
  case DenyList_Username_Hard:
    XX(DLP_USERNAME_CASE_INSENSITIVE_HARD);
    break;
  default:
    DLP_CDBG << _T("Invalid DenyList_Options: (hex)") << std::hex
             << list->options << std::dec << std::endl;
    DLP_ASSERT(false);
    list->data = NULL;
    list->size = 0;
    list->check = NULL;
  }
#undef XX

  if (list->size) {
    opts = list->options & (DenyList_CaseInsensitive | DenyList_Regex);
    switch (opts) {
    case DenyList_Default:
      list->check = STRSTR;
      break;
    case DenyList_CaseInsensitive:
      list->check = STRISTR;
      break;
    case DenyList_Regex:
      list->check = STRESTR;
      break;
    case DenyList_CaseInsensitive | DenyList_Regex:
      list->check = STREISTR;
      break;
    default:
      DLP_CDBG << _T("Invalid DenyList_Options: (hex)") << std::hex
               << list->options << std::dec << std::endl;
      DLP_ASSERT(false);
      list->data = NULL;
      list->size = 0;
      list->check = NULL;
    }
  }

  return list;
}

////////////////////////////////////////////////////////////////////////
DLP_END_NAMESPACE
////////////////////////////////////////////////////////////////////////
