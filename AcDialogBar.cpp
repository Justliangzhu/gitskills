//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "AcDialogBar.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//-----------------------------------------------------------------------------
// Sample Code to create a dialogbar
//
// AfxSetResourceHandle (_hdllInstance) ;
// CWhatEver *p =new CWhatEver ;
// p->Create (
// 		acedGetAcadFrame (), CWhatEver::IDD,
// 		WS_CHILD | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT | CBRS_SIZE_DYNAMIC,
// 		AFX_IDW_CONTROLBAR_FIRST + 32 + 1
// ) ;
// AfxSetResourceHandle (acedGetAcadResourceInstance ()) ;
// p->EnableDocking (CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT) ;
// acedGetAcadFrame ()->FloatControlBar (p, CPoint (100, 100), CBRS_ALIGN_TOP) ;
// acedGetAcadFrame ()->ShowControlBar (p, TRUE, TRUE) ;

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CAcDialogBar, CDialogBar)

	BEGIN_MESSAGE_MAP (CAcDialogBar, CDialogBar)
		//{{AFX_MSG_MAP(CAcDialogBar)
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		ON_WM_MOUSEMOVE()
		ON_WM_SETCURSOR()
		//}}AFX_MSG_MAP
		ON_UPDATE_COMMAND_UI_RANGE (0x0000, 0xffff, OnUpdateUI) //----- See TN031
	END_MESSAGE_MAP ()

	//-----------------------------------------------------------------------------
	int CAcDialogBar::cxMinTrack =GetSystemMetrics (SM_CXMINTRACK) ;
	int CAcDialogBar::cyMinTrack =GetSystemMetrics (SM_CYMINTRACK) * 3 ;
	COLORREF CAcDialogBar::color3DFace =GetSysColor (COLOR_3DFACE) ;
	COLORREF CAcDialogBar::color3DShadow =GetSysColor (COLOR_3DSHADOW) ;
	COLORREF CAcDialogBar::color3DHilight =GetSysColor (COLOR_3DHILIGHT) ;
	COLORREF CAcDialogBar::colorWindowFrame =GetSysColor (COLOR_WINDOWFRAME) ;

	//-----------------------------------------------------------------------------
	CAcDialogBar::CAcDialogBar () : CDialogBar () {
		//{{AFX_DATA_INIT(CAcDialogBar)
		//}}AFX_DATA_INIT
		mAlignment =CBRS_ALIGN_LEFT ;
		mpFeedbackWnd =NULL ;
		mbTracking =FALSE ;
	}

	//----- Just to make Class Wizard happy
	void CAcDialogBar::DoDataExchange (CDataExchange *pDX) {
		//CDialog::DoDataExchange (pDX) ;
		//{{AFX_DATA_MAP(CAcDialogBar)
		//}}AFX_DATA_MAP
	}

	//-----------------------------------------------------------------------------
	void CAcDialogBar::OnUpdateUI (CCmdUI *pCmdUI) {
	}

	//-----------------------------------------------------------------------------
	UINT CAcDialogBar::MapCtrlToString (UINT uCtrlID) {
		return (0) ;
	}

	void CAcDialogBar::OnToolTipText (NMHDR *pNotifyStruct, LRESULT *pResult) {
		TOOLTIPTEXT &t =*((TOOLTIPTEXT *)pNotifyStruct) ;

		if ( t.uFlags & TTF_IDISHWND ) {
			//----- idFrom is actually the HWND of the tool
			UINT nID =MapCtrlToString (::GetDlgCtrlID ((HWND)pNotifyStruct->idFrom)) ;
			if( nID ) {
				t.lpszText =MAKEINTRESOURCE (nID) ;
				t.hinst =AfxGetResourceHandle () ;
			}
		}
	}

	//-----------------------------------------------------------------------------
	BOOL CAcDialogBar::Create (CFrameWnd *pParent, UINT nIDTemplate, UINT style, UINT nID) {
		if ( !CDialogBar::Create (pParent, nIDTemplate, style, nID) )
			return (FALSE) ;
		mSizeCurrent =m_sizeDefault ;
		return (TRUE) ;
	}

	//-----------------------------------------------------------------------------
	CSize CAcDialogBar::CalcDynamicLayout (int nLength, DWORD dwMode) {
		if ( dwMode == LM_HORZ )
			m_sizeDefault.cx =mSizeCurrent.cx =nLength ;
		else if ( dwMode == LM_HORZ + LM_LENGTHY )
			m_sizeDefault.cy =mSizeCurrent.cy =nLength ;
		return (CDialogBar::CalcDynamicLayout (nLength, dwMode)) ;
	}

	CSize CAcDialogBar::CalcFixedLayout (BOOL bStretch, BOOL bHorz) {
		if ( bStretch ) //----- If not docked stretch to fit
			return (CSize (bHorz ? 32767 : m_sizeDefault.cx, bHorz ? m_sizeDefault.cy : 32767)) ;
		else
			return (m_sizeDefault) ;
	}

	//-----------------------------------------------------------------------------
	void CAcDialogBar::DoPaint (CDC *pDC) {
		//----- Call CDialogBar::DoPaint()
		CDialogBar::DoPaint (pDC) ;
		//----- Draw the Splitter bar
		CRect rectSplitter ;
		if ( GetSplitterRect (&rectSplitter) ) {
			//----- Border
			pDC->Draw3dRect (rectSplitter, colorWindowFrame, colorWindowFrame) ;
			rectSplitter.InflateRect (-1, -1) ;
			//----- 3D effect
			pDC->Draw3dRect (rectSplitter, color3DHilight, color3DShadow) ;
			rectSplitter.InflateRect (-1, -1) ;
			//----- Interior
			pDC->FillSolidRect (rectSplitter, color3DFace) ;
		}
	}

	//-----------------------------------------------------------------------------
	UINT CAcDialogBar::GetDockBarID () const {
		switch ( mAlignment ) {
		case CBRS_ALIGN_TOP:
			return (AFX_IDW_DOCKBAR_TOP) ;
		case CBRS_ALIGN_BOTTOM:
			return (AFX_IDW_DOCKBAR_BOTTOM) ;
		case CBRS_ALIGN_LEFT:
			return (AFX_IDW_DOCKBAR_LEFT) ;
		case CBRS_ALIGN_RIGHT:
			return (AFX_IDW_DOCKBAR_RIGHT) ;
		}
		return (0) ;
	}

	//-----------------------------------------------------------------------------
	void CAcDialogBar::StretchToPane () {
		if ( GetSafeHwnd () != NULL && CDialogBar::IsWindowVisible () ) {
			CRect rect, rectd ;
			GetParent ()->GetClientRect (rect) ;
			GetClientRect (rectd) ;
			if ( rectd.Height () == rect.Height () )
				return ;
			SetWindowPos (NULL, -1, -1, rect.Width () + 2, rect.Height () + 2, SWP_NOZORDER | SWP_NOACTIVATE) ;
			//----- Post a WM_SIZE message to ourselves that we will receive after
			//----- the floating/docked information will have been updated.
			PostMessage (WM_SIZE, SIZE_RESTORED, MAKELONG (rect.Width () + 2, rect.Height () + 2)) ;
		}
	}

	BOOL CAcDialogBar::GetSplitterRect (LPRECT pRect) {
		if ( IsFloating () )
			return (FALSE) ;
		//----- We continue only if we are docked
		StretchToPane () ;
		GetClientRect (pRect) ;
		switch ( mAlignment =(GetBarStyle () & CBRS_ALIGN_ANY) ) {
		case CBRS_ALIGN_TOP:
			pRect->top =pRect->bottom - 6 ;
			return (TRUE) ;
		case CBRS_ALIGN_LEFT:
			pRect->left =pRect->right - 6 ;
			return (TRUE) ;
		case CBRS_ALIGN_RIGHT:
			pRect->right =pRect->left + 6 ;
			return (TRUE) ;
		case CBRS_ALIGN_BOTTOM:
			pRect->bottom =pRect->top + 6 ;
			return (TRUE) ;
		}
		return (FALSE) ;
	}

	//-----------------------------------------------------------------------------
	void CAcDialogBar::ToggleBarState (BOOL bShrinkIfDocked /*=TRUE*/) {
		if ( !(GetStyle () & WS_VISIBLE) || IsFloating () || !bShrinkIfDocked ) {
			m_pDockSite->ShowControlBar (this, (GetStyle () & WS_VISIBLE) == 0, FALSE) ;
		} else {
			if ( mSizeCurrent.cx <= 6 || mSizeCurrent.cy <= 6 ) {
				mSizeCurrent =m_sizeDefault ;
			} else {
				mSizeCurrent.cx =6 ;
				mSizeCurrent.cy =6 ;
			}
			m_pDockSite->RecalcLayout () ;
			Invalidate () ;
		}
	}

	void CAcDialogBar::InvertTracker (CWnd *pFeedbackWnd, const CRect &rect) {
		ASSERT ( !rect.IsRectEmpty () ) ;
		CDC *pDC =pFeedbackWnd->GetDC () ;
		//----- Invert the brush pattern (looks just like frame window sizing)
		CBrush *pBrush =CDC::GetHalftoneBrush () ;
		HBRUSH hOldBrush =NULL ;
		if ( pBrush != NULL )
			hOldBrush =(HBRUSH)SelectObject (pDC->m_hDC, pBrush->m_hObject) ;
		pDC->PatBlt (rect.left, rect.top, rect.Width (), rect.Height (), PATINVERT) ;
		if ( hOldBrush != NULL )
			SelectObject (pDC->m_hDC, hOldBrush) ;
		pFeedbackWnd->ReleaseDC (pDC) ;
	}

	BOOL CAcDialogBar::StartTracking (CPoint point, BOOL bDoubleClick /*=FALSE*/) {
		if ( GetSplitterRect (&mRectTracker) && mRectTracker.PtInRect (point) ) {
			if ( bDoubleClick ) {
				ToggleBarState () ;
			} else {
				mbTracking =TRUE ;
				mPtDragStart =point ;
				mPtDragPrevious =point ;
				//----- Capture mouse
				SetCapture () ;
				mpFeedbackWnd =GetTopLevelFrame () ;
				//----- Make sure no updates are pending in the feeback window
				mpFeedbackWnd->RedrawWindow (NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW) ;
				ClientToScreen (&mRectTracker) ;
				mpFeedbackWnd->ScreenToClient (&mRectTracker) ;
				mRectTracker.InflateRect (-1,-1) ;
				InvertTracker (mpFeedbackWnd, mRectTracker) ;
			}
			return (TRUE) ;
		}
		ASSERT ( mbTracking == FALSE ) ;
		return (FALSE) ;
	}

	BOOL CAcDialogBar::IsTracking (CPoint point) {
		if ( mbTracking ) {
			InvertTracker (mpFeedbackWnd, mRectTracker) ;
			switch ( mAlignment ) {
			case CBRS_ALIGN_LEFT:
			case CBRS_ALIGN_RIGHT:
				mRectTracker.OffsetRect (point.x - mPtDragPrevious.x, 0) ;
				break ;
			case CBRS_ALIGN_TOP:
			case CBRS_ALIGN_BOTTOM:
				mRectTracker.OffsetRect (0, point.y - mPtDragPrevious.y) ;
				break ;
			}
			InvertTracker (mpFeedbackWnd, mRectTracker) ;
			mPtDragPrevious =point ;
			return (TRUE) ;
		} else {
			SetCursor (point) ;
		}
		return (FALSE) ;
	}

	BOOL CAcDialogBar::EndTracking (CPoint point) {
		if ( IsTracking (point) ) {
			ReleaseCapture () ;
			InvertTracker (mpFeedbackWnd, mRectTracker) ;
			ASSERT ( mAlignment == (GetBarStyle () & CBRS_ALIGN_ANY) ) ;
			switch ( mAlignment ) {
			case CBRS_ALIGN_LEFT:
				mSizeCurrent.cx +=point.x - mPtDragStart.x ;
				break ;
			case CBRS_ALIGN_RIGHT:
				mSizeCurrent.cx -=point.x - mPtDragStart.x ;
				break ;
			case CBRS_ALIGN_TOP:
				mSizeCurrent.cy +=point.y - mPtDragStart.y ;
				break ;
			case CBRS_ALIGN_BOTTOM:
				mSizeCurrent.cy -=point.y - mPtDragStart.y ;
				break ;
			}
			switch ( mAlignment ) {
			case CBRS_ALIGN_LEFT:
			case CBRS_ALIGN_RIGHT:
				if ( mSizeCurrent.cx <= 18 ) {
					mSizeCurrent.cx =6 ;
				} else {
					if ( mSizeCurrent.cx <= cxMinTrack )
						mSizeCurrent.cx =cxMinTrack ;
					m_sizeDefault.cx =mSizeCurrent.cx ;
				}
				break ;
			case CBRS_ALIGN_TOP:
			case CBRS_ALIGN_BOTTOM:
				if ( mSizeCurrent.cy <= 18 ) {
					mSizeCurrent.cy =6 ;
				} else {
					if ( mSizeCurrent.cy <= cyMinTrack )
						mSizeCurrent.cy =cyMinTrack ;
					m_sizeDefault.cy =mSizeCurrent.cy ;
				}
				break ;
			}
			mbTracking =FALSE ;
			GetTopLevelFrame ()->RecalcLayout (TRUE) ;
			Invalidate () ;
			return (TRUE) ;
		}
		return (FALSE) ;
	}

	void CAcDialogBar::SetCursor (CPoint point) {
		//----- Cache of available cursors
		static HCURSOR hcurArrow =NULL ;
		static HCURSOR hcurVSplit =NULL ;
		static HCURSOR hcurHSplit =NULL ;

		HCURSOR hCursor ;
		CRect rect ;

		if ( GetSplitterRect (&rect) && rect.PtInRect (point) ) {
			switch ( mAlignment ) {
			case CBRS_ALIGN_TOP:
			case CBRS_ALIGN_BOTTOM:
				if ( hcurVSplit == NULL ) {
					if ( (hcurVSplit =::LoadCursor (
						AfxFindResourceHandle (
						MAKEINTRESOURCE (AFX_IDC_VSPLITBAR),
						RT_GROUP_CURSOR
						),
						MAKEINTRESOURCE (AFX_IDC_VSPLITBAR)
						)
						) == NULL
						) {
							//----- Will not look as good
							TRACE0 (_T(/*NOXLATE*/"Warning: Could not find splitter cursor - using system provided alternative.\n")) ;
							hcurVSplit =::LoadCursor (NULL, IDC_SIZENS) ;
					}
				}
				hCursor =hcurVSplit ;
				break ;
			case CBRS_ALIGN_LEFT:
			case CBRS_ALIGN_RIGHT:
				if ( hcurHSplit == NULL ) {
					if ( (hcurHSplit =::LoadCursor (
						AfxFindResourceHandle (
						MAKEINTRESOURCE (AFX_IDC_HSPLITBAR),
						RT_GROUP_CURSOR
						),
						MAKEINTRESOURCE (AFX_IDC_HSPLITBAR)
						)
						) == NULL
						) {
							//----- Will not look as good
							TRACE0 (_T(/*NOXLATE*/"Warning: Could not find splitter cursor - using system provided alternative.\n")) ;
							hcurHSplit =::LoadCursor (NULL, IDC_SIZEWE) ;
					}
				}
				hCursor =hcurHSplit ;
				break ;
			}
		} else {
			if ( hcurArrow == NULL )
				hcurArrow =::LoadCursor (NULL, IDC_ARROW) ;
			hCursor =hcurArrow ;
		}
		ASSERT ( hCursor != NULL ) ;
		::SetCursor (hCursor) ;
	}

	//-----------------------------------------------------------------------------
	void CAcDialogBar::OnLButtonDown (UINT nFlags, CPoint point) {
		if ( (nFlags & MK_CONTROL) || !StartTracking (point) )
			CDialogBar::OnLButtonDown (nFlags, point) ;
	}

	void CAcDialogBar::OnLButtonUp (UINT nFlags, CPoint point) {
		if ( !EndTracking (point) )
			CDialogBar::OnLButtonUp (nFlags, point) ;
	}

	void CAcDialogBar::OnLButtonDblClk (UINT nFlags, CPoint point) {
		if ( (nFlags & MK_CONTROL) || !StartTracking (point, TRUE) )
			CDialogBar::OnLButtonDblClk (nFlags, point) ;
	}

	void CAcDialogBar::OnMouseMove (UINT nFlags, CPoint point) {
		if ( !IsTracking (point) )
			CDialogBar::OnMouseMove (nFlags, point) ;
	}

	BOOL CAcDialogBar::OnSetCursor (CWnd *pWnd, UINT nHitTest, UINT message) {
		if ( nHitTest == HTCLIENT && pWnd == this && !mbTracking )
			return (TRUE) ; //----- Handled in IsTracking called by OnMouseMove
		return (CDialogBar::OnSetCursor (pWnd, nHitTest, message)) ;
	}
