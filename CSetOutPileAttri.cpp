// CSetOutPileAttri.cpp: 实现文件
//
#include "StdAfx.h"
#include "CSetOutPileAttri.h"
#include "afxdialogex.h"
#include "RoadDesignWorkingDatabase.h"


// CSetOutPileAttri 对话框
IMPLEMENT_DYNAMIC(CSetOutPileAttri, CDialog)
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库
CSetOutPileAttri::CSetOutPileAttri(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SETOUTPILEATTRI, pParent)
{

}

CSetOutPileAttri::~CSetOutPileAttri()
{
}

void CSetOutPileAttri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SETOUTPILEATTRI, m_GridCtrl);
}


BEGIN_MESSAGE_MAP(CSetOutPileAttri, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETOUTPILEATTRI_ADD, &CSetOutPileAttri::OnBnClickedButtonSetoutpileattriAdd)
	ON_BN_CLICKED(IDC_BUTTON_SETOUTPILEATTRI_DEL, &CSetOutPileAttri::OnBnClickedButtonSetoutpileattriDel)
	ON_BN_CLICKED(IDOK, &CSetOutPileAttri::OnBnClickedOk)
END_MESSAGE_MAP()


void CSetOutPileAttri::InitGridCtrl()
{
	int i, j;
	ACHAR tmp[50];

	//设置行和列
	m_GridCtrl.SetRowCount(3);
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetColumnCount(3);
	m_GridCtrl.SetFixedColumnCount(1);

	//设置单元格属性
	
	LOGFONT font = { 12, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"Arial" };
	m_Item.mask = GVIF_TEXT | GVIF_FORMAT;
	m_Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;
	m_Item.lfFont = font;

	//传入单元格属性
	for (int k = 0; k < m_GridCtrl.GetColumnCount(); k++)
	{
		for (i = 0; i < m_GridCtrl.GetRowCount(); i++)
		{
			m_Item.row = i;
			m_Item.col = k;
			m_GridCtrl.SetItem(&m_Item);
		}
	}

	//设置标题
	m_GridCtrl.SetItemText(0, 0, L"序 号");
	m_GridCtrl.SetItemText(0, 1, L"里 程");
	m_GridCtrl.SetItemText(0, 2, L"桩号性质");

	//设置宽度与长度
	CRect Rect;
	m_GridCtrl.GetClientRect(&Rect);
	int Onewidth = int(Rect.Width() / 10.0);

	m_GridCtrl.SetColumnWidth(0, Onewidth * 1);
	m_GridCtrl.SetColumnWidth(1, Onewidth * 3);
	m_GridCtrl.SetColumnWidth(2, Rect.Width() - Onewidth * 4);
	for (i = 0; i < m_GridCtrl.GetRowCount(); i++)
		m_GridCtrl.SetRowHeight(i, DEFAULTCOLHEI);

	//设置序列号
	for (i = 1; i < 3; i++)
	{
		_stprintf(tmp, L"%d", i);
		m_GridCtrl.SetItemText(i, 0, tmp);
	}

	//额外设置
	m_GridCtrl.ResetScrollBars();
	m_GridCtrl.SetFocusCell(1, 1);

	if (m_CrossTypeArray.size())
		WriteGridCtrl();

	UpdateData(FALSE);
	m_GridCtrl.Invalidate();
}

// CSetOutPileAttri 消息处理程序
void CSetOutPileAttri::WriteGridCtrl()
{
	//设置行和列
	m_GridCtrl.SetRowCount(1 + m_CrossTypeArray.size());

	//传入单元格属性
	for (int k = 0; k < m_GridCtrl.GetColumnCount(); k++)
	{
		for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		{
			m_Item.row = i;
			m_Item.col = k;
			m_GridCtrl.SetItem(&m_Item);
		}
	}

	for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		m_GridCtrl.SetRowHeight(i, DEFAULTCOLHEI);

	//设置序列号
	ACHAR tmp[50];
	for (int i = 1; i < m_GridCtrl.GetRowCount(); i++)
	{
		_stprintf(tmp, L"%d", i);
		m_GridCtrl.SetItemText(i, 0, tmp);
	}

	for (int i = 0; i < m_CrossTypeArray.size(); i++)
	{
		_stprintf(tmp, L"%.3lf", m_CrossTypeArray[i].Cml);
		m_GridCtrl.SetItemText(i + 1, 1, tmp);

		switch (m_CrossTypeArray[i].Type)
		{
		case 1:
			_tcscpy(tmp, L"1-桥头");
			break;
		case 2:
			_tcscpy(tmp, L"2-桥尾");
			break;
		case 3:
			_tcscpy(tmp, L"3-隧头");
			break;
		case 4:
			_tcscpy(tmp, L"4-隧尾");
			break;
		case 5:
			_tcscpy(tmp, L"5-涵洞头");
			break;
		case 6:
			_tcscpy(tmp, L"6-涵洞尾");
			break;
		default:
			break;
		}

		m_GridCtrl.SetItemText(i + 1, 2, tmp);
	}
}

void CSetOutPileAttri::ReadGridCtrl()
{
	m_CrossTypeArray.clear();
	CrossType tempCrossType;
	CString tempText;

	for (int i = 1; i < m_GridCtrl.GetRowCount(); i++)
	{
		tempText = m_GridCtrl.GetItemText(i, 1);
		if(tempText.IsEmpty())
			continue;

		tempCrossType.Cml = _wtof(tempText);

		tempText = m_GridCtrl.GetItemText(i, 2);
		if (tempText.IsEmpty())
			continue;

		tempCrossType.Type = _wtoi(tempText);
		m_CrossTypeArray.push_back(tempCrossType);
	}
}


BOOL CSetOutPileAttri::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSetOutPileAttri::OnBnClickedButtonSetoutpileattriAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_GridCtrl.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Item.row = nRow;
		m_GridCtrl.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_GridCtrl.GetColumnCount(); i++)
		{
			m_Item.col = i;
			m_GridCtrl.SetItem(&m_Item);
		}
		for (i = nRow; i < m_GridCtrl.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_GridCtrl.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_GridCtrl.GetRowCount(); i++)
			m_GridCtrl.SetRowHeight(i, DEFAULTCOLHEI);

		if (m_GridCtrl.GetRowCount() > 3)
		{
			if (nRow == m_GridCtrl.GetRowCount() - 2)
			{
				for (i = 1; i < m_GridCtrl.GetColumnCount(); i++)
					m_GridCtrl.SetItemText(nRow, i, m_GridCtrl.GetItemText(nRow - 1, i));
			}
			else
				for (i = 1; i < m_GridCtrl.GetColumnCount(); i++)
					m_GridCtrl.SetItemText(nRow, i, m_GridCtrl.GetItemText(nRow + 1, i));
		}
		m_GridCtrl.SetFocusCell(nRow, 1);
	}

	UpdateData(FALSE);
	m_GridCtrl.Invalidate();
}


void CSetOutPileAttri::OnBnClickedButtonSetoutpileattriDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_GridCtrl.GetFocusCell().row;
	if (nRow == m_GridCtrl.GetRowCount() - 1) 
		return;
	if (nRow >= 0)
	{
		m_GridCtrl.DeleteRow(nRow);
		int i;
		for (i = nRow; i < m_GridCtrl.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_GridCtrl.SetItemText(i, 0, str);
		}
		m_GridCtrl.SetFocusCell(nRow, 1);
		m_GridCtrl.Invalidate();
	}
}


void CSetOutPileAttri::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadGridCtrl();

	if (!CheckDataValidate())
		return;

	CDialog::OnOK();
}

//检测数据准确性
bool CSetOutPileAttri::CheckDataValidate()
{
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);

	CString ErrorMess;

	for (int i = 0; i < m_CrossTypeArray.size(); i++)
	{
		if (i == 0 && (m_CrossTypeArray[i].Type % 2 == 0))
		{
			AfxMessageBox(L"首行数据桩号性质设置错误，请检查！");
			return false;
		}

		if ((m_CrossTypeArray[i].Type % 2 == 0) && (m_CrossTypeArray[i - 1].Type != m_CrossTypeArray[i].Type - 1))
		{
			ErrorMess.Format(L"第%d行数据桩号性质设置错误，请检查！", i);
			AfxMessageBox(ErrorMess);
			return false;
		}

		if ((m_CrossTypeArray[i].Type % 2 == 1) && (i > 0) && (m_CrossTypeArray[i - 1].Type % 2 == 1))
		{
			ErrorMess.Format(L"第%d行数据桩号性质设置错误，请检查！", i + 1);
			AfxMessageBox(ErrorMess);
			return false;
		}

		if (pRoad && m_CrossTypeArray[i].Cml > pRoad->LenOfLine)
		{
			ErrorMess.Format(L"第%d行数据里程超出道路里程范围，请检查！", i + 1);
			AfxMessageBox(ErrorMess);
			return false;
		}
	}

	return true;
}
