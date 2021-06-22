// BaseSettting.cpp: implementation of the BaseSettting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BaseSettting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BaseSettting::BaseSettting()
{

}

BaseSettting::~BaseSettting()
{

}

void BaseSettting::Read()
{
	FILE *fp=_wfopen(L"C:\\PLHSetting.ini",L"r");
	if(fp)
	{
		fwscanf(fp,L"%d",&JDTDFS);
		fwscanf(fp,L"%lf",&JiaDianLMD);		
		fclose(fp);
	}
}

void BaseSettting::Write()
{
	FILE *fp=_wfopen(L"C:\\PLHSetting.ini",L"w+");
	if(fp)
	{
		fwprintf(fp,L"%d\n",JDTDFS);
		fwprintf(fp,L"%lf\n",JiaDianLMD);		
		fclose(fp);
	}
}


int BaseSettting::LCTextColor=4;//�����ɫ
int BaseSettting::LCLineColor=2;//�����ɫ
int BaseSettting::TZZTextColor=4;//��������ɫ
int BaseSettting::TZZLineColor=2;//��������ɫ
int BaseSettting::DLBTextColor=5;//������ɫ
int BaseSettting::DLBLineColor=2;//��������ɫ
int BaseSettting::JDLineColor=2;//��������ɫ
int BaseSettting::JDCirRadius=1.5;//����Բ�İ뾶
int BaseSettting::JDTDFS=0;//�����϶���ʽ
int BaseSettting::QXYSTextColor=1;//����Ҫ����ɫ
int BaseSettting::Rstep=0;//�뾶�϶�����Ϊ����
double BaseSettting::JiaDianLMD=1.0;//�е�������