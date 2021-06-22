// LocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LocationDlg.h"
#include "BCTTP.h"
#include "BCTPR.h"
#include "BCTPP.h"
#include "BCPPP.h"
#include "BCCreateXLEntity.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// LocationDlg dialog
#include "BAS_DRAW_FUN.h"
#include "XLDataBase.h"
#include "YYQXLink.h"
#include "PolyLineCal.h"

LocationDlg::LocationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LocationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LocationDlg)
	m_A1 = 0.0;
	m_A2 = 0.0;
	m_AorL = 1;//A
	m_A1 = 0.0;
	m_A2 = 0.0;
	m_Radius = 0.0;
	xlId = NULL;
	//}}AFX_DATA_INIT
}

void LocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LocationDlg)
	DDX_Text(pDX, IDC_EDIT_A1, m_A1);
	DDX_Text(pDX, IDC_EDIT_A2, m_A2);
	DDX_Text(pDX, IDC_EDIT_R, m_Radius);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LocationDlg, CDialog)
	//{{AFX_MSG_MAP(LocationDlg)
	ON_BN_CLICKED(IDC_BUTTON_QXB, OnButtonQxb)
	ON_BN_CLICKED(IDC_BUTTON_TTP, OnButtonTtr)
	ON_BN_CLICKED(IDC_BUTTON_TPR, OnButtonTpr)
	ON_BN_CLICKED(IDC_BUTTON_TPP, OnButtonTpp)
	ON_BN_CLICKED(IDC_BUTTON_PPP, OnButtonPpp)
	ON_BN_CLICKED(IDC_BUTTON_INVERT, OnButtonInvert)
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnButtonLink)
	ON_BN_CLICKED(IDC_RADIO_A, OnRadioA)
	ON_BN_CLICKED(IDC_RADIO_Lo, OnRADIOLo)
	ON_BN_CLICKED(IDC_BUTTON_refresh, OnBUTTONrefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LocationDlg message handlers

BOOL LocationDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	//读取设计车速
	/*
	BAS_DRAW_FUN ob;

	ob.ReadGuifanFile();
	Rmax = mGuiFanCS.TJianR2;
	*/

	if(m_AorL == 1)//A)
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_A);
		pBtn->SetCheck(1);
	}
	else
	{
		CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_Lo);
		pBtn->SetCheck(1);
	}

	Rmax=500.0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void LocationDlg::OnButtonQxb() //曲线板
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE);
	////////////////////////////////////////////////////////////////////
	BCARC BcArcObj;
	AcGePoint3d PT;	
	double R,tmpR;
	ACHAR mes[256];
	R=Rmax;
	_stprintf(mes,L"请输入半径(%.2lf):",R);
	int rc = acedGetReal(mes,&R);

	if(rc!=RTCAN && RTNORM == acedGetPoint(NULL,L"请点取圆弧中点:\n",asDblArray(PT)) )
	{
		BcArcObj.SetMidPoint(PT.y,PT.x);
		BcArcObj.SetRadius(R);
		////////////////////////////////////////////////////////////////////
		AcGePoint3d pk;
		int type;
		struct resbuf result;

		acutPrintf(L"/////////R键-重新输入半径，C键-重新点取圆弧中点!/////////\n");
		for(;;)
		{
			if(ads_grread(1 , &type , &result)!=RTNORM)
			{ 
				break;
			}
			else 
			{
				short KEYCODE=result.resval.rint;
				if(KEYCODE==13 || KEYCODE==32 || type==25)
				{
					break;
				}
				if(KEYCODE==99 || KEYCODE==67)//C键
				{
					if(RTNORM == acedGetPoint(NULL,L"请重新点取圆弧中点:\n",asDblArray(PT)) )
					{
						BcArcObj.SetMidPoint(PT.y,PT.x);
					}
				}
				else if(KEYCODE==114 || KEYCODE==82)//R键
				{
					tmpR=-1.0;
					_stprintf(mes,L"请输入新的半径(%.2lf):",R);				
					rc = acedGetReal(mes,&tmpR);
					if(rc==RTCAN)
						return;
					if(tmpR>0.0)R=tmpR;
					BcArcObj.SetRadius(R);
				}
				else if(KEYCODE==13 || KEYCODE==32)//回车、空格
				{
					break;
				}
				else{}
			}
			if(type == 3)break;
			else if(type==25)//右键
			{
				break;
			}
			else if(type == 5)
			{
				pk.x=result.resval.rpoint[X];
				pk.y=result.resval.rpoint[Y];
				pk.z=result.resval.rpoint[Z];

				BcArcObj.SetOutPoint(pk.y,pk.x);
				BcArcObj.UpdateData();
			}
		}			
		/*
		BCCreateXLEntity OB;
		OB.SetARC(BcArcObj.CenN,BcArcObj.CenE,BcArcObj.R,BcArcObj.startAngle,BcArcObj.endAngle);
		OB.UpdateData();
		BcArcObj.EraseArc();
		*/
	}
	///////////////////////////////////////////////////////////////////
	ShowWindow(TRUE);
	//	acedGetAcadDockCmdLine()->SetFocus();//plh07.18
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnButtonTtr() //plh07.17
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	//	char cmdstr[80];
	//	_tcscpy(cmdstr, "(command \"esc\")\n");
	//	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//
	// TODO: Add your control notification handler code here
	//ShowWindow(FALSE);
	////////////////////////////////////////////////////////////////////
	BCTTP BcTtpObj;
	AcGePoint3d PT1,PT0,PT2;//三个交点	

	ads_name en1,en2;
	AcDbObjectId lId1,lId2;
	AcDbLine *pL1,*pL2;
	double tmpR=0.0;
	bool IsOk=true;

	if(RTNORM==ads_entsel(L"\n请选择第一直线边", en1, asDblArray(PT1)) 
		&& RTNORM==ads_entsel(L"\n请选择第二直线边", en2, asDblArray(PT2)))
	{
		acdbGetObjectId(lId1, en1);
		acdbGetObjectId(lId2, en2);

		AcDbObject *pObj1,*pObj2;
		/*acdbOpenObject(pObj1, lId1, AcDb::kForRead);*/
		if(acdbOpenObject(pObj1, lId1, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj1->close();
		/*acdbOpenObject(pObj2, lId2, AcDb::kForRead);*/
		if(acdbOpenObject(pObj2, lId2, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj2->close();

		if(( pObj1->isKindOf(AcDbLine::desc())
			|| pObj1->isKindOf(AcDbPolyline::desc()) 
			|| pObj1->isKindOf(AcDb2dPolyline::desc())
			|| pObj1->isKindOf(AcDb3dPolyline::desc()) 
			|| pObj1->isKindOf(JD_CENTER::desc()))
			&&(pObj2->isKindOf(AcDbLine::desc())
			|| pObj2->isKindOf(AcDbPolyline::desc())
			|| pObj2->isKindOf(AcDb2dPolyline::desc())
			|| pObj2->isKindOf(AcDb3dPolyline::desc())
			|| pObj2->isKindOf(JD_CENTER::desc()))
			/*&& lId1!=lId2 */)
		{
			AcGePoint3d P1,P2,P3,P4;

			if(pObj1->isKindOf(AcDbLine::desc()))
			{
				pL1=AcDbLine::cast(pObj1);
				P1=pL1->startPoint();
				P2=pL1->endPoint();
			}
			else if(pObj1->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline *poly=AcDbPolyline::cast(pObj1);
				PolyLineCal ob;
				ob.SetNearPoint(PT1);
				ob.SetPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(P1,P2);
			}
			else if(pObj1->isKindOf(AcDb2dPolyline::desc()))
			{
				AcDb2dPolyline *poly=AcDb2dPolyline::cast(pObj1);
				PolyLineCal ob;
				ob.SetNearPoint(PT1);
				ob.Set2dPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(P1,P2);
			}
			else if(pObj1->isKindOf(AcDb3dPolyline::desc()))
			{
				AcDb3dPolyline *poly=AcDb3dPolyline::cast(pObj1);
				PolyLineCal ob;
				ob.SetNearPoint(PT1);
				ob.Set3dPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(P1,P2);
			}
			else if(pObj1->isKindOf(JD_CENTER::desc()))
			{
				JD_CENTER *pGtzx=JD_CENTER::cast(pObj1);
				pGtzx->GetBian(PT1.y,PT1.x,P1.y,P1.x,P2.y,P2.x);
			}

			if(pObj2->isKindOf(AcDbLine::desc()))
			{
				pL2=AcDbLine::cast(pObj2);
				P3=pL2->startPoint();
				P4=pL2->endPoint();
			}
			else if(pObj2->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline *poly=AcDbPolyline::cast(pObj2);
				PolyLineCal ob;
				ob.SetNearPoint(PT2);
				ob.SetPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(P3,P4);
			}
			else if(pObj2->isKindOf(AcDb2dPolyline::desc()))
			{
				AcDb2dPolyline *poly=AcDb2dPolyline::cast(pObj2);
				PolyLineCal ob;
				ob.SetNearPoint(PT2);
				ob.Set2dPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(P3,P4);
			}
			else if(pObj2->isKindOf(AcDb3dPolyline::desc()))
			{
				AcDb3dPolyline *poly=AcDb3dPolyline::cast(pObj2);
				PolyLineCal ob;
				ob.SetNearPoint(PT2);
				ob.Set3dPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(P3,P4);
			}
			else if(pObj2->isKindOf(JD_CENTER::desc()))
			{
				JD_CENTER *pGtzx=JD_CENTER::cast(pObj2);
				pGtzx->GetBian(PT2.y,PT2.x,P3.y,P3.x,P4.y,P4.x);
			}

			BAS_DRAW_FUN BasOb;
			if(BasOb.INTERS(&PT0.x,&PT0.y,P1.x,P1.y,P2.x,P2.y,P3.x,P3.y,P4.x,P4.y)>=0)
			{
				BasOb.GetPerPoint(PT1.x,PT1.y,P1.x,P1.y,P2.x,P2.y,PT1.x,PT1.y);
				BasOb.GetPerPoint(PT2.x,PT2.y,P3.x,P3.y,P4.x,P4.y,PT2.x,PT2.y);
				////////////////////////////////////////////////////////////////////
				AcGePoint3d pk;
				int type;
				struct resbuf result;

				for(;;)
				{
					if(ads_grread(1 , &type , &result)!=RTNORM)
					{ 
						IsOk=false;
						break;
					}
					if(type == 3)break;
					short KEYCODE=result.resval.rint;
					if(KEYCODE==13 || KEYCODE==32 || type==25)
					{
						break;
					}
					if(type == 5)
					{
						pk.x=result.resval.rpoint[X];
						pk.y=result.resval.rpoint[Y];
						pk.z=result.resval.rpoint[Z];

						BcTtpObj.SetThreeJD(PT1.y,PT1.x,PT0.y,PT0.x,PT2.y,PT2.x);
						BcTtpObj.SetPointOnArc(pk.y,pk.x);
						BcTtpObj.UpdateData();

						//plh07.18					
						if(fabs(BcTtpObj.R-tmpR)>0.001)
						{
							ACHAR pszLabel[256];
							_stprintf(pszLabel,L"当前半径为: %.2lf",BcTtpObj.R);
							acedGrText(-1,pszLabel,1);    
							tmpR=BcTtpObj.R;
							m_Radius = BcTtpObj.R; //按1:1:1设置缓和
							if(m_AorL==1)//A
							{
								//								double a = sqrt(0.5*m_Radius*BcTtpObj.Len);
								//					    		char astr[30];
								//								acdbRToS(a,2,0,astr);
								//								m_A1 = _wtof(astr);
								m_A1 = 0;

							}
							else
							{
								//								double a = 0.5*BcTtpObj.Len;
								//								char astr[30];
								//								acdbRToS(a,2,0,astr);
								//						    	m_A1 = _wtof(astr);
								m_A1 = 0;
							}

							m_A2 = m_A1;
							UpdateData(false);
						}					
					}
				}
				if(IsOk)
				{
					BCCreateXLEntity OB;
					UpdateData();
					double a1,a2;
					if(m_AorL==1)//A
					{
						a1 = m_A1;
						a2 = m_A2;
					}
					else
					{
						a1 = sqrt(m_A1 * BcTtpObj.R);
						a2 = sqrt(m_A2 * BcTtpObj.R);
					}

					OB.SetARC_A(BcTtpObj.CenN,BcTtpObj.CenE,BcTtpObj.R,BcTtpObj.startAngle,BcTtpObj.endAngle,a1,a2);
					OB.UpdateData();
					xlId = OB.eId;
					BcTtpObj.EraseArc();
				}
				else
					xlId = NULL;
			}
		}
		else
		{
			acutPrintf(L"所选不为直线或设计线!\n");
		}
	}
	///////////////////////////////////////////////////////////////////
	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnButtonTpr() //plh07.17
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	//	ShowWindow(FALSE);
	////////////////////////////////////////////////////////////////////
	BCTPR BcTprObj;
	AcGePoint3d PT,PT1,PT2;
	AcDbObjectId lId;
	ads_name en;
	ACHAR mes[80];
	double R;
	UpdateData(true);
	R=m_Radius;
	_stprintf(mes,L"\n请输入半径(%.2lf):",R);
	int rc = acedGetReal(mes,&R);
	bool IsOk=true;
	//	if( RTNORM == acedGetPoint(NULL,L"请点取切线起点:\n",asDblArray(PT1))
	//		&& RTNORM == acedGetPoint(asDblArray(PT1),L"请点取切线终点:\n",asDblArray(PT2)) )
	if(rc!=RTCAN &&RTNORM==ads_entsel(L"\n请选择切线\n", en, asDblArray(PT)) )
	{

		m_Radius = R; //按1:1:1设置缓和
		UpdateData(false);

		acdbGetObjectId(lId, en);

		AcDbObject *pObj;
		/*acdbOpenObject(pObj, lId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj, lId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
		if(pObj->isKindOf(AcDbLine::desc())
			|| pObj->isKindOf(AcDbPolyline::desc()) 
			|| pObj->isKindOf(AcDb2dPolyline::desc())
			|| pObj->isKindOf(AcDb3dPolyline::desc()) 
			|| pObj->isKindOf(JD_CENTER::desc()))
		{

			if(pObj->isKindOf(AcDbLine::desc()))
			{
				AcDbLine *pL1=AcDbLine::cast(pObj);
				PT1=pL1->startPoint();
				PT2=pL1->endPoint();
			}
			else if(pObj->isKindOf(AcDbPolyline::desc()))
			{
				AcDbPolyline *poly=AcDbPolyline::cast(pObj);
				PolyLineCal ob;
				ob.SetNearPoint(PT);
				ob.SetPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(PT1,PT2);
			}
			else if(pObj->isKindOf(AcDb2dPolyline::desc()))
			{
				AcDb2dPolyline *poly=AcDb2dPolyline::cast(pObj);
				PolyLineCal ob;
				ob.SetNearPoint(PT);
				ob.Set2dPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(PT1,PT2);
			}
			else if(pObj->isKindOf(AcDb3dPolyline::desc()))
			{
				AcDb3dPolyline *poly=AcDb3dPolyline::cast(pObj);
				PolyLineCal ob;
				ob.SetNearPoint(PT);
				ob.Set3dPoly(poly);
				ob.UpdateData();
				ob.GetBianTwoPoint(PT1,PT2);
			}
			else if(pObj->isKindOf(JD_CENTER::desc()))
			{
				JD_CENTER *pGtzx=JD_CENTER::cast(pObj);
				pGtzx->GetBian(PT.y,PT.x,PT1.y,PT1.x,PT2.y,PT2.x);
			}

			////////////////////////////////////////////////////////////////////
			AcGePoint3d pk;
			int type;
			struct resbuf result;
			BcTprObj.SetRadius(R);
			BcTprObj.SetQX(PT1.y,PT1.x,PT2.y,PT2.x);
			acutPrintf(L"点击 D 键切换方向!\n");

			for(;;)
			{
				if(ads_grread(1 , &type , &result)!=RTNORM)
				{ 
					IsOk=false;
					break;
				}
				short KEYCODE=result.resval.rint;
				if(KEYCODE==13 || KEYCODE==32 || type==25)
				{
					break;
				}
				if(KEYCODE==68 ||KEYCODE==100)
				{
					PT=PT1;
					PT1=PT2;
					PT2=PT;
					BcTprObj.SetQX(PT1.y,PT1.x,PT2.y,PT2.x);
				}
				if(type == 3)break;
				/*
				else if(type==25)
				{
				PT=PT1;
				PT1=PT2;
				PT2=PT;
				BcTprObj.SetQX(PT1.y,PT1.x,PT2.y,PT2.x);
				}
				*/
				else if(type == 5)
				{
					pk.x=result.resval.rpoint[X];
					pk.y=result.resval.rpoint[Y];
					pk.z=result.resval.rpoint[Z];

					BcTprObj.SetPointOnArc(pk.y,pk.x);
					BcTprObj.UpdateData();
					//					if(m_AorL==1)//A
					//					{
					//						double a = sqrt(0.5*m_Radius*BcTprObj.Len);
					//						char astr[30];
					//						acdbRToS(a,2,0,astr);
					//						m_A1 = _wtof(astr);
					//					}
					//					else
					//					{
					//						double a = 0.5*BcTprObj.Len;
					//						char astr[30];
					//						acdbRToS(a,2,0,astr);
					//						m_A1 = _wtof(astr);
					//					}

					m_A2 = m_A1 = 0;
					UpdateData(false);


				}
			}
			if(IsOk)
			{
				double a1,a2;
				if(m_AorL==1)//A
				{
					a1 = m_A1;
					a2 = m_A2;
				}
				else
				{
					a1 = sqrt(m_A1 * BcTprObj.R);
					a2 = sqrt(m_A2 * BcTprObj.R);
				}

				BCCreateXLEntity OB;
				OB.SetARC_A(BcTprObj.CenN,BcTprObj.CenE,BcTprObj.R,BcTprObj.startAngle,BcTprObj.endAngle,a1,a2);
				OB.UpdateData();
				xlId = OB.eId;
				BcTprObj.EraseArc();
			}
			else
				xlId = NULL;
		}
		else
		{
			acutPrintf(L"所选不为直线或设计线!\n");
		}
	}
	///////////////////////////////////////////////////////////////////
	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnButtonTpp() //plh07.17
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	// TODO: Add your control notification handler code here
	//	ShowWindow(FALSE);
	////////////////////////////////////////////////////////////////////
	BCTPP BcTppObj;
	AcGePoint3d PT1,PT2,PT;//切线端点
	AcGePoint3d PT0;//圆弧上一点
	double tmpR=0.0;
	ads_name en;
	AcDbObjectId lId;
	AcDbLine *pL;
	bool IsOk=true;
	if(RTNORM==ads_entsel(L"\n请选择切线", en, asDblArray(PT)))
	{
		acdbGetObjectId(lId, en);

		AcDbObject *pObj;
		/*acdbOpenObject(pObj, lId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj, lId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pObj->close();
		if( pObj->isKindOf(AcDbLine::desc()))
		{
			pL=AcDbLine::cast(pObj);
			PT1=pL->startPoint();
			PT2=pL->endPoint();
		}
		else if(pObj->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *poly=AcDbPolyline::cast(pObj);
			PolyLineCal ob;
			ob.SetNearPoint(PT);
			ob.SetPoly(poly);
			ob.UpdateData();
			ob.GetBianTwoPoint(PT1,PT2);
		}
		else if(pObj->isKindOf(AcDb2dPolyline::desc()))
		{
			AcDb2dPolyline *poly=AcDb2dPolyline::cast(pObj);
			PolyLineCal ob;
			ob.SetNearPoint(PT);
			ob.Set2dPoly(poly);
			ob.UpdateData();
			ob.GetBianTwoPoint(PT1,PT2);
		}
		else if(pObj->isKindOf(AcDb3dPolyline::desc()))
		{
			AcDb3dPolyline *poly=AcDb3dPolyline::cast(pObj);
			PolyLineCal ob;
			ob.SetNearPoint(PT);
			ob.Set3dPoly(poly);
			ob.UpdateData();
			ob.GetBianTwoPoint(PT1,PT2);
		}
		else if(pObj->isKindOf(JD_CENTER::desc()))
		{
			JD_CENTER *pGtzx=JD_CENTER::cast(pObj);
			pGtzx->GetBian(PT.y,PT.x,PT1.y,PT1.x,PT2.y,PT2.x);
		}
		else
		{
			acutPrintf(L"所选不为直线或设计线!\n");
			ShowWindow(TRUE);	
			acDocManager->unlockDocument(acDocManager->curDocument());
			SetFocus();
			return;
		}
		if( RTNORM == acedGetPoint(NULL,L"\n请点取圆弧上一点:",asDblArray(PT0)) )
		{
			double distToPT2,TN,TE;
			TN=PT0.y;
			TE=PT0.x;
			BAS_DRAW_FUN BAS;
			if(BAS.DistToLine(PT1.y,PT1.x,PT2.y,PT2.x,TN,TE,distToPT2)>0.00001)
			{				
				////////////////////////////////////////////////////////////////////
				AcGePoint3d pk;
				int type;
				struct resbuf result;

				for(;;)
				{
					if(ads_grread(1 , &type , &result)!=RTNORM)
					{ 
						IsOk=false;
						break;
					}
					short KEYCODE=result.resval.rint;
					if(KEYCODE==13 || KEYCODE==32 || type==25)
					{
						break;
					}
					if(type == 3)break;
					else if(type == 5)
					{
						pk.x=result.resval.rpoint[X];
						pk.y=result.resval.rpoint[Y];
						pk.z=result.resval.rpoint[Z];
						if(pk.distanceTo(PT0) < 0.001)//ph90
							continue;
						BcTppObj.SetQX(PT1.y,PT1.x,PT2.y,PT2.x);
						BcTppObj.SetFirstPointOnArc(PT0.y,PT0.x);
						BcTppObj.SetSecondPointOnArc(pk.y,pk.x);
						BcTppObj.UpdateData();
						//plh07.18					
						if(fabs(BcTppObj.R-tmpR)>0.001)
						{
							ACHAR pszLabel[256];
							_stprintf(pszLabel,L"当前半径为: %.2lf",BcTppObj.R);
							acedGrText(-1,pszLabel,1);    
							tmpR=BcTppObj.R;

							m_Radius = BcTppObj.R; //按1:1:1设置缓和
							//							if(m_AorL==1)//A
							//							{
							//								double a = sqrt(0.5*m_Radius*BcTppObj.Len);
							//								char astr[30];
							//								acdbRToS(a,2,0,astr);
							//								m_A1 = _wtof(astr);
							//							}
							//							else
							//							{
							//								double a = 0.5*BcTppObj.Len;
							//								char astr[30];
							//								acdbRToS(a,2,0,astr);
							//								m_A1 = _wtof(astr);
							//							}

							m_A2 = m_A1 = 0;
							UpdateData(false);							
						}
					}
				}
				if(IsOk)
				{

					double a1,a2;
					if(m_AorL==1)//A
					{
						a1 = m_A1;
						a2 = m_A2;
					}
					else
					{
						a1 = sqrt(m_A1 * BcTppObj.R);
						a2 = sqrt(m_A2 * BcTppObj.R);
					}
					BCCreateXLEntity OB;

					OB.SetARC_A(BcTppObj.CenN,BcTppObj.CenE,BcTppObj.R,BcTppObj.startAngle,BcTppObj.endAngle,a1,a2);
					OB.UpdateData();
					xlId = OB.eId;
					BcTppObj.EraseArc();
				}
				else
					xlId = NULL;
			}
			else
			{
				acutPrintf(L"第一个点落在线上!\n");
			}			
		}
	}
	///////////////////////////////////////////////////////////////////
	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnButtonPpp() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//	ShowWindow(FALSE);
	////////////////////////////////////////////////////////////////////
	BCPPP BcPppObj;
	AcGePoint3d PT;//
	double tmpR=0.0;
	int type;
	struct resbuf result;
	bool IsOk=true;
	ads_printf(L"\n请拾取圆弧上的点,按右键结束...");
	for(;;)
	{
		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			IsOk=false;
			break;
		}
		short KEYCODE=result.resval.rint;
		if(KEYCODE==13 || KEYCODE==32 || type==25)
		{
			break;
		}
		if(type == 3)break;
		else if(type == 5)
		{
			if(RTNORM==acedGetPoint(NULL,L"\n下一点:",asDblArray(PT)))
			{
				BcPppObj.AppendPointNearArc(PT.y,PT.x);
				BcPppObj.UpdateData();
				//plh07.18					
				if(fabs(BcPppObj.R-tmpR)>0.001)
				{
					ACHAR pszLabel[256];

					_stprintf(pszLabel,L"当前半径为: %.2lf",BcPppObj.R);
					acedGrText(-1,pszLabel,1);    
					tmpR=BcPppObj.R;

					m_Radius = BcPppObj.R; //按1:1:1设置缓和
					//					if(m_AorL==1)//A
					//					{
					//						double a = sqrt(0.5*m_Radius*BcPppObj.Len);
					//						char astr[30];
					//						acdbRToS(a,2,0,astr);
					//						m_A1 = _wtof(astr);
					//					}
					//					else
					//					{
					//						double a =0.5* BcPppObj.Len;
					//						char astr[30];
					//						acdbRToS(a,2,0,astr);
					//						m_A1 = _wtof(astr);
					//					}

					m_A2 = m_A1 = 0;
					UpdateData(false);

				}			
			}
			else 
			{
				IsOk=true;
				break;
			}
		}
	}
	if(IsOk && BcPppObj.PtArray.length()>=3)
	{
		double a1,a2;
		if(m_AorL==1)//A
		{
			a1 = m_A1;
			a2 = m_A2;
		}
		else
		{
			a1 = sqrt(m_A1 * BcPppObj.R);
			a2 = sqrt(m_A2 * BcPppObj.R);
		}
		BCCreateXLEntity OB;
		m_Radius = BcPppObj.R+(a1*a1)/pow((BcPppObj.R),3)/24.0;
		OB.SetARC_A(BcPppObj.CenN,BcPppObj.CenE,m_Radius,BcPppObj.startAngle,BcPppObj.endAngle,a1,a2);
		OB.UpdateData();
		xlId = OB.eId;
	}
	BcPppObj.EraseArc();
	///////////////////////////////////////////////////////////////////
	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	acedGrText(-5,L"完成",0);    
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnButtonInvert() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	
	//	ShowWindow(FALSE);

	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"JCREVERSE\n",0);	
	///////////////////////////////////////////////////////////////////
	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnButtonLink() //plh07.18
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//	ShowWindow(FALSE);
	////////////////////////////////////////////////////////////////////
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"XLLINK\n",0);	
	////////////////////////////////////////////////////////////////////
	/*
	int rc;
	ads_name en1,en2;
	AcGePoint3d PT1,PT2;
	AcDbObjectId lId1,lId2;
	GTZX *pGtzx1,*pGtzx2;
	if(RTNORM==ads_entsel(NULL, en1, asDblArray(PT1)) 
	&& RTNORM==ads_entsel(NULL, en2, asDblArray(PT2)))
	{
	acdbGetObjectId(lId1, en1);
	acdbGetObjectId(lId2, en2);

	AcDbObject *pObj1,*pObj2;
	acdbOpenObject(pObj1, lId1, AcDb::kForRead);
	pObj1->close(); 
	acdbOpenObject(pObj2, lId2, AcDb::kForRead);
	pObj2->close();
	if( pObj1->isKindOf(GTZX::desc())
	&& pObj2->isKindOf(GTZX::desc()) 
	&& lId1!=lId2 )
	{
	//////////////////////////////////////////////////////////////////////////			
	acdbOpenObject(pGtzx1, lId1, AcDb::kForRead);
	pGtzx1->close();
	acdbOpenObject(pGtzx2, lId2, AcDb::kForRead);
	pGtzx2->close();

	//plh07.17
	/ *
	//求出第一条线路的起终点坐标
	double QN1,QE1,ZN1,ZE1;
	QN1=pGtzx1->array[0][1];
	QE1=pGtzx1->array[0][2];
	ZN1=pGtzx1->array[pGtzx1->tarray][8];
	ZE1=pGtzx1->array[pGtzx1->tarray][9];
	//求出第二条线路的起终点坐标
	double QN2,QE2,ZN2,ZE2;
	QN2=pGtzx2->array[0][1];
	QE2=pGtzx2->array[0][2];
	ZN2=pGtzx2->array[pGtzx2->tarray][8];
	ZE2=pGtzx2->array[pGtzx2->tarray][9];
	//起终点到一线终点的距离
	double DistQ1Q2,DistZ1Z2,DistQ2Z1,DistQ1Z2,f;
	DistQ1Q2=BAS_DRAW_FUN::xyddaa(QN1,QE1,QN2,QE2,&f);
	DistZ1Z2=BAS_DRAW_FUN::xyddaa(ZN1,ZE1,ZN2,ZE2,&f);
	DistQ1Z2=BAS_DRAW_FUN::xyddaa(QN1,QE1,ZN2,ZE2,&f);
	DistQ2Z1=BAS_DRAW_FUN::xyddaa(QN2,QE2,ZN1,ZE1,&f);
	double MinDist=min(DistQ1Q2,min(DistZ1Z2,min(DistQ1Z2,DistQ2Z1)));
	if(fabs(DistQ1Q2-MinDist)<0.001)pGtzx1->Invert();
	if(fabs(DistZ1Z2-MinDist)<0.001)pGtzx2->Invert();
	if(fabs(DistQ1Z2-MinDist)<0.001)
	{
	pGtzx1->Invert();
	pGtzx2->Invert();
	}* /


	//确定选择的线元类型及交点号
	int iJD1,iJD2;
	double tylc1,tylc2;//选择点的投影里程
	int XYClass1,XYClass2;//所选线元的类型
	int iXY1,iXY2;//所选线元序号
	tylc1=pGtzx1->PROJ_ML(pGtzx1->array,pGtzx1->tarray,PT1.y,PT1.x,PT1.y,PT1.x);
	tylc2=pGtzx2->PROJ_ML(pGtzx2->array,pGtzx2->tarray,PT2.y,PT2.x,PT2.y,PT2.x);
	//由里程求所选线元的序号
	for(iXY1=pGtzx1->tarray;iXY1>0;iXY1--)if(tylc1>=pGtzx1->array[iXY1][6])break;
	for(iXY2=pGtzx2->tarray;iXY2>0;iXY2--)if(tylc2>=pGtzx2->array[iXY2][6])break;
	//求出线元类型
	XYClass1=(int)(pGtzx1->array[iXY1][0]+0.001);
	XYClass2=(int)(pGtzx2->array[iXY2][0]+0.001);
	//求对应的交点
	if(XYClass1==1)//所选为直线
	{
	iJD1=pGtzx1->DistToNearJD(tylc1,true);
	}
	else//所选不为直线
	{
	for(iJD1=0;iJD1<pGtzx1->NJD;iJD1++)
	{
	if(tylc1>=pGtzx1->QXB[iJD1][0]
	&& tylc1<=pGtzx1->QXB[iJD1][3])break;
	}
	}

	if(XYClass2==1)//所选为直线
	{
	iJD2=pGtzx2->DistToNearJD(tylc2,false);
	}
	else//所选不为直线
	{
	for(iJD2=0;iJD2<pGtzx2->NJD;iJD2++)
	{
	if(tylc2>=pGtzx2->QXB[iJD2][0]
	&& tylc2<=pGtzx2->QXB[iJD2][3])break;
	}
	}

	if(XYClass1==1 && XYClass2==1)//两个实体所选都为直线边,直线直线相连
	{
	AcGePoint3d Pt1(pGtzx1->JDarray[iJD1][2],pGtzx1->JDarray[iJD1][1],0.0);
	AcGePoint3d Pt2(pGtzx1->JDarray[iJD1+1][2],pGtzx1->JDarray[iJD1+1][1],0.0);
	AcGePoint3d Pt3(pGtzx2->JDarray[iJD2-1][2],pGtzx2->JDarray[iJD2-1][1],0.0);
	AcGePoint3d Pt4(pGtzx2->JDarray[iJD2][2],pGtzx2->JDarray[iJD2][1],0.0);
	double T2=pGtzx1->QXB[iJD1][6];//后切长
	double fwj1=pGtzx1->fwj(Pt1.x,Pt1.y,Pt2.x,Pt2.y);
	pGtzx1->getLineEndPt(Pt1,T2,fwj1,Pt1);

	double T1=pGtzx2->QXB[iJD2][5];//前切长////plh04.26
	double fwj2=pGtzx2->fwj(Pt4.x,Pt4.y,Pt3.x,Pt3.y);
	pGtzx2->getLineEndPt(Pt4,T1,fwj2,Pt4);//plh06.01

	AcGePoint3d interPt;

	if( BAS_DRAW_FUN::INTERS(&interPt.x,&interPt.y,Pt1.x,Pt1.y,Pt2.x,Pt2.y,Pt3.x,Pt3.y,Pt4.x,Pt4.y)>=0 )
	{
	double rad;
	if(iJD2<pGtzx2->NJD-1)rad=pGtzx1->GetRFromT1T2(Pt1.x,Pt1.y,interPt.x,interPt.y,Pt4.x,Pt4.y,true,pGtzx1->DV,0,12);
	else rad=pGtzx1->GetRFromT1T2(Pt1.x,Pt1.y,interPt.x,interPt.y,Pt4.x,Pt4.y,true,pGtzx1->DV,0,1);
	if(rad>0)
	{
	//////////////////////////////////////////////////////////////////////////			
	acdbOpenObject(pGtzx1, lId1, AcDb::kForWrite);
	pGtzx1->assertWriteEnabled();

	pGtzx1->NJD=(iJD1+1)+(pGtzx2->NJD-iJD2)+1;
	int i=iJD1+1;
	pGtzx1->JDarray[i][0]=i;
	pGtzx1->JDarray[i][1]=interPt.y;
	pGtzx1->JDarray[i][2]=interPt.x;
	pGtzx1->JDarray[i][3]=pGtzx1->get_l0(rad,pGtzx1->DV);
	pGtzx1->JDarray[i][4]=rad;
	pGtzx1->JDarray[i][5]=pGtzx1->JDarray[i][3];
	for(i=iJD1+2;i<pGtzx1->NJD;i++)
	{
	pGtzx1->JDarray[i][0]=i;
	for(int k=1;k<6;k++)pGtzx1->JDarray[i][k]
	=pGtzx2->JDarray[iJD2+(i-(iJD1+2))][k];
	}
	pGtzx1->close();
	}
	else//plh07.18
	{
	//////////////////////////////////////////////////////////////////////////////////
	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();

	return;
	}
	}			
	}
	else//有一个不为直线边，圆圆连接
	{
	int QXClass;//切线类型
	//判断前后两圆的转向				
	if(pGtzx1->QXB[iJD1][8]<0.0)
	{
	if(pGtzx2->QXB[iJD2][8]<0.0)QXClass=C_F;
	else QXClass=S_F;
	}
	else
	{				
	if(pGtzx2->QXB[iJD2][8]<0.0)QXClass=S_S;
	else QXClass=C_S;
	}
	//圆圆通过直线连接
	double Lo1,R1,A1,Lo2,R2,A2;
	R1=pGtzx1->JDarray[iJD1][4];
	Lo1=pGtzx1->JDarray[iJD1][5];
	R2=pGtzx2->JDarray[iJD2][4];
	Lo2=pGtzx2->JDarray[iJD2][3];
	A1=sqrt(R1*Lo1);
	A2=sqrt(R2*Lo2);
	double CenN1,CenE1,CenN2,CenE2;//圆心坐标
	if(XYClass1==3)iXY1++;
	if(XYClass1==4)iXY1--;
	if(XYClass1==1)iXY1-=2;

	if(XYClass2==1)iXY2+=2;				
	if(XYClass2==3)iXY2++;
	if(XYClass2==4)iXY2--;
	//plh07.18
	if(iXY1<=1)iXY1=1;
	if(iXY2<=1)iXY2=1;
	if(pGtzx1->NJD==2)
	{
	CenN1=pGtzx1->array[1][8];
	CenE1=pGtzx1->array[1][9];
	R1=0.0;
	}
	else
	{
	CenN1=pGtzx1->array[iXY1][8]+R1*cos(pGtzx1->array[iXY1][5]+0.5*PI*pGtzx1->array[iXY1][4]);
	CenE1=pGtzx1->array[iXY1][9]+R1*sin(pGtzx1->array[iXY1][5]+0.5*PI*pGtzx1->array[iXY1][4]);
	}
	if(pGtzx2->NJD==2)
	{
	CenN2=pGtzx2->array[2][8];
	CenE2=pGtzx2->array[2][9];
	R2=0.0;
	}
	else
	{
	CenN2=pGtzx2->array[iXY2][8]+R2*cos(pGtzx2->array[iXY2][5]+0.5*PI*pGtzx2->array[iXY2][4]);
	CenE2=pGtzx2->array[iXY2][9]+R2*sin(pGtzx2->array[iXY2][5]+0.5*PI*pGtzx2->array[iXY2][4]);
	}
	YYQXLink YYQXObj;
	YYQXObj.SetCir1(CenN1,CenE1,R1);
	YYQXObj.SetCir2(CenN2,CenE2,R2);
	YYQXObj.SetA(A1,A2);
	YYQXObj.SetQXClass(QXClass);
	YYQXObj.CalQX();

	//求两个交点
	BAS_DRAW_FUN::INTERS( &pGtzx1->JDarray[iJD1][1],&pGtzx1->JDarray[iJD1][2],
	pGtzx1->JDarray[iJD1-1][1],pGtzx1->JDarray[iJD1-1][2],
	pGtzx1->JDarray[iJD1][1],pGtzx1->JDarray[iJD1][2],
	YYQXObj.GQN1,YYQXObj.GQE1,YYQXObj.GQN2,YYQXObj.GQE2 );

	BAS_DRAW_FUN::INTERS( &pGtzx2->JDarray[iJD2][1],&pGtzx2->JDarray[iJD2][2],
	YYQXObj.GQN1,YYQXObj.GQE1,YYQXObj.GQN2,YYQXObj.GQE2,
	pGtzx2->JDarray[iJD2][1],pGtzx2->JDarray[iJD2][2],
	pGtzx2->JDarray[iJD2+1][1],pGtzx2->JDarray[iJD2+1][2] );

	//////////////////////////////////////////////////////////////////////////			
	acdbOpenObject(pGtzx1, lId1, AcDb::kForWrite);
	pGtzx1->assertWriteEnabled();

	pGtzx1->NJD=(iJD1+1)+(pGtzx2->NJD-iJD2);
	for(int i=iJD1+1;i<pGtzx1->NJD;i++)
	{
	pGtzx1->JDarray[i][0]=i;
	for(int k=1;k<6;k++)pGtzx1->JDarray[i][k]
	=pGtzx2->JDarray[iJD2+(i-(iJD1+1))][k];
	}
	pGtzx1->close();
	}
	//////////////////////////////////////////////////////////////////////////
	acdbOpenObject(pGtzx2, lId2, AcDb::kForWrite);
	pGtzx2->assertWriteEnabled();
	pGtzx2->erase();
	pGtzx2->close();			
	//////////////////////////////////////////////////////////////////////////			
	}
	else
	{
	if(lId1==lId2)ads_printf(L"\n所选为同一条设计线!\n");
	else ads_printf(L"\n所选实体不是设计线!\n");
	}
	}*/

	//////////////////////////////////////////////////////////////////////////////////
	//	ShowWindow(TRUE);	
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	acutPrintf(L"\n命令:");
}

void LocationDlg::OnRadioA() 
{
	// TODO: Add your control notification handler code here
	m_AorL = 1;//A
}

void LocationDlg::OnRADIOLo() 
{
	// TODO: Add your control notification handler code here
	m_AorL = 2;//A
}

void LocationDlg::OnBUTTONrefresh() 
{
	// TODO: Add your control notification handler code here

	UpdateData();
	double a1,a2,r;
	r = m_Radius;
	if(m_AorL==1)//A
	{
		a1 = m_A1;
		a2 = m_A2;
	}
	else
	{
		a1 = sqrt(m_A1 * r);
		a2 = sqrt(m_A2 * r);
	}

	if(xlId)
	{

		JD_CENTER *pJC;
		AcDbObject *pObj=NULL;
		acedGetAcadDwgView()->SetFocus();
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		if(acdbOpenObject(pObj, xlId, AcDb::kForWrite)==Acad::eOk)
		{
			if(pObj->isKindOf(JD_CENTER::desc()))
			{
				pJC = JD_CENTER::cast(pObj);
				if(pJC->JdCenNum>2)
				{
					pJC->assertWriteEnabled();
					pJC->JdCenArray[pJC->JdCenNum-2].A1 = a1; 
					pJC->JdCenArray[pJC->JdCenNum-2].A2 = a2;
					pJC->JdCenArray[pJC->JdCenNum-2].R = r;
				}
			}
			pObj->close();
		}
		acDocManager->unlockDocument(acDocManager->curDocument());
		SetFocus();

	}
}
