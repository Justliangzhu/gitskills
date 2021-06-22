// ToolBarXP.cpp : implementation file
//

#include "stdafx.h"
#include "ToolBarXP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolBarXP

CToolBarXP::CToolBarXP()
{
	m_clrBack = RGB(239, 237, 222);
	m_clrHot = RGB(193, 210, 238);
	m_clrChecked = m_clrHot;
	m_clrSelected = m_clrChecked;
	m_clrLine = RGB(49, 106, 197);

	m_bDrawLine = TRUE;
}

CToolBarXP::~CToolBarXP()
{
}


BEGIN_MESSAGE_MAP(CToolBarXP, CToolBarCtrl)
	//{{AFX_MSG_MAP(CToolBarXP)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolBarXP message handlers
BOOL CToolBarXP::LoadToolBar(UINT nID)
{
	if(!m_hWnd)return FALSE;

	HRSRC hRsrc = ::FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(nID), RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(AfxGetInstanceHandle(), hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	TOOLBAR_DATA* pData = (TOOLBAR_DATA*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;

	BOOL bResult;
	TBBUTTON tbb;
	memset(&tbb,0,sizeof(TBBUTTON));
	tbb.iString = -1;
	int iImage = 0;

	for (int i = 0; i < pData->wItemCount; i++)
	{
		tbb.fsState = TBSTATE_ENABLED;
		tbb.idCommand = pData->items()[i];
		if(tbb.idCommand == 0)
		{
			tbb.fsStyle = TBSTYLE_SEP;
			tbb.iBitmap = -1;
		}
		else
		{
			tbb.fsStyle = TBSTYLE_BUTTON;
			tbb.iBitmap = iImage++;
		}

		bResult = AddButtons(1,&tbb);

		if(!bResult)return FALSE;
	}

	if(bResult)
	{
		TBADDBITMAP tbab;

		tbab.hInst = AfxGetInstanceHandle();
		tbab.nID = nID;
		SendMessage(TB_ADDBITMAP,(WPARAM)pData->wItemCount,(LPARAM)&tbab);
	}

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bResult;
}

//////////////////////////////////////////////////////////////////////////
//	Process CustomDraw notification, the drawing code is all from here
void CToolBarXP::OnCustomDraw( NMHDR * pNotifyStruct, LRESULT* pResult )
{
	NMTBCUSTOMDRAW *pnmcd = (NMTBCUSTOMDRAW *)pNotifyStruct;

	CDC	dc;
	dc.Attach(pnmcd->nmcd.hdc);

	switch (pnmcd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		PreDrawItem(&dc, pnmcd->nmcd.rc, pnmcd->nmcd.uItemState);
		*pResult = TBCDRF_NOEDGES;
		break;
	case CDDS_ITEMPOSTPAINT:
		break;
	}

	dc.Detach();
}

void CToolBarXP::PreDrawItem(CDC *pDC, const RECT &rect, UINT uState)
{
	CRect	rcDraw = rect;
	if (uState & CDIS_HOT)
	{
		CPen	pen(PS_SOLID, 1, m_clrLine);
		CPen	*pOldPen = pDC->SelectObject(&pen);
		CBrush	brush(m_clrHot);
		CBrush	*pOldBr = pDC->SelectObject(&brush);
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOldBr);
		pDC->SelectObject(pOldPen);
	}
	else if (uState & CDIS_CHECKED)
	{
		CPen	pen(PS_SOLID, 1, m_clrLine);
		CPen	*pOldPen = pDC->SelectObject(&pen);
		CBrush	brush(m_clrChecked);
		CBrush	*pOldBr = pDC->SelectObject(&brush);
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOldBr);
		pDC->SelectObject(pOldPen);
	}
	else if (uState & CDIS_SELECTED)
	{
		CPen	pen(PS_SOLID, 1, m_clrLine);
		CPen	*pOldPen = pDC->SelectObject(&pen);
		CBrush	brush(m_clrSelected);
		CBrush	*pOldBr = pDC->SelectObject(&brush);
		pDC->Rectangle(&rect);
		pDC->SelectObject(pOldBr);
		pDC->SelectObject(pOldPen);
	}
}


