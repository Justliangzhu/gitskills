// COpDrawingSheet.cpp: 实现文件
//

#include "StdAfx.h"
#include "COpDrawingSheet.h"


// COpDrawingSheet

IMPLEMENT_DYNAMIC(COpDrawingSheet, CPropertySheet)

COpDrawingSheet::COpDrawingSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	// 添加四个属性页到属性表   
	//AddPage(&m_OvalParaPage);
	//AddPage(&m_HParaPage);
	//AddPage(&m_VParaPage);
	//AddPage(&m_CroParaPage);
}

COpDrawingSheet::COpDrawingSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// 添加四个属性页到属性表   
	//AddPage(&m_OvalParaPage);
	//AddPage(&m_HParaPage);
	//AddPage(&m_VParaPage);
	//AddPage(&m_CroParaPage);
}

COpDrawingSheet::~COpDrawingSheet()
{
}


BEGIN_MESSAGE_MAP(COpDrawingSheet, CPropertySheet)
END_MESSAGE_MAP()


// COpDrawingSheet 消息处理程序
