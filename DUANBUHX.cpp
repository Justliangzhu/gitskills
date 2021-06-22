// DUANBUHX.cpp : implementation file
//

#include "stdafx.h"
#include "DUANBUHX.h"
#include "XLDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DUANBUHX dialog


DUANBUHX::DUANBUHX(CWnd* pParent /*=NULL*/)
	: CDialog(DUANBUHX::IDD, pParent)
{
	//{{AFX_DATA_INIT(DUANBUHX)
	m_Edml = _T("0");
	m_Sdml = _T("0");
	m_ZhSerial = _T("");
	m_ZhSpacer = _T("10");
	m_HxAng = _T("90");
	m_HxBh = _T("1");
	m_Show = L"";
	m_pm = NULL;
	NZID=NYID=0;
	//}}AFX_DATA_INIT
}


void DUANBUHX::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DUANBUHX)
	DDX_Text(pDX, IDC_EDIT_DBEDML, m_Edml);
	DDX_Text(pDX, IDC_EDIT_DBSDML, m_Sdml);
	DDX_Text(pDX, IDC_EDIT_ZHSERIAL, m_ZhSerial);
	DDX_Text(pDX, IDC_EDIT_ZHSPACER, m_ZhSpacer);
	DDX_Text(pDX, IDC_EDIT_HXANG, m_HxAng);
	DDX_Text(pDX,IDC_STATIC_HXBH, m_Show);
	DDX_Text(pDX,IDC_EDIT_HXBH, m_HxBh);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DUANBUHX, CDialog)
	//{{AFX_MSG_MAP(DUANBUHX)
	ON_BN_CLICKED(IDC_BUTTON_PICKST, OnButtonPickst)
	ON_BN_CLICKED(IDC_BUTTON_PICKED, OnButtonPicked)
	ON_BN_CLICKED(IDC_BUTTON_PICKADD, OnButtonPickadd)
	ON_BN_CLICKED(IDC_BUTTON_DBSELXL, OnButtonSelxl)
	ON_BN_CLICKED(IDC_BUTTON_PICKZCBX, OnButtonPickzcbx)
	ON_BN_CLICKED(IDC_BUTTON_PICKYCBX, OnButtonPickycbx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DUANBUHX message handlers

BOOL DUANBUHX::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	LcArr.removeSubArray(0,LcArr.length()-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DUANBUHX::OnButtonPickst() 
{
	// TODO: Add your control notification handler code here
	if(m_pm==NULL)
	{
		ads_printf(L"先选择平面方案!\n");
		return;
	}
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	AcGePoint3d Pt;
	acedGetPoint(NULL,L"\n请拾取起点里程:\n",asDblArray(Pt));

	m_Sdml.Format(L"%0.3lf",m_pm->XLC1(m_pm->PROJ_ML(Pt.y,Pt.x)));		

	UpdateData(false);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable 	
}

void DUANBUHX::OnButtonPicked() 
{
	// TODO: Add your control notification handler code here
	if(m_pm==NULL)
	{
		ads_printf(L"先选择平面方案!\n");
		return;
	}
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	AcGePoint3d Pt;
	acedGetPoint(NULL,L"\n请拾取终点里程:\n",asDblArray(Pt));

	m_Edml.Format(L"%0.3lf",m_pm->XLC1(m_pm->PROJ_ML(Pt.y,Pt.x)));		

	UpdateData(false);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable 		
}

void DUANBUHX::OnButtonPickadd() 
{
	// TODO: Add your control notification handler code here
	if(m_pm==NULL)
	{
		ads_printf(L"先选择平面方案!\n");
		return;
	}
	UpdateData();
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	AcGePoint3d Pt;
	acedGetPoint(NULL,L"\n请拾取里程:\n",asDblArray(Pt));
	CString str;
	str.Format(L"%0.3lf",m_pm->XLC1(m_pm->PROJ_ML(Pt.y,Pt.x)));
	if(m_ZhSerial!="")
		m_ZhSerial=m_ZhSerial+L","+str;
	else
		m_ZhSerial=str;
	UpdateData(false);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable 		
}

void DUANBUHX::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	int i,index=0,len,inf=1;
	double sml,eml,cml;
	AcGePoint2d pt;
	CString str;
	if(m_pm)
	{
		sml = m_pm->TYLC(_wtof(m_Sdml));
		eml = m_pm->TYLC(_wtof(m_Edml));
		m_pm->FormLCB(10,sml,eml);
		pt.y=0;
		for(i=0;i<m_pm->NLCB;i++)
		{
			pt.x = m_pm->pLCB[i].lc;
			LcArr.append(pt);
		}

		while(inf)
		{
			index = m_ZhSerial.Find(L",");
			if(index>=0)
				str = m_ZhSerial.Left(index);
			else
			{
				str = m_ZhSerial;
				inf = 0;
			}
			len = m_ZhSerial.GetLength();
			m_ZhSerial = m_ZhSerial.Right(len-index-1);
			if(str.GetLength()>1)
			{
				cml = m_pm->TYLC(_wtof(str));
				for(i=0;i<LcArr.length();i++)
				{
					if(cml <= m_pm->pLCB[i].lc)
						break;
				}
				if(fabs(cml-m_pm->pLCB[i].lc)>0.001)
				{
					pt.x = cml;
					LcArr.insertAt(i,pt);
				}
			}
		}

		AcGePoint3d spt,ept,sspt,eept;
		BAS_DRAW_FUN::xlpoint PZ;
		int j;
		AcDbObject *pobj;
		AcDbPolyline *pent;
		AcGePoint3dArray pts;
		AcGePlane plane(AcGePoint3d(0,0,0),AcGeVector3d(0,0,1));
		AcDbObjectId eId;
		double ang = _wtof(m_HxAng);
		for(i=0;i<LcArr.length();i++)
		{
			PZ.lc = LcArr[i].x;
			m_pm->xlpoint_pz(&PZ);
			spt.z=ept.z=0;
			spt.y = PZ.x + 200*cos(PZ.a-(180-ang)*PI/180);
			spt.x = PZ.y + 200*sin(PZ.a-(180-ang)*PI/180);
			ept.y = PZ.x + 200*cos(PZ.a+ang*PI/180);
			ept.x = PZ.y + 200*sin(PZ.a+ang*PI/180);
			AcDbLine L1(spt,ept);
			for(j=0;j<NZID;j++)
			{
				/*acdbOpenObject(pobj,ZBXID[j],AcDb::kForRead);*/
				if(acdbOpenObject(pobj, ZBXID[j], AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pobj->close();
				pent = AcDbPolyline::cast(pobj);
				L1.intersectWith(pent,AcDb::kOnBothOperands,plane,pts);
				if(pts.length()>0)
				{
					spt.x = pts[0].x;
					spt.y = pts[0].y;
					L1.setStartPoint(spt);					   
					pts.removeSubArray(0,pts.length()-1);
					//				   break;
				}
			}

			for(j=0;j<NYID;j++)
			{
				/* acdbOpenObject(pobj,YBXID[j],AcDb::kForRead);*/
				if(acdbOpenObject(pobj, YBXID[j], AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pobj->close();
				pent = AcDbPolyline::cast(pobj);
				pent->intersectWith(&L1,AcDb::kOnBothOperands,plane,pts);
				if(pts.length()>0)
				{
					ept.x = pts[0].x;
					ept.y = pts[0].y;
					L1.setEndPoint(ept);
					pts.removeSubArray(0,pts.length()-1);
					//				   break;
				}
			}
			spt = L1.startPoint();
			ept = L1.endPoint();
			eId = m_pm->makeline(spt,ept,2,0,L"端部横线");
			m_pm->SetDBXdata(eId,m_pm->mdbname,m_pm->RoadName,LcArr[i].x,m_HxBh);
		}
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	CDialog::OnOK();
}

void DUANBUHX::OnButtonSelxl() 
{
	// TODO: Add your control notification handler code here
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	CString mdbname,roadname;
	double cml;
	XLDataBase DBS;
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.
	//	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	rc = ads_entsel(L"\n选择平面线路实体 : ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
		ads_printf(L"\nError during object selection");
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		//		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
		EnableWindow(TRUE); // Enable our dialog*/
		return;
	}

	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();

	if(pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		m_pm = JD_CENTER::cast(pObj);
	else
	{
		CString str;
		DBS.GetXLXdata(eId,mdbname,roadname);
		if(mdbname!=""&&roadname!="")
		{
			//			DBS.Read_XLDbs(mdbname,"控制点表",roadname);
			DBS.Read_XLDbs(mdbname,L"线元表",roadname);		
			DBS.Read_XLDbs(mdbname,L"断链表",roadname);
			if(DBS.XYNum>0)
			{
				m_pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				//				m_pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					m_pm->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(m_pm->mdbname,mdbname);
				_tcscpy(m_pm->RoadName,roadname);  

				m_Sdml.Format(L"%0.3lf", m_pm->DLArray[0].ELC);
				m_Edml.Format(L"%0.3lf", m_pm->DLArray[m_pm->DLNum-1].BLC);
				m_Show=mdbname+L","+roadname;
				UpdateData(FALSE);				
			}
		}		
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/   		
}

void DUANBUHX::OnButtonPickzcbx() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int i,type,track=1;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	NZID = 0;
	AcDbObject *pobj;
	AcDbPolyline *pent,*prent=NULL;
	while (track>0) 
	{  
		rc = ads_entsel(L"\n添加左侧边线 : ", en, asDblArray(PT));
		if(rc==RTCAN)
		{
			NZID = 0;
			if(prent)
				prent->unhighlight();
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/			   
			return;
		}
		if (rc != RTNORM) 	   
		{   
			if(prent)
				prent->unhighlight();
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/	
			return; 
		}

		AcDbObjectId eId;
		acdbGetObjectId(eId, en);
		/* acdbOpenObject(pobj,eId,AcDb::kForRead);*/
		if(acdbOpenObject(pobj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pobj->close();
		if(pobj->isKindOf(AcDbPolyline::desc()))
		{
			pent = AcDbPolyline::cast(pobj);  
			pent->highlight();
			if(prent)
				prent->unhighlight();
			for(i=0;i<NZID;i++)
				if(ZBXID[i]==eId)
					break;
			if(i==NZID)
			{
				ZBXID[NZID]=eId;
				NZID++;
				if(NZID==5)
				{
					ads_printf(L"最多只能选择五条左侧边线!\n");
					ShowWindow(SW_SHOW); // Display our dialog again
					SetFocus(); // Reset the focus back to ourselves
					EnableWindow(TRUE); // Enable our dialog*/	
					return; 
				}
			}
			prent = pent;
		}

		if(ads_grread (track, &type, &entdata)!=RTNORM)
			break;
		if((type!=5) && (type!=12))
		{
			if(prent)
				prent->unhighlight();
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/	
			return;
		}
	}
	if(prent)
		prent->unhighlight();	
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/		
}

void DUANBUHX::OnButtonPickycbx() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	int rc;
	AcGePoint3d PT,PT1,PT2;
	ads_name en;
	int i,type,track=1;
	struct resbuf entdata;// 鼠标拖动时的动态坐标
	NYID = 0;
	AcDbObject *pobj;
	AcDbPolyline *pent,*prent=NULL;
	while (track>0) 
	{  
		rc = ads_entsel(L"\n添加右侧边线 : ", en, asDblArray(PT));
		if(rc==RTCAN)
		{
			NYID = 0;
			if(prent)
				prent->unhighlight();
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/			   
			return;
		}
		if (rc != RTNORM) 	   
		{   
			if(prent)
				prent->unhighlight();
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/	
			return; 
		}

		AcDbObjectId eId;
		acdbGetObjectId(eId, en);
		/*acdbOpenObject(pobj,eId,AcDb::kForRead);*/
		if(acdbOpenObject(pobj,eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pobj->close();
		if(pobj->isKindOf(AcDbPolyline::desc()))
		{
			pent = AcDbPolyline::cast(pobj);  
			pent->highlight();
			if(prent)
				prent->unhighlight();	   
			for(i=0;i<NYID;i++)
				if(YBXID[i]==eId)
					break;
			if(i==NYID)
			{
				YBXID[NYID]=eId;
				NYID++;
				if(NYID==5)
				{
					ads_printf(L"最多只能选择五条右侧边线!\n");
					ShowWindow(SW_SHOW); // Display our dialog again
					SetFocus(); // Reset the focus back to ourselves
					EnableWindow(TRUE); // Enable our dialog*/	
					return; 
				}
			}
			prent = pent;
		}

		if(ads_grread (track, &type, &entdata)!=RTNORM)
			break;
		if((type!=5) && (type!=12))
		{
			if(prent)
				prent->unhighlight();
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog*/	
			return;
		}
	}	
	if(prent)
		prent->unhighlight();
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/		
}
