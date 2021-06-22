// StdAfx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__A3AB3EC1_89CC_45E1_8E3D_F0761B84793D__INCLUDED_)
#define AFX_STDAFX_H__A3AB3EC1_89CC_45E1_8E3D_F0761B84793D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)
//#pragma warning(disable: 4098)

#define STRICT
#ifndef WINVER // Allow use of features specific to Windows 95 and Windows NT 4 or later. 
#define WINVER 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later. 
#endif  



#ifndef _WIN32_WINNT // Allow use of features specific to Windows NT 4 or later. 
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target Windows 98 and Windows 2000 or later. 
#endif  



#ifndef _WIN32_WINDOWS // Allow use of features specific to Windows 98 or later. 
#define _WIN32_WINDOWS 0x0501 // Change this to the appropriate value to target Windows Me or later. 
#endif  



#ifndef _WIN32_IE // Allow use of features specific to IE 4.0 or later. 
#define _WIN32_IE 0x0601 // Change this to the appropriate value to target IE 5.0 or later. 
#endif

// 'DEBUG workaround' below prevents the MFC or ATL #include-s 
// from pulling in "afx.h" that would force the debug CRT through
// #pragma-s.
#if defined(_DEBUG) && !defined(_FULLDEBUG_)
#define _DEBUG_WAS_DEFINED
#undef _DEBUG
#pragma message ("     Compiling MFC header files in release mode.")
#endif


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#import "c:\\Program Files\\Common Files\\system\\ado\\msadox.dll"
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF","adoEOF")
//#import "C:\\Program Files\\Common Files\\System\\ado\\MSJRO.DLL"
#include "icrsint.h"

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>          // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>         // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>         // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// AutoCAD MFC Extensions:
#include "adui.h"
#include "acui.h"
#include "acuiComboBox.h"
#include "acuiDialog.h"
#include <gl\gl.h>		// Header File For The OpenGL32 Library
#include <gl\glu.h>		// Header File For The GLu32 Library
//#include <gl\glaux.h>	// Header File For The GLaux Library

/*
 AutoCAD MFC Extensions:
#include "adui.h"
#include "acui.h"
#include "acuiComboBox.h"
#include "acuiDialog.h"

"*/

//#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF","adoEOF") rename("EOS","adoEOS")
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG
#undef _DEBUG_WAS_DEFINED
#endif

#define VERSION_CAD2016
//#define VERSION_CAD2010
#define X64

//20190709	坡度数组与绘图坐标转换
#define MILE		0	//里程Flag
#define LEVEL		1	//高程Flag
#define TO_DRAW		0	//转换为CAD绘制坐标
#define TO_SLOPE	1	//转换为坡度参数

//20191025	GridCtrl默认行高
#define DEFAULTCOLHEI	30

extern HINSTANCE  _hdllInstance ;
#include "rxdebug.h"
// RX Includes
//#include "acad15.h"
#include "rxdebug.h"
#include "acdb.h"               // acdb definitions
#include "rxregsvc.h"           // ARX linker
#include "dbapserv.h"           // Host application services
#include "aced.h"               // aced stuff
#include "adslib.h"             // RXADS definitions
#include "acdocman.h"           // MDI document manager
#include "rxmfcapi.h"           // ObjectARX MFC support
#include "AcExtensionModule.h"  // Utility class for extension dlls
#include "dbptrar.h"
#include "dbosnap.h"

#include "acadi.h"

#ifdef _FULLDEBUG_
#pragma message ("Warning! You should better not run this application in a Standard AutoCAD release.")
#pragma message ("         Due to the memory managment constraints.")
#endif

#endif // !defined(AFX_STDAFX_H__A3AB3EC1_89CC_45E1_8E3D_F0761B84793D__INCLUDED)
#include "triDTM_road.h"  
#include "ori_scale.h"
#include "JD_CENTER.h"
#include "GTZDM.h"
#include "TUN.h"
#include "GlobalZDMdata.h"
#include <afxcontrolbars.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxdlgs.h>
#include <afxdlgs.h>
#include <afxdlgs.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>


//#define  GY_Road_SYS//改移道路嵌入系统
#define GY_ROAD_DULI_SYS//改移道路独立系统
//#define DIGIROAD//digiroad系统

extern int iSelZdm;
extern triDTM_road ptriDTM;
extern ACHAR SysPath[1024];
extern CString Cworkdir,Pro;
extern ORI_SCALE pORI_SCALE;
extern JD_CENTER *pzLinep;
extern TUN_ROAD *GlobalTun;
extern GlobalZDMData GlobalZDMTK;
extern AcDbObjectId ZpId;//记录当前纵坡rr
struct ReactorEntityPair {
    AcDbObjectId entityId;
    AcDbEntityReactor *pEntReactor;
};
