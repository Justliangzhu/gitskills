#pragma once
#include <vector>
#include "GridCtrl.h"
#include "Struct.h"
#include "XLDataBase.h"
// CJPInfor 对话框

class CJPInfor : public CDialog
{
	DECLARE_DYNAMIC(CJPInfor)

public:
	CJPInfor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJPInfor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_JPInfor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	//初始化函数
	virtual BOOL OnInitDialog();

	CString m_MdbName; 	//路径
	CString m_RoadName; //道路名

	CString m_bef_H; 	// 前接坡高程
	CString m_bef_PD; 	// 前接坡坡度
	CString m_bef_PC;  	// 前接坡坡长

	CString m_aft_H; 	// 后接坡高程
	CString m_aft_PD; 	// 后接坡坡度
	CString m_aft_PC;	// 后接坡坡长
	BAS_DRAW_FUN Bas_Fun;
	vector<JPInfor>m_JPInforArray;
	afx_msg void OnBnClickedOk();
	// 是否接坡
	BOOL m_IsJP;
};
