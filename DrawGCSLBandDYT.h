#pragma once

#include "resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"
#include "CSetOutPileAttri.h"

// CDrawGCSLBandDYT �Ի���

class CDrawGCSLBandDYT : public CDialog,public BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(CDrawGCSLBandDYT)

public:
	CDrawGCSLBandDYT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDrawGCSLBandDYT();

// �Ի�������
	enum { IDD = IDD_DIALOG_DrawGCSLBandDYT };
	XLDataBase DBS;
	JD_CENTER *pm;
	CComboBox	m_RoadNameCrl;
	CString	m_RoadName;
	CString	m_PTX;
	CString	m_PTY;
	CString	m_Dbs;
	int DFACTOR;//����
	//20200303����
	std::vector<CString> PathArray;
	std::vector<CrossType> m_CrossTypeArray;	//�ֶ�����ָ��׮������
		// �Ƿ��������Excel
	BOOL m_IsToExcel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonLiulan();
	afx_msg void OnBnClickedButtonSelectxl();
	afx_msg void OnBnClickedButtonPickpt();
	afx_msg void OnCbnSelchangeComboDlname();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnBnClickedButtonSetoutpileattri();

	// //20200303��������������
	BOOL m_PLTab;
	afx_msg void OnBnClickedCheckPl();
};
