/*########################################################################
	Filename: 	ado.h
	----------------------------------------------------
		-- -- 湖南城市学院 信息工程系 000641班
		-- -- 成真
		-- -- anyou@sina.com
		-- -- anyou@msn.com
		-- -- 20/1/2003 19:08
	----------------------------------------------------
	【注意事项】:
		1. 必须在下面指明msado15.dll的全路径,一般在"C:\Program Files\Common
		   Files\System\ADO\"目录下.
		2. 程序必须要初始化COM环境,可以调用CAdoConnection::OleInit()函数初
		   始化COM环境.
		3. 在使用记录集对象前必须先调用CAdoConnection::Connect()连接数据库,
		   连接后可给多个CAdoRecordSet对象使用, 用完后请关闭数据库.
		5. 打开记录集前, 先调用CAdoRecordSet::SetAdoConnection()设置连接对
		   象, 用完后请关闭记录集.
		6. 在编译过程中不用理会下面的编译警告:
		   warning: unary minus operator applied to unsigned type, result
		   still unsigned
		   如果不想此警告出现,可以在StdAfx.h文件中加入这样一行代码以禁止此
		   警告: 	#pragma warning(disable:4146) 
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
//构建-----------------------------------------------
public:
	static BOOL OleInit(); 
	CAdoConnection();
	virtual ~CAdoConnection();

//属性-----------------------------------------------
public:
	BOOL SetConnectTimeOut(long lTime);
	BOOL IsOpen();
	_ConnectionPtr GetConnection();
	
//操作-----------------------------------------------
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

//数据-----------------------------------------------
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
//构建------------------------------------------------
public:
	CAdoRecordSet();
	CAdoRecordSet(CAdoConnection *pConnection);
	virtual ~CAdoRecordSet();
	HRESULT Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenDynamic, LockTypeEnum LockType = adLockOptimistic);
	void SetAdoConnection(CAdoConnection *pConnection);
	BOOL Close();
	
//属性------------------------------------------------	
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
	
//记录集更新操作--------------------------------------
public:
	BOOL AddNew();
	BOOL Update();
	BOOL CancelUpdate();
	BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
	BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);
	
//记录集导航操作--------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, BookmarkEnum Start = adBookmarkCurrent);
	
//其他方法--------------------------------------------
public:
	BOOL Cancel();
	BOOL PutCollect(long index, const _variant_t &pver);
	BOOL GetCollect(_variant_t &index,long lFieldIndex);
	BOOL GetValueString(CString &strValue, CString strFieldName);
	BOOL GetValueString(CString &strValue, long lFieldIndex);
	_RecordsetPtr GetRecordsetPtr(){return m_pRecordset;}

	_RecordsetPtr operator =(_RecordsetPtr &pRecordSet);
	CString Format(_variant_t var);
	
//成员变量--------------------------------------------
protected:
	CAdoConnection     *m_pConnection;
	_RecordsetPtr		m_pRecordset;
	CString				m_strSQL;
};

#endif // !defined(_ANYOU_COOL_ADOCONNETION_H)










































