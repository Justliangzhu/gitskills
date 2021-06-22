// CWorkDir.h: interface for the CWorkDir class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CWORKDIR_H__50EF48AA_4583_4EB9_BA74_DC08C9C97D55__INCLUDED_)
#define AFX_CWORKDIR_H__50EF48AA_4583_4EB9_BA74_DC08C9C97D55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "bas_draw_fun.h"
 
class CWorkDir  : public BAS_DRAW_FUN
{
public:
	CWorkDir();
	virtual ~CWorkDir();
	void WriteWorkdir(CString name);
	void WriteProjName(CString name);
	//��·�������ݿ�mdb��·�������ƣ���Ŀ����
	CString Path,DataPath,DbsPathName,ProjName,JdCenBackUp,JdCenBackUp2x;
	CString ProFname;//��Ŀ���Ʊ���·������project.txt��·��
	CString PmDrawSetFile;
	CString TKDwgFile;
	CString m_ZBZFile;
	bool ReadOk;//�Ƿ�һ������
};

#endif // !defined(AFX_CWORKDIR_H__50EF48AA_4583_4EB9_BA74_DC08C9C97D55__INCLUDED_)
