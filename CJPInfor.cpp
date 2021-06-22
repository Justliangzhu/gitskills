// CJPInfor.cpp: 实现文件
//

#include "stdafx.h"
#include "CJPInfor.h"
#include "afxdialogex.h"
#include "resource.h"
#include "RoadDesignWorkingDatabase.h"

// CJPInfor 对话框
extern CString Cworkdir, Pro;
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;

IMPLEMENT_DYNAMIC(CJPInfor, CDialog)

CJPInfor::CJPInfor(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_JPInfor, pParent)
	, m_bef_H(_T("0.0"))
	, m_bef_PD(_T("0.0"))
	, m_bef_PC(_T("0.0"))
	, m_aft_H(_T("0.0"))
	, m_aft_PD(_T("0.0"))
	, m_aft_PC(_T("0.0"))
	, m_IsJP(FALSE)
{

}

CJPInfor::~CJPInfor()
{
}

void CJPInfor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_bef_H, m_bef_H);
	DDX_Text(pDX, IDC_EDIT_bef_PD, m_bef_PD);
	DDX_Text(pDX, IDC_EDIT_bef_PC, m_bef_PC);
	DDX_Text(pDX, IDC_EDIT_aft_H, m_aft_H);
	DDX_Text(pDX, IDC_EDIT_aft_PD, m_aft_PD);
	DDX_Text(pDX, IDC_EDIT_aft_PC, m_aft_PC);
	DDX_Check(pDX, IDC_CHECK_IsJP, m_IsJP);
}


BEGIN_MESSAGE_MAP(CJPInfor, CDialog)
	ON_BN_CLICKED(IDOK, &CJPInfor::OnBnClickedOk)
END_MESSAGE_MAP()


// CJPInfor 消息处理程序

BOOL CJPInfor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_MdbName = Bas_Fun.GetCurProMdbPath();
	m_RoadName = L"主线";
	g_RoadWorkingDatabase.GetJPData(m_MdbName, m_RoadName, m_JPInforArray);
	CString WindowText = L"设置接坡信息-当前方案：" + Pro;
	SetWindowText(WindowText);
	if (m_JPInforArray.size() > 0)
	{
		m_bef_H.Format(L"%.3f", m_JPInforArray[0].Bef_H);
		m_bef_PD.Format(L"%.3f", m_JPInforArray[0].Bef_PD);
		m_bef_PC.Format(L"%.3f", m_JPInforArray[0].Bef_PC);
		m_aft_H.Format(L"%.3f", m_JPInforArray[0].Aft_H);
		m_aft_PD.Format(L"%.3f", m_JPInforArray[0].Aft_PD);
		m_aft_PC.Format(L"%.3f", m_JPInforArray[0].Aft_PC);
		if (m_JPInforArray[0].Is_JP == 1)
			m_IsJP = TRUE;
		else
			m_IsJP = FALSE;
	}
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CJPInfor::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
	//保存数据
	m_JPInforArray.clear();
	m_JPInforArray.resize(1);
	m_JPInforArray[0].RoadName = m_RoadName;
	m_JPInforArray[0].Bef_H = _wtof(m_bef_H);
	m_JPInforArray[0].Bef_PD = _wtof(m_bef_PD);
	m_JPInforArray[0].Bef_PC = _wtof(m_bef_PC);
	m_JPInforArray[0].Aft_H = _wtof(m_aft_H);
	m_JPInforArray[0].Aft_PD = _wtof(m_aft_PD);
	m_JPInforArray[0].Aft_PC = _wtof(m_aft_PC);
	if (m_IsJP == TRUE)
		m_JPInforArray[0].Is_JP = 1;
	else
		m_JPInforArray[0].Is_JP = 0;
	g_RoadWorkingDatabase.SetJPData(m_MdbName, m_RoadName, m_JPInforArray, true);
	ads_printf(L"成功保存接坡信息数据至数据库！");

}
