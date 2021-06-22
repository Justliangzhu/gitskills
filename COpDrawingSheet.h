#pragma once
#include "StdAfx.h"
#include "COverallDrawParaDlg.h"
#include "CHDrawParaDlg.h"
#include "CVDrawParaDlg.h"
#include "CCrossDrawParaDlg.h"

// COpDrawingSheet

class COpDrawingSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(COpDrawingSheet)

public:
	COpDrawingSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	COpDrawingSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~COpDrawingSheet();

	COverallDrawParaDlg m_OvalParaPage;
	CHDrawParaDlg m_HParaPage;
	CVDrawParaDlg m_VParaPage;
	CCrossDrawParaDlg m_CroParaPage;

protected:
	DECLARE_MESSAGE_MAP()
};


