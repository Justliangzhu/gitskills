// DialogSetDirectRamp.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "DialogSetDirectRamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetDirectRamp dialog

CDialogSetDirectRamp::CDialogSetDirectRamp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetDirectRamp::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CDialogSetDirectRamp)
	m_EditChangedata = _T("25");
	m_EditRampTriMiles = _T("");
	m_EditConnectLen = _T("40");
	m_EditDRampSMiles = _T("");
	m_EditDRampEMiles = _T("");
	m_EditDRampLen = _T("");
	m_EditDRampERad = _T("");
	m_AorL = _T("A");
	m_HLinkModel = 0;
	m_dA1L1 = 1.0;
	m_dA2L2 = 1.0;
	m_dA3L3 = 1.0;
	m_dA4L4 = 1.0;
	m_strW1234 = _T("6.625 10.125 9 2.25");
	m_fldR = _T("");
	m_fldA = _T("");
	m_dOffsetW = 0.0;
	//}}AFX_DATA_INIT

	for(int i=0; i<11; i++)
	{
		XYId[i] = NULL;
	}

	W1 = 6.625;
	W2 = 10.125;
	W3 = 9.0;
	W4 = 2.25;

	A1 = 1.0;
	A2 = 1.0;
	A3 = 1.0;
	A4 = 1.0;

	m_RampPosLeftOrRight = 0;
	m_RampAddOrSub = 0;

	MainVect = 0;
	RampVect = 0;

	SelEntId = NULL;
	ProTriMiles = 0;

	ValidItemType =1;

	DRampTriX =0;
	DRampTriY = 0;
	TriSOrient = 0;
	TriRad = 0;

	ProDRampSMiles = 0;
	ProDRampEMiles = 0;

	DRampSDot[Z] = 0;
	DRampEDot[Z] = 0;
	OrignDot[Z] = 0;

	DRampSOrient = 0;
	DRampEOrient = 0;
	DROriOrient = 0;

	SCirX = 0;
	SCirY = 0;

	DRampSRad = 0;
	DRampERad = 0;
	DRampDef = 0;

	TriRampId      = NULL;
	ConnectLineID  = NULL;
	ConnectCirID   = NULL;

	W1Id = NULL;
	W2Id = NULL;
	W3Id = NULL;
	W4Id = NULL;

	DRampCirX = 0;
	DRampCirY = 0;
	DRampCirRad = 0;
	SelCircleId = NULL;

	EndElementId = NULL;
	OffsetElementId = NULL;

	ChangeValue = 0;

	m_bFitConnectLen  = false;
	m_bCirSelAgain    = true;
	m_bConnectDirect  = true;
	m_bDragConnectCir = false;
	m_bJD_CENTER      = false;

	m_iConnectCirXY = 0;

	ItemNum = 0;
	SLength =0;
	SLength1 =0;

	SprialID1 = NULL;
	SprialID2 = NULL;
	m_LinkModel = 0;
	AddCirId = NULL;

	RampJD_CENTER = NULL;
	RampJD_CENTERID = NULL;
	pGottnRamp=NULL;
}

void CDialogSetDirectRamp::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CDialogSetDirectRamp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDialogSetDirectRamp)
	DDX_Text(pDX, IDC_EDITCHANGINGDATA, m_EditChangedata);
	DDX_Text(pDX, IDC_EDITTRIMILES, m_EditRampTriMiles);
	DDX_Text(pDX, IDC_EDITCHANGINGLEN, m_EditConnectLen);
	DDX_Text(pDX, IDC_EDITDRAMPSTARTMILES, m_EditDRampSMiles);
	DDX_Text(pDX, IDC_EDITDRAMPENDMILES, m_EditDRampEMiles);
	DDX_Text(pDX, IDC_EDITDRAMPLEN, m_EditDRampLen);
	DDX_Text(pDX, IDC_EDITENDRAD, m_EditDRampERad);
	DDX_CBString(pDX, IDC_AL, m_AorL);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_HLinkModel);
	DDX_Text(pDX, IDC_EDITSPRIALA1, m_dA1L1);
	DDX_Text(pDX, IDC_EDITSPRIALA2, m_dA2L2);
	DDX_Text(pDX, IDC_EDITSPRIALA3, m_dA3L3);
	DDX_Text(pDX, IDC_EDITSPRIALA4, m_dA4L4);
	DDX_CBString(pDX, IDC_COMBO_W1234, m_strW1234);
	DDX_Text(pDX, IDC_EDIT_FLD_R, m_fldR);
	DDX_Text(pDX, IDC_EDIT_FLD_A, m_fldA);
	DDX_Text(pDX, IDC_EDIT_OffsetW, m_dOffsetW);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogSetDirectRamp, CDialog)
	//{{AFX_MSG_MAP(CDialogSetDirectRamp)
	ON_BN_CLICKED(IDC_BUTTONTOKNOWMEANING, OnButtontoknowmeaning)
	ON_BN_CLICKED(IDC_BUTTONGETENDRAD, OnButtongetendrad)
	ON_BN_CLICKED(IDC_BUTTONUPDATEDRAMP, OnButtonupdatedramp)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_CBN_SELCHANGE(IDC_AL, OnSelchangeAl)
	ON_CBN_SELENDOK(IDC_AL, OnSelendokAl)
	ON_BN_CLICKED(IDC_BUTTON_IMPLY, OnButtonImply)
	ON_BN_CLICKED(IDC_CHECK_ConnectDirect, OnCHECKConnectDirect)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CDialogSetDirectRamp message handlers

// �Ի����ʼ��
BOOL CDialogSetDirectRamp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// 1. ���û�ѡȡ���ߵ�Ԫ
	if(!SelectMainElement())
	{
		DestroyWindow();
		return FALSE;
	} 
	m_iSelElem = 1;

	// 2. ���û�ѡȡ���ӵ�Ԫ(Բ��Բ��)
	if(!SelectConnectElement())
	{
		DestroyWindow();
		return FALSE;
	}
	m_iSelElem = 2;

	// 3. ���û�ѡȡ��ֹ��Ԫ(��·ʵ�塢ֱ�ߡ�Բ��Բ��)
	if(SelectEndElement())
	{
		m_iSelElem = 3;
	}
	else
	{
		// ��������ֹ���Ƶ�Ԫ��صĲ�������
		GetDlgItem(IDC_STATICSPRIALA3)->EnableWindow(FALSE); 
		GetDlgItem(IDC_EDITSPRIALA3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(FALSE); 
		GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_C)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OffsetW)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_CHECK_ConnectDirect)->EnableWindow(FALSE);
	}

	// 4. �������ý���Ĭ�ϲ���	
	if(SelEntId && SelCircleId) // ��ȷѡ��������������Բ
	{
		GetCenterSideAndLinkDef(); // ȷ������ƫ��,���������ζ���ʼ��Ԫ(���Ӷ�)
	}
	else
	{
		AfxMessageBox(L"���߻�����Բʵ��ID����!\n");
		DestroyWindow();
		return FALSE;
	}

	// 5.��Ĭ��ֵ��ʼ��Ʒ���(Ŀǰֻ��2��Ԫ��ʼ��,��������3��Ԫ��ʼ��)
	if(m_iSelElem == 2)
	{
		ComputerDRampParameters();
	}

	CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_ConnectDirect);
	pBtn->SetCheck(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// "�϶�"��ť��Ϣ
void CDialogSetDirectRamp::OnButtongetendrad()
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE); // ���ضԻ���
	m_bDragConnectCir = true;

	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		ShowWindow(TRUE);
		SetFocus();
		return ;
	}

	DragConnectCir(); // ��̬�϶�����Բ

	acDocManager->unlockDocument(acDocManager->curDocument());	
	UpdateData(FALSE);
	ShowWindow(TRUE);
	m_bDragConnectCir = false;
}

// ���ݶԻ��������������Ϣ���㽥��γ������ٳ�������
void CDialogSetDirectRamp::ComputerDRampParameters()
{
	int i, j, k;

	// 1.�������ζ�
	// �ж�����ԲԲ��λ�ڱ��ٳ����Ĳ���ԲԲ������ƫ��(˳ʱ�뻹����ʱ��)
	// �Ӽ������ʡ�����ԲԲ���������·ǰ������Ĳ���
	// ���㽥����������, ���ٳ���������ꡢ�뾶����λ��(����ƫת��)
	if(SelEntId && SelCircleId) // ��ȷѡ��������������Բ
	{
		if(!GetCenterSideAndLinkDef())
		{
			return;
		}
	}
	else
	{
		AfxMessageBox(L"���߻�����Բʵ��ID����!\n");
		return;
	}

	//	// ����ֱ��ʽ���ٳ���,���������ӻ������߶�
	//	if(m_bFitConnectLen)
	//	{
	//		if(!CalDRampFitConnectLen()) // �̶����Ӷγ���Aֵ
	//		{
	//			return;
	//		}
	//	}
	//	else
	//	{
	//		if(!CalDRampFitCirCenter()) // �̶�����ԲԲ��
	//		{
	//			return;
	//		}
	//	}

	// 2.ģʽ���������ӵ�Ԫ
	double XYArrayTmp[5][10];
	int iXYNum;
	double A[4];
	A[0] = A1;
	A[1] = A2;
	A[2] = A3;
	A[3] = A4;

	if(m_iSelElem == 2) // 2��Ԫģʽ������
	{
		if(m_bFitConnectLen) // �̶����Ӷγ���Aֵ
		{
			// ���㻺������������꼰��λ��
			if(fabs(ValidItemType-1) < DataPrecision) // ֱ��
			{	
				m_LinkModel = -1;

				OrignDot[X] = DRampSDot[X] + _wtof(m_EditConnectLen)*sin(DRampSOrient);
				OrignDot[Y] = DRampSDot[Y] + _wtof(m_EditConnectLen)*cos(DRampSOrient);
				DROriOrient = DRampSOrient;
			}
			else
			{	
				AcGePoint3d CenterTmp;
				GetOtherParameterOfArc(DRampSDot[X],DRampSDot[Y],DRampSOrient,DRampSRad,_wtof(m_EditConnectLen),
					DRampDef,CenterTmp[X],CenterTmp[Y],OrignDot[X],OrignDot[Y]);
				DROriOrient = DRampSOrient + DRampDef*_wtof(m_EditConnectLen)/DRampSRad;
			}
			if(!Msf2ElemFitA(StartElementId,SelCircleId,OrignDot,DROriOrient,m_LinkModel,DRampDef,A,XYArrayTmp,iXYNum))
			{
				return;
			}
		}
		else
		{
			if(!Msf2Elem(StartElementId,SelCircleId,m_LinkModel,DRampDef,A,XYArrayTmp,iXYNum))
			{
				return;
			}
		}
	}
	else if(m_iSelElem == 3) // 3��Ԫģʽ������
	{
		// �������ƫ��,��ֱ������ֹ���Ƶ�Ԫ����,�Ա�����ֹ��Ԫֱ������
		// ���������ֹ���Ƶ�Ԫ��JD_CENTER,�����ƫ��,����������Ԫ����
		AcDbObjectId IdTmp; 
		if(m_bConnectDirect)  
		{
			IdTmp = EndElementId;
		}
		else
		{
			IdTmp = OffsetElementId;
		}
		if(!Msf3Elem(StartElementId,SelCircleId,IdTmp,m_LinkModel,m_HLinkModel,DRampDef,A,XYArrayTmp,iXYNum))
		{
			return;
		}
	}

	// 4.��֯��Ԫ,����Ƕ˲����Ʊ�־��W3,W4
	if(!CreateRampJD_CENTER(XYArrayTmp,iXYNum))
	{
		return;
	}
	pGottnRamp->CalNoseCen(RampJD_CENTER,ProDRampSMiles,(int)flag,W3,W4,m_dNoseCmlOnZX,m_dNoseCmlOnYX,m_NosePt);
	AcGePoint3d NosePt(m_NosePt[1],m_NosePt[0],0.0);

	xlpoint NoseOnZXxlpoint, NoseOnYXxlpoint; // �Ƕ�Բ�������ߺ������ϵ�ͶӰ��
	NoseOnZXxlpoint.lc = m_dNoseCmlOnZX;
	ProDRampEMiles     = m_dNoseCmlOnZX;
	pGottnRamp->xlpoint_pz(&NoseOnZXxlpoint);
	AcGePoint3d NosePtOnZX(NoseOnZXxlpoint.y, NoseOnZXxlpoint.x, 0.0);
	NoseOnYXxlpoint.lc = m_dNoseCmlOnYX;
	RampJD_CENTER->xlpoint_pz(&NoseOnYXxlpoint);
	AcGePoint3d NosePtOnYX(NoseOnYXxlpoint.y, NoseOnYXxlpoint.x, 0.0);

	makeline(NosePtOnZX,NosePt,W3Id,1);
	makeline(NosePtOnYX,NosePt,W4Id,1);

	// ���±��ٳ����յ���̺ͳ���
	m_EditDRampEMiles.Format(L"%lf",ProDRampEMiles);
	m_EditDRampLen.Format(L"%lf",fabs(NoseOnYXxlpoint.lc));

	// ���·�����R,A
	int ItemIndex = RampJD_CENTER->GetXYNoFromTYLC(NoseOnYXxlpoint.lc); // ���ٳ�������Ӧ���ߵ���Ԫ��
	double dType = RampJD_CENTER->XYArray[ItemIndex][0]; // ��ȡ������Ԫ������

	if(fabs(dType-1.0) < DataPrecision)
	{
		m_fldR.Format(L"�����");
	}
	else
	{
		m_fldR.Format(L"%lf",NoseOnYXxlpoint.r);
	}
	if(dType > 2)
	{
		m_fldA.Format(L"%lf",RampJD_CENTER->XYArray[ItemIndex][1]);
	}
	else
	{
		m_fldA.Format(L"��");
	}

	// ���A1,A2,A3,A4
	if(m_LinkModel==-1 || m_LinkModel==2)
	{
		A2 = RouteItemArray[2][1];
		if(m_iSelElem == 3)
		{
			m_iConnectCirXY = 3;
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				A3 = RouteItemArray[4][1];
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(FALSE);
			}
			else
			{
				A3 = RouteItemArray[4][1];
				A4 = RouteItemArray[5][1];
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		A1 = RouteItemArray[2][1];
		A2 = RouteItemArray[3][1];
		if(m_iSelElem == 3)
		{
			m_iConnectCirXY = 4;
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				A3 = RouteItemArray[5][1];
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(FALSE);
			}
			else
			{
				A3 = RouteItemArray[5][1];
				A4 = RouteItemArray[6][1]; 
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(TRUE);
			}
		}
	}
	PutA1A2(); // ��ͳһ��A1A2ֵ��������������

	//////////////////////////////////////////////////////////////////////////
	// ������Ԫ(GrDraw��)

	double len, L, fwj, INDATA[5];
	ads_point pt1, pt2;

	// ���ػ�(���)RouteItemArrayOld,ɾ��ԭ��������Ԫ
	for(i=1; i<m_iXYMax; i++)
	{
		//	if(!m_bDragConnectCir && i>= m_iConnectCirXY)
		//	{
		//		break;
		//	}

		if(RouteItemArrayOld[i][0] > 0.01) // ֻ����Ч����Ԫ,���յ㲻��
		{
			/*
			CalXYXY(RouteItemArrayOld[i],1,pt1[Y],pt1[X],fwj);
			len = 0.0;
			for(k=0; k<5; k++) 
			{
			INDATA[k] = RouteItemArrayOld[i][k];
			}
			L = Get_INDATA_L(INDATA,&j);           
			len += 5.0;
			while(len < L)
			{
			CalXYXY(RouteItemArrayOld[i],len,pt2[Y],pt2[X],fwj);
			acedGrDraw(pt1,pt2,-1,0);	
			len += 5.0;
			pt1[X] = pt2[X];
			pt1[Y] = pt2[Y];
			}
			CalXYXY(RouteItemArrayOld[i],L-0.01,pt2[Y],pt2[X],fwj);
			acedGrDraw(pt1,pt2,-1,0);

			AcGePoint3d pt(pt2[Y],pt2[X],0.0);
			double RePt[2][2];
			CalArrowPt(pt,fwj,RePt);
			pt1[X] = RePt[0][1];
			pt1[Y] = RePt[0][0];
			ads_grdraw(pt1,pt2,-1,0);	
			pt1[X] = RePt[1][1];
			pt1[Y] = RePt[1][0];
			ads_grdraw(pt1,pt2,-1,0);*/
			acedRedraw(NULL, 0);
		}
	}

	// �����µ�RouteItemArray
	for(i=1; i<m_iXYMax; i++)
	{
		if(RouteItemArray[i][0] > 0.01) // ֻ����Ч����Ԫ,���յ㲻��
		{
			CalXYXY(RouteItemArray[i],1,pt1[Y],pt1[X],fwj);
			len = 0.0;
			for(k=0; k<5; k++) 
			{
				INDATA[k] = RouteItemArray[i][k];
			}
			L = Get_INDATA_L(INDATA,&j);           
			len += 5.0;
			while(len < L)
			{
				CalXYXY(RouteItemArray[i],len,pt2[Y],pt2[X],fwj);
				acedGrDraw(pt1,pt2,7,0);	
				len += 5.0;
				pt1[X] = pt2[X];
				pt1[Y] = pt2[Y];
			}
			CalXYXY(RouteItemArray[i],L-0.01,pt2[Y],pt2[X],fwj);
			acedGrDraw(pt1,pt2,7,0);

			AcGePoint3d pt(pt2[Y],pt2[X],0.0);
			double RePt[2][2];
			CalArrowPt(pt,fwj,RePt);
			pt1[X] = RePt[0][1];
			pt1[Y] = RePt[0][0];
			ads_grdraw(pt1,pt2,7,0);	
			pt1[X] = RePt[1][1];
			pt1[Y] = RePt[1][0];
			ads_grdraw(pt1,pt2,7,0);

		}
	}

	for(i=1; i<m_iXYMax; i++)
	{
		for(j=0; j<10; j++) 
		{
			RouteItemArrayOld[i][j] = RouteItemArray[i][j];
		}
	}

	UpdateData(FALSE);
}

// ��Ӧȷ����ť�����
void CDialogSetDirectRamp::OnOK() 
{
	// TODO: Add extra validation here
	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return ;
	}

	//	RampJD_CENTERID = AddEntityToDbs(RampJD_CENTER);

	// д����չ����(���յ㲻д����չ����)
	for(int i=1; i<m_iXYMax; i++) 
	{
		SetXYXdata(XYId[i], XYId[i+1], RouteItemArray[i]);
		if(i==m_iXYMax-1 && m_bConnectDirect) // ������ֹ���Ƶ�Ԫ
		{
			SetXYXdata(XYId[i], XYId[0], RouteItemArray[i]);
		}
	}

	/*	AcDb2dPolyline * Poly2x = NULL;
	if (SprialID1)
	{
	if (acdbOpenObject(Poly2x,SprialID1,AcDb::kForWrite) ==  Acad::eOk) 
	{
	Poly2x->erase();		
	Poly2x->close();
	}
	}
	if (SprialID2)
	{
	if (acdbOpenObject(Poly2x,SprialID2,AcDb::kForWrite) ==  Acad::eOk) 
	{
	Poly2x->erase();		
	Poly2x->close();
	}
	}
	*/

	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());

	CDialog::OnOK();
}

// ��Ӧȡ����ť�����
void CDialogSetDirectRamp::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

// ��Ӧ������ť�����
void CDialogSetDirectRamp::OnButtontoknowmeaning() 
{
	// TODO: Add your control notification handler code here
}

// ��̬����ֱ��ʽ���ٳ���
void CDialogSetDirectRamp::OnButtonupdatedramp() 
{
	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return ;
	}

	if(!GetParameter()) // ��ý�����������������������
	{
		return;
	}

	int MouseType;             // ����������
	struct resbuf ClickResult; // �������������
	short KeyCode;             // ����ؼ������

	for( ; ; ) // ��̬���ѭ��
	{	
		// ��������϶����Ʋ����и���;
		if(ads_grread(1,&MouseType,&ClickResult) != RTNORM) 			
		{
			break;
		}
		KeyCode = ClickResult.resval.rint;
		if(KeyCode==13 || KeyCode==32 || MouseType==25) 
		{
			break;
		}
		if(MouseType == 3) // fabs(MouseType-3) < DataPrecision
		{
			break; // �������˳���̬���ѭ��(ȷ����Ƴɹ�)
		}
		else if(MouseType == 5) // ��������ƶ�
		{	
			PickDotOld = PickDotNew;
			PickDotNew.x = ClickResult.resval.rpoint[X];
			PickDotNew.y = ClickResult.resval.rpoint[Y];
			PickDotNew.z = ClickResult.resval.rpoint[Z];	
			if(PickDotOld.distanceTo(PickDotNew) > 0.01)
			{	
				//				AcGePoint3d  centerPt(DRampCirX,DRampCirY,0.0);
				//				SetConnectCirCoord(centerPt); // ������������ԲԲ������

				ProTriMiles = pGottnRamp->PROJ_ML(PickDotNew.y,PickDotNew.x); // �����Ӧ��·��ͳһ���	
				ComputerDRampParameters(); // ���㽥��μ����ٳ�������,������ͼ��
			}
		}
		else
		{
			acutPrintf(L"MouseType: %d\n", MouseType);
			break; // ���������������,�˳���̬���ѭ��
		}
	}

	Draw(); // ���������· 

	UpdateData(FALSE);
	acDocManager->unlockDocument(acDocManager->curDocument());
}	

// ���㻺�����ϵ�ָ��ֱ�����ΪD�ĵ�
void CDialogSetDirectRamp::GetEndPointOfDRamp()
{
	//	UpdateData(TRUE);
	//	if (fabs(ValidItemType-1) <  DataPrecision)
	//	{	
	//		ItemNum = 2;
	//		// ���㻺����ƫ��(���ٳ����յ�ƫ����ڻ��������ƫ��)
	//		double DistOfTriToDS = GetPreciseLength(DRampTriX,DRampTriY,DRampSDot[X],DRampSDot[Y]);
	//		double DistOfDSToSO = GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]);
	//		double DistOfToAndSo = _wtof(m_EditDRampSDotOffset)-_wtof(m_EditRampTriOffset);
	//		double OffsetOfSprS = (DistOfTriToDS+DistOfDSToSO)*DistOfToAndSo/DistOfTriToDS+_wtof(m_EditRampTriOffset);
	//		double Dist = fabs(_wtof(m_EditDRampEDotOffset)-OffsetOfSprS);
	//		// ��ȡƫת�ǵĴ�С
	//		double DefAngle = ChangeValue;		
	//		// ����ѭ����û������ϵĺ��ʵ�
	//		double ValueA = _wtof(m_EditParaValueA1);
	//		double LenLimS = Dist;
	//		double LenLimE = 1000;
	//		SLength = (LenLimS+LenLimE)/2;
	//		double AbsX=0, AbsY=0, AbsDist=0;
	//		BOOL ContinueOrnot = TRUE;
	//		int i=0;
	//		while (ContinueOrnot)
	//		{
	//			// ����������ȴ��Ļ����㵽ֱ�ߵľ���
	//			AbsX = GetAbsXCorOfSprialCir(ValueA,SLength);
	//			AbsY = GetAbsYCorOfSprialCir(ValueA,SLength);
	//			AbsDist = fabs(AbsY*cos(DefAngle)+AbsX*sin(DefAngle));
	//			// �жϼ��������Ҫ�����;
	//			if (fabs(AbsDist-Dist) > DataPrecision) 
	//			{
	//				if (AbsDist > Dist) 
	//				{
	//					LenLimE = SLength;
	//				}
	//				else
	//				{
	//					LenLimS = SLength;
	//				}
	//				SLength = (LenLimS+LenLimE)/2;
	//			}
	//			else
	//			{
	//				ContinueOrnot = FALSE;
	//			}
	//		}
	//		double OrignAngle = ConvertOrientToAngle(TriSOrient)+(CCLineDef>0?0:1)*PI;
	//		double InterX,InterY;
	//		InterX = GetAbsXCorOfSprialCir(ValueA,SLength);
	//		InterY = GetAbsYCorOfSprialCir(ValueA,SLength);
	//		DRampEDot[Y] = OrignDot[Y]+InterX*sin(OrignAngle)-CCLineDef*((m_RampPosLeftOrRight)==0?-1:1)*InterY*cos(OrignAngle);
	//		DRampEDot[X] = OrignDot[X]+InterX*cos(OrignAngle)+CCLineDef*((m_RampPosLeftOrRight)==0?-1:1)*InterY*sin(OrignAngle);		
	//		m_EditDRampLen.Format(L"%lf",
	//			SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));	
	//		UpdateData(FALSE);
	//	}
	//	else
	//	{
	//		//double SLength = 0;
	//		double CX1,CY1,X2,Y2;
	//		GetOtherParameterOfArc(TriMainX,TriMainY,TriMainOA,TriMainRad,100,DRampDef,CX1,CY1,X2,Y2);
	//		if (m_LinkModel == 2) //LUAN_MODEL
	//		{	
	//			GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,OrignDot[X],OrignDot[Y],DRampSRad,DROriOrient,
	//				DRampERad,_wtof(m_EditParaValueA1),DRampDef,CCLineDef,_wtof(m_EditDRampEDotOffset),2,DRampEDot[X],DRampEDot[Y],SLength);
	//			m_EditDRampLen.Format(L"%lf",
	//				SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//		}
	//		else if	(m_LinkModel == 0) //S_MODEL
	//		{
	//			// �ж�������������
	//			ads_point InputePoint;
	//			InputePoint[X] = FirstSprialEnd[X];
	//			InputePoint[Y] = FirstSprialEnd[Y];
	//			double SelItemType,InterMiles;
	//			xlpoint RPoint;
	//			ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	//			double InterW3 = GetPreciseLength(RPoint.y,RPoint.x,FirstSprialEnd[X],FirstSprialEnd[Y]);
	//			//ads_printf(L"%lf,%lf\n",InterW3,_wtof(m_EditDRampEDotOffset));
	//			if (InterW3 < _wtof(m_EditDRampEDotOffset)) 
	//			{
	//				ItemNum = 2;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,FirstSprialEnd[X],FirstSprialEnd[Y],1E50,FirstSprialEOA,
	//					_wtof(m_EditDRampERad),_wtof(m_EditParaValueA2),-DRampDef,CCLineDef,-_wtof(m_EditDRampEDotOffset),0,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen+SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}
	//			else
	//			{
	//				ItemNum = 1;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,OrignDot[X],OrignDot[Y],DRampSRad,DROriOrient,
	//					1E50,_wtof(m_EditParaValueA1),DRampDef,CCLineDef,-_wtof(m_EditDRampEDotOffset),0,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen-SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}			
	//		}
	//		else
	//		{
	//			// �ж�������������
	//			ads_point InputePoint;
	//			InputePoint[X] = FirstSprialEnd[X];
	//			InputePoint[Y] = FirstSprialEnd[Y];
	//			double SelItemType,InterMiles;
	//			xlpoint RPoint;
	//			ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	//			double InterW3 = GetPreciseLength(RPoint.y,RPoint.x,FirstSprialEnd[X],FirstSprialEnd[Y]);
	//			
	//			InputePoint[X] = SecondSprialEnd[X];
	//			InputePoint[Y] = SecondSprialEnd[Y];
	//			ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	//			double InterW4 = GetPreciseLength(RPoint.y,RPoint.x,SecondSprialEnd[X],SecondSprialEnd[Y]);
	//			if (InterW3 < _wtof(m_EditDRampEDotOffset)) 
	//			{
	//				ItemNum = 2;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,FirstSprialEnd[X],FirstSprialEnd[Y],1E50,FirstSprialEOA,
	//					_wtof(m_EditDRampERad),_wtof(m_EditParaValueA2),DRampDef,CCLineDef,_wtof(m_EditDRampEDotOffset),1,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen+SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}
	//			else
	//			{
	//				ItemNum = 1;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,OrignDot[X],OrignDot[Y],DRampSRad,DROriOrient,
	//					1E50,_wtof(m_EditParaValueA1),DRampDef,CCLineDef,_wtof(m_EditDRampEDotOffset),1,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen-SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}			
	//		}
	//		UpdateData(FALSE);
	//	}

}

// ���ݱ��ٳ�����������Բ�뾶��ϵ���Զ�ȷ�����ӷ�ʽ
void CDialogSetDirectRamp::AutoModel()
{	
	double DistO1O2, fwj;
	DistO1O2 = BAS_DRAW_FUN::xyddaa(SCirY, SCirX, DRampCirY, DRampCirX, &fwj);
	double R1 = DRampSRad;
	double R2 = DRampERad;

	if(DistO1O2 < 0.0001)
	{
		AfxMessageBox(L"��Բͬ��!");
	}
	else if(DistO1O2 < fabs(R1-R2)) 
	{
		m_LinkModel = 2; // ��
	}
	else if(DistO1O2 < (R1+R2))
	{
		m_LinkModel = 1; // C
	}
	else if(DistO1O2 > (R1+R2))
	{
		m_LinkModel = 0; // S
	}
}

//////////////////////////////////////////////////////////////////////////
// �ж�����ԲԲ��λ�ڱ��ٳ����Ĳ���ԲԲ������ƫ��(˳ʱ�뻹����ʱ��)
// �Ӽ������ʡ�����ԲԲ���������·ǰ������Ĳ���
// ���㽥����������, ���ٳ���������ꡢ�뾶����λ��(����ƫת��)
// 2005.07.01 ldf
//////////////////////////////////////////////////////////////////////////
bool CDialogSetDirectRamp::GetCenterSideAndLinkDef()
{		
	// 1.���㽥��γ��ȼ�ƫת��
	double VerDist = 0;
	VerDist = fabs(W2 - W1);
	m_dTriLen = VerDist * _wtof(m_EditChangedata);

	if(_wtof(m_EditChangedata) < DataPrecision) // �ж������Ƿ�������
	{
		acutPrintf(L"����:������Ϊ��!\n");
		return false;		
	}
	ChangeValue = atan(1/_wtof(m_EditChangedata)); // ����ƫת��

	// 2.ȷ������ƫ�򡢶�Ӧ�뾶����λ��
	// �������(��������)��������ͶӰ��
	xlpoint RampTrixlpoint; 
	RampTrixlpoint.lc = ProTriMiles;
	m_EditRampTriMiles.Format(L"%lf", ProTriMiles);
	pGottnRamp->xlpoint_pz(&RampTrixlpoint);
	TriMainX   = RampTrixlpoint.y;
	TriMainY   = RampTrixlpoint.x;
	TriMainRad = RampTrixlpoint.r;
	TriMainOA  = RampTrixlpoint.a;
	AcGePoint3d RampTriProjPt(RampTrixlpoint.y, RampTrixlpoint.x, 0.0);

	//	if(!GetConnectCirCoordRad()) // ʵʱ�������ԲԲ������Ͱ뾶
	//	{
	//		return false;
	//	}


	if(!m_bFitConnectLen || m_bCirSelAgain) // ������Ӷγ��̶�,����Ӧ��ƫ����̶�,�����ټ���
	{
		m_bCirSelAgain = false;

		// ��������ԲԲ���������ϵ�ͶӰ��
		xlpoint CirCenxlpoint; 
		CirCenxlpoint.lc = pGottnRamp->PROJ_ML(DRampCirY, DRampCirX); 
		ProDRampCirMiles = CirCenxlpoint.lc;
		pGottnRamp->xlpoint_pz(&CirCenxlpoint);
		AcGePoint3d CirCenProjPt(CirCenxlpoint.y, CirCenxlpoint.x, 0.0); 

		// ����ԲԲ�ĵ�
		AcGePoint3d CirCenPt(DRampCirX, DRampCirY, 0.0);   

		// �ж�����ԲԲ��λ�ڱ��ٳ����Ĳ���ԲԲ������ƫ��(˳ʱ�뻹����ʱ��)���Ӽ�������
		CCLineDef = getLRRot(RampTriProjPt, CirCenProjPt, CirCenPt);

		m_RampAddOrSub = -1.0 * CCLineDef; // �Ҳ����,������

		// �ж�����ԲԲ���������·ǰ������Ĳ���
		if(CirCenxlpoint.lc > RampTrixlpoint.lc)
		{
			flag = 1.0; // ���ٳ���������ͬ��
			m_RampPosLeftOrRight = CCLineDef;	
		}
		else
		{
			flag = -1.0; // ���ٳ��������߷���
			m_RampPosLeftOrRight = -1.0 * CCLineDef;
		}
	}

	// ������ٳ��������������ͶӰ����Ϣ
	xlpoint DRampSxlpoint; // ���ٳ��������������ͶӰ��
	if(fabs(flag-1.0) < DataPrecision) // ���ٳ���������ͬ��
	{	
		DRampSxlpoint.lc = ProTriMiles + m_dTriLen;
	}
	else
	{
		DRampSxlpoint.lc = ProTriMiles - m_dTriLen;
		if(DRampSxlpoint.lc < 0.0)
		{
			ads_printf(L"###: ���ٳ�������޷�����,�������ز���!\n");
			return false;
		}
	}
	ProDRampSMiles = DRampSxlpoint.lc;
	m_EditDRampSMiles.Format(L"%lf", DRampSxlpoint.lc); // ��ʾ���ٳ�����ʼ׮��
	pGottnRamp->xlpoint_pz(&DRampSxlpoint);
	DRampSMainX   = DRampSxlpoint.y;
	DRampSMainY   = DRampSxlpoint.x;
	DRampSMainRad = DRampSxlpoint.r;
	DRampSMainOA  = DRampSxlpoint.a;
	int ItemIndex = pGottnRamp->GetXYNoFromTYLC(DRampSxlpoint.lc); // ���ٳ�������Ӧ��·����Ԫ��
	ValidItemType = pGottnRamp->XYArray[ItemIndex][0]; // ��ȡ��ѡ��Ԫ������
	MainValueA    = pGottnRamp->XYArray[ItemIndex][1];

	// ������ٳ���׼ȷ��ƫ��(��/��), �����ķ�λ��(����ƫת��)
	DRampDef = pGottnRamp->XYArray[ItemIndex][4]; // ��Ӧ���ߵ�Ԫ��ƫ��
	if(fabs(ValidItemType-1) < DataPrecision) // ���ߵ�ԪΪֱ��ʱ,ԭ��ƫ��ֵΪ0,����Ҫ���⴦�� 
	{
		DRampDef = m_RampPosLeftOrRight;
	}

	DRampSOrient = DRampSMainOA;
	if(flag < 0.0) // ���ٳ��������߷���ʱ,��ƫ��ͷ�λ�����跴��
	{
		DRampSOrient = fmod(DRampSOrient+PI, 2*PI); 
		DRampDef     = -1.0 * DRampDef; 
	}
	DRampSOrient = fmod(DRampSOrient+CCLineDef*ChangeValue, 2*PI);

	// 3.������Ƶ㲢���Ʊ�־��
	// ���㽥����������(���ݶ�Ӧ������ͶӰ�������Ϣ��W1),
	DRampTriX = TriMainX + m_RampPosLeftOrRight * W1 * sin(TriMainOA+PI/2);
	DRampTriY = TriMainY + m_RampPosLeftOrRight * W1 * cos(TriMainOA+PI/2);	

	// ������ٳ����������(���ݶ�Ӧ������ͶӰ�������Ϣ��W2)
	DRampSDot[X] = DRampSMainX + m_RampPosLeftOrRight * W2 * sin(DRampSMainOA+PI/2);
	DRampSDot[Y] = DRampSMainY + m_RampPosLeftOrRight * W2 * cos(DRampSMainOA+PI/2);	
	DRampSDot[Z] = 0;

	// ���ƽ������㡢���ٳ�������־ֱ��(|)
	makeline(RampTriProjPt,AcGePoint3d(DRampTriX,DRampTriY,0),W1Id,1);
	makeline(AcGePoint3d(DRampSMainX,DRampSMainY,0),DRampSDot,W2Id,1);

	// ���ƽ��������(������Խ���)
	AcGePoint2dArray vertices;
	AcGePoint2d Dot;
	int ArraySize = vertices.length();
	if(ArraySize > 0) // ��ն�������
	{
		vertices.removeSubArray(0, ArraySize-1);
	}
	xlpoint xlpointTmp;
	for(xlpointTmp.lc = RampTrixlpoint.lc; fabs(xlpointTmp.lc-DRampSxlpoint.lc)>2.0; xlpointTmp.lc+=flag*2.0)
	{
		double len = fabs(xlpointTmp.lc-RampTrixlpoint.lc);
		double offset = W1 + VerDist*len/m_dTriLen;
		pGottnRamp->xlpoint_pz(&xlpointTmp); // ����ͶӰ��
		Dot[X] = xlpointTmp.y + m_RampPosLeftOrRight*offset*sin(xlpointTmp.a+PI/2);
		Dot[Y] = xlpointTmp.x + m_RampPosLeftOrRight*offset*cos(xlpointTmp.a+PI/2);	
		vertices.append(Dot);
	} // ����ѭ���ṹ��ʼ����������	
	Dot.set(DRampSDot[X],DRampSDot[Y]); // �����һ�����������
	vertices.append(Dot);
	makepolyline(vertices, TriRampId, 2); // ��ɫ

	// 4.������ʼ���Ƶ�Ԫ��ȷ������ģʽ�����¶Ի���ѡ��
	if(fabs(ValidItemType-1) < DataPrecision) // ֱ�� 
	{
		DRampSRad = -1; 

		makeline(DRampSDot,DRampSOrient,0.1,ConnectLineID,0); 
		StartElementId = ConnectLineID;
		m_LinkModel = -1;

		GetDlgItem(IDC_STATICSPRIALA1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITSPRIALA1)->EnableWindow(FALSE);
	}
	else // Բ�򻺺�����
	{
		DRampSRad = DRampSMainRad; // �����߱�������һ��
		// ���ٳ�����㴦��Բ������
		SCirX = DRampSDot[X] + DRampSRad*sin(DRampSOrient + DRampDef*PI/2);
		SCirY = DRampSDot[Y] + DRampSRad*cos(DRampSOrient + DRampDef*PI/2);

		makearc(DRampSDot,DRampSOrient,DRampSRad,0.1,DRampDef,ConnectCirID,0);
		StartElementId = ConnectCirID;
		AutoModel(); // ���ݱ��ٳ������Բ������Բ�뾶��ϵ���Զ�ȷ�����ӷ�ʽ

		GetDlgItem(IDC_STATICSPRIALA1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDITSPRIALA1)->EnableWindow(TRUE);

		if(!m_bFitConnectLen) // ������̶����Ӷγ�,���Զ���ʼAֵ
		{
			// ���ݰ뾶�ı�ֵ����ʼA1��A2
			//			double ScaleValue = DRampSRad / DRampCirRad;
			//			if(ScaleValue < 2.0) 
			//			{
			A1 = 1.0;
			A2 = 1.0;
			PutA1A2(); // ��ͳһ��A1A2ֵ��������������
			//			}
			//			else
			//			{
			//				m_EditParaValueA1.Format(L"%lf", ScaleValue);
			//				m_EditParaValueA2 = "1";
			//			}
		}
	}	

	UpdateData(FALSE);

	return true;
}

// Բ�������㼰Բ���������������Բ������
double CDialogSetDirectRamp::GetArcLength(double CenX,double CenY,double ArcX1,double ArcY1,
										  double ArcX2,double ArcY2,double ArcRad)
{
	double Angle1 = GetAngleOfLineAndXaxes(CenX,CenY,ArcX1,ArcY1); //S
	double Angle2 = GetAngleOfLineAndXaxes(CenX,CenY,ArcX2,ArcY2); //E
	double Angle = 0;
	if (Angle1 > Angle2) 
	{
		Angle = 2*PI-Angle1+Angle2;
	}
	else
	{
		Angle = Angle2-Angle1;
	}

	return  Angle*ArcRad;
}

// ѡȡ���ߵ�Ԫ
bool CDialogSetDirectRamp::SelectMainElement()
{
	ads_name   EntityName; // ��ѡʵ������
	ads_point  BasePoint;  // ��ѡʵ�����

	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return false;
	}

	// ��ʾ�û�ѡ������
	if(ads_entsel(L"\n��ѡȡ����:\n",EntityName,BasePoint) != RTNORM) 
	{
		SelEntId = NULL;
		acutPrintf(L"����ѡȡʧ��!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		DestroyWindow();
		return false;
	}
	if(acdbGetObjectId(SelEntId,EntityName) != Acad::eOk)
	{
		SelEntId = NULL;
		acutPrintf(L"��ȡ����ʵ��IDʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	GetMainElement(); // ������ߵ�Ԫָ��

	ProTriMiles = pGottnRamp->PROJ_ML(BasePoint[Y], BasePoint[X]); // ��ȡ�����Ӧ��ͳһ���

	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

// ѡȡ���ӵ�Ԫ(Բ��Բ��)
bool CDialogSetDirectRamp::SelectConnectElement()
{	
	ads_name   EntityName; // ��ѡʵ������
	ads_point  BasePoint;  // ��ѡʵ�����

	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return false;
	}

	// ѡ����ٳ�������Բ
	if(ads_entsel(L"\n��ѡȡ���ٳ������ӵ�Ԫ(Բ��Բ��):\n",EntityName,BasePoint) != RTNORM) 
	{
		SelCircleId = NULL;
		acutPrintf(L"�ڽ�Բ��Բ��ѡ��ʧ��!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	if(acdbGetObjectId(SelCircleId,EntityName) != Acad::eOk)
	{
		SelCircleId = NULL;
		acutPrintf(L"��ȡʵ�����IDʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	if(!GetConnectCirCoordRad()) // �������ԲԲ������Ͱ뾶
	{
		return false;
	}

	m_bCirSelAgain = true;

	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

// ѡȡ��ֹ���Ƶ�Ԫ(��·ʵ�塢ֱ�ߡ�Բ��Բ��)(��ѡ) 2005.07.19 ldf
bool CDialogSetDirectRamp::SelectEndElement()
{
	ads_name   EntityName; // ��ѡʵ������
	int        resultCode; // �����

	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return false;
	}

	resultCode = ads_entsel(L"\nѡȡ��ֹ���Ƶ�Ԫ(��·ʵ�塢ֱ�ߡ�Բ��Բ��)(��ѡ) : ",EntityName, PickDotEnd);
	if(resultCode == RTCAN || resultCode != RTNORM)
	{
		EndElementId = NULL;
		acutPrintf(L"��ֹ���Ƶ�Ԫû��ѡ���ѡ��ʧ��!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	if(acdbGetObjectId(EndElementId,EntityName) != Acad::eOk)
	{
		EndElementId = NULL;
		acutPrintf(L"��ȡ��ֹ���Ƶ�ԪIDʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	GetOffsetElement(); // ����ƫ��ֵ,ʵʱ�����ֹƫ�õ�Ԫ	

	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

// ����ƫ��ֵ,ʵʱ�����ֹƫ�õ�Ԫ
bool CDialogSetDirectRamp::GetOffsetElement()
{
	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return false;
	}

	// 1.����ѡȡ����ֹ��Ԫ����,����ƫ�õ���ֹ��Ԫ
	AcDbEntity * pEntity = NULL;
	if(acdbOpenObject(pEntity,EndElementId,AcDb::kForRead) != Acad::eOk)
	{
		EndElementId = NULL;
		acutPrintf(L"��ֹ��Ԫ��ʧ��,�ͷ�ʵ��ID!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	if(_tcscmp(pEntity->isA()->name(),L"JD_CENTER") == 0) 
	{
		// JD_CENTER��������Ԫ����
		m_bJD_CENTER = true;
		m_bConnectDirect = false;
		GetDlgItem(IDC_CHECK_ConnectDirect)->EnableWindow(FALSE); 

		JD_CENTER* pEndJD_CENTER;
		pEndJD_CENTER = JD_CENTER::cast(pEntity); 
		double EndProMiles = pEndJD_CENTER->PROJ_ML(PickDotEnd[Y], PickDotEnd[X]);	
		xlpoint xlpoint_End; 
		xlpoint_End.lc = EndProMiles;
		pEndJD_CENTER->xlpoint_pz(&xlpoint_End);
		DRampSMainRad = xlpoint_End.r;
		double dFwj  = xlpoint_End.a;
		int ItemIndex = pEndJD_CENTER->GetXYNoFromTYLC(xlpoint_End.lc); // ��Ԫ��
		double ItemType = pEndJD_CENTER->XYArray[ItemIndex][0]; // ��Ԫ������
		double LeftOrRight = pEndJD_CENTER->XYArray[ItemIndex][4]; // ��Ԫ��ƫ��

		if(fabs(ItemType-1.0) < DataPrecision)
		{
			AcGePoint3d startPt(xlpoint_End.y,xlpoint_End.x,0.0);
			makeline(startPt,dFwj,0.1,OffsetElementId,0);
		}
		else if(ItemType > 1.0)
		{
			AcGePoint3d startPt(xlpoint_End.y,xlpoint_End.x,0.0);
			makearc(startPt,dFwj,xlpoint_End.r,0.1,LeftOrRight,OffsetElementId,0);
		}
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *pCircle = AcDbCircle::cast(pEntity);
		AcGePoint3d centerPt;
		centerPt[X] = (pCircle->center())[X];
		centerPt[Y] = (pCircle->center())[Y];
		double R = pCircle->radius();
		makecircle(centerPt,R,OffsetElementId,0);
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{	
		AcDbArc *pArc = AcDbArc::cast(pEntity);
		AcGePoint3d centerPt;
		centerPt[X] = (pArc->center())[X];
		centerPt[Y] = (pArc->center())[Y];
		double R = pArc->radius();
		makecircle(centerPt,R,OffsetElementId,0);
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbLine") == 0) 
	{
		AcDbLine *pLine = AcDbLine::cast(pEntity);
		AcGePoint3d sPt,ePt;
		sPt = pLine->startPoint();
		ePt = pLine->endPoint();

		makeline(sPt,ePt,OffsetElementId,0);
	}
	else
	{
		pEntity->close(); 	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	pEntity->close(); 	

	// 2.��ƫ��ֵ,�޸���ֹƫ�õ�Ԫ
	if(acdbOpenObject(pEntity,OffsetElementId,AcDb::kForWrite) != Acad::eOk)
	{
		EndElementId = NULL;
		acutPrintf(L"��ֹ��Ԫ��ʧ��,�ͷ�ʵ��ID!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *pCircle = AcDbCircle::cast(pEntity);
		double R = pCircle->radius();
		R = R + m_dOffsetW;
		pCircle->setRadius(R);
		m_dEndElemR = R;
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{	
		AcDbArc *pArc = AcDbArc::cast(pEntity);
		double R = pArc->radius();
		R = R + m_dOffsetW;
		pArc->setRadius(R);
		m_dEndElemR = R;
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbLine") == 0) 
	{
		AcDbLine *pLine = AcDbLine::cast(pEntity);
		AcGePoint3d sPt, ePt;
		sPt = pLine->startPoint();
		ePt = pLine->endPoint();

		double dFwj = fwj(sPt, ePt);
		dFwj = dFwj + SIGN(m_dOffsetW)*0.5*PI;

		AcGePoint3d sPtNew, ePtNew;
		getLineEndPt(sPt,fabs(m_dOffsetW),dFwj,sPtNew);
		getLineEndPt(ePt,fabs(m_dOffsetW),dFwj,ePtNew);
		pLine->setStartPoint(sPtNew);
		pLine->setEndPoint(ePtNew);

		m_dEndElemR = 1e40;
	}
	else
	{
		pEntity->close(); 	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	pEntity->close(); 	

	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());
	return true;
}

// �������ٳ�����·ʵ�� 2005.07.04 ldf
bool CDialogSetDirectRamp::CreateRampJD_CENTER(double XYArrayTmp[][10],int iXYNum)
{
	int i, j;

	// 1.��֯��Ԫ����
	// �����Ԫ
	m_iXYMax = 0; 
	RouteItemArray[0][0] = 0; 
	RouteItemArray[0][1] = DRampSDot[Y];
	RouteItemArray[0][2] = DRampSDot[X];
	RouteItemArray[0][3] = DRampSOrient;
	RouteItemArray[0][4] = 0;

	// ���Ӷ���Ԫ(�����������),�ȼ����䳤��
	double dConnectLen;
	OrignDot[X] = XYArrayTmp[0][9];
	OrignDot[Y] = XYArrayTmp[0][8];
	DROriOrient = XYArrayTmp[0][5];

	//  
	if(!CheckSprial())
	{
		return false;
	}	

	if(fabs(ValidItemType-1) < DataPrecision)
	{
		dConnectLen = DRampSDot.distanceTo(OrignDot);
	}
	else
	{
		dConnectLen = fabs(DROriOrient-DRampSOrient) * DRampSRad;
	}
	if(!m_bFitConnectLen) // �ڲ��̶��������,���Ӷγ��ɼ��㸴��
	{
		m_EditConnectLen.Format(L"%lf", dConnectLen); // �������Ӷγ�
	}

	m_iXYMax++;
	if(fabs(ValidItemType-1) < DataPrecision)
	{
		RouteItemArray[1][0] = 1; // ֱ�߶�
		RouteItemArray[1][1] = dConnectLen;
		RouteItemArray[1][2] = 0;
		RouteItemArray[1][3] = 0;
		RouteItemArray[1][4] = 0;
		RouteItemArray[1][5] = DRampSOrient;
		RouteItemArray[1][6] = 0;
		RouteItemArray[1][7] = 1e40;
		RouteItemArray[1][8] = DRampSDot[Y];
		RouteItemArray[1][9] = DRampSDot[X];
	}
	else if(ValidItemType > 1)
	{
		RouteItemArray[1][0] = 2; // Բ���߶�
		RouteItemArray[1][1] = DRampSRad;
		RouteItemArray[1][2] = dConnectLen;
		RouteItemArray[1][3] = 0;
		RouteItemArray[1][4] = DRampDef;
		RouteItemArray[1][5] = DRampSOrient;
		RouteItemArray[1][6] = 0;
		RouteItemArray[1][7] = DRampSRad;
		RouteItemArray[1][8] = DRampSDot[Y];
		RouteItemArray[1][9] = DRampSDot[X];
	}

	// �м������Ԫ
	for(i=0; i<iXYNum; i++)
	{
		m_iXYMax++;
		for(j=0; j<10; j++)
		{
			RouteItemArray[m_iXYMax][j] = XYArrayTmp[i][j];
			if(j == 6)
			{
				RouteItemArray[m_iXYMax][j] += dConnectLen;
			}
		}
	}

	// �յ���Ԫ
	double INDATA[5], len;

	m_iXYMax++;
	for(i=0; i<5; i++)
	{
		INDATA[i] = RouteItemArray[m_iXYMax-1][i];
	}
	RouteItemArray[m_iXYMax][0] = 0;
	RouteItemArray[m_iXYMax][1] = 0;
	RouteItemArray[m_iXYMax][2] = 0; 
	RouteItemArray[m_iXYMax][3] = 0; 
	RouteItemArray[m_iXYMax][4] = RouteItemArray[m_iXYMax-1][4]; 

	len = Get_INDATA_L(INDATA,&j);
	RouteItemArray[m_iXYMax][6] = RouteItemArray[m_iXYMax-1][6] + len;
	RouteItemArray[m_iXYMax][7] = RouteItemArray[m_iXYMax-1][7];
	CalXYXY(RouteItemArray[m_iXYMax-1],len,RouteItemArray[m_iXYMax][8],RouteItemArray[m_iXYMax][9],RouteItemArray[m_iXYMax][5]);

	// 2.��֯�ѵ�JD_CENTER����
	if(RampJD_CENTER == NULL)
	{
		RampJD_CENTER = new JD_CENTER;
	}
	RampJD_CENTER->XYArray = RouteItemArray;
	RampJD_CENTER->XYNum   = m_iXYMax; // �����Ԫ��(��Ԫ��-1)	

	double endml  = RampJD_CENTER->XYArray[m_iXYMax][6];
	XYId[m_iXYMax]  = SelCircleId; // ������Ԫ 

	RampJD_CENTER->IfCalXY = false;
	RampJD_CENTER->SHOWJD  = false;
	RampJD_CENTER->setDLM(0,0.0,0.0,L"K",L"K");
	RampJD_CENTER->setDLM(1,endml,endml,L"K",L"K");
	//	RampJD_CENTER->XYArrayToJD(); // ���Ҫ���ƽ�������,��������

	return true;
}

void CDialogSetDirectRamp::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bFitConnectLen = !m_bFitConnectLen;
	if(m_bFitConnectLen) // ����̶����Ӷγ�,���ֹ�϶�����Բ
	{
		GetDlgItem(IDC_BUTTONGETENDRAD)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTONGETENDRAD)->EnableWindow(TRUE);
	}
}

// ��黺�����ߵĺ�����
bool CDialogSetDirectRamp::CheckSprial()
{
	// �жϻ���������Ƿ����(!!!��дһ������ר�Ž��й淶�ж�)
	ads_point InputePoint;
	xlpoint RPoint;
	double SelItemType,InterMiles;
	InputePoint[X] = OrignDot[X];
	InputePoint[Y] = OrignDot[Y];
	ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	double deltaMiles1 = flag * (InterMiles - _wtof(m_EditDRampSMiles));
	double deltaMiles2 = flag * (ProDRampCirMiles - InterMiles);
	if(deltaMiles1 < 0.0)
	{
		acutPrintf(L"\n###: ���������Խ�����ٳ������,�������ز���!\n");
		return false;
	}
	if(deltaMiles2 < 0.0)
	{
		acutPrintf(L"\n###: ���������Խ������Բ�е�,�������ز���!\n");
		return false;
	}

	return true;
}

// ʵʱ�������ԲԲ������Ͱ뾶
bool CDialogSetDirectRamp::GetConnectCirCoordRad()
{
	// ������ȡ��ʵ��
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelCircleId,AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"ʵ���ʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// ��ȡ��뾶��Բ������
	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		DRampCirX = (m_pCircle->center())[X];
		DRampCirY = (m_pCircle->center())[Y];
		DRampCirRad = m_pCircle->radius();
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		DRampCirX = (m_pArc->center())[X];
		DRampCirY = (m_pArc->center())[Y];
		DRampCirRad = m_pArc->radius();
	}
	else
	{
		acutPrintf(L"��ѡʵ�岻��Բ��Բ��!\n");
		return false;
	}
	pEntity->close(); // �ر���ѡʵ��

	DRampERad = DRampCirRad;
	m_EditDRampERad.Format(L"%lf", DRampCirRad);

	return true;
}

// ��������Բ�뾶
bool CDialogSetDirectRamp::SetConnectCirRad(double dRadius)
{
	// ������ȡ��ʵ��
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelCircleId,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"ʵ���ʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// ��ȡ��뾶��Բ������
	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		m_pCircle->setRadius(dRadius);
		DRampCirRad = dRadius;
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		m_pArc->setRadius(dRadius);
		DRampCirRad = dRadius;
	}
	else
	{
		acutPrintf(L"��ѡʵ�岻��Բ��Բ��!\n");
		return false;
	}
	pEntity->close(); // �ر���ѡʵ��

	DRampERad = DRampCirRad;

	return true;
}

// ��������ԲԲ������
bool CDialogSetDirectRamp::SetConnectCirCoord(AcGePoint3d centerPt)
{
	// ������ȡ��ʵ��
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelCircleId,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"ʵ���ʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// ������Բ������
	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		m_pCircle->setCenter(centerPt);
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		m_pArc->setCenter(centerPt);
	}
	else
	{
		acutPrintf(L"��ѡʵ�岻��Բ��Բ��!\n");
		return false;
	}
	pEntity->close(); // �ر���ѡʵ��

	return true;
}

// ʵʱ������ߵ�Ԫ
bool CDialogSetDirectRamp::GetMainElement()
{
	// ������ȡ��ʵ��, �������û����ȡ׮��ֵ
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelEntId,AcDb::kForRead) != Acad::eOk)
	{
		SelEntId = NULL;
		acutPrintf(L"ʵ���ʧ��,�ͷ�ʵ��ID!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// �ж���ѡʵ���Ƿ�Ϊ��·ʵ��
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER") != 0) 
	{
		SelEntId = NULL;
		acutPrintf(L"��ѡʵ�岻����·ʵ��,�ͷ�ʵ��ID!\n");
		pEntity->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	pGottnRamp  = JD_CENTER::cast(pEntity); // ��ȡ����ʵ��ָ��
	pEntity->close(); // �ر���ѡʵ��

	return true;
}

// ���Ļ������߲�������(Ч��ͬ��)
void CDialogSetDirectRamp::OnSelchangeAl() 
{
	//	// TODO: Add your control notification handler code here
	//	GetDlgItemText(IDC_AL,m_AorL);
	//	
	//	if(m_AorL == "A")
	//	{
	//		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"A2");
	//		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"A2");
	//	}
	//	else
	//	{
	//		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"L1");
	//		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"L2");
	//	}
	//
	//	UpdateData(FALSE);

}

// ���Ļ������߲�������(Ч��ͬ��)
void CDialogSetDirectRamp::OnSelendokAl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_AorL == "A")
	{
		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"A2");
		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"A2");
		GetDlgItem(IDC_STATICSPRIALA3)->SetWindowText(L"A3");
		GetDlgItem(IDC_STATICSPRIALA4)->SetWindowText(L"A4");
	}
	else
	{
		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"L1");
		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"L2");
		GetDlgItem(IDC_STATICSPRIALA3)->SetWindowText(L"L3");
		GetDlgItem(IDC_STATICSPRIALA4)->SetWindowText(L"L4");
	}

	PutA1A2(); // ��ͳһ��A1A2ֵ��������������

	UpdateData(FALSE);
}

// �ɽ�������������ͳһ��A1A2ֵ
void CDialogSetDirectRamp::GetA1A2()
{
	if(m_AorL == "A") // ��������Aֵ,��ֱ��ʹ��
	{
		A1 = m_dA1L1;
		A2 = m_dA2L2;
	}
	else // ��������Lֵ,����ת����Aֵ,�Ա�ͳһ����: A = sqrt(L * R) 
	{
		A1 = sqrt(m_dA1L1 * DRampSRad); 
		if(m_LinkModel == 2) // ����Aֵ���ر����
		{
			A2 = sqrt(m_dA2L2*DRampSRad*DRampCirRad / fabs(DRampSRad-DRampCirRad));
		}
		else
		{
			A2 = sqrt(m_dA2L2 * DRampCirRad);
		}
	}
}

// ��ͳһ��A1A2ֵ��������������
void CDialogSetDirectRamp::PutA1A2()
{
	if(m_AorL == "A")
	{
		m_dA1L1 = A1;
		m_dA2L2 = A2;
		m_dA3L3 = A3;
		m_dA4L4 = A4;
	}
	else // ��ʾLֵ: L = A*A / R
	{
		m_dA1L1 = A1*A1 / DRampSRad; 
		if(m_LinkModel == 2) // ����Lֵ���ر����
		{
			m_dA2L2 = A2*A2*fabs(DRampSRad-DRampCirRad) / (DRampSRad*DRampCirRad);
		}
		else
		{
			m_dA2L2 = A2*A2 / DRampCirRad;
		}
		if(m_HLinkModel == 2) // ����Lֵ���ر����
		{
			m_dA3L3 = A3*A3*fabs(m_dEndElemR-DRampCirRad) / (m_dEndElemR*DRampCirRad);
		}
		else
		{
			m_dA3L3 = A3*A3 / DRampCirRad; 
		}
		m_dA4L4 = A4*A4 / m_dEndElemR; 
	}
}

// ��ÿ��Ƶ����W1,W2,W3,W4
bool CDialogSetDirectRamp::GetW1234()
{
	int nStart = 0, nEnd, nCount;
	CString strTmp[4];

	for(int i=0; i<3; i++) // W1,W2,W3
	{
		nEnd   = m_strW1234.Find(L" ", nStart);
		nCount = nEnd - nStart;
		if(nCount)
		{
			strTmp[i] = m_strW1234.Mid(nStart, nCount);
		}
		else
		{
			return false;
		}
		nStart = nEnd + 1;
	}

	nCount = m_strW1234.GetLength() - nStart; // W4
	if(nCount)
	{
		strTmp[3] = m_strW1234.Mid(nStart, nCount);
	}
	else
	{
		return false;
	}

	W1 = _wtof(strTmp[0]);
	W2 = _wtof(strTmp[1]);
	W3 = _wtof(strTmp[2]);
	W4 = _wtof(strTmp[3]);

	return true;
}

// Ӧ��(���������ˢ��ͼ��)
void CDialogSetDirectRamp::OnButtonImply() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return ;
	}

	if(!GetParameter()) // ��ý�����������������������
	{
		return;
	}

	ComputerDRampParameters();

	Draw();

	UpdateData(FALSE);
	acDocManager->unlockDocument(acDocManager->curDocument());
}

// ָ���Ƿ�ֱ��������ֹ��Ԫ
void CDialogSetDirectRamp::OnCHECKConnectDirect() 
{
	// TODO: Add your control notification handler code here
	m_bConnectDirect = !m_bConnectDirect;
}

// ��ý����������������������� 2005.07.21 ldf
bool CDialogSetDirectRamp::GetParameter()
{
	UpdateData(TRUE);

	GetMainElement(); // ʵʱ������ߵ�Ԫָ��
	ProTriMiles = _wtof(m_EditRampTriMiles);
	GetA1A2();        // �ɽ�������������ͳһ��A1A2ֵ
	if(!GetW1234())   // ��ÿ��Ƶ����W1,W2,W3,W4
	{
		AfxMessageBox(L"���Ƶ������������!");
		return false;
	}

	if(fabs(_wtof(m_EditDRampERad)-DRampCirRad) > DataPrecision) 
	{
		if(!SetConnectCirRad(_wtof(m_EditDRampERad))) // ������������Բ�뾶
		{
			acutPrintf(L"��������Բ�뾶����!\n");
			return false;
		}
	}

	if(!GetConnectCirCoordRad()) // ʵʱ�������ԲԲ������Ͱ뾶
	{
		return false;
	}

	if(m_iSelElem == 3)
	{
		if(!GetOffsetElement()) // ����ƫ��ֵ,�����ֹƫ�õ�Ԫ
		{
			acutPrintf(L"�����ֹƫ�õ�Ԫ����!\n");
			return false;
		}
	}

	UpdateData(FALSE);
	return true;
}

// ��̬�϶�����Բ
bool CDialogSetDirectRamp::DragConnectCir()
{
	if(!GetParameter()) // ��ý�����������������������
	{
		return false;
	}

	int resultCode; 
	ads_name     EntityName;
	AcDbObjectId SelId;
	AcGePoint3d  PickDotCir;
	AcGePoint3d  mousePtOld, mousePtNew;
	double       deltaX, deltaY;

	int    MouseType;         // ����������
	short  KeyCode;           // ����ؼ������
	struct resbuf resbufTmp;  // �������������

	resultCode = ads_entsel(L"\n��ѡ�����ӵ�Ԫ: ", EntityName, asDblArray(PickDotCir));
	if(resultCode == RTCAN)
	{
		return false;
	}
	if(resultCode != RTNORM) 
	{
		ads_printf(L"���ӵ�Ԫѡ������!\n");
		return false;
	}       

	if(acdbGetObjectId(SelId,EntityName) != Acad::eOk)
	{
		acutPrintf(L"��ȡ���ӵ�ԪIDʧ��!\n");	
		return false;
	}

	if(SelId != SelCircleId)
	{
		ads_printf(L"���ӵ�Ԫѡ������!\n");
		return false;
	}

	UpdateData(FALSE); // ���µ����Ի���,ʵʱ�鿴����
	ShowWindow(TRUE);

	mousePtOld = PickDotCir;
	for(;;)
	{	
		if(ads_grread(1,&MouseType,&resbufTmp) != RTNORM) 			
		{
			break;
		}	
		KeyCode = resbufTmp.resval.rint;
		if(KeyCode==13 || KeyCode==32 || MouseType==25) 
		{
			break;
		}
		if(MouseType == 3) 
		{
			break; // �������˳���̬���ѭ��(ȷ����Ƴɹ�)
		}
		if((MouseType!=5) && (MouseType!=12))
		{
			ads_printf(L"���������!\n");
			break;
		}
		if(MouseType == 5)
		{
			mousePtNew[X] = resbufTmp.resval.rpoint[X];
			mousePtNew[Y] = resbufTmp.resval.rpoint[Y];
			mousePtNew[Z] = 0.0;

			deltaX = mousePtNew[X] - mousePtOld[X];
			deltaY = mousePtNew[Y] - mousePtOld[Y];

			DRampCirX += deltaX;
			DRampCirY += deltaY;
			mousePtOld = mousePtNew;

			AcGePoint3d  centerPt(DRampCirX,DRampCirY,0.0);
			SetConnectCirCoord(centerPt); // ������������ԲԲ������
			ComputerDRampParameters();    // ���½�������
		}
	}

	Draw(); // ���������· 

	return true;
}

// ���������· 2005.07.22 ldf
// XYId[0]��¼��ֹ���Ƶ�ԪID,��Ҫ������ֹ��Ԫʱ����Ϊ���һ����Ԫ�ĺ��ID����
// XYId[1]-->XYId[6]�洢��������ID��,����д����չ����,�±��Ӧ��Ԫ��,
// ����ÿ��д������ʱҪ�ô�������ԪID,���Ի��õ�XYId[7],ͨ��ΪNull
// XYId[8]-->XYId[10]����ʱ��ԪID,
// XYId[8]��Բ��(����Բ��)ID,������Polyline(������)ID
void CDialogSetDirectRamp::Draw()                  
{
	// ��¼��ֹ���Ƶ�ԪID	
	if(m_iSelElem == 3)
	{	
		if(m_bConnectDirect)
		{
			XYId[0] = EndElementId; 
		}
		else
		{
			XYId[0] = OffsetElementId; 
		}
	}
	else
	{
		XYId[0] = SelCircleId; 
	}

	// �������Ӷ�
	if(fabs(ValidItemType-1) < DataPrecision)
	{
		DrawXY(RouteItemArray[1],ConnectLineID,0);
		XYId[1] = ConnectLineID;
	}
	else if(ValidItemType > 1)
	{
		DrawXY(RouteItemArray[1],ConnectCirID,0);
		XYId[1] = ConnectCirID;
	}

	// ���ƻ�������
	if(m_LinkModel==-1 || m_LinkModel==2)
	{
		DrawXY(RouteItemArray[2],SprialID1,1);
		XYId[2] = SprialID1;
		A2 = RouteItemArray[2][1];

		if(m_iSelElem == 3)
		{
			DrawXY(RouteItemArray[3],XYId[8],2);
			m_iConnectCirXY = 3;
			XYId[3] = XYId[8];
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				DrawXY(RouteItemArray[4],XYId[9],1);
				XYId[4] = XYId[9];
				XYId[5] = NULL;
				A3 = RouteItemArray[4][1];
			}
			else
			{
				DrawXY(RouteItemArray[4],XYId[9],1);
				DrawXY(RouteItemArray[5],XYId[10],1);
				XYId[4] = XYId[9];
				XYId[5] = XYId[10];
				XYId[6] = NULL;
				A3 = RouteItemArray[4][1];
				A4 = RouteItemArray[5][1];
			}
		}
		else
		{
			XYId[3] = NULL;
		}
	}
	else
	{
		DrawXY(RouteItemArray[2],SprialID1,1);
		DrawXY(RouteItemArray[3],SprialID2,1);
		XYId[2] = SprialID1;
		XYId[3] = SprialID2;
		A1 = RouteItemArray[2][1];
		A2 = RouteItemArray[3][1];
		if(m_iSelElem == 3)
		{
			DrawXY(RouteItemArray[4],XYId[8],2);
			m_iConnectCirXY = 4;
			XYId[4] = XYId[8];
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				DrawXY(RouteItemArray[5],XYId[9],1);
				XYId[5] = XYId[9];
				XYId[6] = NULL;
				A3 = RouteItemArray[5][1];
			}
			else
			{
				DrawXY(RouteItemArray[5],XYId[9],1);
				DrawXY(RouteItemArray[6],XYId[10],1);
				XYId[5] = XYId[9];
				XYId[6] = XYId[10];
				XYId[7] = NULL;
				A3 = RouteItemArray[5][1];
				A4 = RouteItemArray[6][1]; 
			}
		}
		else
		{
			XYId[4] = NULL;
		}
	}
}


