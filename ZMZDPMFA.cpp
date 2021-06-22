// ZMZDPMFA.cpp : implementation file
//

#include "stdafx.h"
#include "stdarx.h"
#include "ZMZDPMFA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZMZDPMFA dialog

ZMZDPMFA::ZMZDPMFA(CWnd* pParent /*=NULL*/)
	: CDialog(ZMZDPMFA::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZMZDPMFA)
	m_pzld = FALSE;
	pzLine =NULL;
	m_pzld = false;
	Roadname = L"";
	mdbname = L"";
	GTZXId = NULL;
	//}}AFX_DATA_INIT
}

void ZMZDPMFA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZMZDPMFA)
	DDX_Control(pDX, IDC_COMBO_RXD, m_SelRXD);
	DDX_Check(pDX, IDC_CHECKPZLD, m_pzld);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ZMZDPMFA, CDialog)
	//{{AFX_MSG_MAP(ZMZDPMFA)
	ON_BN_CLICKED(IDC_BUTTONSet, OnBUTTONSet)
	ON_BN_CLICKED(IDC_CHECKPZLD, OnCheckpzld)
	ON_BN_CLICKED(IDC_BUTTON_MDB, OnButtonMdb)
	ON_CBN_SELCHANGE(IDC_COMBO_RXD, OnSelchangeComboRxd)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &ZMZDPMFA::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ZMZDPMFA::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZMZDPMFA message handlers

void ZMZDPMFA::OnBUTTONSet() //�ɵ�
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;

	//	acDocManager->setCurDocument(ZPMDOC,AcAp::kNone,Adesk::kTrue);
	rc = ads_entsel(L"\nSelect a  ��· : ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
		ads_alert(L"��ѡʵ�����·ʵ��!");
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
		EnableWindow(TRUE); // Enable our dialog
		return;   
	}
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj,eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(JD_CENTER::desc())) //��GTZXʵ��,ȡ����
	{
		pObj->close();
		pzLine = JD_CENTER::cast(pObj);
		GTZXId = eId;
		mdbname=pzLine->mdbname;
		Roadname=pzLine->RoadName;
	}			
	else
	{
		pObj->close();
		acutPrintf(L"��ѡʵ�岻����·ʵ��!\n");
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog
	m_mode = 1;
	return ;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}

void ZMZDPMFA::OnCheckpzld() 
{
	// TODO: Add your control notification handler code here
	m_pzld = !m_pzld;
}


BOOL ZMZDPMFA::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	pzLine = NULL;
	GetDlgItem(IDC_COMBO_RXD)->EnableWindow(false);
	GetDlgItem(IDC_STATIC_RXD)->EnableWindow(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void ZMZDPMFA::OnButtonMdb() //�Է������ݿ�
{
	// TODO: Add your control notification handler code here
	CString workdir;
	ACHAR str1[180];
	FILE *fpr;
	int i;

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	workdir=Cworkdir;
	CString curprj = workdir + L"\\project";   
	if((fpr = _wfopen(curprj,L"r"))==NULL)   
		curprj = L"";
	else
	{
		fwscanf(fpr,L"%s",str1);
		curprj=str1;
		fclose(fpr);

		//20191217	���Ǻ��ո�ķ���
		curprj = BAS_DRAW_FUN::InStr$toNull(curprj);

		curprj = workdir + L"\\data\\" + curprj + L".mdb";//��ǰ����
	}

	CFileDialog fname(true,NULL,curprj);
	fname.m_ofn.lpstrDefExt = L"MDB";
	fname.m_ofn.lpstrInitialDir=workdir+L"\\data";
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"��·�������ݿ�(*.MDB)\0*.mdb\0\0";
	fname.DoModal();	
	curprj =fname.GetPathName(); 
	mdbname = curprj;	

	if(mdbname != L"")
	{
		//		DBS.Read_XLDbs(mdbname,L"���Ƶ��",L"����");
		DBS.Read_XLDbs(mdbname,L"��Ԫ��",L"����");				
		DBS.Read_XLDbs(mdbname,L"������",L"����");
		if(DBS.XYNum>0)
		{
			pzLine=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
			//			pzLine->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0)
				pzLine->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pzLine->mdbname,mdbname);
			_tcscpy(pzLine->RoadName,L"����");
			DBS.Read_XLDbs(mdbname,L"��·����");
			pzLine->DV = DBS.RoadXZ.DV;
			pzLine->Grade = DBS.RoadXZ.Grade;	//20190806����
		}

		m_SelRXD.ResetContent();
		DBS.m_RoadSum=0;
		DBS.ReadAllRoadName(mdbname);
		for(i=0;i<DBS.m_RoadSum;i++)
			m_SelRXD.AddString(DBS.Road[i].RoadName);
		m_SelRXD.SetCurSel(0);
	}
	GetDlgItem(IDC_COMBO_RXD)->EnableWindow();
	GetDlgItem(IDC_STATIC_RXD)->EnableWindow();		
}



void ZMZDPMFA::OnSelchangeComboRxd() 
{
	// TODO: Add your control notification handler code here
	int roadno=0;
	roadno=m_SelRXD.GetCurSel();
	Roadname = DBS.Road[roadno].RoadName;
	if(mdbname != "")
	{
		XLDataBase DBS;
		//		DBS.Read_XLDbs(mdbname,L"���Ƶ��",Roadname);
		DBS.Read_XLDbs(mdbname,L"��Ԫ��",Roadname);		
		DBS.Read_XLDbs(mdbname,L"������",Roadname);
		if(DBS.XYNum>0)
		{
			if(pzLine) delete pzLine;
			pzLine=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
			//			pzLine->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
			if(DBS.NDL>0)
				pzLine->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pzLine->mdbname,mdbname);
			_tcscpy(pzLine->RoadName,Roadname);
		}

		DBS.Read_XLDbs(mdbname,L"��·����");
		pzLine->DV = DBS.RoadXZ.DV;
	}	
}

void ZMZDPMFA::OnOK()
{
	if (pzLine)
	{
		CDialog::OnOK();
	}
	else
	{
		CDialog::OnCancel();
	}
}





















void ZMZDPMFA::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
	DestroyWindow();
}


void ZMZDPMFA::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
	DestroyWindow();
}
