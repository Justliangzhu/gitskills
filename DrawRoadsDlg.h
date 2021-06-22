#pragma once


// CDrawRoadsDlg �Ի���

class CDrawRoadsDlg : public CDialog
{
	DECLARE_DYNAMIC(CDrawRoadsDlg)

public:
	CDrawRoadsDlg(CWnd* pParent = acedGetAcadFrame());   // ��׼���캯��
	virtual ~CDrawRoadsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DRAWROADS };
	CListBox m_ListBox_UnSel;//δѡ����·
	CListBox m_ListBox_Sel;//��ѡ����·
	CStringArray m_UnSelRoadArray;//δѡ������
	CStringArray m_SelRoadArray;//��ѡ����·����
	CStringArray m_AllRoadArray;//������·����

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSet();
	afx_msg void OnBnClickedButtonSetmoban();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnLbnDblclkListUnsel();
	afx_msg void OnLbnDblclkListSel();
	afx_msg void OnBnClickedOk();
};
