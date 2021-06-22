#pragma once
#include "excel9.h"
class CMROADExcel
{
public:
	CMROADExcel(void);
	~CMROADExcel(void);

//内部成员
private:
	//自创成员
	CString			m_FilePath;			//文件路径
	bool			m_IsInitialize;		//是否进行了初始化工作
	CString			m_LastSheet;		//最后一个打开的表单

	//Excel操作成员
	_Application	m_ExcelApp;			//应用程序
	Workbooks		m_ExcelWorkBooks;	//工作集
	_Workbook		m_ExcelWorkBook;	//单个工作簿
	Worksheets		m_ExcelWorksheets;	//表(Sheet)集
	_Worksheet		m_ExcelWorksheet;	//单个表
	Range			m_ExcelRange;		//单元格

//对外接口
public:
	
	//一、程序操作
	void InitExcel();									//初始化
	void QuitExcel(bool IsOpenExcel = false);			//退出Excel

	//二、路径相关
	void SetFilePath(CString TargetPath);	//设置文件路径
	CString GetFilePath() const;			//获取文件路径
	void CreateExcel();						//创建Excel文件

	//三、与Excel表相关操作
	//（一）表单相关操作
	//1.连接表单
	void ConnectExcelWookSheet(CString SheetName);
	void ConnectExcelWookSheet(CString TargetPath, CString SheetName);
	//2.创建表单
	void CreateSheet(CString SheetName);
	//3.删除表单
	void DeleteSheet(CString SheetName);
	//4.退出表单连接
	void QuitSheetConnect();
	//5.寻找表单是否存在
	bool IsSheetExist(CString SheetName);
	//（二）读写表操作
	void WriteExcel(long Line, long Col, _variant_t Value);
	void ReadExcel(long Line, long Col, _variant_t& Value);
	//（三）单表单内操作
	//1.合并表格
	bool MergeCellInExcel(long sLine, long eLine, CString sCol, CString eCol);
	//2.自动调整宽度
	void AutoSize();
};

