// GouPDSheet.cpp : implementation file
//

#include "stdafx.h"
#include "GouPDSheet.h"
#include "XLDataBase.h"
#include "HdmdataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] =__FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GouPDSheet

IMPLEMENT_DYNAMIC(GouPDSheet, CPropertySheet)

GouPDSheet::GouPDSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

GouPDSheet::GouPDSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	GouPDDlgAry=NULL;
	xlnum = 0;
	road = NULL;

	INIT();
}

GouPDSheet::~GouPDSheet()
{
}


BEGIN_MESSAGE_MAP(GouPDSheet, CPropertySheet)
	//{{AFX_MSG_MAP(GouPDSheet)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

// GouPDSheet message handlers

BOOL GouPDSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	// TODO:  在此添加您的专用代码
	ReadWorkdir();
	CString title;
	title = L"当前方案名: " + Pro;
	this->SetWindowText(title);

	return bResult;
}

BOOL GouPDSheet::INIT() 
{
	CString label;
	ReadWorkdir();

	//CString  HdmFname;
	mdbFname = Cworkdir+L"\\data\\"+Pro+L".mdb";
	//读道路总数
	XLDataBase mdb;
	mdb.ReadAllRoadName(mdbFname);//读取线路数据库中断链表的数据

	xlnum = mdb.m_RoadSum;
	if(xlnum<1)
	{
		AfxMessageBox(L"当前道路总数为0，请先指定当前方案名称和道路名!");
		return false;
	}
	if(xlnum>0)
	{
		if(road) delete[]road; road=NULL;
		road=new RoadGouPDdata[xlnum];
		if(GouPDDlgAry) delete[]GouPDDlgAry; GouPDDlgAry=NULL;
		GouPDDlgAry = new GouPDPage[xlnum*2];
	}

	HdmDataBase hdmmdb;

	int k=0;
	for(int i=0; i<xlnum; i++)
	{
		// 创建每一道路平面对象
		road[i].roadname=mdb.Road[i].RoadName;//左绕行段线路性质为-1
		//		   mdb.Read_XLDbs(mdbFname,"控制点表",road[i].roadname);
		mdb.Read_XLDbs(mdbFname,"线元表",road[i].roadname);				   
		mdb.Read_XLDbs(mdbFname,"断链表",road[i].roadname);

		road[i].pm = NULL;
		if(mdb.XYNum>0)
		{
			road[i].pm=new JD_CENTER(mdb.XYArray,mdb.XYNum);//主线对象
			//			 road[i].pm->SetJdCenterArray(mdb.JdCenArray,mdb.NJdCen);
			if(mdb.NDL>0)
				road[i].pm->setDLB(mdb.DLArray,mdb.NDL);
		}

		//从数据库中读取
		hdmmdb.Read_HdmDbs(mdbFname,L"左沟底坡",road[i].roadname);
		road[i].NZGouPD = hdmmdb.NZGouPD;
		road[i].ZGouPD.RemoveAll();
		int j;
		for(j=0; j<road[i].NZGouPD; j++)
			road[i].ZGouPD.Add(hdmmdb.ZGouPD[j]);  	   		   
		if(hdmmdb.ZGouPD) delete []hdmmdb.ZGouPD;
		hdmmdb.ZGouPD = NULL;
		hdmmdb.NZGouPD = 0;

		hdmmdb.Read_HdmDbs(mdbFname,L"右沟底坡",road[i].roadname);
		road[i].NYGouPD = hdmmdb.NYGouPD;
		road[i].YGouPD.RemoveAll();
		for(j=0; j<road[i].NYGouPD; j++)
			road[i].YGouPD.Add(hdmmdb.YGouPD[j]);  	   		   
		if(hdmmdb.YGouPD) delete []hdmmdb.YGouPD;
		hdmmdb.YGouPD = NULL;
		hdmmdb.NYGouPD = 0;

		GouPDDlgAry[k].pm = road[i].pm;
		GouPDDlgAry[k].NGouPD = road[i].NZGouPD;
		GouPDDlgAry[k].mdbFname = mdbFname;
		GouPDDlgAry[k].roadname = road[i].roadname;
		GouPDDlgAry[k].ZorY = -1;
		for(j=0; j<road[i].NZGouPD; j++)
			GouPDDlgAry[k].GouPD.Add(road[i].ZGouPD[j]);

		road[i].ZGouPDName= road[i].roadname + L"-左沟";
		GouPDDlgAry[k].m_psp.pszTitle=road[i].ZGouPDName ;
		AddPage(&GouPDDlgAry[k]);

		k++;
		GouPDDlgAry[k].pm = road[i].pm;
		GouPDDlgAry[k].NGouPD = road[i].NYGouPD;
		GouPDDlgAry[k].roadname = road[i].roadname;
		GouPDDlgAry[k].ZorY = 1;
		for(j=0; j<road[i].NYGouPD; j++)
			GouPDDlgAry[k].GouPD.Add(road[i].YGouPD[j]);

		road[i].YGouPDName = road[i].roadname + L"-右沟";
		GouPDDlgAry[k].m_psp.pszTitle=road[i].YGouPDName;
		AddPage(&GouPDDlgAry[k]);
		k++;

	}

}


void GouPDSheet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	//		//写数据库
	//		HdmDataBase xlmdb;
	//		for(int i=0; i<dlg.xlnum*2; i++)
	//		{
	//			if(dlg.GouPDDlgAry[i].ZorY==-1)
	//			{
	//		        xlmdb.NZGouPD = dlg.GouPDDlgAry[i].NGouPD;
	//			    if(xlmdb.ZGouPD) delete[]xlmdb.ZGouPD;xlmdb.ZGouPD=NULL;
	//			    if(xlmdb.NZGouPD>0)
	//				{
	//			       xlmdb.ZGouPD = new  BAS_DRAW_FUN::GouPDdata[xlmdb.NZGouPD];
	//			       for(int j=0; j<xlmdb.NZGouPD; j++)
	//                        xlmdb.ZGouPD[j] = dlg.GouPDDlgAry[i].GouPD[j];
	//			   //写数据库
	//                   xlmdb.WriteGouPDB(dlg.mdbFname,dlg.road[i].roadname,"左沟底坡"); 
	//			    ads_printf(L"\n%s的左沟底坡已存入%s中!\n",dlg.road[i].roadname,dlg.mdbFname);
	//				}
	//			}
	//			else
	//			{
	//			    xlmdb.NYGouPD = dlg.GouPDDlgAry[i].NGouPD;
	//			    if(xlmdb.YGouPD) delete[]xlmdb.YGouPD;xlmdb.YGouPD=NULL;
	//			    if(xlmdb.NYGouPD>0)
	//				{
	//			       xlmdb.YGouPD = new  BAS_DRAW_FUN::GouPDdata[xlmdb.NYGouPD];
	//			       for(int j=0; j<xlmdb.NYGouPD; j++)
	//                        xlmdb.YGouPD[j] = dlg.GouPDDlgAry[i].GouPD[j];
	//			   //写数据库
	//                   xlmdb.WriteGouPDB(dlg.mdbFname,dlg.road[i].roadname,"右沟底坡"); 
	//			       ads_printf(L"\n%s的右沟底坡已存入%s中!\n",dlg.road[i].roadname,dlg.mdbFname);
	//				}
	//			}
	//		}
	//
	CPropertySheet::OnClose();
}


void GouPDSheet::OnFinalRelease() 
{
	// TODO: Add your specialized code here and/or call the base class

	CPropertySheet::OnFinalRelease();
}
