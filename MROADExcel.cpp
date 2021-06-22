#include "stdafx.h"
#include "BAS_DRAW_FUN.h"
#include "MROADExcel.h"

extern ACHAR SysPath[1024];
extern CString Cworkdir,Pro;

CMROADExcel::CMROADExcel(void)
{
	AfxOleInit();	//Ole初始化
	m_IsInitialize = false;
	m_LastSheet = L"";
}


CMROADExcel::~CMROADExcel(void)
{
}

//初始化
void CMROADExcel::InitExcel()
{
	//初始化程序
	if (!m_ExcelApp.CreateDispatch(L"Excel.Application", NULL)) 
	{ 
		AfxMessageBox(L"不能打开Excel表格!请确认Excel已经安装正确!"); 
		return ; 
	} 

	//寻找文件，如果不存在则创建
	CFileFind Ffind;
	if(!Ffind.FindFile(m_FilePath))//如果输出文件存在，连接输出文件
	{
		CreateExcel();
	}

	//初始化工作簿、表集
	m_ExcelWorkBooks.AttachDispatch(m_ExcelApp.GetWorkbooks(), true);
	m_ExcelWorkBook.AttachDispatch(m_ExcelWorkBooks.Add(_variant_t(m_FilePath)));
	m_ExcelWorksheets.AttachDispatch(m_ExcelWorkBook.GetWorksheets(), true); 

	m_IsInitialize = true;
}

//断开连接Excel
void CMROADExcel::QuitExcel(bool IsOpenExcel)
{
	if(IsOpenExcel)
	{
		m_ExcelWorksheet.Activate();
		m_ExcelApp.SetVisible(true);
		m_ExcelApp.SetUserControl(true);   //调用excel表
	}
	else
	{
		m_ExcelApp.SetDisplayAlerts(false);
		m_ExcelApp.SetAlertBeforeOverwriting(FALSE);
		m_ExcelWorkBook.SaveAs(COleVariant(m_FilePath), _variant_t((long)56), _variant_t(), _variant_t(), _variant_t(), _variant_t(),
			_variant_t((long)1), _variant_t(), _variant_t(), _variant_t(), _variant_t());
		m_ExcelWorksheet.Activate();
		m_ExcelApp.SetVisible(false);
		m_ExcelApp.SetUserControl(false);   //不调用excel表		
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

//设置文件路径
void CMROADExcel::SetFilePath(CString TargetPath)
{
	m_FilePath = TargetPath;
}

//获取文件路径
CString CMROADExcel::GetFilePath() const
{
	return m_FilePath;
}

//创建Excel文件
void CMROADExcel::CreateExcel()
{
	CString ExcelTemplate;
	ExcelTemplate.Format(L"%s\\lib\\用地模板.xlt", SysPath);			//设定复制文件时的源文件路径

	if(m_FilePath.IsEmpty())
	{
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		m_FilePath = Cworkdir + L"\\RESULT\\" + Pro + L".xlsx";
	}

	CopyFile(ExcelTemplate, m_FilePath, false);
}

//1.连接表单
void CMROADExcel::ConnectExcelWookSheet(CString SheetName)
{
	//检测如果未进行初始化则进行初始化
	if(!m_IsInitialize)
		InitExcel();

	//连接表单
	//暂时隐藏生成表单，还没做好
	//if(!IsSheetExist(SheetName))
	//	CreateSheet(SheetName);
	m_ExcelWorksheet.AttachDispatch(m_ExcelWorksheets.GetItem(_variant_t(SheetName)), true); 
	m_LastSheet = SheetName;
	//得到全部Cells，此时,rgMyRge是cells的集合 
	m_ExcelRange.AttachDispatch(m_ExcelWorksheet.GetCells(), true); 
}
void CMROADExcel::ConnectExcelWookSheet(CString TargetPath, CString SheetName)
{
	m_FilePath = TargetPath;
	ConnectExcelWookSheet(SheetName);
}

////2.创建表单
//void CreateSheet(CString SheetName)
//{
//
//}
////3.删除表单
//void DeleteSheet(CString SheetName)
//{
//
//}
//4.退出表单连接
void CMROADExcel::QuitSheetConnect()
{
	m_ExcelRange.ReleaseDispatch();
	m_ExcelWorksheet.ReleaseDispatch();
}
////5.寻找表单是否存在
//bool CMROADExcel::IsSheetExist(CString SheetName)
//{
//
//}
//写表格操作
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
	//设置宽度
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
