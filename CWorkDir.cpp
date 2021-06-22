// CWorkDir.cpp: implementation of the CWorkDir class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "CWorkDir.h"
//#include "BAS_DRAW_FUN.h" //

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkDir::CWorkDir()
{
	ReadOk=false;
	ACHAR fname[256];

	////////////////////////////////////////////////
	/*ACHAR SysPath[256];
	_tcscpy(fname,acedGetAppName());
	CString fstr;
	fstr = fname;
	fstr.MakeUpper();
	fstr.Replace(L"\\ROADDESIGN.ARX",NULL);
	_tcscpy(SysPath,fstr);*/

	///////////////////////////////////////////////////(zdf��)

	_stprintf(fname,L"%s\\workdir",SysPath);

	FILE *fpr=_wfopen(fname,L"r");
	if(!fpr)
	{
		AfxMessageBox(L"����û���趨��Ŀ����!");
		Cworkdir="";
		return ;
	}

	fwscanf(fpr,L"%s",fname);
	fclose(fpr);
	Cworkdir=fname;
#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	//Cworkdir += "\\���Ƶ�·"; 
#endif
	InStr$toNull();	

	Cworkdir.MakeUpper();
	CString ProFname;
	ProFname=Cworkdir+"\\project";

	fpr = _wfopen(ProFname,L"r");
	if(!fpr)
	{
		AfxMessageBox(L"����û��ָ����Ʒ�������!");
	//	ads_printf(L"����û��ָ����Ʒ�������!\n");
		return ;
	}
	fwscanf(fpr,L"%s",fname);
	fclose(fpr);
	Pro = fname;

	//20191106	�޸Ķ�ȡ
	int len, i;

	len = Pro.GetLength();
	i = Pro.Find('$');
	while (i < len && i >= 0)
	{
		Pro.SetAt(i, ' ');
		i = Pro.Find('$');
	}

	Path = Cworkdir;
	ProjName = Pro ;

	DbsPathName=Path;
	TKDwgFile=Path;
	DbsPathName+="\\DATA\\";
	DataPath=DbsPathName;
	DbsPathName+=ProjName;
	JdCenBackUp=DbsPathName;
	JdCenBackUp2x=DbsPathName;
	PmDrawSetFile=DbsPathName;
	DbsPathName+=".mdb";
	JdCenBackUp+=".JdCen";
	JdCenBackUp2x+=".JdCen2x";
	PmDrawSetFile+=".PmDrawSet";
	TKDwgFile+="\\DATA\\ƽ��ͼ��.dwg";
	m_ZBZFile.Format(L"%s\\dwg\\ָ����.dwg",SysPath);
	ReadOk=true;	
}

CWorkDir::~CWorkDir()
{

}

void CWorkDir::WriteWorkdir(CString name)
{
	name.Replace(L" ",L"!@#$%^&*");
	FILE *fp=_wfopen(L"c:\\PLHworkdir.txt",L"w");
	if(fp)
	{	
		fwprintf(fp,L"%s\n",name);
		fclose(fp);
	}
}

void CWorkDir::WriteProjName(CString name)
{
	name.Replace(L" ",L"!@#$%^&*");

	FILE *fp=NULL;
	fp=_wfopen(ProFname,L"w");
	if(fp)
	{
		fwprintf(fp,L"%s\n",name);
		fclose(fp);
	}
}