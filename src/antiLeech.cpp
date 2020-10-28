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

bool IsTypicalHex(const CString &addon) {
  int n = addon.GetLength();
  if (n < 5 || n > 25)
    return false;

  bool bigalpha = false;
  bool smallalpha = false;
  bool numeric = false;

  // clang-format off
  while (--n) {
    const TCHAR c = addon.GetAt(n);
    /**/ if (c >= _T('0') && c <= _T('9')) numeric = true;
    else if (c >= _T('A') && c <= _T('F')) bigalpha = true;
    else if (c >= _T('a') && c <= _T('f')) smallalpha = true;
    else return false;
  }
  // clang-format on

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

DWORD CantiLeech__ GetDLPVersion() { return DLP_VERSION; }

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
      if (STRECMP(clientversion, _T("eMule v0.(50a|50b|51d|60a)")) != 0) {
        return _T("Old eMule");
      }
    }
    if (STRSTR(clientversion, _T("aMule"))) {
      if (STRECMP(clientversion, _T("aMule v2.3.[12]")) != 0) {
        return _T("Old aMule");
      }
    }
    // TODO
  } else {
    LPCTSTR regex = _T("eMule v0.[56][01][abd] - [A-Za-z]+ v?[0-9.A-Za-z_-]+");
    if (STRECMP(clientversion, regex) != 0) {
      if (STRISTR(clientversion, _T("aMule"))) {
        return _T("Fake aMule");
      }
      return _T("Bad Modstring Scheme");
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

  if (IS_SPACE(modversion)) {
    return _T("Bad MODSTRING");
  }

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

  if (IS_SPACE(username)) {
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
  if (username.IsEmpty() || userhash.IsEmpty()) {
    return NULL;
  }

  static const LPCTSTR community_userhash_list[] = {
      // clang-format off
      _T("154CE646120E96CC798C439A20D26F8D"), // (windows ue)
      _T("455361F9D95C3CD7E6BF2192D1CB3D02"), // (windows ue)
      _T("C8B5F41441C615FBABAD9A7E55294D01"),
      _T("A2221641460E961C8B7FF21A53FB6F6C"), //**Riso64Bit**
      _T("888F4742450EF75F9DD8B7E53FA06FF0"), //**Riso64Bit**
      _T("0B76CC42CB0E81B0DC6120D2BCB36FF9"), //**Riso64Bit**
      _T("EAA383FD9E0E68538C7AC8AD15526F7A"), //**Riso64Bit**
      _T("65C3B2E8940E582630A7F58AF9F26F9E"), // from TaiWan
      _T("9BA09B83DC0EE78BE20280C387936F00"), // from SS1900
      _T("C92859E4860EA0F15F7837750C886FB6"), // from SS1900
      _T("CB42F563EE0EA7907395420CAC146FF5"), // From "qobfxb" multi user
      // clang-format on
  };
  for (int i = 0; i < ARRAY_COUNT(community_userhash_list); ++i) {
    if (STRICMP(userhash, community_userhash_list[i]) == 0)
      return _T("Community Userhash");
  }

  static const LPCTSTR corrupt_userhash_list[] = {
      _T("00000000000E00000000000000006F00"),
      _T("FE000000000E00000000000000006F00"),
      _T("DA1CEEE05B0E5319B3B48CAED24C6F4A"),
  };
  for (int i = 0; i < ARRAY_COUNT(community_userhash_list); ++i) {
    if (STRICMP(userhash, corrupt_userhash_list[i]) == 0)
      return _T("Corrupt UserHash");
  }

  // Check for aedit
  // remark: a unmodded emule can't send a space at last sign
  if (modversion.IsEmpty() && username.Right(1) == _T(" "))
    return _T("AEdit");

  // Check for Hex-Modstring
  if (IsTypicalHex(modversion))
    return _T("Hex-Modstring");

  // community check
  if (username.GetLength() >= 7 && username.Right(1) == _T("]")) {
    // check for special nickaddon
    int find = username.ReverseFind(_T('['));
    if (find >= 0) {
      CString addon = username.Mid(find + 1);
      // int endpos=addon.GetLength()-1;
      if (addon.GetLength() > 2) {
        // Chek for Hex (e.g. X-Treme)
        if (IsTypicalHex(addon.Left(addon.GetLength() - 1)))
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
          return _T("Fake Silver Surfer"); // **Riso64Bit** :: fake silver
                                           // surfer
      }
      // zz_fly :: end
    }
  }

  // thx cyrex
  if (modversion.GetLength() == 10 && username.GetLength() > 4 &&
      STRSTR(username.Right(4), _T("/]")) && STRSTR(username, _T("[SE]"))) {
    return _T("Mystery ModString");
  }

  // Add by SDC team.
#if defined(SPECIAL_DLP_VERSION)
  // clang-format off
  // Some Community UserHash check
  static const LPCTSTR SDC_RefUserHash_list[] = {
    _T("66B002DADE0E6DBEDF4FCCAA380E6FD4"), // From multi user (TW&CN) [DargonD]
    _T("AAEE84C0C30E247CBB99B459255D6F99"), // From NAS_01G multi user [DargonD]
    _T("5E02F74DBA0E8A19DBF6733F0AE66F4A"), // Community UserHash [FzH/DargonD]
    _T("B6491292AE0E07AC8C6045CAC2DD6F9F"), // Community UserHash [FzH/DargonD]
    _T("596B305E050EA842CE38DF3811216F3F"), // Community UserHash [FzH/DargonD]
    _T("B1798B2F620E0B676452C6E2EF706F13"), // Invalid UserHash [DargonD]
    _T("C1533316C00E3E0D0218843A05E46FAC"), // Invalid UserHash [DargonD]
    _T("FE10F3C0610E0A925B85204CE8456F42"), // Invalid UserHash [DargonD]
    _T("C9E61DEEF30E0360E2741C9CF1396F94"), // Invalid UserHash [DargonD]
    _T("559ACC89D80E90C50A7A0CD3224F6F57"), // Invalid UserHash [DargonD]
    _T("6AE1D2DF4B0E8707B6F6BC29E8746F0F"), // Invalid UserHash [DargonD]
    _T("8A537F20B80EF9AF02E59E6C087C6F6B"), // Invalid UserHash [DargonD]
    _T("3F44A7996F0E17D1F4B319EB58B26F64"), // Invalid UserHash [DargonD]
  };
  // clang-forma on
  for (int i = 0; i < ARRAY_COUNT(SDC_RefUserHash_list); ++i) {
    if (STRICMP(userhash, SDC_RefUserHash_list[i]) == 0)
      return _T("[SDC] Community UserHash");
  }

  // clang-forma off
  if (
          // The SDC_RefUserHash_14 with modstring "xl build" will be checked in
          // DLPCheckModstring_Hard function.
          (STRSTR(modversion, _T("xl build")) == NULL &&
           STRICMP(userhash, _T("D0D897BD360EEFF329903E04990B6F86")) == 0) ||
          // The SDC_RefUserHash_15 with NickName "[CHN][VeryCD]QQ" will be checked
          // in DLPCheckUsername_Hard function.
          (STRSTR(username, _T("[CHN][VeryCD]QQ")) == NULL &&
           STRICMP(userhash, _T("36725093E00E9350F7680C871E946FD1")) == 0) ||
          // The SDC_RefUserHash_16 with NickName "[CHN]shaohan" will be checked in
          // DLPCheckUsername_Hard function.
          (STRSTR(username, _T("[CHN]shaohan")) == NULL &&
           STRICMP(userhash, _T("769D36987E0E313A1501967D0F146F7A")) == 0)
      ) {
    return _T("[SDC] Community UserHash");
  }
  // clang-forma on
#endif // SPECIAL_DLP_VERSION

  if (modversion.IsEmpty()) {
    return NULL;
  }
  static const std::pair<LPCTSTR, float> modString_minVer_list[] = {
    {_T("«Xtreme"), 4.4f},
    {_T("«ScarAngel"), 2.5f},
    {_T("«Mephisto"), 1.5f},
    {_T("«MorphXT"), 10.0f},
    {_T("«EastShare"), 13.0f},
    {_T("«StulleMule"), 6.0f},
    // {_T("«Magic Angel"),  3.0f},
    {_T("«DreaMule"), 3.0f},
    {_T("«X-Mod"), 0.0f},
    {_T("«RaJiL"), 2.2f},
  };
  for (int i = 0; i < ARRAY_COUNT(modString_minVer_list); ++i) {
    LPCTSTR tag = STRSTR(username, modString_minVer_list[i].first);
    if (!tag && STRSTR(modversion, modString_minVer_list[i].first)) {
      const float version = STRTOF(modversion.Right(4), NULL);
      if (modString_minVer_list[i].second == 0.0f || version == 9.7f ||
          version >= modString_minVer_list[i].second) {
        return _T("ModString Thief");
      }
    } else if (tag) {
      return _T("ModString Thief");
    }
  }

  {
    // doubled  «...» in the username, like "username «Xtreme #.#» «abcd»"
    LPCTSTR p1, p2;
    if ((p1 = STRSTR(username, _T("«"))) && (p2 = STRSTR(p1 + 1, _T("«")))) {
      if (((p1 = STRSTR(p1 + 1, _T("»"))) && (p1 < p2)) && STRSTR(p2 + 1, _T("»"))) {
        return _T("Bad Username Paddings");
      }
    }
  }

  return NULL;
}

LPCTSTR CantiLeech__ DLPCheckMessageSpam(LPCTSTR messagetext) {
  if (messagetext == NULL) {
    return NULL;
  }

  if (IS_SPACE(messagetext)) {
    return (_T("Spam-Message"));
  }

  static const LPCTSTR spam_message_list[] = {
      _T("ZamBoR"),
      _T("DI-Emule"),
      _T("Join the L33cher"),
      _T("eMule FX"),
      _T("---> ed2k://|file|Ketamine"),
      _T("robot from RIAA, you can't fight"),
      _T("agent from PeerFactor, she advises you to stop"),
      _T("bot from MPAA, you can't do anything against her"),
      _T("[Sangue-Suga]"),   // 3 /2007
      _T("[te@m projekt"),   // 5 /2007
      _T("eMule PRO Ultra"), // 8/2007 //include ultra 1 2 3
      _T("HyperMule"),       // 8/2007
      _T("FXeMule"),
      _T("angelmule.com"), //**Riso64Bit**
      _T("RocketMule"),    //**Riso64Bit**
  };
  for (int i = 0; i < ARRAY_COUNT(spam_message_list); ++i) {
    if (STRCMP(messagetext, spam_message_list[i]) == 0)
      return _T("Spam-Message");
  }

  return NULL;
}

// clang-forma off
//<<< new tags from eMule 0.04x
enum {
  CT_UNKNOWNx0 = 0x00,  // Hybrid Horde protocol
  CT_UNKNOWNx12 = 0x12, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx13 = 0x13, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx14 = 0x14, // http://www.haspepapa-welt.de (DodgeBoards)
  CT_UNKNOWNx15 = 0x15, // http://www.haspepapa-welt.de (DodgeBoards) & DarkMule |eVorte|X|
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
  // CT_UNKNOWNx85 = 0x85, // viper-israel.org and eChanblardNext  //zz_fly, viper become good
  CT_FRIENDSHARING = 0x66, // eWombat  [SNAFU]
  CT_DARK = 0x54,          // eWombat [SNAFU]
};
// clang-forma on

// clang-format off
namespace { namespace p__ {
//>>> eWombat [SNAFU_V3]
static const LPCTSTR apszSnafuTag[] = {
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
  _T("[Chinese Leecher]"), // SquallATF
  // _T("[eChanblardNext]"), // zz_fly
};
} } // namespace{namespace p__{
// clang-format on

LPCTSTR CantiLeech__ DLPCheckHelloTag(UINT tagnumber) {
  using p__::apszSnafuTag;
  LPCTSTR strSnafuTag = NULL;
  // clang-format off
  switch (tagnumber) {
    case CT_UNKNOWNx12:
    case CT_UNKNOWNx13:
    case CT_UNKNOWNx14:
    case CT_UNKNOWNx16:
    case CT_UNKNOWNx17:
    case CT_UNKNOWNxE6: strSnafuTag = apszSnafuTag[0]; break;
    case CT_UNKNOWNx15: strSnafuTag = apszSnafuTag[1]; break;
    case CT_UNKNOWNx22: strSnafuTag = apszSnafuTag[2]; break;
    case CT_UNKNOWNx5D:
    case CT_UNKNOWNx6B:
    case CT_UNKNOWNx6C: strSnafuTag = apszSnafuTag[17]; break;
    case CT_UNKNOWNx74:
    case CT_UNKNOWNx87: strSnafuTag = apszSnafuTag[17]; break;
    case CT_UNKNOWNxF0:
    case CT_UNKNOWNxF4: strSnafuTag = apszSnafuTag[17]; break;
      // case CT_UNKNOWNx69: strSnafuTag=apszSnafuTag[3]; break;
    case CT_UNKNOWNx79: strSnafuTag = apszSnafuTag[4]; break;
    case CT_UNKNOWNx83: strSnafuTag = apszSnafuTag[15]; break;
    case CT_UNKNOWNx76:
    case CT_UNKNOWNxCD: strSnafuTag = apszSnafuTag[16]; break;
    case CT_UNKNOWNx88: strSnafuTag = apszSnafuTag[5]; break;
    case CT_UNKNOWNx8c: strSnafuTag = apszSnafuTag[5]; break;
    case CT_UNKNOWNx8d: strSnafuTag = apszSnafuTag[6]; break;
    case CT_UNKNOWNx99: strSnafuTag = apszSnafuTag[7]; break;
    case CT_UNKNOWNx97:
    case CT_UNKNOWNx98:
    case CT_UNKNOWNx9C:
    case CT_UNKNOWNxDA: strSnafuTag = apszSnafuTag[3]; break;
    case CT_UNKNOWNxC8: // Xman x4
    case CT_UNKNOWNxCE: // Xman 20.08.05
    case CT_UNKNOWNxCF: // Xman 20.08.05
    case CT_UNKNOWNx94: // Xman 20.08.05
    case CT_UNKNOWNxc4: strSnafuTag = apszSnafuTag[8]; break; // USED BY NEW BIONIC => 0x12 Sender
    case CT_UNKNOWNxEC: strSnafuTag = apszSnafuTag[18]; break; // Xman x4 Speedmule
      // case CT_FRIENDSHARING: break;
    case CT_DARK: // STRIKE BACK
    case CT_UNKNOWNx7A:
    case CT_UNKNOWNxCA: strSnafuTag = apszSnafuTag[9]; break;
    case CT_UNKNOWNx4D: strSnafuTag = apszSnafuTag[19]; break; // pimp my mule misuse an official tag in hello
    case CT_UNKNOWNxD2: strSnafuTag = apszSnafuTag[20]; break; // SquallATF
      // case CT_UNKNOWNx85: strSnafuTag=apszSnafuTag[21]; break; //zz_fly
    default: break;
  }
  // clang-format on
  return strSnafuTag;
}

// clang-forma off
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
// clang-forma on

LPCTSTR CantiLeech__ DLPCheckInfoTag(UINT tagnumber) {
  using p__::apszSnafuTag;
  LPCTSTR strSnafuTag = NULL;
  // clang-format off
  switch (tagnumber) {
    case ET_MOD_UNKNOWNx12:
    case ET_MOD_UNKNOWNx13:
    case ET_MOD_UNKNOWNx14:
    case ET_MOD_UNKNOWNx17: strSnafuTag = apszSnafuTag[0]; break; 
    case ET_MOD_UNKNOWNx2F: strSnafuTag = apszSnafuTag[10]; break;
    case ET_MOD_UNKNOWNx36:
    case ET_MOD_UNKNOWNx5B:
    case ET_MOD_UNKNOWNxA6: strSnafuTag = apszSnafuTag[11]; break;
    case ET_MOD_UNKNOWNx60: strSnafuTag = apszSnafuTag[12]; break;
    case ET_MOD_UNKNOWNx76: strSnafuTag = apszSnafuTag[0]; break;
    case ET_MOD_UNKNOWNx50:
    case ET_MOD_UNKNOWNxB1:
    case ET_MOD_UNKNOWNxB4:
    case ET_MOD_UNKNOWNxC8:
    case ET_MOD_UNKNOWNxC9: strSnafuTag = apszSnafuTag[13]; break;
    case ET_MOD_UNKNOWNxDA: strSnafuTag = apszSnafuTag[14]; break;
    default: break;
  }
  // clang-format on
  return strSnafuTag;
}
