#include "stdafx.h"
#include "BAS_DRAW_FUN.h"
#include "MROADExcel.h"

extern ACHAR SysPath[1024];
extern CString Cworkdir,Pro;

CMROADExcel::CMROADExcel(void)
{
	AfxOleInit();	//Ole��ʼ��
	m_IsInitialize = false;
	m_LastSheet = L"";
}


CMROADExcel::~CMROADExcel(void)
{
}

//��ʼ��
void CMROADExcel::InitExcel()
{
	//��ʼ������
	if (!m_ExcelApp.CreateDispatch(L"Excel.Application", NULL)) 
	{ 
		AfxMessageBox(L"���ܴ�Excel���!��ȷ��Excel�Ѿ���װ��ȷ!"); 
		return ; 
	} 

	//Ѱ���ļ�������������򴴽�
	CFileFind Ffind;
	if(!Ffind.FindFile(m_FilePath))//�������ļ����ڣ���������ļ�
	{
		CreateExcel();
	}

	//��ʼ������������
	m_ExcelWorkBooks.AttachDispatch(m_ExcelApp.GetWorkbooks(), true);
	m_ExcelWorkBook.AttachDispatch(m_ExcelWorkBooks.Add(_variant_t(m_FilePath)));
	m_ExcelWorksheets.AttachDispatch(m_ExcelWorkBook.GetWorksheets(), true); 

	m_IsInitialize = true;
}

//�Ͽ�����Excel
void CMROADExcel::QuitExcel(bool IsOpenExcel)
{
	if(IsOpenExcel)
	{
		m_ExcelWorksheet.Activate();
		m_ExcelApp.SetVisible(true);
		m_ExcelApp.SetUserControl(true);   //����excel��
	}
	else
	{
		m_ExcelApp.SetDisplayAlerts(false);
		m_ExcelApp.SetAlertBeforeOverwriting(FALSE);
		m_ExcelWorkBook.SaveAs(COleVariant(m_FilePath), _variant_t((long)56), _variant_t(), _variant_t(), _variant_t(), _variant_t(),
			_variant_t((long)1), _variant_t(), _variant_t(), _variant_t(), _variant_t());
		m_ExcelWorksheet.Activate();
		m_ExcelApp.SetVisible(false);
		m_ExcelApp.SetUserControl(false);   //������excel��		
	}

	m_ExcelWorksheet.ReleaseDispatch();
	m_ExcelWorksheets.ReleaseDispatch();
	m_ExcelWorkBook.ReleaseDispatch();
	m_ExcelWorkBooks.ReleaseDispatch();
	m_ExcelWorkBook.Close(_variant_t((long)1), _variant_t(), _variant_t());
	m_ExcelWorkBooks.Close();
	
	m_ExcelApp.Quit();
	m_ExcelApp.ReleaseDispatch();

	m_IsInitialize = false;
}

//�����ļ�·��
void CMROADExcel::SetFilePath(CString TargetPath)
{
	m_FilePath = TargetPath;
}

//��ȡ�ļ�·��
CString CMROADExcel::GetFilePath() const
{
	return m_FilePath;
}

//����Excel�ļ�
void CMROADExcel::CreateExcel()
{
	CString ExcelTemplate;
	ExcelTemplate.Format(L"%s\\lib\\�õ�ģ��.xlt", SysPath);			//�趨�����ļ�ʱ��Դ�ļ�·��

	if(m_FilePath.IsEmpty())
	{
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		m_FilePath = Cworkdir + L"\\RESULT\\" + Pro + L".xlsx";
	}

	CopyFile(ExcelTemplate, m_FilePath, false);
}

//1.���ӱ�
void CMROADExcel::ConnectExcelWookSheet(CString SheetName)
{
	//������δ���г�ʼ������г�ʼ��
	if(!m_IsInitialize)
		InitExcel();

	//���ӱ�
	//��ʱ�������ɱ�����û����
	//if(!IsSheetExist(SheetName))
	//	CreateSheet(SheetName);
	m_ExcelWorksheet.AttachDispatch(m_ExcelWorksheets.GetItem(_variant_t(SheetName)), true); 
	m_LastSheet = SheetName;
	//�õ�ȫ��Cells����ʱ,rgMyRge��cells�ļ��� 
	m_ExcelRange.AttachDispatch(m_ExcelWorksheet.GetCells(), true); 
}
void CMROADExcel::ConnectExcelWookSheet(CString TargetPath, CString SheetName)
{
	m_FilePath = TargetPath;
	ConnectExcelWookSheet(SheetName);
}

////2.������
//void CreateSheet(CString SheetName)
//{
//
//}
////3.ɾ����
//void DeleteSheet(CString SheetName)
//{
//
//}
//4.�˳�������
void CMROADExcel::QuitSheetConnect()
{
	m_ExcelRange.ReleaseDispatch();
	m_ExcelWorksheet.ReleaseDispatch();
}
////5.Ѱ�ұ��Ƿ����
//bool CMROADExcel::IsSheetExist(CString SheetName)
//{
//
//}
//д������
void CMROADExcel::WriteExcel(long Line, long Col, _variant_t Value)
{
	m_ExcelRange.SetItem(_variant_t(Line), _variant_t(Col), Value);
}

bool CMROADExcel::MergeCellInExcel(long sLine, long eLine, CString sCol, CString eCol)
{
	ACHAR scp[10];
	ACHAR fcp[10];
	LPDISPATCH pd = NULL;

	m_ExcelRange.ReleaseDispatch();

	_stprintf(scp, L"%s%ld", sCol, sLine);
	_stprintf(fcp, L"%s%ld", eCol, eLine);
	VERIFY(pd = m_ExcelWorksheet.GetRange(COleVariant(scp), COleVariant(fcp)));
	m_ExcelRange.AttachDispatch(pd);
	m_ExcelRange.Merge(_variant_t(false));

	return true;
}

void CMROADExcel::AutoSize()
{
	//���ÿ��
	Range Cols;
	m_ExcelRange.ReleaseDispatch();
	LPDISPATCH pd = NULL;
	ACHAR scp[10];
	ACHAR fcp[10];
	_stprintf(scp, L"%s%ld", L"A", 1);
	_stprintf(fcp, L"%s%ld", L"F", 18);
	VERIFY(pd = m_ExcelWorksheet.GetRange(COleVariant(scp), COleVariant(fcp)));
	m_ExcelRange.AttachDispatch(pd);
	Cols = m_ExcelRange.GetEntireColumn();
	Cols.AutoFit();
}
