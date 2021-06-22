// HdmText.cpp: implementation of the HdmText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "HdmText.h"
#include "dbproxy.h"
#include "geassign.h"
#include "acgi.h"
#include "math.h"
extern double Sfactor;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ACRX_DXF_DEFINE_MEMBERS(HdmText_ROAD, AcDbEntity,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						AcDbProxyEntity::kAllAllowedBits,
						0,HDMTEXT,RailHdm
						);

HdmText_ROAD::HdmText_ROAD()
{
	ZZ_twh = ZY_twh = m_fillarea = m_cutarea = 0.0;	
	YZ_twh = YY_twh  = 0.0;	
	DuoluoInf=offsetX=YoffsetX=BTinf=Y_BTinf=LZDK=RZDK=EarH=0.0;
	DorS=0;
	int i;
	for(i=0; i<30; i++)
		m_ZBP[i]=m_YBP[i]=0.0;
}

HdmText_ROAD::~HdmText_ROAD()
{

}

Adesk::Boolean HdmText_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{
	assertReadEnabled();
	//绘左侧地面线
	pWd->subEntityTraits().setColor(2);

	AcGePoint3d origin;
	ACHAR Str[30];
	AcGePoint3d pt[4];
	AcGeVector3d direction(1.0, 0.0, 0.0);
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面
	AcGiTextStyle *pTextStyle;

	//	pTextStyle = new AcGiTextStyle;

	//  pTextStyle->setFont(L"宋体",false,false,100,0);
	if(DorS==1)//单路面
	{
		origin.x = mpt.x - 0.7*_tcslen(m_ckml);
		origin.y = mpt.y ;
		pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, m_ckml);

		origin.y = mpt.y - 5.0;
		if(fabs(ZZ_twh-ZY_twh)<0.1)
		{
			if(ZZ_twh>0.0)
				_stprintf(Str,L"T = %0.3lf",ZZ_twh);
			else
				_stprintf(Str,L"W = %0.3lf",fabs(ZZ_twh));
		}
		else//左右不等高,标两个填挖高
		{
			if(ZZ_twh>0.0)
				_stprintf(Str,L"ZT = %0.3lf",ZZ_twh);
			else
				_stprintf(Str,L"ZW = %0.3lf",fabs(ZZ_twh));
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
			origin.y = mpt.y - 5.0;
			if(ZY_twh>0.0)
				_stprintf(Str,L"YT = %0.3lf",ZY_twh);
			else
				_stprintf(Str,L"YW = %0.3lf",fabs(ZY_twh));
		}

		pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
		origin.y -=  5.0;
		_stprintf(Str,L"AT = %0.2lf",m_fillarea);
		pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);
		origin.y -=  5.0;
		_stprintf(Str,L"AW = %0.2lf",m_cutarea);
		pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);

	}

	else if(DorS==2)//双路面，先标主线
	{
		origin.x = mpt.x - 0.7*_tcslen(m_ckml);
		origin.y = mpt.y ;
		pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, m_ckml);

		origin.y = mpt.y - 5.0;
		if(XJJ>-0.0001)//左线为主线，右线分离
		{
			if(fabs(ZZ_twh-ZY_twh)<0.1)
			{
				if(ZZ_twh>0.0)
					_stprintf(Str,L"T = %0.3lf",ZZ_twh);
				else
					_stprintf(Str,L"W = %0.3lf",fabs(ZZ_twh));
			}
			else//左右不等高,标两个填挖高
			{
				if(fabs(ZZ_twh)>0.001)
				{
					if(ZZ_twh>0.0)
						_stprintf(Str,L"ZT = %0.3lf",ZZ_twh);
					else 
						_stprintf(Str,L"ZW = %0.3lf",fabs(ZZ_twh));
					pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
				}
				if(fabs(ZY_twh)>0.001)
				{
					origin.y = origin.y  - 5.0;
					if(ZY_twh>0.0)
						_stprintf(Str,L"YT = %0.3lf",ZY_twh);
					else
						_stprintf(Str,L"YW = %0.3lf",fabs(ZY_twh));
				}

			}
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
			origin.y -=  5.0;
			_stprintf(Str,L"AT = %0.2lf",m_fillarea);
			pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);
			origin.y -=  5.0;
			_stprintf(Str,L"AW = %0.2lf",m_cutarea);
			pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);
			origin.y -=  5.0;
			//标分离线（右线）

			origin.x = mpt.x +  XJJ*Sfactor - 0.7*_tcslen(FLD_ckml);
			origin.y = origin.y;
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, FLD_ckml);
			origin.y -=  5.0;
			if(fabs(YZ_twh-YY_twh)<0.1)
			{
				if(YZ_twh>0.0)
					_stprintf(Str,L"T = %0.3lf",YZ_twh);
				else
					_stprintf(Str,L"W = %0.3lf",fabs(YZ_twh));
			}
			else//左右不等高,标两个填挖高
			{
				if(fabs(YZ_twh)>0.001)
				{
					if(YZ_twh>0.0)
						_stprintf(Str,L"ZT = %0.3lf",YZ_twh);
					else
						_stprintf(Str,L"ZW = %0.3lf",fabs(YZ_twh));
					pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
				}
				if(fabs(YY_twh)>0.001)
				{
					origin.y = origin.y - 5.0;
					if(YY_twh>0.0)
						_stprintf(Str,L"YT = %0.3lf",YY_twh);
					else
						_stprintf(Str,L"YW = %0.3lf",fabs(YY_twh));
				}				
			}
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);

		}
		else//左线分离,右线为主线
		{
			if(fabs(YZ_twh-YY_twh)<0.1)
			{
				if(YZ_twh>0.0)
					_stprintf(Str,L"T = %0.3lf",YZ_twh);
				else
					_stprintf(Str,L"W = %0.3lf",fabs(YZ_twh));
			}
			else//左右不等高,标两个填挖高
			{
				if(fabs(YZ_twh)>0.001)
				{
					if(YZ_twh>0.0)
						_stprintf(Str,L"ZT = %0.3lf",YZ_twh);
					else
						_stprintf(Str,L"ZW = %0.3lf",fabs(YZ_twh));
					pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
				}
				if(fabs(YY_twh)>0.001)
				{
					origin.y = origin.y  - 5.0;
					if(YY_twh>0.0)
						_stprintf(Str,L"YT = %0.3lf",YY_twh);
					else
						_stprintf(Str,L"YW = %0.3lf",fabs(YY_twh));
				}
			}
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
			origin.y -=  5.0;
			_stprintf(Str,L"AT = %d",int(m_fillarea+0.5));
			pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);
			origin.y -=  5.0;
			_stprintf(Str,L"AW = %d",int(m_cutarea+0.5));
			pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);
			origin.y -=  5.0;
			//标分离线（左线）

			origin.x = mpt.x + XJJ*Sfactor- 0.7*_tcslen(FLD_ckml);
			origin.y = origin.y ;
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, FLD_ckml);
			origin.y -=  5.0;
			if(fabs(ZZ_twh-ZY_twh)<0.1)
			{
				if(YZ_twh>0.0)
					_stprintf(Str,L"T = %0.3lf",ZZ_twh);
				else
					_stprintf(Str,L"W = %0.3lf",fabs(ZZ_twh));
			}
			else//左右不等高,标两个填挖高
			{
				if(fabs(ZZ_twh)>0.0001)
				{
					if(ZZ_twh>0.0)
						_stprintf(Str,L"ZT = %0.3lf",ZZ_twh);
					else
						_stprintf(Str,L"ZW = %0.3lf",fabs(ZZ_twh));
					pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
					origin.y = origin.y - 5.0;
				}

				if(fabs(ZY_twh)>0.0001)
					if(ZY_twh>0.0)
						_stprintf(Str,L"YT = %0.3lf",ZY_twh);
					else
						_stprintf(Str,L"YW = %0.3lf",fabs(ZY_twh));
			}
			pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);

		}
	}

	NoteBP(pWd);//标注边坡
	return Adesk::kTrue;
}

Acad::ErrorStatus HdmText_ROAD::subTransformBy(const AcGeMatrix3d& xform)
{
	assertWriteEnabled();	
	mpt.transformBy(xform);

	return AcDbEntity::subTransformBy(xform);
}

void  HdmText_ROAD::NoteBP(AcGiWorldDraw* pWd)//标注边坡
{
	int i,ptnum,sno,ibppt,ibp;
	ACHAR Str[20],mes[280];
	AcGePoint3d pt1,pt2,mpt;
	AcGeVector3d direction(1.0, 0.0, 0.0);
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面

	ptnum = m_ZSlopPt.length();

	//	if(ptnum > 1)//一级边坡
	//	{
	ibppt = 0;
	ibp = 0;
	while(ptnum >= ibppt+2)
	{
		//  pt1 = m_ZSlopPt.at(0);
		//  pt2 = m_ZSlopPt.at(1);
		pt1 = m_ZSlopPt.at(ibppt);
		pt2 = m_ZSlopPt.at(ibppt+1);

		_tcscpy(Str,L"1:");
		//note_side_slop(pt1,pt2,m_ZBP[0],Str,1.5,pWd);
		note_side_slop(pt1,pt2,m_ZBP[ibp],Str,1.5,pWd);
		ibppt += 2;
		ibp++;		   

	}

	/*		if(ptnum > 3)//二级边坡
	{
	pt1 = m_ZSlopPt.at(2);
	pt2 = m_ZSlopPt.at(3);
	_tcscpy(Str,L"1:");
	note_side_slop(pt1,pt2,m_ZBP[1],Str,1.5,pWd);

	if(ptnum > 5)//三级边坡
	{
	pt1 = m_ZSlopPt.at(4);
	pt2 = m_ZSlopPt.at(5);
	_tcscpy(Str,L"1:");
	note_side_slop(pt1,pt2,m_ZBP[2],Str,1.5,pWd);
	}
	}
	}*/

	ptnum = m_YSlopPt.length();
	_tcscpy(Str,L"1:");
	ibppt = 0;
	ibp = 0;
	while(ptnum >= ibppt+2)
	{
		pt2 = m_YSlopPt.at(ibppt);
		pt1 = m_YSlopPt.at(ibppt+1);
		_tcscpy(Str,L"1:");
		note_side_slop(pt1,pt2,m_YBP[ibp],Str,1.5,pWd);
		ibppt += 2;
		ibp++;		 

	}
}
//-----------------------------------------------------------------------------

void HdmText_ROAD::note_side_slop(AcGePoint3d pt1,AcGePoint3d pt2,double m,ACHAR prestr[20],
								  double txth,AcGiWorldDraw* pWd)
{
	AcGePoint3d mpt;
	AcGeVector3d direction,PerDir;
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面
	ACHAR s[10];

	direction = pt1 - pt2;
	PerDir = direction.perpVector();
	if(fabs(pt1.y-pt2.y) < 5.0)
		return;

	mpt.x = 0.5*(pt1.x+pt2.x);
	mpt.y = 0.5*(pt1.y+pt2.y);
	// ads_printf(L"%lf %lf\n",m,m*10-((int)(m+0.001))*10);
	mpt += PerDir;

	if(fabs(m-(int)(m+0.001))<0.1)
	{
		_stprintf(s,L"%d\n",(int)(m+0.01));
	}
	else if(fabs(m*10-(int)((m+0.001)*10)) < 0.1)
	{
		//	  ads_printf(L"%lf \n",m);
		_stprintf(s,L"%2.1f\n",(float)m);
	}
	else
	{
		_stprintf(s,L"%3.2f\n",(float)m);
	}
	_tcscat(prestr,s);
	pWd->geometry().text(mpt, normal, direction, 2.0, 1.0, 0.0, prestr);

}

void  HdmText_ROAD::OutDrawBP(double NoteTextH)//标注边坡
{
	int i,ptnum,sno,ibppt,ibp;
	ACHAR Str[20],mes[280];
	AcGePoint3d pt1,pt2,mpt;
	AcGeVector3d direction(1.0, 0.0, 0.0);
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"横断面图-横坡");

	ptnum = m_ZSlopPt.length();

	//	if(ptnum > 1)//一级边坡
	//	{
	ibppt = 0;
	ibp = 0;
	while(ptnum >= ibppt+2)
	{
		//  pt1 = m_ZSlopPt.at(0);
		//  pt2 = m_ZSlopPt.at(1);
		pt2 = m_ZSlopPt.at(ibppt);
		pt1 = m_ZSlopPt.at(ibppt+1);

		_tcscpy(Str,L"1:");
		//note_side_slop(pt1,pt2,m_ZBP[0],Str,1.5,pWd);
		//梁柱9.29，边坡标注保留字体大小
		note_side_slop_outdraw(pt1,pt2,m_ZBP[ibp],Str, /*NoteTextH*/0.3 * Sfactor, LayerName);
		ibppt += 2;
		ibp++;		   

	}

	/*		if(ptnum > 3)//二级边坡
	{
	pt1 = m_ZSlopPt.at(2);
	pt2 = m_ZSlopPt.at(3);
	_tcscpy(Str,L"1:");
	note_side_slop(pt1,pt2,m_ZBP[1],Str,1.5,pWd);

	if(ptnum > 5)//三级边坡
	{
	pt1 = m_ZSlopPt.at(4);
	pt2 = m_ZSlopPt.at(5);
	_tcscpy(Str,L"1:");
	note_side_slop(pt1,pt2,m_ZBP[2],Str,1.5,pWd);
	}
	}
	}*/

	ptnum = m_YSlopPt.length();
	_tcscpy(Str,L"1:");
	ibppt = 0;
	ibp = 0;
	while(ptnum >= ibppt+2)
	{
		pt1 = m_YSlopPt.at(ibppt);
		pt2 = m_YSlopPt.at(ibppt+1);
		_tcscpy(Str,L"1:");
		note_side_slop_outdraw(pt1, pt2, m_YBP[ibp], Str, 0.3 * Sfactor, LayerName);
		ibppt += 2;
		ibp++;		 

	}
}


void HdmText_ROAD::note_side_slop_outdraw(AcGePoint3d pt1,AcGePoint3d pt2,double m,ACHAR prestr[20],
										  double txth, ACHAR* LayerName)
{
	AcGePoint3d mpt;
	AcGeVector3d direction,PerDir;
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面
	ACHAR s[10];

	direction = pt1 - pt2;
	PerDir = direction.perpVector();
	if(fabs(pt1.y-pt2.y) < 5.0)
		return;

	mpt.x = 0.5*(pt1.x+pt2.x);
	mpt.y = 0.5*(pt1.y+pt2.y);
	// ads_printf(L"%lf %lf\n",m,m*10-((int)(m+0.001))*10);
	mpt -= 1.5*PerDir;

	if(fabs(m-(int)(m+0.001))<0.1)
	{
		_stprintf(s,L"%d\n",(int)(m+0.01));
	}
	else if(fabs(m*10-(int)((m+0.001)*10)) < 0.1)
	{
		//	  ads_printf(L"%lf \n",m);
		_stprintf(s,L"%2.1f\n",(float)m);
	}
	else
	{
		_stprintf(s,L"%3.2f\n",(float)m);
	}
	_tcscat(prestr,s);
	AcGeVector3d Xnormal(1.0, 0.0, 0.0);
	double angle,ang;
	//angle = direction.angleTo(Xnormal);
	ads_point bpt1,bpt2;
	bpt1[0]=pt1.x,bpt1[1]=pt1.y;
	bpt2[0]=pt2.x,bpt2[1]=pt2.y;
	ang=ads_angle(bpt1,bpt2);
	//ang=ang*180/pi;
	//ads_point pt;
	//pt[0]=mpt.x,pt[1]=mpt.y;
	//acedCommandS(RTSTR,L"text",RTSTR,L"j",RTSTR,L"ce",RTPOINT,mpt,RTREAL,txth,
	//	RTREAL,ang,RTSTR,prestr,0);


	maketext(mpt,prestr,ang,txth,2,3,L"宋体", LayerName);
	//pWd->geometry().text(mpt, normal, direction, 2.0, 1.0, 0.0, prestr);

}

Adesk::Boolean HdmText_ROAD::Draw(int iColor)
{

	assertReadEnabled();
	/*//绘左侧地面线
	//    pWd->subEntityTraits().setColor(2);

	AcGePoint3d origin;
	char Str[30];
	AcGePoint3d pt[4];
	AcGeVector3d direction(1.0, 0.0, 0.0);
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面
	AcGiTextStyle *pTextStyle;

	//	pTextStyle = new AcGiTextStyle;

	//  pTextStyle->setFont(L"宋体",false,false,100,0);

	origin.x = mpt.x;
	origin.y = mpt.y ;
	maketext(origin,m_ckml,0,2.0,iColor,2);
	//pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, m_ckml);

	origin.y = mpt.y - 5.0;
	if(m_twh>0.0)
	_stprintf(Str,L"T = %0.3lf",m_twh);
	else
	_stprintf(Str,L"W = %0.3lf",fabs(m_twh));

	//	pWd->geometry().text(origin, normal, direction, 2.0, 1.0, 0.0, Str);
	maketext(origin,Str,0,2.0,iColor,2);
	origin.y -=  5.0;
	_stprintf(Str,L"AT = %0.3lf",m_fillarea);
	maketext(origin,Str,0,2.0,iColor,2);
	//	pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);

	origin.y -=  5.0;
	_stprintf(Str,L"AW = %0.3lf",m_cutarea);
	//	pWd->geometry().text(origin, normal, direction, 2.0,1.0,0.0, Str);
	maketext(origin,Str,0,2.0,iColor,2);*/
	//  NoteBP();//标注边坡

	return Adesk::kTrue;
}
