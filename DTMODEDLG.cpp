// DTMODEDLG.cpp : implementation file
//

#include "stdafx.h"
#include "DTMODEDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DTMODEDLG dialog
extern DTMODEDLG *DTBXDlg;

DTMODEDLG::DTMODEDLG(CWnd* pParent /*=NULL*/)
	: CDialog(DTMODEDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(DTMODEDLG)
	m_ChangeDYR = FALSE;
	m_ChangeREnd = FALSE;
	m_ChangeRStart = FALSE;
	m_DelEnt = true;
	m_DragBC = true;
	m_LjEnd = true;
	m_LjStart = true;
	m_A1L1 = _T("1");
	m_A2L2 = _T("1");
	m_A3L3 = _T("1");
	m_A4L4 = _T("1");
	m_EndR = _T("");
	m_MoveR = _T("");
	m_StartR = _T("");
	m_BC = _T("5");
	m_Rcha = _T("0");
	m_A1L1SHOW = _T("A1:");
	m_A2L2SHOW = _T("A2:");
	m_A3L3SHOW = _T("A3:");
	m_A4L4SHOW = _T("A4:");
	pEnt1=pEnt2=pEnt2=NULL;
	m_QTYPE = 1;
	m_HTYPE = 1;
	CirID = 0;
	XLNUM = 1;
	xynum = 0;
	xynum2 = 0;
	//}}AFX_DATA_INIT
}

void DTMODEDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DTMODEDLG)
	DDX_Check(pDX, IDC_CHECK_CHANGEDYR, m_ChangeDYR);
	DDX_Check(pDX, IDC_CHECK_CHANGEREND, m_ChangeREnd);
	DDX_Check(pDX, IDC_CHECK_CHANGESTR, m_ChangeRStart);
	DDX_Check(pDX, IDC_CHECK_DELENT, m_DelEnt);
	DDX_Check(pDX, IDC_CHECK_DRAGBC, m_DragBC);
	DDX_Check(pDX, IDC_CHECK_LJEND, m_LjEnd);
	DDX_Check(pDX, IDC_CHECK_LJSTART, m_LjStart);
	DDX_Text(pDX, IDC_EDIT_A1L1, m_A1L1);
	DDX_Text(pDX, IDC_EDIT_A2L2, m_A2L2);
	DDX_Text(pDX, IDC_EDIT_A3L3, m_A3L3);
	DDX_Text(pDX, IDC_EDIT_A4L4, m_A4L4);
	DDX_Text(pDX, IDC_EDIT_ENDR, m_EndR);
	DDX_Text(pDX, IDC_EDIT_MOVECR, m_MoveR);
	DDX_Text(pDX, IDC_EDIT_STARTR, m_StartR);
	DDX_Text(pDX, IDC_EDIT_DRAGBC, m_BC);
	DDX_Text(pDX, IDC_EDIT_RCHAZHI, m_Rcha);
	DDX_Text(pDX, IDC_STATIC_A1L1, m_A1L1SHOW);
	DDX_Text(pDX, IDC_STATIC_A2L2, m_A2L2SHOW);
	DDX_Text(pDX, IDC_STATIC_A3L3, m_A3L3SHOW);
	DDX_Text(pDX, IDC_STATIC_A4L4, m_A4L4SHOW);
	DDX_Radio(pDX, IDC_RADIO_CTYPE, m_QTYPE);
	DDX_Radio(pDX, IDC_RADIO_CTYPEH, m_HTYPE);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DTMODEDLG, CDialog)
	//{{AFX_MSG_MAP(DTMODEDLG)
	ON_BN_CLICKED(IDC_RADIO_SELPOINT, OnRadioSelpoint)
	ON_BN_CLICKED(IDC_RADIO_CIRPOINT, OnRadioCirpoint)
	ON_BN_CLICKED(IDC_RADIO_LENGTH, OnRadioLength)
	ON_BN_CLICKED(IDC_RADIO_AVALUE, OnRadioAvalue)
	ON_BN_CLICKED(IDC_RADIO_UNCLOCK, OnRadioUnclock)
	ON_BN_CLICKED(IDC_RADIO_CLOCK, OnRadioClock)
	ON_BN_CLICKED(IDC_RADIO_CTYPEH, OnRadioCtypeh)
	ON_BN_CLICKED(IDC_RADIO_STYPEH, OnRadioStypeh)
	ON_BN_CLICKED(IDC_RADIO_CTYPE, OnRadioCtype)
	ON_BN_CLICKED(IDC_RADIO_STYPE, OnRadioStype)
	ON_BN_CLICKED(IDC_CHECK_DRAGBC, OnCheckDragbc)
	ON_BN_CLICKED(IDC_CHECK_DELENT, OnCheckDelent)
	ON_BN_CLICKED(IDC_CHECK_LJSTART, OnCheckLjstart)
	ON_BN_CLICKED(IDC_CHECK_LJEND, OnCheckLjend)
	ON_BN_CLICKED(IDC_CHECK_CHANGESTR, OnCheckChangestr)
	ON_BN_CLICKED(IDC_CHECK_CHANGEDYR, OnCheckChangedyr)
	ON_BN_CLICKED(IDC_CHECK_CHANGEREND, OnCheckChangerend)
	ON_BN_CLICKED(IDC_BUTTON_DRAG, OnButtonDrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DTMODEDLG message handlers

BOOL DTMODEDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	for(int i=0; i<5; i++)
	{
		xyid[i] = 0;
		xyid2[i] = 0;
	}

	CButton *pBtn;

	ShqDir = 1;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_CLOCK);
	pBtn->SetCheck(1);		    //起始缓曲方向

	DragPos = 1;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_CIRPOINT);
	pBtn->SetCheck(1);		//拖动位置

	CsType = 1;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_AVALUE);
	pBtn->SetCheck(1);//参数类型

	GetDlgItem(IDC_EDIT_STARTR)->EnableWindow(false);//起始元半径
	GetDlgItem(IDC_EDIT_MOVECR)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ENDR)->EnableWindow(false);

	hQXType  = 0;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_STYPEH);//前双曲线类型
	pBtn->SetCheck(1);
	qQXType = 0;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO_STYPE);//后双曲线类型
	pBtn->SetCheck(1);
	double R1=0,R2=0,R3=0;
	AcGePoint3d Center1, Center2,Center3;
	if(IDarr[0]==0)
	{
		GetDlgItem(IDC_EDIT_A1L1)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_A2L2)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_CTYPE)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_STYPE)->EnableWindow(false);
		qQXType = 1;
	}
	else
	{
		/*acdbOpenObject(pEnt1,IDarr[0],AcDb::kForRead);*/
		if(acdbOpenObject(pEnt1,IDarr[0], AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		pEnt1->close();
		/*acdbOpenObject(pEnt2,IDarr[1],AcDb::kForRead);*/
		if(acdbOpenObject(pEnt2,IDarr[1], AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		pEnt2->close();
		//2019.11.4
		//根据主线id重新绘制直线或曲线，获取新线形的id
		AcDbObjectId lineid;
		BAS_DRAW_FUN BAS;
		if (_tcscmp(pEnt1->isA()->name(), L"JD_CENTER") == 0)
		{
			BAS.resetjd_centerid(PT,pEnt1,lineid);
			if (acdbOpenObject(pEnt1, lineid, AcDb::kForRead) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pEnt1->close();
			IDarr[0] = lineid;
		}
		////
		if((pEnt1->isKindOf(AcDb2dPolyline::desc())
			|| pEnt1->isKindOf(AcDbPolyline::desc())
			|| pEnt1->isKindOf(AcDbLine::desc()))
			&&(pEnt2->isKindOf(AcDbArc::desc())
			||pEnt2->isKindOf(AcDbCircle::desc())))//直线和圆
		{
			GetDlgItem(IDC_EDIT_A2L2)->EnableWindow(false);
			GetDlgItem(IDC_RADIO_CTYPE)->EnableWindow(false);
			GetDlgItem(IDC_RADIO_STYPE)->EnableWindow(false);
			m_StartR="100000";
			if(pEnt2->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt2);
				Center2.x=ARC->center().y;
				Center2.y=ARC->center().x;
				R2=ARC->radius();
			}
			else
			{
				AcDbCircle *Cir=AcDbCircle::cast(pEnt2);
				Center2.x=Cir->center().y;
				Center2.y=Cir->center().x;
				R2=Cir->radius();
			}
			m_MoveR.Format(L"%0.2lf",R2);
			MCenX = Center2.x;
			MCenY = Center2.y;
		}
		else if((pEnt1->isKindOf(AcDbArc::desc())
			|| pEnt1->isKindOf(AcDbCircle::desc()))&&
			(pEnt2->isKindOf(AcDbArc::desc())
			|| pEnt2->isKindOf(AcDbCircle::desc())))//圆和圆
		{
			if(pEnt1->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt1);
				Center1.x=ARC->center().y;
				Center1.y=ARC->center().x;
				R1=ARC->radius();
			}
			else
			{
				AcDbCircle *Cir=AcDbCircle::cast(pEnt1);
				Center1.x=Cir->center().y;
				Center1.y=Cir->center().x;
				R1=Cir->radius();
			}

			if(pEnt2->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt2);
				Center2.x=ARC->center().y;
				Center2.y=ARC->center().x;
				R2=ARC->radius();
			}
			else
			{
				AcDbCircle *Cir=AcDbCircle::cast(pEnt2);
				Center2.x=Cir->center().y;
				Center2.y=Cir->center().x;
				R2=Cir->radius();
			}
			m_StartR.Format(L"%0.2lf",R1);
			m_MoveR.Format(L"%0.2lf",R2);
			MCenX = Center2.x;
			MCenY = Center2.y;
			if(AutoModel(Center1,Center2,R1,R2)==1)//设为C
			{
				qQXType  = 1;
				m_QTYPE = 0;
				pBtn = (CButton *)GetDlgItem(IDC_RADIO_CTYPE);
				pBtn->SetCheck(1);
				pBtn = (CButton *)GetDlgItem(IDC_RADIO_STYPE);
				pBtn->SetCheck(0);				
			}
		}
	}
	if(IDarr[2]==0)
	{
		GetDlgItem(IDC_EDIT_A3L3)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_A4L4)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_CTYPEH)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_STYPEH)->EnableWindow(false);
	}
	else
	{
		/*acdbOpenObject(pEnt2,IDarr[1],AcDb::kForRead);*/
		if(acdbOpenObject(pEnt2,IDarr[1], AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		pEnt2->close();		
		/*acdbOpenObject(pEnt3,IDarr[2],AcDb::kForRead);*/
		if(acdbOpenObject(pEnt3,IDarr[2], AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		pEnt3->close();	
		//第三个线元可能是主线，特殊考虑
		AcDbObjectId lineid1;
		BAS_DRAW_FUN BAS1;
		if (_tcscmp(pEnt3->isA()->name(), L"JD_CENTER") == 0)
		{
			BAS1.resetjd_centerid(PT1,pEnt3, lineid1);
			if (acdbOpenObject(pEnt3, lineid1, AcDb::kForRead) != Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return false;
			}
			pEnt3->close();
			IDarr[2] = lineid1;
		}
		if((pEnt3->isKindOf(AcDb2dPolyline::desc())
			|| pEnt3->isKindOf(AcDbPolyline::desc())
			|| pEnt3->isKindOf(AcDbLine::desc()))
			&&(pEnt2->isKindOf(AcDbArc::desc())
			||pEnt2->isKindOf(AcDbCircle::desc())))//直线和圆
		{
			GetDlgItem(IDC_EDIT_A4L4)->EnableWindow(false);
			GetDlgItem(IDC_RADIO_CTYPEH)->EnableWindow(false);
			GetDlgItem(IDC_RADIO_STYPEH)->EnableWindow(false);
			m_EndR = "100000";
			if(pEnt2->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt2);
				Center2.x=ARC->center().y;
				Center2.y=ARC->center().x;
				R2=ARC->radius();
				m_MoveR.Format(L"%0.2lf",R2);
				MCenX = Center2.x;
				MCenY = Center2.y;

			}
			else
			{
				AcDbCircle *Cir=AcDbCircle::cast(pEnt2);
				Center2.x=Cir->center().y;
				Center2.y=Cir->center().x;
				R2=Cir->radius();
				m_MoveR.Format(L"%0.2lf",R2);
				MCenX = Center2.x;
				MCenY = Center2.y;

			}

		}
		else if(pEnt3->isKindOf(AcDbArc::desc())
			|| pEnt3->isKindOf(AcDbCircle::desc()))//圆和圆
		{
			if(pEnt3->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt3);
				Center3.x=ARC->center().y;
				Center3.y=ARC->center().x;
				R3=ARC->radius();
			}
			else
			{
				AcDbCircle *Cir=AcDbCircle::cast(pEnt3);
				Center3.x=Cir->center().y;
				Center3.y=Cir->center().x;
				R3=Cir->radius();
			}			
			if(pEnt2->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt2);
				Center2.x=ARC->center().y;
				Center2.y=ARC->center().x;
				R2=ARC->radius();
			}
			else
			{
				AcDbCircle *Cir=AcDbCircle::cast(pEnt2);
				Center2.x=Cir->center().y;
				Center2.y=Cir->center().x;
				R2=Cir->radius();
			}

			m_MoveR.Format(L"%0.2lf",R2);
			m_EndR.Format(L"%0.2lf",R3);
			MCenX = Center2.x;
			MCenY = Center2.y;
			if(AutoModel(Center2,Center3,R2,R3)==1)//设为C
			{
				hQXType  = 1;	           
				m_HTYPE = 0;
				pBtn = (CButton *)GetDlgItem(IDC_RADIO_CTYPEH);
				pBtn->SetCheck(1);
				pBtn = (CButton *)GetDlgItem(IDC_RADIO_STYPEH);
				pBtn->SetCheck(0);				
			}

		}		
	}
	m_MoveR.Format(L"%0.2lf",R2);
	//	ads_printf(L"%s %s %s\n",m_StartR,m_MoveR,m_EndR);
	UpdateData(false);
	if(fabs(_wtof(m_Rcha))>0.001)
	{
		AcDbCircle *Cir = new AcDbCircle(Center2,AcGeVector3d(0,0,1),R2+_wtof(m_Rcha));
		CirID = AddEntityToDbs(Cir);
	}
	CalXYData();


	//	GetDlgItem(IDC_EDIT_DRAGBC)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DTMODEDLG::OnRadioSelpoint() 
{
	// TODO: Add your control notification handler code here
	DragPos = 0;
}

void DTMODEDLG::OnRadioCirpoint() 
{
	// TODO: Add your control notification handler code here
	DragPos = 1;
}

void DTMODEDLG::OnRadioLength() 
{
	// TODO: Add your control notification handler code here
	if(CsType==1)
	{
		if(_wtof(m_StartR) > 0.1&& _wtof(m_StartR) < 99999)
			m_A1L1.Format(L"%0.3lf",sqrt(_wtof(m_A1L1)*_wtof(m_StartR)));
		else
			m_A1L1.Format(L"%0.3lf",sqrt(_wtof(m_A1L1)*_wtof(m_MoveR)));
		m_A2L2.Format(L"%0.3lf",sqrt(_wtof(m_A2L2)*_wtof(m_MoveR)));
		m_A3L3.Format(L"%0.3lf",sqrt(_wtof(m_A3L3)*_wtof(m_MoveR)));
		if(_wtof(m_EndR) > 0.1&& _wtof(m_EndR) < 99999)
			m_A4L4.Format(L"%0.3lf",sqrt(_wtof(m_A4L4)*_wtof(m_EndR)));
	}
	CsType = 0;
	m_A1L1SHOW = "L1:";
	m_A2L2SHOW = "L2:";
	m_A3L3SHOW = "L3:";
	m_A4L4SHOW = "L4:";

	UpdateData(false);

}

void DTMODEDLG::OnRadioAvalue() 
{
	// TODO: Add your control notification handler code here
	if(CsType==0)
	{
		if(_wtof(m_StartR) > 0.1&& _wtof(m_StartR) < 99999)		
			m_A1L1.Format(L"%0.3lf",_wtof(m_A1L1)*_wtof(m_A1L1)/_wtof(m_StartR));
		else
			m_A1L1.Format(L"%0.3lf",_wtof(m_A1L1)*_wtof(m_A1L1)/_wtof(m_MoveR));
		m_A2L2.Format(L"%0.3lf",_wtof(m_A2L2)*_wtof(m_A2L2)/_wtof(m_MoveR));
		m_A3L3.Format(L"%0.3lf",_wtof(m_A3L3)*_wtof(m_A3L3)/_wtof(m_MoveR));
		if(_wtof(m_EndR) > 0.1&& _wtof(m_EndR) < 99999)	
			m_A4L4.Format(L"%0.3lf",_wtof(m_A4L4)*_wtof(m_A4L4)/_wtof(m_EndR));
	}
	CsType = 1;
	m_A1L1SHOW = "A1:";
	m_A2L2SHOW = "A2:";
	m_A3L3SHOW = "A3:";
	m_A4L4SHOW = "A4:";
	UpdateData(false);
}

void DTMODEDLG::OnRadioUnclock() 
{
	// TODO: Add your control notification handler code here
	ShqDir = -1;
}

void DTMODEDLG::OnRadioClock() 
{
	// TODO: Add your control notification handler code here
	ShqDir = 1; 
}

void DTMODEDLG::OnRadioCtypeh() 
{
	// TODO: Add your control notification handler code here
	hQXType = 1;
}

void DTMODEDLG::OnRadioStypeh() 
{
	// TODO: Add your control notification handler code here
	hQXType = 0;
}

void DTMODEDLG::OnRadioCtype() 
{
	// TODO: Add your control notification handler code here
	qQXType = 1;
}

void DTMODEDLG::OnRadioStype() 
{
	// TODO: Add your control notification handler code here
	qQXType = 0;
}

void DTMODEDLG::OnCheckDragbc() 
{
	// TODO: Add your control notification handler code here
	m_DragBC = !m_DragBC;
	if(m_DragBC)
		GetDlgItem(IDC_EDIT_DRAGBC)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_DRAGBC)->EnableWindow(false);
}

void DTMODEDLG::OnCheckDelent() 
{
	// TODO: Add your control notification handler code here
	m_DelEnt = !m_DelEnt;
}

void DTMODEDLG::OnCheckLjstart() 
{
	// TODO: Add your control notification handler code here
	m_LjStart = !m_LjStart;
}

void DTMODEDLG::OnCheckLjend() 
{
	// TODO: Add your control notification handler code here
	m_LjEnd = !m_LjEnd;
}

void DTMODEDLG::OnCheckChangestr() 
{
	// TODO: Add your control notification handler code here
	m_ChangeRStart = !m_ChangeRStart;
	if(m_ChangeRStart)
		GetDlgItem(IDC_EDIT_STARTR)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_STARTR)->EnableWindow(false);
}

void DTMODEDLG::OnCheckChangedyr() 
{
	// TODO: Add your control notification handler code here
	m_ChangeDYR = !m_ChangeDYR;
	if(m_ChangeDYR)
		GetDlgItem(IDC_EDIT_MOVECR)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_MOVECR)->EnableWindow(false);
}

void DTMODEDLG::OnCheckChangerend() 
{
	// TODO: Add your control notification handler code here
	m_ChangeREnd = !m_ChangeREnd;
	if(m_ChangeREnd)
		GetDlgItem(IDC_EDIT_ENDR)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_ENDR)->EnableWindow(false);
}
//计算线元数据
void DTMODEDLG::CalXYData() 
{
	int SXYNum,EXYNum,k,j;
	double A[2],LL;
	double SXYArray[2][10],EXYArray[2][10],MXY[10],INDATA[5];
	SXYNum=EXYNum=0;
	UpdateData(true);
	qQXType = !m_QTYPE;
	hQXType = !m_HTYPE;
	//    ads_printf(L"PID=%ld CID=%ld AID=%ld\n",IDarr[0],IDarr[1],IDarr[2]);
	//改半径的问题?
	//1.计算起始控制单元与连接元线元数据
	XLNUM = 1;
	if(IDarr[0]!=0)//有起始单元
	{
		changeid = false;
		A[0] = _wtof(m_A1L1) , A[1] = _wtof(m_A2L2);
		if(CsType==0)//L value
		{
			A[0] =sqrt(_wtof(m_A1L1)*_wtof(m_StartR));
			A[1] =sqrt(_wtof(m_A2L2)*_wtof(m_MoveR));
		}
		changeAcDbObjectId(IDarr[0]);
		Msf2Elem(IDarr[0], IDarr[1], qQXType, ShqDir, A, SXYArray, SXYNum);
		if(SXYNum==0)
		{
			xynum =0 ;
			return;
		}
		if(SXYNum>0)
		{
			if(CsType)//A值
				m_A1L1.Format(L"%0.3lf",SXYArray[0][1]);
			else
			{
				LL = 0;
				for(k=0;k<5;k++) INDATA[k]=SXYArray[0][k];
				LL=Get_INDATA_L(INDATA,&j);                
				m_A1L1.Format(L"%0.3lf",LL);
			}
			if(SXYNum==2)
			{
				if(CsType)//A值
					m_A2L2.Format(L"%0.3lf",SXYArray[1][1]);
				else
				{
					LL = 0;
					for(k=0;k<5;k++) INDATA[k]=SXYArray[1][k];
					LL=Get_INDATA_L(INDATA,&j);                
					m_A2L2.Format(L"%0.3lf",LL);
				}
			}
		}
	}

	//2.计算终止控制单元与连接元线元数据
	if(IDarr[2]!=0)//有终止单元
	{
		changeid = true;
		A[0] = _wtof(m_A3L3) , A[1] = _wtof(m_A4L4);
		if(CsType==0)//L value
		{
			A[0] =sqrt(_wtof(m_A3L3)*_wtof(m_MoveR));
			A[1] =sqrt(_wtof(m_A4L4)*_wtof(m_EndR));
		}
		changeAcDbObjectId(IDarr[2]);
		if(qQXType==0 && IDarr[0]!=0 && IDarr[1]!=0 && IDarr[2]!=0)//s
			Msf2Elem(IDarr[1],IDarr[2],hQXType,-1*ShqDir,A,EXYArray,EXYNum);
		else
			Msf2Elem(IDarr[1],IDarr[2],hQXType,ShqDir,A,EXYArray,EXYNum);

		if(CirID!=0 && fabs(_wtof(m_Rcha))>0.001)
		{
			if(qQXType==0 && IDarr[0]!=0 && IDarr[1]!=0 && IDarr[2]!=0)//s
				Msf2Elem(CirID,IDarr[2],hQXType,-1*ShqDir,A,EXYArray,EXYNum);
			else
				Msf2Elem(CirID,IDarr[2],hQXType,ShqDir,A,EXYArray,EXYNum);
			XLNUM = 2;
		}

		if(EXYNum==0)
		{
			xynum =0 ;
			return;
		}
		if(EXYNum>0)
		{
			if(CsType)//A值
				m_A3L3.Format(L"%0.3lf",EXYArray[0][1]);
			else
			{
				LL = 0;
				for(k=0;k<5;k++) INDATA[k]=EXYArray[0][k];
				LL=Get_INDATA_L(INDATA,&j);                
				m_A3L3.Format(L"%0.3lf",LL);
			}       
			if(EXYNum==2)
			{
				if(CsType)//A值
				{
					m_A4L4.Format(L"%0.3lf",EXYArray[1][1]);
				}
				else
				{		
					LL = 0;
					for(k=0;k<5;k++) INDATA[k]=EXYArray[1][k];
					LL=Get_INDATA_L(INDATA,&j);                
					m_A4L4.Format(L"%0.3lf",LL);
				}
			}
		}	
	}
	int n;
	for(n=0; n<SXYNum; n++)
		for(int j=0; j<10; j++)
			XY[n][j] = SXYArray[n][j];
	xynum = n;
	//3.计算连接圆线元数据
	if(IDarr[0]!=0 && IDarr[2]!=0 && CirID==0)
	{
		MXY[0] = 2.0;
		MXY[1] = _wtof(m_MoveR);
		MXY[3] =  0.0;
		if(qQXType==0 && IDarr[0]!=0 && IDarr[1]!=0 && IDarr[2]!=0)
			MXY[4] =  -ShqDir;
		else
			MXY[4] =  ShqDir;
		MXY[7] =  _wtof(m_MoveR);

		double Spt[2],Ept[2],INDATA[5],len,sfwj,efwj,zj;

		for (int i=0;i<5;i++) INDATA[i]=SXYArray[SXYNum-1][i];
		int j;

		len=Get_INDATA_L(INDATA,&j);
		xlpoint PBI,PZ;
		PBI.a  = SXYArray[SXYNum-1][5];
		PBI.lc = SXYArray[SXYNum-1][6];
		PBI.x  = SXYArray[SXYNum-1][8];
		PBI.y  = SXYArray[SXYNum-1][9];
		// len 为该线元的长度 m
		xlzbxy(&PBI,&PZ,len,INDATA,0);
		Spt[0] = PZ.x;
		Spt[1] = PZ.y;
		sfwj = PZ.a;
		MXY[8] = Spt[0];
		MXY[9] = Spt[1];
		MXY[5] = sfwj;
		MXY[6] = PBI.lc + len;
		//计算圆心到起点的方位角
		efwj = EXYArray[0][5];
		if(MXY[4]<0.0)//左转
		{
			sfwj +=0.5*PI;
			if(sfwj > 2*PI)
				sfwj-=2*PI;
			efwj +=0.5*PI;
			if(efwj > 2*PI)
				efwj-=2*PI;
			zj = sfwj - efwj;
			if(zj<0.0)
				zj+=2*PI;
		}
		else
		{
			sfwj -=0.5*PI;
			if(sfwj < 0)
				sfwj+=2*PI;
			efwj -=0.5*PI;
			if(efwj < 0)
				efwj+=2*PI;
			zj = efwj - sfwj;
			if(zj<0.0)
				zj+=2*PI;
		}

		MXY[2] =  MXY[1] * fabs(zj);

		for(j=0; j<10; j++)
			XY[n][j] = MXY[j];
		n++;
	}
	UpdateData(false);
	//4.生成整体线元数据
	if(XLNUM==1) 
	{
		int i;
		for( i=0; i<EXYNum; i++)
			for(int j=0; j<10; j++)
				XY[n+i][j] = EXYArray[i][j];
		xynum = n+i;
	}
	else
	{
		for(n=0; n<EXYNum; n++)
			for(int j=0; j<10; j++)
				XY2[n][j] = EXYArray[n][j];
		xynum2 = n;
	}	
}

BOOL DTMODEDLG::changeAcDbObjectId(AcDbObjectId& Id1)
{
	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if (acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite) != Acad::eOk)
	{
		ads_printf(L"CAD文档打开错误!\n");
		return FALSE;
	}
	// 打开Id1实体
	AcDbEntity* pEntity = NULL;
	if (acdbOpenObject(pEntity, Id1, AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return FALSE;
	}
	pEntity->close(); // 关闭所选实体

	if (_tcscmp(pEntity->isA()->name(), L"AcDbPolyline") == 0)
	{
		if (changeid == false)
		{
			Id1 = NULL;
			AcDbPolyline* m_polyline = AcDbPolyline::cast(pEntity);
			int numverts = 0;
			int i = 0;
			int trow = 0;
			numverts = m_polyline->numVerts();
			AcGePoint3d  location, location1;
			for (i = 0; i < numverts; i++)
			{  //寻找的某顶点
				trow = i;
				m_polyline->getPointAt(i, location);
				m_polyline->getPointAt(i + 1, location1);
				BAS_DRAW_FUN_RAIL bas;
				double dXchuizu, dYchuizu;
				bas.ChuizuToZx(dXchuizu, dYchuizu, PT.x, PT.y, location.x, location.y, location1.x, location1.y);
				if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0&& (dYchuizu - location.y) * (dYchuizu - location1.y))
				{
					AcDbLine* pl1 = new AcDbLine(location, location1);
					Id1 = AddEntityToDbs(pl1);
					break;
				}
			}
		}
		else
		{
			Id1 = NULL;
			AcDbPolyline* m_polyline1 = AcDbPolyline::cast(pEntity);
			int numverts = 0;
			int i = 0;
			int trow = 0;
			numverts = m_polyline1->numVerts();
			AcGePoint3d  location, location1;
			for (i = 0; i < numverts; i++)
			{  //寻找的某顶点
				trow = i;
				m_polyline1->getPointAt(i, location);
				m_polyline1->getPointAt(i + 1, location1);
				BAS_DRAW_FUN_RAIL bas;
				double dXchuizu, dYchuizu;
				bas.ChuizuToZx(dXchuizu, dYchuizu, PT1.x, PT1.y, location.x, location.y, location1.x, location1.y);
				if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0 && (dYchuizu - location.y) * (dYchuizu - location1.y))
				{
					AcDbLine* pl2 = new AcDbLine(location, location1);
					Id1 = AddEntityToDbs(pl2);
					break;
				}
			}
			
		}
		return true;
	}
	else
		return FALSE;
}

void DTMODEDLG::OnButtonDrag() 
{
	// TODO: Add your control notification handler code here
	int track=1,type,i;
	struct resbuf result ;
	AcGePoint3d pt,PT;
	ads_point Ptold,Ptnew;
	int count=0,rc,j;
	ads_name en;
	AcDbObjectId SelId;
	double fwj;

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	ShowWindow(FALSE);	
	acedGetAcadDwgView()->SetFocus();
	rc = ads_entsel(L"\n请选择动圆: ", en, asDblArray(PT));
	if(rc==RTCAN)
	{
		ShowWindow(true);	
		SetFocus();
		return;
	}
	if (rc != RTNORM) 
	{
		ads_printf(L"选择有误!\n");
		ShowWindow(true);	
		SetFocus();
		return;
	}       

	int idy=-1;
	acdbGetObjectId(SelId, en);
	for(i=0;i<3;i++)
	{
		if(SelId == IDarr[i])
		{
			idy = i;
			break;
		}
	}

	if(idy==-1)
	{
		ads_printf(L"动圆选择有误!\n");
		ShowWindow(true);	
		SetFocus();
		return;
	}

	AcDbObject *pEnt;
	AcDbArc *ARC;
	AcDbCircle *Circle;
	/*acdbOpenObject(pEnt,IDarr[idy],AcDb::kForRead);*/
	if(acdbOpenObject(pEnt,IDarr[idy], AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pEnt->close();		
	if(!pEnt->isKindOf(AcDbArc::desc())&&!pEnt->isKindOf(AcDbCircle::desc()))
	{
		ads_printf(L"应选择圆，而非直线!\n");
		ShowWindow(true);	
		SetFocus();
		return;
	}
	Ptold[X]=0;
	Ptold[Y]=0;
	Ptold[Z]=0;

	UpdateEnt();

	double RePt[2][2],l;
	ads_point pt1,pt2;
	int k;
	double INDATA[5],len;
	for(;;)
	{		
		if(ads_grread(1 , &type , &result)!=RTNORM)
		{ 
			break;
		}
		//		ads_printf(L"type=%d\n",type);
		if(type == 3)      break;
		if( (type!=5) && (type!=12) )
		{
			ads_printf(L"必须用鼠标!\n");
			break ;
		}
		//	if(type == 5)
		//	{
		Ptnew[Y] = result.resval.rpoint[Y];
		Ptnew[X] = result.resval.rpoint[X];
		Ptnew[Z] = 0.0;

		if(fabs(Ptold[X])>0.1&&caldistance(Ptnew,Ptold)>0.1)
		{
			pt.x = Ptnew[X];
			pt.y = Ptnew[Y];
			pt.z = 0; 

			/*acdbOpenObject(pEnt,IDarr[idy],AcDb::kForWrite);*/
			if(acdbOpenObject(pEnt,IDarr[idy],AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				ARC=AcDbArc::cast(pEnt);
				if(DragPos==1)
					ARC->setCenter(pt);
				else if(DragPos==0)
				{
					//计算圆心位置pt
					//1.计算矢量
					AcGeVector3d  vec(pt.x-Ptold[X],pt.y-Ptold[Y],pt.z-Ptold[Z]);
					//2.移动量
					AcGePoint3d cen;
					cen = ARC->center();
					cen.transformBy(vec);
					ARC->setCenter(cen);
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				Circle=AcDbCircle::cast(pEnt);
				if(DragPos==1)
					Circle->setCenter(pt);
				else if(DragPos==0)
				{
					//计算圆心位置pt
					//1.计算矢量
					AcGeVector3d  vec(pt.x-Ptold[X],pt.y-Ptold[Y],pt.z-Ptold[Z]);
					//2.移动量
					AcGePoint3d cen;
					cen = Circle->center();
					cen.transformBy(vec);
					Circle->setCenter(cen);
					//ads_printf(L"R=%lf \n",Circle->radius());
				}
			}
			pEnt->close();

			if(fabs(_wtof(m_Rcha))>0.001 && CirID!=0 && idy==1)
			{
				/*	acdbOpenObject(pEnt,CirID,AcDb::kForWrite);*/
				if(acdbOpenObject(pEnt,CirID,AcDb::kForWrite)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}

				if(pEnt->isKindOf(AcDbCircle::desc()))
				{
					Circle=AcDbCircle::cast(pEnt);
					Circle->setCenter(pt);
				}
				pEnt->close();
			}
			else
				XLNUM = 1;

		}

		//			if(count==0)
		//			{
		//				//计算出第几个单元idy及圆心坐标
		//			  idy =	FindWichDY(pt);
		//              count++;
		//			}			
		//      	UpdateEnt(pt.x,pt.y,idy);     

		if(caldistance(Ptnew,Ptold)>0.1)
		{
			CalXYData();		
			//			   GRDrawXY(xynum,XYOLD);
			//               if(XLNUM>1)
			//				   	GRDrawXY(xynum2,XYOLD2);
			acedRedraw(NULL, 0);
			GRDrawXY(xynum,XY);			  
			if(XLNUM>1)
				GRDrawXY(xynum2,XY2);

		}
		Ptold[X]=Ptnew[X];
		Ptold[Y]=Ptnew[Y];
		Ptold[Z]=Ptnew[Z];
		for(i=0; i<xynum; i++)
		{
			for(j=0;j<10;j++)  
				XYOLD[i][j] = XY[i][j] ;

		}
		if(XLNUM>1)
		{
			for(i=0; i<xynum2; i++)
			{
				for(j=0;j<10;j++)  
					XYOLD2[i][j] = XY2[i][j] ;
			}
		}

		//	}
	}
	for(i=0; i<xynum; i++)
	{
		if(XY[i][0]<1.1)
			DrawXY(XY[i],xyid[i],7);
		else if(XY[i][0]>2.1)
			DrawXY(XY[i],xyid[i],6);
		else
			DrawXY(XY[i],xyid[i],2);
	}	
	if(XLNUM>1)
	{
		for(i=0; i<xynum2; i++)
		{
			if(XY2[i][0]<1.1)
				DrawXY(XY2[i],xyid2[i],7);
			else if(XY2[i][0]>2.1)
				DrawXY(XY2[i],xyid2[i],6);
			else
				DrawXY(XY2[i],xyid2[i],2);
		}	
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(true);	
	SetFocus();
}
//更新实体
void DTMODEDLG::UpdateEnt() 
{
	UpdateData();
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//取出初始圆半径
	double SR,ER,MR,MinDis,dis;
	SR = _wtof(m_StartR);
	ER = _wtof(m_EndR);
	MR = _wtof(m_MoveR);
	AcGePoint3d cen;

	AcDbObject *pEnt;

	if(IDarr[0]!=0)
	{
		//判别是否为圆弧
		/*acdbOpenObject(pEnt,IDarr[0],AcDb::kForWrite);*/
		if(acdbOpenObject(pEnt,IDarr[0],AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}

		if(pEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc *ARC=AcDbArc::cast(pEnt);
			ARC->setRadius(SR);      
		}
		else if(pEnt->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle *Circle=AcDbCircle::cast(pEnt);
			Circle->setRadius(SR);
		}
		pEnt->close();

	}
	if(IDarr[1]!=0)
	{
		//判别是否为圆弧
		/*	acdbOpenObject(pEnt,IDarr[1],AcDb::kForWrite);*/
		if(acdbOpenObject(pEnt,IDarr[1],AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		if(pEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc *ARC=AcDbArc::cast(pEnt);
			ARC->setRadius(MR);
			cen = ARC->center(); 
		}
		else if(pEnt->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle *Circle=AcDbCircle::cast(pEnt);
			Circle->setRadius(MR);
			cen = Circle->center();
		}
		pEnt->close();
	}

	if(IDarr[2]!=0)
	{
		//判别是否为圆弧
		/*	acdbOpenObject(pEnt,IDarr[2],AcDb::kForWrite);*/
		if(acdbOpenObject(pEnt,IDarr[2],AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		if(pEnt->isKindOf(AcDbArc::desc()))
		{
			AcDbArc *ARC=AcDbArc::cast(pEnt);
			ARC->setRadius(ER);
		}
		else if(pEnt->isKindOf(AcDbCircle::desc()))
		{
			AcDbCircle *Circle=AcDbCircle::cast(pEnt);
			Circle->setRadius(ER);
		}
		pEnt->close();
	}

	if(fabs(_wtof(m_Rcha))>0.001)
	{
		if(CirID==0)
		{
			AcDbCircle *Cir = new AcDbCircle(cen,AcGeVector3d(0,0,1),MR+_wtof(m_Rcha));
			CirID = AddEntityToDbs(Cir);
		}
		else
		{
			/*acdbOpenObject(pEnt,CirID,AcDb::kForWrite);*/
			if(acdbOpenObject(pEnt,CirID,AcDb::kForWrite)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				Circle->setRadius(MR+_wtof(m_Rcha));
			}
			pEnt->close();
		}
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
}

int DTMODEDLG::FindWichDY(AcGePoint3d pt)
{
	double dis,MinDis = 1.0E40;
	int inf;
	inf = 1;
	int idy;
	idy = -1;

	if(DragPos==1)//拖动圆心
	{
		AcDbObject *pEnt;
		if(IDarr[0]!=0)
		{
			//判别是否为圆弧
			/*acdbOpenObject(pEnt,IDarr[0],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,IDarr[0], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt);
				dis = sqrt((ARC->center().x-pt.x)*(ARC->center().x-pt.x)+(ARC->center().y-pt.y)*(ARC->center().y-pt.y));
				if(dis<MinDis)
				{
					MinDis = dis;                     
					idy = 0;
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				dis = sqrt((Circle->center().x-pt.x)*(Circle->center().x-pt.x)+(Circle->center().y-pt.y)*(Circle->center().y-pt.y));
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 0;
				}
			}
			pEnt->close();
		}
		if(IDarr[1]!=0)
		{
			//判别是否为圆弧
			/*acdbOpenObject(pEnt,IDarr[1],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,IDarr[1], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt);
				dis = sqrt((ARC->center().x-pt.x)*(ARC->center().x-pt.x)+(ARC->center().y-pt.y)*(ARC->center().y-pt.y));
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 1;
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				dis = sqrt((Circle->center().x-pt.x)*(Circle->center().x-pt.x)+(Circle->center().y-pt.y)*(Circle->center().y-pt.y));
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 1;
				}
			}
			pEnt->close();
		}
		if(IDarr[2]!=0)
		{
			//判别是否为圆弧
			/*acdbOpenObject(pEnt,IDarr[1],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,IDarr[1], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt);
				dis = sqrt((ARC->center().x-pt.x)*(ARC->center().x-pt.x)+(ARC->center().y-pt.y)*(ARC->center().y-pt.y));
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 2;
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				dis = sqrt((Circle->center().x-pt.x)*(Circle->center().x-pt.x)+(Circle->center().y-pt.y)*(Circle->center().y-pt.y));
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 2;
				}
			}
			pEnt->close();
		}	

		return   idy;
	}
	else//边
	{
		AcDbObject *pEnt;
		if(IDarr[0]!=0)
		{
			//判别是否为圆弧
			/*acdbOpenObject(pEnt,IDarr[0],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,IDarr[0], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt);
				dis = fabs(sqrt((ARC->center().x-pt.x)*(ARC->center().x-pt.x)+(ARC->center().y-pt.y)*(ARC->center().y-pt.y)) - ARC->radius());
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 0;
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				dis = fabs(sqrt((Circle->center().x-pt.x)*(Circle->center().x-pt.x)+(Circle->center().y-pt.y)*(Circle->center().y-pt.y))-Circle->radius());
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 0;
				}
			}
			pEnt->close();
		}
		if(IDarr[1]!=0)
		{
			//判别是否为圆弧
			/*acdbOpenObject(pEnt,IDarr[1],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,IDarr[1], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt);
				dis = fabs(sqrt((ARC->center().x-pt.x)*(ARC->center().x-pt.x)+(ARC->center().y-pt.y)*(ARC->center().y-pt.y)) - ARC->radius());
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 1;
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				dis = fabs(sqrt((Circle->center().x-pt.x)*(Circle->center().x-pt.x)+(Circle->center().y-pt.y)*(Circle->center().y-pt.y))-Circle->radius());
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 1;
				}
			}
			pEnt->close();
		}
		if(IDarr[2]!=0)
		{
			//判别是否为圆弧
			/*acdbOpenObject(pEnt,IDarr[1],AcDb::kForRead);*/
			if(acdbOpenObject(pEnt,IDarr[1], AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return 0;
			}
			if(pEnt->isKindOf(AcDbArc::desc()))
			{
				AcDbArc *ARC=AcDbArc::cast(pEnt);
				dis = fabs(sqrt((ARC->center().x-pt.x)*(ARC->center().x-pt.x)+(ARC->center().y-pt.y)*(ARC->center().y-pt.y)) - ARC->radius());
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 2;
				}
			}
			else if(pEnt->isKindOf(AcDbCircle::desc()))
			{
				AcDbCircle *Circle=AcDbCircle::cast(pEnt);
				dis = fabs(sqrt((Circle->center().x-pt.x)*(Circle->center().x-pt.x)+(Circle->center().y-pt.y)*(Circle->center().y-pt.y))-Circle->radius());
				if(dis<MinDis)
				{
					MinDis = dis;
					idy = 2;
				}
			}
			pEnt->close();
		}
		//计算新的圆心位置

		return idy;
	}
}

void DTMODEDLG::OnOK() 
{
	// TODO: Add extra validation here	
	CDialog::OnOK();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	for(int i=0; i<xynum; i++)
	{
		if(XY[i][0]<1.1)
			DrawXY(XY[i],xyid[i],7);
		else if(XY[i][0]>2.1)
			DrawXY(XY[i],xyid[i],6);
		else
			DrawXY(XY[i],xyid[i],2);
	}	
	if(XLNUM>1)
	{
		int i;
		for( i=0; i<xynum2; i++)
		{
			if(XY2[i][0]<1.1)
				DrawXY(XY2[i],xyid2[i],7);
			else if(XY2[i][0]>2.1)
				DrawXY(XY2[i],xyid2[i],6);
			else
				DrawXY(XY2[i],xyid2[i],2);
		}
	}

	for(int  i=0; i<xynum-1; i++)
	{
		SetXYXdata(xyid[i], xyid[i+1],XY[i]);
	}

	if(xynum > 0)
	{
		SetXYXdata(xyid[xynum-1],0,XY[xynum-1]);
		if(m_LjStart)
		{
			double xy[10],pt[2],fwj;
			for(int j=0;j<10;j++)
				xy[j] = 0;
			xy[4] = ShqDir;
			pt[0] = XY[0][8],pt[1] = XY[0][9];
			fwj = XY[0][5];
			if(IDarr[0])
			{
				/* acdbOpenObject(pEnt1,IDarr[0],AcDb::kForRead);*/
				if(acdbOpenObject(pEnt1,IDarr[0], AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pEnt1->close();
				if(pEnt1->isKindOf(AcDbLine::desc()))
				{
					AcDbLine *line;
					/*  acdbOpenObject(pEnt1,IDarr[0],AcDb::kForWrite);*/
					if(acdbOpenObject(pEnt1,IDarr[0],AcDb::kForWrite)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					line=AcDbLine::cast(pEnt1);
					AcGePoint3d ept(pt[1],pt[0],0.0);
					double sspt[2],eept[2],fwj1;
					sspt[0]=line->startPoint().y;
					sspt[1]=line->startPoint().x;
					eept[0]=line->endPoint().y;
					eept[1]=line->endPoint().x;
					xyddaa(sspt[0],sspt[1],eept[0],eept[1],&fwj1);
					if(fabs(fwj-fwj1)<0.001)
						line->setEndPoint(ept);
					else
					{
						line->setStartPoint(ept);
						AcGePoint3d pt1,pt2;
						pt1 = line->startPoint();
						pt2 = line->endPoint();
						line->setStartPoint(pt2);
						line->setEndPoint(pt1);					 
					}
					line->close();				  
				}

				SetXYXdata(IDarr[0],xyid[0],xy);
			}
			else
				SetXYXdata(IDarr[1],xyid[0],xy);
		}
		if(m_LjEnd&&XLNUM==1)
		{  
			if(IDarr[2])
			{
				/*	acdbOpenObject(pEnt1,IDarr[2],AcDb::kForRead);*/
				if(acdbOpenObject(pEnt1,IDarr[2], AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pEnt1->close();
				if(pEnt1->isKindOf(AcDbLine::desc()))
				{
					AcDbLine *line;
					/*	acdbOpenObject(pEnt1,IDarr[2],AcDb::kForWrite);*/
					if(acdbOpenObject(pEnt1,IDarr[2],AcDb::kForWrite)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					line=AcDbLine::cast(pEnt1);
					double INDATA[5],len,fwj,pt[2];
					for(int i=0; i<5; i++)
					{
						INDATA[i] = XY[xynum-1][i];
					}
					int j;
					len = Get_INDATA_L(INDATA,&j);
					CalXYXY(XY[xynum-1],len,pt[0],pt[1],fwj);
					AcGePoint3d spt(pt[1],pt[0],0.0);
					double sspt[2],eept[2],fwj1;
					sspt[0]=line->startPoint().y;
					sspt[1]=line->startPoint().x;
					eept[0]=line->endPoint().y;
					eept[1]=line->endPoint().x;
					xyddaa(sspt[0],sspt[1],eept[0],eept[1],&fwj1);	
					if(fabs(fwj-fwj1)<0.001)
						line->setStartPoint(spt);
					else
					{
						line->setEndPoint(spt);
						AcGePoint3d pt1,pt2;
						pt1 = line->startPoint();
						pt2 = line->endPoint();
						line->setStartPoint(pt2);
						line->setEndPoint(pt1);		
					}
					line->close();				  
				}

				SetXYXdata(xyid[xynum-1],IDarr[2],XY[xynum-1]);	

			}
			else
				SetXYXdata(xyid[xynum-1],IDarr[1],XY[xynum-1]);
		}
		if(XLNUM==2)
			SetXYXdata(xyid[xynum-1],IDarr[1],XY[xynum-1]);
	}

	if(XLNUM>1)
	{
		for(int i=0; i<xynum2-1; i++)
		{
			SetXYXdata(xyid2[i], xyid2[i+1],XY2[i]);
		}
		if(xynum2 > 0)
		{
			SetXYXdata(xyid2[xynum2-1],0,XY2[xynum2-1]);
			double xy[10];
			for(int j=0;j<10;j++)
				xy[j] = 0;

			if(qQXType==0&& IDarr[0]!=0 && IDarr[1]!=0 && IDarr[2]!=0)//s
				xy[4] = -ShqDir;
			else
				xy[4] = ShqDir;

			if(CirID)
				SetXYXdata(CirID,xyid2[0],xy);	
			if(m_LjEnd)
			{  
				if(IDarr[2])
				{
					/* acdbOpenObject(pEnt1,IDarr[2],AcDb::kForRead);*/
					if(acdbOpenObject(pEnt1,IDarr[2], AcDb::kForRead)!=Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return;
					}
					pEnt1->close();
					if(pEnt1->isKindOf(AcDbLine::desc()))
					{
						AcDbLine *line;
						/* acdbOpenObject(pEnt1,IDarr[2],AcDb::kForWrite);*/
						if(acdbOpenObject(pEnt1,IDarr[2],AcDb::kForWrite)!=Acad::eOk)
						{
							ads_printf(L"打开实体失败！\n");
							return;
						}
						line=AcDbLine::cast(pEnt1);
						double INDATA[5],len,fwj,pt[2];
						for(int i=0; i<5; i++)
						{
							INDATA[i] = XY[xynum-1][i];
						}
						int j;
						len = Get_INDATA_L(INDATA,&j);
						CalXYXY(XY[xynum-1],len,pt[0],pt[1],fwj);
						AcGePoint3d spt(pt[1],pt[0],0.0);
						double sspt[2],eept[2],fwj1;
						sspt[0]=line->startPoint().y;
						sspt[1]=line->startPoint().x;
						eept[0]=line->endPoint().y;
						eept[1]=line->endPoint().x;
						xyddaa(sspt[0],sspt[1],eept[0],eept[1],&fwj1);	
						if(fabs(fwj-fwj1)<0.001)
							line->setStartPoint(spt);
						else
						{
							line->setEndPoint(spt);
							AcGePoint3d pt1,pt2;
							pt1 = line->startPoint();
							pt2 = line->endPoint();
							line->setStartPoint(pt2);
							line->setEndPoint(pt1);		
						}
						line->close();				  
					}					 
					SetXYXdata(xyid2[xynum2-1],IDarr[2],XY2[xynum2-1]);
				}
			}
		}
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	if(DTBXDlg)
	{
		delete DTBXDlg;
		DTBXDlg = NULL;
	}
}

void DTMODEDLG::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void DTMODEDLG::GRDrawXY(int num,double XYarr[][10])
{ 
	ads_point pt1,pt2,Arpt1,Arpt2;
	double fwj,INDATA[5],l,len,RePt[2][2];
	int k,j;
	AcGePoint3d pt;

	for(int i=0; i<num; i++)
	{
		if(fabs(XYarr[i][0]-2.0)>0.01)
		{
			CalXYXY(XYarr[i],0.001,pt1[Y],pt1[X],fwj);
			l= 0.0;
			for(k=0;k<5;k++) INDATA[k]=XYarr[i][k];
			len=Get_INDATA_L(INDATA,&j);           
			l += 5.0;
			while(l < len)
			{
				CalXYXY(XYarr[i],l,pt2[Y],pt2[X],fwj);
				ads_grdraw(pt1,pt2,-1,0);	

				l += 5.0;
				pt1[X]=pt2[X],pt1[Y]=pt2[Y];
			}
			CalXYXY(XYarr[i],len-0.001,pt2[Y],pt2[X],fwj);
			ads_grdraw(pt1,pt2,-1,0);	                   
		}

		pt.x = XYarr[i][8];
		pt.y = XYarr[i][9];
		CalArrowPt(pt,XYarr[i][5],RePt);

		Arpt1[X] = RePt[0][1];
		Arpt1[Y] = RePt[0][0];
		Arpt2[X] = pt.y;
		Arpt2[Y] = pt.x;
		ads_grdraw(Arpt1,Arpt2,-1,0);		
		Arpt1[X] = RePt[1][1];
		Arpt1[Y] = RePt[1][0];
		ads_grdraw(Arpt1,Arpt2,-1,0);	
	}
}
