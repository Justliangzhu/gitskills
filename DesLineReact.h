// DesLineReact.h: interface for the DesLineReact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESLINEREACT_H__4A7F69C1_96B8_11D4_AF18_006067772FF4__INCLUDED_)
#define AFX_DESLINEREACT_H__4A7F69C1_96B8_11D4_AF18_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LTGou.h"
#include "designline.h"
#include "earthline.h"
#include "JSGOU.h"
#include "HdmText.h"

class DesLineReact : public AcDbEntityReactor 
{
public:
	DesLineReact(AcDbObjectId LEarthId,AcDbObjectId REarthId,AcDbObjectId LTgouId,AcDbObjectId JSgouId,AcDbObjectId NoteId);
	virtual ~DesLineReact();
	virtual void modified(const AcDbObject* dbObj);     
	AcDbObjectId  m_LEarthObjId,m_REarthObjId,m_GouObjId,m_JSgouId,m_NoteId;
public:
	Adesk::Boolean YR_setCROSSAutoDesignData(int k,double EarH,DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou, EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);	  
	Adesk::Boolean R_setCROSSAutoDesignData(int k,double EarH,DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou, EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);	  
	Adesk::Boolean L_setCROSSAutoDesignData(int k,double EarH,DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou, EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);

   int CrosNew(int MR,double cml,double ZJ,double Yd,int EarPtSum,double EarPtList[],double RHF[10],
	   double& FillORCut,double& TWH,int& DesPtSum,double DesPtList[],
	   //�������������������,����ˮ���������������,�ڷ����
	   double& FillArea,double &FillGLTArea,double &FillSSTArea,double& QBTArea,double& CutArea,
	   //�õؿ�
	   double& ZDK,
	   //��ˮ������(1-ˮ����0-�½�ǽ)������,���꣬������x,y,�������������ˮ�����칵�����������������ˮ�����칵��
	   int &GouProperty, int &GouPtNum, double gou[],double &GouH,double &GouX,double &GouTarea,double &GouWarea,
	   //��ˮ������,����
	   int &JSGouPtNum, double JSgou[],
	   //�������,�����������,��ǽ����
	   double &HJarea,double &AllGouJQArea,double &DQlength,double &Grasslength);

   void AutoDesign(EarthLine_ROAD * l_earthline,LTGOU_ROAD * LTGou,JSGOU_ROAD* JSGou,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);
   void DoubleLMAutoDesign(DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD* JSGou,EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);	   
  void SingleLMAutoDesign(DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou,EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);
public:
	DesignLine_ROAD *designline;
	   
	
};

#endif // !defined(AFX_DESLINEREACT_H__4A7F69C1_96B8_11D4_AF18_006067772FF4__INCLUDED_)





















