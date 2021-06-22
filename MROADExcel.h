#pragma once
#include "excel9.h"
class CMROADExcel
{
public:
	CMROADExcel(void);
	~CMROADExcel(void);

//�ڲ���Ա
private:
	//�Դ���Ա
	CString			m_FilePath;			//�ļ�·��
	bool			m_IsInitialize;		//�Ƿ�����˳�ʼ������
	CString			m_LastSheet;		//���һ���򿪵ı�

	//Excel������Ա
	_Application	m_ExcelApp;			//Ӧ�ó���
	Workbooks		m_ExcelWorkBooks;	//������
	_Workbook		m_ExcelWorkBook;	//����������
	Worksheets		m_ExcelWorksheets;	//��(Sheet)��
	_Worksheet		m_ExcelWorksheet;	//������
	Range			m_ExcelRange;		//��Ԫ��

//����ӿ�
public:
	
	//һ���������
	void InitExcel();									//��ʼ��
	void QuitExcel(bool IsOpenExcel = false);			//�˳�Excel

	//����·�����
	void SetFilePath(CString TargetPath);	//�����ļ�·��
	CString GetFilePath() const;			//��ȡ�ļ�·��
	void CreateExcel();						//����Excel�ļ�

	//������Excel����ز���
	//��һ������ز���
	//1.���ӱ�
	void ConnectExcelWookSheet(CString SheetName);
	void ConnectExcelWookSheet(CString TargetPath, CString SheetName);
	//2.������
	void CreateSheet(CString SheetName);
	//3.ɾ����
	void DeleteSheet(CString SheetName);
	//4.�˳�������
	void QuitSheetConnect();
	//5.Ѱ�ұ��Ƿ����
	bool IsSheetExist(CString SheetName);
	//��������д�����
	void WriteExcel(long Line, long Col, _variant_t Value);
	void ReadExcel(long Line, long Col, _variant_t& Value);
	//�����������ڲ���
	//1.�ϲ����
	bool MergeCellInExcel(long sLine, long eLine, CString sCol, CString eCol);
	//2.�Զ��������
	void AutoSize();
};

