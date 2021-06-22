// OutTAB.cpp: implementation of the OutTAB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OutTAB.h"
#include "TABDlg.h"
#include "JD_CENTER.h"
#include "comdef.h" 
#include "QBTMLDlg.h"
#include "hdmdatabase.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OutTAB::OutTAB()
{
	CrsData=NULL;
	TSF_Ncrs=0;
	LXTW=NULL;
	NLXTW = 0;	
	pYdByKm = NULL;
	NYdByKm = 0;
	BGPSG = NULL;
	NBGPSG = 0;
}

OutTAB::~OutTAB()
{

}

void OutTAB::OUT_YDBYKM()
{
	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\用地面积表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));

	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	LPDISPATCH pRange=NULL;
	Range m_range;
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();

	for(int i=0;i<NYdByKm;i++)
	{	
		pRange = m_worksheet.GetCell(i+2,1);
		m_range.AttachDispatch(pRange);	  
		m_range.SetValue(COleVariant(pYdByKm[i].lcfw));
		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pYdByKm[i].zarea+pYdByKm[i].yarea));			   		   
	}

	//删除模板工作表

	if(pYdByKm)
	{
		delete []pYdByKm;
		pYdByKm = NULL;
		NYdByKm = 0;
	}
}

void OutTAB::OUT_ZZZB()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	//if(dlg.DoModal()==IDOK)
	//{

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];

	_stprintf(filepath, L"%s\\lib\\excel\\逐桩坐标表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));

	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	//   CalZX(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
	//   CalFX(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
	FillZzzb(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);


	//  }
}

void OutTAB::FillZzzb(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int i, totalnum, row, col, flag1, flag2,m=0;
	long shnum;
	ACHAR shname[30], filepath[180], pageinfo[80];

	RoadStr = RoadStr + " " + DuanName;
	//读桩号序列表
	XLDataBase mdb;		
	JD_CENTER *pm;
	GTZDM_ROAD *zdm=NULL;
	// 创建每一道路平面对象

	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);			
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	pm = NULL;

	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	} 

	double Scml,Ecml,cml,Zx,Zy,Yx,Yy;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);
	//    zdm = ReadZm(PrjMdb,RoadName, L"全幅",pm);
	mdb.Read_XLDbs(PrjMdb, L"桩号序列",RoadName);

	// 填写工程名称及段号
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"B4")));//lbw 
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));

	flag1 = 0; shnum = 1;
	//   FILE *fpw;
	//	fpw = _wfopen(L"E:\\辅路中线高程.txt", L"w");
	if(mdb.NSTA<1)
		ads_alert(L"请先生成桩号序列!");

	CString mes;
	for(i=0;i<mdb.NSTA;i++)
	{   
		//  cml = pm->TYLC(mdb.STA[i].lc);
		cml = mdb.STA[i].lc;
		if(cml+0.1<Scml) 
		{
			m++;
			continue;
		}
		if(cml-0.1>Ecml) break;
		if((i-m)%31==0)
		{
			if((i-m)%93==0)
			{
				// 复制模板工作表然后填写
				m_app.Run(COleVariant(L"Macro1"),COleVariant(shnum));//lbw
				pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
				m_worksheet.AttachDispatch(pWorksheet);
				m_worksheet.Select();
				_stprintf(shname, L"逐桩坐标%d",shnum);
				m_worksheet.SetName(shname);
				_stprintf(pageinfo, L"第  %d  页     共  %d  页",
					Spage+shnum-1, PageSum);
				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"K4")));	  
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(pageinfo));
				shnum++;
			}
			row = 9;
			flag2 = flag1%3;
			if(flag2==0) col = 2;
			if(flag2==1) col = 6;
			if(flag2==2) col = 10;
			flag1++;
		}

		pRange = m_worksheet.GetCell(row,col);
		m_range.AttachDispatch(pRange);
		CString  ckml;
		ckml = pm->XLC(cml);
		m_range.SetValue(COleVariant(ckml));
		double x,y,z=0;
		xlpoint PZ;

		PZ.lc = cml;
		//	  if(zdm)
		//	     z = zdm->rdes_level(PZ.lc);
		pm->xlpoint_pz(&PZ);
		x = PZ.x;
		y = PZ.y;

		//      Zx = x + 32.875*cos(PZ.a-0.5*pi);
		//	  Zy = y + 32.875*sin(PZ.a-0.5*pi);
		//      Yx = x + 32.875*cos(PZ.a+0.5*pi);
		//	  Yy = y + 32.875*sin(PZ.a+0.5*pi);
		//	  if(cml>28180&&cml<29050)
		//		  fwprintf(fpw, L"%0.3lf %0.3lf\n",cml,z);
		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(x));
		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(y));
		row++;
	}
	//	fclose(fpw);

	//删除模板工作表
	pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	m_worksheet.Delete();

}
void OutTAB::ExcelLoad()
{

	LPDISPATCH pDisp;
	LPUNKNOWN pUnk;
	CLSID clsid;
	//	BeginWaitCursor();
	COleException error; 
	::CLSIDFromProgID(L"Excel.Application",&clsid);
	if(::GetActiveObject(clsid,NULL,&pUnk)==S_OK)
	{
		VERIFY(pUnk->QueryInterface(IID_IDispatch,
			(void* *)&pDisp)==S_OK);
		m_app.AttachDispatch(pDisp);
		pUnk->Release();
		TRACE("attach complete");
	}
	else 
	{
		if(!m_app.CreateDispatch(L"Excel.Application",&error))
		{
			ACHAR szErrMsg[255];
			error.GetErrorMessage(szErrMsg,255);
			AfxMessageBox(szErrMsg,MB_ICONSTOP);
			return;
		}
	}
	//   EndWaitCursor();
}

void OutTAB::OUT_ZXQXZJ() 
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	//if(dlg.DoModal()==IDOK)
	//{
	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[180];
	_stprintf(filepath, L"%s\\lib\\excel\\直线曲线转角表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	FillZxqxzj(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
}

void OutTAB::FillZxqxzj(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int flag;
	int i, j, row, totalnum,m=1;
	long shnum;
	ACHAR filepath[180], shname[30], pageinfo[80];


	RoadStr = RoadStr + " " + DuanName;
	XLDataBase mdb;		
	JD_CENTER *pm;
	// 创建每一道路平面对象
	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
	//	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);		
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	pm = NULL;

	if(mdb.NJdCen>0)
	{
		pm=(JD_CENTER *)new JD_CENTER();//主线对象
		pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		pm->JD0 = mdb.JdCenArray[0].A2;
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
		pm->CreateZQZJB();
	}
	else
	{
		ads_alert(L"平面控制点数为零");
		return;
	}



	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));
	shnum = 1;
	row = 12;    //第一张表循环时row = 12
	totalnum = pm->JdCenNum;

	if(totalnum >= 1 )
	{
		//第一个特殊区别对待
		if(totalnum > 29)   // 若有两张表,需要复制模板
		{
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));

			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();

			_stprintf(shname, L"直线曲线转角%d",shnum);
			m_worksheet.SetName(shname);
			_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"S4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));

			shnum++;

		}

		pRange=m_worksheet.GetRange(COleVariant(L"A9"));
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(short(1)));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pm->ZQZJB[0].JDZH));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pm->ZQZJB[0].JDN));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pm->ZQZJB[0].JDE));
	}

	for(i=m;i<totalnum;i++)
	{
		flag=0;
		//if(ckmltocml(zqz.zh)-0.1>ckmltocml(m_chzml)) break;

		pRange=m_worksheet.GetCell(row-1,19);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pm->ZQZJB[i-1].JIAZX));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		double dms;
		dms = dms_rad(pm->ZQZJB[i-1].FWJ);
		ACHAR str[256];
		_stprintf(str, L"%0.5lf",dms);
		dms = _wtof(str);
		m_range.SetValue(COleVariant(dms));

a200:  pRange=m_worksheet.GetCell(row,1);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(long(i+2-m)));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		ACHAR ckml[80];
		_tcscpy(ckml,pm->ZQZJB[i].JDZH);
		m_range.SetValue(COleVariant(ckml));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pm->ZQZJB[i].JDN));
		pRange=m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pm->ZQZJB[i].JDE));
		double zj;
		zj = dms_rad(pm->ZQZJB[i].AFA);
		_stprintf(str, L"%0.5lf",zj);
		zj = _wtof(str);

		if(fabs(zj) <= 1.0e-6)
		{
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else if(zj>0.0)
		{
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);


			m_range.SetValue(COleVariant(zj));
		}
		else
		{

			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(-zj));
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}

		for(j=0;j<8;j++)
		{
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);
			if(j==0)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].R2));
			else if(j==1)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].A1));
			else if(j==2)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].A2));
			else if(j==3)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].Ls1));
			else if(j==4)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].Ls2));
			else if(j==5)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].T1));
			else if(j==6)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].T2));
			else if(j==7)
				m_range.SetValue(COleVariant(pm->ZQZJB[i].L));
		}

		for(j=0;j<4;j++)
		{
			pRange=m_range.GetNext();
			m_range.AttachDispatch(pRange);

			ACHAR ckml[80];
			_tcscpy(ckml,pm->ZQZJB[i].ZHZH);
			if(j==0)
				m_range.SetValue(COleVariant(ckml));
			_tcscpy(ckml,pm->ZQZJB[i].HYZH);
			if(j==1)
				m_range.SetValue(COleVariant(ckml));
			_tcscpy(ckml,pm->ZQZJB[i].YHZH);
			if(j==2)
				m_range.SetValue(COleVariant(ckml));
			_tcscpy(ckml,pm->ZQZJB[i].HZZH);
			if(j==3)
				m_range.SetValue(COleVariant(ckml));
		}

		row = row +3;

		if(flag==0&&(i-m+1)%28==0&&i+1<totalnum)
		{

			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));

			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));

			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"直线曲线转角%d",shnum);
			m_worksheet.SetName(shname);  // 写表名
			_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"S4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));  // 填写页信息

			shnum++;
			row = 9;
			flag=1;
			goto a200;
		}

	}
	//删除模板工作表
	if(totalnum>29)
	{
		pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
		m_worksheet.AttachDispatch(pWorksheet);
		m_worksheet.Select();
		m_worksheet.Delete();
	}
	else
	{
		_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage, PageSum);
		VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"S4")));	  
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pageinfo));
	}
}


void OutTAB::OUT_ZPSQX()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\纵坡竖曲线表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	Fillzpsqx(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);


}

//*************************************** 
//   Fillzpsqx   填写纵坡曲线表
//*************************************** 
void OutTAB::Fillzpsqx(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int i, j, totalnum, row, flag1,m=0;
	long shnum;

	RoadStr = RoadStr + " " + DuanName;

	XLDataBase mdb;
	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);			
	mdb.Read_XLDbs(PrjMdb, L"纵断链表",RoadName);

	pzLinep = NULL;
	if(mdb.XYNum>0)
	{
		pzLinep=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pzLinep->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NZDL>0)
			pzLinep->setDLB(mdb.ZDLArray,mdb.NZDL);
	}
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));
	flag1 = 0; shnum = 1;

	FillzpsqxByXZ(pzLinep,PrjMdb,RoadName,Sdml,Edml,DuanName,Spage,PageSum, L"全幅",shnum);

	FillzpsqxByXZ(pzLinep,PrjMdb,RoadName,Sdml,Edml,DuanName,Spage,PageSum, L"左幅",shnum);

	FillzpsqxByXZ(pzLinep,PrjMdb,RoadName,Sdml,Edml,DuanName,Spage,PageSum, L"右幅",shnum);


	//	pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
	//	m_worksheet.AttachDispatch(pWorksheet);
	//	m_worksheet.Select();
	//	m_worksheet.Delete();

}


void OutTAB::FillzpsqxByXZ(JD_CENTER *pm,CString PrjMdb,CString RoadName,double Sdml,double Edml,CString DuanName,int Spage,int PageSum,CString XZ,long &shnum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	XLDataBase mdb;		
	int j, totalnum, row, flag1,m=0;
	bool flag;
	// 创建每一道路平面对象
	flag = true;
	double Scml,Ecml,cml;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);
	pzLinep = pm;
	_tcscpy(pzLinep->mdbname , PrjMdb);
	_tcscpy(pzLinep->RoadName , RoadName);
	double PD_array[MAXBPDNUM][4];
	int NBPD;
	int deshxsw,pdxsw;
	ACHAR GH[8];
	ACHAR shname[30], filepath[180], pageinfo[80];

	//读全幅
	int res = mdb.Read_XLDbs(PrjMdb, L"坡度表",RoadName,XZ);
	int res1 = mdb.Read_XLDbs(PrjMdb, L"纵断链表",RoadName);
	if(!res)//读坡度
		return;

	NBPD = mdb.NPD;
	if(NBPD<1)
		return;

	int i;
	for(i=0;i<NBPD;i++)
	{
		PD_array[i][0] = TYLC1(mdb.PDB[i].ml,mdb.NZDL,mdb.ZDLArray);	
		PD_array[i][1] = mdb.PDB[i].Level;
		PD_array[i][2] = mdb.PDB[i].Rshu;			
		PD_array[i][3] = 0.0;
	} 

	if(mdb.PDB) 
	{
		deshxsw = (int)(PD_array[0][2]+0.1);
		pdxsw = int(mdb.PDB[0].RLen+0.001);
		if(mdb.PDB) delete[]mdb.PDB;
		mdb.PDB = NULL;
	}

	GTZDM_ROAD *ZM;
	_tcscpy(GH, L"K");
	ZM = new GTZDM_ROAD(NBPD, PD_array, &GH[0], 0, deshxsw);
	ZM->setINIT(3, 3, 1, 4, 10);

	//计算每个变坡点
	struct BPDdata
	{
		double cml;
		ACHAR ckml[30];
		double level;
		double Rshu,T,E;
		double PD,PC;
		double JiaZX;				
	} pddata[MAXBPDNUM];

	for (i = 0; i < NBPD; i++)
	{
		_tcscpy(pddata[i].ckml, ZM->XLC(PD_array[i][0]));   
		pddata[i].cml = PD_array[i][0];
		pddata[i].level = PD_array[i][1];
		pddata[i].Rshu =  PD_array[i][2];
		if (i < NBPD - 1 && PD_array[i + 1][0] - PD_array[i][0] > 0.0001)
		{
			pddata[i].PD = (PD_array[i + 1][1] - PD_array[i][1]) * 100.0 / (PD_array[i + 1][0] - PD_array[i][0]);
			ACHAR tmp[30];
			acdbRToS(pddata[i].PD, 2, 3, tmp);
			pddata[i].PD = _wtof(tmp);
			pddata[i].PC = PD_array[i + 1][0] - PD_array[i][0];
		}
		else
			pddata[i].PD = pddata[i].PC = 0.0 ;
	}

	//计算竖曲线T,E,JIaZX
	pddata[0].T = pddata[0].E = 0.0;
	for (i = 1; i < NBPD; i++)
	{
		pddata[i].T = pddata[i].Rshu*fabs(pddata[i - 1].PD - pddata[i].PD) / 200.0;
		pddata[i].E = ZM->DLEVEL(i, pddata[i].cml);
		pddata[i - 1].JiaZX = pddata[i - 1].PC - pddata[i].T - pddata[i - 1].T;
	}

	pddata[NBPD - 1].T = pddata[NBPD - 1].E = pddata[NBPD - 1].JiaZX = 0.0;
	//	pddata[i-1].JiaZX = pddata[i-1].PC - pddata[i].T - pddata[i-1].T;
	// 填表
	// 填写工程名称及段号

	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheet = NULL;

	totalnum = NBPD;
	int count;//每页count
	count = 0;
	for (i = 0; i < totalnum; i++)
	{   
		//读纵坡竖曲线数据
		if (pddata[i].cml + 0.1 < Scml)
		{
			m++;
			continue;
		}

		if (pddata[i].cml - 0.1 > Ecml) break;

		if((i-m)%29==0 || count >= 29) //写满一页
		{
			// 复制模板工作表然后填写
			if(flag)
			{
				m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
				pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
				m_worksheet.AttachDispatch(pWorksheet);
				m_worksheet.Select();
				_stprintf(shname, L"%s纵坡竖曲线%d",XZ,shnum);
				m_worksheet.SetName(shname);
				_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"K4")));	  
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(pageinfo));
				shnum++;
				row = 9;
				if(shnum>2)
					i--;
				count = 0;
			}
			if(shnum>1)
				flag = !flag;
		}
		pRange = m_worksheet.GetCell(row,1);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(long(i+1)));

		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pddata[i].ckml));

		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pddata[i].level));

		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);

		for(j=0; j<3; j++)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			if(j==0)
				m_range.SetValue(COleVariant(pddata[i].Rshu));
			if(j==1)
				m_range.SetValue(COleVariant(pddata[i].T));
			if(j==2)
				m_range.SetValue(COleVariant(pddata[i].E));
		}

		if(count<30 && row>9)
		{
			pRange = m_worksheet.GetCell(row+1,8);
			if(pddata[i-1].PD > -0.0001 )
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(pddata[i-1].PD));
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
			}
			else 
			{	 
				pddata[i-1].PD = -pddata[i-1].PD;
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(pddata[i-1].PD));
			}

			for(j=0; j<2; j++)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				if(j==0)
					m_range.SetValue(COleVariant(pddata[i-1].PC));
				if(j==1)
					m_range.SetValue(COleVariant(pddata[i-1].JiaZX));			
			}
		}

		row++;
		count++;
	}

}
//逐桩用地坐标表
void OutTAB::OUT_ZZYDZB()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\逐桩用地坐标表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	Fillzzydzb(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
}

//逐桩用地坐标面积表
void OutTAB::OUT_ZZYDZBMJ()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\逐桩用地坐标表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	Fillzzydzb(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
}

void OutTAB::Fillzzydzb(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int i, totalnum, row, col, m=0,j;
	long shnum;
	ACHAR shname[30], filepath[180], pageinfo[80];
	double sj[10];


	RoadStr = RoadStr + " " + DuanName;
	XLDataBase mdb;		
	JD_CENTER *pm;
	// 创建每一道路平面对象
	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);			
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}
	double Scml,Ecml,cml;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);
	ReadWorkdir();
	CString ZDKfname;
	CString name,Dir;
	name=PrjMdb;
	name.MakeUpper();
	ReadWorkdir();

	Dir=Cworkdir;
	Dir.MakeUpper();
	Cworkdir=Dir;
	name.Replace(Cworkdir,NULL);
	name.Replace(L"\\DATA\\", NULL);
	name.Replace(L".MDB", NULL);
	ZDKfname =Cworkdir + L"\\data\\" + L"hdmZDK." + name + L"." + RoadName;
	// 读总桩数
	FILE *fp;
	if((fp=_wfopen(ZDKfname, L"r"))==NULL)
	{
		AfxMessageBox(L"无法打开占地宽文件1!");
		//exit(0);
		return;
	}
	fwscanf(fp, L"%d",&totalnum);
	// 填写工程名称及段号
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));
	shnum = 1;

	double PerCml,PerZK,PerYK;
	ACHAR PerCkml[30];
	PerCml = PerZK = PerYK = 0.0;
	double AreaOnePage = 0.0;//每一页的用地面积
	double AllArea = 0.0;//总计用地面积

	double dml,zk,yk,zx,yx;
	int itmp;
	ACHAR ckml[30];
	bool IsFindFirstSta = false;//是否找到第一个桩号
	for(i=0;i<totalnum;i++)
	{   
		fwscanf(fp, L"%lf%lf%d%lf%lf",&cml,&dml,&itmp,&zk,&yk);
		_tcscpy(ckml,pm->XLC(cml));

		if(cml+0.1<Scml || itmp>0) 
		{
			m++;
			continue;
		}
		if(cml-0.1>Ecml) break;
		if((i-m)%22==0)
		{

			pRange = m_worksheet.GetCell(30,10);
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(AllArea));

			// 复制模板工作表然后填写
			m_app.Run(COleVariant(L"Macro1"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"逐桩用地坐标%d",shnum);
			m_worksheet.SetName(shname);
			_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"M4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			shnum++;

			row = 8;
			col = 1;

			AreaOnePage = 0.0;

			if (shnum>2)//如果不是第一张表，第一行数据是上一张表的最后一行数据
			{
				pRange = m_worksheet.GetCell(row,col);
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(PerCkml));
				for(j=0;j<8;j++)
				{
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
					m_range.SetValue(COleVariant(sj[j]));
				}

				row++;
			}
			else
			{
				PerCml = cml;
				PerZK = PerYK = 0.0;
				_tcscpy(PerCkml, L"\0");
			}

		}
		xlpoint PZ;
		double mx,my,fwj;
		PZ.lc = cml;
		pm->xlpoint_pz(&PZ);
		mx = PZ.x ,my = PZ.y,fwj=PZ.a;
		sj[0] = PZ.x;
		sj[1] = PZ.y;
		sj[2] = fabs(zk);
		sj[3] = PZ.x + fabs(zk)*cos(fwj-0.5*PI);
		sj[4] = PZ.y + fabs(zk)*sin(fwj-0.5*PI);
		sj[5] = fabs(yk);
		sj[6] = PZ.x + fabs(yk)*cos(fwj+0.5*PI);
		sj[7] = PZ.y + fabs(yk)*sin(fwj+0.5*PI);
		sj[8] = 0.5 * (PerZK + sj[2] + PerYK + sj[5]) * (cml-PerCml);
		AreaOnePage += sj[8];
		AllArea += sj[8];
		sj[9] = AreaOnePage;

		PerCml = cml;
		PerZK = sj[2];
		PerYK = sj[5];
		_tcscpy(PerCkml, ckml);

		pRange = m_worksheet.GetCell(row,col);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ckml));
		if (row==8)
		{
			for(j=0;j<8;j++)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(sj[j]));
			}
		}
		else
		{
			for(j=0;j<10;j++)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(sj[j]));
			}
		}

		row++;
	}

	pRange = m_worksheet.GetCell(30,10);
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(AllArea));

	//删除模板工作表
	pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	m_worksheet.Delete();
	fclose(fp);
}


//*************************************** 
//   Fillzpsqx   填写路基设计表
//*************************************** 
void OutTAB::OUT_LJSJB()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\路基设计表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	Fillljdatab(dlg.m_PrjMdb,dlg.RoadName,dlg.m_RoadStr,dlg.m_Sdml,dlg.m_Edml,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
}

//*************************************** 
//   Fillzpsqx   填写边沟排水沟设计表
//*************************************** 
void OutTAB::OUT_BGPSGSJB()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\边沟排水沟设计表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	Cal_BGPSG(dlg.m_PrjMdb,dlg.RoadName,dlg.m_Sdml,dlg.m_Edml);
	FillBgpsg(dlg.m_RoadStr,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
}
void OutTAB::FillBgpsg(CString RoadStr,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int i, totalnum, row, col, flag1, flag2,m=0,j;
	long shnum;
	ACHAR shname[30], filepath[180], pageinfo[80];
	double sj[8];
	CString name;
	// 	m_worksheet.Select();
	//	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	//	m_range.AttachDispatch(pRange);
	//	m_range.SetValue(COleVariant(RoadStr));
	flag1 = 0; shnum = 1;
	name = RoadStr + DuanName;

	double dml,zk,yk,zx,yx;
	int itmp;
	ACHAR ckml[30];
	row = 7;
	LPDISPATCH pWorksheet = NULL;
	for(i=0;i<NBGPSG;i++)
	{   
		if(i%32==0)
		{
			// 复制模板工作表然后填写
			m_app.Run(COleVariant(L"mycro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"边沟排水沟%d",shnum);
			m_worksheet.SetName(shname);
			_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"Q2")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A2")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(name));
			shnum++;
			row = 7;
		}

		pRange = m_worksheet.GetCell(row,2);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ckml));

		pRange = m_worksheet.GetCell(row,3);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZLJBYDesh));


		pRange = m_worksheet.GetCell(row,4);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZPJDesh));
		pRange = m_worksheet.GetCell(row,5);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZPJWid));
		pRange = m_worksheet.GetCell(row,6);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZSLTWid));
		pRange = m_worksheet.GetCell(row,7);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZGouToCenWid));
		pRange = m_worksheet.GetCell(row,8);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZGouDesh));
		pRange = m_worksheet.GetCell(row,9);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].ZGouHt));

		pRange = m_worksheet.GetCell(row,12);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YLJBYDesh));
		pRange = m_worksheet.GetCell(row,13);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YPJDesh));
		pRange = m_worksheet.GetCell(row,14);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YPJWid));
		pRange = m_worksheet.GetCell(row,15);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YSLTWid));
		pRange = m_worksheet.GetCell(row,16);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YGouToCenWid));
		pRange = m_worksheet.GetCell(row,17);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YGouDesh));
		pRange = m_worksheet.GetCell(row,18);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(BGPSG[i].YGouHt));
		row++;
	}
	//删除模板工作表
	pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	m_worksheet.Delete();
}
//-------------------------------------------------------------------------------------
void OutTAB::Cal_BGPSG(CString PrjMdb,CString RoadName,double Sdml,double Edml) //生成边沟排水沟数据表
{
	FILE *fpr;
	char fname[80],str[80];
	double  temp,tmp;
	int i,form_sum,k,itmp,j,n1,n2;
	int Ljdnum,Rjdnum;
	double Hc,Lwd,Rwd;
	double ZJH,ZPJH,RJH,RPJH,ZH;
	double deltax,deltay,aa;
	double dml,cml;
	double EarH,ZEarH,REarH;
	int NumL,NumR;
	double dtmp,xjj;
	ACHAR tmpstr[30];
	int ZDMXZ,YDMXZ,Y_ZDMXZ,Y_YDMXZ;
	double RHFL[10],RHFR[10],Y_RHFL[10],Y_RHFR[10];
	double Zt,Yt,Y_Zt,Y_Yt;
	int Lnpt,Rnpt,Y_Lnpt,Y_Rnpt;
	int ZGouPtNum,YGouPtNum,JSGouPtNum,IfHasZlink,IfHasYlink;
	double RPTL[200][2],RPTR[200][2];
	double ZGouPt[8][2],YGouPt[8][2];
	bool IfBriTunEnd;
	double Lgouh,Rgouh;
	int IsInsertHdm;
	double OffsetByXLZX = 0.0;//暂时

	XLDataBase mdb;		
	JD_CENTER *pm;
	// 创建每一道路平面对象
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);		
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}
	double Scml,Ecml;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);

	CString HDMdesfname;
	CString name,Dir;
	name=PrjMdb;
	name.MakeUpper();
	ReadWorkdir();

	Dir=Cworkdir;
	Dir.MakeUpper();
	Cworkdir=Dir;
	name.Replace(Cworkdir,NULL);
	name.Replace(L"\\DATA\\", NULL);
	name.Replace(L".MDB", NULL);
	HDMdesfname =Cworkdir + "\\data\\" + "hdmresult." + name + "." + RoadName;
	// 读总桩数

	if((fpr=_wfopen(HDMdesfname, L"r"))==NULL)
	{
		AfxMessageBox(L"无法打开横断面结果文件1!");
		exit(0);
	}

	fwscanf(fpr, L"%d" , &form_sum);
	form_sum = fabs(1.0*form_sum);
	if(RoadName!="主线")
		fwscanf(fpr, L"%lf%lf%lf%lf%lf%lf",&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp);

	if(form_sum>0 )
	{
		if(BGPSG) delete[]BGPSG;
		BGPSG = NULL;
		BGPSG =  new BGPSGdata[form_sum]; 		
	}
	else
	{
		ads_alert(L"桩号总数为0!\n");
		return;
	}

	NBGPSG = 0;
	for(k=0; k<form_sum; k++)
	{
		//读设计信息			 
		fwscanf(fpr , L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
			&cml,&dml,&IsInsertHdm,&IfHasYlink,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
			&ZEarH,&OffsetByXLZX,&xjj,&dtmp,&dtmp,&dtmp,tmpstr);

		REarH = ZEarH;
		IfHasZlink=0;
		IfHasYlink=0;
		//1.写左线左侧
		fwscanf(fpr, L"%d",&ZDMXZ);
		for(int j=0;j<3;j++)
			fwscanf(fpr, L"%lf",&dtmp);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf",&RHFL[2*j],&RHFL[2*j+1]);
		//左设计点
		fwscanf(fpr, L"%lf%lf%d",&Zt,&dtmp,&Lnpt);//换行
		for(j=1; j<=Lnpt; j++)
			fwscanf(fpr, L"%lf%lf",&RPTL[j][0],&RPTL[j][1]);

		//左水沟点
		fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&Lgouh,&itmp,&ZGouPtNum,&JSGouPtNum);
		for(j=0; j<ZGouPtNum; j++)
			fwscanf(fpr, L"%lf%lf",&ZGouPt[j][0],&ZGouPt[j][1]);
		//左截水沟
		for(j=0;j<JSGouPtNum;j++)
		{
			fwscanf(fpr, L"%lf%lf",&dtmp,&dtmp);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr, L"%d",&YDMXZ);
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf",&RHFR[2*j],&RHFR[2*j+1]);
		//右设计点
		fwscanf(fpr, L"%lf%lf%d ",&Yt,&dtmp,&Rnpt);
		for(j=1; j<=Rnpt; j++)
			fwscanf(fpr, L"%lf%lf",&RPTR[j][0],&RPTR[j][1]);

		//右水沟点
		fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&Rgouh,&itmp,&YGouPtNum,&JSGouPtNum);
		for(j=0; j<YGouPtNum; j++)
			fwscanf(fpr, L"%lf%lf",&YGouPt[j][0],&YGouPt[j][1]);
		//右截水沟
		for(j=0;j<JSGouPtNum;j++)
			fwscanf(fpr, L"%lf%lf",&dtmp,&dtmp);

		if(IfHasZlink )//左有关联道路
		{
			double dtmp;
			int itmp,jsgnum,num;

			fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				tmp			
				);
			//hdmdes[jcross].zlink->glRoadName = tmp;
			//1.写左线左侧
			fwscanf(fpr, L"%d",&itmp);
			//写路面点
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);
			//写路面点
			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
			for(j=0; j<itmp; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//左截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//2.写左线右侧
			//写路面点
			fwscanf(fpr, L"%d",&itmp);
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);

			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&num);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);

			//右水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}		
		}

		if(IfHasYlink)//右有关联道路
		{
			double dtmp;
			int itmp,jsgnum,num;

			fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
				&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
				tmp			
				);
			//hdmdes[jcross].zlink->glRoadName = tmp;
			//1.写左线左侧
			fwscanf(fpr, L"%d",&itmp);
			//写路面点
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);
			//写路面点
			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
			for(j=0; j<itmp; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//左截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//2.写左线右侧
			//写路面点
			fwscanf(fpr, L"%d",&itmp);
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);

			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&num);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);

			//右水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}		
		}
		if(cml>=Scml-0.001&&cml<=Ecml+0.001 && (ZDMXZ<3 || YDMXZ<3) && IsInsertHdm==0)
		{
			_tcscpy(BGPSG[NBGPSG].ckml,pm->XLC(cml));
			BGPSG[NBGPSG].ZPJWid=BGPSG[NBGPSG].ZPJDesh=BGPSG[NBGPSG].ZGouDesh=BGPSG[NBGPSG].ZLJBYDesh=BGPSG[NBGPSG].ZSLTWid=BGPSG[NBGPSG].ZGouHt=BGPSG[NBGPSG].ZGouToCenWid=0.0;
			BGPSG[NBGPSG].YPJWid=BGPSG[NBGPSG].YPJDesh=BGPSG[NBGPSG].YGouDesh=BGPSG[NBGPSG].YLJBYDesh=BGPSG[NBGPSG].YSLTWid=BGPSG[NBGPSG].YGouHt=BGPSG[NBGPSG].YGouToCenWid=0.0;

			BGPSG[NBGPSG].ZLJBYDesh = RHFL[9];

			if(Lnpt>4)
			{
				BGPSG[NBGPSG].ZPJWid = fabs(RPTL[Lnpt][0]-RPTL[5][0]);
				BGPSG[NBGPSG].ZPJDesh = RPTL[Lnpt][1]+ZEarH;
			}
			if(Zt>0.1)//挖方
			{
				BGPSG[NBGPSG].ZSLTWid = fabs(RPTL[9][0]-RPTL[8][0]);
				BGPSG[NBGPSG].ZGouToCenWid = 0.5*fabs(RPTL[6][0]+RPTL[7][0]);
				BGPSG[NBGPSG].ZGouDesh = 0.5*(RPTL[6][1]+RPTL[7][1])+ZEarH;
				BGPSG[NBGPSG].ZGouHt = fabs(0.5*(RPTL[6][1]+RPTL[7][1])-RPTL[5][1]);			   
			}
			else//填方
			{
				if(ZGouPtNum>5)
				{
					BGPSG[NBGPSG].ZSLTWid = fabs(ZGouPt[1][0]-ZGouPt[0][0]);
					BGPSG[NBGPSG].ZGouToCenWid = 0.5*fabs(ZGouPt[2][0]+ZGouPt[3][0]);
					//			BGPSG[NBGPSG].ZGouDesh = 0.5*(ZGouPt[2][1]+ZGouPt[3][1])+ZEarH;
					BGPSG[NBGPSG].ZGouDesh = Lgouh+ZEarH;
					BGPSG[NBGPSG].ZGouHt = fabs(0.5*(ZGouPt[2][1]+ZGouPt[3][1])-ZGouPt[1][1]);	
				}
			}

			BGPSG[NBGPSG].YLJBYDesh = RHFR[9];
			if(Rnpt>4)
			{
				BGPSG[NBGPSG].YPJWid = fabs(RPTR[Rnpt][0]-RPTR[5][0]);
				BGPSG[NBGPSG].YPJDesh = RPTR[Rnpt][1]+REarH;
			}
			if(Yt>0.1)//挖方
			{
				BGPSG[NBGPSG].YSLTWid = fabs(RPTR[9][0]-RPTR[8][0]);
				BGPSG[NBGPSG].YGouToCenWid = 0.5*(RPTR[6][0]+RPTR[7][0]);
				BGPSG[NBGPSG].YGouDesh = 0.5*(RPTR[6][1]+RPTR[7][1])+REarH;
				BGPSG[NBGPSG].YGouHt = fabs(0.5*(RPTR[6][1]+RPTR[7][1])-RPTR[5][1]);			   
			}
			else//填方
			{
				if(YGouPtNum>5)
				{
					BGPSG[NBGPSG].YSLTWid = fabs(YGouPt[1][0]-YGouPt[0][0]);
					BGPSG[NBGPSG].YGouToCenWid = 0.5*(YGouPt[2][0]+YGouPt[3][0]);
					//		BGPSG[NBGPSG].YGouDesh = 0.5*(YGouPt[2][1]+YGouPt[3][1])+REarH;//应加上浆厚
					BGPSG[NBGPSG].YGouDesh = Rgouh+REarH;
					BGPSG[NBGPSG].YGouHt = fabs(0.5*(YGouPt[2][1]+YGouPt[3][1])-YGouPt[1][1]);	
				}
			}
			NBGPSG++;
		}
	}
	fclose(fpr);
}

void OutTAB::Fillljdatab(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL, pWorksheet;
	Range m_range;
	long i, j, row, totalnum, shnum;
	FILE *fp;
	ACHAR filepath[180], shname[30], pageinfo[80];
	ACHAR prezh[30];
	ACHAR CKSML[80],CKEML[80],ck[50][80];//本页起始、终止现场里程
	int IfReadFile;
	double dtmp;

	XLDataBase mdb;		
	JD_CENTER *pm;
	//创建每一道路平面对象
	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);			
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	mdb.Read_XLDbs(PrjMdb, L"桩号序列",RoadName);
	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}
	double Scml,Ecml,cml,desh;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);
	// 创建纵面对象
	GTZDM_ROAD *AllZM,*ZZM,*YZM;
	AllZM=ZZM=YZM=NULL;

	AllZM = ReadZm(PrjMdb,RoadName, L"全幅",pm);

	ZZM = ReadZm(PrjMdb,RoadName, L"左幅",pm);

	YZM = ReadZm(PrjMdb,RoadName, L"右幅",pm);

	//横断面
	ReadWorkdir();
	CString HDMfname;
	CString name,Dir;
	name=PrjMdb;
	name.MakeUpper();
	ReadWorkdir();

	Dir=Cworkdir;
	Dir.MakeUpper();
	Cworkdir=Dir;
	name.Replace(Cworkdir,NULL);
	name.Replace(L"\\DATA\\", NULL);
	name.Replace(L".MDB", NULL);
	HDMfname =Cworkdir + "\\data\\" + "hdmresult." + name + "." + RoadName;


	int zory;
	int inf;
	bool IfOutYZM;
	bool IfYSheetStartPage=false;
	shnum = 1;
	zory=-1;
	IfOutYZM = true;
L20:
	fp = _wfopen(HDMfname, L"r");
	if(!fp)
	{
		ads_alert(L"无法打开横断面结果数据文件!");
		return;
	}

	fwscanf(fp, L"%ld",&totalnum);

	totalnum = fabs(1.0*totalnum);
	if(totalnum <= 0) 
	{
		AfxMessageBox(L"Eerror ljdatab.1");
		return;
	}

	if(RoadName!="主线")
	{
		for(j=0; j<6; j++)
			fwscanf(fp, L"%lf",&dtmp);
	}



	row = 0;
	OutTAB::LJSJBdata ljdata,preljdata ,tmpljdata;
	bool ifkm;
	//先生成全幅或左幅坡路基设计表
	for(i=0; i<totalnum; i++)
	{   	

		int IsInserthdm;
		inf = ReadACrossResultData(fp,pm,ljdata,AllZM,ZZM,YZM,zory,IsInserthdm);	

		if(IsInserthdm>0)//单独输出右幅坡范围内的表
			continue;

		if(zory>0 && inf<1)//单独输出右幅坡范围内的表
			continue;

		//	  continue;

		ifkm = false;
		cml = pm->TYLC(ljdata.ckml);

		if(cml+0.1<Scml) 
			continue;
		if(cml-0.1>Ecml) break;

		//判别是否在桩号序列中
		if(!CheckIfInSta(cml,mdb.NSTA,mdb.STA))
			continue;

		//桥隧中间桩要跳过

		//  if((ljdata.ZDMXZ==3&&ljdata.YDMXZ==3) || (ljdata.ZDMXZ==4&&ljdata.YDMXZ==4))
		//	  continue;

		_tcscpy(ck[row],ljdata.ckml);//记录里程
		if(row==0)
			_tcscpy(CKSML,ljdata.ckml);//本页起始里程

		if(i==0 || (shnum == 1 && row == 0) || (IfYSheetStartPage && row==0))  
			_tcscpy(prezh,ljdata.ckml);
		//else if((ljdata.flag==3.0 || ljdata.flag==4.0)&&ispage(prezh,ljdata.ckml))
		//	  else if(ispage(prezh,ljdata.ckml))//公里桩
		//	       goto a700;

		if(row>32 || glzhuang(ljdata.ckml))
		{
			if(i>0)
			{
				_tcscpy(CKEML,ck[row-1]);//本页终止里程

				DrawPm(pm,CKSML,CKEML,ck,row-1,m_worksheet);
				if(zory<0)
					DrawZm(pm,AllZM,CKSML,CKEML,ck,row-1,m_worksheet);
				else
					DrawZm(pm,YZM,CKSML,CKEML,ck,row-1,m_worksheet);
				//	DrawZm(pm,AllZM,CKSML,CKEML,ck,row-1,m_worksheet);

			}
		}
		if(i==0 || row>32 || (glzhuang(prezh)&&row>9)||(shnum == 1 && row == 0)||(IfYSheetStartPage && row==0))
		{
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"路基设计%d",shnum);
			if(IfYSheetStartPage)
				_stprintf(shname, L"(右幅坡)路基设计%d",shnum);
			m_worksheet.SetName(shname);

			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(RoadStr));
			_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"AD4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));

			shnum++;

			_tcscpy(CKSML,ck[row]);//本页起始里程
			if(i>0 && row>0)
			{
				tmpljdata = ljdata;
				ljdata = preljdata;
				ifkm = true;
			}
			row = 0;
			_tcscpy(ck[row],CKSML);//记录里程
			if(IfYSheetStartPage)
				IfYSheetStartPage = false;
		}	
		//ads_printf(L"row=%ld %lf %lf %s\n",row,ljdata.dmgc,ljdata.DeshNoVC,ljdata.ckml);
L100:	  
		pRange = m_worksheet.GetCell(row+9,1);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ljdata.ckml));

		pRange = m_worksheet.GetCell(row+9,6);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ljdata.dmgc));

		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ljdata.DeshNoVC));

		if(fabs(ljdata.dh) <= 1.0e-6)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else if(ljdata.dh> 0.0)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(ljdata.dh));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(-ljdata.dh));
		}

		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ljdata.DeshHasVC));
		if(fabs(ljdata.twh) <= 1.0e-6)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else if(ljdata.twh> 0.0)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(ljdata.twh));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(-ljdata.twh));
		}

		for(j=0;j<2;j++)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			if(j==0)
				m_range.SetValue(COleVariant(ljdata.Zljwid));
			if(j==1)
				m_range.SetValue(COleVariant(ljdata.Yljwid));

		}
		for(j=0;j<5;j++)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			if(j==0)
				m_range.SetValue(COleVariant(ljdata.Zljdh));
			if(j==1)
				m_range.SetValue(COleVariant(ljdata.Zcxddh));
			if(j==2)
				m_range.SetValue(COleVariant(ljdata.Mdh));
			if(j==3)
				m_range.SetValue(COleVariant(ljdata.Ycxddh));
			if(j==4)
				m_range.SetValue(COleVariant(ljdata.Yljdh));
		}

		if(fabs(ljdata.SGtwh) <= 1.0e-6)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else if(ljdata.SGtwh > 0.0)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(ljdata.SGtwh));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}
		else
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(-ljdata.SGtwh));
		}
		pRange = m_worksheet.GetCell(row+9,34);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ljdata.BL));
		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(ljdata.BR));

		row ++;
		_tcscpy(prezh,ljdata.ckml);
		preljdata = ljdata;

		if(ifkm)
		{
			ljdata = tmpljdata;
			ifkm = false;

			_tcscpy(ck[row],ljdata.ckml);//记录里程
			goto L100;

		}

	}

	_tcscpy(CKEML,ck[row-1]);//本页终止里程
	//	if(IfReadFile)
	//	{
	DrawPm(pm,CKSML,CKEML,ck,row-1,m_worksheet);
	if(zory<0)
		DrawZm(pm,AllZM,CKSML,CKEML,ck,row-1,m_worksheet);
	else
		DrawZm(pm,YZM,CKSML,CKEML,ck,row-1,m_worksheet);

	//	}
	fclose(fp);
	////////////////////////////////////////////////////////////////////////////////////////
	//	再生成右幅坡内的表
	if(IfOutYZM && YZM)
	{
		zory = 1;
		IfOutYZM = false;
		IfYSheetStartPage = true;
		goto L20;
	}

	pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));

	m_worksheet.AttachDispatch(pWorksheet);

	m_worksheet.Select();

	m_worksheet.Delete();

}
/*
void HdmDes::ReadACrossResultData(FILE *fpr,int iroad , int jcross)
{
char tmp[256];
fwscanf(fpr , L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
&road[iroad].hdmdes[jcross].cml,
&road[iroad].hdmdes[jcross].dml,
&road[iroad].hdmdes[jcross].IfHasZlink,
&road[iroad].hdmdes[jcross].IfHasYlink,
&road[iroad].hdmdes[jcross].LZDK,
&road[iroad].hdmdes[jcross].RZDK,
&road[iroad].hdmdes[jcross].Ldeltah,
&road[iroad].hdmdes[jcross].Rdeltah,
//	&road[iroad].hdmdes[jcross].Y_Ldeltah,
&road[iroad].hdmdes[jcross].ZDorS,
&road[iroad].hdmdes[jcross].YDorS,
&road[iroad].hdmdes[jcross].newLYd,
&road[iroad].hdmdes[jcross].newRYd,			
&road[iroad].hdmdes[jcross].xjj,
&road[iroad].hdmdes[jcross].glcml,
tmp
);
road[iroad].hdmdes[jcross].FLDname = tmp;
road[iroad].hdmdes[jcross].glRoadName = tmp;

CString GH;
double dml = road[iroad].pm->XLC(road[iroad].hdmdes[jcross].cml,GH);
_tcscpy(road[iroad].hdmdes[jcross].ckml,road[iroad].pm->LCchr(GH,dml,3));
//1.写左线左侧
fwscanf(fpr, L"%d",&road[iroad].hdmdes[jcross].ZDMXZ);
for(int j=0;j<3;j++)
fwscanf(fpr, L"%lf",&road[iroad].hdmdes[jcross].zlmhd[j]);

//写路面点
for(j=0;j<5;j++)
fwscanf(fpr, L"%lf%lf",&road[iroad].hdmdes[jcross].RHFL[2*j],&road[iroad].hdmdes[jcross].RHFL[2*j+1]);
//左设计点
fwscanf(fpr, L"%lf%lf%d",&road[iroad].hdmdes[jcross].Zt,&road[iroad].hdmdes[jcross].ZHc,&road[iroad].hdmdes[jcross].Lnpt);//换行

if(road[iroad].hdmdes[jcross].RPTL.length()>0)
road[iroad].hdmdes[jcross].RPTL.removeSubArray(0,road[iroad].hdmdes[jcross].RPTL.length()-1);

for(j=0; j<road[iroad].hdmdes[jcross].Lnpt; j++)
{
double x,y;
fwscanf(fpr, L"%lf%lf",&x,&y);
AcGePoint3d pt(x,y,0);
road[iroad].hdmdes[jcross].RPTL.append(pt);
}
//左水沟点
fwscanf(fpr, L"%d%lf%d%d%d",&road[iroad].hdmdes[jcross].LIfSetGouH,&road[iroad].hdmdes[jcross].LGouH,&road[iroad].hdmdes[jcross].LGouProperty,&road[iroad].hdmdes[jcross].LGouPtNum,&road[iroad].hdmdes[jcross].LJSGouPtNum);
for(j=0; j<road[iroad].hdmdes[jcross].LGouPtNum; j++)
fwscanf(fpr, L"%lf%lf",&road[iroad].hdmdes[jcross].Lgou[j*2],&road[iroad].hdmdes[jcross].Lgou[j*2+1]);
//左截水沟
for(j=0;j<road[iroad].hdmdes[jcross].LJSGouPtNum;j++)
{
fwscanf(fpr, L"%lf%lf",&road[iroad].hdmdes[jcross].LJSgou[j*2],&road[iroad].hdmdes[jcross].LJSgou[j*2+1]);
}
//2.写左线右侧
//写路面点
fwscanf(fpr, L"%d",&road[iroad].hdmdes[jcross].YDMXZ);
for(j=0;j<3;j++)
fwscanf(fpr, L"%lf",&road[iroad].hdmdes[jcross].ylmhd[j]);

for(j=0;j<5;j++)
fwscanf(fpr, L"%lf%lf",&road[iroad].hdmdes[jcross].RHFR[2*j],&road[iroad].hdmdes[jcross].RHFR[2*j+1]);
//右设计点
fwscanf(fpr, L"%lf%lf%d ",&road[iroad].hdmdes[jcross].Yt,&road[iroad].hdmdes[jcross].YHc,&road[iroad].hdmdes[jcross].Rnpt);
if(road[iroad].hdmdes[jcross].RPTR.length()>0)
road[iroad].hdmdes[jcross].RPTR.removeSubArray(0,road[iroad].hdmdes[jcross].RPTR.length()-1);

for(j=0; j<road[iroad].hdmdes[jcross].Rnpt; j++)
{
double x,y;
fwscanf(fpr, L"%lf%lf",&x,&y);
AcGePoint3d pt(x,y,0);
road[iroad].hdmdes[jcross].RPTR.append(pt);
}	//右水沟点
fwscanf(fpr, L"%d%lf%d%d%d",&road[iroad].hdmdes[jcross].RIfSetGouH,&road[iroad].hdmdes[jcross].RGouH,&road[iroad].hdmdes[jcross].RGouProperty,&road[iroad].hdmdes[jcross].RGouPtNum,&road[iroad].hdmdes[jcross].RJSGouPtNum);
for(j=0; j<road[iroad].hdmdes[jcross].RGouPtNum; j++)
fwscanf(fpr, L"%lf%lf",&road[iroad].hdmdes[jcross].Rgou[j*2],&road[iroad].hdmdes[jcross].Rgou[j*2+1]);
//右截水沟
for(j=0;j<road[iroad].hdmdes[jcross].RJSGouPtNum;j++)
fwscanf(fpr, L"%lf%lf",&road[iroad].hdmdes[jcross].RJSgou[j*2],&road[iroad].hdmdes[jcross].RJSgou[j*2+1]);

if(road[iroad].hdmdes[jcross].IfHasZlink )//左有关联道路
{
road[iroad].hdmdes[jcross].zlink = new CRSDAT;
fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
&road[iroad].hdmdes[jcross].zlink->cml,
&road[iroad].hdmdes[jcross].zlink->dml,
&road[iroad].hdmdes[jcross].zlink->newLYd,
&road[iroad].hdmdes[jcross].zlink->newRYd,
&road[iroad].hdmdes[jcross].zlink->xjj,
&road[iroad].hdmdes[jcross].zlink->FillArea,
&road[iroad].hdmdes[jcross].zlink->CutArea,
&road[iroad].hdmdes[jcross].zlink->Ldeltah,
&road[iroad].hdmdes[jcross].zlink->Rdeltah,

tmp			
);


road[iroad].hdmdes[jcross].zlink->glRoadName = tmp;
//1.写左线左侧
fwscanf(fpr, L"%d",&road[iroad].hdmdes[jcross].zlink->ZDMXZ);

//写路面点
for(j=0;j<3;j++)
fwscanf(fpr, L"%lf ",&road[iroad].hdmdes[jcross].zlink->zlmhd[j]);

//写路面点
for(j=0;j<5;j++)
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].zlink->RHFL[2*j],&road[iroad].hdmdes[jcross].zlink->RHFL[2*j+1]);

//左设计点
fwscanf(fpr, L"%lf%lf%d ",&road[iroad].hdmdes[jcross].zlink->Zt,&road[iroad].hdmdes[jcross].zlink->ZHc,&road[iroad].hdmdes[jcross].zlink->Lnpt);//换行

if(road[iroad].hdmdes[jcross].zlink->RPTL.length()>0)
road[iroad].hdmdes[jcross].zlink->RPTL.removeSubArray(0,road[iroad].hdmdes[jcross].zlink->RPTL.length()-1);

for(j=0; j<road[iroad].hdmdes[jcross].zlink->Lnpt; j++)
{
double x,y;
fwscanf(fpr, L"%lf%lf",&x,&y);
AcGePoint3d pt(x,y,0);
road[iroad].hdmdes[jcross].zlink->RPTL.append(pt);
}

//左水沟点
fwscanf(fpr, L"%d%lf%d%d%d ",&road[iroad].hdmdes[jcross].zlink->LIfSetGouH,&road[iroad].hdmdes[jcross].zlink->LGouH,&road[iroad].hdmdes[jcross].zlink->LGouProperty,&road[iroad].hdmdes[jcross].zlink->LGouPtNum,&road[iroad].hdmdes[jcross].zlink->LJSGouPtNum);
for(j=0; j<road[iroad].hdmdes[jcross].zlink->LGouPtNum; j++)
{
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].zlink->Lgou[j*2],&road[iroad].hdmdes[jcross].zlink->Lgou[j*2+1]);
}

//左截水沟
for(j=0;j<road[iroad].hdmdes[jcross].zlink->LJSGouPtNum;j++)
{
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].zlink->LJSgou[j*2],&road[iroad].hdmdes[jcross].zlink->LJSgou[j*2+1]);
}

//2.写左线右侧
//写路面点
fwscanf(fpr, L"%d",&road[iroad].hdmdes[jcross].zlink->YDMXZ);
for(j=0;j<3;j++)
fwscanf(fpr, L"%lf ",&road[iroad].hdmdes[jcross].zlink->ylmhd[j]);

for(j=0;j<5;j++)
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].zlink->RHFR[2*j],&road[iroad].hdmdes[jcross].zlink->RHFR[2*j+1]);
//右设计点
fwscanf(fpr, L"%lf%lf%d ",&road[iroad].hdmdes[jcross].zlink->Yt,&road[iroad].hdmdes[jcross].zlink->YHc,&road[iroad].hdmdes[jcross].zlink->Rnpt);

if(road[iroad].hdmdes[jcross].zlink->RPTR.length()>0)
road[iroad].hdmdes[jcross].zlink->RPTR.removeSubArray(0,road[iroad].hdmdes[jcross].zlink->RPTR.length()-1);

for(j=0; j<road[iroad].hdmdes[jcross].zlink->Rnpt; j++)
{
double x,y;
fwscanf(fpr, L"%lf%lf",&x,&y);
AcGePoint3d pt(x,y,0);
road[iroad].hdmdes[jcross].zlink->RPTR.append(pt);
}

//右水沟点
fwscanf(fpr, L"%d%lf%d%d%d ",&road[iroad].hdmdes[jcross].zlink->RIfSetGouH,&road[iroad].hdmdes[jcross].zlink->RGouH,&road[iroad].hdmdes[jcross].zlink->RGouProperty,&road[iroad].hdmdes[jcross].zlink->RGouPtNum,&road[iroad].hdmdes[jcross].zlink->RJSGouPtNum);
for(j=0; j<road[iroad].hdmdes[jcross].zlink->RGouPtNum; j++)
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].zlink->Rgou[j*2],&road[iroad].hdmdes[jcross].zlink->Rgou[j*2+1]);
//右截水沟
for(j=0;j<road[iroad].hdmdes[jcross].zlink->RJSGouPtNum;j++)
{
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].zlink->RJSgou[j*2],&road[iroad].hdmdes[jcross].zlink->RJSgou[j*2+1]);
}		
}

if(road[iroad].hdmdes[jcross].IfHasYlink )//左有关联道路
{
road[iroad].hdmdes[jcross].ylink = new CRSDAT;
fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
&road[iroad].hdmdes[jcross].ylink->cml,
&road[iroad].hdmdes[jcross].ylink->dml,
&road[iroad].hdmdes[jcross].ylink->newLYd,
&road[iroad].hdmdes[jcross].ylink->newRYd,
&road[iroad].hdmdes[jcross].ylink->xjj,
&road[iroad].hdmdes[jcross].ylink->FillArea,
&road[iroad].hdmdes[jcross].ylink->CutArea,
&road[iroad].hdmdes[jcross].ylink->Ldeltah,
&road[iroad].hdmdes[jcross].ylink->Rdeltah,
tmp			
);
road[iroad].hdmdes[jcross].ylink->glRoadName = tmp;
//1.写左线左侧
fwscanf(fpr, L"%d",&road[iroad].hdmdes[jcross].ylink->ZDMXZ);
//写路面点
for(j=0;j<3;j++)
fwscanf(fpr, L"%lf ",&road[iroad].hdmdes[jcross].ylink->zlmhd[j]);
//写路面点
for(j=0;j<5;j++)
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].ylink->RHFL[2*j],&road[iroad].hdmdes[jcross].ylink->RHFL[2*j+1]);
//左设计点
fwscanf(fpr, L"%lf%lf%d ",&road[iroad].hdmdes[jcross].ylink->Zt,&road[iroad].hdmdes[jcross].ylink->ZHc,&road[iroad].hdmdes[jcross].ylink->Lnpt);//换行

if(road[iroad].hdmdes[jcross].ylink->RPTL.length()>0)
road[iroad].hdmdes[jcross].ylink->RPTL.removeSubArray(0,road[iroad].hdmdes[jcross].ylink->RPTL.length()-1);

for(j=0; j<road[iroad].hdmdes[jcross].ylink->Lnpt; j++)
{
double x,y;
fwscanf(fpr, L"%lf%lf",&x,&y);
AcGePoint3d pt(x,y,0);
road[iroad].hdmdes[jcross].ylink->RPTL.append(pt);
}
//左水沟点
fwscanf(fpr, L"%d%lf%d%d%d ",&road[iroad].hdmdes[jcross].ylink->LIfSetGouH,&road[iroad].hdmdes[jcross].ylink->LGouH,&road[iroad].hdmdes[jcross].ylink->LGouProperty,&road[iroad].hdmdes[jcross].ylink->LGouPtNum,&road[iroad].hdmdes[jcross].ylink->LJSGouPtNum);
for(j=0; j<road[iroad].hdmdes[jcross].ylink->LGouPtNum; j++)
{
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].ylink->Lgou[j*2],&road[iroad].hdmdes[jcross].ylink->Lgou[j*2+1]);
}
//左截水沟
for(j=0;j<road[iroad].hdmdes[jcross].ylink->LJSGouPtNum;j++)
{
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].ylink->LJSgou[j*2],&road[iroad].hdmdes[jcross].ylink->LJSgou[j*2+1]);
}
//2.写左线右侧
//写路面点
fwscanf(fpr, L"%d",&road[iroad].hdmdes[jcross].ylink->YDMXZ);
for(j=0;j<3;j++)
fwscanf(fpr, L"%lf ",&road[iroad].hdmdes[jcross].ylink->ylmhd[j]);

for(j=0;j<5;j++)
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].ylink->RHFR[2*j],&road[iroad].hdmdes[jcross].ylink->RHFR[2*j+1]);
//右设计点
fwscanf(fpr, L"%lf%lf%d ",&road[iroad].hdmdes[jcross].ylink->Yt,&road[iroad].hdmdes[jcross].ylink->YHc,&road[iroad].hdmdes[jcross].ylink->Rnpt);
if(road[iroad].hdmdes[jcross].ylink->RPTR.length()>0)
road[iroad].hdmdes[jcross].ylink->RPTR.removeSubArray(0,road[iroad].hdmdes[jcross].ylink->RPTR.length()-1);

for(j=0; j<road[iroad].hdmdes[jcross].ylink->Rnpt; j++)
{
double x,y;
fwscanf(fpr, L"%lf%lf",&x,&y);
AcGePoint3d pt(x,y,0);
road[iroad].hdmdes[jcross].ylink->RPTR.append(pt);
}
//右水沟点
fwscanf(fpr, L"%d%lf%d%d%d ",&road[iroad].hdmdes[jcross].ylink->RIfSetGouH,&road[iroad].hdmdes[jcross].ylink->RGouH,&road[iroad].hdmdes[jcross].ylink->RGouProperty,&road[iroad].hdmdes[jcross].ylink->RGouPtNum,&road[iroad].hdmdes[jcross].ylink->RJSGouPtNum);
for(j=0; j<road[iroad].hdmdes[jcross].ylink->RGouPtNum; j++)
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].ylink->Rgou[j*2],&road[iroad].hdmdes[jcross].ylink->Rgou[j*2+1]);
//右截水沟
for(j=0;j<road[iroad].hdmdes[jcross].ylink->RJSGouPtNum;j++)
{
fwscanf(fpr, L"%lf%lf ",&road[iroad].hdmdes[jcross].ylink->RJSgou[j*2],&road[iroad].hdmdes[jcross].ylink->RJSgou[j*2+1]);
}		
}
road[iroad].hdmdes[jcross].DorS = 1;

}
*/
//ZorY=-1 取左侧设计高，1-取右侧设计高
//左右填挖是否相等 0-相等 1-不相等需要出表
//////////////////////////////////////////////////////////////////////////
//偏移量对这个函数的标高计算是否有影响？？？
//////////////////////////////////////////////////////////////////////////
int OutTAB::ReadACrossResultData(FILE *fpr,JD_CENTER *pm,LJSJBdata &ljdata,GTZDM_ROAD *allzm,GTZDM_ROAD *zzm,GTZDM_ROAD *yzm,int ZorY,int &IsInserthdm)
{
	ACHAR tmp[256];
	double dtmp,LYd,RYd,xjj;
	int DorS;
	int ZDMXZ,YDMXZ,Y_ZDMXZ,Y_YDMXZ,Lnpt,Rnpt;
	double zlmhd[3],ylmhd[3],Y_zlmhd[3],Y_ylmhd[3];
	double RHFL[10],RHFR[10],Y_RHFL[10],Y_RHFR[10];
	double ZHc,YHc,Y_ZHc,Y_YHc;
	double  ZPJD[2],YPJD[2],Y_ZPJD[2],Y_YPJD[2];
	double x,y;
	double Zljwid,Yljwid;
	int LGouProperty,LGouPtNum,LJSGouPtNum;
	int RGouProperty,RGouPtNum,RJSGouPtNum;
	int Y_LGouProperty,Y_LGouPtNum,Y_LJSGouPtNum;
	int Y_RGouProperty,Y_RGouPtNum,Y_RJSGouPtNum;
	double Lgou[36],Rgou[36],Y_Lgou[36],Y_Rgou[36];
	double LJSgou[36],RJSgou[36],Y_LJSgou[36],Y_RJSgou[36];
	double Zt,Yt,Y_Zt,Y_Yt;
	int Y_Lnpt,Y_Rnpt;
	int IfHasZlink,IfHasYlink;
	int IfOutTab=0;
	double OffsetByXLZX = 0.0;

	for(int i=0; i<2; i++)
	{
		ZPJD[i]=0.0;
		YPJD[i]=0.0;
		Y_ZPJD[i]=0.0;
		Y_YPJD[i]=0.0;
	}
	DorS=1;

	fwscanf(fpr , L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
		&ljdata.cml,
		&dtmp,
		&IsInserthdm,
		&IfHasYlink,
		&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&LYd,&OffsetByXLZX,&xjj,&dtmp,&dtmp,&dtmp,tmp
		);

	RYd = LYd;

	IfHasZlink = 0;
	IfHasYlink = 0;
	CString GH;
	double dml = pm->XLC(ljdata.cml,GH);
	_tcscpy(ljdata.ckml,pm->LCchr(GH,dml,3));
	//1.写左线左侧
	fwscanf(fpr, L"%d",&ljdata.ZDMXZ);
	int j;
	for(j=0;j<3;j++)
		fwscanf(fpr, L"%lf",&zlmhd[j]);

	//写路面点
	for(j=0;j<5;j++)
		fwscanf(fpr, L"%lf%lf",&RHFL[2*j],&RHFL[2*j+1]);
	//左设计点
	fwscanf(fpr, L"%lf%lf%d",&dtmp,&ZHc,&Lnpt);//换行
	for(j=0; j<Lnpt; j++)
		fwscanf(fpr, L"%lf%lf",&ZPJD[0],&ZPJD[1]);

	int itmp;
	//左水沟点
	fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&dtmp,&LGouProperty,&LGouPtNum,&LJSGouPtNum);
	for(j=0; j<LGouPtNum; j++)
		fwscanf(fpr, L"%lf%lf",&Lgou[j*2],&Lgou[j*2+1]);

	//左截水沟
	for(j=0;j<LJSGouPtNum;j++)
		fwscanf(fpr, L"%lf%lf",&LJSgou[j*2],&LJSgou[j*2+1]);

	//2.写左线右侧
	//写路面点
	fwscanf(fpr, L"%d",&ljdata.YDMXZ);
	for(j=0;j<3;j++)
		fwscanf(fpr, L"%lf",&ylmhd[j]);

	for(j=0;j<5;j++)
		fwscanf(fpr, L"%lf%lf",&RHFR[2*j],&RHFR[2*j+1]);
	//右设计点
	fwscanf(fpr, L"%lf%lf%d ",&Yt,&YHc,&Rnpt);
	for(j=0; j<Rnpt; j++)
		fwscanf(fpr, L"%lf%lf",&YPJD[0],&YPJD[1]);
	//右水沟点
	fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&dtmp,&RGouProperty,&RGouPtNum,&RJSGouPtNum);
	for(j=0; j<RGouPtNum; j++)
		fwscanf(fpr, L"%lf%lf",&Rgou[j*2],&Rgou[j*2+1]);
	//右截水沟
	for(j=0;j<RJSGouPtNum;j++)
		fwscanf(fpr, L"%lf%lf",&RJSgou[j*2],&RJSgou[j*2+1]);

	if(DorS==2)//双路面
	{
		fwscanf(fpr, L"%d",&Y_ZDMXZ);
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf",&Y_zlmhd[j]);

		//3.写右线左侧
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf",&Y_RHFL[2*j],&Y_RHFL[2*j+1]);
		//右左设计点
		fwscanf(fpr, L"%lf%lf%d",&Y_Zt,&Y_ZHc,&Y_Lnpt);
		for(j=0;j<Y_Lnpt;j++)
			fwscanf(fpr, L"%lf%lf",&Y_ZPJD[0],&Y_ZPJD[1]);

		//		fwscanf(fpr, L"%lf%lf",&Y_RPTL[j].x,&Y_RPTL[j].y);
		//右左水沟点
		fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&dtmp,&Y_LGouProperty,&Y_LGouPtNum,&Y_LJSGouPtNum);
		for(j=0; j<Y_LGouPtNum; j++)
			fwscanf(fpr, L"%lf%lf",&Y_Lgou[j*2],&Y_Lgou[j*2+1]);
		//右左截水沟
		for(j=0;j<Y_LJSGouPtNum;j++)
			fwscanf(fpr, L"%lf%lf",&Y_LJSgou[j*2],&Y_LJSgou[j*2+1]);

		//3.写右线右侧
		fwscanf(fpr, L"%d",&Y_YDMXZ);
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf",&Y_ylmhd[j]);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf",&Y_RHFR[2*j],&Y_RHFR[2*j+1]);

		//右右设计点
		fwscanf(fpr, L"%lf%lf%d",&Y_Yt,&Y_YHc,&Y_Rnpt);
		for(j=0;j<Y_Rnpt;j++)
			fwscanf(fpr, L"%lf%lf",&Y_YPJD[0],&Y_YPJD[1]);
		//右右水沟点
		fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&dtmp,&Y_RGouProperty,&Y_RGouPtNum,&Y_RJSGouPtNum);
		for(j=0; j<Y_RGouPtNum; j++)
			fwscanf(fpr, L"%lf%lf",&Y_Rgou[j*2],&Y_Rgou[j*2+1]);
		//右右截水沟
		for(j=0;j<Y_RJSGouPtNum;j++)
			fwscanf(fpr, L"%lf%lf",&Y_RJSgou[j*2],&Y_RJSgou[j*2+1]);
	}

	if(IfHasZlink)//左有关联道路
	{
		//	hdmdes[jcross].zlink = new CRSDAT;
		double dtmp;
		int itmp,jsgnum,num;

		fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			&dtmp,
			tmp			
			);
		//hdmdes[jcross].zlink->glRoadName = tmp;
		//1.写左线左侧
		fwscanf(fpr, L"%d",&itmp);
		//写路面点
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf ",&dtmp);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//左设计点
		fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
		for(j=0; j<itmp; j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//左水沟点
		fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
		{
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		}
		//左截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr, L"%d",&itmp);
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf ",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//右设计点
		fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&num);
		for(j=0; j<num; j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);

		//右水沟点
		fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//右截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		}		
	}

	if(IfHasYlink)//右有关联道路
	{
		double dtmp;
		int itmp,jsgnum,num;

		fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
			&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
			tmp			
			);
		//hdmdes[jcross].zlink->glRoadName = tmp;
		//1.写左线左侧
		fwscanf(fpr, L"%d",&itmp);
		//写路面点
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf ",&dtmp);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//左设计点
		fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
		for(j=0; j<itmp; j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//左水沟点
		fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
		{
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		}
		//左截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr, L"%d",&itmp);
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf ",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//右设计点
		fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&num);
		for(j=0; j<num; j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);

		//右水沟点
		fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
		for(j=0; j<num; j++)
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		//右截水沟
		for(j=0;j<jsgnum;j++)
		{
			fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
		}		
	}


	///付值
	//战时只考虑左副设计标高	
	double cml;
	cml = ljdata.cml;
	if(ZorY<0)
	{
		ljdata.DeshHasVC = CalDesH(cml,zzm,allzm);
		ljdata.DeshNoVC = CalDesHWithoutVerCurve(cml,zzm,allzm);
	}
	else
	{
		ljdata.DeshHasVC = CalDesH(cml,yzm,allzm);
		ljdata.DeshNoVC = CalDesHWithoutVerCurve(cml,yzm,allzm);
	}
	ljdata.dh = ljdata.DeshHasVC - ljdata.DeshNoVC;

	if(DorS==1)
	{
		ljdata.dmgc = LYd;
		// 		if(ZorY<0)
		// 		    ljdata.twh = ZHc;//战时只考虑左幅填挖高
		// 		else
		//          ljdata.twh = YHc;//战时只考虑左幅填挖高
		ljdata.twh = ljdata.DeshHasVC - ljdata.dmgc;
		if(ZorY>0 && yzm && cml>yzm->CM[0]-0.001 && cml<yzm->CM[(yzm->NCM-2)*5])
		{
			//	ads_printf(L"cml=%lf %lf %lf\n",cml,yzm->CM[0],yzm->CM[(yzm->NCM-2)*5]);
			IfOutTab = 1;
		}

		double zdesh = CalDesH(cml,zzm,allzm);
		ljdata.Zljdh = RHFL[9] - zdesh;
		ljdata.Zcxddh = RHFL[5] - zdesh;
		ljdata.Mdh = RHFL[3] - zdesh;
		double ydesh = CalDesH(cml,yzm,allzm);
		ljdata.Yljdh = RHFR[9] - ydesh;
		ljdata.Ycxddh = RHFR[5] - ydesh;
		ljdata.Mdh = RHFR[3] - ydesh;

		//    ljdata.Zljwid = fabs(ZPJD[0]);
		//	if(LGouPtNum>0)
		//   ljdata.Zljwid = fabs(Lgou[(LGouPtNum-1)*2]);
		ljdata.Zljwid = fabs(RHFL[8]);
		//  if(LJSGouPtNum>0)
		//      ljdata.Zljwid = fabs(LJSgou[(LJSGouPtNum-1)*2]);
		ljdata.Yljwid = fabs(RHFR[8]);
		//	if(RGouPtNum>0)
		//     ljdata.Yljwid = Rgou[(RGouPtNum-1)*2];
		//  if(RJSGouPtNum>0)
		//    ljdata.Yljwid = RJSgou[(RJSGouPtNum-1)*2];
		//////////////////////////////////////////////////////////////////////////
		double LMHDOnXLZX = 0.0;//线路中线处的路面厚度
		if (OffsetByXLZX>0.001)//路基中线在线路中线右侧
		{
			if (OffsetByXLZX < RHFL[4])
				LMHDOnXLZX = zlmhd[0];
			else if (OffsetByXLZX<RHFL[6])
				LMHDOnXLZX = zlmhd[1];
			else if (OffsetByXLZX<RHFL[8]+0.001)
				LMHDOnXLZX = zlmhd[2];
			else
			{
				LMHDOnXLZX = 0.0;
				ads_printf(L"\n%s处路基中线相当于线路中线的偏移量已超过路面宽，请检查路面宽数据!",ljdata.ckml);
			}
			ljdata.SGtwh =  ljdata.twh - LMHDOnXLZX;
			ljdata.BL = fabs(ZPJD[0]+OffsetByXLZX);
			ljdata.BR = fabs(YPJD[0]+OffsetByXLZX);
		}
		else if (OffsetByXLZX<-0.001)
		{
			if (-OffsetByXLZX < RHFR[4])
				LMHDOnXLZX = ylmhd[0];
			else if (-OffsetByXLZX<RHFR[6])
				LMHDOnXLZX = ylmhd[1];
			else if (-OffsetByXLZX<RHFR[8]+0.001)
				LMHDOnXLZX = ylmhd[2];
			else
			{
				LMHDOnXLZX = 0.0;
				ads_printf(L"\n%s处路基中线相当于线路中线的偏移量已超过路面宽，请检查路面宽数据!",ljdata.ckml);
			}
			ljdata.SGtwh =  ljdata.twh - LMHDOnXLZX;
			ljdata.BL = fabs(ZPJD[0]+OffsetByXLZX);
			ljdata.BR = fabs(YPJD[0]+OffsetByXLZX);
		}
		else
		{
			if(ZorY<0)
				ljdata.SGtwh =  ljdata.twh - zlmhd[0];
			else
				ljdata.SGtwh =  ljdata.twh - ylmhd[0];
			ljdata.BL = fabs(ZPJD[0]);
			ljdata.BR = YPJD[0];
		}
		//////////////////////////////////////////////////////////////////////////

		if(ljdata.ZDMXZ==3 || ljdata.ZDMXZ==4)//左侧断面为桥中或隧中
		{
			ljdata.twh = 0.0;//战时只考虑左幅填挖高
			ljdata.Zljdh = 0.0;
			ljdata.Zcxddh = 0.0;
			ljdata.Mdh = 0.0;
			ljdata.Zljwid = 0.0;
			ljdata.SGtwh =  0.0;
			ljdata.BL = 0.0;					
		}

		if(ljdata.YDMXZ==3 || ljdata.YDMXZ==4)//左侧断面为桥中或隧中
		{
			ljdata.twh = 0.0;//战时只考虑左幅填挖高
			ljdata.Yljdh = 0.0;
			ljdata.Ycxddh = 0.0;
			ljdata.Mdh = 0.0;
			ljdata.Yljwid = 0.0;
			ljdata.SGtwh =  0.0;
			ljdata.BR = 0.0;					
		}


	}
	else
	{
		if(xjj<-0.0001)//左线分离
		{
			ljdata.dmgc = RYd;
			ljdata.twh = Y_ZHc;//战时只考虑左幅填挖高
			if(fabs(Y_ZHc-Y_YHc)>0.001)
				ads_printf(L"%s右关联断面左右幅填挖高不一致，取左幅填挖高!\n",ljdata.ckml,Y_ZHc,Y_YHc);
			ljdata.Zljdh = Y_RHFL[9] - ljdata.DeshHasVC;
			ljdata.Zcxddh = Y_RHFL[5] - ljdata.DeshHasVC;
			ljdata.Mdh = Y_RHFL[3] - ljdata.DeshHasVC;
			double ydesh = CalDesH(cml,yzm,allzm);
			ljdata.Yljdh = Y_RHFR[9] - ydesh;
			ljdata.Ycxddh = Y_RHFR[5] - ydesh;
			ljdata.Mdh = Y_RHFR[3] - ydesh;
			ljdata.Zljwid = fabs(Y_RHFL[8]) + fabs(xjj);
			ljdata.Yljwid = Y_RHFR[8];
			ljdata.SGtwh =  ljdata.twh - Y_zlmhd[0];
			ljdata.BL = fabs(ZPJD[0]) + fabs(xjj);
			ljdata.BR = Y_YPJD[0] ;			 



		}
		else
		{
			ljdata.dmgc = LYd;
			ljdata.twh = ZHc;//战时只考虑左幅填挖高
			if(fabs(ZHc-YHc)>0.001)
				ads_printf(L"%s左右幅填挖高不一致，取左幅填挖高!\n",ljdata.ckml,ZHc,YHc);
			ljdata.Zljdh = RHFL[9] - ljdata.DeshHasVC;
			ljdata.Zcxddh = RHFL[5] - ljdata.DeshHasVC;
			ljdata.Mdh = RHFL[3] - ljdata.DeshHasVC;
			double ydesh = CalDesH(cml,yzm,allzm);
			ljdata.Yljdh = RHFR[9] - ydesh;
			ljdata.Ycxddh = RHFR[5] - ydesh;
			ljdata.Mdh = RHFR[3] - ydesh;
			ljdata.Zljwid = fabs(RHFL[8]);//fabs(ZPJD[0]);
			//if(LGouPtNum>0)
			//	ljdata.Zljwid = Lgou[(LGouPtNum-1)*2];
			//if(LJSGouPtNum>0)
			//	ljdata.Zljwid = LJSgou[(LJSGouPtNum-1)*2];
			ljdata.Yljwid = RHFR[8];
			//Y_YPJD[0];
			//if(Y_RGouPtNum>0)
			//	ljdata.Yljwid = Y_Rgou[(Y_RGouPtNum-1)*2] + xjj;
			//if(Y_RJSGouPtNum>0)
			//	ljdata.Yljwid = Y_RJSgou[(Y_RJSGouPtNum-1)*2] + xjj;
			ljdata.SGtwh =  ljdata.twh - zlmhd[0];
			ljdata.BL = fabs(ZPJD[0]);
			ljdata.BR = Y_YPJD[0] + xjj;
		}
	}
	return IfOutTab;
}

GTZDM_ROAD *OutTAB::ReadZm(CString mdbname,CString RoadName,CString XZ,JD_CENTER *pm)
{
	XLDataBase DBS;
	double PD_array[MAXBPDNUM][4],*dmx=NULL;
	int deshxsw,pdxsw;
	ACHAR GH[8];
	GTZDM_ROAD *ZM;

	ZM = NULL;
	pzLinep = pm;
	int res = DBS.Read_XLDbs(mdbname, L"坡度表",RoadName,XZ);
	int res1 = DBS.Read_XLDbs(mdbname, L"纵断链表",RoadName);
	if(res&&res1)//读坡度
	{
		int NBPD = DBS.NPD;
		int i;
		for(i=0;i<NBPD;i++)
		{
			PD_array[i][0] = TYLC1(DBS.PDB[i].ml,DBS.NZDL,DBS.ZDLArray);
			PD_array[i][1] = DBS.PDB[i].Level;
			PD_array[i][2] = DBS.PDB[i].Rshu;			
			PD_array[i][3] = 0.0;
		} 

		if(DBS.PDB) 
		{
			deshxsw = (int)(PD_array[0][2]+0.1);
			pdxsw = int(DBS.PDB[0].RLen+0.001);
			if(DBS.PDB) delete[]DBS.PDB;
			DBS.PDB = NULL;
		}
		if(NBPD>0)
		{
			pzLinep = pm;
			_tcscpy(pzLinep->mdbname , mdbname);
			_tcscpy(pzLinep->RoadName , RoadName); 
			_tcscpy(GH, L"K");
			ZM = new GTZDM_ROAD(NBPD,PD_array,&GH[0],0,deshxsw);
			ZM->setINIT(3,3,1,4,10); 
			ZM->DLNum = DBS.NZDL;
			for(i=0;i<DBS.NZDL;i++)
				ZM->setDLM(i,DBS.ZDLArray[i].BLC,DBS.ZDLArray[i].ELC,DBS.ZDLArray[i].BGH,DBS.ZDLArray[i].EGH);
			ZM->Property = XZ;
		}

	}
	else
	{
		//		ads_printf(L"\n读坡度表失败!");
		return NULL;
	}
	return ZM;
}

double OutTAB::CalDesH(double cml,GTZDM_ROAD *ZM1,GTZDM_ROAD *AllZM)
{
	double desh;
	double sml,eml;

	desh = 0.0;
	if(ZM1)
	{
		sml = ZM1->CM[0];//起始连续里程
		eml =  ZM1->CM[(ZM1->NCM-2)*5];
		if (cml>sml-0.001 && cml<eml+0.01) 
		{
			desh = ZM1->rdes_level(cml);//从左幅取
			if(desh>-0.9)
				return desh;
		}
	}
	if(AllZM)
		desh = AllZM->rdes_level(cml);
	return desh;
}


double OutTAB::CalDesHWithoutVerCurve(double cml,GTZDM_ROAD *ZM1,GTZDM_ROAD *AllZM)
{
	double desh;
	double sml,eml;

	desh = 0.0;
	if(ZM1)
	{
		sml = ZM1->CM[0];//起始连续里程
		eml =  ZM1->CM[(ZM1->NCM-2)*5];
		if (cml>sml-0.001 && cml<eml+0.01) 
		{
			desh = ZM1->rdes_levelWithoutVerCurve(cml);//从左幅取
			if(desh>-0.9)
				return desh;
		}
	}
	if(AllZM)
		desh = AllZM->rdes_levelWithoutVerCurve(cml);
	return desh;
}
bool OutTAB::ispage(ACHAR *str1,ACHAR *str2)
{
	double ml;
	ACHAR ck[50];
	int m,n;
	split_ckml(str1,&ml,ck);
	m=int((ml+0.01)/1000.0);
	split_ckml(str2,&ml,ck);
	n=int((ml+0.01)/1000.0);
	if(n>m) return 1;
	else return 0;
}
// 判断是否为整公里桩  extern function
bool OutTAB::glzhuang(ACHAR *str)
{
	double ml;
	ACHAR ck[50];
	split_ckml(str,&ml,ck);
	if(fmod(ml+0.0001 , 1000.0)<0.1) return 1;
	else return 0;
}

void OutTAB::DrawPm(JD_CENTER *pm,ACHAR CKSML[256],ACHAR CKEML[256],ACHAR ckml[50][80],int nRow,_Worksheet wsMysheet)
{
	double sml,eml,fsml,feml,XZ,BXZ,EXZ,ZX,fsml1,feml1,bx,by;//本页的起始和终止里程
	float sy,ey,sx,ex;
	sml = pm->TYLC(CKSML); 
	eml = pm->TYLC(CKEML);
	if(eml-sml<0)
		return;//不画
	int is,ie;
	is = pm->GetXYNoFromTYLC(sml);
	ie = pm->GetXYNoFromTYLC(eml);
	//ads_printf();
	//从is到ie绘平面分段

	ACHAR scp[10];
	_tcscpy(scp, L"B9");
	Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
	float rowh = _variant_t(rgMyRge.GetRowHeight());

	for(int i=is; i<=ie; i++)
	{
		//获取分段起始里程，终止里程，分段性质,转向（起终分段里程特殊处理）
		if(i==is || i==0)
			fsml = sml;
		else if(i>0)
			fsml = pm->XYArray[i][6];

		if(i>0)
			fsml1 = pm->XYArray[i][6];
		else
			fsml1 = pm->XYArray[i][4];

		if(i==ie)
			feml = eml;
		else
			feml = pm->XYArray[i+1][6];

		if(i<pm->XYNum-1)
			feml1 = pm->XYArray[i+1][6];
		else
			feml1 = pm->XYArray[i][6];



		XZ =  pm->XYArray[i][0];//线元性质
		ZX = pm->XYArray[i][4];//转向
		//计算起终行
		int startRow = TranCmlToRow(pm,fsml,ckml,nRow)+9;
		int row,endRow;
		row = TranCmlToRow(pm,feml,ckml,nRow);
		if(row==nRow)
			row++;
		endRow = row + 9;

		if(endRow<startRow)
			return;

		if(i>is)
		{
			bx = ex;
			by = ey;			
		}

		sy = RowsToFloat(startRow,wsMysheet);
		ey = RowsToFloat(endRow,wsMysheet);

		if(i>=is&&i<=ie)
		{
			sy+=0.5*rowh;
			ey+=0.5*rowh;			
		}

		if(i>0)
			BXZ = pm->XYArray[i-1][0];
		else
			BXZ = pm->XYArray[i][0];

		if(i<pm->XYNum)
			EXZ = pm->XYArray[i+1][0];
		else
			EXZ = pm->XYArray[i][0];

		int idai;
		double dml,bm;
		dml = pm->XLC1(feml);
		bm = dml - (int)((dml+0.001)/1000.0)*1000.0;
		ACHAR bmstr[80];
		if(bm>99.999)
			_stprintf(bmstr, L"+%0.3lf",bm);
		else if(bm>9.999)
			_stprintf(bmstr, L"+0%0.3lf",bm);
		else if(bm>0.999)
			_stprintf(bmstr, L"+00%0.3lf",bm);
		else
			_stprintf(bmstr, L"+000%0.3lf",bm);

		ACHAR chr[20];
		if(i<pm->XYNum-1)
			pm->GetQxTZDStr(i+1,chr);
		else
			pm->GetQxTZDStr(i,chr);

		Shapes myShapes;
		myShapes=wsMysheet.GetShapes();
		if(XZ==1.0)//直线
		{

			float left,width;//绘图域左距,宽度
			left=_variant_t(rgMyRge.GetLeft());
			width=_variant_t(rgMyRge.GetWidth());
			sx = ex = left+width;
			if(i>is)
				myShapes.AddLine(bx,by,sx,sy);

			myShapes.AddLine(sx,sy,ex,ey);
			//画标注线
			if(fabs(feml-feml1)<0.1)//标注
			{
				myShapes.AddLine(ex,ey,ex-0.5*width,ey);//画横线

				DrawText(1,ex-0.5*width,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,	chr);//注里程
				DrawText(1,ex-width,ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
			}

			if(fabs(sy-ey)>3*rowh)//标注A-
			{
				float ax,ay;
				ax = sx;
				ay = 0.5*(sy+ey);
				DrawText(3,ax,ay-rowh,width,0.5*rowh,10,myShapes, L"R—∞");//注里程	lbw			
			}
		}
		else if(XZ==3.0 || XZ==5.0)//前缓
		{

			if(ZX<0.0)//左转
			{
				ACHAR scp[10];
				_tcscpy(scp, L"B9");
				Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
				float rowh = _variant_t(rgMyRge.GetRowHeight());
				float left,width;//绘图域左距,宽度
				left=_variant_t(rgMyRge.GetLeft());
				width=_variant_t(rgMyRge.GetWidth());

				if((feml1-fsml1)>0.001)
					sx = left + width - (fsml-fsml1)*0.5*width/(feml1-fsml1) ;
				else
					sx = left + width;


				if((feml1-fsml1)>0.001)
					ex = left + 0.5*width + (feml1-feml)*0.5*width/(feml1-fsml1) ;
				else
					ex = left + 0.5*width;	
				if(i>is)
					myShapes.AddLine(bx,by,sx,sy);

				myShapes.AddLine(sx,sy,ex,ey);
				//画标注线
				if(fabs(feml-feml1)<0.1)//标注
				{
					myShapes.AddLine(ex,ey,ex+0.5*width,ey);//画横线

					DrawText(1,ex,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,chr);//注里程
					DrawText(1,ex,ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
				}

				if(fabs(sy-ey)>3*rowh)//标注A-
				{
					float ax,ay;
					ax = 0.5*(sx+ex);
					ay = 0.5*(sy+ey);
					ACHAR str[30];
					_stprintf(str, L"A—%0.2lf",pm->XYArray[i][1]);
					DrawText(3,ax,ay-rowh,width,0.5*rowh,10,myShapes,str);//注里程				
				}
			}
			else
			{
				ACHAR scp[10];
				_tcscpy(scp, L"C9");
				Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
				float rowh = _variant_t(rgMyRge.GetRowHeight());
				float left,width;//绘图域左距,宽度
				left=_variant_t(rgMyRge.GetLeft());
				width=_variant_t(rgMyRge.GetWidth());

				if((feml1-fsml1)>0.001)
					sx = left + (fsml-fsml1)*0.5*width/(feml1-fsml1) ;
				else
					sx = left ;

				if((feml1-fsml1)>0.001)
					ex = left + 0.5*width - (feml1-feml)*0.5*width/(feml1-fsml1) ;
				else
					ex = left + 0.5*width;	
				if(i>is)
					myShapes.AddLine(bx,by,sx,sy);//画横线*/

				myShapes.AddLine(sx,sy,ex,ey);//画横线*/


				if(fabs(feml-feml1)<0.1)//标注
				{
					myShapes.AddLine(ex,ey,ex-0.5*width,ey);//画横线					
					DrawText(1,ex-0.5*width,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,	chr);//注里程				
					DrawText(1,ex-1.5*width,ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
				}

				if(fabs(sy-ey)>3*rowh)//标注A-
				{
					float ax,ay;
					ax = 0.5*(sx+ex);
					ay = 0.5*(sy+ey);
					ACHAR str[30];
					_stprintf(str, L"A—%0.2lf",pm->XYArray[i][1]);
					DrawText(3,ax-0.5*width,ay-rowh,width,0.5*rowh,10,myShapes,str);//注里程				
				}
			}			
		}
		else if(XZ==4.0 || XZ==6.0)
		{

			if(ZX<0.0)//左转
			{
				ACHAR scp[10];
				_tcscpy(scp, L"B9");
				Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
				float rowh = _variant_t(rgMyRge.GetRowHeight());
				float left,width;//绘图域左距,宽度
				left=_variant_t(rgMyRge.GetLeft());
				width=_variant_t(rgMyRge.GetWidth());

				if((feml1-fsml1)>0.001)
					sx = left + 0.5*width + (fsml-fsml1)*0.5*width/(feml1-fsml1) ;
				else
					sx = left + 0.5*width;	

				if((feml1-fsml1)>0.001)
					ex = left + width - (feml1-feml)*0.5*width/(feml1-fsml1) ;
				else
					ex = left + width;

				if(i>is)
					myShapes.AddLine(bx,by,sx,sy);//画横线*/

				myShapes.AddLine(sx,sy,ex,ey);//画横线*/

				if(fabs(feml-feml1)<0.1)//标注
				{
					myShapes.AddLine(ex,ey,ex+0.5*width,ey);//画横线

					DrawText(1,ex,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,chr);//注里程
					DrawText(1,ex-width,ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
				}

				if(fabs(sy-ey)>3*rowh)//标注A-
				{
					float ax,ay;
					ax = 0.5*(sx+ex);
					ay = 0.5*(sy+ey);
					ACHAR str[30];
					_stprintf(str, L"A—%0.2lf",pm->XYArray[i][1]);
					DrawText(3,ax,ay-rowh,width,0.5*rowh,10,myShapes,str);//注里程				
				}

			}
			else
			{
				ACHAR scp[10];
				_tcscpy(scp, L"C9");
				Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
				float rowh = _variant_t(rgMyRge.GetRowHeight());
				float left,width;//绘图域左距,宽度
				left=_variant_t(rgMyRge.GetLeft());
				width=_variant_t(rgMyRge.GetWidth());

				if((feml1-fsml1)>0.001)
					sx = left + 0.5*width - (fsml-fsml1)*0.5*width/(feml1-fsml1) ;
				else
					sx = left + 0.5*width;	

				if((feml1-fsml1)>0.001)
					ex = left + (feml1-feml)*0.5*width/(feml1-fsml1) ;
				else
					ex = left ;

				if(i>is)
					myShapes.AddLine(bx,by,sx,sy);//画横线*/

				myShapes.AddLine(sx,sy,ex,ey);//画横线*/

				if(fabs(feml-feml1)<0.1)//标注
				{
					myShapes.AddLine(ex-0.5*width,ey,ex,ey);//画横线

					DrawText(1,ex-0.5*width,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,chr);//注里程
					DrawText(1,left-width,0.5*rowh+ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
				}

				if(fabs(sy-ey)>3*rowh)//标注A-
				{
					float ax,ay;
					ax = 0.5*(sx+ex);
					ay = 0.5*(sy+ey);
					ACHAR str[30];
					_stprintf(str, L"A—%0.2lf",pm->XYArray[i][1]);
					DrawText(3,ax-0.5*width,ay-rowh,width,0.5*rowh,10,myShapes,str);//注里程				
				}
			}			
		}
		else if(XZ==2.0)//圆
		{
			if(ZX<0.0)//左转
			{
				ACHAR scp[10];
				_tcscpy(scp, L"B9");
				Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
				float rowh = _variant_t(rgMyRge.GetRowHeight());
				float left,width;//绘图域左距,宽度
				left=_variant_t(rgMyRge.GetLeft());
				width=_variant_t(rgMyRge.GetWidth());

				sx = ex = left + 0.5*width;
				if(i>is)
					myShapes.AddLine(bx,by,sx,sy);//画横线*/

				myShapes.AddLine(sx,sy,ex,ey);//画横线*/

				if(fabs(feml-feml1)<0.1)//标注
				{
					myShapes.AddLine(ex,ey,ex+0.5*width,ey);//画横线

					DrawText(1,ex,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,chr);//注里程
					DrawText(1,left,ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
				}

				if(fabs(sy-ey)>3*rowh)//标注A-
				{
					float ax,ay;
					ax = 0.5*(sx+ex);
					ay = 0.5*(sy+ey);
					ACHAR str[30];
					_stprintf(str, L"R—%0.2lf",pm->XYArray[i][1]);
					DrawText(3,ax,ay-rowh,width,0.5*rowh,10,myShapes,str);//注里程				
				}

			}
			else
			{
				ACHAR scp[10];
				_tcscpy(scp, L"C9");
				Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
				float rowh = _variant_t(rgMyRge.GetRowHeight());
				float left,width;//绘图域左距,宽度
				left=_variant_t(rgMyRge.GetLeft());
				width=_variant_t(rgMyRge.GetWidth());

				sx = ex = left + 0.5*width;
				if(i>is)
					myShapes.AddLine(bx,by,sx,sy);//画横线*/

				myShapes.AddLine(sx,sy,ex,ey);//画横线*/

				if(fabs(feml-feml1)<0.1)//标注
				{
					myShapes.AddLine(ex-0.5*width,ey,ex,ey);//画横线
					//	ads_printf(L"ex=%lf ey=%lf\n",ex,ey);
					DrawText(1,ex-0.5*width,ey-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,chr);//注里程
					//	ads_printf(L"bmstr=%s\n",bmstr);
					DrawText(1,left-width,ey,2*width,0.5*rowh,10,myShapes,bmstr);//注里程				
				}

				if(fabs(sy-ey)>3*rowh)//标注A-
				{
					float ax,ay;
					ax = 0.5*(sx+ex);
					ay = 0.5*(sy+ey);
					ACHAR str[30];
					_stprintf(str, L"R—%0.2lf",pm->XYArray[i][1]);
					DrawText(3,ax-0.5*width,ay-rowh,width,0.5*rowh,10,myShapes,str);//注里程				
				}

			}
		}
	}
}


int OutTAB::FindWichPd(double cml,GTZDM_ROAD *zm)
{
	int i;
	for( i=0;i<zm->NCM;i++)
	{
		if(cml < zm->CM[i*5]+0.001)
			break;		
	}
	if(i==zm->NCM)
		i--;
	return i;
}
void OutTAB::DrawZm(JD_CENTER *pm,GTZDM_ROAD *zm,ACHAR CKSML[256],ACHAR CKEML[256],ACHAR ckml[50][80],int nRow,_Worksheet wsMysheet)
{
	double sml,eml,fsml,fcml,feml,XZ,BXZ,EXZ,ZX,bx,by,T,my,bpd,ebd;//本页的起始和终止里程
	float sy,ey,sx,ex;
	ACHAR fbmstr[30],ebmstr[30],cbmstr[30],hcstr[30],str[200],bpdstr[30],epdstr[30];
	bool IfHasNotePd;

	IfHasNotePd = false;
	sml = pm->TYLC(CKSML); 
	eml = pm->TYLC(CKEML); 
	if(eml-sml<0)
		return;//不画
	int is,ie;
	is = FindWichPd(sml,zm);
	ie = FindWichPd(eml,zm);
	if(is>0)
		is--;
	//从is到ie绘平面分段

	ACHAR scp[10];
	_tcscpy(scp, L"D9");
	Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
	float rowh = _variant_t(rgMyRge.GetRowHeight());

	Shapes myShapes;
	myShapes=wsMysheet.GetShapes();

	for(int i=is; i<=ie; i++)
	{
		//获取变坡点中心里程、起点里程和终点里程

		fcml = zm->CM[i*5];//变坡点中心里程
		T =  zm->CalT(i);
		fsml = fcml - T;
		feml = fcml + T;

		fsml = fsml < sml ? sml : fsml;
		feml = feml > eml ? eml : feml;

		if(fabs(T)<5)
			continue;

		//计算起终行
		int startRow = TranCmlToRow(pm,fsml,ckml,nRow)+9;
		int mRow = TranCmlToRow(pm,fcml,ckml,nRow)+9;
		int row,endRow;
		row = TranCmlToRow(pm,feml,ckml,nRow);
		if(row==nRow)
			row++;
		endRow = row + 9;

		if(endRow<startRow)
			continue;
		sy = RowsToFloat(startRow,wsMysheet);
		ey = RowsToFloat(endRow,wsMysheet);
		my = RowsToFloat(mRow,wsMysheet);

		if(startRow>9&&startRow<nRow+9)
			sy+=0.5*rowh;
		if(endRow>9&&endRow<nRow+9)
			ey+=0.5*rowh;			
		if(mRow>9&&mRow<nRow+9)
			my+=0.5*rowh;			

		int idai;
		double dml,bm,R,E;
		R = zm->CM[i*5+4];
		E = zm->rdes_levelWithoutVerCurve(fcml) - zm->rdes_level (fcml);
		cmltobmstr(pm,fsml,fbmstr);
		cmltobmstr(pm,feml,ebmstr);
		cmltobmstr(pm,fcml,cbmstr);
		_stprintf(hcstr, L"%0.3lf",zm->CM[i*5+1]);
		_stprintf(str, L"R-%0.0lf   T-%0.0lf   E-%0.2lf",R,fabs(T),fabs(E));

		if(zm->CM[i*5+2]==0.0)
			_tcscpy(bpdstr, L"0.00%");
		else if(zm->CM[i*5+2]>0.0000001)
			_stprintf(bpdstr, L"+%0.2lf%%",zm->CM[i*5+2]*100);
		else
			_stprintf(bpdstr, L"-%0.2lf%%",fabs(zm->CM[i*5+2]*100));

		if(zm->CM[(i+1)*5+2]==0.0)
			_tcscpy(epdstr, L"0.00%");
		else if(zm->CM[(i+1)*5+2]>0.0000001)
			_stprintf(epdstr, L"+%0.2lf%%",zm->CM[(i+1)*5+2]*100);
		else
			_stprintf(epdstr, L"-%0.2lf%%",fabs(zm->CM[(i+1)*5+2]*100));


		if(E>0.0)//凸形
		{
			ACHAR scp[10];
			_tcscpy(scp, L"E9");
			Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
			float left,width;//绘图域左距,宽度
			left=_variant_t(rgMyRge.GetLeft());
			width=_variant_t(rgMyRge.GetWidth());
			//绘竖线
			sx = ex = left + 0.75*width;
			myShapes.AddLine(sx,sy,ex,ey);

			//标注起始里程
			if(fsml>sml && fsml<eml)
			{
				IfHasNotePd = true;
				DrawText(1,left,sy-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,fbmstr);//注里程
				//绘横线
				myShapes.AddLine(sx-0.5*width,sy,ex,sy);

				//注坡度
				if(sy>13*rowh)
					DrawText(3,left,sy-5*rowh,0.5*width,0.5*rowh,10,myShapes,bpdstr);//注里程				
			}
			if(feml>sml && feml<eml)
			{
				IfHasNotePd = true;
				DrawText(1,left,ey,0.5*width,0.5*rowh,10,myShapes,ebmstr);//注里程
				//绘横线
				myShapes.AddLine(sx-0.5*width,ey,ex,ey);
				//注坡度
				if(ey<(nRow+4)*rowh)
					DrawText(3,left,ey+3*rowh,0.5*width,0.5*rowh,10,myShapes,epdstr);//注里程

			}

			if(fcml>sml && fcml<eml)
			{
				IfHasNotePd = true;
				DrawText(1,left-0.25*width,my-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,cbmstr);//注里程
				DrawText(1,left-width,my,1.5*width,0.5*rowh,10,myShapes,hcstr);//注里程
				//绘横线
				myShapes.AddLine(sx-0.6*width,my,ex,my);
			}
			//标注R,T,E
			if(ey-my>my-sy && ey-my>5*rowh)//标在下面
			{
				IfHasNotePd = true;
				DrawText(3,left,my+0.2*(ey-my),0.5*width,0.8*(ey-my),10,myShapes,str);//注里程
			}
			else if(my-sy>5*rowh)//标在up面
			{
				IfHasNotePd = true;
				DrawText(3,left,sy+0.2*(ey-my),0.5*width,0.8*(my-sy),10,myShapes,str);//注里程
			}

		}
		else 
		{
			ACHAR scp[10];
			_tcscpy(scp, L"D9");
			Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
			float left,width;//绘图域左距,宽度
			left=_variant_t(rgMyRge.GetLeft());
			width=_variant_t(rgMyRge.GetWidth());
			//绘竖线
			sx = ex = left + 0.25*width;
			myShapes.AddLine(sx,sy,ex,ey);
			//标注起始里程
			if(fsml>sml && fsml<eml)
			{
				IfHasNotePd = true;
				DrawText(1,left,sy-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,fbmstr);//注里程
				//绘横线
				myShapes.AddLine(sx+0.5*width,sy,ex,sy);
				if(sy>13*rowh)
					DrawText(3,left,sy-5*rowh,0.5*width,0.5*rowh,10,myShapes,bpdstr);//注里程

			}
			if(feml>sml && feml<eml)
			{
				IfHasNotePd = true;
				DrawText(1,left,ey,0.5*width,0.5*rowh,10,myShapes,ebmstr);//注里程
				//绘横线
				myShapes.AddLine(sx+0.5*width,ey,ex,ey);
				if(ey<(nRow+4)*rowh)
					DrawText(3,left,ey+3*rowh,0.5*width,0.5*rowh,10,myShapes,epdstr);//注里程				
			}

			if(fcml>sml && fcml<eml)
			{
				IfHasNotePd = true;
				DrawText(1,left+0.25*width,my-0.6*rowh,0.5*width,0.5*rowh,10,myShapes,cbmstr);//注里程
				DrawText(1,left+0.25*width,my,1.5*width,0.5*rowh,10,myShapes,hcstr);
				//绘横线
				myShapes.AddLine(sx+0.6*width,my,ex,my);
			}			

			if(ey-my>my-sy && ey-my>5*rowh)//标在下面
			{
				IfHasNotePd = true;
				DrawText(3,left+0.25*width,my+0.2*(ey-my),0.5*width,0.8*(ey-my),10,myShapes,str);//注里程
			}
			else if(my-sy>5*rowh)//标在up面
			{
				IfHasNotePd = true;
				DrawText(3,left+0.25*width,sy+0.2*(ey-my),0.5*width,0.8*(my-sy),10,myShapes,str);//注里程
			}

		}
	}

	if(!IfHasNotePd)//本页尚未标住任何东西，标注坡度
	{
		fcml = 0.5*(sml+eml);
		is = FindWichPd(fcml,zm);

		if(zm->CM[is*5+2]==0.0)
			_tcscpy(epdstr, L"0.00%");
		else if(zm->CM[is*5+2]>0.0000001)
			_stprintf(epdstr, L"+%0.2lf%%",zm->CM[is*5+2]*100);
		else
			_stprintf(epdstr, L"-%0.2lf%%",fabs(zm->CM[is*5+2]*100));

		int mRow = TranCmlToRow(pm,fcml,ckml,nRow)+9;
		my = RowsToFloat(mRow,wsMysheet);
		ACHAR scp[10];
		_tcscpy(scp, L"E9");
		Range rgMyRge=wsMysheet.GetRange(COleVariant(scp));
		float left,width;//绘图域左距,宽度
		left=_variant_t(rgMyRge.GetLeft());
		width=_variant_t(rgMyRge.GetWidth());

		DrawText(3,left,my,0.5*width,0.5*rowh,10,myShapes,epdstr);//注坡度lbw		
	}

}

//---------------------------------------------------
int OutTAB::TranCmlToRow(JD_CENTER *pm,double fsml,ACHAR ckml[50][80],int nRow)
{
	double cml;
	int i;
	for( i=0; i<=nRow; i++)
	{
		cml = pm->TYLC(ckml[i]);
		if(cml>=fsml-0.1)
			break;
	}

	return i;
}
float OutTAB::RowsToFloat(int Rows,_Worksheet wsMysheet)
{
	ACHAR scp[10],fcp[10];
	_stprintf(scp, L"%s%d", L"A",Rows);//有待修改利用模板
	_stprintf(fcp, L"%s%d", L"A",Rows);
	Range rgMyRge;
	LPDISPATCH pd=NULL;
	VERIFY(pd = wsMysheet.GetRange(COleVariant(scp),COleVariant(fcp)));
	rgMyRge.AttachDispatch(pd);
	return _variant_t(rgMyRge.GetTop());
	//	return rgMyRge.GetTop();
}
void OutTAB::DrawText(long orient,float left,float top,float width,
					  float height,long fontsize,Shapes &mShapes,ACHAR *str)
{
	Shape mShape;
	FillFormat mFillFormat;
	TextFrame mTextFrame;
	LineFormat mLineFormat;
	Characters Characters;
	FontXls mFont;

	mShape=mShapes.AddTextbox(1,left,top,width,height);//
	mFillFormat=mShape.GetFill();
	mFillFormat.SetTransparency(1);

	mTextFrame=mShape.GetTextFrame();
	mLineFormat=mShape.GetLine();
	mLineFormat.SetVisible(0);
	mTextFrame.SetOrientation(orient);
	mTextFrame.SetAutoSize(true);
	//mTextFrame.SetAutoMargins(true);
	//Characters=mTextFrame.Characters(COleVariant((short)1),COleVariant((short)50));
	//Characters.SetText(str);

	//Characters=mTextFrame.Characters(COleVariant((long)1),COleVariant((long)50));
	//Characters.SetText(str);//lbw

	//mFont=Characters.GetFont();
	//mFont.SetSize(COleVariant(fontsize));

}
void OutTAB::cmltobmstr(JD_CENTER *pm,double cml, ACHAR bmstr[])
{
	int idai;
	double dml,bm;
	CString GH;

	dml = pm->XLC(cml,GH);
	bm = dml - (int)((dml+0.001)/1000.0)*1000.0;
	if(bm>99.999)
		_stprintf(bmstr, L"+%0.0lf",bm);
	else if(bm>9.999)
		_stprintf(bmstr, L"+0%0.0lf",bm);
	else if(bm>0.999)
		_stprintf(bmstr, L"+00%0.0lf",bm);
	else if(bm>0.09)
		_stprintf(bmstr, L"+000%0.0lf",bm);	
	else
		_tcscpy(bmstr, L"+000");	
}

void OutTAB::OUT_TSFJSB() 
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\路基土石方计算表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	if(Cal_TSFJSB(dlg.m_PrjMdb,dlg.RoadName,dlg.m_Sdml,dlg.m_Edml))
	{
		FillLjtsfjs(dlg.m_RoadStr,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
	}



}
///////////////////////////////////////////////////
bool  OutTAB::Cal_TSFJSB(CString PrjMdb,CString RoadName,double Sdml,double Edml)//2001.5.29 
{
	// TODO: Add your control notification handler code here
	FILE *fpr;
	struct oridata
	{
		int DorS,ZDMXZ,YDMXZ,Y_ZDMXZ,Y_YDMXZ;
		double cml,FillSoilArea,FillRockArea;
		double CutArea[6];
	} *Crs=NULL;
	CString fname;
	int i,j,m,NCRS,M,DelSum,iWFFL,iLTTL;
	double tmp,AreaSum,BTinf,at,aw,Lnpt,Rnpt,FillArea,CutArea;
	//	char mes[80];
	double K[6],WF[6],WFSUM,SoilORRockPer[6],FillVolSum;
	CString str;
	int ncrs,stanum;
	int TorS;
	JD_CENTER *pm;
	ACHAR mes[30];

	XLDataBase mdb;		
	// 创建每一道路平面对象
	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);		
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}	

	double Scml,Ecml,cml;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);

	TSF_Ncrs = 0;
	HdmDataBase hdmmdb;		
	hdmmdb.Read_HdmDbs(PrjMdb,RoadName, L"松方压实系数");
	for(i=0; i<6; i++)
		K[i]=hdmmdb.YSXS[i];

	CString name;
	name=PrjMdb;
	name.MakeUpper();
	ReadWorkdir();

	CString Dir;
	Dir=Cworkdir;
	Dir.MakeUpper();
	Cworkdir=Dir;
	name.Replace(Cworkdir,NULL);
	name.Replace(L"\\DATA\\", NULL);
	name.Replace(L".MDB", NULL);

	fname=Cworkdir + "\\data\\" + "HdmDetailArea." + name + "." + RoadName;
	if((fpr=_wfopen(fname , L"r"))==NULL)
	{
		ads_alert(L"无法打开hdmarea文件!");
		return false;
	}

	fwscanf(fpr, L"%d",&ncrs);
	//开空间
	TSF_Ncrs = 0;
	if(ncrs>0)
	{
		Crs = new oridata [ncrs];
		CrsData = new TSFoutput [ncrs];

		for(int j=0; j<ncrs; j++)
		{

			double dtmp;
			int itmp;

			fwscanf(fpr, L"%lf%d%d%d%d%d",&Crs[j].cml,&Crs[j].DorS,&Crs[j].ZDMXZ,&Crs[j].YDMXZ,&Crs[j].Y_ZDMXZ,&Crs[j].Y_YDMXZ);
			fwscanf(fpr, L"%d%lf%lf%lf%lf",&TorS,&FillArea,&dtmp,&dtmp,&dtmp);
			//拟修改
			//_stprintf(mes, L"%0.2lf",FillArea);
			//FillArea = _wtof(mes);

			Crs[j].FillSoilArea =  Crs[j].FillRockArea = 0.0;
			if(TorS==0)
				Crs[j].FillSoilArea = FillArea;
			else
				Crs[j].FillRockArea = FillArea;


			fwscanf(fpr, L"%lf%lf%lf%lf",&aw,&dtmp,&dtmp,&dtmp);
			//拟修改
			//_stprintf(mes, L"%0.2lf",aw);
			//aw = _wtof(mes);
			int k;
			for(k=0; k<6; k++)
				fwscanf(fpr, L"%lf",&SoilORRockPer[k]);
			fwscanf(fpr, L"%lf%lf%lf%lf",&dtmp,&dtmp,&dtmp,&dtmp);
			fwscanf(fpr, L"%lf%lf%lf%lf",&dtmp,&dtmp,&dtmp,&dtmp);
			fwscanf(fpr, L"%lf%lf%lf%lf",&dtmp,&dtmp,&dtmp,&dtmp);
			fwscanf(fpr, L"%lf%lf%lf%lf",&dtmp,&dtmp,&dtmp,&dtmp);
			fwscanf(fpr, L"%lf%lf%lf",&dtmp,&dtmp,&dtmp);
			fwscanf(fpr, L"%lf%lf%lf",&dtmp,&dtmp,&dtmp);


			if(Crs[j].cml+0.1<Scml) 
				continue;
			if(Crs[j].cml-0.1>Ecml) break;


			for(k=0; k<6; k++)
			{
				Crs[j].CutArea[k] = aw * SoilORRockPer[k] *0.01;
				//拟修改
				//_stprintf(mes, L"%0.2lf",Crs[j].CutArea[k] );
				//Crs[j].CutArea[k]  = _wtof(mes);
			}

			for(k=5; k>=0; k--)
			{
				if(Crs[j].CutArea[k]>0.001)
				{
					double asum=0;
					for(int n=k-1; n>=0; n--)//计算剩余的总和
						asum += Crs[j].CutArea[n];
					//
					Crs[j].CutArea[k] = aw - asum;                         
					break;
				}		  
			}


			////////////////////////////////////////////////////////////////
			//	   if(Crs[j].DorS==1)
			{
				if((Crs[j].ZDMXZ==3||Crs[j].ZDMXZ==4) && (Crs[j].YDMXZ==3||Crs[j].YDMXZ==4))
					continue;
				if(Crs[j].DorS>0)//为增加断面不计算土方
					continue;
			}
			//	   else
			//	   {
			//		   if((Crs[j].ZDMXZ==3||Crs[j].ZDMXZ==4) && (Crs[j].YDMXZ==3||Crs[j].YDMXZ==4)&&
			//			   (Crs[j].Y_ZDMXZ==3||Crs[j].Y_ZDMXZ==4) && (Crs[j].Y_YDMXZ==3||Crs[j].Y_YDMXZ==4))
			//			   continue;
			//	   }

			_tcscpy(CrsData[TSF_Ncrs].ckml,pm->XLC(Crs[j].cml));
			CrsData[TSF_Ncrs].cml = Crs[j].cml;
			AreaSum = aw;
			CrsData[TSF_Ncrs].CutAreaSum = AreaSum;
			CrsData[TSF_Ncrs].FillSoilArea= Crs[j].FillSoilArea;
			CrsData[TSF_Ncrs].FillRockArea= Crs[j].FillRockArea;
			CrsData[TSF_Ncrs].IfBriTunEnd = false;
			// if(Crs[j].DorS==1)
			//{
			if((Crs[j].ZDMXZ==1||Crs[j].ZDMXZ==2) && (Crs[j].YDMXZ==1||Crs[j].YDMXZ==2 || Crs[j].YDMXZ==3 || Crs[j].YDMXZ==4)||
				(Crs[j].YDMXZ==1||Crs[j].YDMXZ==2) && (Crs[j].ZDMXZ==1||Crs[j].ZDMXZ==2 || Crs[j].ZDMXZ==3 || Crs[j].ZDMXZ==4))
				CrsData[TSF_Ncrs].IfBriTunEnd = true;
			// }
			//lse
			//
			// if((Crs[j].ZDMXZ==1||Crs[j].ZDMXZ==2) && (Crs[j].YDMXZ==1||Crs[j].YDMXZ==2)&&
			//  (Crs[j].Y_ZDMXZ==1||Crs[j].Y_ZDMXZ==2) && (Crs[j].Y_YDMXZ==1||Crs[j].Y_YDMXZ==2))
			//   CrsData[TSF_Ncrs].IfBriTunEnd = true;
			//

			CrsData[TSF_Ncrs].IfBriTunStart = false;
			//   if(Crs[j].DorS==1)
			// {
			if((Crs[j].ZDMXZ==-1||Crs[j].ZDMXZ==-2) && (Crs[j].YDMXZ==-1||Crs[j].YDMXZ==-2||Crs[j].YDMXZ==3||Crs[j].YDMXZ==4) ||
				(Crs[j].YDMXZ==-1||Crs[j].YDMXZ==-2) && (Crs[j].ZDMXZ==-1||Crs[j].ZDMXZ==-2||Crs[j].ZDMXZ==3||Crs[j].ZDMXZ==4) )
				CrsData[TSF_Ncrs].IfBriTunStart = true;
			// }
			//lse
			//
			// if((Crs[j].ZDMXZ==-1||Crs[j].ZDMXZ==-2) || (Crs[j].YDMXZ==-1||Crs[j].YDMXZ==-2)&&
			//  (Crs[j].Y_ZDMXZ==-1||Crs[j].Y_ZDMXZ==-2) && (Crs[j].Y_YDMXZ==-1||Crs[j].Y_YDMXZ==-2))
			//   CrsData[TSF_Ncrs].IfBriTunStart = true;
			//


			if(j==0||TSF_Ncrs==0||CrsData[TSF_Ncrs].IfBriTunEnd)//桥尾或隧道尾
			{

				CrsData[TSF_Ncrs].dis = 0.0;
				CrsData[TSF_Ncrs].CutVolSum = 0.0;
				CrsData[TSF_Ncrs].FillVolSum= 0.0;
				CrsData[TSF_Ncrs].TQ = 0.0;
				CrsData[TSF_Ncrs].BzwyfSoilSum = 0.0;
				CrsData[TSF_Ncrs].BzwyfRockSum = 0.0;
				for(k=0; k<6; k++)
				{
					CrsData[TSF_Ncrs].CutVol[k] = 0.0;
					CrsData[TSF_Ncrs].CutPer[k] = 0.0;
					CrsData[TSF_Ncrs].BZLY[k] = 0.0;
					CrsData[TSF_Ncrs].BZWYF[k] = 0.0;
				}

			}
			else
			{
				CrsData[TSF_Ncrs].dis = CrsData[TSF_Ncrs].cml  - CrsData[TSF_Ncrs-1].cml ;
				//		     CrsData[TSF_Ncrs].CutVolSum = 0.5*(CrsData[TSF_Ncrs].CutAreaSum+CrsData[j-1].CutAreaSum)*CrsData[TSF_Ncrs].dis;
				//		     CrsData[TSF_Ncrs].FillVolSum = 0.5*(CrsData[TSF_Ncrs].FillSoilArea+CrsData[TSF_Ncrs].FillRockArea+CrsData[j-1].FillSoilArea+CrsData[j-1].FillRockArea)*CrsData[TSF_Ncrs].dis;
				CrsData[TSF_Ncrs].CutVolSum = 0.5*(CrsData[TSF_Ncrs].CutAreaSum+CrsData[TSF_Ncrs-1].CutAreaSum)*CrsData[TSF_Ncrs].dis;
				CrsData[TSF_Ncrs].FillVolSum = 0.5*(CrsData[TSF_Ncrs].FillSoilArea+CrsData[TSF_Ncrs].FillRockArea+CrsData[TSF_Ncrs-1].FillSoilArea+CrsData[TSF_Ncrs-1].FillRockArea)*CrsData[TSF_Ncrs].dis;
				//			 ads_printf(L"%lf %lf %lf %lf\n",Crs[j].cml,Crs[j-1].cml,CrsData[TSF_Ncrs].CutAreaSum,CrsData[j-1].CutAreaSum);
				for(k=0; k<6; k++)
				{
					//2002.7.30  CrsData[TSF_Ncrs].CutVol[j] = 0.5*(Crs[j].CutArea[j]+Crs[j-1].CutArea[j])*CrsData[TSF_Ncrs].dis;
					CrsData[TSF_Ncrs].CutVol[k] = CrsData[TSF_Ncrs].CutVolSum * SoilORRockPer[k]/100.0;
					if(CrsData[TSF_Ncrs].CutVolSum>0.0)//有挖方
					{
						if( CrsData[TSF_Ncrs].CutVol[k]>0.0)
							CrsData[TSF_Ncrs].CutPer[k] = SoilORRockPer[k];
						else
							CrsData[TSF_Ncrs].CutPer[k] = 0.0;

					}
					else
						CrsData[TSF_Ncrs].CutPer[k] = 0.0;
				}

				//--------------------------计算本桩利用-----------------------
				if(CrsData[TSF_Ncrs].FillVolSum>0.0)//有填方
				{
					for(k=0; k<6; k++)
						WF[k]= CrsData[TSF_Ncrs].CutVol[k] / K[k];//类石可利用数
					WFSUM = WF[0]+WF[1]+WF[2]+WF[3]+WF[4]+WF[5];

					if(CrsData[TSF_Ncrs].FillVolSum>WFSUM)//所有挖方均被利用,且有填缺
					{ 
						for(k=0; k<6; k++)
						{
							CrsData[TSF_Ncrs].BZLY[k] = CrsData[TSF_Ncrs].CutVol[k];
							CrsData[TSF_Ncrs].BZWYF[k] = 0.0;
						}
						CrsData[TSF_Ncrs].TQ =  CrsData[TSF_Ncrs].FillVolSum-WFSUM;
						CrsData[TSF_Ncrs].BzwyfSoilSum = 0.0;
						CrsData[TSF_Ncrs].BzwyfRockSum = 0.0;
					}
					else////挖方中只有填方部分被利用,且有挖余
					{
						FillVolSum = CrsData[TSF_Ncrs].FillVolSum;
						CrsData[TSF_Ncrs].BzwyfSoilSum = 0.0; 
						for(k=0; k<6; k++)
						{
							CrsData[TSF_Ncrs].BZLY[k] = 0.0;
							if(CrsData[TSF_Ncrs].CutVol[k]>0.0 && FillVolSum>0.01)
							{
								if(CrsData[TSF_Ncrs].CutVol[k]>FillVolSum*K[k])
									CrsData[TSF_Ncrs].BZLY[k] = FillVolSum*K[k];
								else
									CrsData[TSF_Ncrs].BZLY[k] = CrsData[TSF_Ncrs].CutVol[k];//全部利用

								FillVolSum = FillVolSum - CrsData[TSF_Ncrs].BZLY[k]/K[k];//第j类土全部利用后剩余							 	 
							}
							CrsData[TSF_Ncrs].BZWYF[k] = CrsData[TSF_Ncrs].CutVol[k] - CrsData[TSF_Ncrs].BZLY[k];

						}
						CrsData[TSF_Ncrs].BzwyfSoilSum  += CrsData[TSF_Ncrs].BZWYF[0]+CrsData[TSF_Ncrs].BZWYF[1]+CrsData[TSF_Ncrs].BZWYF[2]; 
						CrsData[TSF_Ncrs].BzwyfRockSum  += CrsData[TSF_Ncrs].BZWYF[3]+CrsData[TSF_Ncrs].BZWYF[4]+CrsData[TSF_Ncrs].BZWYF[5]; 
						CrsData[TSF_Ncrs].TQ = 0.0;                   
					}
				}
				else//无填方
				{

					CrsData[TSF_Ncrs].BzwyfSoilSum  = 0.0; 
					CrsData[TSF_Ncrs].BzwyfRockSum  = 0.0; 
					for(k=0; k<6; k++)
					{
						CrsData[TSF_Ncrs].BZLY[k] = 0.0;
						CrsData[TSF_Ncrs].BZWYF[k] = CrsData[TSF_Ncrs].CutVol[k];
						if(k<3)
							CrsData[TSF_Ncrs].BzwyfSoilSum += CrsData[TSF_Ncrs].BZWYF[k];
						else
							CrsData[TSF_Ncrs].BzwyfRockSum += CrsData[TSF_Ncrs].BZWYF[k];
					}
					CrsData[TSF_Ncrs].TQ = 0.0;			  
				}
				//--------------------------计算本桩利用-----------------------
			}
			TSF_Ncrs++;     
		}
	}
	if(Crs) delete []Crs;	   
	fclose(fpr);
	return true;
	//	for(j=0; j<TSF_Ncrs; j++)
	//		ads_printf(L"%d %d %s %lf %lf %lf\n",j,TSF_Ncrs,CrsData[j].ckml,CrsData[j].CutAreaSum,CrsData[j].FillSoilArea,CrsData[j].FillRockArea);
}
//*************************************** 
//   FillLjtsfjs   填写路基土石方计算表
//*************************************** 
void OutTAB::FillLjtsfjs(CString RoadStr,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int flag;
	int i, j, row, totalnum;
	long shnum;
	// 4 路基土石方计算表
	struct lujitsfjs
	{
		ACHAR zhnum[30];       //   桩号
		double hdm[3];        //   横断面  
		double wf[30];        //   本桩挖余方数量
	} tsf;
	ACHAR filepath[180], shname[30], pageinfo[80];
	double num[30]={0.0},pkm[30]={0};
	double BTinf;
	ACHAR prezh[30];
	int flag1;


	RoadStr = RoadStr + " " + DuanName;
	// 填写工程名称及段号
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));
	flag1 = 0; shnum = 1;
	totalnum=TSF_Ncrs;

	row = 1;

	if(totalnum >= 1 )
	{
		//第一个特殊区别对待
		if(row==1 || totalnum > 35)   // 若有两张表,需要复制模板
		{
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"土石方计算%d",shnum);
			m_worksheet.SetName(shname);
			_stprintf(pageinfo, L" 第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"AF4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			shnum++;
		}

		_tcscpy(prezh,CrsData[0].ckml);
		pRange=m_worksheet.GetRange(COleVariant(L"A10"));
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(CrsData[0].ckml));
		for(j=0; j<3; j++)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			if(j==0 && CrsData[0].CutAreaSum>0.0)
				m_range.SetValue(COleVariant(CrsData[0].CutAreaSum));
			if(j==1 && CrsData[0].FillSoilArea>0.0)
				m_range.SetValue(COleVariant(CrsData[0].FillSoilArea));
			if(j==2 && CrsData[0].FillRockArea>0.0)
				m_range.SetValue(COleVariant(CrsData[0].FillRockArea));
		}	   
	}

	for(i=1; i<totalnum; i++)
	{  
		flag=0;
		_tcscpy(tsf.zhnum,CrsData[i].ckml);
		tsf.hdm[0] = CrsData[i].CutAreaSum;
		tsf.hdm[1] = CrsData[i].FillSoilArea;
		tsf.hdm[2] = CrsData[i].FillRockArea;
		tsf.wf[0] = CrsData[i].dis;
		tsf.wf[1] = CrsData[i].CutVolSum;
		for(j=0; j<6; j++)
		{
			tsf.wf[2+j*2] = CrsData[i].CutPer[j];
			tsf.wf[2+j*2+1] = CrsData[i].CutVol[j];
		}
		tsf.wf[14] = CrsData[i].FillVolSum;
		for(j=0; j<6; j++)
			tsf.wf[15+j] = CrsData[i].BZLY[j];
		tsf.wf[21] = CrsData[i].TQ;
		for(j=0; j<3; j++)
			tsf.wf[22+j] = CrsData[i].BZWYF[j];
		tsf.wf[25] = CrsData[i].BzwyfSoilSum;
		for(j=3; j<6; j++)
			tsf.wf[26+j-3] =CrsData[i].BZWYF[j];
		tsf.wf[29] = CrsData[i].BzwyfRockSum;

		/*
		if(_tcscmp(CrsData[i].ckml, L"AK122+175")==0)
		{
		CString mes;
		mes.Format(L"%d %s %s\n",CrsData[i].IfBriTunEnd,prezh,CrsData[i].ckml);
		ads_alert(mes);
		}*/

		if(CrsData[i].IfBriTunEnd&&ispage(prezh,CrsData[i].ckml))
			goto a500;

		pRange=m_worksheet.GetCell(2*row+9,5);
		m_range.AttachDispatch(pRange);
		for(j=0; j<30; j++)
		{

			if(tsf.wf[j]<=1e-6)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				continue;
			}
			if(j!=1&&j!=25&&j!=29&&j!=2&&j!=4&&j!=6
				&&j!=8&&j!=10&&j!=12)
				num[j]=num[j]+tsf.wf[j];
			m_range.SetValue(COleVariant(tsf.wf[j]));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}

a300:
		pRange = m_worksheet.GetCell(row*2+10,1);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(tsf.zhnum));
		for(j=0; j<3; j++)
		{
			if(tsf.hdm[j]<=1e-6)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				continue;
			}
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(tsf.hdm[j]));
		}
		row++;

		if(flag==0&&(row-1)%34 ==0 &&i+1<totalnum||
			flag==0&&glzhuang(tsf.zhnum)&&i+1<totalnum)		  
		{
a500:
			for(j=0; j<30; j++) pkm[j]=pkm[j]+num[j];
			if(glzhuang(tsf.zhnum)||ispage(prezh,tsf.zhnum))
			{
				pkm[1]=pkm[3]+pkm[5]+pkm[7]+pkm[9]+pkm[11]+pkm[13];
				pkm[25]=pkm[22]+pkm[23]+pkm[24];
				pkm[29]=pkm[26]+pkm[27]+pkm[28];

				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A80")));
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(L"本公里小计"));

				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"E80")));
				m_range.AttachDispatch(pRange);
				for(j=0; j<30; j++)
				{

					if(pkm[j]<=1e-6)
					{
						pRange = m_range.GetNext();
						m_range.AttachDispatch(pRange);
						continue;
					}
					m_range.SetValue(COleVariant(pkm[j]));
					pkm[j]=0.0;
					num[j]=0.0;
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
				}
			}
			else
			{
				num[1]=num[3]+num[5]+num[7]+num[9]+num[11]+num[13];
				num[25]=num[22]+num[23]+num[24];
				num[29]=num[26]+num[27]+num[28];

				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A80")));
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(L"本页小计"));

				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"E80")));
				m_range.AttachDispatch(pRange);
				for(j=0; j<30; j++)
				{

					if(num[j]<=1e-6)
					{
						pRange = m_range.GetNext();
						m_range.AttachDispatch(pRange);
						continue;
					}
					m_range.SetValue(COleVariant(num[j]));
					num[j]=0.0;
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
				}
			}
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"土石方计算%d",shnum);
			m_worksheet.SetName(shname);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(RoadStr));
			_stprintf(pageinfo, L" 第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"AF4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			shnum++;
			row = 0;
			flag=1;
			goto a300;
		}
		if(i+1==totalnum)
		{

			for(j=0; j<30; j++) pkm[j]=pkm[j]+num[j];
			pkm[1]=pkm[3]+pkm[5]+pkm[7]+pkm[9]+pkm[11]+pkm[13];
			pkm[25]=pkm[22]+pkm[23]+pkm[24];
			pkm[29]=pkm[26]+pkm[27]+pkm[28];

			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A80")));
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(L"本公里小计"));

			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"E80")));
			m_range.AttachDispatch(pRange);
			for(j=0; j<30; j++)
			{

				if(pkm[j]<=1e-6)
				{
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
					continue;
				}
				m_range.SetValue(COleVariant(pkm[j]));
				pkm[j]=0.0;
				num[j]=0.0;
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
			}
		}
		_tcscpy(prezh,tsf.zhnum);
	}
	if(totalnum>35)
	{
		pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
		m_worksheet.AttachDispatch(pWorksheet);
		m_worksheet.Select();
		m_worksheet.Delete();
	}
	else
	{
		_stprintf(pageinfo, L" 第  %d  页     共  %d  页",Spage, PageSum);
		VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"AF4")));	  
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pageinfo));
	}
	if(CrsData) delete []CrsData;	   
}

//////////////////////////////////////////////////////////////////////////
//计算土石方总量
void OutTAB::CalAllTSF(CString PrjMdb,CString RoadName,double Sdml,double Edml, double& FillVolSum, double& CutVolSum) //生成每公里土石方数量计算表
{
	// TODO: Add your control notification handler code here
	FillVolSum = 0.0;
	CutVolSum = 0.0;
	int KMnum = 0;
	KMTSFdata KMdata[100];
	ACHAR ck[50];
	int i,j;
	double sml;
	int skml,ekml,inf;
	double sml1;
	ACHAR KSml[80],KEml[80];
	double KmCutVol[6],DisSum;
	double KmFillVolSum,KmCutVolSum;

	Cal_TSFJSB(PrjMdb,RoadName,Sdml,Edml);//2001.5.29 


	for(j=0; j<6; j++)
		KmCutVol[j] = 0.0;

	KMnum = 0;
	for(i=0; i<TSF_Ncrs; i++)
	{
		if((i)==0 || CrsData[i].IfBriTunEnd)//桥尾、隧尾
		{
			_tcscpy(KSml , CrsData[i].ckml);
			DisSum = 0;
			KmFillVolSum = 0.0;
			KmCutVolSum = 0.0;
			for(j=0; j<6; j++)
				KmCutVol[j] = 0.0;
			_tcscpy(KEml , CrsData[i].ckml);
			continue;
		}

		KmFillVolSum += CrsData[i].FillVolSum;
		KmCutVolSum += CrsData[i].CutVolSum;
		DisSum += CrsData[i].dis;
		for(j=0; j<6; j++)
			KmCutVol[j] += CrsData[i].CutVol[j];

		split_ckml(CrsData[i].ckml, &sml, ck);

		//若无公里桩
		inf = 0;
		if(i>0)
		{
			split_ckml(KEml, &sml1, ck);
			skml = (int)(sml1/1000.0+0.0001);
			ekml = (int)(sml/1000.0+0.0001);
			if(fmod(sml+0.0001 , 1000.0) > 0.1 && ekml - skml > 0)//跨公里且无公里桩
			{
				inf = 1;			 
			}
		}

		_tcscpy(KEml , CrsData[i].ckml);
		if(i>0)
		{
			if(fmod(sml+0.0001 , 1000.0) < 0.1 || i==TSF_Ncrs-1 || CrsData[i].IfBriTunStart||inf==1)//整公里桩或最后一个桩或桥隧头
			{

				KMdata[KMnum].DisSum = DisSum;
				_tcscpy(KMdata[KMnum].KSml , KSml);
				_tcscpy(KMdata[KMnum].KEml , KEml);
				ACHAR mes[30];
				_stprintf(mes, L"%0.2lf",KmCutVolSum);
				KMdata[KMnum].KmCutVolSum = _wtof(mes);
				for(j=0; j<6; j++)
				{
					_stprintf(mes, L"%0.2lf",KmCutVol[j]);
					KMdata[KMnum].KmCutVol[j] = _wtof(mes);
				}
				_stprintf(mes, L"%0.2lf",KmFillVolSum);
				KMdata[KMnum].KmFillVolSum = _wtof(mes);

				_tcscpy(KSml , KEml);
				KmCutVolSum = 0.0;
				KmFillVolSum = 0.0;
				for(j=0; j<6; j++)
					KmCutVol[j] = 0.0;
				DisSum = 0;
				KMnum++;
			}
		}
	}

	for (i=0; i<KMnum; i++)
	{
		FillVolSum += KMdata[i].KmFillVolSum;
		CutVolSum += KMdata[i].KmCutVolSum;
	}

}

//-------------------------------------------------------------------------
void OutTAB::Cal_MGLTSFB(CString PrjMdb,CString RoadName,double Sdml,double Edml,int &KMnum,KMTSFdata KMdata[500]) //生成每公里土石方数量计算表
{
	// TODO: Add your control notification handler code here
	ACHAR ck[50];
	int i,j;
	double sml;
	int skml,ekml,inf;
	double sml1;
	ACHAR KSml[80],KEml[80];
	double KmCutVol[6],DisSum;
	double KmFillVolSum,KmCutVolSum;

	Cal_TSFJSB(PrjMdb,RoadName,Sdml,Edml);//2001.5.29 


	for(j=0; j<6; j++)
		KmCutVol[j] = 0.0;

	KMnum = 0;
	for(i=0; i<TSF_Ncrs; i++)
	{
		if((i)==0 || CrsData[i].IfBriTunEnd)//桥尾、隧尾
		{
			_tcscpy(KSml , CrsData[i].ckml);
			DisSum = 0;
			KmFillVolSum = 0.0;
			KmCutVolSum = 0.0;
			for(j=0; j<6; j++)
				KmCutVol[j] = 0.0;
			_tcscpy(KEml , CrsData[i].ckml);
			continue;
		}

		KmFillVolSum += CrsData[i].FillVolSum;
		KmCutVolSum += CrsData[i].CutVolSum;
		DisSum += CrsData[i].dis;
		for(j=0; j<6; j++)
			KmCutVol[j] += CrsData[i].CutVol[j];

		split_ckml(CrsData[i].ckml, &sml, ck);

		//若无公里桩
		inf = 0;
		if(i>0)
		{
			split_ckml(KEml, &sml1, ck);
			skml = (int)(sml1/1000.0+0.0001);
			ekml = (int)(sml/1000.0+0.0001);
			if(fmod(sml+0.0001 , 1000.0) > 0.1 && ekml - skml > 0)//跨公里且无公里桩
			{
				inf = 1;			 
			}
		}

		_tcscpy(KEml , CrsData[i].ckml);
		if(i>0)
		{
			if(fmod(sml+0.0001 , 1000.0) < 0.1 || i==TSF_Ncrs-1 || CrsData[i].IfBriTunStart||inf==1)//整公里桩或最后一个桩或桥隧头
			{

				KMdata[KMnum].DisSum = DisSum;
				_tcscpy(KMdata[KMnum].KSml , KSml);
				_tcscpy(KMdata[KMnum].KEml , KEml);
				ACHAR mes[30];
				_stprintf(mes, L"%0.2lf",KmCutVolSum);
				KMdata[KMnum].KmCutVolSum = _wtof(mes);
				for(j=0; j<6; j++)
				{
					_stprintf(mes, L"%0.2lf",KmCutVol[j]);
					KMdata[KMnum].KmCutVol[j] = _wtof(mes);
				}
				_stprintf(mes, L"%0.2lf",KmFillVolSum);
				KMdata[KMnum].KmFillVolSum = _wtof(mes);

				_tcscpy(KSml , KEml);
				KmCutVolSum = 0.0;
				KmFillVolSum = 0.0;
				for(j=0; j<6; j++)
					KmCutVol[j] = 0.0;
				DisSum = 0;
				KMnum++;
			}
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////
void OutTAB::OUT_MGLTSFB()
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\路基每公里土石方表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	KMTSFdata KMdata[500];
	int KMnum;
	Cal_MGLTSFB(dlg.m_PrjMdb,dlg.RoadName,dlg.m_Sdml,dlg.m_Edml,KMnum,KMdata);
	Fillmgltsfsl(dlg.m_RoadStr,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum,KMnum,KMdata);


}
//*************************************** 
//   Fillmgltsfsl   填写路基每公里土石方表
//*************************************** 
void OutTAB::Fillmgltsfsl(CString RoadStr,CString DuanName,int Spage,int PageSum,int KMnum,KMTSFdata KMdata[500])
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	double num[9]={0.0};
	int i,j, totalnum, row,m=0;
	long shnum;
	struct mgltsfsl
	{
		ACHAR kml1[30],kml2[30];       //   桩号
		double cm[9]; 
	} mgl;
	ACHAR shname[30], filepath[280], pageinfo[80],kml[200];
	// 读总桩数
	totalnum = KMnum;


	RoadStr = RoadStr + " " + DuanName;
	// 填写工程名称及段号
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));
	shnum = 1;

	for(i=0;i<totalnum;i++)
	{   
		if(i%30==0)
		{
			// 复制模板工作表然后填写
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"每公里土石方(1)%d",shnum);
			m_worksheet.SetName(shname);
			_stprintf(pageinfo, L"第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"Y4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			shnum++;
			row=1;
		}
		_tcscpy(mgl.kml1,KMdata[i].KSml);
		_tcscpy(mgl.kml2,KMdata[i].KEml);
		mgl.cm[0] = KMdata[i].DisSum;
		mgl.cm[1] = KMdata[i].KmCutVolSum;
		for(j=0;j<6;j++)
			mgl.cm[2+j] = KMdata[i].KmCutVol[j];
		mgl.cm[8] = KMdata[i].KmFillVolSum;

		_stprintf(kml, L"%s～%s",mgl.kml1,mgl.kml2);

		pRange = m_worksheet.GetCell(row+8,1);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(short(i+1-m)));
		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(kml));
		for(j=0; j<9; j++)
		{
			if(mgl.cm[j]<=1e-6)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				continue;
			}
			num[j]=num[j]+mgl.cm[j];
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(mgl.cm[j]));
		}

		row++;

	}
	_stprintf(kml, L"本段小计");

	pRange = m_worksheet.GetCell(39,2);
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(kml));
	for(j=0; j<9; j++)
	{
		if(num[j]<=1e-6)
		{
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			continue;
		}
		pRange = m_range.GetNext();
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(num[j]));
	}
	//删除模板工作表
	pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	m_worksheet.Delete();

}

//-------------------------------------------------------------------------------------
void OutTAB::Cal_LXTW(CString PrjMdb,CString RoadName,double Sdml,double Edml) //生成路线填挖数据表
{
	FILE *fp,*fpr,*fpw;
	ACHAR fname[256],str[256];
	double  temp;
	int i,form_sum,k,itmp,j,n1,n2;
	int Ljdnum,Rjdnum;
	double Hc,Lwd,Rwd;
	double ZJH,ZPJH,RJH,RPJH,ZH;
	double deltax,deltay,aa;
	double dml,cml;
	double EarH,ZEarH,REarH;
	int NumL,NumR;
	double Lpt[400],Rpt[400];
	int NumLByLJZX,NumRByLJZX;
	double LptByLJZX[400],RptByLJZX[400];
	double EarHByLJZX;
	double dtmp,xjj,tmp;
	int DorS,IfHasZlink,IfHasYlink;
	ACHAR tmpstr[30];
	int ZDMXZ,YDMXZ,Y_ZDMXZ,Y_YDMXZ;
	double RHFL[10],RHFR[10],Y_RHFL[10],Y_RHFR[10];
	double Zt,Yt,Y_Zt,Y_Yt;
	int Lnpt,Rnpt,Y_Lnpt,Y_Rnpt;
	double RPTL[50][2],RPTR[50][2],Y_RPTL[50][2],Y_RPTR[50][2];
	int GouPtNum,JSGouPtNum;
	bool IfBriTunEnd;
	int IsInsertHdm;
	double OffsetByXLZX = 0.0;//暂时

	XLDataBase mdb;		
	JD_CENTER *pm;
	// 创建每一道路平面对象
	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);		
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}
	double Scml,Ecml;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);

	ReadWorkdir();
	CString DMXfname;
	CString HDMdesfname;
	CString name,Dir;
	name=PrjMdb;
	name.MakeUpper();
	ReadWorkdir();

	Dir=Cworkdir;
	Dir.MakeUpper();
	Cworkdir=Dir;
	name.Replace(Cworkdir,NULL);
	name.Replace(L"\\DATA\\", NULL);
	name.Replace(L".MDB", NULL);
	DMXfname =Cworkdir + "\\data\\" + "hdmdmx." + name + "." + RoadName;
	HDMdesfname =Cworkdir + "\\data\\" + "hdmresult." + name + "." + RoadName;
	// 读总桩数

	if((fp=_wfopen(DMXfname, L"r"))==NULL)
	{
		AfxMessageBox(L"无法打开地面线文件1!");
		exit(0);
	}

	fwscanf(fp, L"%d",&itmp);

	if((fpr=_wfopen(HDMdesfname, L"r"))==NULL)
	{
		AfxMessageBox(L"无法打开横断面结果文件1!");
		exit(0);
	}

	fwscanf(fpr, L"%d" , &form_sum);
	form_sum = fabs(1.0*form_sum);
	if(RoadName!="主线")
		fwscanf(fpr, L"%lf%lf%lf%lf%lf%lf",&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp);

	if(form_sum>0 )
	{
		if(LXTW) delete[]LXTW;
		LXTW =  new LXTWdata[form_sum]; 		
	}
	else
	{
		ads_alert(L"桩号总数为0!\n");
		return;
	}

	NLXTW = 0;

	for(k=0; k<form_sum; k++)
	{
		//读地面线
		fwscanf(fp, L"%lf%lf%d%d",&dml,&EarH,&NumL,&NumR);
		int n;
		for( n=1; n<=NumL; n++)
		{
			fwscanf(fp, L"%lf%lf",&Lpt[n*2],&Lpt[n*2+1]);
			//			  Lpt[n*2+1] = Lpt[n*2+1] + EarH;
		}
		Lpt[0] = Lpt[2];
		Lpt[1] = Lpt[3];
		for(n=1; n<=NumR; n++)
		{
			fwscanf(fp, L"%lf%lf",&Rpt[n*2],&Rpt[n*2+1]);
			//	          Rpt[n*2+1] = Rpt[n*2+1] + EarH;
		}
		Rpt[0] = Rpt[2];
		Rpt[1] = Rpt[3];
		CalNewZB(OffsetByXLZX,EarH,Lpt,NumL,Rpt,NumR,EarHByLJZX,LptByLJZX,NumLByLJZX,RptByLJZX,NumRByLJZX);
		for (n=0; n<=NumLByLJZX; n++)
		{
			LptByLJZX[n*2+1] = LptByLJZX[n*2+1] + EarHByLJZX;
		}
		for (n=0; n<=NumRByLJZX; n++)
		{
			RptByLJZX[n*2+1] = RptByLJZX[n*2+1] + EarHByLJZX;
		}
		//读设计信息		
		//  fwscanf(fpr , L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
		fwscanf(fpr , L"%lf%lf%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%s",
			&cml,&dml,&IsInsertHdm,&IfHasYlink,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
			&ZEarH,&OffsetByXLZX,&xjj,&dtmp,&dtmp,&dtmp,str);

		REarH = ZEarH;
		IfHasZlink=IfHasYlink=0;
		//1.写左线左侧
		fwscanf(fpr, L"%d",&ZDMXZ);
		for(int j=0;j<3;j++)
			fwscanf(fpr, L"%lf",&dtmp);
		//写路面点
		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf",&RHFL[2*j],&RHFL[2*j+1]);
		//左设计点
		fwscanf(fpr, L"%lf%lf%d",&Zt,&dtmp,&Lnpt);//换行
		for(j=1; j<=Lnpt; j++)
			fwscanf(fpr, L"%lf%lf",&RPTL[j][0],&RPTL[j][1]);

		//左水沟点
		fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&dtmp,&itmp,&GouPtNum,&JSGouPtNum);
		for(j=0; j<GouPtNum; j++)
			fwscanf(fpr, L"%lf%lf",&dtmp,&dtmp);
		//左截水沟
		for(j=0;j<JSGouPtNum;j++)
		{
			fwscanf(fpr, L"%lf%lf",&dtmp,&dtmp);
		}
		//2.写左线右侧
		//写路面点
		fwscanf(fpr, L"%d",&YDMXZ);
		for(j=0;j<3;j++)
			fwscanf(fpr, L"%lf",&dtmp);

		for(j=0;j<5;j++)
			fwscanf(fpr, L"%lf%lf",&RHFR[2*j],&RHFR[2*j+1]);
		//右设计点
		fwscanf(fpr, L"%lf%lf%d ",&Yt,&dtmp,&Rnpt);
		for(j=1; j<=Rnpt; j++)
			fwscanf(fpr, L"%lf%lf",&RPTR[j][0],&RPTR[j][1]);

		//右水沟点
		fwscanf(fpr, L"%d%lf%d%d%d",&itmp,&dtmp,&itmp,&GouPtNum,&JSGouPtNum);
		for(j=0; j<GouPtNum; j++)
			fwscanf(fpr, L"%lf%lf",&dtmp,&dtmp);
		//右截水沟
		for(j=0;j<JSGouPtNum;j++)
			fwscanf(fpr, L"%lf%lf",&dtmp,&dtmp);

		if(IfHasZlink )//左有关联道路
		{
			double dtmp;
			int itmp,jsgnum,num;

			fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				&dtmp,
				tmp			
				);
			//hdmdes[jcross].zlink->glRoadName = tmp;
			//1.写左线左侧
			fwscanf(fpr, L"%d",&itmp);
			//写路面点
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);
			//写路面点
			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
			for(j=0; j<itmp; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//左截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//2.写左线右侧
			//写路面点
			fwscanf(fpr, L"%d",&itmp);
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);

			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&num);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);

			//右水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}		
		}

		if(IfHasYlink)//右有关联道路
		{
			double dtmp;
			int itmp,jsgnum,num;

			fwscanf(fpr , L"%lf%lf%lf%lf%lf%lf%lf%s",
				&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,&dtmp,
				tmp			
				);
			//hdmdes[jcross].zlink->glRoadName = tmp;
			//1.写左线左侧
			fwscanf(fpr, L"%d",&itmp);
			//写路面点
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);
			//写路面点
			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&itmp);//换行
			for(j=0; j<itmp; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//左水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//左截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}
			//2.写左线右侧
			//写路面点
			fwscanf(fpr, L"%d",&itmp);
			for(j=0;j<3;j++)
				fwscanf(fpr, L"%lf ",&dtmp);

			for(j=0;j<5;j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右设计点
			fwscanf(fpr, L"%lf%lf%d ",&dtmp,&dtmp,&num);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);

			//右水沟点
			fwscanf(fpr, L"%d%lf%d%d%d ",&itmp,&dtmp,&itmp,&num,&jsgnum);
			for(j=0; j<num; j++)
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			//右截水沟
			for(j=0;j<jsgnum;j++)
			{
				fwscanf(fpr, L"%lf%lf ",&dtmp,&dtmp);
			}		
		}
		/////////////////////////////////////////////////////////////////////

		// 计算填挖数据
		if(cml>=Scml-0.001&&cml<=Ecml+0.001&&IsInsertHdm==0)
		{
			IfBriTunEnd = false;
			if((ZDMXZ==3&&YDMXZ==3) || (ZDMXZ==4&&YDMXZ==4))
				continue;
			if((ZDMXZ==1&&YDMXZ==1) || (ZDMXZ==2&&YDMXZ==2))
				IfBriTunEnd = true;
			Ljdnum = Lnpt;
			Rjdnum = Rnpt;
			ZJH = RHFL[9];
			RJH = RHFR[9];

			ZPJH = EarHByLJZX + RPTL[Ljdnum][1];
			LXTW[NLXTW].ZTWH = ZJH - ZPJH;
			RPJH = EarHByLJZX + RPTR[Rjdnum][1];
			LXTW[NLXTW].RTWH = RJH - RPJH;

			//计算边坡斜长
			LXTW[NLXTW].ZBPXC = 0.0;
			LXTW[NLXTW].RBPXC = 0.0;
			LXTW[NLXTW].Wlen = 0.0;
			//左侧
			if(Zt<0.0001)//路堤
			{
				if(Ljdnum>5)
				{
					for(j=5; j<Ljdnum; j++)
					{
						deltax = RPTL[j+1][0] - RPTL[j][0];
						deltay = RPTL[j+1][1] - RPTL[j][1];
						LXTW[NLXTW].ZBPXC += sqrt(deltax*deltax+deltay*deltay);
					}
				}		  
			}
			else//路堑
			{
				if(Ljdnum>9)
				{
					for(j=9; j<Ljdnum; j++)
					{
						deltax = RPTL[j+1][0] - RPTL[j][0];
						deltay = RPTL[j+1][1] - RPTL[j][1];
						LXTW[NLXTW].ZBPXC  += sqrt(deltax*deltax+deltay*deltay);
					}
					Lwd =fabs(RHFL[8]);
					LXTW[NLXTW].Wlen  += Cal_LmWfLen(NumLByLJZX,LptByLJZX,Lwd,4,RHFL);
					LXTW[NLXTW].ZBPXC = -fabs(LXTW[NLXTW].ZBPXC);
				}		     
			}

			if(Yt<0.0001)//路堤
			{
				if(Rjdnum>5)
				{
					for(j=5; j<Rjdnum; j++)
					{
						deltax = RPTR[j+1][0] - RPTR[j][0];
						deltay = RPTR[j+1][1] - RPTR[j][1];
						LXTW[NLXTW].RBPXC += sqrt(deltax*deltax+deltay*deltay);
					}
				}	  
			}
			else//路堑
			{
				if(Rjdnum>9)
				{
					for(j=5; j<Rjdnum; j=j++)
					{
						deltax = RPTR[j+1][0] - RPTR[j][0];
						deltay = RPTR[j+1][1] - RPTR[j][1];
						LXTW[NLXTW].RBPXC += sqrt(deltax*deltax+deltay*deltay);
					}
					Rwd = RHFR[8];
					LXTW[NLXTW].Wlen  += Cal_LmWfLen(NumRByLJZX,RptByLJZX,Rwd,4,RHFR);
					LXTW[NLXTW].RBPXC = -fabs(LXTW[NLXTW].RBPXC);
				}		     
			}	   
			Ljdnum = Lnpt;
			Rjdnum = Rnpt;
			ZJH = RHFL[9];
			RJH = RHFR[9];
			LXTW[NLXTW].cml = cml;
			_tcscpy(LXTW[NLXTW].ckml,pm->XLC(cml));
			ZPJH = EarHByLJZX + RPTL[Ljdnum][1];
			LXTW[NLXTW].ZTWH = ZJH - ZPJH;
			RPJH = EarHByLJZX + RPTR[Rjdnum][1];
			LXTW[NLXTW].RTWH = RJH - RPJH;

			//计算边坡斜长
			LXTW[NLXTW].ZBPXC = 0.0;
			LXTW[NLXTW].RBPXC = 0.0;
			LXTW[NLXTW].Wlen = 0.0;
			//左侧
			if(Zt<0.0001)//路堤
			{
				if(Ljdnum>5)
				{
					for(j=5; j<Ljdnum; j++)
					{
						deltax = RPTL[j+1][0] - RPTL[j][0];
						deltay = RPTL[j+1][1] - RPTL[j][1];
						LXTW[NLXTW].ZBPXC += sqrt(deltax*deltax+deltay*deltay);
					}
				}		  
			}
			else//路堑
			{
				if(Ljdnum>9)
				{
					for(j=9; j<Ljdnum; j++)
					{
						deltax = RPTL[j+1][0] - RPTL[j][0];
						deltay = RPTL[j+1][1] - RPTL[j][1];
						LXTW[NLXTW].ZBPXC  += sqrt(deltax*deltax+deltay*deltay);
					}
					Lwd =fabs(RHFL[8]);
					LXTW[NLXTW].Wlen  += Cal_LmWfLen(NumLByLJZX,LptByLJZX,Lwd,4,RHFL);
					LXTW[NLXTW].ZBPXC = -fabs(LXTW[NLXTW].ZBPXC);
				}		     
			}

			if(Yt<0.0001)//路堤
			{
				if(Rjdnum>5)
				{
					for(j=5; j<Rjdnum; j++)
					{
						deltax = RPTR[j+1][0] - RPTR[j][0];
						deltay = RPTR[j+1][1] - RPTR[j][1];
						LXTW[NLXTW].RBPXC += sqrt(deltax*deltax+deltay*deltay);
					}
				}	  
			}
			else//路堑
			{
				if(Rjdnum>9)
				{
					for(j=5; j<Rjdnum; j=j++)
					{
						deltax = RPTR[j+1][0] - RPTR[j][0];
						deltay = RPTR[j+1][1] - RPTR[j][1];
						LXTW[NLXTW].RBPXC += sqrt(deltax*deltax+deltay*deltay);
					}
					//	   HdmData[k].RBPXC +=  fabs(RPTR[4][0]) - Rwd;
					Rwd = RHFR[8];
					LXTW[NLXTW].Wlen  += Cal_LmWfLen(NumRByLJZX,RptByLJZX,Rwd,4,RHFR);
					// HdmData[k].Wlen += HdmData[k].RBPXC;
					LXTW[NLXTW].RBPXC = -fabs(LXTW[NLXTW].RBPXC);
				}		     
			}

			if(NLXTW==0 || IfBriTunEnd)
			{
				LXTW[NLXTW].Wjj = 0;
				LXTW[NLXTW].Warea = 0;
			}
			else
			{
				LXTW[NLXTW].Wjj =  LXTW[NLXTW].cml -  LXTW[NLXTW-1].cml;
				LXTW[NLXTW].Warea = 0.5*(LXTW[NLXTW].Wlen+LXTW[NLXTW-1].Wlen)*LXTW[NLXTW].Wjj;
			}		
			NLXTW++;
		}
		//----------------------------------计算路线填挖表所需数据-----------

		//     fwprintf(fpw, L"%s %lf %lf %lf %lf %lf %lf %lf\n",
		//		  HdmData[k].ckml,HdmData[k].ZTWH,HdmData[k].RTWH,HdmData[k].ZBPXC,HdmData[k].RBPXC,HdmData[k].Wlen,HdmData[k].Wjj,HdmData[k].Warea);
		//	   if(HdmData[k].Lpt) free(HdmData[k].Lpt);
		//	   if(HdmData[k].Rpt) free(HdmData[k].Rpt);	   	    
	} /*end while*/
	//  AfxMessageBox(L"ok");
	//ads_printf(L"NW=%d\n",NLXTW);
	fclose(fp);
	fclose(fpr);
}
//--------------------------------------------------------------------------
double OutTAB::Cal_LmWfLen(int NumPt,double EarPt[],double wd,int DesNpt,double DESPT[10])
{
	double Wflen,x1,EarY1,DesY1,deltaY1,x2,EarY2,DesY2,DeltaY2,DeltaY1;
	int i,NearPt;
	//	char mes[280];

	i=0;
	while(EarPt[i*2] < wd && i<=NumPt)
		i++;

	Wflen = 0.0;
	NearPt = i;

	for(i=0; i<NearPt; i++)
	{
		x1 = EarPt[i*2];
		if(x1>wd)
			x1 = wd;

		DesY1 = Cal_DesY(x1,DesNpt,DESPT);
		EarY1 = Cal_EarY(x1,NumPt,EarPt);
		DeltaY1= DesY1 - EarY1;

		x2 = EarPt[(i+1)*2];
		if(x2>wd)
			x2 = wd;
		DesY2 = Cal_DesY(x2,DesNpt,DESPT);
		EarY2 = Cal_EarY(x2,NumPt,EarPt);
		DeltaY2= DesY2 - EarY2;

		//_stprintf(mes, L"x1=%lf Desy1=%lf Eary1=%lf x2=%lf desy2=%lf eary2=%lf",x1,DesY1,EarY1,x2,DesY2,EarY2);
		//	AfxMessageBox(mes);

		if(DeltaY1<=0 && DeltaY2<=0)//x1--x2段为挖方
			Wflen += x2-x1;
		else if(DeltaY1<=0 && DeltaY2>=0)//x1挖,x2填
			Wflen += fabs(DeltaY1)*(x2-x1)/(fabs(DeltaY1)+DeltaY2);
		else if(DeltaY1>=0 && DeltaY2<=0)//x1填,x2挖
			Wflen += fabs(DeltaY2)*(x2-x1)/(DeltaY1+fabs(DeltaY2));
	}
	return Wflen;
}

double OutTAB::Cal_DesY(double x,int DesNpt,double DESPT[])
{
	double DesY;
	int i;

	DesY = DESPT[(DesNpt-1)*2+1];
	for(i=0; i<DesNpt; i++)
	{
		if(fabs(DESPT[i*2])>x)
		{
			if(i>0)
			{
				DesY = DESPT[(i-1)*2+1] + (DESPT[i*2+1]-DESPT[(i-1)*2+1])*(x-fabs(DESPT[(i-1)*2]))/(fabs(DESPT[i*2])-fabs(DESPT[(i-1)*2]));
				break;
			}
		}
	}

	return DesY;
}

double OutTAB::Cal_EarY(double x,int EarNpt,double EARPT[])
{
	double EarY;
	int i;

	EarY = EARPT[1];
	for(i=0; i<=EarNpt; i++)
	{
		if(fabs(EARPT[i*2])>x)
		{
			if(i>0)
			{
				EarY = EARPT[(i-1)*2+1] + (EARPT[i*2+1]-EARPT[(i-1)*2+1])*(x-EARPT[(i-1)*2])/(EARPT[i*2]-EARPT[(i-1)*2]);
				break;
			}
		}
	}

	return EarY;
}
//*************************************** 
//   FillLxtw   填写路线填挖数据表
//*************************************** 
void OutTAB::FillLxtw(CString RoadStr,CString DuanName,int Spage,int PageSum)
{
	LPDISPATCH pRange=NULL;
	Range m_range;
	int flag;
	int i, j, row, totalnum;
	long shnum;
	struct lxtwjs
	{
		ACHAR zhnum[30];      //  桩号
		double tw[7];
	} lxtw;
	ACHAR filepath[180], shname[30], pageinfo[80];
	double num[11]={0.0}, pkm[11]={0.0};
	ACHAR prezh[30];


	RoadStr = RoadStr + " " + DuanName;
	totalnum = NLXTW;
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	m_range.SetValue(COleVariant(RoadStr));

	shnum = 1;
	row = 1;

	if(totalnum >= 1 )
	{
		//第一个特殊区别对待
		if(totalnum > 33)   // 若有两张表,需要复制模板
		{
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"路线填挖%d",shnum);
			m_worksheet.SetName(shname);
			_stprintf(pageinfo, L" 第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"I4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			shnum++;
		}
		_tcscpy(lxtw.zhnum,LXTW[0].ckml);
		lxtw.tw[0] = LXTW[0].ZTWH;
		lxtw.tw[1] = LXTW[0].RTWH;
		lxtw.tw[2] = LXTW[0].ZBPXC;
		lxtw.tw[3] = LXTW[0].RBPXC;
		lxtw.tw[4] = LXTW[0].Wlen;
		lxtw.tw[5] = LXTW[0].Wjj;
		lxtw.tw[6] = LXTW[0].Warea;

		_tcscpy(prezh,lxtw.zhnum);
		pRange=m_worksheet.GetRange(COleVariant(L"A9"));
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(lxtw.zhnum));
		for(j=0; j<4; j++)
		{
			if(lxtw.tw[j] > 0.0)
			{
				num[2*j] = num[2*j] + lxtw.tw[j]; //填方和
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(lxtw.tw[j]));
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
			}
			else if(lxtw.tw[j] < 0.0)
			{
				num[2*j+1] = num[2*j+1] - lxtw.tw[j]; //挖方和
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(-lxtw.tw[j]));
			}
			else
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);

			}
		}
		if(lxtw.tw[4] > 0.0)
		{
			num[8] = num[8] + lxtw.tw[4];  //长度和
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(lxtw.tw[4]));
		}
	}

	for(i=1; i<totalnum; i++)
	{  
		flag=0;
		_tcscpy(lxtw.zhnum,LXTW[i].ckml);
		lxtw.tw[0] = LXTW[i].ZTWH;
		lxtw.tw[1] = LXTW[i].RTWH;
		lxtw.tw[2] = LXTW[i].ZBPXC;
		lxtw.tw[3] = LXTW[i].RBPXC;
		lxtw.tw[4] = LXTW[i].Wlen;
		lxtw.tw[5] = LXTW[i].Wjj;
		lxtw.tw[6] = LXTW[i].Warea;

		//	   _tcscpy(prezh,lxtw.zhnum);  szf 2006.1.19
		//	   if(ispage(prezh,lxtw.zhnum))
		//	        goto a1500;

		pRange=m_worksheet.GetCell(2*row+8,11);
		m_range.AttachDispatch(pRange);
		for(j=5; j<7; j++)
		{
			if(lxtw.tw[j]<=1e-6)
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				continue;
			}
			//		  if(j!=1&&j!=25&&j!=29&&j!=2&&j!=4&&j!=6
			//			&&j!=8&&j!=10&&j!=12)
			num[4+j]=num[4+j]+lxtw.tw[j];
			m_range.SetValue(COleVariant(lxtw.tw[j]));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
		}

a1300:
		pRange = m_worksheet.GetCell(row*2+9,1);
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(lxtw.zhnum));
		for(j=0; j<4; j++)
		{
			if(lxtw.tw[j] > 0.0)
			{
				num[2*j] = num[2*j] + lxtw.tw[j]; //填方和
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(lxtw.tw[j]));
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
			}
			else if(lxtw.tw[j] < 0.0)
			{
				num[2*j+1] = num[2*j+1] - lxtw.tw[j]; //挖方和
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(-lxtw.tw[j]));
			}
			else
			{
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);

			}
		}
		if(lxtw.tw[4] > 0.0)
		{
			num[8] = num[8] + lxtw.tw[4];
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(lxtw.tw[4]));
		}

		if(glzhuang(lxtw.zhnum)&&_tcscmp(prezh,lxtw.zhnum)==0)//szf 2006.1.19
		{
			_tcscpy(prezh,lxtw.zhnum);
			continue;
		}
		row++;
		if(flag==0&&(row-1)%32 ==0 &&i+1<totalnum||
			flag==0&&glzhuang(lxtw.zhnum)&&i+1<totalnum)
		{
a1500:
			for(j=0; j<11; j++) pkm[j]=pkm[j]+num[j];
			if(glzhuang(lxtw.zhnum)||ispage(prezh,lxtw.zhnum))
			{
				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A75")));
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(L"本公里小计"));	  
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				for(j=0; j<9; j++)
				{
					if(pkm[j]<=1e-6)
					{
						pRange = m_range.GetNext();
						m_range.AttachDispatch(pRange);
						continue;
					}
					m_range.SetValue(COleVariant(pkm[j]));
					pkm[j]=0.0;
					num[j]=0.0;
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
				}


				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"K74")));
				m_range.AttachDispatch(pRange);
				for(j=9; j<11; j++)
				{				  
					if(pkm[j]<=1e-6)
					{
						pRange = m_range.GetNext();
						m_range.AttachDispatch(pRange);
						continue;
					}
					m_range.SetValue(COleVariant(pkm[j]));
					pkm[j]=0.0;
					num[j]=0.0;
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
				}
			}
			else
			{

				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A75")));
				m_range.AttachDispatch(pRange);
				m_range.SetValue(COleVariant(L"本页小计"));

				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
				for(j=0; j<9; j++)
				{
					if(num[j]<=1e-6)
					{
						pRange = m_range.GetNext();
						m_range.AttachDispatch(pRange);
						continue;
					}
					m_range.SetValue(COleVariant(num[j]));
					num[j]=0.0;
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
				}

				VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"K74")));
				m_range.AttachDispatch(pRange);
				for(j=9; j<11; j++)
				{

					if(num[j]<=1e-6)
					{
						pRange = m_range.GetNext();
						m_range.AttachDispatch(pRange);
						continue;
					}
					m_range.SetValue(COleVariant(num[j]));
					num[j]=0.0;
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
				}
			}
			m_app.Run(COleVariant(L"MyMacro"),COleVariant(shnum));
			pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
			m_worksheet.AttachDispatch(pWorksheet);
			m_worksheet.Select();
			_stprintf(shname, L"线路填挖%d",shnum);
			m_worksheet.SetName(shname);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(RoadStr));
			_stprintf(pageinfo, L" 第  %d  页     共  %d  页",Spage+shnum-1, PageSum);
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"I4")));	  
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(pageinfo));
			shnum++;
			row = 0;
			flag=1;
			goto a1300;
		}
		if(i+1==totalnum)
		{
			for(j=0; j<11; j++) pkm[j]=pkm[j]+num[j];
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A75")));
			m_range.AttachDispatch(pRange);
			m_range.SetValue(COleVariant(L"本公里小计"));

			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			for(j=0; j<9; j++)
			{
				if(pkm[j]<=1e-6)
				{
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);
					continue;
				}
				m_range.SetValue(COleVariant(pkm[j]));
				pkm[j]=0.0;
				num[j]=0.0;
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
			}
			VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"K74")));
			m_range.AttachDispatch(pRange);
			for(j=9; j<11; j++)
			{			  
				if(pkm[j]<=1e-6)
				{
					pRange = m_range.GetNext();
					m_range.AttachDispatch(pRange);				  
					continue;
				}
				m_range.SetValue(COleVariant(pkm[j]));
				pkm[j]=0.0;
				num[j]=0.0;
				pRange = m_range.GetNext();
				m_range.AttachDispatch(pRange);
			}
		}
		_tcscpy(prezh,lxtw.zhnum);
	}   

	if(totalnum>33)
	{
		pWorksheet=m_worksheets.GetItem(COleVariant(short(1)));
		m_worksheet.AttachDispatch(pWorksheet);
		m_worksheet.Select();
		m_worksheet.Delete();
	}
	else
	{
		_stprintf(pageinfo, L" 第  %d  页     共  %d  页",Spage, PageSum);
		VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"I4")));	  
		m_range.AttachDispatch(pRange);
		m_range.SetValue(COleVariant(pageinfo));
	}
}

void OutTAB::OUT_LXTWB() 
{
	CTABDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;

	ExcelLoad();
	LPDISPATCH pWorkbooks;
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE("Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}
	VERIFY(pWorkbooks=m_app.GetWorkbooks());
	m_workbooks.AttachDispatch(pWorkbooks);
	LPDISPATCH pWorkbook = NULL;
	LPDISPATCH pWorksheets = NULL;
	//Open  Excel templete file;
	ACHAR filepath[280];
	_stprintf(filepath, L"%s\\lib\\excel\\路线填挖数据表.xlt",SysPath);
	VERIFY(pWorkbook=m_workbooks.Open(filepath));
	m_workbook.AttachDispatch(pWorkbook);
	pWorksheets=m_workbook.GetWorksheets();
	ASSERT(pWorksheets!=NULL);
	m_worksheets.AttachDispatch(pWorksheets);

	Cal_LXTW(dlg.m_PrjMdb,dlg.RoadName,dlg.m_Sdml,dlg.m_Edml); //生成路线填挖数据表
	FillLxtw(dlg.m_RoadStr,dlg.m_DuanName,dlg.m_Spage,dlg.m_PageSum);
}


void OutTAB::CalZX(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{

	LPDISPATCH pRange=NULL;
	Range m_range;
	int i, totalnum, row, col, flag1, flag2,m=0;
	long shnum;
	char shname[30], filepath[180], pageinfo[80];

	RoadStr = RoadStr + " " + DuanName;
	//读桩号序列表
	XLDataBase mdb;		
	JD_CENTER *pm;
	GTZDM_ROAD *zdm=NULL;
	double jd;
	// 创建每一道路平面对象

	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);			
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);

	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}

	double Scml,Ecml,cml,Zx,Zy,Yx,Yy;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);
	zdm = ReadZm(PrjMdb,RoadName, L"全幅",pm);
	//	mdb.Read_XLDbs(PrjMdb, L"桩号序列",RoadName);

	FILE *fpr;
	fpr=_wfopen(L"c:\\123\\主左.txt", L"r");
	double ZSta[100],ZJiao[100],YSta[100],YJiao[100];
	int NSTA;
	fwscanf(fpr, L"%d",&NSTA);
	for( i=0; i<NSTA; i++)
		fwscanf(fpr, L"%lf%lf",&ZSta[i],&ZJiao[i]);
	fclose(fpr);

	//	FILE *fpr;
	fpr=_wfopen(L"c:\\123\\主右.txt", L"r");
	fwscanf(fpr, L"%d",&NSTA);
	for( i=0; i<NSTA; i++)
		fwscanf(fpr, L"%lf%lf",&YSta[i],&YJiao[i]);
	fclose(fpr);

	FILE *Zfpw,*Yfpw;
	Zfpw = _wfopen(L"c:\\123\\主线左.txt", L"w");
	Yfpw = _wfopen(L"c:\\123\\主线右.txt", L"w");

	double ZX_Pt[6][3],YX_Pt[6][3];
	double pt[2],prjml,bh,z;
	double lmhp;
	xlpoint PZ;
	for(i=0;i<NSTA;i++)
	{   
		cml = pm->TYLC(ZSta[i]);  
		jd = ZJiao[i];
		CalPZXPT(pm,cml,270+jd,2.6,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		//	  ads_printf(L"z=%0.3lf %lf\n",z,prjml);	
		ZX_Pt[5][0] = pt[0];
		ZX_Pt[5][1] = pt[1];
		ZX_Pt[5][2] = z + 0.4*0.02;

		//	  if(cml<28634.37)
		//		  jd = 27.252;
		//	  else if(cml<28714.64)
		//		  jd = 28.748;
		//	  else if(cml<28794.64)
		//		  jd = 31.275;
		//	  else if(cml<28810.4)
		//		  jd = 33.026;

		CalPZXPT(pm,cml,270+jd,7.0,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[4][0] = pt[0];
		ZX_Pt[4][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[4][2] = bh - (7-2.6)*0.02;

		CalPZXPT(pm,cml,270+jd,12.25,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[3][0] = pt[0];
		ZX_Pt[3][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[3][2] = bh - (12.25-2.6)*0.02;

		CalPZXPT(pm,cml,270+jd,17.5,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[2][0] = pt[0];
		ZX_Pt[2][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[2][2] = bh - (17.5-2.6)*0.02;

		CalPZXPT(pm,cml,270+jd,21.9,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[1][0] = pt[0];
		ZX_Pt[1][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[1][2] = bh - (21.9-2.6)*0.02;

		fwprintf(Zfpw, L"%0.3lf ",ZSta[i]);
		int j;
		for( j=1; j<=5; j++)
			fwprintf(Zfpw, L"%0.3lf %0.3lf %0.3lf ",ZX_Pt[j][0],ZX_Pt[j][1],ZX_Pt[j][2] );

		fwprintf(Zfpw, L"\n");


		/////////////////////////////////////////////////////////
		cml = pm->TYLC(YSta[i]); 
		jd = YJiao[i];
		CalPZXPT(pm,cml,90+jd,2.6,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);

		//		  if(cml<28634.4)
		//			  jd = 27.252;
		//		  else if(cml<28714.2)
		//			  jd = 28.748;
		//		  else if(cml<28794.2)
		//			  jd = 31.275;
		//		  else if(cml<28810.4)
		//			  jd = 33.026;
		//		  
		lmhp =  Callmhp(PZ.lc);

		YX_Pt[5][0] = pt[0];
		YX_Pt[5][1] = pt[1];
		YX_Pt[5][2] = z - 0.4*lmhp;

		CalPZXPT(pm,cml,90+jd,7.0,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[4][0] = pt[0];
		YX_Pt[4][1] = pt[1];
		bh = z - 0.4*lmhp;
		YX_Pt[4][2] = bh + (7-2.6)*lmhp;

		CalPZXPT(pm,cml,90+jd,12.25,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[3][0] = pt[0];
		YX_Pt[3][1] = pt[1];
		bh = z - 0.4*lmhp;
		YX_Pt[3][2] = bh + (12.25-2.6)*lmhp;

		CalPZXPT(pm,cml,90+jd,17.5,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[2][0] = pt[0];
		YX_Pt[2][1] = pt[1];
		bh = z - 0.4*lmhp;
		YX_Pt[2][2] = bh + (17.5-2.6)*lmhp;

		CalPZXPT(pm,cml,90+jd,21.9,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[1][0] = pt[0];
		YX_Pt[1][1] = pt[1];
		bh = z - 0.4*lmhp;
		YX_Pt[1][2] = bh + (21.9-2.6)*lmhp;




		//	  Zy = y + 32.875*sin(PZ.a-0.5*pi);
		//      Yx = x + 32.875*cos(PZ.a+0.5*pi);
		//	  Yy = y + 32.875*sin(PZ.a+0.5*pi);
		//	  if(cml>28180&&cml<29050)

		fwprintf(Yfpw, L"%0.3lf ",YSta[i]);
		for( j=1; j<=5; j++)
			fwprintf(Yfpw, L"%0.3lf %0.3lf %0.3lf ",YX_Pt[j][0],YX_Pt[j][1],YX_Pt[j][2] );

		fwprintf(Yfpw, L"\n");

	}
	fclose(Zfpw);
	fclose(Yfpw);


	//删除模板工作表

}

void OutTAB::CalPZXPT(JD_CENTER *pm,double cml,double jd,double dis,double pt[2],double &prjml)
{
	//1计算直线
	double spt[2],ept[2],dis1,dis2,mdis,xpt[2],mpt[2];
	xlpoint PZ;
	double projml;


	PZ.lc = cml;
	pm->xlpoint_pz(&PZ);
	spt[0]=PZ.x,spt[1]=PZ.y;
	ept[0] = spt[0] + 50.0*cos(PZ.a + jd*pi/180.0);
	ept[1] = spt[1] + 50.0*sin(PZ.a + jd*pi/180.0);
	//2取两点计算距中线距离
	dis1=0.0;
	projml=pm->PROJ_ML(ept[0],ept[1]);
	PZ.lc = projml;
	pm->xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	dis2 = sqrt((xpt[0]-ept[0])*(xpt[0]-ept[0])+(xpt[1]-ept[1])*(xpt[1]-ept[1]));
	//3二分叠代计算出pt
	mpt[0] = 0.5*(spt[0]+ept[0]);
	mpt[1] = 0.5*(spt[1]+ept[1]);
	projml=pm->PROJ_ML(mpt[0],mpt[1]);
	PZ.lc = projml;
	pm->xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	mdis = sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1]));

	while(fabs(mdis-dis)>0.0001)
	{
		if(mdis>dis)
			ept[0]=mpt[0],ept[1]=mpt[1];
		else
			spt[0]=mpt[0],spt[1]=mpt[1];
		mpt[0] = 0.5*(spt[0]+ept[0]);
		mpt[1] = 0.5*(spt[1]+ept[1]);
		projml=pm->PROJ_ML(mpt[0],mpt[1]);
		PZ.lc = projml;
		pm->xlpoint_pz(&PZ);
		xpt[0]=PZ.x,xpt[1]=PZ.y;
		mdis = sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1]));			
	}
	pt[0]=mpt[0],pt[1]=mpt[1];
	prjml = projml;
}

double OutTAB::Callmhp(double ml)
{
	double lmhp;

	if(ml<28445.17)
		lmhp = -0.02;
	else if(ml<28665.17)
		lmhp = (-2 + (ml-28445.17)*4/(28665.17-28445.17))*1.0/100.0;
	else
		lmhp = 0.02;
	return lmhp;
}
//
//void OutTAB::CalFX(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
//{
//
//	LPDISPATCH pRange=NULL;
//    Range m_range;
//	int i, totalnum, row, col, flag1, flag2,m=0;
//	long shnum;
//	char shname[30], filepath[180], pageinfo[80];
//	
//	RoadStr = RoadStr + " " + DuanName;
////读桩号序列表
//	XLDataBase mdb;		
//	JD_CENTER *pm;
//	GTZDM *zdm=NULL;
//	double jd;
//	// 创建每一道路平面对象
//
//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);		
//	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);
//
//	pm = NULL;
//	if(mdb.NJdCen>0)
//	{
//		pm=(JD_CENTER *)new JD_CENTER;//主线对象
//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
//		if(mdb.NDL>0)
//		   pm->setDLB(mdb.DLArray,mdb.NDL);
//	}
//
//	double Scml,Ecml,cml,Zx,Zy,Yx,Yy;
//	Scml = pm->TYLC(Sdml);
//	Ecml = pm->TYLC(Edml);
//    zdm = ReadZm(PrjMdb,RoadName, L"全幅",pm);
////	mdb.Read_XLDbs(PrjMdb, L"桩号序列",RoadName);
//
//	FILE *fpr;
//	fpr=_wfopen(L"c:\\123\\辅左1.txt", L"r");
//	double ZSta[100],ZJiao[100],YSta[100],YJiao[100],cml1,cml2,pt1[2],pt2[2],pt[2];
//	int NSTA;
//	xlpoint PZ;
//	fwscanf(fpr, L"%d",&NSTA);
//	for( i=0; i<NSTA; i++)
//	{
//		fwscanf(fpr, L"%lf%lf",&cml1,&cml2);
//		PZ.lc = cml1;
//		pm->xlpoint_pz(&PZ);
//		pt1[0] = PZ.x + 38.5*cos(PZ.a-0.5*pi);
//		pt1[1] = PZ.y + 38.5*sin(PZ.a-0.5*pi);
//
//		PZ.lc = cml2;
//		pm->xlpoint_pz(&PZ);
//		pt2[0] = PZ.x + 27.5*cos(PZ.a-0.5*pi);
//		pt2[1] = PZ.y + 27.5*sin(PZ.a-0.5*pi);
//		double fwj,pt[2];
//		xyddaa(pt1[0],pt1[1],pt2[0],pt2[1],&fwj);
//		pm->CalJD_ZX_XL(pt1,fwj,pt,ZSta[i]);		 
//	}
//
//	for( i=0; i<NSTA; i++)
//		fwscanf(fpr, L"%lf",&ZJiao[i]);
//	fclose(fpr);
//
//
//
//	fpr=_wfopen(L"c:\\123\\辅右1.txt", L"r");
//	fwscanf(fpr, L"%d",&NSTA);
//	for( i=0; i<NSTA; i++)
//	{
//		fwscanf(fpr, L"%lf%lf",&cml1,&cml2);
//		PZ.lc = cml1;
//		pm->xlpoint_pz(&PZ);
//		pt1[0] = PZ.x + 38.5*cos(PZ.a+0.5*pi);
//		pt1[1] = PZ.y + 38.5*sin(PZ.a+0.5*pi);
//
//		PZ.lc = cml2;
//		pm->xlpoint_pz(&PZ);
//		pt2[0] = PZ.x + 27.5*cos(PZ.a+0.5*pi);
//		pt2[1] = PZ.y + 27.5*sin(PZ.a+0.5*pi);
//
//		double fwj,pt[2];
//		xyddaa(pt1[0],pt1[1],pt2[0],pt2[1],&fwj);
//		pm->CalJD_ZX_XL(pt1,fwj,pt,YSta[i]);	
//	}
//
//	for( i=0; i<NSTA; i++)
//		fwscanf(fpr, L"%lf",&YJiao[i]);
//	fclose(fpr);
//
//
//    FILE *Zfpw,*Yfpw;
//	Zfpw = _wfopen(L"c:\\123\\辅线左.txt", L"w");
//	Yfpw = _wfopen(L"c:\\123\\辅线右.txt", L"w");
//
//	double ZX_Pt[6][3],YX_Pt[6][3];
//	double prjml,bh,z;
//	double lmhp;
//
//	for(i=0;i<NSTA;i++)
//	{   
//		cml = pm->TYLC(ZSta[i]);  
////		CalPZXPT(pm,cml,270+27.252,27.1,pt,prjml);
//		jd = ZJiao[i];
//		CalPFXPT(pm,cml,-1,32.875,90+jd,(32.875-27.1),pt,prjml);
//		PZ.lc = prjml;
//		if(zdm)
//			z = zdm->rdes_level(prjml);
//      ZX_Pt[5][0] = pt[0];
//	  ZX_Pt[5][1] = pt[1];
//	  ZX_Pt[5][2] = z + 0.4*0.02;
//
////	  if(cml<28651.9)
////		  jd = 27.688;
////	  else if(cml<28733.6)
////		  jd = 29.3148;
////	  else if(cml<28815.6)
////		  jd = 31.925;
////	  else if(cml<28832.3)
////		  jd = 33.722;
//	  
//	  
//	  CalPFXPT(pm,cml,-1,32.875,90+jd,(32.875-30.0),pt,prjml);
//	  PZ.lc = prjml;
//	  if(zdm)
//		  z = zdm->rdes_level(prjml);
//	  ZX_Pt[4][0] = pt[0];
//	  ZX_Pt[4][1] = pt[1];
//	  bh = z + 0.4*0.02;
//	  ZX_Pt[4][2] = bh - (30-27.1)*0.02;
//	  
//	  CalPFXPT(pm,cml,-1,32.875,270+jd,0,pt,prjml);
//	  PZ.lc = prjml;
//	  if(zdm)
//		  z = zdm->rdes_level(prjml);
//	  ZX_Pt[3][0] = pt[0];
//	  ZX_Pt[3][1] = pt[1];
//	  bh = z + 0.4*0.02;
//	  ZX_Pt[3][2] = bh - (32.875-27.1)*0.02;
//	  
//      CalPFXPT(pm,cml,-1,32.875,270+jd,(35.75-32.875),pt,prjml);
//	  PZ.lc = prjml;
//	  if(zdm)
//		  z = zdm->rdes_level(prjml);
//	  ZX_Pt[2][0] = pt[0];
//	  ZX_Pt[2][1] = pt[1];
//	   bh = z + 0.4*0.02;
//	  ZX_Pt[2][2] = bh - (35.75-27.1)*0.02;
//	  
//      CalPFXPT(pm,cml,-1,32.875,270+jd,(38.65-32.875),pt,prjml);
//	  PZ.lc = prjml;
//	  if(zdm)
//		  z = zdm->rdes_level(prjml);
//	  ZX_Pt[1][0] = pt[0];
//	  ZX_Pt[1][1] = pt[1];
//	   bh = z + 0.4*0.02;
//	  ZX_Pt[1][2] = bh - (38.65-27.1)*0.02;
//
//	   fwprintf(Zfpw, L"%0.3lf ",ZSta[i]);
//		  for(int j=1; j<=5; j++)
//			  fwprintf(Zfpw, L"%0.3lf %0.3lf %0.3lf ",ZX_Pt[j][0],ZX_Pt[j][1],ZX_Pt[j][2] );
//		  
//		  fwprintf(Zfpw, L"\n");
//		  
//
///////////////////////////////////////////////////////////
//		  cml = pm->TYLC(YSta[i]); 
////		  CalPZXPT(pm,cml,90+27.252,27.1,pt,prjml);
//		  jd = YJiao[i];
//          CalPFXPT(pm,cml,1,32.875,270+jd,(32.875-27.1),pt,prjml);
//		  PZ.lc = prjml;
//		  if(zdm)
//			  z = zdm->rdes_level(prjml);
////		  if(cml<28617.5)
////		     jd = 26.857;
////	      else if(cml<28696)
////		     jd = 28.234;
////	      else if(cml<28774.2)
////		     jd = 30.653;
////	      else if(cml<28789.5)
////		     jd = 32.36;
//	  
//          YX_Pt[5][0] = pt[0];
//	      YX_Pt[5][1] = pt[1];
//	      YX_Pt[5][2] = z + 0.4*0.02;
//
////		  CalPZXPT(pm,cml,90+27.252,30,pt,prjml);
//          CalPFXPT(pm,cml,1,32.875,270+jd,(32.875-30),pt,prjml);
//		  PZ.lc = prjml;
//		  if(zdm)
//			  z = zdm->rdes_level(prjml);
//		  
//		  YX_Pt[4][0] = pt[0];
//		  YX_Pt[4][1] = pt[1];
//		  bh = z + 0.4*0.02;
//	      YX_Pt[4][2] = bh - (30-27.1)*0.02;
//	  
//         CalPFXPT(pm,cml,1,32.875,90+jd,0,pt,prjml);
//		  PZ.lc = prjml;
//		  if(zdm)
//			  z = zdm->rdes_level(prjml);
//		  lmhp =  Callmhp(PZ.lc);
//		  YX_Pt[3][0] = pt[0];
//		  YX_Pt[3][1] = pt[1];
//		   bh = z + 0.4*0.02;
//	      YX_Pt[3][2] = bh - (32.875-27.1)*0.02;
//
//          CalPFXPT(pm,cml,1,32.875,90+jd,(35.75-32.875),pt,prjml);
//		  PZ.lc = prjml;
//		  if(zdm)
//			  z = zdm->rdes_level(prjml);
//		  YX_Pt[2][0] = pt[0];
//		  YX_Pt[2][1] = pt[1];
//		  bh = z + 0.4*0.02;
//		  YX_Pt[2][2] = bh - (35.75-27.1)*0.02;
//
//	      CalPFXPT(pm,cml,1,32.875,90+jd,(38.65-32.875),pt,prjml);
//		  PZ.lc = prjml;
//		  if(zdm)
//			  z = zdm->rdes_level(prjml);
//		  YX_Pt[1][0] = pt[0];
//		  YX_Pt[1][1] = pt[1];
//		   bh = z + 0.4*0.02;
//		  YX_Pt[1][2] =  bh - (38.65-27.1)*0.02;
//		 	  
////	  Zy = y + 32.875*sin(PZ.a-0.5*pi);
////      Yx = x + 32.875*cos(PZ.a+0.5*pi);
////	  Yy = y + 32.875*sin(PZ.a+0.5*pi);
////	  if(cml>28180&&cml<29050)
//
//	  fwprintf(Yfpw, L"%0.3lf ",YSta[i]);
//	  for( j=1; j<=5; j++)
//		  fwprintf(Yfpw, L"%0.3lf %0.3lf %0.3lf ",YX_Pt[j][0],YX_Pt[j][1],YX_Pt[j][2]);
//	  
//	  fwprintf(Yfpw, L"\n");
//	  
//	}
// 	fclose(Zfpw);
//	fclose(Yfpw);
//
//	//删除模板工作表
//}

void OutTAB::CalPFXPT(JD_CENTER *pm,double cml,int ZorY,double distozx,double jd,double dis,double pt[2],double &prjml)
{
	//1计算直线
	double spt[2],ept[2],dis1,dis2,mdis,xpt[2],mpt[2],FxMpt[2];
	xlpoint PZ;
	double projml;
	PZ.lc = cml;
	pm->xlpoint_pz(&PZ);
	FxMpt[0]=PZ.x + distozx*cos(PZ.a+ZorY*0.5*pi);
	FxMpt[1]=PZ.y + distozx*sin(PZ.a+ZorY*0.5*pi);
	spt[0]=FxMpt[0],spt[1]=FxMpt[1];
	ept[0] = FxMpt[0] + 20.0*cos(PZ.a + jd*pi/180.0);
	ept[1] = FxMpt[1] + 20.0*sin(PZ.a + jd*pi/180.0);
	//2取两点计算距中线距离
	dis1=0.0;
	projml=pm->PROJ_ML(ept[0],ept[1]);
	PZ.lc = projml;
	pm->xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	dis2 =  fabs(distozx-sqrt((xpt[0]-ept[0])*(xpt[0]-ept[0])+(xpt[1]-ept[1])*(xpt[1]-ept[1])));
	//3二分叠代计算出pt
	mpt[0] = 0.5*(spt[0]+ept[0]);
	mpt[1] = 0.5*(spt[1]+ept[1]);
	projml=pm->PROJ_ML(mpt[0],mpt[1]);
	PZ.lc = projml;
	pm->xlpoint_pz(&PZ);
	xpt[0]=PZ.x,xpt[1]=PZ.y;
	mdis = fabs(distozx-sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1])));

	while(fabs(mdis-dis)>0.0001)
	{
		if(mdis>dis)
			ept[0]=mpt[0],ept[1]=mpt[1];
		else
			spt[0]=mpt[0],spt[1]=mpt[1];
		mpt[0] = 0.5*(spt[0]+ept[0]);
		mpt[1] = 0.5*(spt[1]+ept[1]);
		projml=pm->PROJ_ML(mpt[0],mpt[1]);
		PZ.lc = projml;
		pm->xlpoint_pz(&PZ);
		xpt[0]=PZ.x,xpt[1]=PZ.y;
		mdis = fabs(distozx-sqrt((xpt[0]-mpt[0])*(xpt[0]-mpt[0])+(xpt[1]-mpt[1])*(xpt[1]-mpt[1])));			
	}
	pt[0]=mpt[0],pt[1]=mpt[1];
	prjml = projml;
}

void OutTAB::CalFX(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum)
{

	LPDISPATCH pRange=NULL;
	Range m_range;
	int i, totalnum, row, col, flag1, flag2,m=0;
	long shnum;
	char shname[30], filepath[180], pageinfo[80];

	RoadStr = RoadStr + " " + DuanName;
	//读桩号序列表
	XLDataBase mdb;		
	JD_CENTER *pm;
	GTZDM_ROAD *zdm=NULL;
	double jd;
	// 创建每一道路平面对象

	//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);
	mdb.Read_XLDbs(PrjMdb, L"线元表",RoadName);		
	mdb.Read_XLDbs(PrjMdb, L"断链表",RoadName);

	pm = NULL;
	if(mdb.XYNum>0)
	{
		pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
		//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
		if(mdb.NDL>0)
			pm->setDLB(mdb.DLArray,mdb.NDL);
	}

	double Scml,Ecml,cml,Zx,Zy,Yx,Yy;
	Scml = pm->TYLC(Sdml);
	Ecml = pm->TYLC(Edml);
	zdm = ReadZm(PrjMdb,RoadName, L"全幅",pm);
	//	mdb.Read_XLDbs(PrjMdb, L"桩号序列",RoadName);

	FILE *fpr;
	fpr=_wfopen(L"c:\\123\\辅左1.txt", L"r");
	double ZSta[100],ZJiao[100],YSta[100],YJiao[100],cml1,cml2,pt1[2],pt2[2],pt[2];
	int NSTA;
	xlpoint PZ;
	fwscanf(fpr, L"%d",&NSTA);
	for( i=0; i<NSTA; i++)
	{
		fwscanf(fpr, L"%lf%lf",&cml1,&cml2);
		PZ.lc = cml1;
		pm->xlpoint_pz(&PZ);
		pt1[0] = PZ.x + 38.5*cos(PZ.a-0.5*pi);
		pt1[1] = PZ.y + 38.5*sin(PZ.a-0.5*pi);

		PZ.lc = cml2;
		pm->xlpoint_pz(&PZ);
		pt2[0] = PZ.x + 27.5*cos(PZ.a-0.5*pi);
		pt2[1] = PZ.y + 27.5*sin(PZ.a-0.5*pi);
		double fwj,pt[2];
		xyddaa(pt1[0],pt1[1],pt2[0],pt2[1],&fwj);
		pm->CalJD_ZX_XL(pt1,fwj,pt,ZSta[i]);		 
	}

	for( i=0; i<NSTA; i++)
		fwscanf(fpr, L"%lf",&ZJiao[i]);
	fclose(fpr);



	fpr=_wfopen(L"c:\\123\\辅右1.txt", L"r");
	fwscanf(fpr, L"%d",&NSTA);
	for( i=0; i<NSTA; i++)
	{
		fwscanf(fpr, L"%lf%lf",&cml1,&cml2);
		PZ.lc = cml1;
		pm->xlpoint_pz(&PZ);
		pt1[0] = PZ.x + 38.5*cos(PZ.a+0.5*pi);
		pt1[1] = PZ.y + 38.5*sin(PZ.a+0.5*pi);

		PZ.lc = cml2;
		pm->xlpoint_pz(&PZ);
		pt2[0] = PZ.x + 27.5*cos(PZ.a+0.5*pi);
		pt2[1] = PZ.y + 27.5*sin(PZ.a+0.5*pi);

		double fwj,pt[2];
		xyddaa(pt1[0],pt1[1],pt2[0],pt2[1],&fwj);
		pm->CalJD_ZX_XL(pt1,fwj,pt,YSta[i]);	
		//		if(fabs(YSta[i]-28388.861)<0.001)
		//		{
		//			ads_printf(L"%lf %lf %lf %lf\n",pt1[0],pt1[1],pt2[0],pt2[1]);
		//		}
	}

	for( i=0; i<NSTA; i++)
		fwscanf(fpr, L"%lf",&YJiao[i]);
	fclose(fpr);


	FILE *Zfpw,*Yfpw;
	Zfpw = _wfopen(L"c:\\123\\辅线左.txt", L"w");
	Yfpw = _wfopen(L"c:\\123\\辅线右.txt", L"w");

	double ZX_Pt[6][3],YX_Pt[6][3];
	double prjml,bh,z;
	double lmhp;

	for(i=0;i<NSTA;i++)
	{   
		cml = pm->TYLC(ZSta[i]);  
		jd = ZJiao[i];
		CalPZXPT(pm,cml,270+jd,27.1,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		//	  ads_printf(L"z=%0.3lf %lf\n",z,prjml);	
		ZX_Pt[5][0] = pt[0];
		ZX_Pt[5][1] = pt[1];
		ZX_Pt[5][2] = z + 0.4*0.02;

		//	  if(cml<28634.37)
		//		  jd = 27.252;
		//	  else if(cml<28714.64)
		//		  jd = 28.748;
		//	  else if(cml<28794.64)
		//		  jd = 31.275;
		//	  else if(cml<28810.4)
		//		  jd = 33.026;

		CalPZXPT(pm,cml,270+jd,30,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[4][0] = pt[0];
		ZX_Pt[4][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[4][2] = bh - (30-27.1)*0.02;

		CalPZXPT(pm,cml,270+jd,32.875,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[3][0] = pt[0];
		ZX_Pt[3][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[3][2] = bh - (32.875-27.1)*0.02;

		CalPZXPT(pm,cml,270+jd,35.75,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[2][0] = pt[0];
		ZX_Pt[2][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[2][2] = bh - (35.75-27.1)*0.02;

		CalPZXPT(pm,cml,270+jd,38.65,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		ZX_Pt[1][0] = pt[0];
		ZX_Pt[1][1] = pt[1];
		bh = z + 0.4*0.02;
		ZX_Pt[1][2] = bh - (38.65-27.1)*0.02;

		fwprintf(Zfpw, L"%0.3lf ",ZSta[i]);
		int j;
		for( j=1; j<=5; j++)
			fwprintf(Zfpw, L"%0.3lf %0.3lf %0.3lf ",ZX_Pt[j][0],ZX_Pt[j][1],ZX_Pt[j][2] );

		fwprintf(Zfpw, L"\n");


		/////////////////////////////////////////////////////////
		cml = pm->TYLC(YSta[i]); 
		jd = YJiao[i];
		CalPZXPT(pm,cml,90+jd,27.1,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);

		//		  if(cml<28634.4)
		//			  jd = 27.252;
		//		  else if(cml<28714.2)
		//			  jd = 28.748;
		//		  else if(cml<28794.2)
		//			  jd = 31.275;
		//		  else if(cml<28810.4)
		//			  jd = 33.026;
		//		  
		//	  lmhp =  Callmhp(PZ.lc);

		YX_Pt[5][0] = pt[0];
		YX_Pt[5][1] = pt[1];
		YX_Pt[5][2] = z + 0.4*0.02;

		CalPZXPT(pm,cml,90+jd,30,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[4][0] = pt[0];
		YX_Pt[4][1] = pt[1];
		bh =  z + 0.4*0.02;
		YX_Pt[4][2] = bh  - (30-27.1)*0.02;

		CalPZXPT(pm,cml,90+jd,32.875,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[3][0] = pt[0];
		YX_Pt[3][1] = pt[1];
		bh =  z + 0.4*0.02;
		YX_Pt[3][2] = bh  - (32.875-27.1)*0.02;

		CalPZXPT(pm,cml,90+jd,35.75,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[2][0] = pt[0];
		YX_Pt[2][1] = pt[1];
		bh =  z + 0.4*0.02;
		YX_Pt[2][2] = bh  - (35.75-27.1)*0.02;


		CalPZXPT(pm,cml,90+jd,38.65,pt,prjml);
		PZ.lc = prjml;
		if(zdm)
			z = zdm->rdes_level(prjml);
		lmhp =  Callmhp(PZ.lc);
		YX_Pt[1][0] = pt[0];
		YX_Pt[1][1] = pt[1];
		bh =  z + 0.4*0.02;
		YX_Pt[1][2] = bh  - (38.65-27.1)*0.02;





		//	  Zy = y + 32.875*sin(PZ.a-0.5*pi);
		//      Yx = x + 32.875*cos(PZ.a+0.5*pi);
		//	  Yy = y + 32.875*sin(PZ.a+0.5*pi);
		//	  if(cml>28180&&cml<29050)

		fwprintf(Yfpw, L"%0.3lf ",YSta[i]);
		for( j=1; j<=5; j++)
			fwprintf(Yfpw, L"%0.3lf %0.3lf %0.3lf ",YX_Pt[j][0],YX_Pt[j][1],YX_Pt[j][2] );

		fwprintf(Yfpw, L"\n");

	}
	fclose(Zfpw);
	fclose(Yfpw);

}


void OutTAB::OUT_QBTB()
{
	//对话框输入里程分段数据
	int nfd;
	double *fdml=NULL;
	QBTMLDlg dlg;
	CString mdbname,roadname;
	if(dlg.DoModal()==IDOK)
	{
		nfd = dlg.NFD;
		if(nfd<1)
			return;
		fdml = new double[nfd*3];
		int i;
		for( i=0; i<nfd; i++)
		{
			fdml[i*3]=dlg.FDML[i*2];
			fdml[i*3+1]=dlg.FDML[i*2+1];
			fdml[i*3+2]=0.0;
		}
		if(dlg.FDML) delete []dlg.FDML;

		mdbname = dlg.m_mdbname;
		roadname = dlg.RoadName;
		XLDataBase mdb;		
		JD_CENTER *pm;
		// 创建每一道路平面对象

		//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
		mdb.Read_XLDbs(mdbname, L"线元表",roadname);			
		mdb.Read_XLDbs(mdbname, L"断链表",roadname);
		pm = NULL;

		if(mdb.XYNum>0)
		{
			pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
			//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
			if(mdb.NDL>0)
				pm->setDLB(mdb.DLArray,mdb.NDL);
		} 
		else
			return;


		//整理成按公里分段\

		int NFD=0;
		double FDML[3000];
		double sml,eml;
		for(i=0; i<nfd; i++)
		{
			sml = pm->TYLC(fdml[i*3]);
			eml = pm->TYLC(fdml[i*3+1]);

			pm->FormLCB(1000.0,sml,eml);
			for(int j=0; j<pm->pLCB.GetSize()-1; j++)
			{
				if(pm->pLCB[j+1].lc-pm->pLCB[j].lc > 1.0)
				{
					FDML[NFD*3]=pm->XLC1(pm->pLCB[j].lc);
					FDML[NFD*3+1]=pm->XLC1(pm->pLCB[j+1].lc);
					FDML[NFD*3+2]=0.0;
					NFD++;
				}
			}
		}



		//读qbt文件
		ReadWorkdir();
		CString QBTfname;
		CString name,Dir;
		name=mdbname;
		name.MakeUpper();
		ReadWorkdir();

		Dir=Cworkdir;
		Dir.MakeUpper();
		Cworkdir=Dir;
		name.Replace(Cworkdir,NULL);
		name.Replace(L"\\DATA\\", NULL);
		name.Replace(L".MDB", NULL);
		QBTfname =Cworkdir + "\\data\\" + "hdmQBT." + name + "." + roadname;
		// 读总桩数
		FILE *fp;
		if((fp=_wfopen(QBTfname, L"r"))==NULL)
		{
			AfxMessageBox(L"无法打开清表土文件1!");
			exit(0);
		}
		int ncrs;
		QBTnode *crs;

		fwscanf(fp, L"%d",&ncrs);
		if(ncrs<1)
		{
			ads_alert(L"请先设计横断面!");
			return;
		}
		crs = new QBTnode[ncrs];
		double dtmp;
		for(i=0; i<ncrs; i++)
			fwscanf(fp, L"%lf%lf%lf%lf%lf%lf",&crs[i].cml,&dtmp,&dtmp,&dtmp,&crs[i].Aqbt,&crs[i].Awyn);
		fclose(fp);
		//计算数量
		double len;
		for( i=0; i<NFD; i++)
			FDML[i*3+2]=Cal_QBTVol(FDML[i*3],FDML[i*3+1],ncrs,crs,1,len);
		//填表
		ExcelLoad();
		LPDISPATCH pWorkbooks;
		CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
		m_app.SetUserControl(TRUE);
		if(pWnd!=NULL)
		{
			TRACE("Excel window found\n");
			pWnd->ShowWindow(SW_SHOWMAXIMIZED);
			pWnd->UpdateWindow();
			pWnd->BringWindowToTop();
		}
		VERIFY(pWorkbooks=m_app.GetWorkbooks());
		m_workbooks.AttachDispatch(pWorkbooks);
		LPDISPATCH pWorkbook = NULL;
		LPDISPATCH pWorksheets = NULL;
		//Open  Excel templete file;
		ACHAR filepath[280];
		_stprintf(filepath, L"%s\\lib\\excel\\砍树挖根数量表.xlt",SysPath);
		VERIFY(pWorkbook=m_workbooks.Open(filepath));

		m_workbook.AttachDispatch(pWorkbook);
		pWorksheets=m_workbook.GetWorksheets();
		ASSERT(pWorksheets!=NULL);
		m_worksheets.AttachDispatch(pWorksheets);

		LPDISPATCH pRange=NULL;
		Range m_range;
		LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
		m_worksheet.AttachDispatch(pWorksheet);
		m_worksheet.Select();

		ACHAR str[256],shname[256];
		long shnum=1;
		int row=11;
		for( i=0;i<NFD;i++)
		{	

			if(i>0 && i%29==0)
			{
				// 复制模板工作表然后填写

				//	m_worksheets.Copy(COleVariant(short(shnum)),COleVariant(short(shnum+2)));

				m_app.Run(COleVariant(L"mycro"),COleVariant(shnum));

				pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
				m_worksheet.AttachDispatch(pWorksheet);
				m_worksheet.Select();
				_stprintf(shname, L"砍树挖根数量表%d",shnum);
				m_worksheet.SetName(shname);
				shnum++;
				row=11;
			}

			pRange = m_worksheet.GetCell(row,1);
			m_range.AttachDispatch(pRange);	  
			//	m_range.SetValue(COleVariant(long(i%29)));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			_stprintf(str, L"%s～%s",pm->XLC(FDML[i*3]),pm->XLC(FDML[i*3+1]));
			m_range.SetValue(COleVariant(str));
			pRange = m_worksheet.GetCell(row,14);
			m_range.AttachDispatch(pRange);	  
			m_range.SetValue(COleVariant(FDML[i*3+2]));
			row++;
		}

		//删除模板工作表

		if(crs)	delete []crs;
		if(fdml)  delete []fdml;

	}

}

double  OutTAB::Cal_QBTVol(double sml,double eml,int ncrs,QBTnode crs[],int IFQB,double &AllLen)
{
	double Vol=0.0,len;
	int i,sno,eno;

	AllLen = 0.0;
	i=0;
	while(i<ncrs && crs[i].cml<sml-0.001)
		i++;
	sno = i;

	if(sno>=ncrs)
		return 0.0;

	i=sno;
	while(i<ncrs && crs[i].cml<eml+0.001)
		i++;
	eno = i;
	if(eno>=ncrs)
		eno=ncrs-1;


	for(i=sno; i<eno;i++)
	{
		len = crs[i+1].cml - crs[i].cml;
		AllLen = AllLen + len;
		if(IFQB)//1-清表
			Vol = Vol  + (crs[i+1].Aqbt+crs[i].Aqbt)*0.5*len/100.0;
		else//挖淤泥
			Vol = Vol + (crs[i+1].Awyn+crs[i].Awyn)*0.5*len;

	}

	return Vol;

}


void OutTAB::OUT_WYNB()//挖淤泥表
{
	//对话框输入里程分段数据
	int nfd;
	double *fdml=NULL;
	QBTMLDlg dlg;
	CString mdbname,roadname;
	if(dlg.DoModal()==IDOK)
	{

		nfd = dlg.NFD;
		if(nfd<1)
			return;
		fdml = new double[nfd*2];
		int i;
		for(i=0; i<nfd; i++)
		{
			fdml[i*2]=dlg.FDML[i*2];
			fdml[i*2+1]=dlg.FDML[i*2+1];
		}

		///////////////
		// 		int NFD;
		// 		double *FDML=NULL;
		// 		NFD = dlg.NFD;
		// 		
		// 		if(NFD<1)
		// 			return;
		// 		FDML = new double[NFD*4];
		// 		for(int i=0; i<NFD; i++)
		// 		{
		// 			FDML[i*4]=dlg.FDML[i*2];
		// 			FDML[i*4+1]=dlg.FDML[i*2+1];
		// 			FDML[i*4+2]=0.0;
		// 			FDML[i*4+3]=0.0;
		// 		}
		if(dlg.FDML) delete []dlg.FDML;
		mdbname = dlg.m_mdbname;
		roadname = dlg.RoadName;
		XLDataBase mdb;		
		JD_CENTER *pm;
		// 创建每一道路平面对象

		//	mdb.Read_XLDbs(PrjMdb, L"控制点表",RoadName);	
		mdb.Read_XLDbs(mdbname, L"线元表",roadname);			
		mdb.Read_XLDbs(mdbname, L"断链表",roadname);
		pm = NULL;

		if(mdb.XYNum>0)
		{
			pm=(JD_CENTER *)new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
			//    	pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
			if(mdb.NDL>0)
				pm->setDLB(mdb.DLArray,mdb.NDL);
		} 
		else
			return;

		int NFD=0;
		double FDML[4000];
		double sml,eml;
		for(i=0; i<nfd; i++)
		{
			sml = pm->TYLC(fdml[i*2]);
			eml = pm->TYLC(fdml[i*2+1]);

			pm->FormLCB(1000.0,sml,eml);
			for(int j=0; j<pm->pLCB.GetSize()-1; j++)
			{
				if(pm->pLCB[j+1].lc-pm->pLCB[j].lc > 1.0)
				{
					FDML[NFD*4]=pm->XLC1(pm->pLCB[j].lc);
					FDML[NFD*4+1]=pm->XLC1(pm->pLCB[j+1].lc);
					FDML[NFD*4+2]=0.0;
					FDML[NFD*4+3]=0.0;
					NFD++;
				}
			}
		}

		//读qbt文件
		ReadWorkdir();
		CString QBTfname;
		CString name,Dir;
		name=mdbname;
		name.MakeUpper();
		ReadWorkdir();

		Dir=Cworkdir;
		Dir.MakeUpper();
		Cworkdir=Dir;
		name.Replace(Cworkdir,NULL);
		name.Replace(L"\\DATA\\", NULL);
		name.Replace(L".MDB", NULL);
		QBTfname =Cworkdir + "\\data\\" + "hdmQBT." + name + "." + roadname;
		// 读总桩数
		FILE *fp;
		if((fp=_wfopen(QBTfname, L"r"))==NULL)
		{
			AfxMessageBox(L"无法打开清表土文件1!");
			exit(0);
		}
		int ncrs;
		QBTnode *crs;

		fwscanf(fp, L"%d",&ncrs);
		if(ncrs<1)
		{
			ads_alert(L"请先设计横断面!");
			return;
		}
		crs = new QBTnode[ncrs];
		double dtmp;
		for(i=0; i<ncrs; i++)
			fwscanf(fp, L"%lf%lf%lf%lf%lf%lf",&crs[i].cml,&dtmp,&dtmp,&dtmp,&crs[i].Aqbt,&crs[i].Awyn);
		fclose(fp);
		//计算数量
		double len;
		for( i=0; i<NFD; i++)
			FDML[i*4+2]=Cal_QBTVol(FDML[i*4],FDML[i*4+1],ncrs,crs,0,FDML[i*4+3]);
		//填表
		ExcelLoad();
		LPDISPATCH pWorkbooks;
		CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
		m_app.SetUserControl(TRUE);
		if(pWnd!=NULL)
		{
			TRACE("Excel window found\n");
			pWnd->ShowWindow(SW_SHOWMAXIMIZED);
			pWnd->UpdateWindow();
			pWnd->BringWindowToTop();
		}
		VERIFY(pWorkbooks=m_app.GetWorkbooks());
		m_workbooks.AttachDispatch(pWorkbooks);
		LPDISPATCH pWorkbook = NULL;
		LPDISPATCH pWorksheets = NULL;
		//Open  Excel templete file;
		ACHAR filepath[280];
		_stprintf(filepath, L"%s\\lib\\excel\\挖淤泥排水数量表.xlt",SysPath);
		VERIFY(pWorkbook=m_workbooks.Open(filepath));

		m_workbook.AttachDispatch(pWorkbook);
		pWorksheets=m_workbook.GetWorksheets();
		ASSERT(pWorksheets!=NULL);
		m_worksheets.AttachDispatch(pWorksheets);

		LPDISPATCH pRange=NULL;
		Range m_range;
		LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
		m_worksheet.AttachDispatch(pWorksheet);
		m_worksheet.Select();

		ACHAR str[256],shname[256];
		long shnum=1;
		int row=9;
		for( i=0;i<NFD;i++)
		{	

			if(i>0 && i%31==0)
			{
				// 复制模板工作表然后填写

				m_app.Run(COleVariant(L"mycro"),COleVariant(shnum));

				pWorksheet=m_worksheets.GetItem(COleVariant(short(shnum+1)));
				m_worksheet.AttachDispatch(pWorksheet);
				m_worksheet.Select();
				_stprintf(shname, L"挖淤泥排水数量表%d",shnum);
				m_worksheet.SetName(shname);
				shnum++;
				row=9;
			}

			pRange = m_worksheet.GetCell(row,1);
			m_range.AttachDispatch(pRange);	  
			//	m_range.SetValue(COleVariant(long(i%29)));
			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			_stprintf(str, L"%s～%s",pm->XLC(FDML[i*4]),pm->XLC(FDML[i*4+1]));
			m_range.SetValue(COleVariant(str));

			pRange = m_range.GetNext();
			m_range.AttachDispatch(pRange);
			acdbRToS(FDML[i*4+3],2,3,str);
			m_range.SetValue(COleVariant(str));

			pRange = m_worksheet.GetCell(row,7);
			m_range.AttachDispatch(pRange);	  
			m_range.SetValue(COleVariant(FDML[i*4+2]));
			row++;
		}

		//删除模板工作表

		if(crs)	delete []crs;
		if(fdml)  delete []fdml;

	}

}
//判别给定里程是否在桩号序列表中
bool OutTAB::CheckIfInSta(double cml,int NSTA,	LCBdata STA[])
{
	for(int i=0; i<NSTA; i++)
	{
		if(STA[i].lc>cml+0.1)
			return false;
		if(fabs(STA[i].lc-cml)<0.001)
			return true;
	}
	return false;
}


//--------------------------------------------------------------------------------------
//将坐标轴平移offsetX（左负右正）后，形成新的地面线坐标
void OutTAB::CalNewZB(double offsetX,double earh,double Learpt[],int lnpt,double Rearpt[],int rnpt,
					  double &NewYd,double NewLearpt[],int &newLnpt,double NewRearpt[],int &newRnpt)
{
	int ptnum;
	//NewYd-新的中心地面高
	if(offsetX<0.0)//左移
		Gety(fabs(offsetX),lnpt,Learpt,NewYd,ptnum);   
	else
		Gety(offsetX,rnpt,Rearpt,NewYd,ptnum);   

	double earpt[500];
	int npt=0;
	int i;
	for( i=lnpt-1; i>=0; i--)
	{
		earpt[npt*2] = -1*fabs(Learpt[i*2]) - offsetX;
		earpt[npt*2+1] =  Learpt[i*2+1]-NewYd; 
		npt++;
	}

	for(i=0; i<rnpt; i++)
	{
		earpt[npt*2] = Rearpt[i*2] - offsetX;
		earpt[npt*2+1] = Rearpt[i*2+1] - NewYd; 
		npt++;
	}

	newLnpt=1;
	newRnpt=1;
	NewLearpt[0*2] = 0;
	NewLearpt[0*2+1] = 0;
	NewRearpt[0*2] = 0;
	NewRearpt[0*2+1] = 0;
	for(i=0; i<npt; i++)
	{
		if(earpt[i*2]<0.0)
		{
			//NewLearpt[newLnpt*2] = fabs(earpt[i*2]);
			//NewLearpt[newLnpt*2+1] = earpt[i*2+1];
			newLnpt++;
		}
		else
		{
			NewRearpt[newRnpt*2] = fabs(earpt[i*2]);
			NewRearpt[newRnpt*2+1] = earpt[i*2+1];
			newRnpt++;
		}		
	}
	int j=1;
	for(i=newLnpt-2;i>=0;i--)
	{
		NewLearpt[j*2] = fabs(earpt[i*2]);//左为正
		NewLearpt[j*2+1] = earpt[i*2+1];
		j++;
	}
	NewYd = NewYd + earh;
}

int OutTAB::Gety(double x,int ptsum,double ptlist[],double &Yd , int &ptnum)
{
	int i;

	i = 0;
	Yd = ptlist[1];
	ptnum = 0;

	while(x > ptlist[i*2] + 0.0001&& i<ptsum)
		i++;

	if(i==ptsum)
	{
		Yd = ptlist[(i-1)*2];
		ptnum = i-1;
		return 0;//找不到
	}
	if(i==0)
		return -1;		

	ptnum = i-1;
	Yd = ptlist[(i-1)*2+1] + (x-ptlist[(i-1)*2])*(ptlist[i*2+1]-ptlist[(i-1)*2+1])/(ptlist[i*2]-ptlist[(i-1)*2]);
	return 1;//找到
}









