//-----------------------------------------------------------------------------

//----- To Create a new Dialogbar, derive your own class from the
//----- CAcDialogbar class below. Then the implementation is
//----- similar to a normal CDialog derived class
//-----
//----- But do not forget when working with AutoCAD, that this object
//----- is a modeless window

#ifndef _ACDIALOGBAR_H_
#define _ACDIALOGBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAcDialogBar : public CDialogBar {
	DECLARE_DYNAMIC (CAcDialogBar)

	DWORD mAlignment ;
	CSize mSizeCurrent ;

	CWnd *mpFeedbackWnd ;

	BOOL mbTracking ;
	CRect mRectTracker ;
	CPoint mPtDragStart ;
	CPoint mPtDragPrevious ;

	static int cxMinTrack ;
	static int cyMinTrack ;
	static COLORREF color3DFace ;
	static COLORREF color3DShadow ;
	static COLORREF color3DHilight ;
	static COLORREF colorWindowFrame ;

public:
	CAcDialogBar () ;

	//{{AFX_DATA(CAcDialogBar)
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAcDialogBar)
	public:
	virtual BOOL Create(CFrameWnd *pParent, UINT nIDTemplate, UINT style, UINT nID);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	virtual CSize CalcDynamicLayout (int nLength, DWORD dwMode) ;
	virtual CSize CalcFixedLayout (BOOL bStretch, BOOL bHorz) ;

	virtual void DoPaint (CDC *pDC) ;

	UINT GetDockBarID () const ;

	void StretchToPane () ;
	BOOL GetSplitterRect (LPRECT pRect) ;

	void ToggleBarState (BOOL bShrinkIfDocked =TRUE) ;

	void InvertTracker (CWnd *pFeedbackWnd, const CRect &rect) ;
	BOOL StartTracking (CPoint point, BOOL bDoubleClick =FALSE) ;
	BOOL IsTracking (CPoint point) ;
	BOOL EndTracking (CPoint point) ;

	void SetCursor (CPoint point) ;

	virtual UINT MapCtrlToString (UINT uCtrlID) ;

protected:
	//{{AFX_MSG(CAcDialogBar)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg void OnUpdateUI (CCmdUI *pCmdUI) ;
	afx_msg void OnToolTipText (NMHDR *pNotifyStruct, LRESULT *pResult) ;

	DECLARE_MESSAGE_MAP ()
} ;

#endif //----- _ACDIALOGBAR_H_
