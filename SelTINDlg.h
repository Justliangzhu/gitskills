#pragma once
#include "resource.h"
#include "afxwin.h"

// CSelTINDlg �Ի���

class CSelTINDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelTINDlg)

public:
	CSelTINDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSelTINDlg();
	CStringArray m_TINNameArray;//��ģ��������
	CString m_SelTinName;//ѡ�����ģ����
// �Ի�������
	enum { IDD = IDD_DIALOG_SELECTTIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CombTINName;
	void IniTINArray();//��ʼ����ģ�����б�
	int AddOneName(CString FileNameStr);//����·�����뵽�б���,���ؼ���������б���±꣬��
	void CopyTin();//������ģ
	void SetCurTin();	//2019.3.9�޸�
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	CListBox m_TinNameListBox;	//2019.3.9��Ϊlistbox��ʾ
	afx_msg void OnLbnDblclkListSeltin();
	afx_msg void OnBnClickedButtonDeletetinname();
	bool DeleteTin(CString TinName);
	CString m_CurTinName;
};
