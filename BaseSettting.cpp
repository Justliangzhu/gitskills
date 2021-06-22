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


int BaseSettting::LCTextColor=4;//里程字色
int BaseSettting::LCLineColor=2;//里程线色
int BaseSettting::TZZTextColor=4;//特征点字色
int BaseSettting::TZZLineColor=2;//特征线字色
int BaseSettting::DLBTextColor=5;//断链字色
int BaseSettting::DLBLineColor=2;//断链线字色
int BaseSettting::JDLineColor=2;//交点连线色
int BaseSettting::JDCirRadius=1.5;//交点圆的半径
int BaseSettting::JDTDFS=0;//交点拖动方式
int BaseSettting::QXYSTextColor=1;//曲线要素颜色
int BaseSettting::Rstep=0;//半径拖动步长为任意
double BaseSettting::JiaDianLMD=1.0;//夹点灵敏度