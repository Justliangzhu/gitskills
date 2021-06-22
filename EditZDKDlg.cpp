// EditZDKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "EditZDKDlg.h"
#include "XLDataBase.h"

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
extern CString Cworkdir,Mdf;
// CEditZDKDlg 对话框

IMPLEMENT_DYNAMIC(CEditZDKDlg, CDialog)
	CEditZDKDlg::CEditZDKDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditZDKDlg::IDD, pParent)
{
	m_ZDKArray.RemoveAll();
}

CEditZDKDlg::~CEditZDKDlg()
{
	m_ZDKArray.RemoveAll();
}

void CEditZDKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_LIST_EDITZDK, m_grid);
}


BEGIN_MESSAGE_MAP(CEditZDKDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_INS, OnBnClickedButtonIns)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBnClickedButtondel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEditZDKDlg 消息处理程序

void CEditZDKDlg::OnBnClickedButtonIns()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		int i;
		for(i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.col=i;
			m_grid.SetItem(&Item);
		}
		for(i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}

		if(m_grid.GetRowCount()>3)
		{
			if(nRow==m_grid.GetRowCount()-2)
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
			}
			else
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
			}
		}
		m_grid.SetFocusCell(nRow,1);

		m_grid.Invalidate();
	}

}

void CEditZDKDlg::OnBnClickedButtondel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_grid.GetFocusCell().row;
	if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}
}

void CEditZDKDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int RowSum = m_grid.GetRowCount();
	m_ZDKArray.RemoveAll();
	ZDKDAT TempZDK;
	CString Text;
	for (int i=0; i<RowSum; i++)
	{
		Text = m_grid.GetItemText(i+1,1);
		Text.Trim();
		if (!Text.IsEmpty())
			TempZDK.dml = _wtof(Text);
		else
			continue;

		Text = m_grid.GetItemText(i+1,2);
		Text.Trim();
		TempZDK.LZDK = _wtof(Text);

		Text = m_grid.GetItemText(i+1,3);
		Text.Trim();
		TempZDK.RZDK = _wtof(Text);

		TempZDK.cml = m_DBS.TYLC1(TempZDK.dml, m_DBS.NDL, m_DBS.DLArray);
		TempZDK.IsInsertHdm = 0.0;

		m_ZDKArray.Add(TempZDK);
	}

	int ZDKSum = m_ZDKArray.GetSize();
	int i;
	for (i=0;i<ZDKSum-1;i++)
	{
		for (int j=i+1;j<ZDKSum;j++)
		{
			if (m_ZDKArray[i].cml>m_ZDKArray[j].cml)
			{
				TempZDK=m_ZDKArray[i];
				m_ZDKArray[i]=m_ZDKArray[j];
				m_ZDKArray[j]=TempZDK;
			}
		}
	}
	CString ZDKfname =Cworkdir + "\\data\\" + "hdmZDK." + m_RoadName + "." + "主线";
	FILE *fp;
	if((fp=_wfopen(ZDKfname,L"w"))==NULL)
	{
		ads_printf(L"无法打开占地宽文件!\n");
	}
	else
	{
		fwprintf(fp,L"%d\n",ZDKSum);
		for(i=0;i<ZDKSum;i++) 
			fwprintf(fp ,L"\n%0.3lf %0.3lf %d %0.3lf %0.3lf",
			m_ZDKArray[i].cml,m_ZDKArray[i].dml,m_ZDKArray[i].IsInsertHdm,m_ZDKArray[i].LZDK,m_ZDKArray[i].RZDK);
	}
	fclose(fp);
	OnOK();
}

BOOL CEditZDKDlg::OnInitDialog()
{
	//AfxMessageBox(L"2");
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	SetGrid();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CEditZDKDlg::SetGrid()
{
	ACHAR  tmp[80];
	int i,j;
	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);

	m_DBS.ReadWorkdir();
	m_RoadName = Pro;
	m_ZDKArray.RemoveAll();
	int ZDKSum = 0;

	CString MdbName = Cworkdir + "\\DATA\\" + m_RoadName + ".mdb";
	if (!m_DBS.Read_XLDbs(MdbName,L"断链表",L"主线"))
	{
		AfxMessageBox(MdbName + "中无平面设计数据!");
		OnCancel();
	}

	CString ZDKfname =Cworkdir + "\\data\\" + "hdmZDK." + m_RoadName + "." + "主线";
	FILE *fp;
	if((fp=_wfopen(ZDKfname,L"r"))==NULL)
	{
		ads_printf(L"无法打开占地宽文件!\n");
	}
	else
	{
		fwscanf(fp,L"%d",&ZDKSum);
		ZDKDAT TempZDK;
		for(i=0;i<ZDKSum;i++)
		{
			fwscanf(fp,L"%lf%lf%d%lf%lf",
				&TempZDK.cml,&TempZDK.dml,&TempZDK.IsInsertHdm,&TempZDK.LZDK,&TempZDK.RZDK);
			if (!TempZDK.IsInsertHdm)
				m_ZDKArray.Add(TempZDK);
		}
		ZDKSum = m_ZDKArray.GetSize();
	}

	m_grid.SetRowCount(ZDKSum+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(4);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

	Item.nFormat = DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.row = 0;
	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		for( i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}		
		m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}

	m_grid.SetItemText(0,0,L"序号");
	m_grid.SetItemText(0,1,L"里程");
	m_grid.SetItemText(0,2,L"左占地宽(m)");
	m_grid.SetItemText(0,3,L"右占地宽(m)");

	if(ZDKSum != 0)
	{
		for(i=0; i<ZDKSum; i++)
		{
			_stprintf(tmp,L"%d", i+1);
			m_grid.SetItemText(i+1, 0, tmp);

			_stprintf(tmp,L"%.3lf", m_ZDKArray[i].dml);
			m_grid.SetItemText(i+1, 1, tmp);

			_stprintf(tmp,L"%.3lf", m_ZDKArray[i].LZDK);
			m_grid.SetItemText(i+1, 2, tmp);

			_stprintf(tmp,L"%.3lf", m_ZDKArray[i].LZDK);
			m_grid.SetItemText(i+1, 3, tmp);
		}  
	}

	m_grid.SetColumnWidth(0,40);
	for( i=1;i<4;i++)
		m_grid.SetColumnWidth(i,85);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
}