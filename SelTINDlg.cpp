// SelTINDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelTINDlg.h"
#include "BAS_DRAW_FUN.h"
#include<fstream>
#include<iostream>
#include <iomanip>
using namespace std;

extern CString Cworkdir;


// CSelTINDlg �Ի���

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


// CSelTINDlg ��Ϣ�������

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
			//�ж��Ƿ����ļ���
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

//����·�����뵽�б���
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
		/*AfxMessageBox(L"��ָ������Ŀ¼�ͷ�������!");
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
		Mes.Format(L"����\"%s\"�ļ�ʧ�ܣ���ȷ�ϸ��ļ��Ƿ���ڣ�", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\Point.xyz", dirstr);
	_stprintf(sourceName, L"%s\\Point.xyz", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"����\"%s\"�ļ�ʧ�ܣ���ȷ�ϸ��ļ��Ƿ���ڣ�", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\TIN.tri", dirstr);
	_stprintf(sourceName, L"%s\\TIN.tri", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"����\"%s\"�ļ�ʧ�ܣ���ȷ�ϸ��ļ��Ƿ���ڣ�", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\Grid.idx", dirstr);
	_stprintf(sourceName, L"%s\\Grid.idx", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"����\"%s\"�ļ�ʧ�ܣ���ȷ�ϸ��ļ��Ƿ���ڣ�", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\Edge.inf", dirstr);
	_stprintf(sourceName, L"%s\\Edge.inf", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"����\"%s\"�ļ�ʧ�ܣ���ȷ�ϸ��ļ��Ƿ���ڣ�", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	_stprintf(targetName, L"%s\\TIN.inf", dirstr);
	_stprintf(sourceName, L"%s\\TIN.inf", subDirstr);
	CopySucess = CopyFile(sourceName, targetName, false);
	if (!CopySucess)
	{
		Mes.Format(L"����\"%s\"�ļ�ʧ�ܣ���ȷ�ϸ��ļ��Ƿ���ڣ�", sourceName);
		AfxMessageBox(Mes);
		return;
	}

	Mes.Format(L"��ѡ��%sΪ��ǰ��ģ", tinName);
	AfxMessageBox(Mes);

	//д����ǰ��ģ��
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
//2019.3.9��ֲ��ok��ť
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
		AfxMessageBox(L"��ǰ����ģ��ϵͳĿǰ��Ĭ����ģ��");
		return;
	}

	CString CurMessage = L"��ѡ��" + TinName + L"����Ϊ��ǰѡ����ģ��";
	m_CurTinName = TinName;
	UpdateData(FALSE);
	AfxMessageBox(CurMessage);
	return;
}




void CSelTINDlg::OnLbnDblclkListSeltin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetCurTin();
	OnOK();
}


void CSelTINDlg::OnBnClickedButtonDeletetinname()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ��ǰѡ�е���ģ����
	CString TinName;
	int i = m_TinNameListBox.GetCurSel();
	if (i == LB_ERR)
	{
		AfxMessageBox(L"��ǰδѡ����ģ��");
		return;
	}
	m_TinNameListBox.GetText(i, TinName);

	//ѯ���û��Ƿ����ɾ�������ǵ��û��������Ŀ����ԣ�
	CString CurMessage = L"�Ƿ�ȷ��ɾ����ģ��" + TinName + L"��";
	int UserFeedback = AfxMessageBox(CurMessage, MB_YESNO);
	if (UserFeedback == IDNO)
		return;

	//ִ��ɾ������
	if (DeleteTin(TinName))
		AfxMessageBox(L"ɾ����ģ�ɹ���");
	else
		AfxMessageBox(L"δ�ܳɹ�ɾ����ģ������PMAP�ļ��¶�Ӧ����ģ�ļ��Ƿ���������ռ��");

	//�����б�ؼ������ڵ�ǰ����ѡ��һ��Ĭ�ϵ�����ģ
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
	//���ò���
	CString targetName;
	CString dirstr = Cworkdir + L"\\PMAP";

	//��һ����ɾ��
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
