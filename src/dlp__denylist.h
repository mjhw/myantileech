// -*- mode: c++; coding: utf-8 -*-
//////////////////////////////////////////////////////////////////////////
// Author:   mjhw<371432590@qq.com>
// License:  GNU GPL
//////////////////////////////////////////////////////////////////////////
#ifndef DLP_PRIVATE__DLP_DENYLIST
#error "Private file"
#endif

#ifndef _T
#define _T(s) DLP_L(s)
#endif

////////////////////////////////////////////////////////////////////////
DLP_BEGIN_NAMESPACE
////////////////////////////////////////////////////////////////////////

#ifndef DLP_WIN32
using namespace win32_data_types;
#endif

static const LPCTSTR DLP_MODVERSION_SOFT_ICASE[] = {
    _T("Rockesel"),
    _T("HARDMULE"),
    _T("Community"),
    _T("IcE-MoD"),
    _T("a-eDit"),
    _T("Ultimativ"),
    _T("Ultimate"),
    _T("Enter MoD Name"),
    _T("La tua Modstring"), // italian
    _T("Your Modstring"),
    _T("C-E-R-E-B-R-O"), // added dlarge
    _T("NewMule"),
    _T("smart- muli"),
    _T("TCMatic 3"), // 1/2007 version 3 is the public version and used as
    // leecher
    _T("uptempo"),
    _T(".COM"), // no domain name in modstring
    _T(".ORG"),
    _T(".NET"),
    _T(".BIZ"),
    _T(".INFO"),
}; // DLP_MODVERSION_SOFT_ICASE

static const LPCTSTR DLP_MODVERSION_HARD_ICASE[] = {
    _T("080509"),      // XL
    _T("20080505"),    // XL
    _T("080620"),      // XL
    _T("080307"),      // XL
    _T("20071122"),    // XL
    _T("20080228"),    // XL
    _T("build 11230"), // XL
    _T("20080923"),    // XL
    _T("v 080828"),    // XL
    _T("XL8828"),      // XL
    _T("xl build"),    // XL
    _T("XunL"),        // XL
    _T("Xthunder"),    // XL
    _T("ZZULL"),       // XL
    _T("XunaLei"),     // XL
    _T("0.49b"),
    _T("0.49c"),
    _T("Addiction"),
    _T("amule"), // fake version, amule never write a modstring here
    _T("Anonymous Mod"),
    _T("AntraX MoD"),
    _T("BigBang"),
    _T("BlueEarth"),
    _T("Bondage"),
    _T("BRAZILINJAPAN"),   // no source
    _T("centraldivx.com"), // no source
    _T("DeSfAlko"),
    _T("Devils Mod"),
    _T("Div eMule"),
    _T("Div pro"),
    _T("Down.co.il"),
    _T("DVD-START.COM"),
    _T("easyMule2"), // protocol bug, lack maintaince, ban
    _T("Ebola"),
    _T("Emule"),
    _T("eMule 0.4"), // some bad mods write clientversion in modstring
    _T("eMuleLife"),
    _T("eMule v"),
    _T("eMuleTorrent"), // GPL-Breaker [冰靈曦曉]
    _T("eMulix"),
    _T("eMule-GIFC"), // GPL-Breaker [DragonD]
    _T("Flux "),
    _T("FreeCD"), // BitComet, changed to hardban
    _T("GangBang"),
    _T("Hiroshima"),
    _T("HyperTraxx"),
    _T("Kamikaze"),
    _T("LoCMuLe"),
    _T("Metha"),
    _T("Methadone"),
    _T("miniMule"), // a compatible client, but without share file option.
    _T("Nagasaki"),
    _T("Neo-R"),
    _T("Neo-RS"),
    _T("NetF WARP 9"), // should be NetF WARP 0.3a.9
    _T("Okaemule"),
    _T("Okamula"),
    _T("Okinawa"),
    _T("OrAnGe"),
    _T("Penthotal"),
    _T("Perestroika"),
    _T("PlayMule"), // PlayMule
    _T("Potenza"),
    _T("PR0 "),     // 0(zero)
    _T("PRO "),     // o
    _T("PROeMule"), // WiZaRd
    _T("pwNd muLe"),
    _T("Pwr eMule"),
    _T("Rastak"),
    _T("Red Projekt"),
    _T("Reptil-Crew-3"),
    _T("RocketMule"),
    _T("Sharinghooligan"),
    _T("Sikombious"),
    _T("SpeedShare"),
    _T("TCMod"),
    _T("The Killer Bean"),
    _T("Thor "),
    _T("Titandonkey"),
    _T("TM0910"),
    _T("UMatic"),
    _T("UMatiX-45a"),
    _T("UniATeam"),
    _T("VMULE"), // israel
    _T("WarezFaw"),
    _T("Wodan"),
    _T("Yotoruma"),
    _T("ZZ-R "),
    _T("ZZ-RS "),
    _T("ZZULtimativ-R"),
    _T("€xORz!§T"),
    _T("00de"),
    _T("00.de"), // WiZaRd
    _T("0FF "),  // 6/2007
    _T("800STER"),
    _T("8OOSTER"),
    _T("Administrator"),
    _T("AeOnFlux"), // 8/2007
    _T("A.i.d.e-A.D.S.L"),
    _T("AIDEADSL"),
    _T("albaR"),
    _T("AngelDr"),   // 5/2006
    _T("Angelmule"), // JvA: no sources, no changelog, community username,...
    _T("Apace"),
    _T("Applejuice"), // 6/2007 now ban instead score reduce
    _T("APRC"),       // MyTh
    _T("Arabella"),
    _T("Asiklar"), // MyTh apple-com
    _T("B00ST"),
    _T("Bad Donkey"), // WiZaRd
    _T("B@d-D3vi7"),
    _T("BlackAngel"),
    _T("BLACKMULE"),
    _T("BlackStar"), // added dlarge
    _T("blackviper"),
    _T("BlueHex"),
    _T("BOO$T"),
    _T("ChímÊrÂ"),
    _T("ÇhïmerÀ"),
    _T("CryptedSpeed"),
    _T("D10T"),           // 12/2006
    _T("DeathAngel"),     // based on Xtreme 8/2006
    _T("Dein Modstring"), // JvA: moved up from soft because also used by
    // Applejuice
    _T("Devil"),
    _T("DSL-Light-Client"), // WiZaRd
    _T("DVD-RS"),
    _T("Dying Angel"),
    _T("easyMule"),
    _T("eChanblard v7.0"),
    _T("Ef-mod 2.0 "), // Xman this mod can be abused as a full leecher
    _T("Elben"),       // WiZaRd
    _T("Element"),
    _T("Elfen"),
    _T("emule.co.il"),
    _T("EPB"),    // MyTh
    _T("Esekci"), // JvA: no sources, no changelog, ...
    _T("Evil Mod"),
    _T("Exorzist"),
    _T("EYE888"),      // compatible client in china, but no src
    _T("Final Fight"), // added dlarge
    _T("Fincan"),
    _T("Fireball"), // added dlarge 'standart String'
    _T("Fire eMule"),
    _T("FlowerPower"),
    _T("FREAK MOD VENOM"),
    _T("FX eMule"),
    _T("FXeMule"),
    _T("Goop.co.il"),
    _T("GPS2Crew"),
    _T("h34r7b34k3r"),
    _T("HARDPAW"),
    _T("Hardstyle"), // MyTh
    _T("Hawkstar"),
    _T("Heartbreaker"),
    _T("HighTime"),
    _T("Hyperdrive"), // 1/2007
    _T("Hypnotix"),
    _T("iberica"), // by briandgwx
    _T("K.O.T."),  // added dlarge
    _T("l!0net"),
    _T("li0net"),
    _T("Licokine"), // added dlarge
    _T("li()net"),
    _T("lionet"),
    _T("L!()Net"),
    _T("L!()Netw0rk"),
    _T("l!onet"),
    _T("L!ONetwork"),
    _T("Love-Angel"),
    _T("LSD"),
    _T("MaGiX"),         // default modstring if activated and unchanged
    _T("maultierpower"), // maultier-power.com sponsorize applejuice
    _T("Merza"),         // added dlarge
    _T("MirageMod"),
    _T("Mørph"),
    _T("MorphJC"),       // bad 'Justice CS' and PBF for incomplete files
    _T("Morph XT"),      // very bad mod (MPAA ?)
    _T("nEwLoGic"),      // added dlarge
    _T("NextEvolution"), // 1/2007
    _T("NFO.Co.iL"),
    _T("No Ratio"),      // based on scarangel 7/2006
    _T("oFF *+*"),       // 10/2006
    _T("OO.de"),         // WiZaRd
    _T("OOde"),          // WiZaRd
    _T("[OO.de-L33CH4"), // Stulle
    _T("OS_"),           // Xman most are found via other checks, but not all
    _T("Peizheng"),      // gpl-breaker
    _T("Pigpen"),
    _T("Pimp"), // 3/2007
    _T("PirateMule"),
    _T("Plus Plus"), // some of them did not banned in bin
    _T("PoWeR MoD"),
    _T("PP-edition"),
    _T("pP.r12b"),  // MyTh
    _T("pP.r8b"),   // 5/2006
    _T("PROemule"), // 9/2006
    _T("rabb_it"),
    _T("rabbit"),
    _T("RapCom"), // added dlarge
    _T("Rappi"),
    _T("Raptor"),
    _T("@RAPTOR"),
    _T("Razorback"),
    _T("RC-ATLANTIS"),
    _T("RIAA"),
    _T("ROCKFORCE"),
    _T("RSVCD"),
    _T("SBI leecher"), // added dlarge
    _T("ServerClient"),
    _T("Shadow"),         // MyTh
    _T("Simple Leecher"), // 9/2006
    _T("Simple Life"),    // MyTh
    _T("SmartMuli"),      // 12/2006
    _T("SPEED EMULE"),    // MyTh
    _T("SpeedLoad"),
    _T("SpeedX"),
    _T("SPIKE2 +"), // MyTh hard leecher
    _T("StulleMule Plus"),
    _T("StulleMule v"), // real modstring is "StulleMule #.#", no 'v'
    _T("SunPower"),     // added dlarge 'standart String'
    _T("SuperKiller"),
    _T("SuperKiller"),     // added dlarge
    _T("the fonz"),        // 12/2006
    _T("T-L-N BO0ST"),     // by briandgwx
    _T("T L N B O O S T"), // by taz-me
    _T("TLN eMule"),
    _T("Tombstone Next"),     // 10/2006
    _T("Tombstone Reloaded"), // 5/2006
    _T("Torenkey"),
    _T("TR-P2P-MoD"),   // JvA: bad client
    _T("TS Next Lite"), // added dlarge
    _T("TurkMule"),     // 1/2007
    _T("Tyrantmule"),   // MyTh
    _T("TYRANUS"),      // MyTh
    _T("UBR-Mod"),
    _T("UltiAnalyzer"),
    _T("Ulti F"),
    _T("UlTiMaTiC "), // based on MA 3.5
    _T("UltiMatiX"),
    _T("UltraFast"), // thl
    _T("Unbuyi"), // a client announced that it is based on a its framework, but
                  // in fact it just copy VeryCD's code //Chengr28
    _T("VeryCD"),
    _T("waZZa"),  // added dlarge
    _T("WebeSo"), // compatible client in china, but no src //Chengr28
    _T("Wikinger"),
    _T("x0Rz!$T"), // E/€xorzist
    _T("X-Cite"),  // added dlarge
    _T("-XDP-"),
    _T("XDP "), // 6/2007
    _T("X-Ray v"),
    _T("Xtreme Xtended"), // Xman 15.08.05
    _T("ZamBoR"),
    _T("ZZULtra"),
}; //  DLP_MODVERSION_HARD_ICASE

static const LPCTSTR DLP_USERNAME_HARD_ICASE[] = {
    _T("00de.de"),
    _T("28881.com"),        // MTVP2P(2013) [雁蝎]
    _T("a1[VeryCD]xthame"), // XL
    _T("adunanza.italiazip.com"),
    _T("AE CoM UseR"),
    _T("Angel eMule"),
    _T("AngelMule"),
    _T("anubisp2p.com"),
    _T("Applejuice"),
    _T("bigbang-emule.de.vu"),
    _T("bigbang.to"),
    _T("blubster.com"),
    _T("BTFaw.Com"),
    _T(".c0.il"),       // 0, zero
    _T(".net «Xtreme"), // eMule -LPE-, Fake ModString
    _T("[Chinfo]"),
    _T("[CHN]__VRom"),  // P2PSearcher, new version [dark]
    _T("[CHN]X_jIQ"),   // P2PSearcher, old version
    _T("[CHN]shaohan"), // Xunlei Offline Download Server and Moblie System Apps
                        // [Glasses 王子]
    _T("[CHN]sf"),      // P2PSearcher, new version
    _T("[ CHN]"),       // a space after bracket
    _T("[CHN][VeryCD]QQ"), // QQDownload
    _T("[CHN][VeryCD][username]"),
    _T("[CHN]yourname"), // Some old chinese leecher and default nickname in
                         // some QQDownload client
    _T("[CHN][VeryCD][Your"),
    _T("cruxp2p.com"),
    _T("CryptMule.de.vu"),
    _T("dbgo.com"),
    _T("devhancer"),
    _T("devhancer.com"),
    _T("[Devils]["), // 2009/12/25
    _T("dianlei.com"),
    _T("Div.Co.il"),
    _T("Div eMule 2007"),
    _T("donkey.com"),
    _T("downloademulegratis.com"),
    _T("download-gratis-emule.com"),
    _T("eDonkey2008"),
    _T("-=EGOist=-"),
    _T("EGOmule"),
    _T("emuIe-co.net"),
    _T("emuIe-project.net"), // phishing site
    _T("emule24horas.com"),
    _T("eMule Accelerator"),
    _T("emulea.com"),
    _T("e-mule.be"),
    _T("[eMuleBT]"),
    _T("emule.cc"),
    _T("emuleclassic.com"),
    _T("emule.co.il"),
    _T("emule.com"),
    _T("e-mule-"),     // detect any mirror simil to "e-mule-it.com"
    _T("[emule.de v"), // default name: [emule.de v ##]
    _T("emuleds.com"),
    _T("emuleech"),
    _T("emuleech.com"),
    _T("emule-element"),
    _T("emule.es"),
    _T("emule-features.6x.to"),
    _T("emulefileswap.com"),
    _T("emule.fr"),
    _T("emulegold.com"),
    _T("emule-gratis.it"),
    _T("emulegratis.net"),
    _T("emuleitaliano.com"),
    _T("emuleitalianogratis.com"),
    _T("emule-italy.it"),
    _T("emule--it.com"),
    _T("emule-mods.biz"),
    _T("emule-mods.cc"), // Xman 01/07
    _T("emule.net"),
    _T("emulenet.de.vu"),
    _T("emule-ng.com"),
    _T("e-mule.nu"),
    _T("emule.org"),
    _T("emuleplus.com"),
    _T("emuleplusplus.de"),
    _T("emulepro.6x.to"),
    _T("emule-pro.blogspot.com"),
    _T("eMulePro.de.vu"),
    _T("emuleproject.com"),
    _T("emule-projet"),
    _T("eMule Pro Ultra"),
    _T("emule-rocket.com"),
    _T("emule.ru"),
    _T("emulesoftware.com"),
    _T("emulesoftware.com"),
    _T("emule-speed"),
    _T("emulespeedup.de.vu"),
    _T("emule.to"),
    _T("emuleultra.com"),
    _T("eMuleUniATeam"),
    _T("emulext.net"),
    _T("e-Sipa"),
    _T("ExtrEMule"),
    _T("fastsearchbooster.biz"),
    _T("fastsearchbooster.biz"),
    _T("FincanMod"), // fincan
    _T("Finc@nMod"),
    _T("finder.co.il"),
    _T("Fireball"),
    _T("Fireb@ll"),
    _T("Flashget"),      // FlashGet
    _T("FUCKLW"),        // added dlarge
    _T("futuremod.de"),  //  JvA: apple-com adress
    _T("futuremods.de"), // Xman 10/06
    _T("futurezone-reloaded"),
    _T("Gate-To-Darkness.com"),
    _T("getfasterp2p.com"),
    _T("Goop.Co.il"), // israel community
    _T("gps2c.6x.to"),
    _T("gps2crew"),
    _T("gratis.emule49-info.com"),
    _T("gratis-emule.com"),
    _T("hermesp2p.com"),
    _T("http://alpha-gaming.net"),
    _T("http://emule.net"),
    _T("http://emule-projekt.net"),
    _T("http://emulo.net"),
    _T("http://projekt.org"),
    _T("http://www.net-xfer.com"), // netxfer
    _T("http://www.official-emule.com"),
    _T("http://yo.com"),
    _T("HubbleKadTracker"), // GPL-Breaker [冰靈曦曉]
    _T("intelpeers.com"),
    _T("Intuition"),
    _T("italian.eazel.com"),
    _T("joop.Co.il"),
    _T("kaggo.com"),
    _T("Ketamine"),
    _T("l!0net"),
    _T("L!0Network"),
    _T("leecher.biz"),        // added dlarge
    _T("leecherclients.org"), // Xman 10/06
    _T("leecher-mod.net"),    // Xman 02/07
    _T("leecher-world.com"),  // added dlarge
    _T("lh.2y.net"),
    _T("LHeMule"),
    _T("lhnet.co.il"),
    _T("li()net"),
    _T("li@network"),
    _T("Li()Network"),
    _T("Li@Network"),
    _T("lionetwork"),
    _T("[lionheart"),
    _T("L!()Network"),
    _T("l!onetwork"),
    _T("!Lou-Nissart!"), // no src only BIN (kick from upload)
    _T("[LSD.19"), // Xman 21.06.2005 definitive not a good mod, with protocol
                   // bugs
    _T("manolito.com"),
    _T("maomao.eu"),
    _T("maultier-power"),
    _T("mega-emule.com"),
    _T("meteorshare.com"),
    _T("mkp2p"),
    _T("mods.sub.cc"),
    _T("MonkeyP2P"),
    _T("monster-mod.com"),
    _T("mp3edonkeysearch.com"),
    _T("mp3rocket.com"),
    _T("MTVP2P"), // community username from Chengr28
    _T("Muli_Checka"),
    _T("net2search.com"),
    _T("netemule.com"),
    _T("nFo.Co.il"),
    _T("nuovaversione.com"),
    _T("OFF+"),
    _T("OFF +"),
    _T("official-emule"),
    _T("p2phood.com"),
    _T("p2psharing.biz"),
    _T("Paf.co.il"),
    _T("phpnuke.org"),
    _T("piolet.com"),
    _T("PlayMule"),
    _T("[PPMule]"),
    _T("pro-sharing.com"),
    _T("pro-sharing.com"),
    _T("pwr.co.il"),
    _T("[Pwr Mule]Usuario"),
    _T("qobfxb"),     // community username
    _T("QQDownload"), // tencent
    _T("r3wlx.com"),
    _T("RAPCOM"),
    _T("@ Raptor"), // added dlarge
    _T("Razorback"),
    _T("RC-ATLANTIS"),
    _T("Reptil-Crew-3"), // Reptil mod
    _T("R-Mod"),
    _T("ROCKFORCE"),
    _T("rsvcd-crew"),
    _T("RSVCD-Forum"),
    _T("scarica-emule-gratis.com"),
    _T("scaricareemule.com"),
    _T("shareghost.com"),
    _T("sharing-devils"), //  leecher community
    _T("speed-downloading.com"),
    _T("speedyp2p.com"),
    _T(".::Stenoco-Zone::."),
    _T("SunPower"),
    _T("super4.com"),
    _T("superemule.6x.to"),
    _T("tangot.com"),
    _T("[TEC]"), // fincan
    _T("Titandonkey"),
    _T("Titanesel.tk"),
    _T("titanload.to"),
    _T("titanmule"),
    _T("TLN eMule"),
    _T("TLNGuest"),
    _T("Torenkey"),
    _T("truxshare.com"),
    _T("TUOTU"),
    _T("turbomule "),
    _T("Ulti-Board"),
    _T("Ultimativ"),
    _T("Ultim@tiv"),
    _T("UnKnOwN pOiSoN"),
    _T("version049c-official.com"),
    _T("VgroupTeam"), // Random ModString [doompower]
    _T("vgo.21cn"),
    _T("VMULE 2007"),
    _T("warezfaw"),
    _T("Wikinger"),
    _T("wikingergilde"),
    _T("wnet.co.il"),
    _T("www.aideadsl.com"),
    _T("Www.D-iL.Net"),
    _T("www.emuleproject.com"),
    _T("www.extremule.com"),
    _T("Www.NFOil.com"),
    _T("xtmhtl [ePlus]"), // same name, same userhash
    _T("ynet.co.il"),
    _T("zultrax.com"),
    _T("ZZ-R "),
    _T("ZZULtimativ-R"),
}; // DLP_USERNAME_HARD_ICASE

static const LPCTSTR DLP_USERNAME_SOFT_ICASE[] = {
    _T(">>Power-Mod"), // Xman 15.08.05
    // zz_fly Start korea
    _T("DONKEY2007"), // korea
    _T("www.Freang.com"), _T("www.pruna.com"), _T("[KOREA]"), _T("superemule"),
    _T("PRUNA 2008"), _T("MOYAM"), _T("eDonkey2009"),
    // zz_fly End
}; // DLP_USERNAME_SOFT_ICASE

static const LPCTSTR DLP_USERHASH_SOFT_ICASE[] = {
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
    _T("00000000000E00000000000000006F00"),
    _T("FE000000000E00000000000000006F00"),
    _T("DA1CEEE05B0E5319B3B48CAED24C6F4A"),
}; // DLP_USERHASH_SOFT_ICASE

////////////////////////////////////////////////////////////////////////
DLP_END_NAMESPACE
////////////////////////////////////////////////////////////////////////
