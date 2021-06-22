// DrawBianXianDlg.cpp : implementation file
// ������·ƫ�ñ��߶Ի��� 2005.07.12 ldf
//
//////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "resource.h"
#include "DrawBianXianDlg.h"
#include "XLDataBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ������·ƫ�ñ��߶Ի���

CDrawBianXianDlg::CDrawBianXianDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawBianXianDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawBianXianDlg)
	m_strStyle = _T("ȫ��");
	m_strMethod = _T("�ȿ�");
	m_dScml = 0.0;
	m_dEcml = 0.0;
	m_strSWidth = _T("11 -11 10.5 -10.5");
	m_strEWidth = _T("11 -11 10.5 -10.5");
	m_dStep = 2.0;
	m_pOffsetRoad=NULL;
	//}}AFX_DATA_INIT
}

void CDrawBianXianDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawBianXianDlg)
	DDX_CBString(pDX, IDC_COMBO_STYLE, m_strStyle);
	DDX_CBString(pDX, IDC_COMBO_METHOD, m_strMethod);
	DDX_Text(pDX, IDC_EDIT_Sml, m_dScml);
	DDX_Text(pDX, IDC_EDIT_Eml, m_dEcml);
	DDX_CBString(pDX, IDC_COMBO_SW, m_strSWidth);
	DDX_CBString(pDX, IDC_COMBO_EW, m_strEWidth);
	DDX_Text(pDX, IDC_EDIT_STEP, m_dStep);
	DDV_MinMaxDouble(pDX, m_dStep, 0., 10000000.);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDrawBianXianDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawBianXianDlg)
	ON_BN_CLICKED(IDC_BUTTON_GET_ROAD, OnButtonGetRoad)
	ON_BN_CLICKED(IDC_BUTTON_GET_SML, OnButtonGetSml)
	ON_BN_CLICKED(IDC_BUTTON_GET_EML, OnButtonGetEml)
	ON_CBN_SELENDOK(IDC_COMBO_METHOD, OnSelendokComboMethod)
	ON_CBN_SELENDOK(IDC_COMBO_SW, OnSelendokComboSw)
	ON_CBN_SELENDOK(IDC_COMBO_STYLE, OnSelendokComboStyle)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CDrawBianXianDlg message handlers

BOOL CDrawBianXianDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	// ��ʼΪ�ȿ��ƫ��
	m_strEWidth = m_strSWidth; 
	GetDlgItem(IDC_COMBO_EW)->EnableWindow(FALSE);

	m_pOffsetRoad  = NULL;
	m_OffsetRoadID = NULL;
	m_dBaseProMile = 0.0;

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// ѡ��ƫ����·����Ԫ
void CDrawBianXianDlg::OnButtonGetRoad() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return ;
	}

	ads_name   EntityName; // ��ѡʵ������
	ads_point  BasePoint;  // ��ѡʵ�����

	// ��ʾ�û�ѡ������
	if(ads_entsel(L"\n��ѡȡƫ��ʵ��:\n",EntityName,BasePoint) != RTNORM) 
	{
		m_OffsetRoadID = NULL;
		acutPrintf(L"ƫ��ѡȡʧ��!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		DestroyWindow();
		return ;
	}
	if(acdbGetObjectId(m_OffsetRoadID,EntityName) != Acad::eOk)
	{
		m_OffsetRoadID = NULL;
		acutPrintf(L"��ȡƫ��ʵ��IDʧ��!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return ;
	}

	if(!GetOffsetRoad()) // ���ƫ��ʵ��ָ��
	{
		return;
	}

	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());

	// ��ƫ������ȷ����ʼ׮�ŷ�Χ
	m_dBaseProMile = m_pOffsetRoad->PROJ_ML(BasePoint[Y], BasePoint[X]); // ��ȡ�����Ӧ��ͳһ���

	if(m_strStyle == "ȫ��")
	{
		int iXYNum = m_pOffsetRoad->XYNum;
		m_dScml = m_pOffsetRoad->XYArray[0][4];
		m_dEcml = m_pOffsetRoad->XYArray[iXYNum][6];
	}
	else // ������Ԫ
	{
		int ItemIndex = m_pOffsetRoad->GetXYNoFromTYLC(m_dBaseProMile);      // �����Ӧ��·����Ԫ��
		m_dScml = m_pOffsetRoad->XYArray[ItemIndex][6];
		m_dEcml = m_pOffsetRoad->XYArray[ItemIndex+1][6];
	}

	UpdateData(FALSE);
}

// ʵʱ���ƫ��ʵ��ָ��
bool CDrawBianXianDlg::GetOffsetRoad()
{
	// ������ȡ��ʵ��, �������û����ȡ׮��ֵ
	double cml;
	CString mdbname,roadname;
	XLDataBase DBS;

	AcDbEntity * pEntity = NULL;
	if(acdbOpenObject(pEntity,m_OffsetRoadID,AcDb::kForRead) != Acad::eOk)
	{
		m_OffsetRoadID = NULL;
		acutPrintf(L"ʵ���ʧ��,�ͷ�ʵ��ID!\n");	
		return false;
	}
	pEntity->close();
	// �ж���ѡʵ���Ƿ�Ϊ��·ʵ��
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER") != 0) 
	{
		DBS.GetXLXdata(m_OffsetRoadID,mdbname,roadname);
		if(mdbname!=""&&roadname!="")
		{
			//			DBS.Read_XLDbs(mdbname,"���Ƶ��",roadname);
			DBS.Read_XLDbs(mdbname,L"��Ԫ��",roadname);				   
			DBS.Read_XLDbs(mdbname,L"������",roadname);
			if(DBS.XYNum>0)
			{
				m_pOffsetRoad=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
				//				m_pOffsetRoad->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					m_pOffsetRoad->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(m_pOffsetRoad->mdbname,mdbname);
				_tcscpy(m_pOffsetRoad->RoadName,roadname); 
				return true;
			}
		}
		else
		{
			m_OffsetRoadID = NULL;
			acutPrintf(L"��ѡʵ�岻����·ʵ��,�ͷ�ʵ��ID!\n");
			return false;
		}			
	}	
	m_pOffsetRoad  = JD_CENTER::cast(pEntity); // ��ȡ����ʵ��ָ��

	return true;
}

// ���ƫ����ʼ׮��
void CDrawBianXianDlg::OnButtonGetSml() 
{
	// TODO: Add your control notification handler code here
	GetCml(m_dScml); // ��̬ʰȡ׮��
	UpdateData(FALSE);
}

// ���ƫ����ֹ׮��
void CDrawBianXianDlg::OnButtonGetEml() 
{
	// TODO: Add your control notification handler code here
	GetCml(m_dEcml); // ��̬ʰȡ׮��
	UpdateData(FALSE);
}

// ������·ƫ�ñ���
void CDrawBianXianDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	// ��ÿ������
	if(!GetOffSetWidth(m_strSWidth, m_SWidth))
	{
		return;
	}
	if(m_strMethod == "�ȿ�")
	{
		m_strEWidth = m_strSWidth;	
	}
	if(!GetOffSetWidth(m_strEWidth, m_EWidth))
	{
		return;
	}

	if(m_SWidth.GetSize() != m_EWidth.GetSize()) 
	{
		AfxMessageBox(L"��ʼ����ֹ������б���һһ��Ӧ!");
		return;
	}

	if(!GetOffsetRoad()) // ʵʱ���ƫ��
	{
		return;
	}

	// ��ñ��ʽ
	int bkmode = 0; // ���ģʽ,��ʼΪ0: 0--�ȿ����Ա�� 1--�����߱��
	if(m_strMethod == "�����߱��")
	{
		bkmode = 1;
	}

	// ������·ƫ�ñ���
	for(int i=0; i<m_SWidth.GetSize(); i++)
	{
		m_pOffsetRoad->DrawKD(m_dScml,m_SWidth[i],m_dEcml,m_EWidth[i],bkmode,7,m_dStep);
	}

	CDialog::OnOK();
}

// ���ƫ�ÿ��
bool CDrawBianXianDlg::GetOffSetWidth(CString strWidth, CArray<double,double>& arrays)
{
	arrays.RemoveAll();

	if(strWidth.GetLength() < 1)
	{
		AfxMessageBox(L"����ȷ�������ƫ�ÿ��!");
		return false;
	}

	int nStart = 0, nEnd, nCount;
	CString strTmp;
	double  dTmp;
	nEnd   = strWidth.Find(L" ", nStart);
	while(nEnd != -1)
	{
		nCount = nEnd - nStart;
		if(nCount)
		{
			strTmp = strWidth.Mid(nStart, nCount);
			dTmp = _wtof(strTmp);
			arrays.Add(dTmp);
		}
		nStart = nEnd + 1;
		nEnd   = strWidth.Find(L" ", nStart);
	}

	nCount = strWidth.GetLength() - nStart; // ���һ��
	if(nCount)
	{
		strTmp = strWidth.Mid(nStart, nCount);
		dTmp = _wtof(strTmp);
		arrays.Add(dTmp);
	}

	return true;
}

// ��̬ʰȡ׮��
void CDrawBianXianDlg::GetCml(double& dCml)
{
	acedGetAcadDwgView()->SetFocus(); // ��������CAD�ĵ�
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return ;
	}

	GetOffsetRoad(); // ʵʱ������ߵ�Ԫָ��

	int MouseType;             // ����������
	struct resbuf ClickResult; // �������������
	short KeyCode;             // ����ؼ������

	AcGePoint3d PickDot;
	for( ; ; ) // ��̬ʰȡ׮��ѭ��
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
		if(fabs(MouseType - 3.0) < DataPrecision) 
		{
			break; // �������˳�ʰȡѭ��
		}
		else if(MouseType == 5) // ��������ƶ�
		{
			PickDot.x = ClickResult.resval.rpoint[X];
			PickDot.y = ClickResult.resval.rpoint[Y];
			PickDot.z = ClickResult.resval.rpoint[Z];	

			dCml = m_pOffsetRoad->PROJ_ML(PickDot.y,PickDot.x); // �����Ӧ��·��ͳһ���	
			UpdateData(FALSE);
		}
		else
		{
			break; // ���������������,�˳���̬���ѭ��
		}
	}	

	acDocManager->unlockDocument(acDocManager->curDocument());
}

// ѡ��ƫ�÷���
void CDrawBianXianDlg::OnSelendokComboMethod() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_strMethod == "�ȿ�")
	{
		m_strEWidth = m_strSWidth; // ���յ���һ��
		GetDlgItem(IDC_COMBO_EW)->EnableWindow(FALSE);	
	}
	else
	{
		GetDlgItem(IDC_COMBO_EW)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

// ѡ�������ʼ�������
void CDrawBianXianDlg::OnSelendokComboSw() 
{
	// TODO: Add your control notification handler code here

	//	GetDlgItemText(IDC_COMBO_SW, m_strSWidth);

	//	GetDlgItem(IDC_COMBO_SW)->UpdateData(TRUE);
	//
	//	if(m_strMethod == "�ȿ�")
	//	{
	//		m_strEWidth = m_strSWidth; // ���յ���һ��
	//		GetDlgItem(IDC_COMBO_EW)->SetWindowText(m_strEWidth);
	//	}
	//	
	//	UpdateData(FALSE);
}

// ���������ʼ�������
//void CDrawBianXianDlg::OnEditchangeComboSw() 
//{
// TODO: Add your control notification handler code here
//	UpdateData(TRUE);

//	if(m_strMethod == "�ȿ�")
//	{
//		GetDlgItemText(IDC_COMBO_SW,m_strSWidth);
//		m_strEWidth = m_strSWidth; // ���յ���һ��
//		GetDlgItem(IDC_COMBO_EW)->SetWindowText(m_strEWidth);
//	}


//	UpdateData(FALSE);
//}

// ѡ��ƫ������
void CDrawBianXianDlg::OnSelendokComboStyle() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	// ���ѡ����ƫ��,��ƫ�����ͳ�ʼ׮�ŷ�Χ
	if(m_pOffsetRoad)
	{
		if(m_strStyle == "ȫ��")
		{
			int iXYNum = m_pOffsetRoad->XYNum;
			m_dScml = m_pOffsetRoad->XYArray[0][4];
			m_dEcml = m_pOffsetRoad->XYArray[iXYNum][6];
		}
		else // ������Ԫ
		{
			int ItemIndex = m_pOffsetRoad->GetXYNoFromTYLC(m_dBaseProMile); // �����Ӧ��·����Ԫ��
			m_dScml = m_pOffsetRoad->XYArray[ItemIndex][6];
			m_dEcml = m_pOffsetRoad->XYArray[ItemIndex+1][6];
		}
	}

	UpdateData(FALSE);
}

// �رնԻ���ʱ�ͷſռ�
void CDrawBianXianDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(m_pOffsetRoad)
	{
		delete m_pOffsetRoad;
		m_pOffsetRoad = NULL;
	}
	if(m_OffsetRoadID)
	{
		m_OffsetRoadID = NULL;
	}

	CDialog::OnClose();
}
