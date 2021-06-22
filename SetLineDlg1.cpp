// SetLineDlg1.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "SetLineDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLineDlg dialog


CSetLineDlg::CSetLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLineDlg)
	m_pianjiao = 0.0;
	m_pianjv = 0.0;
	m_slcd = 0.0;
	m_slex = 0.0;
	m_sley = 0.0;
	m_slfwj = 0.0;
	m_slsx = 0.0;
	m_slsy = 0.0;
	m_cixiang = 0;
	m_pianxiang = 0;
	m_FreeOrNot = FALSE;
	//}}AFX_DATA_INIT
	Vcexiang = -1;
	Vpianxiang = -1;
	CreatTOrModifyF = TRUE;
	ClickCout = 0;
	CanCreatRampItem = FALSE;
}

CSetLineDlg::CSetLineDlg(AcDbObjectId EnId,CWnd* pParent)
	: CDialog(CSetLineDlg::IDD, pParent)
{
	m_pianjiao = 0.0;
	m_pianjv = 0.0;
	m_slcd = 0.0;
	m_slex = 0.0;
	m_sley = 0.0;
	m_slfwj = 0.0;
	m_slsx = 0.0;
	m_slsy = 0.0;
	m_cixiang = 0;
	m_pianxiang = 0;
	//
	Vcexiang = -1;
	Vpianxiang = -1;
	JCId = EnId;
	InputEntId= EnId;
	m_FreeOrNot = FALSE;
	CreatTOrModifyF = TRUE;
	ClickCout = 0;
	pLine = NULL;
	LineId = NULL;
	CanCreatRampItem = FALSE;

	

}
void CSetLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLineDlg)
	DDX_Control(pDX, IDC_CHECKFREESET, m_CheckFreeset);
	DDX_Text(pDX, IDC_EDITPINAJIAO, m_pianjiao);
	DDX_Text(pDX, IDC_EDITPINAJV, m_pianjv);//ƫ��
	DDX_Text(pDX, IDC_EDITSLCD, m_slcd);
	DDX_Text(pDX, IDC_EDITSLEX, m_slex);
	DDX_Text(pDX, IDC_EDITSLEY, m_sley);
	DDX_Text(pDX, IDC_EDITSLFWJ, m_slfwj);
	DDX_Text(pDX, IDC_EDITSLSX, m_slsx);
	DDX_Text(pDX, IDC_EDITSLSY, m_slsy);
	DDX_Radio(pDX, IDC_RADIOZOUCE, m_cixiang);
	DDX_Radio(pDX, IDC_RADIOZOUPIAN, m_pianxiang);
	DDX_Check(pDX, IDC_CHECKFREESET, m_FreeOrNot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLineDlg, CDialog)
	//{{AFX_MSG_MAP(CSetLineDlg)
	ON_BN_CLICKED(IDSLOK, OnSlok)
	ON_BN_CLICKED(IDSLCANCEL, OnSlcancel)
	ON_BN_CLICKED(IDC_BUTTONSME, OnButtonsme)
	//	ON_BN_CLICKED(IDC_BUTTONSMS, OnButtonsms)
	ON_BN_CLICKED(IDC_RADIOYOUCE, OnRadioyouce)
	ON_BN_CLICKED(IDC_RADIOYOUPIAN, OnRadioyoupian)
	ON_BN_CLICKED(IDC_RADIOZOUCE, OnRadiozouce)
	ON_BN_CLICKED(IDC_RADIOZOUPIAN, OnRadiozoupian)
	//	ON_BN_CLICKED(IDC_BUTTONSLSD, OnButtonslsd)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTONUPDATE, OnButtonupdate)
	ON_BN_CLICKED(IDC_CHECKFREESET, OnCheckfreeset)
	//	ON_BN_CLICKED(IDC_BUTSETSDOT, OnButsetsdot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLineDlg message handlers

void CSetLineDlg::OnSlok() 
{

	OnButtonupdate();
	//�����ǰ���ܴ����ѵ���Ԫ;
	if (!CanCreatRampItem)
	{
		return ;
	}
	//����CAD�ĵ�;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	if (acdbOpenObject(pLine,LineId,AcDb::kForWrite) == Acad::eOk)
	{
		pLine->erase();
		pLine->close();
	}
	else 
		return ;

	//	if (SelItem == 1||SelItem == 2)
	//	{
	JD_CENTER *JdCenObj=new JD_CENTER;
	//		JdCenObj->assertWriteEnabled();
	JdCenObj->SetXYArray(RouteArray);
	m_XYNum = JdCenObj->XYNum;
	XYarray xray;
	xray.eID=0;
	int i,j;
	for(i=0;i<=m_XYNum;i++)
	{
		for(j=0;j<10;j++)
			xray.xyarr[j]=JdCenObj->XYArray[i][j];
		pXYArr.Add(xray);
	}

	for(i=1;i<=m_XYNum;i++)
	{
		if(pXYArr[i].xyarr[0]<1.1)
			DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,7);
		else if(pXYArr[i].xyarr[0]>2.1)
			DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,6);
		else
			DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,2);
	}

	int res=GetXdataFromEntity(InputEntId,xray);

	if(res)//InputEntId����չ����
		SetXYXdata(InputEntId, pXYArr[1].eID,xray.xyarr);	
	SetXYXdata(pXYArr[1].eID,0,pXYArr[1].xyarr);

	//		AddEntityToDbs(JdCenObj);
	/*
	}
	else 
	{
	JD_CENTER *pJC= NULL;
	acdbOpenObject(pJC,InputEntId,AcDb::kForWrite);
	pJC->assertWriteEnabled();
	if (SelItem == 31)
	{
	pJC->ReplaceItem(RouteArray);
	}
	else
	{
	pJC->AddXYArray(RouteArray);
	}		
	pJC->close();
	}*/


	acDocManager->unlockDocument(acDocManager->curDocument());
	CanCreatRampItem = FALSE;
	//SetFocus();
	CDialog::OnOK();	
}

void CSetLineDlg::OnSlcancel() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	if (acdbOpenObject(pLine,LineId,AcDb::kForWrite) == Acad::eOk)
	{
		pLine->erase();
		pLine->close();
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	CDialog::OnCancel();
}

void CSetLineDlg::OnButtonsme() //�϶��յ�
{
	struct resbuf resOSMODE;//����OSNAP
	resOSMODE.restype=RTSHORT;

	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	UpdateData(TRUE);
	if (CreatTOrModifyF) 
	{		
		//��ȡ���㵥Ԫ;
		//GetComputerItemOfRamps(ComItem,NULL,1);
		/*GetComItemFromSelEntity(InputEntId,ComItem);  */                      // :E,C,S;
		GetComItemFromSelEntity(InputEntId,InterItem,ComItem.ItemDef,FALSE);// :E(O);
		CreatTOrModifyF = FALSE;
	}
	SelItem = int(ComItem.ItemType);
	//�����ѡ��ԪΪ31,�򷵻�;
	/*if (SelItem == 31) 
	{	
	ads_printf(L"\n��ǰ����ֱ�ߵ�Ԫ������!\n");
	return ;
	}*/
	//�����ѡ��Ԫ��33,35�򷵻�;
	if (SelItem == 33||SelItem == 35) 
	{	
		ads_printf(L"\n��ǰ��������ֱ�ߵ�Ԫ!\n");
		return ;
	}
	//;
	double LineLen = 0,CirDef = 0;
	// ����ֱ�ߵ�������;
	double LineSX,LineSY,LineOA,LineR;	
	LineSX = InterItem.FDotX;
	LineSY = InterItem.FDotY;
	LineOA = InterItem.FDotOrient;
	//���ѡ����Ԫ��36��Ҫ��ȡ��Դ������;
	if (SelItem == 36) 
	{
		//�����ߵ�Դ��;
		GetOrignCorOfSprial(ComItem.FDotY,ComItem.FDotX,ComItem.ItemStartR,
			ComItem.FDotOrient,ComItem.ItemPro,ComItem.ItemDef,ComItem.RadForWard,
			LineSY,LineSX,LineOA);
	}
	if (m_FreeOrNot&&(SelItem == 2||SelItem == 32)) 
	{
		//��ʱΪԲ�����꼰�뾶R;
		LineSX = ComItem.FDotX;
		LineSY = ComItem.FDotY;
		LineR = ComItem.ItemPro;
		CirDef = ComItem.ItemDef;
	}
	//�����µ����(ƫ����ƫ��,ֱ�ߵĳ��������),�µ�ֱ�߷�λ��;
	double SPOffset,SPDeflect;
	if (m_cixiang == 0) SPOffset = -1;
	else SPOffset = 1;
	if (m_pianxiang == 0) SPDeflect = -1;
	else SPDeflect = 1;
	ModifyStartDot(LineSX,LineSY,SPOffset,m_pianjv,LineOA,SPDeflect,m_pianjiao);
	//�����϶�ѭ��;	
	AcGePoint3d StaDot,EndDot,PickDot;
	StaDot[X] = LineSX;
	StaDot[Y] = LineSY;
	StaDot[Z] = 0;
	EndDot[Z] = 0;

	int type;
	struct resbuf result;
	for( ; ; )
	{
		acedGetVar(L"autosnap",&resOSMODE);
		short OsnapValue=resOSMODE.resval.rint;

		if (ads_grread(1 , &type , &result) != RTNORM)
		{ 
			break;
		}
		short KEYCODE=result.resval.rint;
		if(KEYCODE==13 || KEYCODE==32 || type==25)
		{
			break;
		}
		if(type == 3)
		{
			break;
		}
		else if(type == 5)
		{
/*			if(OsnapValue>0)//��׽��ʽ��
			{
				ads_point PtTemp;

				acedOsnap(		
					result.resval.rpoint,L"endp,int",PtTemp);
				double dist = sqrt((PtTemp[X]- result.resval.rpoint[0])*(PtTemp[X]- result.resval.rpoint[0])+(PtTemp[1]- result.resval.rpoint[1])*(PtTemp[1]- result.resval.rpoint[1]));
				if(dist > 0.01)
				{
					result.resval.rpoint[0]=PtTemp[0];
					result.resval.rpoint[1]=PtTemp[1];
				}

			}*/

			PickDot.x=result.resval.rpoint[X];
			PickDot.y=result.resval.rpoint[Y];
			PickDot.z=result.resval.rpoint[Z];			
			if (m_FreeOrNot&&(SelItem == 2||SelItem == 32)) //ǰ��Բ���߻���Բ��
			{				
				EndDot[X] = PickDot.x;
				EndDot[Y] = PickDot.y;

				LineLen = GetPreciseLength(LineSX,LineSY,EndDot[X],EndDot[Y]);
				LineOA = GetAngleOfLineAndXaxes(LineSX,LineSY,EndDot[X],EndDot[Y]);
				//���Բ����ֱ���յ����벻����;
				if (LineLen < LineR) continue;
				if (LineLen < DataPrecision) continue;
				//������߷���;		
				LineOA = LineOA+CirDef*acos(LineR/LineLen);
				//������ת���ɷ�λ��;
				LineOA = ConvertAngleToOrient(LineOA);
				//��ȡ�е����꼴ֱ���������;
				StaDot[X] = LineSX+LineR*sin(LineOA);
				StaDot[Y] = LineSY+LineR*cos(LineOA);
			}	
			else  
			{
				GetProjectDotOfLine(StaDot[X],StaDot[Y],GetValueKByOrient(LineOA*180/PI),
					PickDot.x,PickDot.y,EndDot[X],EndDot[Y]);
			}
			if(pLine == NULL)
			{
				LineId = makeline1(StaDot,EndDot,3);
				/*	acdbOpenObject(pLine,LineId,AcDb::kForWrite);*/
				if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
				{
					ads_printf(L"��ʵ��ʧ�ܣ�\n");
					return;
				}
				pLine->close();
			}
			else
			{
				/*acdbOpenObject(pLine,LineId,AcDb::kForWrite);*/
				if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
				{
					ads_printf(L"��ʵ��ʧ�ܣ�\n");
					return;
				}
				pLine->setStartPoint(StaDot);
				pLine->setEndPoint(EndDot);
				pLine->close();
			}
			m_slsx = ChangeDataPrecision(StaDot[X]);
			m_slsy = ChangeDataPrecision(StaDot[Y]);
			m_slex = ChangeDataPrecision(EndDot[X]);
			m_sley = ChangeDataPrecision(EndDot[Y]);
			m_slcd = ChangeDataPrecision(distfwj(StaDot[X],StaDot[Y],EndDot[X],EndDot[Y],&LineOA));
			m_slfwj = ChangeDataPrecision(LineOA*180/PI);			
			UpdateData(FALSE);
		}
	}
	LineLen	=GetPreciseLength(StaDot[X],StaDot[Y],EndDot[X],EndDot[Y]);
	LineOA = ConvertAngleToOrient(
		GetAngleOfLineAndXaxes(StaDot[X],StaDot[Y],EndDot[X],EndDot[Y]));
	//������Ԫ;
	RouteArray[1][0] = 1;
	RouteArray[1][1] = LineLen;
	RouteArray[1][2] = 0;
	RouteArray[1][4] = 0;
	RouteArray[1][3] = 0;
	RouteArray[1][5] = LineOA;
	RouteArray[1][6] = 0;
	RouteArray[1][7] = 1E50;
	RouteArray[1][8] = StaDot[Y];
	RouteArray[1][9] = StaDot[X];
	//������Ԫ;
	RouteArray[2][0] = 0;
	RouteArray[2][1] = 0;
	RouteArray[2][2] = 0;
	RouteArray[2][3] = 0;
	RouteArray[2][4] = 0;
	RouteArray[2][5] = LineOA;
	RouteArray[2][6] = 0;
	RouteArray[2][7] = 1E50;
	RouteArray[2][8] = EndDot[Y];
	RouteArray[2][9] = EndDot[X];
	//������Ԫ;
	CanCreatRampItem = TRUE;
	if (SelItem ==1 ||SelItem == 2)
	{		
		RouteArray[0][0] = 0;
		RouteArray[0][1] = RouteArray[1][8];
		RouteArray[0][2] = RouteArray[1][9];
		RouteArray[0][3] = RouteArray[1][5];
		RouteArray[0][4] = RouteArray[1][6];
		RouteArray[0][5] = 0;
		RouteArray[0][6] = 0;
		RouteArray[0][7] = 0;
		RouteArray[0][8] = 0;
		RouteArray[0][9] = 0;
	}
	else
	{
		AcDbObject* pObj;
		if(acdbOpenObject(pObj,InputEntId,AcDb::kForRead)!=Acad::eOk)
			return;
		if(!pObj->isKindOf(JD_CENTER::desc()))
		{
			pObj->close();	
			RouteArray[0][0] = 0;
			RouteArray[0][1] = RouteArray[1][8];
			RouteArray[0][2] = RouteArray[1][9];
			RouteArray[0][3] = RouteArray[1][5];
			RouteArray[0][4] = RouteArray[1][6];
			RouteArray[0][5] = 0;
			RouteArray[0][6] = 0; 
			RouteArray[0][7] = 0;
			RouteArray[0][8] = 0;
			RouteArray[0][9] = 0;
			return ;		
		}
		JD_CENTER * pJdcenter = JD_CENTER::cast(pObj);


		/*JD_CENTER * pJdcenter = NULL;
		if (acdbOpenObject(pObj,InputEntId,AcDb::kForRead) != Acad::eOk)
		{
		pJdcenter=JD_CENTER::cast(pObj);
		RouteArray[0][0] = 0;
		RouteArray[0][1] = RouteArray[1][8];
		RouteArray[0][2] = RouteArray[1][9];
		RouteArray[0][3] = RouteArray[1][5];
		RouteArray[0][4] = RouteArray[1][6];
		RouteArray[0][5] = 0;
		RouteArray[0][6] = 0; 
		RouteArray[0][7] = 0;
		RouteArray[0][8] = 0;
		RouteArray[0][9] = 0;
		return ;
		}
		pJdcenter->close();*/
		//���ǰ��ΪJDֱ��ʵ��;
		if (SelItem == 31)
		{
			for(int i=0; i<10; i++)
			{
				RouteArray[0][i] = pJdcenter->XYArray[pJdcenter->XYNum-2][i];
				RouteArray[1][i] = pJdcenter->XYArray[pJdcenter->XYNum-1][i];
			}
			RouteArray[1][1] = pJdcenter->XYArray[pJdcenter->XYNum-1][1] + LineLen;
		}
		else 
		{
			for(int i=0; i<10; i++)
			{
				RouteArray[0][i] = pJdcenter->XYArray[pJdcenter->XYNum-1][i];
			}
			//����ֱ��������;
			RouteArray[1][6] = pJdcenter->XYArray[pJdcenter->XYNum][6];
			//�����JDԲʵ����Ҫ�޸�Բ�����Ȳ��ز�������ֱ��������;
			if (SelItem == 32 && m_FreeOrNot) 
			{
				double Sfwj = RouteArray[0][5],Efwj = RouteArray[1][5];
				double zj = ZXJ(Efwj,Sfwj);
				if (zj*RouteArray[0][4]<0) 
					zj = 2*PI-fabs(zj);
				RouteArray[0][2] = fabs(RouteArray[0][1]*zj);	
				RouteArray[1][6] = RouteArray[0][6] + RouteArray[0][2] ;
			}
			//����������Ҫ���������󻺲�������ֱ��;
			if (SelItem == 36) 
			{
				RouteArray[0][0] = 4.0;
				RouteArray[1][6] = RouteArray[0][6] + 
					RouteArray[0][1]*RouteArray[0][1]/RouteArray[0][2];
			}

		}	
		pJdcenter->close();
	}
	RouteArray[2][6] = RouteArray[1][6] +RouteArray[1][1];

	CanCreatRampItem = TRUE;

	acDocManager->unlockDocument(acDocManager->curDocument());
	//SetFocus();
	ads_printf(L"\nCommand:");
}

void CSetLineDlg::OnButtonsms() 
{
	// TODO: Add your control notification handler code here

}

void CSetLineDlg::OnRadioyouce() 
{
	// TODO: Add your control notification handler code here
	Vcexiang = 1;
}

void CSetLineDlg::OnRadioyoupian() 
{
	// TODO: Add your control notification handler code here
	Vpianxiang = 1;
}

void CSetLineDlg::OnRadiozouce() 
{
	// TODO: Add your control notification handler code here
	Vcexiang = -1;
}

void CSetLineDlg::OnRadiozoupian() 
{
	// TODO: Add your control notification handler code here
	Vpianxiang = -1;
}

void CSetLineDlg::OnButtonslsd() 
{
}

afx_msg LRESULT CSetLineDlg::OnAcadKeepFocus(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

BOOL CSetLineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//struct resbuf resOSMODE;//����OSNAP
	//resOSMODE.restype = RTSHORT;
	// ����CAD�ĵ�;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	// ��ȡ������;
	UpdateData(TRUE);
	// ������ȡ��ʵ��;
	AcDbObject* pEntity = NULL;
	if (acdbOpenObject(pEntity, InputEntId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	if (pEntity == NULL)
	{
		return false;
	}
	pEntity->close();

	// ���Զ���ʵ��Ͷ���߱任��ֱ��zl11.4
	BOOL IsRampRoad = FALSE;
	BAS_DRAW_FUN BAS1;
	AcGePoint3d startPt;
	AcDbObjectId lineId=NULL;
	if (_tcscmp(pEntity->isA()->name(), L"JD_CENTER") == 0)
	{
		
		startPt=BAS1.resetjd_centerid(point, pEntity, lineId);
		IsRampRoad = TRUE;
		if (acdbOpenObject(pEntity, lineId, AcDb::kForRead) != Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return false;
		}
		pEntity->close();
		InputEntId = lineId;
	}
	//���򿪵���һ��Ļ������ߣ�������ֱ�ߣ�����֮������2019.11.4zl
	if (_tcscmp(pEntity->isA()->name(), L"AcDbPolyline") == 0) //��������
	{
		XYarray xray;
		int res = GetXdataFromEntity(InputEntId, xray);
		if (res == 0)
		{
			InputEntId = NULL;
			AcDbPolyline* m_polyline = AcDbPolyline::cast(pEntity);
			int numverts = 0;
			int i = 0;
			int trow = 0;
			numverts = m_polyline->numVerts();
			AcGePoint3d  location, location1;
			for (i = 0; i < numverts; i++)
			{  //Ѱ�ҵ�ĳ����
				trow = i;
				m_polyline->getPointAt(i, location);
				m_polyline->getPointAt(i + 1, location1);
				BAS_DRAW_FUN_RAIL bas;
				double dXchuizu, dYchuizu;
				bas.ChuizuToZx(dXchuizu, dYchuizu, point.x, point.y, location.x, location.y, location1.x, location1.y);
				if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0 && (dYchuizu - location.y) * (dYchuizu - location1.y))
				{
					AcDbLine* pl1 = new AcDbLine(location, location1);
					InputEntId = AddEntityToDbs(pl1);
					if (acdbOpenObject(pEntity, InputEntId, AcDb::kForRead) != Acad::eOk)
					{
						ads_printf(L"��ʵ��ʧ�ܣ�\n");
						return false;
					}
					/*			pEntity->close();
								InputEntId = lineId;
								m_slsx = pl1->startPoint().x;
								m_slsy = pl1->startPoint().y;
								m_slex = pl1->endPoint().x;
								m_sley = pl1->endPoint().y;
								m_slcd = pl1->startPoint().distanceTo(pl1->endPoint());
								InputEntId = AddEntityToDbs(pl1);*/
					break;
				}
			}
			/*ads_printf(L"ѡ��ʵ����Ч!\n");
			pEntity->close();*/
		}
	}
	//�õ��µ�ʵ��
	GetComItemFromSelEntity(InputEntId, ComItem);                        // :E,C,S;
	GetComItemFromSelEntity(InputEntId, InterItem, ComItem.ItemDef, FALSE);

	// TODO: Add extra initialization here
	if (pEntity->isKindOf(AcDbLine::desc()))
	{
		m_slsx = ComItem.FDotX;
		m_slsy = ComItem.FDotY;
	}
	else if (pEntity->isKindOf(AcDbArc::desc()))
	{
		//GetComItemFromSelEntity(InputEntId, ComItem);                        // :E,C,S;
		//GetComItemFromSelEntity(InputEntId, InterItem, ComItem.ItemDef, FALSE);
		m_slsx = ComItem.FDotX;
		m_slsy = ComItem.FDotY;
		/*ComItem.FDotOrient = ConvertAngleToOrient(arc->endAngle() + PI / 2);
		m_slsx = arc;
		m_slsy = startPt.y;*/
		//double angle;
		//angle = arc->startAngle();
		//angle = 2 * PI - angle;
		//if (angle > 2 * PI)angle -= 2 * PI;
		////double angletemp = FwjtoAngX(angle);
		//m_slfwj = ChangeDataPrecision(angle * 180 / PI);
	}
	pEntity->close();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetLineDlg::OnButtonupdate() 
{
	//����CAD�ĵ�;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	
	//���Ѵ�������ʵ���ȡ�����յ�����;	
	if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
	{
		return ;
	}
	//��ȡֱ��ʵ�����յ�����;	
	double LineSX = (pLine->startPoint())[X];
	double LineSY = (pLine->startPoint())[Y];
	double LineEX = (pLine->endPoint())[X];
	double LineEY = (pLine->endPoint())[Y];
	pLine->close();
	//��ȡֱ�ߵķ�λ��;
	double ValueOrient;
	distfwj(LineSX,LineSY,LineEX,LineEY,&ValueOrient);
	//���ˢ��ǰ�������ƫ��(ƫ��),���ƫ��;
	double SPDeflect ;
	if (m_pianxiang == 0) SPDeflect = -1;
	else SPDeflect = 1;

	double SPOffset;
	if (m_cixiang == 0) SPOffset = -1;
	else SPOffset = 1;
	BackTonotModifidOfSDot(LineSX,LineSY,SPOffset,m_pianjv,ValueOrient,SPDeflect,m_pianjiao);
	// 2005.06.12 hhp
	if (SelItem == 31) 
	{
		RouteArray[1][1] = RouteArray[1][1] - m_slcd;
	}
	//��ȡ�µ�����;
	UpdateData(TRUE);
	//�����µ����(ƫ����ƫ��,ֱ�ߵĳ��������),�µ�ֱ�߷�λ��;
	if (m_cixiang == 0) SPOffset = -1;
	else SPOffset = 1;

	if (m_pianxiang == 0) SPDeflect = -1;
	else SPDeflect = 1;
	ModifyStartDot(LineSX,LineSY,SPOffset,m_pianjv,ValueOrient,SPDeflect,m_pianjiao);
	//��ȡֱ�ߵĳ���;
	double LineLen = m_slcd;
	//����ֱ�ߵ��յ�;
	LineEX = LineSX + LineLen*sin(ValueOrient);
	LineEY = LineSY + LineLen*cos(ValueOrient);
	//�޸�ֱ������;
	if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
	{
		return ;
	}
	//��ȡֱ��ʵ�����յ�����;	
	pLine->setStartPoint(AcGePoint3d(LineSX,LineSY,0.0));
	pLine->setEndPoint(AcGePoint3d(LineEX,LineEY,0.0));
	pLine->close();
	//���¶Ի���������;
	m_slsx = LineSX;
	m_slex = LineSY;
	m_slsy = LineEX;
	m_sley = LineEY;
	m_slcd = distfwj(LineSX,LineSY,LineEX,LineEY,&ValueOrient);
	m_slfwj = ValueOrient*180/PI;
	UpdateData(FALSE);
	//����������Ԫ����;
	//������Ԫ;
	if (SelItem == 31) 
	{
		RouteArray[1][1] = RouteArray[1][1]+m_slcd;
		RouteArray[1][5] = ValueOrient;
		RouteArray[1][7] = 1E50;
	}
	else
	{
		RouteArray[1][1] = m_slcd;
		RouteArray[1][5] = ValueOrient;
		RouteArray[1][7] = 1E50;
		RouteArray[1][8] = LineSY;
		RouteArray[1][9] = LineSX;
	}
	//������Ԫ;
	RouteArray[2][5] = ValueOrient;
	RouteArray[2][6] = RouteArray[1][6]+RouteArray[1][1];
	RouteArray[2][7] = 1E50;
	RouteArray[2][8] = LineSY+RouteArray[1][1]*cos(ValueOrient);
	RouteArray[2][9] = LineSX+RouteArray[1][1]*sin(ValueOrient);

	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();				
}

void CSetLineDlg::OnCheckfreeset() 
{
	// TODO: Add your control notification handler code here
	ClickCout+=1;
	if (fmod(ClickCout,2) !=0) 
		m_FreeOrNot = TRUE;
	else m_FreeOrNot = FALSE;	
}

// �������
void CSetLineDlg::OnButsetsdot() 
{

}
