#if !defined(AFX_ToolBarXP_H__F739196A_2383_11D5_99BD_5254AB339987__INCLUDED_)
#define AFX_ToolBarXP_H__F739196A_2383_11D5_99BD_5254AB339987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarXP.h : header file
//

struct TOOLBAR_DATA
{
	WORD wVersion;
	WORD wWidth;
	WORD wHeight;
	WORD wItemCount;

	WORD* items()
		{ return (WORD*)(this+1); }
};

/////////////////////////////////////////////////////////////////////////////
// CToolBarXP window

class  CToolBarXP : public CToolBarCtrl
{
// Construction
public:
	CToolBarXP();

// Attributes
private:
	COLORREF	m_clrBack;	
	COLORREF	m_clrHot;
	COLORREF	m_clrSelected;
	COLORREF	m_clrChecked;
	COLORREF	m_clrLine;

	BOOL		m_bDrawLine;	//Draw a line rect around the button when hot or selected

protected:
	void		PreDrawItem(CDC *pDC, const RECT &rect, UINT uState);

// Operations
public:
	void		SetBackColor(COLORREF clr) { m_clrBack = clr; }
	void		SetHotColor(COLORREF clr) { m_clrHot = clr; }
	void		SetSelectedColor(COLORREF clr) { m_clrSelected = clr; }
	void		SetCheckedColor(COLORREF clr) { m_clrChecked = clr; }
	void		SetLineColor(COLORREF clr) {m_clrLine = clr; }

	void		EnableDrawLine(BOOL b) { m_bDrawLine = b; };

	//Load a toolbar resource
	BOOL	LoadToolBar(UINT nID);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarXP)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolBarXP();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarXP)
	//}}AFX_MSG
	afx_msg void OnCustomDraw( NMHDR * pNotifyStruct, LRESULT* result );
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ToolBarXP_H__F739196A_2383_11D5_99BD_5254AB339987__INCLUDED_)
