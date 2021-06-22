//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "DockLocationTool.h"
#include "dirdlgdlg.h"
#include "ZDMStandardDlg.h"
#include "SetCGBZDlg.h"
#include "SetJKNorm.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC(DockLocationTool, CAcUiDockControlBar)

BEGIN_MESSAGE_MAP(DockLocationTool, CAcUiDockControlBar)
	//{{AFX_MSG_MAP(DockLocationTool)
	ON_WM_CREATE()
	ON_COMMAND(ID_MENUITEM_INPUTDBSBYJD, OnMenuitemINPUTDBSBYJD)//从数据库读取
	ON_COMMAND(ID_MENUITEM_INPUTDBSBYXY, OnMenuitemINPUTDBSBYXY)//从数据库读取
	ON_COMMAND(ID_MENUITEM_OUTPUTDBS, OnMenuitemOUTPUTDBS)//保存到数据库
	ON_COMMAND(ID_MENUITEM_DATA, OnMenuitemDATA)//保存到数据库
	
	ON_COMMAND(ID_MENUITEM_LOCATIONTOOL1, OnMenuitemLocationtool1)
	ON_COMMAND(ID_MENUITEM_LOCATIONTOOL2, OnMenuitemLocationtool2)
	ON_COMMAND(ID_MENUITEM_ZJBSCDSJ,OnMenuitemZJBSCD)
	ON_COMMAND(ID_MENUITEM_INPUTJDCEN, OnMenuitemInPutJDCen)
	ON_COMMAND(ID_MENUITEM_INPUTXY, OnMenuitemInPutXY)
	ON_COMMAND(ID_MENUITEM_DRAWPM, OnMenuitemDrawPm)
	ON_COMMAND(ID_MENUITEM_DRAWGRID, OnMenuitemDrawGRID)
	ON_COMMAND(ID_MENUITEM_DRAWZDM, OnMenuitemDrawZdm)	
	ON_COMMAND(ID_MENUITEM_AJUSTAXLE, OnMenuitemAJUSTAXLE)
	ON_COMMAND(ID_MENUITEM_ADDAXLE, OnMenuitemAddAXLE)
	ON_COMMAND(ID_MENUITEM_DELAXLE, OnMenuitemDelAXLE)
	ON_COMMAND(ID_MENUITEM_OUTZDM, OnMenuitemOutZdm)	
	ON_COMMAND(ID_MENUITEM_REVERSE, OnMenuitemReverse)
	ON_COMMAND(ID_MENUITEM_TRANJD, OnMenuitemTranJD)
	ON_COMMAND(ID_MENUITEM_TRANCENTER, OnMenuitemTranCenter)
	ON_COMMAND(ID_MENUITEM_EDITJDCEN, OnMenuitemEditJD)
	ON_COMMAND(ID_MENUITEM_LINKXL, OnMenuitemLINKXL)
	ON_COMMAND(ID_MENUITEM_ZXYTOXL, OnMenuitemZXYTOXL)//主线元转线路
	ON_COMMAND(ID_MENUITEM_NEWXM, OnMenuitemNEWXM)//新建
	ON_COMMAND(ID_MENUITEM_BDXM, OnMenuitemBDXM)//标定
	ON_COMMAND(ID_MENUITEM_ZQZJB, OnMenuitemZQZJB)//直曲转角表
	ON_COMMAND(ID_MENUITEM_ZDQXB, OnMenuitemZDQXB)//匝道曲线表	
	ON_COMMAND(ID_MENUITEM_ZDZDZBB, OnMenuitemZDZDZBB)//匝道主点坐标表
	ON_COMMAND(ID_MENUITEM_ZDZZZBB, OnMenuitemZDZZZBB)//匝道主桩坐标表
	ON_COMMAND(ID_MENUITEM_ADDDL, OnMenuitemADDDL)//增加断链
	ON_COMMAND(ID_MENUITEM_DELDL, OnMenuitemDELDL)//删除断链
	ON_COMMAND(ID_MENUITEM_INPUTEXJDCEN, OnMenuitemINPUTEXJDCEN)//录入二线平面数据
	ON_COMMAND(ID_MENUITEM_QIECHUIXIAN, OnMenuitemQIECHUIXIAN)//切线垂线工具
	ON_COMMAND(ID_MENUITEM_LIANXUDINGXIAN, OnMenuitemLIANXUDINGXIAN)//连续定线
	ON_COMMAND(ID_MENUITEM_DELFLD, OnMenuitemDELFLD)//"删除分离段"	
	ON_COMMAND(ID_MENUITEM_EDITDL, OnMenuitemEDITDL)//编辑断链
	ON_COMMAND(ID_MENUITEM_ZMEDITDL, OnMenuitemZMEDITDL)//编辑断链
	ON_COMMAND(ID_MENUITEM_JDTDFS, OnMenuitemJDTDFS)//设置交点拖动方式
	ON_COMMAND(ID_MENUITEM_AUTOTK, OnMenuitemAUTOTK)//自动布图框
	ON_COMMAND(ID_MENUITEM_OUTPM, OnMenuitemOUTPM)//分幅输出平面图
	ON_COMMAND(ID_MENUITEM_CBQXYS, OnMenuitemDrawCBQXYS)//曲线要素图
	ON_COMMAND(ID_MENUITEM_HXYYSB, OnMenuitemDrawXYYSB)//线元要数图
	ON_COMMAND(ID_MENUITEM_DBDRAWZX,OnMenuitemDrawDBZX)//绘端部中线
    ON_COMMAND(ID_MENUITEM_DBDRAWBD, OnMenuitemDrawDBBDCIR)
	ON_COMMAND(ID_MENUITEM_DBDRAWHX, OnMenuitemDrawDBHX)
	ON_COMMAND(ID_MENUITEM_DBBZ, OnMenuitemDBBZ)
	ON_COMMAND(ID_MENUITEM_XGDB, OnMenuitemXGDB)

	ON_COMMAND(ID_MENUITEM_PMTSET, OnMenuitemPMTSET)//参数设置
	ON_COMMAND(ID_MENUITEM_LMKD, OnMenuitemLMKD)//add
	ON_COMMAND(ID_MENUITEM_DRAWLMX, OnMenuitemLMX)//绘路面线/add
	ON_COMMAND(ID_MENUITEM_DRAWPZX, OnMenuitemPZX)
	ON_COMMAND(ID_MENUITEM_GETLMKTOFILE, OnMenuitemGetLmkToFile)//add
	ON_COMMAND(ID_MENUITEM_EDITLMK, OnMenuitemEditLmk)//add
	ON_COMMAND(ID_MENUITEM_PPCD, OnMenuitemPPCD)//爬坡车道
	ON_COMMAND(ID_MENUITEM_CALBD,OnMenuitemCalBD)//计算鼻端
	ON_COMMAND(ID_MENUITEM_OUTHDM, OnMenuitemOutHdm)	
	ON_COMMAND(ID_MENUITEM_DRAWHDM, OnMenuitemDrawHdm)	
	ON_COMMAND(ID_MENUITEM_JMBX,OnMenuitemJMBX)
	ON_COMMAND(ID_MENUITEM_ZDCT,OnMenuitemZDCT)
//建立数模
    ON_COMMAND(ID_MENUITEMtqsj, OnMENUITEMtqsj)
    ON_COMMAND(ID_MENUITEMdealdata, OnMENUITEMdealdata)
    ON_COMMAND(ID_MENUITEMcreateTin, OnMENUITEMcreateTin)
    ON_COMMAND(ID_MENUITEMdisplay, OnMENUITEMdisplay)
//纵断面
	ON_COMMAND(ID_MENUITEMZDMFA, OnMenuitemzdmfa)//倒入纵断面方案
    ON_COMMAND(ID_MENUITEM_PDX, OnMenuitemPdx)//直线转纵断面设计线
	ON_COMMAND(ID_MENUITEMzmzdpmfa, OnMENUITEMzmzdpmfa)//指定平面方案
	ON_COMMAND(ID_MENUITEM_DMX, OnMenuitemDMX)//点绘地面线
	ON_COMMAND(ID_MENUITEM_BXDMX,OnMenuitemBXDMX)//点绘地面线
	ON_COMMAND(ID_MENUITEM_INBXDMX,OnMenuitemINBXDMX)
	ON_COMMAND(ID_MENUITEM_SAVEBXDMX,OnMenuitemSAVEBXDMX)//保存边线地面线
    ON_COMMAND(ID_MENUITEM_ZDJP, OnMenuitemZDJP)//楂道接坡
	ON_COMMAND(ID_MENUITEMKZBG, OnMenuitemKZBG)//绘控制标高
	ON_COMMAND(ID_MENUITEMsetZmCs, OnMENUITEMsetZmCs)//设定纵面参数
	ON_COMMAND(ID_MENUITEMsetSJBZ, OnMENUITEMsetZMSJBZ)//设定设计标准
	ON_COMMAND(ID_MENUITEM_LPSJ, OnMenuitemLpsj)//拉坡设计
	ON_COMMAND(ID_MENUITEM_MdyPd, OnMENUITEMMdyPd)//调整坡度
	ON_COMMAND(ID_MENUITEM_ZPSJB, OnMenuitemZpsjb)//编辑坡度
	ON_COMMAND(ID_MENUITEM_addBpd, OnMENUITEMaddBpd)
	ON_COMMAND(ID_MENUITEM_DelBpd, OnMENUITEMDelBpd)
	ON_COMMAND(ID_MENUITEM_LinkBD, OnMENUITEMLinkBD)//坡段连接
	ON_COMMAND(ID_MENUITEMSAVEZDM, OnMenuitemsavezdm)//保存纵面方案
	ON_COMMAND(ID_MENUITEM_CalBri, OnMENUITEMCalBri)//自动设桥
	ON_COMMAND(ID_MENUITEM_DelBri, OnMENUITEMDelBri)
	ON_COMMAND(ID_MENUITEM_AddBri, OnMENUITEMAddBri)
	ON_COMMAND(ID_MENUITEM_EditBri, OnMENUITEMEditBri)
	ON_COMMAND(ID_MENUITEM_InputBri, OnMENUITEMInputBri)
	ON_COMMAND(ID_MENUITEM_PickDmx, OnMENUITEMPickDmx)//屏幕采集地面线
	ON_COMMAND(ID_MENUITEMEditDMD, OnMENUITEMEditDMD)//编辑地面点
	
	ON_COMMAND(ID_MENUITEM_CalTun, OnMENUITEMCalTun)	
	ON_COMMAND(ID_MENUITEM_Deltun, OnMENUITEMDeltun)
	ON_COMMAND(ID_MENUITEM_addtun, OnMENUITEMaddtun)
	ON_COMMAND(ID_MENUITEM_EditTun, OnMENUITEMEditTun)
	ON_COMMAND(ID_MENUITEM_InputTun, OnMENUITEMInputTun)

	ON_COMMAND(ID_MENUITEM_SETCGBZ, OnMENUITEMSETCGBZ)//设置超高标准
	ON_COMMAND(ID_MENUITEM_SETJKBZ, OnMENUITEMSETJKBZ)//设置加宽标准	
	ON_COMMAND(ID_MENUITEM_CALCGFD, OnMENUITEMCALCGFD)//计算超高分段
	ON_COMMAND(ID_MENUITEM_CXCGFD, OnMENUITEMCXCGFD)//查询超高分段

	//横断面设计
	ON_COMMAND(ID_MENUITEMsetHmCs, OnMENUITEMsetHmCs)//设定横断面参数
	ON_COMMAND(ID_MENUITEM_CalSta, OnMENUITEMCalSta)//计算桩号序列
	ON_COMMAND(ID_MENUITEM_CalNose, OnMENUITEMCalNose)//计算桩号序列
	
	ON_COMMAND(ID_MENUITEM_GetDmx, OnMENUITEMGetDmx)//提取地面线
	ON_COMMAND(ID_MENUITEM_HdmAutoDes, OnMENUITEMHdmAutoDes)//横断面自动设计
	ON_COMMAND(ID_MENUITEM_InputHdm, OnMENUITEMInputHdm)//导入横断面
	ON_COMMAND(ID_MENUITEM_ChangBP, OnMENUITEMChangeBp)//导入横断面
	ON_COMMAND(ID_MENUITEM_SAVEHDM, OnMENUITEMSAVEHDM)//导入横断面
	ON_COMMAND(ID_MENUITEM_GOUPD, OnMENUITEMGOUPD)//导入横断面
		
	//用地设计	
	ON_COMMAND(ID_MENUITEM_InputYD, OnMENUITEMInputYD)
	ON_COMMAND(ID_MENUITEM_DelYDZ, OnMENUITEMDelYDZ)
	ON_COMMAND(ID_MENUITEM_ADDYDZ, OnMenuitemAddydz)
	ON_COMMAND(ID_MENUITEM_EDITYDDATA, OnMenuitemEdityddata)
	ON_COMMAND(ID_MENUITEM_CHANGEYDZ, OnMenuitemChangeydz)
	ON_COMMAND(ID_MENUITEM_SAVEYDDATA, OnMenuitemSaveyddata)
//车速检测
    ON_COMMAND(ID_MENUITEMjscs, OnMENUITEMjscs)
    ON_COMMAND(ID_MENUITEMcsct, OnMENUITEMcsct)
    ON_COMMAND(ID_MENUITEMcscb, OnMENUITEMcscb)
    

    //表格输出
    ON_COMMAND(ID_MENUITEM_ZXQXZJB, OnMenuitemZXQXZJB)
	ON_COMMAND(ID_MENUITEM_ZZZBB, OnMenuitemZZZBB)
	ON_COMMAND(ID_MENUITEM_ZPSQXB, OnMenuitemZPSQXB)
	ON_COMMAND(ID_MENUITEM_ZZYDZBB, OnMenuitemZZYDZBB)
	ON_COMMAND(ID_MENUITEM_TSFJSB, OnMenuitemTSFJSB)
	ON_COMMAND(ID_MENUITEM_GCSLB, OnMenuitemGCSLB)
	ON_COMMAND(ID_MENUITEM_DYT, OnMenuitemDYT)
	ON_COMMAND(ID_MENUITEM_MGLTSFB, OnMenuitemMGLTSFB)
	ON_COMMAND(ID_MENUITEM_LXTWSJB, OnMenuitemLXTWSJB)
	ON_COMMAND(ID_MENUITEM_LJSJB, OnMenuitemLJSJB)
	ON_COMMAND(ID_MENUITEM_BGPSG, OnMenuitemBGPSG)
	ON_COMMAND(ID_MENUITEM_QBTTAB, OnMenuitemQBTTAB)
	ON_COMMAND(ID_MENUITEM_WYNTAB, OnMenuitemWYNTAB)
	
//三维可视
	ON_COMMAND(ID_MENUITEMdx3d, OnMenuitemDX3D)
	ON_COMMAND(ID_MENUITEMlx3d, OnMenuitemLX3D)
	ON_COMMAND(ID_MENUITEM3DFrameBrowser,OnMenuitemView3D)
	ON_COMMAND(ID_MENUITEM3DPerspective,OnMenuitemPerspective)

//大临工程
ON_COMMAND(ID_MENUITEM_INITLW, OnMenuitemDX3D)
ON_COMMAND(ID_MENUITEM_SYSMLW, OnMenuitemLX3D)
ON_COMMAND(ID_MENUITEM_ZTOPLW, OnMenuitemView3D)

//辅助工具
    ON_COMMAND(ID_MENUITEM_PMGUIDE, OnMenuitemPmGuide)
    ON_COMMAND(ID_MENUITEM_DELAYER, OnMenuitemDELAYER)
	ON_COMMAND(ID_MENUITEM_RUBBER, OnMenuitemRUBBER)
	ON_COMMAND(ID_MENUITEM_LCZZB, OnMenuitemLczzb)
	ON_COMMAND(ID_MENUITEM_ZBZLC, OnMenuitemZbzlc)
	ON_COMMAND(ID_MENUITEM_CXLMBG, OnMenuitemCxlmbg)
	ON_COMMAND(ID_MENUITEM_CXYDAREA,OnMenuitemCxYdArea)
	ON_COMMAND(ID_MENUITEM_FACHECK,OnMenuitemProCheck)
	ON_COMMAND(ID_MENUITEM_XLJD, OnMenuitemXLJD)
	ON_COMMAND(ID_MENUITEM_RIBBON, OnMenuitemRIBBON)
	ON_COMMAND(ID_MENUITEM_TOOL, OnMenuitemTOOL)
//帮助
    ON_COMMAND(ID_MENUITEM_HELP, OnMenuitemHELP)
	
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
// DockLocationTool *pInstance =new DockLocationTool ;
// pInstance->Create (acedGetAcadFrame (),L"My title bar") ;
// pInstance->EnableDocking (CBRS_ALIGN_ANY) ;
// pInstance->RestoreControlBar () ;

static CLSID clsDockLocationTool ={ 0xefb18527, 0x192c, 0x41c2, { 0x9f, 0x68, 0x21, 0x93, 0x8f, 0x4d, 0x1f, 0xfe } } ;

//-----------------------------------------------------------------------------
DockLocationTool::DockLocationTool() : CAcUiDockControlBar () {
	//{{AFX_DATA_INIT(DockLocationTool)
	//}}AFX_DATA_INIT
	m_MODEL=ISLOCATION;
}

BOOL DockLocationTool::Create(CWnd*pParent, LPCTSTR lpszTitle)
{
	CRect rect (0, 0, 200 , 25) ;
	if(m_MODEL==ISLOCATION)rect.SetRect(0,0,95,350);

	CString strWndClass ;
	strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;
	if (!CAcUiDockControlBar::Create (
		strWndClass,
		lpszTitle,
		WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN,
		rect,
		pParent, 0))return (FALSE) ;

	SetToolID (&clsDockLocationTool) ;



	//CString strWndClass ;
	//strWndClass =AfxRegisterWndClass (CS_DBLCLKS, LoadCursor (NULL, IDC_ARROW)) ;

	//CRect rect (0, 0, 100, 20) ;
	//if ( !CAcUiDockControlBar::Create (
	//	strWndClass,
	//	lpszTitle,
	//	WS_VISIBLE |WS_CHILD |  WS_CLIPCHILDREN ,
	//	rect,
	//	pParent, 0
	//	)
	//	)
	//	return (FALSE) ;
	///*SetBkColor(GetDC()->m_hDC,RGB(0,255,255));*/
	//SetToolID (&clsDockLocationTool) ;

	////----- TODO: Add your code here

	//return (TRUE) ;

	//----- TODO: Add your code here
	
	return (TRUE) ;
}

int DockLocationTool::OnCreate (LPCREATESTRUCT lpCreateStruct) {
	if ( CAcUiDockControlBar::OnCreate (lpCreateStruct) == -1 )
		return (-1) ;

	//----- TODO: Add your code here
	CAcModuleResourceOverride resourceOverride;	
	CRect lpRect;
	this->GetWindowRect(lpRect);
	if(m_MODEL==ISLOCATION)
	{
//		lpRect.left+=4;
//		lpRect.top+=6;
//		LnDlg.Create(IDD_DIALOG_LOCALION1,this);
//		LnDlg.SetWindowPos(NULL,lpRect.left+4,lpRect.top+6,85,400,SWP_SHOWWINDOW);
//		LnDlg.ShowWindow(TRUE);
	}
	else if(m_MODEL==ISMENU) 
	{
		if (!m_wndMenuBar.Create(WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST | CCS_NODIVIDER | CCS_NORESIZE|CBRS_SIZE_FIXED, 
			CRect(0,0,1,1), this , 892535))
			return -1;
		m_wndMenuBar.LoadMenu(CreateMenuXP());
	}
	return (0) ;
}

void DockLocationTool::SizeChanged (CRect *lpRect, BOOL bFloating, int flags)
{
	//----- TODO: Add your code here

	if(m_MODEL==ISLOCATION)
	{
/*
		lpRect->right=lpRect->left+85;
		MoveWindow(lpRect);
*/
//		LnDlg.SetWindowPos(NULL,lpRect->left+4,lpRect->top+6,80,400,SWP_SHOWWINDOW);
	}
	else if(m_MODEL==ISMENU)
	{
//		lpRect->InflateRect(-6,-6);
		m_wndMenuBar.SetWindowPos(NULL,lpRect->left+4,lpRect->top+5,lpRect->Width(),20,SWP_SHOWWINDOW);
	}
}

CMenuXP* DockLocationTool::CreateMenuXP()
{
	CMenuXP	*pMenu = new CMenuXP;
		
	//You can unrecomment the next two line to load a normal menu
	// XPMenu
	pMenu->CreatePopupMenu();
	
	CMenuXP	*pPopup,*pPopup2;

	pPopup = new CMenuXP;
	pPopup->CreatePopupMenu();
	pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
	pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"项目管理"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_NEWXM,L"新建项目"));
	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BDXM,L"打开项目"));
	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DATA,L"数据管理"));
	
	pPopup = new CMenuXP;
	pPopup->CreatePopupMenu();
	pPopup->AddSideBar(new CMenuXPSideBar(24,L"DGRoad V7.0"));
	pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"建立数模")); 
     	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMtqsj,L"提取数据"));	
     	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	    pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdealdata,L"处理数据"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
	    pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcreateTin,L"建立数模"));
	    pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMChangeTin, L"切换数模"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
	    pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdisplay,L"显示数模"));	
	
	
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"平面设计"));
		  pPopup2 = new CMenuXP;
		  pPopup2->CreatePopupMenu();
		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYJD,L"导入方案"));
		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTJDCEN,L"录入查询"));
//		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN,L"查询数据"));
	      pPopup2->AppendODMenu(0, new CMenuXPSeparator());	
	      pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL,L"实体转换"));		  
		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LIANXUDINGXIAN,L"连续布线"));
		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL2,L"离散布线"));	
		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_REVERSE,L"线路反向"));
		  pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LINKXL,L"线路连接"));		  					  
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"交点模式 "));  

   	    pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
		pPopup2->CreatePopupMenu();
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTDBSBYXY,L"导入方案"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INPUTXY,L"录入查询"));
//		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITJDCEN,L"查询数据"));
	    pPopup2->AppendODMenu(0, new CMenuXPSeparator());		
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZJBSCDSJ,L"直接式变速车道"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LOCATIONTOOL1,L"模式布线"));
        pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_JMBX,L"积木布线"));
		pPopup2->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXYTOXL,L"实体转换"));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TRANJD,L"线元转交点"));
		pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"线元模式 "));   
   	    pPopup->AppendODMenu(0, new CMenuXPSeparator());		

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITDL,L"编辑断链"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDDL,L"增加断链"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELDL,L"删除断链"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());	
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPUTDBS,L"保存方案"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());			

		//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TRANCENTER,L"转成圆心数据"));		
		
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LMKD,L"路面宽度"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWLMX,L"绘路面线"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPZX,L"绘偏置线"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GETLMKTOFILE,L"提取路面宽"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITLMK,L"编辑路面宽"));
      //  pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PPCD,L"爬坡车道"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CALBD,L"鼻端计算"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup2 = new CMenuXP;
	    pPopup2->CreatePopupMenu();
	    pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SETCGBZ,L"设置超高标准 "));
		pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SETJKBZ,L"设置加宽标准 "));
	    pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CALCGFD,L"计算超高加宽分段 "));
	    pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXCGFD,L"查询超高加宽分段 "));
	    pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"计算超高加宽 "));   
   	    pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_QIECHUIXIAN,L"绘切垂线"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_JDTDFS,L"拖动设置"));//设置交点拖动方式
		//szf
	pPopup = new CMenuXP;
	pPopup->CreatePopupMenu();
	pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
	pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"纵断面设计"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetZmCs,L"基本参数 "));
	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMzmzdpmfa,L"选择平面 "));
    pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetSJBZ,L"设计标准 "));
	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMZDMFA,L"导入纵面 "));	
	pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
    pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZMEDITDL,L"编辑断链"));
    pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup2 = new CMenuXP;
	pPopup2->CreatePopupMenu();
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PickDmx,L"屏幕采集 "));
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DMX,L"数模内插 "));
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD,L"列表编辑 "));
	pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"地面线 "));
	
	pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
	pPopup2 = new CMenuXP;
	pPopup2->CreatePopupMenu();
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_INBXDMX,L"数据库导入 "));
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BXDMX,L"内插地面线 "));
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEMEditDMD,L"列表编辑 "));
	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEBXDMX,L"存入数据库 "));
	pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"边线地面线 "));
	
   	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDJP,L"匝道接坡 "));	
	pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
	
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMKZBG,L"绘控制标高 "));	
	pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
	   pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PDX,L"直线转坡度线 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LPSJ,L"拉坡设计 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_MdyPd,L"调整坡度 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZPSJB,L"编辑坡度 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addBpd,L"增加变坡点 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBpd,L"删除变坡点 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LinkBD,L"坡段连接 "));
//	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
//	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZMcheck,L"违规检测"));

	   pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"坡度设计"));   
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
   	   pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalBri,L"自动设桥 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputBri,L"导入桥梁 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelBri,L"删除桥梁 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AddBri,L"增加桥梁 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditBri,L"列表编辑 "));
	   pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"桥梁设置"));   
	   pPopup->AppendODMenu(0, new CMenuXPSeparator()); 
	   pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalTun,L"自动设隧 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputTun,L"导入隧道 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_Deltun,L"删除隧道 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_addtun,L"增加隧道 "));
	   pPopup2->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EditTun,L"列表编辑 "));
	   pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"隧道设置"));  
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMSAVEZDM,L"保存方案"));

	   pPopup = new CMenuXP;
	   pPopup->CreatePopupMenu();
	   pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
	   pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"横断面设计"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMsetHmCs,L"基本参数 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CalSta,L"桩号序列 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GetDmx,L"提取地面线 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HdmAutoDes,L"自动设计 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputHdm,L"导入横断面 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ChangBP,L"修改边坡 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GOUPD,L"沟底纵坡 "));
       pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEHDM,L"保存方案 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   	   
	   pPopup = new CMenuXP;
	   pPopup->CreatePopupMenu();
	   pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
	   pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"用地设计"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_InputYD,L"导入用地数据 "));
/*
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DelYDZ,L"删除用地桩 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDYDZ,L"增加用地桩 "));*/

	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_EDITYDDATA,L"编辑用地数据 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CHANGEYDZ,L"修改用地桩 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_SAVEYDDATA,L"保存用地数据 "));
	   pPopup->AppendODMenu(0, new CMenuXPSeparator());
//szf
/*
	pPopup = new CMenuXP;
	pPopup->CreatePopupMenu();
	pPopup->AddSideBar(new CMenuXPSideBar(20,L"  平  面  出  图  "));
	pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"平面出图"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"选择线路"));
	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"绘线位"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"标注里程及断链"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"标注主点里程"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"绘交点连线"));
	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"自动布图框"));
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"分幅输出平面图"));	
*/
	
	pPopup = new CMenuXP;
	pPopup->CreatePopupMenu();
	pPopup->AddSideBar(new CMenuXPSideBar(24,L"DGRoad V7.0"));
	pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"车速检测")); 
     	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMjscs,L"计算车速"));	
     	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	    pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcsct,L"车速出图"));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
	    pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMcscb,L"车速出表"));


	pPopup = new CMenuXP;
	pPopup->CreatePopupMenu();
	pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
	pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"图形输出"));
//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMTSET,L"参数设置"));
//	pPopup->AppendODMenu(0, new CMenuXPSeparator());
     pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWPM,L"绘平面图"));
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWGRID,L"绘经纬网格"));
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CBQXYS,L"绘曲线要素"));
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HXYYSB,L"绘线元要素表"));
       pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTPM,L"分幅输出"));	
    pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"平面图"));   
	pPopup->AppendODMenu(0, new CMenuXPSeparator());

     pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBDRAWZX,L"绘中线"));
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBDRAWBD,L"绘鼻端圆"));
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBDRAWHX,L"绘横线"));
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DBBZ,L"端部标注"));   
	   pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_XGDB,L"修改端部"));    
    pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(1,L"端部数据图"));   
	pPopup->AppendODMenu(0, new CMenuXPSeparator());

	 pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	  	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWZDM,L"绘纵面图"));
    	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_AJUSTAXLE,L"调整断高"));
    	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ADDAXLE,L"增加断轴"));
    	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAXLE,L"删除断轴"));
    	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTZDM,L"输出纵面图"));
    pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(2,L"纵断面图"));   
	pPopup->AppendODMenu(0, new CMenuXPSeparator());

	pPopup2 = new CMenuXP;
	   pPopup2->CreatePopupMenu();
	  	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DRAWHDM,L"绘横断面图"));
    	pPopup2->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_OUTHDM,L"输出横断面图"));
    pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(3,L"横断面图")); 

	pPopup->AppendODMenu(0, new CMenuXPSeparator());
	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDCT,L"一键式自动成图"));
//	pPopup->AppendODMenu(0, new CMenuXPSeparator());

	pPopup = new CMenuXP;
	  pPopup->CreatePopupMenu();
	  pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
	  pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"表格输出 "));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZXQXZJB,L"直线曲线转角表"));
       pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZZZBB,L"逐桩坐标表"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZPSQXB,L"纵坡竖曲线表"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZZYDZBB, L"逐桩用地坐标表"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TSFJSB, L"土石方数量计算表"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_GCSLB, L"工程数量表"));
	   pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DYT, L"路基大样图"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_MGLTSFB,L"每公里土石方数量表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LXTWSJB,L"路线填挖数据表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LJSJB,L"路基设计表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_BGPSG,L"边沟排水沟设计表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_QBTTAB,L"清表数量表"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_WYNTAB,L"挖淤数量表"));
 //   pPopup->AppendODPopup(0, pPopup2, new CMenuXPText(4,L"表格输出"));   
//	pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"三维可视 ")); 
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMdx3d,L"三维模型拼合计算 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEMlx3d,L"CAD线路三维建模 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM3DFrameBrowser,L"实时交互漫游 "));	
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM3DPerspective,L"生成透视图 "));	
		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AddSideBar(new CMenuXPSideBar(20,L"DGRoad V7.0"));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"辅助工具"));
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_LCZZB,L"里程转坐标 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZBZLC,L"坐标转里程 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXLMBG,L"查询路面标高 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_CXYDAREA,L"查询用地面积 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_FACHECK,L"方案审查 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_PMGUIDE,L"平面导航 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_XLJD,L"线路求交 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_DELAYER,L"删除一层 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RUBBER,L"橡皮擦 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_RUBBER, L"打开Ribbon "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());

		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_TOOL,L"工具条 "));
		pPopup->AppendODMenu(0, new CMenuXPSeparator());


		pPopup = new CMenuXP;
		pPopup->CreatePopupMenu();
		pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_HELP,L"搜索..."));
		pMenu->AppendODPopup(0, pPopup, new CMenuXPText(1,L"帮助"));
	
//	pPopup->AppendODMenu(0, new CMenuXPSeparator());
//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZQZJB,L"直曲转角表"));
//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDQXB,L"匝道曲线表"));
//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDZDZBB,L"匝道主点坐标表"));
//	pPopup->AppendODMenu(0, new CMenuXPText(ID_MENUITEM_ZDZZZBB,L"匝道主桩坐标表"));
	
// 	pPopup->SetBackColor(RGB(0xf2, 0xf2, 0xf2));
//	pPopup->SetSelectedBarColor(RGB(0x00, 0xcc, 0xcc));
//	pPopup->SetIconAreaColor(RGB(0xff, 0x99, 0x00));
//	pPopup->SetMenuStyle(CMenuXP::STYLE_XP);	
	return pMenu;	
}


bool DockLocationTool::CanFrameworkTakeFocus()
{
	if (GetKeepFocus())
    {
        return false;
    }	
    return CAcUiDockControlBar::CanFrameworkTakeFocus();
}


BOOL DockLocationTool::AddCustomMenuItems(LPARAM hMenu)
{
	HMENU hPopMenu;
	hPopMenu=(HMENU)(hMenu&0xffff);
	CMenu* pop_menu = CMenu::FromHandle(hPopMenu);
	
	CPoint pt(::GetMessagePos());
	//	::ScreenToClient(m_wndTree.m_hWnd,&pt);
	
	
	//UINT nFlags;
	//HTREEITEM hitem = m_wndTree.HitTest( pt, &nFlags );
	
	//	if( nFlags & TVHT_NOWHERE )
	{
		//		pop_menu->AppendMenu(MF_SEPARATOR);
		//		pop_menu->AppendMenu(MF_STRING,ID_ADD_ITEM,L"ADD");
	}
	//	else if( nFlags & TVHT_ONITEM )
	{
		//		pop_menu->AppendMenu(MF_SEPARATOR);
		//		pop_menu->AppendMenu(MF_STRING,ID_ADD_ITEM,L"Add");
		//		pop_menu->AppendMenu(MF_STRING,ID_ADD_CHILD,L"Add Child");
		//		pop_menu->AppendMenu(MF_STRING,ID_DELETE_ITEM,L"Delete");
	}
	return true;
}

inline bool DockLocationTool::GetKeepFocus() const
{
	return m_bKeepFocus;
}

inline void DockLocationTool::SetKeepFocus(const bool newVal)
{
	m_bKeepFocus = newVal;
}

void DockLocationTool::GetFloatingMinSize(long *MinWide,long *MinHeight)
{
	if(m_MODEL==ISLOCATION)
	{
		*MinWide = 90;
		*MinHeight = 400;
	}
	if(m_MODEL==ISMENU)
	{
		*MinWide = 200;
		*MinHeight = 45;
	}
//	*MinWide = 700;
//	*MinHeight = 30;
}

BOOL DockLocationTool::CreateControlBar(LPCREATESTRUCT lpCreateStruct)//去X
{
	return TRUE;	
}

void DockLocationTool::OnMenuitemzdmfa() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	
	_tcscpy(cmdstr,L"(command \"INPUTZMFA_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);		 			
}
//直线转坡度线
void DockLocationTool::OnMenuitemPdx()//直线转纵断面设计线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"IDZDM_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMENUITEMsetZMSJBZ()//设置纵面标准
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"SetZMBZ_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMzmzdpmfa() //指定平面方案
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	
	_tcscpy(cmdstr,L"(command \"ZM_ZDPMFA_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMENUITEMPickDmx() //采集地面数据
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"PickZdmDmx_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMEditDMD() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	
	acDocManager->setCurDocument(acDocManager->curDocument(),AcAp::kNone,Adesk::kTrue);
	_tcscpy(cmdstr,L"(command \"EDITDMD_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
		 
}

void DockLocationTool::OnMenuitemZDJP() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	
	acDocManager->setCurDocument(acDocManager->curDocument(),AcAp::kNone,Adesk::kTrue);
	_tcscpy(cmdstr,L"(command \"ZDJP\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);		 
}

void DockLocationTool::OnMenuitemDMX() //点绘地面线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"DMX_Road\")\n");	
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}
//
void DockLocationTool::OnMenuitemINBXDMX()
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"INBXDMX\")\n");	
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void DockLocationTool::OnMenuitemBXDMX() //点绘边线地面线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"BXDMX\")\n");	
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMenuitemSAVEBXDMX() //点绘边线地面线
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"SAVEBXDMX\")\n");	
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMenuitemKZBG()
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"KZBG\")\n");	
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMsetZmCs() //纵断面初始化
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"InitZdm_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMenuitemLpsj() //拉坡设计
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"CZMLPSJ_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMMdyPd() //调整坡度
{
	// TODO: Add your command handler code here		
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"MDYPD_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void DockLocationTool::OnMenuitemZpsjb() //纵坡数据表
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"ZPB_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMaddBpd() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"ADDBPD_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}

void DockLocationTool::OnMENUITEMDelBpd() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"DELBPD_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}
void DockLocationTool::OnMENUITEMLinkBD() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"LINKPDX_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMenuitemsavezdm() //保存纵面方案
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"SAVEZDMPRJ_Road\")\n");
    acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);		 		
}

void DockLocationTool::OnMENUITEMCalBri() //自动设桥
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"CalBri_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}
void DockLocationTool::OnMENUITEMDelBri() //删除桥梁
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"DelBri_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMAddBri() //增加桥梁
{
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"AddBri_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMCalTun() //自动设隧
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"CalTun_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}
void DockLocationTool::OnMENUITEMDeltun() //删除隧道
{
	// TODO: Add your command handler code here
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"DelTun_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMENUITEMaddtun() //增加隧道
{
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"AddTun_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void DockLocationTool::OnMENUITEMEditBri() //编辑桥
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"EDITBri_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMENUITEMEditTun() //编辑隧
{
	// TODO: Add your command handler code here
    ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"EDITTun_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMENUITEMInputTun() //导入隧道(数据库)
{
    // TODO: Add your command handler code here
    ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"MdbInTun_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMENUITEMInputBri() //导入桥梁
{
	// TODO: Add your command handler code here		
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"InputBri_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
}

void DockLocationTool::OnMenuitemLMKD()//定义路面宽度
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"Lmk\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}	

void DockLocationTool::OnMenuitemGetLmkToFile()//重实体中提取边线坐标到文件中
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"GetLmk\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMenuitemEditLmk()//重实体中提取边线坐标到文件中
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"EditLmk\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMenuitemLMX()//绘路面线
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"DrawLMX\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}

void DockLocationTool::OnMenuitemPZX()//绘路面线
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"DrawPZX\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}	

void DockLocationTool::OnMenuitemCalBD()//绘路面线
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"CalBD\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	
}	

void DockLocationTool::OnMenuitemADDDL()//增加断链
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCADDDL\n",0);		
}
void DockLocationTool::OnMenuitemDELDL()//删除断链
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDELDL\n",0);		
}

void DockLocationTool::OnMenuitemLocationtool1() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"LOCATIONTOOL\n",0);		
}

void DockLocationTool::OnMenuitemLocationtool2() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"RPMLOC\n",0);		
}

void DockLocationTool::OnMenuitemZJBSCD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZJBSCD\n",0);
}

void DockLocationTool::OnMenuitemInPutJDCen()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"INJD\n",0);		
}

void DockLocationTool::OnMenuitemInPutXY()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"INXY\n",0);		
}

void DockLocationTool::OnMenuitemDrawPm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDRAWPM_Road\n",0);		
}

void DockLocationTool::OnMenuitemDrawGRID()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRAWGRID_Road\n",0);		
}

void DockLocationTool::OnMenuitemDrawZdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRAWZDM_Road\n",0);		
}

void DockLocationTool::OnMenuitemDrawHdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DRAWHDMT_Road\n",0);		
}

void DockLocationTool::OnMenuitemAJUSTAXLE()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"AJUSTAXLE_Road\n",0);		
}
void DockLocationTool::OnMenuitemAddAXLE()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ADDAXLE_Road\n",0);		
}

void DockLocationTool::OnMenuitemDelAXLE()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DELAXLE_Road\n",0);		
}

void DockLocationTool::OnMenuitemOutZdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZDM_Road\n",0);		
}

void DockLocationTool::OnMenuitemOutHdm()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTHDM_Road\n",0);		
}

void DockLocationTool::OnMenuitemReverse()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCREVERSE\n",0);		
}

void DockLocationTool::OnMenuitemTranJD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CXJ\n",0);		
}

void DockLocationTool::OnMenuitemTranCenter()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"TRANCENTER\n",0);		
}

void DockLocationTool::OnMenuitemEditJD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"EDITJDCEN\n",0);		
}

void DockLocationTool::OnMenuitemLINKXL()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XLLINK\n",0);		
}

void DockLocationTool::OnMenuitemZXYTOXL()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZXYTOXL\n",0);
}

void DockLocationTool::OnMenuitemNEWXM()//新建项目
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"NewMdfRoad\n",0);
}

void DockLocationTool::OnMenuitemBDXM()
{
	// TODO: Add your command handler code here	
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SelMdfRoad\n",0);
}

void DockLocationTool::OnMenuitemINPUTDBSBYJD()//从数据库读取
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OPENPMDBSBYJD\n",0);	
}

void DockLocationTool::OnMenuitemINPUTDBSBYXY()//从数据库读取
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OPENPMDBSBYXY\n",0);	
}

void DockLocationTool::OnMenuitemOUTPUTDBS()//保存到数据库
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SAVEPMDBS\n",0);	
}

void DockLocationTool::OnMenuitemZQZJB()//直曲转角表
{
	acutPrintf(L"直曲转角表!\n");	
}

void DockLocationTool::OnMenuitemZDQXB()//匝道曲线表	
{
	acutPrintf(L"匝道曲线表!\n");	
}

void DockLocationTool::OnMenuitemZDZDZBB()//匝道主点坐标表
{
	acutPrintf(L"匝道主点坐标表!\n");
}

void DockLocationTool::OnMenuitemZDZZZBB()//匝道主桩坐标表
{
	acutPrintf(L"匝道主桩坐标表!\n");
}

void DockLocationTool::OnMenuitemINPUTEXJDCEN()//录入二线平面数据
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCEXSJ\n",0);	
}

void DockLocationTool::OnMenuitemQIECHUIXIAN()//切线垂线工具
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCPROJPOINT\n",0);	
}

void DockLocationTool:: OnMenuitemLIANXUDINGXIAN()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCADDXL\n",0);	
}

void DockLocationTool::OnMenuitemDELFLD()//删除分离段
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDELFLD\n",0);	
}

void DockLocationTool::OnMenuitemEDITDL()//编辑断链
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCDLB\n",0);	
}

void DockLocationTool::OnMenuitemZMEDITDL()//编辑断链
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCZMDLB_Road\n",0);	
}
void DockLocationTool::OnMenuitemJDTDFS()//设置交点拖动方式
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCJDTDFS\n",0);	
}

void DockLocationTool::OnMenuitemAUTOTK()//自动布图框
{
}

void DockLocationTool::OnMenuitemOUTPM()//分幅输出平面图	
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCAUTOPM_Road\n",0);		
}

void DockLocationTool::OnMenuitemDrawCBQXYS()
{
 /*
   SetDrawQXYS *qxdlg=NULL;
    qxdlg = new SetDrawQXYS(acedGetAcadFrame());
    qxdlg->Create (IDD_DIALOGQXYS, acedGetAcadFrame());
 		  //  move the window over so we can see the control lines
    qxdlg->ShowWindow(SW_SHOW);*/
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"DRAWQXYS_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);  

}
void DockLocationTool::OnMenuitemDrawXYYSB()
{
	ACHAR cmdstr[80];
	// TODO: Add your command handler code here
	_tcscpy(cmdstr,L"(command \"DRAWXYYSB_Road\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0); 
}

void DockLocationTool::OnMenuitemPMTSET()//平面图参数设置
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCPMDRAWSET\n",0);		
}

void DockLocationTool::OnMENUITEMtqsj() 
{
	// TODO: Add your command handler code here
    LoadArx(L"dtmoutdata.arx",L"outdata_l");	
}

void DockLocationTool::OnMENUITEMdealdata() 
{
	// TODO: Add your command handler code here
	ACHAR fname[256];
    _stprintf(fname ,L"%s\\dldata.exe",SysPath);
	//  runexe(fname);    
	WinExec((LPCSTR)fname,1);
}

void DockLocationTool::OnMENUITEMcreateTin() 
{
	// TODO: Add your command handler code here
	ACHAR fname[256];
    _stprintf(fname ,L"%s\\CreateTin.exe",SysPath);
	//	runexe(fname);
	WinExec((LPCSTR)fname,1);
}

void DockLocationTool::OnMENUITEMdisplay() 
{
	// TODO: Add your command handler code here
	if(ptriDTM.DTMHasRead==1)
	    ptriDTM.ReleaseDtm();//释放Dtm
	
	LoadArx(L"3d.arx",L"draw");	
}
bool DockLocationTool::LoadArx(ACHAR ArxFname[256],ACHAR cmd[80])
{
	ACHAR cmdstr[80];
	
	// if(!CheckDog())
	//    	return false;
	
	//    if(!CheckNetDog())
	//     return false;
	acedGetAcadDwgView()->SetFocus();
	ACHAR fname[256];
	_stprintf(fname ,L"%s\\%s",SysPath,ArxFname);

	acedArxLoad(fname);
	acDocManager->activateDocument(acDocManager->curDocument());
	_stprintf(cmdstr ,L"(command \"%s\")\n",cmd);
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
	
	return 1;
	
}

void DockLocationTool::OnMENUITEMSETCGBZ() //设置超高标准
{
	// TODO: Add your command handler code here
	SetCGBZDlg dlg;
	dlg.DoModal();
	
}

void DockLocationTool::OnMENUITEMSETJKBZ() //设置加宽标准
{
	// TODO: Add your command handler code here
	CSetJKNorm dlg;
	dlg.DoModal();
	
}

void DockLocationTool::OnMENUITEMCALCGFD() //计算超高分段
{
	// TODO: Add your command handler code here

	acedGetAcadDwgView()->SetFocus();

	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CALCGFD\n",0);				

}

void DockLocationTool::OnMENUITEMCXCGFD() //查询超高分段
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CXCGFD\n",0);				
	
}

void DockLocationTool::OnMENUITEMsetHmCs() //横断面初始化
{
	ACHAR fname[256];
    _stprintf(fname ,L"%s\\Roadhdmdata.exe",SysPath);
	//  runexe(fname);    
	WinExec((LPCSTR)fname,1);   
}

void DockLocationTool::OnMENUITEMCalSta() //计算桩号序列
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CALSTA_Road\n",0);				
}

void DockLocationTool::OnMENUITEMAjustSta()//调整桩号
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"AjustSta\n",0);
}

void DockLocationTool::OnMENUITEMGetDmx() //横断面地面线
{
	// TODO: Add your command handler code here
	if(ptriDTM.DTMHasRead==1)
	    ptriDTM.ReleaseDtm();//释放Dtm
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"GetDmx_Road\n",0);
}

void DockLocationTool::OnMENUITEMHdmAutoDes() //横断面自动设计
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"HdmAutoDes_Road\n",0);
}

void DockLocationTool::OnMENUITEMInputHdm() //导入横断面
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"InHdm_Road\n",0);
}

void DockLocationTool:: OnMENUITEMChangeBp() //修改边坡
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"EDITHDMBP_Road\n",0);
}
void DockLocationTool:: OnMENUITEMSAVEHDM() //修改边坡
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SAVEHDM_Road\n",0);
}
 


void DockLocationTool::OnMENUITEMInputYD() //导入用地数据
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZDKDES\n",0);
}

void DockLocationTool::OnMENUITEMDelYDZ() //删除用地桩
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DELZDK\n",0);	
}

void DockLocationTool::OnMenuitemAddydz() //添加用地桩
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ADDZDK\n",0);
}

void DockLocationTool::OnMenuitemEdityddata() //编辑用地数据
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"EDITZDK\n",0);
}

void DockLocationTool::OnMenuitemChangeydz() //修改用地桩
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CHANGEZDK\n",0);
}

void DockLocationTool::OnMenuitemSaveyddata() //保存用地数据
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"SAVEZDK\n",0);
}
	
void DockLocationTool::OnMenuitemZXQXZJB()
{
// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZXQXZJ\n",0);
}
void DockLocationTool::OnMenuitemZZZBB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZZZB\n",0);
}
void DockLocationTool::OnMenuitemZPSQXB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZPSQX\n",0);
}
void DockLocationTool::OnMenuitemZZYDZBB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTZZYDZB_Road\n",0);
}
void DockLocationTool::OnMenuitemTSFJSB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTTSFJSB_Road\n",0);
}
void DockLocationTool::OnMenuitemGCSLB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"OUTGCSLB_Road\n", 0);
}

	void DockLocationTool::OnMenuitemDYT()
	{
		// TODO: Add your command handler code here
		acedGetAcadDwgView()->SetFocus();
		acDocManager->sendStringToExecute(acDocManager->curDocument(), L"OUTDYT_Road\n", 0);
	}
void DockLocationTool::OnMenuitemMGLTSFB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTMGLTSFB\n",0);
}
void DockLocationTool::OnMenuitemLXTWSJB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTLXTWB\n",0);
}
void DockLocationTool::OnMenuitemLJSJB()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTLJSJB\n",0);
}

void DockLocationTool::OnMenuitemBGPSG()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OUTBGPSG\n",0);
}

void DockLocationTool::OnMenuitemDATA()
{
	LoadArx(L"projmdb.arx",L"tree");	
}
 
void DockLocationTool::OnMENUITEMGOUPD()
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"AdjGouPD_Road\n",0);
}

void DockLocationTool::OnMenuitemDELAYER()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DELAYER\n",0);
}

void DockLocationTool::OnMenuitemRUBBER()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"RUBBER\n",0);
}

void DockLocationTool::OnMenuitemTOOL()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"VIZTOOL\n",0);
}

void DockLocationTool::OnMenuitemRIBBON()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(), L"RIBBONn", 0);
}

void DockLocationTool::OnMenuitemDX3D()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"DX3D\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void DockLocationTool::OnMenuitemLX3D()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"LX3D\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void DockLocationTool::OnMenuitemView3D()
{
	ACHAR fname[256];
    _stprintf(fname ,L"%s\\T3DView.exe",SysPath);
	WinExec((LPCSTR)fname,1);	
}

void DockLocationTool::OnMenuitemINITLW()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"INITLW\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
}
void DockLocationTool::OnMenuitemSYSMLW()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"SYSMLW\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
}
void DockLocationTool::OnMenuitemZTOPLW()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"ZTOPLW\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(), cmdstr, 0);
}

void DockLocationTool::OnMenuitemPmGuide()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"PmGuide\n",0);	
}

void DockLocationTool::OnMenuitemLczzb() 
{
	// TODO: Add your command handler code here
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"PMLCTOZB\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);					
}
void DockLocationTool::OnMenuitemZbzlc()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"PMZBTOLC\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMenuitemCxlmbg()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"CXLMBG\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void DockLocationTool::OnMenuitemCxYdArea()
{
	ACHAR cmdstr[80];
	_tcscpy(cmdstr,L"(command \"CXYDAREA\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
void DockLocationTool::OnMenuitemProCheck()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ProCheck\n",0);	
}
void DockLocationTool::OnMenuitemJMBX() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JMBX\n",0);
}
void DockLocationTool::OnMenuitemZDCT() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"ZDCT\n",0);
}
void DockLocationTool::OnMenuitemXLJD() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XLJD\n",0);
}	

void DockLocationTool::OnMENUITEMCalNose() 
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"CalNose\n",0);
}	

void DockLocationTool::OnMenuitemDrawDBZX()//绘端部中线
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBZX\n",0);
}
void DockLocationTool::OnMenuitemDrawDBBDCIR()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBCIR\n",0);
}
void DockLocationTool::OnMenuitemDrawDBHX()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBHX\n",0);
}
void DockLocationTool::OnMenuitemDBBZ()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"DrawDBBZ\n",0);
}

void DockLocationTool::OnMenuitemXGDB()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XGDBBZ\n",0);
}

void DockLocationTool::OnMenuitemPerspective()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"GenPers\n",0);
}

void DockLocationTool::OnMenuitemPPCD()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"PPCD\n",0);
}


void DockLocationTool::OnMenuitemHELP()
{

	ACHAR hlpname[256];
	_stprintf(hlpname,L"%s\\DGRoadHelp.chm",SysPath);
	acedHelp(hlpname, NULL, 0);

}

void DockLocationTool::OnMenuitemQBTTAB()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OutQBTB\n",0);
}
void DockLocationTool::OnMenuitemWYNTAB()
{
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"OutWYNB\n",0);
}
//计算车速
void DockLocationTool::OnMENUITEMjscs()
{
   LoadArx(L"csjy.arx",L"CalSpeedDlg");	
}

//车速出图
void DockLocationTool::OnMENUITEMcsct()
{
   LoadArx(L"csjy.arx",L"OutSpeedPic");	
}
//车速出表
void DockLocationTool::OnMENUITEMcscb()
{
   LoadArx(L"csjy.arx",L"SpeedExcelDlg");	
}









