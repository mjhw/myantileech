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

#ifdef DLP_WIN32
#include <tchar.h>
#else
#include <stdio.h>
#include <iconv.h>
#endif

#include <vector>
#include <memory>

#include "dlp_functions.h"

#define DLP_PRIVATE__DLP_DENYLIST
#include "dlp__denylist.h"

#define DLP_USERHASH_DENYLIST_PATH _T("/home/zyh/.aMule/userhash.deny")
#define BEGIN_PRIVATE_NAMESPACE                                                \
  namespace {                                                                  \
  namespace p__ {
#define END_PRIVATE_NAMESPACE                                                  \
  }                                                                            \
  }
#define ARRAY_COUNT(a) int(sizeof(a) / sizeof(a[0]))
#define XX_GENERATE_CODES(XX)                                                  \
  XX(DLP_MODVERSION_HARD_ICASE)                                                \
  XX(DLP_MODVERSION_SOFT_ICASE)                                                \
  XX(DLP_USERNAME_HARD_ICASE)                                                  \
  XX(DLP_USERNAME_SOFT_ICASE)                                                  \
  XX(DLP_USERHASH_SOFT_ICASE)                                                  \
  /* END */

////////////////////////////////////////////////////////////////////////
DLP_BEGIN_NAMESPACE
////////////////////////////////////////////////////////////////////////

BEGIN_PRIVATE_NAMESPACE

#define XX(dl)                                                                 \
  const LPCTSTR *dl##_data;                                                    \
  size_t dl##_size;
XX_GENERATE_CODES(XX)
#undef XX

typedef std::array<TCHAR, 32 + 1> str32_t;
static std::vector<str32_t> DLP_USERHASH_SOFT_ICASE_vec;

#ifndef DLP_WIN32
struct IConvClose {
  void operator()(iconv_t *cd) { iconv_close(*cd); }
};
bool UTF32_TO_UTF8(const wchar_t *wcs, std::string &mbs) {
  char buf[240];
  char *inbuf, *outbuf;
  size_t inbytesleft, outbytesleft;

  mbs.clear();

  iconv_t cd = iconv_open("UTF-8", "UTF-32LE");
  if (cd == iconv_t(-1)) {
    return false;
  }

  std::unique_ptr<iconv_t, IConvClose> autoclose(&cd);
  inbuf = (char *)wcs;
  inbytesleft = wcslen(wcs) * sizeof(wchar_t);

  while (inbytesleft) {
    size_t inbytesleft_ = std::min(sizeof(buf) / 6U, inbytesleft);
    inbytesleft -= inbytesleft_;
    outbuf = buf;
    outbytesleft = sizeof(buf);
    if (iconv(cd, &inbuf, &inbytesleft_, &outbuf, &outbytesleft) ==
        size_t(-1)) {
      return false;
    }
    mbs.append(buf, sizeof(buf) - outbytesleft);
  }

  return true;
}
#endif

void SetUserHashDenyList(LPCTSTR filepath) {
#ifdef DLP_WIN32
  FILE *fp = _tfopen(filepath, _T("rb"), _SH_DENYWR);
#else
  FILE *fp;
  {
    std::string mbs;
    if (!UTF32_TO_UTF8(filepath, mbs))
      return;
    fp = fopen(mbs.c_str(), "rb");
  }
#endif
  if (NULL == fp) {
    return;
  }
  std::unique_ptr<FILE, int (*)(FILE *)> autoclose(fp, &fclose);
  DLP_USERHASH_SOFT_ICASE_vec.clear();
  DLP_USERHASH_SOFT_ICASE_vec.reserve(20);
  str32_t s = {_T('\0')};
  int i = 0;
  TCHAR ch;
  bool read_eol = false;
  while (fread(&ch, sizeof(ch), 1, fp) == 1) {
    if (read_eol) {
      read_eol = !(ch == _T('\r') || ch == _T('\n'));
      continue;
    }
    if (ISSPACE(ch)) {
      continue;
    }
    if (ISXDIGIT(ch)) {
      s[i] = ch;
      if (++i < ARRAY_COUNT(s) - 1) {
        continue;
      }
      DLP_USERHASH_SOFT_ICASE_vec.push_back(s);
    }
    i = 0;
    read_eol = true;
  }
  if (ferror(fp)) {
    DLP_USERHASH_SOFT_ICASE_vec.clear();
  } else if (!DLP_USERHASH_SOFT_ICASE_vec.empty()) {
    DLP_USERHASH_SOFT_ICASE_data =
        (const LPCTSTR *)DLP_USERHASH_SOFT_ICASE_vec.data();
    DLP_USERHASH_SOFT_ICASE_size = DLP_USERHASH_SOFT_ICASE_vec.size();
  }
}

void InitGlobalDenyList(void) {
#define XX(dl)                                                                 \
  dl##_data == NULL;                                                           \
  dl##_size = 0;
  XX_GENERATE_CODES(XX)
#undef XX

  try {
    SetUserHashDenyList(DLP_USERHASH_DENYLIST_PATH);
  } catch (...) {
    // ;
  }

#define XX(dl)                                                                 \
  if (dl##_data == NULL) {                                                     \
    dl##_data = dl;                                                            \
    dl##_size = ARRAY_COUNT(dl);                                               \
  }
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
  list->data = p__::dl##_data;                                                 \
  list->size = p__::dl##_size;
  switch (opts) {
  case DenyList_ModVersion_Hard:
    XX(DLP_MODVERSION_HARD_ICASE);
    break;
  case DenyList_ModVersion_Soft:
    XX(DLP_MODVERSION_HARD_ICASE);
    break;
  case DenyList_UserName_Hard:
    XX(DLP_USERNAME_HARD_ICASE);
    break;
  case DenyList_UserHash_Soft:
    XX(DLP_USERHASH_SOFT_ICASE);
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
