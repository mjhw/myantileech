// DLP = Dynamic Leecher Protection
// this code is part of Xtreme-Mod
// author: Xman

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "antiLeech.h"

#include "dlp_denylist.h"
#include "dlp_functions.h"

// Add by SDC team.
#if defined(SPECIAL_DLP_VERSION)
#define DLPVERSION 44005
#define SDC_VERSION_DETACH 44, 0, 0, 5
#else // Official
#define DLPVERSION 44
#endif

#ifdef DLP_WIN32
#define CantiLeech__ DLP_API
#else
#define CantiLeech__ CantiLeech::
#endif

#define ARRAY_COUNT(a) int(sizeof(a) / sizeof(a[0]))
#define ASSERT(x) // assert(x)

////////////////////////////////////////////////////////////////////////
#ifndef DLP_WIN32
using namespace DLP_NAMESPACE::win32_data_types;
#endif
////////////////////////////////////////////////////////////////////////

using DLP_NAMESPACE::DenyList;
using DLP_NAMESPACE::DenyListOptions;

namespace {

//>>> eWombat [SNAFU_V3]
LPCTSTR apszSnafuTag[] = {
    _T("[DodgeBoards]"),
    _T("[DodgeBoards & DarkMule eVorteX]"),
    _T("[DarkMule v6 eVorteX]"),
    _T("[eMuleReactor]"),
    _T("[Bionic]"),
    _T("[LSD7c]"),
    _T("[0x8d] unknown Leecher - (client version:60)"),
    _T("[RAMMSTEIN]"),
    _T("[MD5 Community]"),
    _T("[new DarkMule]"),
    _T("[OMEGA v.07 Heiko]"),
    _T("[eMule v0.26 Leecher]"),
    _T("[Hunter]"),
    _T("[Bionic 0.20 Beta]"),
    _T("[Rumata (rus)(Plus v1f)]"),
    _T("[Fusspi]"),
    _T("[donkey2002]"),
    _T("[md4]"),
    _T("[SpeedMule]"),
    _T("[pimp]"),
    _T("[Chinese Leecher]") // SquallATF
                            //,_T("[eChanblardNext]") // zz_fly
};

bool IsTypicalHex(const CString &addon) {
  int n = addon.GetLength();
  if (n < 5 || n > 25)
    return false;

  bool bigalpha = false;
  bool smallalpha = false;
  bool numeric = false;

  while (--n) {
    const TCHAR c = addon.GetAt(n);
    /**/ if (c >= _T('0') && c <= _T('9'))
      numeric = true;
    else if (c >= _T('A') && c <= _T('F'))
      bigalpha = true;
    else if (c >= _T('a') && c <= _T('f'))
      smallalpha = true;
    else
      return false;
  }

  return numeric && (bigalpha != smallalpha);
}

} // namespace

#ifdef DLP_WIN32
BOOL WINAPI DllMain(HANDLE hModule, DWORD dwFunction, LPVOID lpNot) {
  switch (dwFunction) {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}
#endif

#ifdef DLP_WIN32
void CantiLeech__ TestFunc() {
  ::MessageBox(NULL, _T("Inside the DLL!"), _T("Nix"), 0);
}
#endif

DWORD CantiLeech__ GetDLPVersion() { return DLPVERSION; }

LPCTSTR CantiLeech__ DLPCheckUserhash(const PBYTE userhash) {
  // No more AJ check
  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckModstring_Hard(LPCTSTR modversion,
                                            LPCTSTR clientversion) {
  if (modversion == NULL || clientversion == NULL) {
    return NULL;
  }

  /// Allow-List:
  if (IS_EMPTY(modversion)) {
    if (STRSTR(clientversion, _T("eMule"))) {
      if (!STRSTR(clientversion, _T("eMule v0.50a")) &&
          !STRSTR(clientversion, _T("eMule v0.50b")) &&
          !STRSTR(clientversion, _T("eMule v0.60a")) &&
          !STRSTR(clientversion, _T("eMule v0.51d"))) {
        return _T("Bad eMule");
      }
    }
    if (STRSTR(clientversion, _T("aMule"))) {
      if (!STRSTR(clientversion, _T("aMule v2.3.2")) &&
          !STRSTR(clientversion, _T("aMule v2.3.1"))) {
        return _T("Bad eMule");
      }
    }
    // TODO
  } else {
    LPCTSTR regex = _T("eMule v0.[56][01][abd] - [a-z]+ v?[0-9.a-z_-]+");
    if (STREICMP(clientversion, regex) != 0) {
      DLP_CDBG << _T("CilentVersion:\"") << clientversion << "\"" << std::endl;
      return _T("Bad Modstring Scheme");
    }
    if (STRISTR(clientversion, _T("aMule"))) {
      return _T("Fake aMule");
    }
    // TODO
  }

  DenyList denylist;
  denylist.options = DenyList_Modversion_Hard | DenyList_CaseInsensitive;
  GetDenyList(&denylist);
  for (size_t i = 0; i < denylist.size; ++i) {
    if (denylist.check(modversion, denylist.data[i]))
      return _T("Bad MODSTRING");
  }

  // Add by SDC team.
#if defined(SPECIAL_DLP_VERSION)
  // Some Bad MODSTRING check
  if (STRSTR(modversion, _T("eMule-GIFC")) // GPL-Breaker [DragonD]
      || (STRSTR(clientversion, _T("0.49c")) &&
          STRSTR(modversion, _T("X-Ray 2."))) // Fake X-Ray Mod [**Riso64Bit**]
      || (STRSTR(clientversion, _T("0.48a")) &&
          STRSTR(modversion, _T("MorphCA"))) // Fake MorphCA [DargonD]
      || STRSTR(modversion, _T("0.50a")) // It should be a ClientVersion, not a
                                         // ModString [DargonD]
      || STRSTR(clientversion, _T("4.0h")) // New SpeedyP2P client
      || STRSTR(modversion, _T("OS"))      // GPL-Breaker [ieD2k]
      || STRSTR(modversion, _T("THC"))     // Fake queues client [Bill Lee]
      || STRSTR(modversion, _T("EggAche")) // Custom ModString
      || STRSTR(modversion, _T("DarkSky")) // Custom ModString
      || STRSTR(clientversion,
                _T("eMule v5.6a")) // Fake official version [冰靈曦曉]
      || STRSTR(modversion, _T("eMuleTorrent")) // GPL-Breaker [冰靈曦曉]
      /* END */) {
    return _T("[SDC] Bad ModString");
  }
#endif

  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckModstring_Soft(LPCTSTR modversion,
                                            LPCTSTR clientversion) {
  if (modversion == NULL || clientversion == NULL) {
    return NULL;
  }

  DenyList denylist;
  denylist.options = DenyList_Modversion_Soft | DenyList_CaseInsensitive;
  GetDenyList(&denylist);
  for (size_t i = 0; i < denylist.size; ++i) {
    if (denylist.check(modversion, denylist.data[i]))
      return _T("Bad MODSTRING");
  }

  if ((STRSTR(clientversion, _T("lphant v2.01")) &&
       STRSTR(modversion, _T("Plus"))) // www.lphantplus.com, no src
      /* END */) {
    return _T("Bad MODSTRING");
  }

  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckUsername_Hard(LPCTSTR username) {
  if (username == NULL) {
    return NULL;
  }

  DenyList denylist;
  denylist.options = DenyList_Username_Hard | DenyList_CaseInsensitive;
  GetDenyList(&denylist);
  for (size_t i = 0; i < denylist.size; ++i) {
    if (denylist.check(username, denylist.data[i]))
      return _T("Bad USERNAME");
  }

  // new ketamine
  // if(STRISTR(username, _T("[ePlus]")) != StrRStrI(username,NULL,
  // _T("[ePlus]"))) //old version
  if (STRLEN(username) >= 14) {
    LPCTSTR tempstr = STRSTR(username, _T("[ePlus]"));
    if (tempstr && STRLEN(tempstr) >= 7)
      if (STRSTR(tempstr + 7, _T("[ePlus]")))
        return _T("new Ketamine");
  }

  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckUsername_Soft(LPCTSTR username) {
  if (username == NULL) {
    return NULL;
  }

  if ( // Xman 15.08.05
      STRISTR(username, _T(">>Power-Mod"))
      // Xman 1/2007
      /*
        STRISTR(username,_T("AppleJuice [")) && STRISTR(username,_T("]")) ||
        STRISTR(username,_T("AppleJuice Mod [")) && STRISTR(username,_T("]")) ||
        STRISTR(username,_T("AppleJuice eMule [")) && STRISTR(username,_T("]"))
        //5/2007
      */
      // zz_fly Start korea
      || STRSTR(username, _T("DONKEY2007")) // korea
      || STRSTR(username, _T("www.Freang.com")) ||
      STRSTR(username, _T("www.pruna.com")) ||
      STRSTR(username, _T("[KOREA]")) || STRSTR(username, _T("superemule")) ||
      STRSTR(username, _T("PRUNA 2008")) || STRSTR(username, _T("MOYAM")) ||
      STRSTR(username, _T("eDonkey2009"))
      // zz_fly End
      /* END */) {
    return _T("Bad USERNAME");
  }

  // bad mods, where every second sign is
  // enough to check two places

  if (STRLEN(username) >= 4 && username[3] <= 0x1F && username[1] <= 0x1F) {
    return _T("Community-Username"); // community (based on LSD or it's
                                     // smasher)
  }

  if (STRLEN(username) >= 4 && username[0] == _T('v') &&
      STRSTR(username, _T(":com "))) {
    return _T("X-Treme");
  }

  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckNameAndHashAndMod(
#ifdef DLP_WIN32
    CString username, CString &userhash, CString &modversion
#else
    const CString &username, const CString &userhash, const CString &modversion
#endif
) {
  if (username.IsEmpty() || userhash.IsEmpty())
    return NULL;

  // community userhash check
  static const TCHAR refuserhash0[] =
      _T("154CE646120E96CC798C439A20D26F8D"); // (windows ue)
  static const TCHAR refuserhash1[] =
      _T("455361F9D95C3CD7E6BF2192D1CB3D02"); // (windows ue)
  static const TCHAR refuserhash2[] = _T("C8B5F41441C615FBABAD9A7E55294D01");
  static const TCHAR refuserhash6[] =
      _T("A2221641460E961C8B7FF21A53FB6F6C"); //**Riso64Bit**
  static const TCHAR refuserhash7[] =
      _T("888F4742450EF75F9DD8B7E53FA06FF0"); //**Riso64Bit**
  static const TCHAR refuserhash8[] =
      _T("0B76CC42CB0E81B0DC6120D2BCB36FF9"); //**Riso64Bit**
  static const TCHAR refuserhash9[] =
      _T("EAA383FD9E0E68538C7AC8AD15526F7A"); //**Riso64Bit**
  static const TCHAR refuserhash10[] =
      _T("65C3B2E8940E582630A7F58AF9F26F9E"); // from TaiWan
  static const TCHAR refuserhash11[] =
      _T("9BA09B83DC0EE78BE20280C387936F00"); // from SS1900
  static const TCHAR refuserhash12[] =
      _T("C92859E4860EA0F15F7837750C886FB6"); // from SS1900
  static const TCHAR refuserhash13[] =
      _T("CB42F563EE0EA7907395420CAC146FF5"); // From "qobfxb" multi user
                                              // [DargonD]

  // SDC fixed
#if defined(SPECIAL_DLP_VERSION)
  if (STRICMP(userhash, refuserhash0) == 0 ||
      STRICMP(userhash, refuserhash1) == 0 ||
      STRICMP(userhash, refuserhash2) == 0 ||
      STRICMP(userhash, refuserhash6) == 0 ||
      STRICMP(userhash, refuserhash7) == 0 ||
      STRICMP(userhash, refuserhash8) == 0 ||
      STRICMP(userhash, refuserhash9) == 0 ||
      STRICMP(userhash, refuserhash10) == 0 ||
      STRICMP(userhash, refuserhash11) == 0 ||
      STRICMP(userhash, refuserhash12) == 0 ||
      // The official refuserhash13 with NickName "qobfxb" will be checked in
      // DLPCheckUsername_Hard function.
      (!STRSTR(username, _T("qobfxb")) &&
       STRICMP(userhash, refuserhash13) == 0))
    return _T("[SDC] Community UserHash");
#else // Official
  if (STRICMP(userhash, refuserhash0) == 0 ||
      STRICMP(userhash, refuserhash1) == 0 ||
      STRICMP(userhash, refuserhash2) == 0 ||
      STRICMP(userhash, refuserhash6) == 0 ||
      STRICMP(userhash, refuserhash7) == 0 ||
      STRICMP(userhash, refuserhash8) == 0 ||
      STRICMP(userhash, refuserhash9) == 0 ||
      STRICMP(userhash, refuserhash10) == 0 ||
      STRICMP(userhash, refuserhash11) == 0 ||
      STRICMP(userhash, refuserhash12) == 0 ||
      STRICMP(userhash, refuserhash13) == 0)
    return _T("Community Userhash");
#endif

  // corrupt userhash check
  static const TCHAR refuserhash3[] = _T("00000000000E00000000000000006F00");
  static const TCHAR refuserhash4[] = _T("FE000000000E00000000000000006F00");
  if (STRICMP(userhash, refuserhash3) == 0 ||
      STRICMP(userhash, refuserhash4) == 0)
    return _T("Corrupt UserHash");

    // SDC fixed
    // Community Userhash check, thanks SquallATF.
#if defined(SPECIAL_DLP_VERSION)
  static const TCHAR RefUserHash_5[] = _T("DA1CEEE05B0E5319B3B48CAED24C6F4A");
  if (!STRSTR(username, _T("QQDownload")) &&
      STRICMP(userhash, RefUserHash_5) ==
          0) // The official refuserhash5 with NickName "QQDownload" will be
             // checked in DLPCheckUsername_Hard function.
    return _T("[SDC] Bad UserHash");
#else // Official
  static const TCHAR refuserhash5[] = _T("DA1CEEE05B0E5319B3B48CAED24C6F4A");
  if (STRICMP(userhash, refuserhash5) == 0)
    return _T("Bad Userhash");
#endif

  // zz_fly End

  // Check for aedit
  // remark: a unmodded emule can't send a space at last sign
  if (modversion.IsEmpty() && username.Right(1) == _T(" "))
    return _T("AEdit");

  // Check for Hex-Modstring
  if (IsTypicalHex(modversion))
    return _T("Hex-Modstring");

  // community check
  if (username.GetLength() >= 7 && username.Right(1) == _T("]")) {
    /* no more needed
    //check for gamer
    //two checks should be enough.
    if(username.Right(6).Left(1)==userhash.Mid(5,1)
    && username.Right(3).Left(1)==userhash.Mid(7,1)
    )
    return _T("old united");
    */

    // check for special nickaddon
    int find = username.ReverseFind(_T('['));
    if (find >= 0) {
      CString addon = username.Mid(find + 1);
      // int endpos=addon.GetLength()-1;
      if (addon.GetLength() > 2) {
        // check for snake //12/2006
        /* no more needed, better detection inside Xtreme
           for(int i=0; i<endpos;i++)
           {
           if( !(addon.GetAt(i)>=_T('0') && addon.GetAt(i)<=_T('9')) )
           {
           i=endpos+1;
           }
           }
           if(i==endpos)
           return _T("Snake");
        */

        // Chek for Hex (e.g. X-Treme)
        // SDC fixed
#if defined(SPECIAL_DLP_VERSION)
        auto AddonLeft = addon.Left(addon.GetLength() - 1);
        if (IsTypicalHex(AddonLeft))
#else // Official
        if (IsTypicalHex(addon.Left(addon.GetLength() - 1)))
#endif
          return _T("Hex-Code-Addon");
      }
      // zz_fly :: start
      if (find == username.GetLength() - 6) {
        bool bFoundRandomPadding = false;
        _TINT ch;
        for (int i = 1; i < 5; i++) {
          ch = username.GetAt(find + i);
          if (ISPUNCT(ch) || /*ISSPACE(ch) ||*/ ISCNTRL(ch)) {
            bFoundRandomPadding = true;
            break;
          }
        }
        if (bFoundRandomPadding && !modversion.IsEmpty() &&
            (username.Find(_T("http://emule-project.net [")) == 0) &&
            (find == 25))
          return _T("TLH Community"); // username like
                                      // "http://emule-project.net [random]"
        if (bFoundRandomPadding && modversion.IsEmpty() &&
            (find == username.Find(_T('['))))
          return _T("GhostMod"); // username has a random padding [random], it
                                 // should be a mod function, but there is no
                                 // modstring
        if (bFoundRandomPadding &&
            (username.Find(_T("Silver Surfer User")) == 0) &&
            (modversion.Find(_T("Silver")) == -1))
          return _T("Fake Silver Surfer"); //**Riso64Bit** :: fake silver
                                           // surfer
      }
      // zz_fly :: end
    }
  }

  // thx cyrex
  if (modversion.GetLength() == 10 && username.GetLength() > 4 &&
      STRSTR(username.Right(4), _T("/]")) && STRSTR(username, _T("[SE]")))
    return _T("Mystery ModString");

    // Add by SDC team.
#if defined(SPECIAL_DLP_VERSION)
  // Some Community UserHash check
  static const TCHAR SDC_RefUserHash_1[] =
      _T("66B002DADE0E6DBEDF4FCCAA380E6FD4"); // From multi user (TW&CN)
                                              // [DargonD]
  static const TCHAR SDC_RefUserHash_2[] =
      _T("AAEE84C0C30E247CBB99B459255D6F99"); // From NAS_01G multi user
                                              // [DargonD]
  static const TCHAR SDC_RefUserHash_3[] =
      _T("5E02F74DBA0E8A19DBF6733F0AE66F4A"); // Community UserHash
                                              // [FzH/DargonD]
  static const TCHAR SDC_RefUserHash_4[] =
      _T("B6491292AE0E07AC8C6045CAC2DD6F9F"); // Community UserHash
                                              // [FzH/DargonD]
  static const TCHAR SDC_RefUserHash_5[] =
      _T("596B305E050EA842CE38DF3811216F3F"); // Community UserHash
                                              // [FzH/DargonD]
  static const TCHAR SDC_RefUserHash_6[] =
      _T("B1798B2F620E0B676452C6E2EF706F13"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_7[] =
      _T("C1533316C00E3E0D0218843A05E46FAC"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_8[] =
      _T("FE10F3C0610E0A925B85204CE8456F42"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_9[] =
      _T("C9E61DEEF30E0360E2741C9CF1396F94"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_10[] =
      _T("559ACC89D80E90C50A7A0CD3224F6F57"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_11[] =
      _T("6AE1D2DF4B0E8707B6F6BC29E8746F0F"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_12[] =
      _T("8A537F20B80EF9AF02E59E6C087C6F6B"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_13[] =
      _T("3F44A7996F0E17D1F4B319EB58B26F64"); // Invalid UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_14[] =
      _T("D0D897BD360EEFF329903E04990B6F86"); // Xunlei
  static const TCHAR SDC_RefUserHash_15[] =
      _T("36725093E00E9350F7680C871E946FD1"); // Tencent Offline Download
                                              // Server UserHash [DargonD]
  static const TCHAR SDC_RefUserHash_16[] =
      _T("769D36987E0E313A1501967D0F146F7A"); // UserHash of Xunlei Offline
                                              // Download Server and Moblie
                                              // System Apps [pandaleo]
  if (STRICMP(userhash, SDC_RefUserHash_1) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_2) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_3) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_4) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_5) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_6) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_7) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_8) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_9) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_10) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_11) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_12) == 0 ||
      STRICMP(userhash, SDC_RefUserHash_13) == 0 ||
      (!STRSTR(modversion, _T("xl build")) &&
       STRICMP(userhash, SDC_RefUserHash_14) ==
           0) || // The SDC_RefUserHash_14 with modstring "xl build" will be
                 // checked in DLPCheckModstring_Hard function.
      (!STRSTR(username, _T("[CHN][VeryCD]QQ")) &&
       STRICMP(userhash, SDC_RefUserHash_15) ==
           0) || // The SDC_RefUserHash_15 with NickName "[CHN][VeryCD]QQ" will
                 // be checked in DLPCheckUsername_Hard function.
      (!STRSTR(username, _T("[CHN]shaohan")) &&
       STRICMP(userhash, SDC_RefUserHash_16) ==
           0)) // The SDC_RefUserHash_16 with NickName "[CHN]shaohan" will be
               // checked in DLPCheckUsername_Hard function.
    return _T("[SDC] Community UserHash");
#endif

  if (modversion.IsEmpty())
    return NULL;

#define NUMBERSOFSTRING 9
  static const CString testModString[] = {_T("Xtreme"),
                                          _T("ScarAngel"),
                                          _T("Mephisto"),
                                          _T("MorphXT"),
                                          _T("EastShare"),
                                          _T("StulleMule"),
                                          /*_T("Magic Angel"),*/ _T("DreaMule"),
                                          _T("X-Mod"),
                                          _T("RaJiL")};
  static const float testMinVer[] = {4.4f, 2.5f,           1.5f, 10.0f, 13.0f,
                                     6.0f, /*3.0f,*/ 3.0f, 0.0f, 2.2f};
  for (int i = 0; i < NUMBERSOFSTRING; i++) {
    bool tag1 = (username.Find(_T('«') + testModString[i]) != -1);
    if (modversion.Find(testModString[i]) != -1) {
      float version = STRTOF(modversion.Right(4), NULL);
      if (!tag1 && ((testMinVer[i] == 0.0f) || (version == 9.7f) ||
                    (version >= testMinVer[i])))
        return _T("ModString Thief");
    } else if (tag1)
      return _T("ModString Thief");
  }

  // doubled  «...» in the username, like "username «Xtreme #.#» «abcd»"
  int posr1 = username.Find(_T('»'));
  int posr2 = username.ReverseFind(_T('»'));
  if ((posr1 > 5) && (posr2 - posr1 > 5) &&
      ((username.GetAt(posr1 - 5) == _T('«')) ||
       (username.GetAt(posr2 - 5) == _T('«'))))
    return _T("Bad Username Paddings");
  // zz_fly :: end

  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckMessageSpam(LPCTSTR messagetext) {
  if (messagetext == NULL)
    return NULL;

  if (STRSTR(messagetext, _T("ZamBoR")) ||
      STRSTR(messagetext, _T("DI-Emule")) ||
      STRSTR(messagetext, _T("Join the L33cher")) ||
      STRSTR(messagetext, _T("eMule FX")) ||
      STRSTR(messagetext, _T("---> ed2k://|file|Ketamine")) ||
      STRSTR(messagetext, _T("robot from RIAA, you can't fight")) ||
      STRSTR(messagetext,
             _T("agent from PeerFactor, she advises you to stop")) ||
      STRSTR(messagetext,
             _T("bot from MPAA, you can't do anything against her")) ||
      STRSTR(messagetext, _T("[Sangue-Suga]")) || // 3 /2007
      STRSTR(messagetext, _T("[te@m projekt")) || // 5 /2007
      STRSTR(messagetext,
             _T("eMule PRO Ultra")) ||        // 8/2007 //include ultra 1 2 3
      STRSTR(messagetext, _T("HyperMule")) || // 8/2007
      STRSTR(messagetext, _T("FXeMule")) ||
      STRSTR(messagetext, _T("angelmule.com")) || //**Riso64Bit**
      STRSTR(messagetext, _T("RocketMule"))       //**Riso64Bit**
  )
    return (_T("Spam-Message"));

  if (CString(messagetext).Trim().IsEmpty())
    return (_T("Spam-Message"));

  return NULL;
}

//<<< new tags from eMule 0.04x
enum {
  CT_UNKNOWNx0 = 0x00,  // Hybrid Horde protocol
  CT_UNKNOWNx12 = 0x12, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx13 = 0x13, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx14 = 0x14, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx15 =
      0x15, // http://www.haspepapa-welt.de (DodgeBoards) & DarkMule |eVorte|X|
  CT_UNKNOWNx16 = 0x16, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx17 = 0x17, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx4D = 0x4D, // pimp my mule (00de)
  CT_UNKNOWNxE6 = 0xE6, // http://www.haspepapa-welt.de
  CT_UNKNOWNx22 = 0x22, // DarkMule |eVorte|X|
  CT_UNKNOWNx5D = 0x5D, // md4
  CT_UNKNOWNx63 = 0x63, // ?
  CT_UNKNOWNx64 = 0x64, // ?
  CT_UNKNOWNx69 = 0x69, // eMuleReactor //Xman don't use this, it's webcache!
  CT_UNKNOWNx6B = 0x6B, // md4
  CT_UNKNOWNx6C = 0x6C, // md4
  CT_UNKNOWNx74 = 0x74, // md4
  CT_UNKNOWNx76 = 0x76, // www.donkey2002.to
  CT_UNKNOWNx79 = 0x79, // Bionic
  CT_UNKNOWNx7A = 0x7A, // NewDarkMule
  CT_UNKNOWNx83 = 0x83, // Fusspi
  CT_UNKNOWNx87 = 0x87, // md4
  CT_UNKNOWNx88 = 0x88, // DarkMule v6 |eVorte|X|
  CT_UNKNOWNx8c = 0x8c, // eMule v0.27c [LSD7c]
  CT_UNKNOWNx8d = 0x8d, // unknown Leecher - (client version:60)
  CT_UNKNOWNx94 = 0x94, // 00.de community  //Xman 20.08.05
  CT_UNKNOWNx97 = 0x97, // Emulereactor Community Mod
  CT_UNKNOWNx98 = 0x98, // Emulereactor Community Mod
  CT_UNKNOWNx99 = 0x99, // eMule v0.26d [RAMMSTEIN 8b]
  CT_UNKNOWNx9C = 0x9C, // Emulereactor Community Mod
  CT_UNKNOWNxbb = 0xbb, // emule.de (client version:60)
  CT_UNKNOWNxc4 = 0xc4, // MD5 Community from new bionic - hello
  CT_UNKNOWNxC8 = 0xc8, // MD5 Community from new bionic - hello //Xman x4
  CT_UNKNOWNxCA = 0xCA, // NewDarkMule
  CT_UNKNOWNxCD = 0xCD, // www.donkey2002.to
  CT_UNKNOWNxCE = 0xCE, // FRZ community  //Xman 20.08.05
  CT_UNKNOWNxCF = 0xCF, // FRZ community  //Xman 20.08.05
  CT_UNKNOWNxDA = 0xDA, // Emulereactor Community Mod
  CT_UNKNOWNxEC = 0xec, // SpeedMule and clones	//Xman x4
  CT_UNKNOWNxF0 = 0xF0, // Emulereactor Community Mod
  CT_UNKNOWNxF4 = 0xF4, // Emulereactor Community Mod
  CT_UNKNOWNxD2 = 0xD2, // Chinese Leecher //SquallATF
  // CT_UNKNOWNx85 = 0x85, // viper-israel.org and eChanblardNext  //zz_fly,
  // viper become good
  CT_FRIENDSHARING = 0x66, // eWombat  [SNAFU]
  CT_DARK = 0x54,          // eWombat [SNAFU]
};

LPCTSTR CantiLeech__ DLPCheckHelloTag(UINT tagnumber) {
  LPCTSTR strSnafuTag = NULL;
  switch (tagnumber) {
  case CT_UNKNOWNx12:
  case CT_UNKNOWNx13:
  case CT_UNKNOWNx14:
  case CT_UNKNOWNx16:
  case CT_UNKNOWNx17:
  case CT_UNKNOWNxE6:
    strSnafuTag = apszSnafuTag[0];
    break; // buffer=_T("DodgeBoards");break;
  case CT_UNKNOWNx15:
    strSnafuTag = apszSnafuTag[1];
    break; // buffer=_T("DodgeBoards & DarkMule |eVorte|X|");break;
  case CT_UNKNOWNx22:
    strSnafuTag = apszSnafuTag[2];
    break; // buffer=_T("DarkMule v6 |eVorte|X|");break;
  case CT_UNKNOWNx5D:
  case CT_UNKNOWNx6B:
  case CT_UNKNOWNx6C:
    strSnafuTag = apszSnafuTag[17];
    break;
  case CT_UNKNOWNx74:
  case CT_UNKNOWNx87:
    strSnafuTag = apszSnafuTag[17];
    break;
  case CT_UNKNOWNxF0:
  case CT_UNKNOWNxF4:
    strSnafuTag = apszSnafuTag[17];
    break;
    // case CT_UNKNOWNx69:
    // strSnafuTag=apszSnafuTag[3];break;//buffer=_T("eMuleReactor");break;
  case CT_UNKNOWNx79:
    strSnafuTag = apszSnafuTag[4];
    break; // buffer=_T("Bionic");break;
  case CT_UNKNOWNx83:
    strSnafuTag = apszSnafuTag[15];
    break; // buffer=_T("Fusspi");break;
  case CT_UNKNOWNx76:
  case CT_UNKNOWNxCD:
    strSnafuTag = apszSnafuTag[16];
    break; // buffer=_T("www.donkey2002.to");break;
  case CT_UNKNOWNx88:
    strSnafuTag = apszSnafuTag[5]; //[LSD7c]
    break;
  case CT_UNKNOWNx8c:
    strSnafuTag = apszSnafuTag[5];
    break; // buffer=_T("[LSD7c]");break;
  case CT_UNKNOWNx8d:
    strSnafuTag = apszSnafuTag[6];
    break; // buffer=_T("[0x8d] unknown Leecher - (client
           // version:60)");break;
  case CT_UNKNOWNx99:
    strSnafuTag = apszSnafuTag[7];
    break; // buffer=_T("[RAMMSTEIN]");break; //STRIKE BACK
  case CT_UNKNOWNx97:
  case CT_UNKNOWNx98:
  case CT_UNKNOWNx9C:
  case CT_UNKNOWNxDA:
    strSnafuTag = apszSnafuTag[3];
    break;            // buffer=_T("eMuleReactor");break;
  case CT_UNKNOWNxC8: // Xman x4
  case CT_UNKNOWNxCE: // Xman 20.08.05
  case CT_UNKNOWNxCF: // Xman 20.08.05
  case CT_UNKNOWNx94: // Xman 20.08.05
  case CT_UNKNOWNxc4:
    strSnafuTag = apszSnafuTag[8];
    break; // buffer=_T("[MD5 Community]");break;	//USED BY NEW BIONIC =>
           // 0x12 Sender
  case CT_UNKNOWNxEC:
    strSnafuTag = apszSnafuTag[18];
    break; // Xman x4 Speedmule
    // case CT_FRIENDSHARING: //STRIKE BACK
    // break;
  case CT_DARK: // STRIKE BACK
  case CT_UNKNOWNx7A:
  case CT_UNKNOWNxCA:
    strSnafuTag = apszSnafuTag[9];
    break; // buffer=_T("new DarkMule");
  case CT_UNKNOWNx4D:
    strSnafuTag = apszSnafuTag[19];
    break; // pimp my mule misuse an official tag in hello
  case CT_UNKNOWNxD2:
    strSnafuTag = apszSnafuTag[20];
    break; // SquallATF
           // case CT_UNKNOWNx85:
           //	strSnafuTag=apszSnafuTag[21];break;//zz_fly
  }

  return strSnafuTag;
}

// unknown eMule tags
enum {
  ET_MOD_UNKNOWNx12 = 0x12, // http://www.haspepapa-welt.de
  ET_MOD_UNKNOWNx13 = 0x13, // http://www.haspepapa-welt.de
  ET_MOD_UNKNOWNx14 = 0x14, // http://www.haspepapa-welt.de
  ET_MOD_UNKNOWNx17 = 0x17, // http://www.haspepapa-welt.de
  ET_MOD_UNKNOWNx2F = 0x2F, // eMule v0.30 [OMEGA v.07 Heiko]
  ET_MOD_UNKNOWNx30 = 0x30, // aMule 1.2.0
  ET_MOD_UNKNOWNx36 = 0x36, // eMule v0.26
  ET_MOD_UNKNOWNx3C = 0x3C, // enkeyDev.6 / LamerzChoice 9.9a
  ET_MOD_UNKNOWNx41 =
      0x41, // CrewMod (pre-release mod based on Plus) identification
  ET_MOD_UNKNOWNx42 =
      0x42, // CrewMod (pre-release mod based on Plus) key verification
  ET_MOD_UNKNOWNx43 =
      0x43, // CrewMod (pre-release mod based on Plus) version info
  ET_MOD_UNKNOWNx50 = 0x50, // Bionic 0.20 Beta]
  ET_MOD_UNKNOWNx59 = 0x59, // emule 0.40 / eMule v0.30 [LSD.12e]
  ET_MOD_UNKNOWNx5B = 0x5B, // eMule v0.26
  ET_MOD_UNKNOWNx60 = 0x60, // eMule v0.30a Hunter.6 + eMule v0.26
  ET_MOD_UNKNOWNx64 = 0x64, // LSD.9dT / Athlazan(0.29c)Alpha.3
  ET_MOD_UNKNOWNx76 = 0x76, // http://www.haspepapa-welt.de (DodgeBoards)
  ET_MOD_UNKNOWNx84 = 0x84, // eChanblardv3.2
  ET_MOD_UNKNOWNx85 = 0x85, // ?
  ET_MOD_UNKNOWNx86 = 0x86, // ?
  ET_MOD_UNKNOWNx93 = 0x93, // ?
  ET_MOD_UNKNOWNxA6 = 0xA6, // eMule v0.26
  ET_MOD_UNKNOWNxB1 = 0xB1, // Bionic 0.20 Beta]
  ET_MOD_UNKNOWNxB4 = 0xB4, // Bionic 0.20 Beta]
  ET_MOD_UNKNOWNxC8 = 0xC8, // Bionic 0.20 Beta]
  ET_MOD_UNKNOWNxC9 = 0xC9, // Bionic 0.20 Beta]
  ET_MOD_UNKNOWNxDA = 0xDA, // Rumata (rus)(Plus v1f) - leecher mod?
};

LPCTSTR CantiLeech__ DLPCheckInfoTag(UINT tagnumber) {
  LPCTSTR strSnafuTag = NULL;

  switch (tagnumber) {
  case ET_MOD_UNKNOWNx12:
  case ET_MOD_UNKNOWNx13:
  case ET_MOD_UNKNOWNx14:
  case ET_MOD_UNKNOWNx17:
    strSnafuTag = apszSnafuTag[0];
    break; //("[DodgeBoards]")
  case ET_MOD_UNKNOWNx2F:
    strSnafuTag = apszSnafuTag[10];
    break; // buffer=_T("[OMEGA v.07 Heiko]");break;
  case ET_MOD_UNKNOWNx36:
  case ET_MOD_UNKNOWNx5B:
  case ET_MOD_UNKNOWNxA6:
    strSnafuTag = apszSnafuTag[11];
    break; // buffer=_T("eMule v0.26 Leecher");break;
  case ET_MOD_UNKNOWNx60:
    strSnafuTag = apszSnafuTag[12];
    break; // buffer=_T("[Hunter]");break; //STRIKE BACK
  case ET_MOD_UNKNOWNx76:
    strSnafuTag = apszSnafuTag[0];
    break; // buffer=_T("[DodgeBoards]");break;
  case ET_MOD_UNKNOWNx50:
  case ET_MOD_UNKNOWNxB1:
  case ET_MOD_UNKNOWNxB4:
  case ET_MOD_UNKNOWNxC8:
  case ET_MOD_UNKNOWNxC9:
    strSnafuTag = apszSnafuTag[13];
    break; // buffer=_T("[Bionic 0.20 Beta]");break;
  case ET_MOD_UNKNOWNxDA:
    strSnafuTag = apszSnafuTag[14];
    break; // buffer=_T("[Rumata (rus)(Plus v1f)]");break;
  }

  return strSnafuTag;
}
