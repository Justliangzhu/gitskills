// TRIMPMT.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "TRIMPMT.h"


// CTRIMPMT 对话框

IMPLEMENT_DYNAMIC(CTRIMPMT, CDialog)
	CTRIMPMT::CTRIMPMT(CWnd* pParent /*=NULL*/)
	: CDialog(CTRIMPMT::IDD, pParent)
{
}

CTRIMPMT::~CTRIMPMT()
{
}

void CTRIMPMT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTRIMPMT, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTRIMPMT 消息处理程序

void CTRIMPMT::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
