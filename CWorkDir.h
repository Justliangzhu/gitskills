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
	//总路径，数据库mdb的路径及名称，项目名称
	CString Path,DataPath,DbsPathName,ProjName,JdCenBackUp,JdCenBackUp2x;
	CString ProFname;//项目名称保存路径，“project.txt”路径
	CString PmDrawSetFile;
	CString TKDwgFile;
	CString m_ZBZFile;
	bool ReadOk;//是否一切正常
};

#endif // !defined(AFX_CWORKDIR_H__50EF48AA_4583_4EB9_BA74_DC08C9C97D55__INCLUDED_)
