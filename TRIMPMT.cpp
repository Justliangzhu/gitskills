// TRIMPMT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "TRIMPMT.h"


// CTRIMPMT �Ի���

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


// CTRIMPMT ��Ϣ�������

void CTRIMPMT::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
