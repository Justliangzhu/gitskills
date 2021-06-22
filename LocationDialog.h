#if !defined(AFX_LOCATIONDIALOG_H__7E204848_201D_4F5E_983E_7AD6B021488A__INCLUDED_)
#define AFX_LOCATIONDIALOG_H__7E204848_201D_4F5E_983E_7AD6B021488A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocationDialog.h : header file
//
#include "ZYLink.h"
#include "YYLuanLink.h"
#include "YYSLink.h"
#include "LLTuLink.h"
#include "resource.h"
#include "bas_draw_fun.h"
#define ZY_MODEL 0
#define S_MODEL 1
#define C_MODEL 2
#define LUAN_MODEL 3
#define TU_MODEL 4

#define A_VALUE 0
#define Lo_VALUE 1
/////////////////////////////////////////////////////////////////////////////
// LocationDialog dialog

class LocationDialog : public CDialog , BAS_DRAW_FUN
{
// Construction
public:
	void AutoModel();
	LocationDialog(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(LocationDialog)
	enum { IDD = IDD_DIALOG_LOCALION1 };
	int		m_LocationModel;
	int		m_LRFlag;
	int		m_AorL;
	double	m_AL1;
	double	m_AL2;

	//}}AFX_DATA
	double startN1,startE1,startN2,startE2,endN1,endE1,endN2,endE2;//ֱ�����˵�
	double CenN1,CenE1,CenN2,CenE2;//Բ��
	double R1,R2;//�뾶
	int LeftOrRight;//����ת
	int FirstObjClass,SecondObjClass;
	AcDbObjectId FirstObjId,SecondObjId; 
	AcDbObjectId SpiralId1,SpiralId2; 
	AcDbObjectId AtextId1,AtextId2;
	AcDbObjectId LtextId1,LtextId2;
	AcDbLine *Line1,*Line2;
	AcDbArc *Arc1,*Arc2;
	AcDbCircle *Cir1,*Cir2;
//��ΪJDCenter��line\arc����
	int XY_No,XY_Type,XY_JDNo;



	bool SetObjData(const AcDbObject *pEnt,int FirstOrSecond,ads_point pt);
	void RegenSpiral(); //��������
	int  ChooseCircleMode();//ѡ��ֱ���϶���ʽ0-�϶��뾶��1-�϶�Բ�ģ�2-ƽ��ֱ��
	int  ChooseZXMode();//ѡ��ֱ���϶���ʽ0-�϶��յ㣬1-�϶���㣬2-ƽ��ֱ��

	void FindWichType(const AcDbObject *obj,ads_point pt,int &XYNo,int &type,int &JDNo);
	
	void LinkBothJdCen(AcDbObjectId XLId1,AcDbObjectId XLId2,int XYClass1,int XYClass2,
		int JDNo1,int JDNo2);
    void TranLineOrArcToXL(AcDbObjectId eId,AcDbObjectId &lxId,int &jdno,int &xy_type);		
	void deleteEnt(AcDbObjectId &ObjId);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LocationDialog)

	virtual void OnOK();
//	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
//	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
//protected:

	// Generated message map functions
	//{{AFX_MSG(LocationDialog)
	afx_msg void OnButtonSelectFirst();
	afx_msg void OnButtonSelectSecond();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONtest();
	afx_msg void OnButtonDttd();

//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCATIONDIALOG_H__7E204848_201D_4F5E_983E_7AD6B021488A__INCLUDED_)
