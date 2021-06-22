// InRoadNameDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "InRoadNameDlg.h"
#include "XLdatabase.h"
#include "bas_draw_fun.h"
#include "RoadDesignWorkingDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库

/////////////////////////////////////////////////////////////////////////////
// InRoadNameDlg dialog


InRoadNameDlg::InRoadNameDlg(AcDbObjectId pmId,CWnd* pParent /*=NULL*/)
	: CDialog(InRoadNameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(InRoadNameDlg)
	m_RoadName = "";
	m_path = "";
	INpm = NULL;
	eId = pmId;
	BGH = "K";
	EGH = "K";
	BLC = -100.0;
	ELC = -100.0;
	IfSetRoadName = false;
	//}}AFX_DATA_INIT
}


void InRoadNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InRoadNameDlg)
	DDX_Text(pDX, IDC_EDIT_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_path, m_path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(InRoadNameDlg, CDialog)
	//{{AFX_MSG_MAP(InRoadNameDlg)
	ON_BN_CLICKED(IDC_RADIO_LftLine, OnRADIOLftLine)
	ON_BN_CLICKED(IDC_RADIO_MainLine, OnRADIOMainLine)
	ON_BN_CLICKED(IDC_RADIO_RtLine, OnRADIORtLine)
	ON_BN_CLICKED(IDC_BUTTON_browser, OnBUTTONbrowser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InRoadNameDlg message handlers

void InRoadNameDlg::OnRADIOLftLine() 
{
	// TODO: Add your control notification handler code here
	mode = -1;
	GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(true);	
	//道路名称自动付值
	//1.首先读m_path中的控制点表主线数据
	//2.建立主线对象
	if(m_RoadName=="主线" || m_RoadName.Find(L"右线")>=0)
		IfSetRoadName = true;
	if(!IfSetRoadName)
		return;

	JD_CENTER *pJC=NULL;
	if(m_path != "")
	{
		XLDataBase DBS;
		//		DBS.Read_XLDbs(m_path, L"控制点表", L"主线");
		DBS.Read_XLDbs(m_path, L"线元表", L"主线");		
		DBS.Read_XLDbs(m_path, L"断链表", L"主线");
		if(DBS.XYNum>0)
		{
			pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			//			pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0)
				pJC->setDLB(DBS.DLArray,DBS.NDL);
		}	
	}


	if(pJC)
	{
		//3.计算分离段起点投影到主线里程
		double tylc1=pJC->PROJ_ML(INpm->XYArray[1][8],INpm->XYArray[1][9]);
		BLC = pJC->XLC(tylc1,BGH);
		//4.组成道路名称
		m_RoadName.Format(L"左线%0.1lf", BLC/1000.0);

		tylc1=pJC->PROJ_ML(INpm->XYArray[INpm->XYNum][8],INpm->XYArray[INpm->XYNum][9]);//终点
		ELC = pJC->XLC(tylc1,EGH);

		delete pJC;
	}
	else
	{
		ads_printf(L"\n错误: 数据库中无主线数据，请先保存主线平面再设计分离段!\n");
		m_RoadName = "左线";
	}
	UpdateData(false);
}

void InRoadNameDlg::OnRADIOMainLine() 
{
	// TODO: Add your control notification handler code here
	mode = 0;
	m_RoadName = "主线" ;
	GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(false);	
	UpdateData(false);			
}

void InRoadNameDlg::OnRADIORtLine() 
{
	// TODO: Add your control notification handler code here
	mode = 1;
	GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(true);					
	// TODO: Add your control notification handler code here
	if(m_RoadName=="主线" || m_RoadName.Find(L"左线")>=0)
		IfSetRoadName = true;
	if(!IfSetRoadName)
		return;
	//道路名称自动付值
	//1.首先读m_path中的控制点表主线数据
	//2.建立主线对象
	JD_CENTER *pJC=NULL;
	if(m_path != "")
	{
		XLDataBase DBS;
		//		DBS.Read_XLDbs(m_path, L"控制点表", L"主线");
		DBS.Read_XLDbs(m_path, L"线元表", L"主线");				
		DBS.Read_XLDbs(m_path, L"断链表", L"主线");
		if(DBS.XYNum>0)
		{
			pJC=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			//			pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0) 
				pJC->setDLB(DBS.DLArray,DBS.NDL);
		}				
		else
			ads_alert(L"无断链");
	}

	if(pJC)
	{
		//3.计算分离段起点投影到主线里程
		double tylc1=pJC->PROJ_ML(INpm->XYArray[1][8],INpm->XYArray[1][9]);
		BLC = pJC->XLC(tylc1,BGH);
		//4.组成道路名称
		m_RoadName.Format(L"右线%0.1lf", BLC/1000.0);

		tylc1=pJC->PROJ_ML(INpm->XYArray[INpm->XYNum][8],INpm->XYArray[INpm->XYNum][9]);//终点
		ELC = pJC->XLC(tylc1,EGH);
		delete pJC;
	}
	else
	{
		ads_printf(L"错误: 数据库中无主线数据，请先保存主线平面再设计分离段!\n");
		m_RoadName = "右线";
	}	
	UpdateData(false);
}

BOOL InRoadNameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	AcDbObject *pObj;
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
		return false;
	pObj->close();	   
	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{		
		INpm = JD_CENTER::cast(pObj);
		m_RoadName = INpm->RoadName;
		m_path = INpm->mdbname;		
	}
	else
	{
		ads_printf(L"错误：您选择的不是平面实体!\n");
		return false;
	}

	// TODO: Add extra initialization here
	if(m_RoadName == "主线")//道路名非空
	{
		CButton *pbutton=(CButton *)GetDlgItem(IDC_RADIO_MainLine);
		pbutton->SetCheck(1);
		GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(false);				
		mode = 0;
	}
	else if(m_RoadName.Find(L"右")>=0)
	{
		CButton *pbutton=(CButton *)GetDlgItem(IDC_RADIO_RtLine);
		pbutton->SetCheck(1);
		GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(TRUE);		
		mode = 1;
	}
	else if(m_RoadName.Find(L"左")>=0)
	{
		CButton *pbutton=(CButton *)GetDlgItem(IDC_RADIO_LftLine);
		pbutton->SetCheck(1);
		GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(TRUE);		
		mode = -1;
	}
	else if(m_RoadName==""||m_RoadName=="未命名道路")
	{
		CButton *pbutton=(CButton *)GetDlgItem(IDC_RADIO_MainLine);
		pbutton->SetCheck(1);
		m_RoadName = "主线";
		GetDlgItem(IDC_EDIT_RoadName)->EnableWindow(false);				
		mode = 1;
		IfSetRoadName = true;
	}

	if(m_path=="" || m_path=="未命名")
	{
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		m_path=Cworkdir+"\\data\\"+Pro+".mdb";
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void InRoadNameDlg::OnBUTTONbrowser() 
{
	// TODO: Add your control notification handler code here
	CString workdir,curpm;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{
	workdir += "\\data";   
	CString DefaultName = m_path;
	CFileDialog fname(false,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
		m_path =fname.GetPathName();
	UpdateData(false);
	//	}
}

void InRoadNameDlg::OnOK() 
{
	// TODO: Add extra validation here	
	CDialog::OnOK();
	//更新实体
	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	AcDbObject *pObj;

	XLDataBase DBS;
	if(acdbOpenObject(pObj, eId, AcDb::kForWrite)!=Acad::eOk)
		return ;

	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{		
		INpm = JD_CENTER::cast(pObj);
		INpm->assertWriteEnabled();
		INpm->setRoadName(m_RoadName);
		INpm->setMdbName(m_path);

		if(mode!=0)//非主线，则休正起始与终止断链
		{
			if(BLC>-0.1)
			{

				INpm->setDLM(0,BLC,INpm->DLArray[0].ELC,BGH,INpm->DLArray[0].EGH);
				if(INpm->JdCenNum>0)
					INpm->JdCenArray[0].A1 = BLC;
			}
			if(ELC>-0.1)
			{
				INpm->setDLM(0,INpm->DLArray[INpm->DLNum-1].BLC,ELC,INpm->DLArray[INpm->DLNum-1].BGH,EGH);
			}

		}
		INpm->close();
	
#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
		if (INpm->m_RelRailName=="")
		{
			CString MdfRoadNmae = DBS.GetProByMdbName(m_path);
			CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
			if(DBS.Read_XLDbs(MdfName, L"改移道路表", MdfRoadNmae))
			{
				INpm->m_RelRailName = DBS.pModifyRoad[0].RailName;
			}
			if (INpm->m_RelRailName!="")
			{
				INpm->Caljd();
			}

		}
#endif
		


	}
	else
	{
		ads_printf(L"错误：您选择的不是平面实体!\n");
		return;
	}

	//20191023新增	传入数据到活动数据库中
	g_RoadWorkingDatabase.SetHorizontalRoad(INpm);

	//写入数据库
	DBS.NDL=INpm->DLNum;
	DBS.DLArray=new BAS_DRAW_FUN::DLBZ[DBS.NDL];
	//	acutPrintf(L"NDL=%d\n ",DBS.NDL);
	int i;
	for( i=0;i<DBS.NDL;i++)
		DBS.DLArray[i]=INpm->DLArray[i];

	DBS.NZDL=INpm->DLNum;
	DBS.ZDLArray=new BAS_DRAW_FUN::DLBZ[DBS.NZDL];
	//	acutPrintf(L"NDL=%d\n ",DBS.NDL);
	for(i=0;i<DBS.NZDL;i++)
		DBS.ZDLArray[i]=INpm->DLArray[i];//纵断面断链表

	DBS.XYNum = INpm->XYNum;
	DBS.XYArray = new double[DBS.XYNum+1][10];
	int j;

	for(i=0;i<=DBS.XYNum;i++)
	{
		for(j=0;j<10;j++)
			DBS.XYArray[i][j]=INpm->XYArray[i][j];
	}
	DBS.XYArray[0][5]=INpm->JD0;
	DBS.NJdCen=INpm->JdCenNum;
	if(DBS.NJdCen>0)
	{
		DBS.JdCenArray=new BAS_DRAW_FUN::JdCenter[DBS.NJdCen];
		for(i=0;i<DBS.NJdCen;i++)
		{
			DBS.JdCenArray[i]=INpm->JdCenArray[i];
		}
		DBS.JdCenArray[0].A2=INpm->JD0;
	}


	/*
	else
	{
	INpm->XYArrayToJD();
	DBS.NJdCen=INpm->JdCenNum;
	if(DBS.NJdCen>0)
	DBS.JdCenArray=new BAS_DRAW_FUN::JdCenter[DBS.NJdCen];		
	}*/


	if(m_path != "")
	{

		DBS.Write_XLDbs(m_path, L"道路名",m_RoadName);
		if(DBS.NJdCen>0)
			DBS.Write_XLDbs(m_path, L"控制点表",m_RoadName);

		DBS.Write_XLDbs(m_path, L"断链表",m_RoadName);
		DBS.Write_XLDbs(m_path, L"纵断链表",m_RoadName);
		DBS.Write_XLDbs(m_path, L"线元表",m_RoadName);

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))		//////////////////////////////////////////////////////////////////////////
		////将线路属性写入改移道路表
		CString MdfRoadNmae = DBS.GetProByMdbName(m_path);
		CString MdfName = Cworkdir + "\\DATA\\改移道路.mdf";
		DBS.Read_XLDbs(MdfName, L"改移道路表", MdfRoadNmae);
		if (DBS.ModifyRoadSum<1)
		{
			DBS.ModifyRoadSum = 1;
			if (DBS.pModifyRoad)
			{
				delete DBS.pModifyRoad;
				DBS.pModifyRoad=NULL;
			}
			DBS.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[DBS.ModifyRoadSum];
			DBS.IniMdfRoadData(MdfRoadNmae, DBS.pModifyRoad[0]);
		}
		else if (DBS.ModifyRoadSum>1)
		{
			AfxMessageBox(m_RoadName + "在改移道路表中有多条记录!");
			return;
		}

		ACHAR Ckml[20],GH[30];
		double dml;
		DBS.pModifyRoad[0].RailName = INpm->m_RelRailName;
		//DBS.pModifyRoad[0].RoadName = MdfRoadNmae;
		DBS.pModifyRoad[0].SCkml = INpm->LCchr(INpm->DLArray[0].EGH, INpm->DLArray[0].ELC, 3);
		DBS.pModifyRoad[0].ECkml = INpm->LCchr(INpm->DLArray[INpm->DLNum-1].BGH, INpm->DLArray[INpm->DLNum-1].BLC, 3);
		DBS.pModifyRoad[0].RoadLen = INpm->LenOfLine;
		DBS.pModifyRoad[0].RailCkml = INpm->m_InterSectData.InterCkml;
		//ads_printf(L"%s",DBS.pModifyRoad[0].RailCkml);
		DBS.pModifyRoad[0].Raillevel = INpm->m_InterSectData.InterDesH;
		DBS.pModifyRoad[0].RoadDml = INpm->m_InterSectData.CurDml;
		DBS.pModifyRoad[0].InterAng = INpm->m_InterSectData.JiaJiao;
		DBS.pModifyRoad[0].DetaH = INpm->m_InterSectData.DetaH;
		DBS.pModifyRoad[0].LmArea = DBS.pModifyRoad[0].LmWide * DBS.pModifyRoad[0].RoadLen;
		DBS.Write_XLDbs(MdfName, L"改移道路表",MdfRoadNmae);
		//DBS.Write_XLDbs(m_path, L"改移道路表", m_RoadName);

		//20191030	新增存储交叉信息
		DBS.m_RoadNRailInterInforArray.clear();
		DBS.m_RoadNRailInterInforArray.push_back(INpm->m_InterSectData);
		DBS.Write_XLDbs(m_path, L"道铁交叉信息表", m_RoadName);
#endif
		

	}
	ACHAR mes[256];
	_stprintf(mes, L"已将数据存入%s数据库,道路名: %s",m_path,m_RoadName);
	ads_alert(mes);
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();		

}
