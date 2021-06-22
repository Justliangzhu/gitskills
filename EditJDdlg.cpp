// EditJDdlg.cpp : implementation file
//
#include "stdafx.h"
#include "StdArx.h"
#include "EditJDdlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditJDdlg dialog
EditJDdlg::EditJDdlg(CWnd* pParent /*=NULL*/,AcDbObjectId eId)
	: CDialog(EditJDdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditJDdlg)
	m_zxId = eId;
	m_gzxs = FALSE;
	//}}AFX_DATA_INIT
}


void EditJDdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditJDdlg)
	DDX_Control(pDX, IDC_COMBOJD, m_jdsel);
	DDX_Check(pDX, IDC_CHECKGZ, m_gzxs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditJDdlg, CDialog)
	//{{AFX_MSG_MAP(EditJDdlg)
	ON_BN_CLICKED(IDC_BUTTONAPPLY, OnButtonapply)
	ON_BN_CLICKED(IDC_CHECKGZ, OnCheckgz)
	ON_CBN_SELCHANGE(IDC_COMBOJD, OnSelchangeCombojd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditJDdlg message handlers

void EditJDdlg::OnButtonapply() 
{
	// TODO: Add your control notification handler code here
	int i;
	CString str;
	AcDbObject *pObj;
	UpdateData();
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	/* acdbOpenObject(pObj, m_zxId, AcDb::kForWrite);*/
	if(acdbOpenObject(pObj,m_zxId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pzx = JD_CENTER::cast(pObj);
	pzx->assertWriteEnabled();

	i=m_jdsel.GetCurSel();
	GetDlgItemText(IDC_EDITX,str);	
	pzx->JdCenArray[i].N=_wtof(str);
	GetDlgItemText(IDC_EDITY,str);	
	pzx->JdCenArray[i].E=_wtof(str);
	GetDlgItemText(IDC_EDITBJ,str);	
	pzx->JdCenArray[i].R=_wtof(str);
	GetDlgItemText(IDC_EDITQHC,str);	
	pzx->JdCenArray[i].Ls1=_wtof(str);
	GetDlgItemText(IDC_EDITHHC,str);	
	pzx->JdCenArray[i].Ls2=_wtof(str);
	pzx->close();	
	acDocManager->unlockDocument(acDocManager->curDocument());
	drawdata(i);
	m_jdsel.SetFocus();
	UpdateData(false);
}

void EditJDdlg::OnCheckgz() 
{
	// TODO: Add your control notification handler code here
	m_gzxs = !m_gzxs;
}

void EditJDdlg::OnSelchangeCombojd() 
{
	// TODO: Add your control notification handler code here
	int i=0;
	ads_point pt1;

	i=m_jdsel.GetCurSel();
	/*
	acdbOpenObject(pzx, m_zxId, AcDb::kForRead);	 
	pzx->assertReadEnabled();
	pzx->close();*/

	if(m_gzxs)
	{
		struct resbuf res;
		res.restype = RTSHORT;
		res.resval.rint = 0;
		acedSetVar(L"cmdecho",&res);
		ACHAR cmdstr[80];
		double ht=300;
		pt1[X]=pzx->JdCenArray[i].N,	pt1[Y]=pzx->JdCenArray[i].E;
		FILE *pfw;
		pfw = _wfopen(L"c:\\tmp.scr", L"w");
		fwprintf(pfw , L"zoom\n");
		fwprintf(pfw , L"C\n");
		fwprintf(pfw , L"%lf,%lf\n",pt1[Y],pt1[X]);
		fwprintf(pfw , L"%lf\n",ht);
		fclose(pfw);
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		acedGetAcadDwgView()->SetFocus();
		acDocManager->activateDocument(acDocManager->curDocument());
		_tcscpy(cmdstr , L"(command \"script\" \"c:\\\\tmp\")\n");
		acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);	
		acDocManager->unlockDocument(acDocManager->curDocument());
		m_jdsel.SetFocus();
	}

	drawdata(i);
	UpdateData(false);
	return;
}

void EditJDdlg::drawdata(int i)
{
	ACHAR str[80];	
	if(i==0)
		_stprintf(str,L"起点");
	else if(i==pzx->JdCenNum-1)
		_stprintf(str,L"终点");
	else
	{
		if(pzx->QXB[i][8]==-1)
			_stprintf(str,L"Az%0.6lf",pzx->QXB[i][4]);	
		else
			_stprintf(str,L"Ay%0.6lf",pzx->QXB[i][4]);	
	}
	GetDlgItem(IDC_STATICZXJ)->SetWindowText(str);
	_stprintf(str,L"%0.6lf",pzx->JdCenArray[i].N);
	GetDlgItem(IDC_EDITX)->SetWindowText(str);
	_stprintf(str,L"%0.6lf",pzx->JdCenArray[i].E);
	GetDlgItem(IDC_EDITY)->SetWindowText(str);

	//现场里程转统一里程 再转带冠号的现场里程
	ACHAR gh[8],ckml[80] ;
	double xlc,lc,len;
	_stprintf(str,L"%0.6lf",pzx->JdCenArray[i].R);
	GetDlgItem(IDC_EDITBJ)->SetWindowText(str);
	_stprintf(str,L"%0.6lf",pzx->JdCenArray[i].Ls1);
	GetDlgItem(IDC_EDITQHC)->SetWindowText(str);
	_stprintf(str,L"%0.6lf",pzx->JdCenArray[i].Ls2);
	GetDlgItem(IDC_EDITHHC)->SetWindowText(str);	

}
BOOL EditJDdlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int i;
	ACHAR str[80];
	AcDbObject *pObj;
	/*acdbOpenObject(pObj, m_zxId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, m_zxId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return false;
	}
	pzx = JD_CENTER::cast(pObj);
	pzx->assertReadEnabled();
	pzx->close();

	for(i=0;i<pzx->JdCenNum;i++)
	{
		_stprintf(str,L"JD%d",pzx->JD0+i);
		m_jdsel.AddString(str);
	}
	m_jdsel.SetCurSel(1);

	drawdata(1);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void EditJDdlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	int i=m_jdsel.GetCurSel();
	// TODO: Add your message handler code here
	/*
	acdbOpenObject(pzx, m_zxId, AcDb::kForRead);	 
	pzx->assertReadEnabled();
	pzx->close();*/

	drawdata(i);
	UpdateData(false);
	// Do not call CDialog::OnPaint() for painting messages

}

BOOL EditJDdlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if(pMsg->message==WM_KEYDOWN)
	{	
		if(pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return true;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);

}
