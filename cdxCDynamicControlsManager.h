// cdxCDynamicControlsManager.h: interface for the cdxCDynamicControlsManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDXCDYNAMICCONTROLSMANAGER_H__6517AE13_5D12_11D2_BE4C_000000000000__INCLUDED_)
#define AFX_CDXCDYNAMICCONTROLSMANAGER_H__6517AE13_5D12_11D2_BE4C_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include	"cdxCSizeIconCtrl.h"
typedef cdxCSizeIconCtrl	cdxCSizeCtrl;

//
// cdxCDynamicControlsManager.h : header file
// -----------------------------------------------------------------------
// Author:  Hans Bühler (hans.buehler@student.hu-berlin.de)
//          codex design (http://www-pool.mathematik.hu-berlin.de/~codex
// Version: 1.3
// Release: 3 (Jan 1999 to www.codeguru.com)
// -----------------------------------------------------------------------
// Changes for V1.1:
// - cdxCSizeCtrl is now only a typedef on cdxCSizeIconCtrl which is been
//   but in two extra files (header and impl.) to make it available to
//   the programmer even if you don't use cdxCDynamicControls.
//   The include/impl file for cdxCSizeIconCtrl must be available.
// - GetSizeIconBitmap() is been deleted.
// - ICON_CONTROL_ID has been changed to AFX_IDW_SIZE_BOX
// Changes for V1.2:
// - cdxCDynamicControlsManager::DoOnGetMinMaxInfo() has been modified
//   (thanks to a bug report by Michel Wassink <mww@mitutoyo.nl>):
//   Now, if you don't call SetMaxSize(), the maximum position of a window
//   will not be changed.
//	  BUG: Under W95 and W98, resizing didn't work properly REMOVED.
// Changes for V1.3:
// - FindSzControl() and RemSzControl() have been added.
// -----------------------------------------------------------------------
// Comments welcome.
//

/*
 * cdxCDynamicControlsManager
 * ==========================
 * Makes any CWnd derived class capable of dynamic control resizing
 * and repositioning.
 * Moreover, it can set a window's max/min tracking size (the size
 * the user can change) and add a nice sizing icon to the windows
 * lower right corner (if the window does not have one - as dialogs).
 *
 * To make any CWnd derived capable of automatically displaying
 * its controls, you embed a member of this class in your window
 * (or you derive your class from both this class and your window
 * base class - that depends on how you want to use the member
 * functions of this class).
 *
 * Then, the following functions must be called
 *
 *		DoInitWindow()			-	Must be called after the window became
 *										valid (i.e. CWnd::m_hWnd is non-zero)
 *										and has its initial (minimum) size.
 *		DoOnSize()				-	by the OnSize() message handler
 *		DoOnGetMinMaxInfo()	-	by the OnGetMinMaxInfo() message handler
 *		DoDestroyWindow()		-	by DestroyWindow().
 *
 * See cdxCSizingDialog.h for an example.
 * 
 * NOTE:
 * Unfortunately, we cannot derive this class from CObject, because
 * those macros DECLARE_xxx are too lame to handle multipile derived
 * classes if both classes have been derived from the same base-class
 * CObject.
 */

class cdxCDynamicControlsManager
{
	//
	// various constants
	//
public:
	enum Mode		// flags for AddSzControl()
	{
		mdNone		=	0,					// does nothing
		mdResize		=	1,					// resize in that dimension
		mdRepos		=	2,					// reposition
		mdRelative	=	3,					// center (size by delta/2 and repos by delta/2)
		md__Last		=	mdRelative
	};

	enum Freedom
	{
		fdNone		=	0,					// might be used but I don't imagine what you want from this ??
		fdHoriz		=	0x01,				// horizantally sizable only
		fdVert		=	0x02,				// vertically sizable only
		fdAll			=	fdHoriz|fdVert	// sizable in all directions
	};

	enum RestoreFlags
	{
		rflg_none			=	0,			// only load window position
		rflg_state			=	0x01,		// make window iconic/zoomed if been before
		rflg_visibility	=	0x02,		// hide/show window as been before
		rflg_all				=	rflg_state|rflg_visibility
	};

	enum
	{
		ICON_CONTROL_ID	=	AFX_IDW_SIZE_BOX
	};

	//
	// an astract handle to a sizeable control that you can
	// use to add further controls to
	// see discussion of AddSzControl()
	//
public:
	class ControlPosition
	{
	protected:
		ControlPosition() {}
	public:
		virtual ~ControlPosition() {}

	public:
		virtual void Add(CWnd & ctrl) = NULL;
	};

	//
	// internal storage class for controls and their
	// original positions and their behaviour settings
	//
private:
	class ControlData : public ControlPosition
	{
		//
		// all controls with the same positioning arguments
		// (used by Add())
		// Note that the window is not need to be already created
		//
	private:
		struct Control
		{
		private:
			Control	*m_pNext;		// next
			CWnd		& m_rCtrl;		// the control

		public:
			Control(CWnd & ctrl, Control *pNext = NULL) : m_pNext(pNext), m_rCtrl(ctrl) {}
			virtual ~Control() { if(m_pNext) delete m_pNext; }	// no need to be virtual

			bool IsMemberOf(CWnd & ctrl) { if(&ctrl == &m_rCtrl) return true; return m_pNext ? m_pNext->IsMemberOf(ctrl) : false; }

			void Add(CWnd & ctrl, int x, int y, int wid, int hi);
			void Position(int x, int y, int wid, int hi, bool bAl);
		};

		friend struct Control;

	private:
		cdxCDynamicControlsManager
								& m_rMaster;			// the master class
		ControlData			*m_pNext,*m_pPrev;	// a linked list (root in m_rMaster.m_pFirst)
		Control				m_Ctrl;					// control link list
		BYTE					m_dX1pcnt,m_dX2pcnt,
								m_dY1pcnt,m_dY2pcnt;	// percentage values of deltaX(Y) to add to coordinate
		CRect					m_rectOriginal;		// original position of control(s)

	public:
		ControlData(cdxCDynamicControlsManager & rMaster, CWnd & ctrl, BYTE dX1pcnt, BYTE dX2pcnt, BYTE dY1pcnt, BYTE dY2pcnt);
		virtual ~ControlData();

		bool IsMemberOf(CWnd & ctrl) { return m_Ctrl.IsMemberOf(ctrl); }

		void OnSize(const CSize & szDelta);
		ControlData *GetNext(void) { return m_pNext; }

		virtual void Add(CWnd & ctrl) { m_Ctrl.Add(ctrl,m_rectOriginal.left,m_rectOriginal.top,m_rectOriginal.Width(),m_rectOriginal.Height()); }

	};

	//
	// my members
	//

	friend class ControlData;

private:
	ControlData		*m_pFirst;
	CWnd				*m_pWnd;					// Use Init() !!!!!!!!!
	CSize				m_szClientRelative,	// original's window size (client !!) - used in OnSize() to calculate delta size
						m_szMin,					// minimum size (whole window)
						m_szMax;					// maximum (whole window)
	Freedom			m_Freedom;				// what is allowed
	cdxCSizeCtrl	*m_pWndSizeIcon;		// the icon control
public:
	UINT				m_idSizeIcon;			// ID of the icon control (you can set this to change the default, ICON_CONTROL_ID)

public:
	cdxCDynamicControlsManager() : m_pFirst(NULL), m_pWnd(NULL), m_Freedom(fdAll), m_pWndSizeIcon(NULL), m_idSizeIcon(ICON_CONTROL_ID) {}
	virtual ~cdxCDynamicControlsManager() { DoDestroyWindow(); }

	//
	// check status
	//

	bool IsReady(void) const { return (m_pWnd != NULL) && ::IsWindow(m_pWnd->m_hWnd); }

	//
	// get some basics
	//

	const CSize & GetMinSize(void) const { return m_szMin; }
	const CSize & GetMaxSize(void) const { return m_szMax; }
	Freedom GetFreedom(void) const { return m_Freedom; }

	//
	// wanna change some basics ?
	//
	bool SetMinMaxSize(const CSize & szMin, const CSize & szMax, bool bResizeIfNecessary = true);
	bool FixWindowSize(void);
	void SetFreedom(Freedom fd) { m_Freedom = fd; }
	void HideSizeIcon(void);
	void ShowSizeIcon(void);

	//
	// add controls to handle
	//
	ControlPosition *AddSzControl(CWnd & ctrl, Mode modeX, Mode modeY);
	ControlPosition *AddSzXControl(CWnd & ctrl, Mode modeX) { return AddSzControl(ctrl,modeX,mdNone); }
	ControlPosition *AddSzYControl(CWnd & ctrl, Mode modeY) { return AddSzControl(ctrl,mdNone,modeY); }

	virtual ControlPosition *AddSzControlEx(CWnd & ctrl, BYTE dX1pcnt, BYTE dX2pcnt, BYTE dY1pcnt, BYTE dY2pcnt);
	ControlPosition *AddSzXControlEx(CWnd & ctrl, BYTE dX1pcnt, BYTE dX2pcnt) { return AddSzControlEx(ctrl,dX1pcnt,dX2pcnt,0,0); }
	ControlPosition *AddSzYControlEx(CWnd & ctrl, BYTE dY1pcnt, BYTE dY2pcnt) { return AddSzControlEx(ctrl,0,0,dY1pcnt,dY2pcnt); }

	//
	// advanced (new to V1.3)
	//
	ControlPosition *FindSzControl(CWnd & ctrl);
	bool RemSzControl(CWnd & ctrl) { ControlPosition *p = FindSzControl(ctrl); if(!p) return false; delete p; return true; }

	//
	// these must be called by the appropiate message handlers of the window
	// class you're deriving from
	//
	void DoInitWindow(CWnd & rWnd, Freedom fd = fdAll, bool bSizeIcon = false, const CSize * pBaseClientSize = NULL);
	void DoOnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	void DoOnSize(UINT nType, int cx, int cy);
	void DoDestroyWindow(void);

	//
	// some helpers
	//
	void ReorganizeControls(bool bRedraw = true);
	void ReorganizeControlsAdvanced(const CRect & rectWin, CRect rectClient, bool bRedraw = true);
	bool StretchWindow(const CSize & szDelta) { ASSERT(IsReady()); return StretchWindow(*m_pWnd,szDelta); }
	bool StretchWindow(int iAddPcnt) { ASSERT(IsReady()); return StretchWindow(*m_pWnd,iAddPcnt); }
	CSize GetWindowSize(void) { ASSERT(IsReady()); return GetWindowSize(*m_pWnd); }
	bool RestoreWindowPosition(LPCTSTR lpszProfile, UINT restoreFlags = rflg_none) { ASSERT(IsReady()); return RestoreWindowPosition(*m_pWnd,lpszProfile,restoreFlags); }
	bool StoreWindowPosition(LPCTSTR lpszProfile) { ASSERT(IsReady()); return StoreWindowPosition(*m_pWnd,lpszProfile); }


	//
	// helpers; static
	//
	static bool StretchWindow(CWnd & rWnd, const CSize & szDelta);
	static bool StretchWindow(CWnd & rWnd, int iAddPcnt);
	static CSize GetWindowSize(CWnd & rWnd);
	static bool RestoreWindowPosition(CWnd & rWnd, LPCTSTR lpszProfile, UINT restoreFlags = rflg_none);
	static bool StoreWindowPosition(CWnd & rWnd, LPCTSTR lpszProfile);

	//
	// misc
	//
public:
	/* removed */ //static CBitmap & GetSizeIconBitmap(CSize * pSzBmp = NULL);
	static CImageList & GetSizeIconImageList(CSize * pSzBmp = NULL) { if(pSzBmp) *pSzBmp = cdxCSizeIconCtrl::M_ilImage.Size(); return cdxCSizeIconCtrl::M_ilImage; }
};

/*
 * cdxCSizeCtrl
 * ============
 * Is now a typedef to cdxCSizeIconCtrl - see above.
 */

/////////////////////////////////////////////////////////////////////////////
// cdxCDynamicControlsManager::ControlData::Control inlines
/////////////////////////////////////////////////////////////////////////////

/*
 * add a control that has the same coordinates as the
 * control embedded in the ControlData object.
 * The coordinates are needed to immediately place the
 * control to the original control's position.
 */

inline void cdxCDynamicControlsManager::ControlData::Control::Add(CWnd & ctrl, int x, int y, int wid, int hi)
{
	VERIFY( m_pNext = new Control(ctrl,m_pNext) );
	m_pNext->Position(x,y,wid,hi,false);
}

/*
 * apply new position to all "Control" controls
 * we don't change the z-order here !
 */

inline void cdxCDynamicControlsManager::ControlData::Control::Position(int x, int y, int wid, int hi, bool bAll)
{
	if(::IsWindow(m_rCtrl.m_hWnd))		// those window don't need to exist :)
	{
		VERIFY( m_rCtrl.SetWindowPos(&CWnd::wndBottom,x,y,wid,hi,
						SWP_NOCOPYBITS|SWP_NOOWNERZORDER|
						SWP_NOACTIVATE|SWP_NOZORDER) );
	}
	if(m_pNext && bAll)
		m_pNext->Position(x,y,wid,hi,true);
}

/////////////////////////////////////////////////////////////////////////////
// cdxCDynamicControlsManager inlines
/////////////////////////////////////////////////////////////////////////////

/*
 * add a control - we leave that work
 * to the embedded ControlData class
 */

inline cdxCDynamicControlsManager::ControlPosition *cdxCDynamicControlsManager::AddSzControlEx(CWnd & ctrl, BYTE dX1pcnt, BYTE dX2pcnt, BYTE dY1pcnt, BYTE dY2pcnt)
{
	ASSERT(IsReady());			// don't called DoInitWindow() before ?
	ASSERT((dX1pcnt <= dX2pcnt) && (dY1pcnt <= dY2pcnt));
										// NOT recommended !!!!!!!

	ControlData	*si	=	new ControlData(*this, ctrl, dX1pcnt, dX2pcnt, dY1pcnt, dY2pcnt);
	ASSERT(si != NULL);			// if you don't throw exceptions :)
	return si;
}

/*
 * find a control's ControlPosition
 */

inline cdxCDynamicControlsManager::ControlPosition *cdxCDynamicControlsManager::FindSzControl(CWnd & ctrl)
{
	ASSERT(::IsWindow(ctrl.m_hWnd));	// will work for exiting windows only !

	for(ControlData *si = m_pFirst; si; si = si->GetNext())
		if(si->IsMemberOf(ctrl))
			return si;

	return NULL;
}

/*
 * adding controls by my nice constants
 */

inline cdxCDynamicControlsManager::ControlPosition *cdxCDynamicControlsManager::AddSzControl(CWnd & ctrl, Mode modeX, Mode modeY)
{
	BYTE	dX1pcnt	=	0,
			dX2pcnt	=	0,
			dY1pcnt	=	0,
			dY2pcnt	=	0;

	switch(modeX)
	{
		default	:			ASSERT(false);			// unknown value for modeX
		case	mdNone	:	break;
		case	mdRepos	:	dX1pcnt	=	dX2pcnt	=	100;
								break;
		case	mdResize	:	dX2pcnt	=	100;
								break;
		case	mdRelative:	dX1pcnt	=	dX2pcnt	=	100 / 2;
								break;
	}

	switch(modeY)
	{
		default	:			ASSERT(false);			// unknown value for modeY
		case	mdNone	:	break;
		case	mdRepos	:	dY1pcnt	=	dY2pcnt	=	100;
								break;
		case	mdResize	:	dY2pcnt	=	100;
								break;
		case	mdRelative:	dY1pcnt	=	dY2pcnt	=	100 / 2;
								break;
	}

	return AddSzControlEx(ctrl,dX1pcnt,dX2pcnt,dY1pcnt,dY2pcnt);
}

/////////////////////////////////////////////////////////////////////////////

/*
 * Reposition
 */

inline void cdxCDynamicControlsManager::ReorganizeControls(bool bRedraw)
{
	ASSERT(IsReady());

	CRect	clrect;
	m_pWnd->GetClientRect(&clrect);
	CRect	winrect;
	m_pWnd->GetWindowRect(&winrect);
	ReorganizeControlsAdvanced(winrect,clrect,bRedraw);
}

/////////////////////////////////////////////////////////////////////////////

/*
 * fill in MINMAXINFO as requested
 * Call your CWnd's OnGetMinMaxInfo first !
 *
 * changed due to a but report by Michel Wassink <mww@mitutoyo.nl>
 */

inline void cdxCDynamicControlsManager::DoOnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	if(IsReady())
	{
		lpMMI->ptMinTrackSize.x	=	m_szMin.cx;
		lpMMI->ptMinTrackSize.y	=	m_szMin.cy;

		if(m_Freedom & fdHoriz)
		{
			if(m_szMax.cx)
				lpMMI->ptMaxTrackSize.x	=	m_szMax.cx;
		}
		else
			lpMMI->ptMaxTrackSize.x	=	m_szMin.cx;

		if(m_Freedom & fdVert)
		{
			if(m_szMax.cy)
				lpMMI->ptMaxTrackSize.y	=	m_szMax.cy;
		}
		else
			lpMMI->ptMaxTrackSize.y	=	m_szMin.cy;
	}
}

/*
 * handle OnSize - we can't rely on cx,cy being client dimensions
 * as stated in the documentation ...
 */

inline void cdxCDynamicControlsManager::DoOnSize(UINT nType, int cx, int cy)
{
	if(IsReady() && (nType != SIZE_MINIMIZED))
		ReorganizeControls(true);
}

/*
 * free all memory
 * after having called this function, you can reuse the object
 * although I wouldn't recommend to do so :)
 */

inline void cdxCDynamicControlsManager::DoDestroyWindow(void)
{
	while(m_pFirst)
		delete m_pFirst;
	if(m_pWndSizeIcon)
	{
		if(::IsWindow(m_pWndSizeIcon->m_hWnd))
			m_pWndSizeIcon->DestroyWindow();
		delete m_pWndSizeIcon;
		m_pWndSizeIcon	=	NULL;
	}
	m_pWnd = NULL;
}

#endif // !defined(AFX_CDXCDYNAMICCONTROLSMANAGER_H__6517AE13_5D12_11D2_BE4C_000000000000__INCLUDED_)
