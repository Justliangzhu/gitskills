/*########################################################################
	Filename: 	ado.h
	----------------------------------------------------
		-- -- ���ϳ���ѧԺ ��Ϣ����ϵ 000641��
		-- -- ����
		-- -- anyou@sina.com
		-- -- anyou@msn.com
		-- -- 20/1/2003 19:08
	----------------------------------------------------
	��ע�����:
		1. ����������ָ��msado15.dll��ȫ·��,һ����"C:\Program Files\Common
		   Files\System\ADO\"Ŀ¼��.
		2. �������Ҫ��ʼ��COM����,���Ե���CAdoConnection::OleInit()������
		   ʼ��COM����.
		3. ��ʹ�ü�¼������ǰ�����ȵ���CAdoConnection::Connect()�������ݿ�,
		   ���Ӻ�ɸ����CAdoRecordSet����ʹ��, �������ر����ݿ�.
		5. �򿪼�¼��ǰ, �ȵ���CAdoRecordSet::SetAdoConnection()�������Ӷ�
		   ��, �������رռ�¼��.
		6. �ڱ�������в����������ı��뾯��:
		   warning: unary minus operator applied to unsigned type, result
		   still unsigned
		   �������˾������,������StdAfx.h�ļ��м�������һ�д����Խ�ֹ��
		   ����: 	#pragma warning(disable:4146) 
  ########################################################################*/
#if !defined(_ANYOU_COOL_ADOCONNETION_H)
#define _ANYOU_COOL_ADOCONNETION_H

#if _MSC_VER > 1000
#pragma once
#endif 
 
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF","adoEOF")

/*########################################################################
			------------------------------------------------
						CAdoConnection class
			------------------------------------------------
  ########################################################################*/
class CAdoConnection
{
//����-----------------------------------------------
public:
	static BOOL OleInit(); 
	CAdoConnection();
	virtual ~CAdoConnection();

//����-----------------------------------------------
public:
	BOOL SetConnectTimeOut(long lTime);
	BOOL IsOpen();
	_ConnectionPtr GetConnection();
	
//����-----------------------------------------------
public:
	BOOL Connect(LPCTSTR strConnect, long lOptions = adConnectUnspecified);
	BOOL Disconnect();

	BOOL Cancel();

	long BeginTrans();
	BOOL RollbackTrans();
	BOOL CommitTrans();

	_RecordsetPtr OpenSchema(SchemaEnum QueryType);
	CString GetLastError();

	BOOL Execute(CString strSQL);

//����-----------------------------------------------
protected:
	CString			m_strConnect;
	_ConnectionPtr	m_pConnection;
};

/*########################################################################
			------------------------------------------------
						CAdoRecordSet class
			------------------------------------------------
  ########################################################################*/

class CAdoRecordSet 
{
//����------------------------------------------------
public:
	CAdoRecordSet();
	CAdoRecordSet(CAdoConnection *pConnection);
	virtual ~CAdoRecordSet();
	HRESULT Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenDynamic, LockTypeEnum LockType = adLockOptimistic);
	void SetAdoConnection(CAdoConnection *pConnection);
	BOOL Close();
	
//����------------------------------------------------	
public:
	BOOL IsEOF();
	BOOL IsBOF();
	
	long GetState();
	long GetStatus();
	
	long GetRecordCount();
	long GetFieldsCount();

	CString GetLastError();

	CString GetFieldName(long lIndex);
	DWORD GetFieldType(long lIndex);
	DWORD GetFieldAttributes(long lIndex);
	long GetFieldDefineSize(long lIndex);
	long GetFieldActualSize(long lIndex);
    int GetRowCount();

	Fields* GetFields();
	FieldPtr GetField(long lIndex);
	const _RecordsetPtr& GetRecordset();
	
//��¼�����²���--------------------------------------
public:
	BOOL AddNew();
	BOOL Update();
	BOOL CancelUpdate();
	BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
	BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);
	
//��¼����������--------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, BookmarkEnum Start = adBookmarkCurrent);
	
//��������--------------------------------------------
public:
	BOOL Cancel();
	BOOL PutCollect(long index, const _variant_t &pver);
	BOOL GetCollect(_variant_t &index,long lFieldIndex);
	BOOL GetValueString(CString &strValue, CString strFieldName);
	BOOL GetValueString(CString &strValue, long lFieldIndex);
	_RecordsetPtr GetRecordsetPtr(){return m_pRecordset;}

	_RecordsetPtr operator =(_RecordsetPtr &pRecordSet);
	CString Format(_variant_t var);
	
//��Ա����--------------------------------------------
protected:
	CAdoConnection     *m_pConnection;
	_RecordsetPtr		m_pRecordset;
	CString				m_strSQL;
};

#endif // !defined(_ANYOU_COOL_ADOCONNETION_H)










































