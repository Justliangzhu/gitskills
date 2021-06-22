#if !defined(AFX_DRAWPMDLG_H__AE45B1F4_8C6D_4C1E_8D5A_7F181CF6860A__INCLUDED_)
#define AFX_DRAWPMDLG_H__AE45B1F4_8C6D_4C1E_8D5A_7F181CF6860A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawPmDlg.h : header file
//
#include "Resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// CDrawPmDlg dialog

class CDrawPmDlg : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CDrawPmDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	CDrawPmDlg(CString MdbName);
	XLDataBase DBS;
	JD_CENTER *pm;
	int DrawMode;//��ͼ��ʽ 0  ��λͼ 1 ƽ������ͼ 2 �õ�ͼ
	int m_DrawAll;//�Ƿ��������·		0-����������·	1-���Ƶ�����·	20190706	��������ȥ���Ի�
	int m_ZDKMode;////����ռ��ͼ��ʽ
	bool WriteCSFile(CString FName);//дƽ���ͼ�����ļ�
	bool ReadCSFile(CString FName);//��ƽ���ͼ�����ļ�
	bool ReadCSFileForDraw();//��ȡƽ������ļ��������ָ�롣
// Dialog Data
	//{{AFX_DATA(CDrawPmDlg)
	enum { IDD = IDD_DIALOG_DRAWPM };
	CComboBox	m_RoadName;
	CString	m_ROAD;
	double	m_ZD;
	double	m_WHT;
	double	m_QD;
	CString	m_Dbs;//���ݿ�
	CString	m_SCALE;//������
	CString	m_XSW;//��̱���С��λ��
	CString	m_LCBZ;//��̱�ע
	BOOL	m_DrawBM;	//��ˮ׼��
	BOOL	m_DrawDX;	//�浼��
	BOOL	m_DrawTun;	//�����
	BOOL	m_DrawCulvet;	//С�ź�
	BOOL	m_DrawDZQ;	//�������
	double	m_ZcLen;	//����õر�ע�߳�
	double	m_YcLen;	//�Ҳ��õر�ע�߳�
	BOOL	m_DelEntInf;//ɾ������ʵ��
	BOOL	m_BZRAL;	//��ע�뾶����
	CString	m_ZxWid;	//��·���߿��
	BOOL	m_drawfldzdk;//�����ռ�ؿ�
	CString	m_YDXWid;	//�õ�׮�߿��
	BOOL m_DrawJD;		//�Ƿ��ע����
	//}}AFX_DATA
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawPmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawPmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONOpenDbs();
	afx_msg void OnRADIODrawALL();
	afx_msg void OnRADIODrawSingle();
	virtual void OnOK();
	afx_msg void OnSelchangeCOMBORoadName();
	afx_msg void OnRadioxwt();
	afx_msg void OnRadiopmztt();
	afx_msg void OnRadiozdt();
	virtual void OnCancel();
	afx_msg void OnRadiolcwid();
	afx_msg void OnRadionecor();
	afx_msg void OnRadioboth();
	afx_msg void OnRadionone();
	afx_msg void OnCheckBmbridge();
	afx_msg void OnCheckBm();
	afx_msg void OnCheckSmallbridge();
	afx_msg void OnCheckTun();
	afx_msg void OnCheckDx();
	afx_msg void OnCheckDelent();
	afx_msg void OnCheckShowdbs();
	afx_msg void OnCheckFldzdk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWPMDLG_H__AE45B1F4_8C6D_4C1E_8D5A_7F181CF6860A__INCLUDED_)
