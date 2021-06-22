// SelTINDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelTINDlg.h"
#include "BAS_DRAW_FUN.h"
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;

extern CString Cworkdir;


// CSelTINDlg 对话框

IMPLEMENT_DYNAMIC(CSelTINDlg, CDialog)

CSelTINDlg::CSelTINDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelTINDlg::IDD, pParent)
	, m_CurTinName(_T(""))
{

}

CSelTINDlg::~CSelTINDlg()
{
	m_TINNameArray.RemoveAll();
}

void CSelTINDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SELECTTIN, m_CombTINName);
	DDX_Control(pDX, IDC_LIST_SelTin, m_TinNameListBox);
	DDX_Text(pDX, IDC_EDIT_SELECTTIN, m_CurTinName);
}


BEGIN_MESSAGE_MAP(CSelTINDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSelTINDlg::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_LIST_SelTin, &CSelTINDlg::OnLbnDblclkListSeltin)
	ON_BN_CLICKED(IDC_BUTTON_DeleteTinName, &CSelTINDlg::OnBnClickedButtonDeletetinname)
END_MESSAGE_MAP()


// CSelTINDlg 消息处理程序

void CSelTINDlg::IniTINArray()
{
	m_TINNameArray.RemoveAll();
	m_CombTINName.ResetContent();
	ACHAR szPath[MAX_PATH];   
	CString CurTinName;
	ACHAR tempTinName[256];
	_tcscpy(tempTinName, L"");
	_stprintf(szPath, L"%s\\PMAP\\CurTIN", Cworkdir);
	FILE* pFile = _wfopen(szPath, L"r");
	if (pFile)
	{
		fwscanf(pFile, L"%s", tempTinName);
		fclose(pFile);
		CurTinName = tempTinName;
		if (!CurTinName.IsEmpty())
		{
			m_TINNameArray.Add(CurTinName);
			m_CombTINName.AddString(CurTinName);
			m_TinNameListBox.AddString(CurTinName);
		}
	}
	else
		CurTinName.Empty();

	_stprintf(szPath, L"%s\\PMAP\\*.*", Cworkdir);
	CFileFind FileFind;
	BOOL NeedFind = TRUE;
	CString FileNameStr, pathName, ProjTitle;
	if(FileFind.FindFile(szPath))
	{
		for(;;)
		{
			NeedFind = FileFind.FindNextFile();

			FileNameStr = FileFind.GetFileName();
			pathName = FileFind.GetFilePath();
			//判断是否是文件夹
			if(!FileFind.IsDirectory())
			{
				if ((FileNameStr.Find(L"PtIndex.idx") > -1 && FileNameStr !=  L"PtIndex.idx")
					|| (FileNameStr.Find(L"Point.xyz") > -1 && FileNameStr !=  L"Point.xyz")
					|| (FileNameStr.Find(L"TIN.tri") > -1 && FileNameStr !=  L"TIN.tri")
					|| (FileNameStr.Find(L"Grid.idx") > -1 && FileNameStr !=  L"Grid.idx")
					|| (FileNameStr.Find(L"Edge.inf") > -1 && FileNameStr !=  L"Edge.inf")
					|| (FileNameStr.Find(L"Tin.inf") > -1 && FileNameStr !=  L"Tin.inf"))
				{
					AddOneName(FileNameStr);
				}
			}

			if(NeedFind == FALSE)
				break;
		}
	}
}

//将线路名加入到列表中
int CSelTINDlg::AddOneName(CString FileNameStr)
{
	CString Name;
	int i = 0;
	i = FileNameStr.ReverseFind('.');
	if (i > 0)
	{
		Name = FileNameStr.Mid(i+1);
		int j=0;
		for (; j<m_TINNameArray.GetSize(); j++)
			if (m_TINNameArray[j] == Name)
				break;
		if (j == m_TINNameArray.GetSize())
		{
			m_TINNameArray.Add(Name);
			m_CombTINName.AddString(Name);
			m_TinNameListBox.AddString(Name);
			return m_TINNameArray.GetSize()-1;
		}
	}
	return -1;
}

BOOL CSelTINDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BAS_DRAW_FUN ob;

	if(!ob.ReadWorkdir())
	{
		/*AfxMessageBox(L"请指定工作目录和方案名称!");
		return;*/
	}
	IniTINArray();

	int ComboCount = m_TinNameListBox.GetCount();
	if(ComboCount > 0)
		m_TinNameListBox.SetCurSel(0);

	CString TinName;
	int i = m_TinNameListBox.GetCurSel();
	if (i == LB_ERR)
		m_CurTinName = L"";
	else
		m_TinNameListBox.GetText(i, m_CurTinName);

	UpdateData(FALSE);
	return TRUE;
}

void CSelTINDlg::OnBnClickedOk()
{
	SetCurTin();
	OnOK();
}

void CSelTINDlg::CopyTin()
{
	CString tinName;
	int ix=m_CombTINName.GetCurSel();
	m_CombTINName.GetLBText(ix, tinName);

	bool CopySucess;
	ACHAR targetName[1024], sourceName[1024];
	CString Mes;
	CString dirstr, subDirstr;

	dirstr=Cworkdir + L"\\PMAP";
	subDirstr=dirstr+L"\\"+tinName;

	_stprintf(targetName, L"%s\\PtIndex.idx", dirstr);
	_stprintf(sourceName, L"%s\\PtIndex.idx", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"复制\"%s\"文件失败，请确认该文件是否存在！", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\Point.xyz", dirstr);
	_stprintf(sourceName, L"%s\\Point.xyz", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"复制\"%s\"文件失败，请确认该文件是否存在！", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\TIN.tri", dirstr);
	_stprintf(sourceName, L"%s\\TIN.tri", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"复制\"%s\"文件失败，请确认该文件是否存在！", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\Grid.idx", dirstr);
	_stprintf(sourceName, L"%s\\Grid.idx", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"复制\"%s\"文件失败，请确认该文件是否存在！", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\Edge.inf", dirstr);
	_stprintf(sourceName, L"%s\\Edge.inf", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"复制\"%s\"文件失败，请确认该文件是否存在！", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\TIN.inf", dirstr);
	_stprintf(sourceName, L"%s\\TIN.inf", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"复制\"%s\"文件失败，请确认该文件是否存在！", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	Mes.Format(L"已选择%s为当前数模", tinName);
	AfxMessageBox(Mes);

	//写出当前数模名
	ofstream outfile;
	ACHAR shumoNameDir[520];
	char shumoNameDirChar[520];
	char tinNameChar[520];

	_stprintf(shumoNameDir, L"%s\\shumo.txt", dirstr);
	USES_CONVERSION; // conversion
	strcpy(shumoNameDirChar, T2A(shumoNameDir));
	strcpy(tinNameChar, T2A(tinName));
	outfile.open(shumoNameDirChar);
	outfile<<tinNameChar<<endl;
	outfile.close();
}
//2019.3.9移植自ok按钮
void CSelTINDlg::SetCurTin()
{
	CString TinName;
	int i = m_TinNameListBox.GetCurSel();
	if (i == LB_ERR)
		TinName = L"";
	else
		m_TinNameListBox.GetText(i, TinName);
	
	ACHAR FileName[MAX_PATH];
	_stprintf(FileName, L"%s\\PMAP\\CurTin", Cworkdir);
	FILE* pFile = _wfopen(FileName, L"w");
	if (pFile)
	{
		fwprintf(pFile, L"%s", TinName);
		fclose(pFile);
	}

	if (TinName.IsEmpty())
	{
		AfxMessageBox(L"当前无数模，系统目前无默认数模！");
		return;
	}

	CString CurMessage = L"已选择【" + TinName + L"】作为当前选择数模！";
	m_CurTinName = TinName;
	UpdateData(FALSE);
	AfxMessageBox(CurMessage);
	return;
}




void CSelTINDlg::OnLbnDblclkListSeltin()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCurTin();
	OnOK();
}


void CSelTINDlg::OnBnClickedButtonDeletetinname()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取当前选中的数模名称
	CString TinName;
	int i = m_TinNameListBox.GetCurSel();
	if (i == LB_ERR)
	{
		AfxMessageBox(L"当前未选择数模！");
		return;
	}
	m_TinNameListBox.GetText(i, TinName);

	//询问用户是否真的删除（考虑到用户可能误点的可能性）
	CString CurMessage = L"是否确认删除数模【" + TinName + L"】";
	int UserFeedback = AfxMessageBox(CurMessage, MB_YESNO);
	if (UserFeedback == IDNO)
		return;

	//执行删除操作
	if (DeleteTin(TinName))
		AfxMessageBox(L"删除数模成功！");
	else
		AfxMessageBox(L"未能成功删除数模，请检测PMAP文件下对应的数模文件是否被其他程序占用");

	//更新列表控件，并在当前界面选择一个默认的新数模
	int DeleteTinIndex = m_TinNameListBox.FindString(-1, TinName);
	m_TinNameListBox.DeleteString(DeleteTinIndex);
	if (m_TinNameListBox.GetCount() > 0)
	{
		m_TinNameListBox.SetCurSel(0);
		m_TinNameListBox.GetText(0, m_CurTinName);
		UpdateData(FALSE);
	}

}

bool CSelTINDlg::DeleteTin(CString TinName)
{
	//设置参数
	CString targetName;
	CString dirstr = Cworkdir + L"\\PMAP";

	//逐一进行删除
	targetName = dirstr + L"\\Edge.inf." + TinName;
	if (PathFileExists(targetName) && DeleteFile(targetName) == 0)
		return false;
	
	targetName = dirstr + L"\\Grid.idx." + TinName;
	if (PathFileExists(targetName) && DeleteFile(targetName) == 0)
		return false;

	targetName = dirstr + L"\\Point.xyz." + TinName;
	if (PathFileExists(targetName) && DeleteFile(targetName) == 0)
		return false;

	targetName = dirstr + L"\\PtIndex.idx." + TinName;
	if (PathFileExists(targetName) && DeleteFile(targetName) == 0)
		return false;

	targetName = dirstr + L"\\TIN.inf." + TinName;
	if (PathFileExists(targetName) && DeleteFile(targetName) == 0)
		return false;

	targetName = dirstr + L"\\TIN.tri." + TinName;
	if (PathFileExists(targetName) && DeleteFile(targetName) == 0)
		return false;

	return true;
}
