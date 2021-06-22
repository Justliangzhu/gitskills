// BlockComputerFun.cpp: implementation of the BlockComputerFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BlockComputerFun.h"
#include "JD_CENTER.H"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BlockComputerFun::BlockComputerFun()
{

}

BlockComputerFun::~BlockComputerFun()
{

}
//����ֱ֪�ߵķ�λ��ʱ���ֱ�ߵ�б��K;
double BlockComputerFun::GetValueKByOrient(double ValueOrient)
{
	//����б�ʱ���;
	double ValueK;
	//�жϷ�λ���Ƿ���0��180��;
	if (fmod(ValueOrient,360) == 0||
		fmod(ValueOrient,360) == 180)
	{
		ValueK = 1E50;//�����;
	}
	else ValueK = tan(PI/2-ValueOrient*PI/180);
	//���������Kֵ;
	return ValueK;
}
//��֪�����ȡ��������(��ֱ�߳���);
double BlockComputerFun::GetPreciseLength(double x1,double y1,double x2,double y2)
{
	//����������,�ݺ����������Լ��ݺ���������ֵ����;
	double Distance,DistanceX,DistanceY,InterimX,InterimY;
	//��ȡ�ݺ������Լ��ݺ���������ֵ;
	DistanceX = x1-x2;
	DistanceY = y1-y2;
	InterimX = fabs(DistanceX);
	InterimY = fabs(DistanceY);
	//��ȡ���ݺ���������ֵ�����������С��;
	double ValueMax,ValueMin;
	ValueMax = (InterimX>InterimY)?InterimX:InterimY;
	ValueMin = (InterimX<InterimY)?InterimX:InterimY;
	//��ȡ���ݺ���������ֵ����С��������ߵı�ֵ;
	double ValueScale;
	ValueScale = ValueMin/ValueMax;
	//��ȡ�����ľ���;
	Distance = ValueMax*sqrt(1.0+ValueScale*ValueScale);
	//������������;
	return Distance;	
}
//��ֱ����һ����ֱ���ϵ�ͶӰ������;
void BlockComputerFun::GetProjectDotOfLine(double LineX1,double LineY1,double ValueK,
										   double SelDotX,double SelDotY,double &ProjX,double &ProjY)
{
	//�����ǰֱ����Y��ƽ�м�ValueK��ֵΪ�����;
	if (ValueK == 1E50)
	{
		//ͶӰ��������ֱ�ӿ���;
		ProjX = LineX1;   //(��ProjX = LineX2);
		ProjY = SelDotY;  
	}
	else
	{
		double ValueA,ValueB;
		ValueA = ValueK/sqrt(ValueK*ValueK+1);
		ValueB = 1/sqrt(ValueK*ValueK+1);
		//��ȡѡȡ����ֱ�ߵľ���;
		double Dist;
		Dist = ValueA*SelDotX-ValueB*SelDotY-(ValueA*LineX1-ValueB*LineY1);
		//�����ǰ����ֱ������;
		if (Dist == 0) 
		{
			//ͶӰ�㼴Ϊѡȡ��;
			ProjX = SelDotX;   
			ProjY = SelDotY; 
		}
		else 
		{
			//��ȡѡȡ��ͶӰ������;
			ProjX = SelDotX - Dist*ValueA;
			ProjY = SelDotY + Dist*ValueB;
		}
	}
}
double BlockComputerFun::ConvertAngleToOrient(double AngleOfLineAndXaxes)
{
	AngleOfLineAndXaxes = AngleOfLineAndXaxes*180/PI;
	//������ֵ�Ƿ��Ƿ�����ֵ(����0~360�ȼ������);
	if (fabs(AngleOfLineAndXaxes)>360) 
	{
		//�������Ľ�ֵ����ɾ���ֵ��0~360��ȵĽ�ֵ;
		AngleOfLineAndXaxes = fmod(AngleOfLineAndXaxes,360);
	}
	//�������Ľ�ֵ����ɾ���ֵ��0~360��ȵ�����;
	if (AngleOfLineAndXaxes<0) 
		AngleOfLineAndXaxes = AngleOfLineAndXaxes+360;
	//���������ֵ��0~90��֮��(�൱�ڷ�λ�ǵ�һ����);
	if (AngleOfLineAndXaxes>=0&&AngleOfLineAndXaxes<90) 
		return (90-AngleOfLineAndXaxes)*PI/180;
	//���������ֵ��90~360��֮��(�൱�ڷ�λ�ǵ���,��,������);
	else return (450-AngleOfLineAndXaxes)*PI/180;
	//���ؽ�ֵ�ĵ�λ�Ƕ�;
}

void BlockComputerFun::GetCrossDotOfLine(double ValueK1,double LineX1,double LineY1,
										 double ValueK2,double LineX2,double LineY2,
										 double &CrossX,double &CrossY)
{
	//�����һ��ֱ����Y��ƽ��;
	if (ValueK1 == 1E50)
	{
		CrossX = LineX1;
		CrossY = ValueK2*(LineX1-LineX2)+LineY2;
		return ;
	}
	//����ڶ���ֱ����Y��ƽ��;
	else if (ValueK2 == 1E50)
	{
		CrossX = LineX2;
		CrossY = ValueK1*(LineX2-LineX1)+LineY1;
		return ;
	}
	//�����ֱ��ƽ��;
	else if(ValueK1 == ValueK2)
	{
		CrossX = 1E50;
		CrossY = 1E50;
		return ;
	}
	else
	{
		CrossX = (ValueK1*LineX1-ValueK2*LineX2-LineY1+LineY2)/(ValueK1-ValueK2);
		CrossY = ValueK1*(CrossX-LineX1)+LineY1;
		return ;
	}
}

//��ֱ����һ�㵽ֱ�ߵľ���;
double BlockComputerFun::GetDistOfLineAndSelDot(double ValueK,double LineX,double LineY,
												double SelDotX,double SelDotY)
{
	//���ֱ����Y��ƽ��;
	if (ValueK ==1E40)
		return SelDotX-LineX;
	else
		return (ValueK*SelDotX-SelDotY-(ValueK*LineX-LineY))/sqrt(ValueK*ValueK+1);
}


BOOL BlockComputerFun::GetCenterCorAndRadius(double CurCenX,double CurCenY,double CurRadius,double PreDeflect,double &ItemDeflect,
											 double FixDotX,double FixDotY,double MoveDotX,double MoveDotY,
											 double &ComCenX,double &ComCenY,double &ComRadius)
{
	//����Բ�����һ���ĳ���;
	double Length,Angle1;
	Length = distfwj(CurCenX,CurCenY,FixDotX,FixDotY,&Angle1);
	//�����ǰԲ������Բ�����;
	if (Length > CurRadius+0.0001)
	{	
		//ItemDeflect��ʾ����ʱ������ƫ���߻�����ƫ����;
		//�����ڶ����λ��;
		//�жϵ��Ƿ����ڵ�ǰԲ��;
		double InterAngle,InterLen;
		InterLen = distfwj(CurCenX,CurCenY,MoveDotX,MoveDotY,&InterAngle);
		if (fabs(InterLen - CurRadius)<DataPrecision) 
		{
			return FALSE;	
		}
		//������Բ���������������д��߷���;
		double MidDotX,MidDotY,ValueK;
		MidDotX = (FixDotX+MoveDotX)/2;
		MidDotY = (FixDotY+MoveDotY)/2;
		ValueK = GetValueKByTwoPoint(FixDotX,FixDotY,MoveDotX,MoveDotY);
		//����д��������ᴹֱ;
		if (fabs(ValueK)<0.00000001) ValueK = 1E50;
		else if (fabs(ValueK- 1E50)<0.00000001) ValueK = 0;
		else ValueK = -1/ValueK;
		//����ʱͨ����֪����������֪Բ���е�Բ��뾶��Բ������;
		if (fabs(ValueK- 1E50)<0.00000001)
		{
			//������Բ��Բ�����������Ϊ;
			ComCenX = MidDotX;
			//��������Բ��Բ������������;
			//��������ϵ������;
			double FacA,FacB,FacC;
			FacA = 4*((FixDotY-CurCenY)*(FixDotY-CurCenY)-CurRadius*CurRadius);
			FacB = 4*(CurCenY-FixDotY)*((ComCenX-FixDotX)*(ComCenX-FixDotX)-
				(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+
				FixDotY*FixDotY-CurCenY*CurCenY)+8*CurRadius*CurRadius*CurCenY;
			FacC = -4*CurRadius*CurRadius*((ComCenX-CurCenX)*(ComCenX-CurCenX)+CurCenY*CurCenY)
				+((ComCenX-FixDotX)*(ComCenX-FixDotX)-(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+
				FixDotY*FixDotY-CurCenY*CurCenY)*((ComCenX-FixDotX)*(ComCenX-FixDotX)
				-(ComCenX-CurCenX)*(ComCenX-CurCenX)
				-CurRadius*CurRadius+FixDotY*FixDotY-CurCenY*CurCenY);
			//���Yֵ��С;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double Y1,Y2;
				Y1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				//�ж���ЧԲ��ֵ;
				if (PreDeflect == 1)//ǰһ��Ԫ����ƫ;
				{				
					if (fabs(sqrt((CurCenX-ComCenX)*(CurCenX-ComCenX)+
						(CurCenY-Y1)*(CurCenY-Y1))+ItemDeflect*CurRadius-
						sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y1)*(FixDotY-Y1)))<0.00000001)
						ComCenY = Y1;
					else if 
						(fabs(sqrt((CurCenX-ComCenX)*(CurCenX-ComCenX)+
						(CurCenY-Y2)*(CurCenY-Y2))+ItemDeflect*CurRadius-
						sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y2)*(FixDotY-Y2)))<0.00000001)
						ComCenY = Y2;
					else 
					{
						return FALSE;
					}
				}
				else//ǰһ��Ԫ����ƫ;
				{
					if (fabs(sqrt((CurCenX-ComCenX)*(CurCenX-ComCenX)+
						(CurCenY-Y1)*(CurCenY-Y1))-ItemDeflect*CurRadius-
						sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y1)*(FixDotY-Y1)))<0.00000001)
						ComCenY = Y1;
					else if 
						(fabs(sqrt((CurCenX-ComCenX)*(CurCenX-ComCenX)+
						(CurCenY-Y2)*(CurCenY-Y2))-ItemDeflect*CurRadius-
						sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y2)*(FixDotY-Y2)))<0.00000001)
						ComCenY = Y2;
					else return FALSE;
				}
				//��ȡ����Բ�İ뾶;
				ComRadius = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+
					(FixDotY-ComCenY)*(FixDotY-ComCenY));
			}		
		}
		else
		{
			//��ʱ��֪Բ�ĺ�������������֮��Ĺ�ϵY=K(X-X1)+Y1;;
			//�������ϵ������;
			double FacA,FacB,FacC,FacD,FacE;
			FacE = 2*(ValueK*(CurCenY-FixDotY)+(CurCenX-FixDotX));
			FacD = FixDotX*FixDotX+(MidDotY-ValueK*MidDotX-FixDotY)*(MidDotY-ValueK*MidDotX-FixDotY)
				-(CurCenX*CurCenX+CurRadius*CurRadius+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY));

			FacA = 4*CurRadius*CurRadius*(1+ValueK*ValueK)-FacE*FacE;
			FacB = 8*CurRadius*CurRadius*(ValueK*(MidDotY-ValueK*MidDotX-CurCenY)-CurCenX)-2*FacD*FacE;
			FacC = 4*CurRadius*CurRadius*(CurCenX*CurCenX+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY))-FacD*FacD;
			//���Xֵ��С;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double X1,X2,Y1,Y2;
				X1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y1 = ValueK*(X1-MidDotX)+MidDotY;
				X2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				Y2 = ValueK*(X2-MidDotX)+MidDotY;
				//�ж���ЧԲ��ֵ;
				if (PreDeflect == 1)//ǰһ��Ԫ����ƫ;
				{
					if (fabs(sqrt((CurCenX-X1)*(CurCenX-X1)+
						(CurCenY-Y1)*(CurCenY-Y1))+ItemDeflect*CurRadius-
						sqrt((FixDotX-X1)*(FixDotX-X1)+(FixDotY-Y1)*(FixDotY-Y1)))<0.00000001) 
					{
						ComCenX = X1;
						ComCenY = Y1;
					}
					else if 
						(fabs(sqrt((CurCenX-X2)*(CurCenX-X2)+
						(CurCenY-Y2)*(CurCenY-Y2))+ItemDeflect*CurRadius-
						sqrt((FixDotX-X2)*(FixDotX-X2)+(FixDotY-Y2)*(FixDotY-Y2)))<0.00000001)
					{
						ComCenX = X2;
						ComCenY = Y2;
					}
					else return FALSE;
				}
				else//ǰһ��Ԫ����ƫ;
				{
					if (fabs(sqrt((CurCenX-X1)*(CurCenX-X1)+
						(CurCenY-Y1)*(CurCenY-Y1))-ItemDeflect*CurRadius-
						sqrt((FixDotX-X1)*(FixDotX-X1)+(FixDotY-Y1)*(FixDotY-Y1)))<0.00000001) 
					{
						ComCenX = X1;
						ComCenY = Y1;
					}
					else if 
						(fabs(sqrt((CurCenX-X2)*(CurCenX-X2)+
						(CurCenY-Y2)*(CurCenY-Y2))-ItemDeflect*CurRadius-
						sqrt((FixDotX-X2)*(FixDotX-X2)+(FixDotY-Y2)*(FixDotY-Y2)))<0.00000001)
					{
						ComCenX = X2;
						ComCenY = Y2;
					}
					else return FALSE;
				}
				//��ȡ����Բ�İ뾶;
				ComRadius = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+
					(FixDotY-ComCenY)*(FixDotY-ComCenY));
			}		
		}
		return TRUE;
	}
	//�����ǰԲԲ����;
	else if (Length < CurRadius-0.0001)
	{
		//ItemDeflect��ʾ����ʱ���ô�뾶Բ(1)���λ���С�뾶(-1)Բ�β���;
		//�����ڶ����λ��;
		//�жϵ��Ƿ����ڵ�ǰԲ��;
		double InterAngle,InterLen;
		InterLen = distfwj(CurCenX,CurCenY,MoveDotX,MoveDotY,&InterAngle);
		if (InterLen >= CurRadius) return FALSE;
		//������Բ���������������д��߷���;
		double MidDotX,MidDotY,ValueK;
		MidDotX = (FixDotX+MoveDotX)/2;
		MidDotY = (FixDotY+MoveDotY)/2;
		ValueK = GetValueKByTwoPoint(FixDotX,FixDotY,MoveDotX,MoveDotY);
		//����д��������ᴹֱ;
		if (fabs(ValueK)<0.00000001) ValueK = 1E50;
		else if (ValueK == 1E50) ValueK = 0;
		else ValueK = -1/ValueK;
		//����ʱͨ����֪����������֪Բ���е�Բ��뾶��Բ������;
		if (ValueK == 1E50)
		{
			//������Բ��Բ�����������Ϊ;
			ComCenX = MidDotX;
			//��������Բ��Բ������������;
			//��������ϵ������;
			double FacA,FacB,FacC;
			FacA = 4*((FixDotY-CurCenY)*(FixDotY-CurCenY)-CurRadius*CurRadius);
			FacB = 4*(CurCenY-FixDotY)*((ComCenX-FixDotX)*(ComCenX-FixDotX)-
				(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+
				FixDotY*FixDotY-CurCenY*CurCenY)+8*CurRadius*CurRadius*CurCenY;
			FacC = -4*CurRadius*CurRadius*((ComCenX-CurCenX)*(ComCenX-CurCenX)+CurCenY*CurCenY)
				+((ComCenX-FixDotX)*(ComCenX-FixDotX)-(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+
				FixDotY*FixDotY-CurCenY*CurCenY)*((ComCenX-FixDotX)*(ComCenX-FixDotX)
				-(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+FixDotY*FixDotY-CurCenY*CurCenY);
			//���Yֵ��С;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double Y1,Y2;
				Y1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				//�ж���ЧԲ��ֵ;
				double Radius1,Radius2;
				Radius1 = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y1)*(FixDotY-Y1));
				Radius2 = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y2)*(FixDotY-Y2));
				//��ƫ���в���;
				if (ItemDeflect == 1) 
				{
					if (Radius1>=Radius2)
					{
						ComCenY = Y1;
						ComRadius = Radius1;
					}
					else
					{
						ComCenY = Y2;
						ComRadius = Radius2;
					}
				}
				//��ƫ���в���;
				else if (ItemDeflect == -1) 
				{
					if (Radius1<Radius2)
					{
						ComCenY = Y1;
						ComRadius = Radius1;
					}
					else
					{
						ComCenY = Y2;
						ComRadius = Radius2;
					}
				}
				else return FALSE;
			}				
		}
		else
		{
			//��ʱ��֪Բ�ĺ�������������֮��Ĺ�ϵY=K(X-X1)+Y1;;
			//�������ϵ������;
			double FacA,FacB,FacC,FacD,FacE;
			FacE = 2*(ValueK*(CurCenY-FixDotY)+(CurCenX-FixDotX));
			FacD = FixDotX*FixDotX+(MidDotY-ValueK*MidDotX-FixDotY)*(MidDotY-ValueK*MidDotX-FixDotY)
				-(CurCenX*CurCenX+CurRadius*CurRadius+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY));

			FacA = 4*CurRadius*CurRadius*(1+ValueK*ValueK)-FacE*FacE;
			FacB = 8*CurRadius*CurRadius*(ValueK*(MidDotY-ValueK*MidDotX-CurCenY)-CurCenX)-2*FacD*FacE;
			FacC = 4*CurRadius*CurRadius*(CurCenX*CurCenX+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY))-FacD*FacD;
			//���Xֵ��С;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double X1,X2,Y1,Y2;
				X1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y1 = ValueK*(X1-MidDotX)+MidDotY;
				X2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				Y2 = ValueK*(X2-MidDotX)+MidDotY;
				//�ж���ЧԲ��ֵ;
				double Radius1,Radius2;
				Radius1 = sqrt((FixDotX-X1)*(FixDotX-X1)+(FixDotY-Y1)*(FixDotY-Y1));
				Radius2 = sqrt((FixDotX-X2)*(FixDotX-X2)+(FixDotY-Y2)*(FixDotY-Y2));
				//��ƫ���в���;
				//PreDeflect=ItemDeflect;
				if (ItemDeflect == 1) 
				{
					if (Radius1>=Radius2)
					{
						ComCenX = X1;
						ComCenY = Y1;
						ComRadius = Radius1;
					}
					else
					{
						ComCenX = X2;
						ComCenY = Y2;
						ComRadius = Radius2;
					}
				}
				//��ƫ���в���;
				else if (ItemDeflect == -1) 
				{
					if (Radius1<Radius2)
					{
						ComCenX = X1;
						ComCenY = Y1;
						ComRadius = Radius1;
					}
					else
					{
						ComCenX = X2;
						ComCenY = Y2;
						ComRadius = Radius2;
					}
				}
				else return FALSE;
			}				
		}
		return TRUE;
	}
	else 
	{
		double CirOrient;
		CirOrient = ConvertAngleToOrient(GetAngleOfLineAndXaxes(CurCenX,CurCenY,FixDotX,FixDotY))+PreDeflect*PI/2;
		ItemDeflect = GetDeflectOfDotToLine(FixDotX,FixDotY,CirOrient,MoveDotX,MoveDotY);
		ComRadius = GetRadiusByStartDotAndSelDot(FixDotX,FixDotY,CirOrient,ItemDeflect,MoveDotX,MoveDotY,ComCenX,ComCenY);
		return TRUE;
	}

}

void BlockComputerFun::ShowArcOnScreen(double CenX,double CenY,double ArcRadius,
									   double ItemDeflect,double ArcDotX1,double ArcDotY1,/*�е�*/
									   double ArcDotX2,double ArcDotY2/*���*/,AcDbObjectId &ArcId,AcDbArc * pArc)
{
	//��ȡԲ����������Ӧ���ķ���;
	double TangeAngle,MoveDotAngle;
	ads_point StartDot,EndDot;
	StartDot[X] = CenX;
	StartDot[Y] = CenY;
	EndDot[X] = ArcDotX1;
	EndDot[Y] = ArcDotY1;
	TangeAngle = ads_angle(StartDot,EndDot);
	EndDot[X] = ArcDotX2;
	EndDot[Y] = ArcDotY2;
	MoveDotAngle = ads_angle(StartDot,EndDot);
	//����Բ��;
	if (pArc == NULL)
	{
		pArc = new AcDbArc;
		ArcId = AddEntityToDbs(pArc);
	}
	//����Բ�����շ��Ǳ���;
	double StartAngle,EndAngle;
	//�����ǰԲ������ƫ��;
	if (ItemDeflect == 1)
	{
		EndAngle = TangeAngle;
		StartAngle = MoveDotAngle;
	}
	else 
	{
		EndAngle = MoveDotAngle;
		StartAngle = TangeAngle;
	}	
	//�޸�Բ��;
	//acdbOpenObject(pArc,ArcId,AcDb::kForWrite);
	if(acdbOpenObject(pArc,ArcId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"��ʵ��ʧ�ܣ�\n");
		return;
	}
	pArc->setCenter(AcGePoint3d(CenX,CenY,0.0));
	pArc->setRadius(ArcRadius);
	pArc->setStartAngle(StartAngle);
	pArc->setEndAngle(EndAngle);
	pArc->close();	
}
void BlockComputerFun::GetTangentCorOfTwoCircle(double CenX1,double CenY1,double CenRadius1,
												double CenX2,double CenY2,double CenRadius2,
												double &TanX,double &TanY)
{
	//������Բ��֮��ľ���;
	double Dist,Angle;
	Dist = distfwj(CenX1,CenY1,CenX2,CenY2,&Angle);
	Angle = angleX(AcGePoint3d(CenX1,CenY1,0.0),AcGePoint3d(CenX2,CenY2,0.0));
	//�����ǰ��Բ�����й�ϵ;
	if (fabs(Dist-CenRadius1-CenRadius2)<0.00000001)
	{
		TanX = CenX1+CenRadius1*sin(ConvertAngleToOrient(Angle));
		TanY = CenY1+CenRadius1*cos(ConvertAngleToOrient(Angle));
	}
	//2Բ����1Բ;
	else if (fabs(Dist+CenRadius1-CenRadius2)<0.00000001)
	{
		Angle+=PI;
		TanX = CenX1+CenRadius1*sin(ConvertAngleToOrient(Angle));
		TanY = CenY1+CenRadius1*cos(ConvertAngleToOrient(Angle));
	}
	//1Բ����2Բ;
	else
	{
		TanX = CenX1+CenRadius1*sin(ConvertAngleToOrient(Angle));
		TanY = CenY1+CenRadius1*cos(ConvertAngleToOrient(Angle));
	}
}
double BlockComputerFun::GetValueKByTwoPoint(double LineX1,double LineY1,double LineX2,double LineY2)
{
	//������ݲ�����;
	double DistX,DistY;
	DistX = LineX1 - LineX2;
	DistY = LineY1 - LineY2;
	//�ж�DistX�Ƿ�Ϊ0;
	if (fabs(DistX) < 0.00000001) return 1E50;
	else return DistY/DistX;
}
double BlockComputerFun::GetDistOfLineAndDot(double LineX1,double LineY1,double LineX2,
											 double LineY2,double DotX,double DotY)
{
	//��ȡֱ�ߵ�б��;
	double ValueK  = GetValueKByTwoPoint(LineX1,LineY1,LineX2,LineY2);
	//����㵽ֱ�ߵ�ʵ�ʾ���;
	if (ValueK == 1E50)
	{
		return DotX - LineX1;
	}
	else
	{
		return (ValueK*DotX-ValueK*LineX1+LineY1-DotY)
			/sqrt(1+ValueK*ValueK);
	}
}
int BlockComputerFun::GetDotAtWhichSide(double LineX1,double LineY1,double LineX2,
										double LineY2,double DotX,double DotY)
{
	//��ȡ�㵽ֱ�ߵľ���;
	double CurDist;
	CurDist = GetDistOfLineAndDot(LineX1,LineY1,LineX2,
		LineY2,DotX,DotY);
	//�������ֱ�ߵ������Ϸ�ʱ;
	if (CurDist < -0.00000001) return -1;
	//�������ֱ������;
	else if (fabs(CurDist) <= 0.00000001) return 0;
	else return 1;
}

int BlockComputerFun::DotAtMiddleOrOut(double LineX1,double LineY1,double LineX2,
									   double LineY2,double DotX,double DotY)
{
	//��ȡ��ǰֱ��ĳһ����Զ��;
	double InterX,InterY;
	double ValueK = GetValueKByTwoPoint(LineX1,LineY1,LineX2,LineY2);
	//��������Զ�������;
	if (ValueK == 1E50) 
	{
		InterX = LineX1;
		InterY = -9999999999;
	}
	else
	{
		InterX = -9999999999;
		InterY = ValueK*InterX-ValueK*LineX1+LineY1;
	}
	//����ѡ������ֱ���ϵ�ͶӰ��;
	double ProjX,ProjY;
	GetProjectDotOfLine(LineX1,LineY1,ValueK,DotX,DotY,ProjX,ProjY);
	//����ֱ���ϵ�һ��ڶ��㼰ѡ�����ͶӰ��������Զ��ľ���;
	double Dist1,Dist2,DistP,A;
	Dist1 = distfwj(InterX,InterY,LineX1,LineY1,&A);
	Dist2 = distfwj(InterX,InterY,LineX2,LineY2,&A);
	DistP = distfwj(InterX,InterY,ProjX,ProjY,&A);
	//���ͶӰ����ֱ�ߵ�һ�ڶ���֮��;
	if (((Dist1<=DistP)&&(DistP<=Dist2))||
		((Dist2<=DistP)&&(DistP<=Dist1)))
		return 1;
	//���ͶӰ����ֱ�ߵ�һ�ڶ���֮��;
	else return -1;
}

////����λ��ת���ɷ���;
double BlockComputerFun::ConvertOrientToAngle(double ValueOrient)
{
	ValueOrient = ValueOrient*180/PI;
	//��λ�ǵķ�Χ��:0~360��;
	//��������ķ�λ����0~90�ȼ�(�൱�ڷ��ǵĵ�һ����);
	if (ValueOrient>0 && ValueOrient<=90) 
		return (90-ValueOrient)*PI/180;
	//��������ķ�λ����90~360�ȼ�(�൱�ڷ��ǵĵ���,��,������);
	else 
		return (450-ValueOrient)*PI/180;
}

//����ĳһ��Ԫ������,�������Ԫ����һ�������;
int BlockComputerFun::ComputerCorOfAnyDotOnItem(RouteDot &SDot,RouteDot &EDot,double CirLen,
												double ValueA,double ERadius,int ItemType,int ItemDeflect)
{  
	//�����ǰ��Ԫ��ֱ����;
	if (ItemType == LineItem) 
	{
		EDot.DotX = SDot.DotX+CirLen*sin(SDot.OrientAngle);
		EDot.DotY = SDot.DotY+CirLen*cos(SDot.OrientAngle);
		EDot.OrientAngle = SDot.OrientAngle;
		EDot.CurRadius = 1E50;
		EDot.CurMiles = SDot.CurMiles+CirLen;

		return 1;
	}
	//�����ǰ��Ԫ��Բ��;
	else if (ItemType == CirOrArcItem)
	{
		////��㴦�İ뾶��ΪԲ���뾶;
		//����Բ�ĵ�����;
		double CenX,CenY;
		//��ȡ��㵽Բ�ĵķ�λ��;
		double InterOrient = SDot.OrientAngle + ItemDeflect*PI/2;
		if (InterOrient<DataPrecision) 
			InterOrient+=2*PI;
		CenX = SDot.DotX + SDot.CurRadius*sin(InterOrient);
		CenY = SDot.DotY + SDot.CurRadius*cos(InterOrient);
		//��ȡ�յ㴦�����귽λ��;
		EDot.OrientAngle = SDot.OrientAngle + ItemDeflect*CirLen/SDot.CurRadius;
		//��ȡԲ�ĵ��յ㷽��ķ�λ��;
		InterOrient = EDot.OrientAngle - ItemDeflect*PI/2;
		//�յ�뾶��Ϊ���뾶;
		EDot.CurRadius = SDot.CurRadius;
		//�����յ������;
		EDot.DotX = CenX + EDot.CurRadius*sin(InterOrient);
		EDot.DotY = CenY + EDot.CurRadius*cos(InterOrient);
		//�յ㴦�����Ϊ;
		EDot.CurMiles = SDot.CurMiles + CirLen;

		return 1;
	}
	else if (ItemType == SprialItem) 
	{
		int RadForward = GetRadiusForward(SDot.CurRadius,ERadius);
		if (RadForward == LargeToSmall) 
		{	
			//SDot.CurRadius = 1E50;
			//��ȡ���߳���;
			double RealLen;
			RealLen = ValueA*ValueA/ERadius;

			//��ǰָ�����ȴ�������ƫת��;
			double DeflectAngle;
			DeflectAngle = CirLen*CirLen/(2*ERadius*RealLen);
			//��������������;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
			//�������ߵ�Դ�����;
			double OrignX,OrignY,OrignOrient;
			OrignX = SDot.DotX;
			OrignY = SDot.DotY;
			OrignOrient = SDot.OrientAngle;

			//��ȡԴ�㴦�ķ���;
			double AngleOfOrign;
			AngleOfOrign = ConvertOrientToAngle(OrignOrient);
			EDot.OrientAngle = SDot.OrientAngle+ItemDeflect*DeflectAngle;
			//��ָ�����ȴ��������;
			EDot.DotX = OrignX+InterX*sin(AngleOfOrign)-ItemDeflect*InterY*cos(AngleOfOrign);
			EDot.DotY = OrignY+InterX*cos(AngleOfOrign)+ItemDeflect*InterY*sin(AngleOfOrign);
			//��ʼ������㴦�İ뾶;
			if (CirLen > 0) EDot.CurRadius = ValueA*ValueA/CirLen;
			else EDot.CurRadius = 1E50;
			EDot.CurMiles = SDot.CurMiles+CirLen;

			return 1;
			//////////////////////////////////////////////////////////////////////////
		}
		else if (RadForward == BigToSmall) 
		{
			//CirLen>=0,ValueA!=0;
			if (ValueA < DataPrecision)
				return -1;
			//��ȡԴ�����꼰��Ӧ���ķ���;
			//��ȡ��㵽Դ������߳�;
			double CurLength = ValueA*ValueA/SDot.CurRadius;
			//��ȡ��Դ�㵽����ƫ��;
			double DeflectAngle = CurLength/(2*SDot.CurRadius);
			//��ȡԴ�㴦�ķ�λ��;
			double OrignOrient = SDot.OrientAngle - ItemDeflect*DeflectAngle;			
			//��ȡԴ�㴦�ķ���;
			double OrignAngle = ConvertOrientToAngle(OrignOrient);
			//��ȡԴ�㴦������;
			double OrignX,OrignY;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CurLength);
			InterY = GetAbsYCorOfSprialCir(ValueA,CurLength);
			OrignX = SDot.DotX - InterX*sin(OrignAngle)+ItemDeflect*InterY*cos(OrignAngle);
			OrignY = SDot.DotY - InterX*cos(OrignAngle)-ItemDeflect*InterY*sin(OrignAngle);
			//��ǰ�뾶���ɴ�С;
			double ForLength = CurLength+CirLen;
			//��ȡָ�����ȴ����������;
			InterX = GetAbsXCorOfSprialCir(ValueA,ForLength);
			InterY = GetAbsYCorOfSprialCir(ValueA,ForLength);			
			//��ȡָ�����ȴ��ľ�������;
			EDot.DotX = OrignX+InterX*sin(OrignAngle)-ItemDeflect*InterY*cos(OrignAngle);
			EDot.DotY = OrignY+InterX*cos(OrignAngle)+ItemDeflect*InterY*sin(OrignAngle);
			//��ȡָ�����ȴ��İ뾶;
			EDot.CurRadius = ValueA*ValueA/ForLength;
			//��ȡָ�����ȴ��ķ�λ��;
			EDot.OrientAngle = OrignOrient+ItemDeflect*ForLength*ForLength/(2*ValueA*ValueA);
			//���󴦵����;
			EDot.CurMiles = SDot.CurMiles+CirLen;

			return 1;
		}
		else if (RadForward == SmallToBig) 
		{
			//��ǰValueA������0,CirLen���ڵ���0,Big<1E50;
			if (ValueA <DataPrecision) 
				return -1;
			//��ȡԴ������;
			double OrignX,OrignY,OrignOrient;
			//��ǰ����������Ϊ;
			double CurCirLen = ValueA*ValueA/SDot.CurRadius;
			//���ߵ�ƫת��Ϊ;
			double DivertAngle = CurCirLen/(2*SDot.CurRadius);
			//Դ������귽λ��Ϊ;
			OrignOrient = SDot.OrientAngle + ItemDeflect*DivertAngle;
			//��ȡ����ת����;
			double Angle = ConvertOrientToAngle(OrignOrient) - ItemDeflect*PI;
			double InterX = GetAbsXCorOfSprialCir(ValueA,CurCirLen);
			double InterY = GetAbsYCorOfSprialCir(ValueA,CurCirLen);
			//���Դ������;
			OrignX = SDot.DotX - InterX*sin(Angle)-ItemDeflect*InterY*cos(Angle);
			OrignY = SDot.DotY - InterX*cos(Angle)+ItemDeflect*InterY*sin(Angle);			
			//��ǰ������Ӧ�����߳�Ϊ;
			double CurLen = CurCirLen - CirLen;
			//��ȡ������ڻ������϶�Ӧ���������;
			InterX = GetAbsXCorOfSprialCir(ValueA,CurLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CurLen);			
			//��ȡ����������;
			EDot.DotX = OrignX+InterX*sin(Angle)+/*-*/ItemDeflect*InterY*cos(Angle);
			EDot.DotY = OrignY+InterX*cos(Angle)-/*+*/ItemDeflect*InterY*sin(Angle);
			//��ȡ����㴦�ķ�λ��,�뾶�����;
			EDot.CurRadius = ValueA*ValueA/CurLen;
			EDot.OrientAngle = SDot.OrientAngle + ItemDeflect*(ValueA*ValueA/(2*SDot.CurRadius*SDot.CurRadius)-
				ValueA*ValueA/(2*EDot.CurRadius*EDot.CurRadius));
			EDot.CurMiles  = SDot.CurMiles+CirLen;

			return 1;
		}
		else if (RadForward == SmallToLarge)
		{
			//��ǰValueA������0,CirLen���ڵ���0;
			if (ValueA < DataPrecision) 
				return FALSE;
			//��ȡԴ������;
			double OrignX,OrignY,OrignOrient;
			//��ǰ����������Ϊ;
			double CurCirLen = ValueA*ValueA/SDot.CurRadius;
			//���ߵ�ƫת��Ϊ;
			double DivertAngle = CurCirLen/(2*SDot.CurRadius);
			//Դ������귽λ��Ϊ;
			OrignOrient = SDot.OrientAngle + ItemDeflect*DivertAngle;
			//��ȡ����ת����;
			double Angle = ConvertOrientToAngle(OrignOrient) - ItemDeflect*PI;

			double InterX = GetAbsXCorOfSprialCir(ValueA,CurCirLen);
			double InterY = GetAbsYCorOfSprialCir(ValueA,CurCirLen);
			//���Դ������;
			OrignX = SDot.DotX - InterX*sin(Angle)-/*+*/ItemDeflect*InterY*cos(Angle);
			OrignY = SDot.DotY - InterX*cos(Angle)+/*-*/ItemDeflect*InterY*sin(Angle);
			//��ǰ������Ӧ�����߳�Ϊ;
			double CurLen = CurCirLen - CirLen;
			//��ȡ������ڻ������϶�Ӧ���������;
			InterX = GetAbsXCorOfSprialCir(ValueA,CurLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CurLen);			
			//��ȡ����������;
			EDot.DotX = OrignX+InterX*sin(Angle)+/*-*/ItemDeflect*InterY*cos(Angle);
			EDot.DotY = OrignY+InterX*cos(Angle)-/*+*/ItemDeflect*InterY*sin(Angle);
			//��ȡ����㴦�ķ�λ��,�뾶�����;
			if (CurLen < DataPrecision) 
				EDot.CurRadius = 1E50;
			else
				EDot.CurRadius = ValueA*ValueA/CurLen;
			if (EDot.CurRadius == 1E50) 
				EDot.OrientAngle = SDot.OrientAngle + ItemDeflect*(ValueA*ValueA/(2*SDot.CurRadius*SDot.CurRadius));
			else
				EDot.OrientAngle = SDot.OrientAngle + ItemDeflect*(ValueA*ValueA/(2*SDot.CurRadius*SDot.CurRadius)-
				ValueA*ValueA/(2*EDot.CurRadius*EDot.CurRadius));
			EDot.CurMiles  = SDot.CurMiles+CirLen;

			return 1;		
		}
		else 
			return -1;
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////
//��ȡn�׳�(Factorial)ֵ��С;
long  BlockComputerFun::GetValueOfFactorial(long ValueNum)
{
	//�жϵ�ǰ�׳��Ƿ���0!����ʽ;
	if (ValueNum == 0) return 1;
	//�����ֵ����;
	long ReturnValue = 1;
	//����ѭ����ȡn�׳�ֵ;
	for(long i=1;i<ValueNum+1;i++)
		ReturnValue = ReturnValue*i;
	//��������ֵ;
	return ReturnValue;
}
//��ȡn��m�η�ֵ��С;
double  BlockComputerFun::GetValueOfMonWithNumPower(double ValuePar,long NumPower)
{   
	//�����ֵ����;
	double BaseVar = 1.0;
	//����η�ֵNumPower����0; 
	if (NumPower>0)
	{				
		for (long i=1;i<=NumPower;i++)
			BaseVar = BaseVar*ValuePar;
	}
	//����η�ֵNumPower����0; 
	else if (NumPower == 0) return 1;
	//����η�ֵNumPowerС��0; 
	else
	{
		for (long i=1;i<=fabs(1.0*NumPower);i++)
			BaseVar = BaseVar*ValuePar;	
		BaseVar = 1/BaseVar;
	}
	//��������ֵ;
	return BaseVar;
}
//���ݴ���Ļ������������߳��Ȼ�ȡ�����������������ϵ�еĺ��;
double BlockComputerFun::GetAbsXCorOfSprialCir(double ValueA,double CirLength)
{
	//���AֵΪ0��;
	if(ValueA <= DataPrecision) return 0.0;
	//�����ֵ�������м���ӱ���;
	double BaseSum=0.0,InterimX=0.0;
	//����ѭ����ȡ���󾫶�ֵ;
	for(long n=0;n<6;n++)
	{
		InterimX = GetValueOfMonWithNumPower(-1.0,n+2)*
			GetValueOfMonWithNumPower(CirLength,4*n+1)/
			((4*n+1)*GetValueOfMonWithNumPower(2*ValueA*ValueA,2*n)*GetValueOfFactorial(2*n));
		BaseSum = BaseSum + InterimX;
	}
	//��������ֵ;
	return BaseSum;
}
//���ݴ���Ļ������������߳��Ȼ�ȡ�����������������ϵ�е��ݾ�;
double BlockComputerFun::GetAbsYCorOfSprialCir(double ValueA,double CirLength)
{
	//���AֵΪ0��;
	if(ValueA <= DataPrecision) return 0.0;
	//�����ֵ�������м���ӱ���;
	double BaseSum=0.0,InterimY=0.0;
	//����ѭ����ȡ���󾫶�ֵ;	
	for(long n=1;n<7;n++)
	{
		InterimY = GetValueOfMonWithNumPower(-1.0,n+1)*
			GetValueOfMonWithNumPower(CirLength,4*n-1)/
			((4*n-1)*GetValueOfMonWithNumPower(2*ValueA*ValueA,2*n-1)*
			GetValueOfFactorial(2*n-1));				
		BaseSum = BaseSum + InterimY;
	}
	//��������ֵ;	
	return BaseSum;		
}

//��������ֱ����X��ķ���;
double BlockComputerFun::GetAngleOfLineAndXaxes(double StartX,double StartY,double EndX,double EndY)
{
	//��������ads_point�����;
	ads_point SDot,EDot;
	//ָ��ֱ�ߵ����յ�;
	SDot[X]=StartX;
	SDot[Y]=StartY;
	EDot[X]=EndX;
	EDot[Y]=EndY;
	//����ֱ����X��ļн�;
	return ads_angle(SDot,EDot);
}

//������·��Ԫ��ĳ������귽λ�ǻ�ȡ��·����;
int BlockComputerFun::GetItemCurForward(double DotOrientAngle)
{
	//��������ĽǶȴ���2*PI;
	if (DotOrientAngle > 2*PI + DataPrecision) 
	{
		DotOrientAngle = DotOrientAngle - 2*PI;
	}
	//��������ĽǶ�С��0;
	if (DotOrientAngle < 0)
	{
		DotOrientAngle = DotOrientAngle + 2*PI;
	}
	//�б�������;
	if (fabs(DotOrientAngle-2*PI)<DataPrecision||
		fabs(DotOrientAngle)< DataPrecision) 
		return VerticalUp;	
	else if (fabs(DotOrientAngle-PI/2)<DataPrecision) 
		return HorizontalRight;	
	else if (fabs(DotOrientAngle-PI)<DataPrecision) 
		return VerticalDown;
	else if (fabs(DotOrientAngle-3*PI/2)<DataPrecision) 
		return HorizontalLeft;
	else if (DotOrientAngle>0 && DotOrientAngle<PI/2)
		return ToFirstArea;
	else if (DotOrientAngle>PI/2 && DotOrientAngle<PI)
		return ToSecondArea;
	else if (DotOrientAngle>PI && DotOrientAngle<3*PI/2)
		return ToThreeArea;
	else if (DotOrientAngle>3*PI/2 && DotOrientAngle<2*PI)
		return ToFourArea;
	else return 99999;
}
//�ɵ�������ֱ��(������)�Ĳ���,ȷ���¸���·��Ԫ��ƫ��;
int BlockComputerFun::GetDeflectOfDotToLine(double LineX,double LineY,double LineOrient,
											double SelDotX,double SelDotY)
{
	//����㵽ֱ�ߵľ������;
	double Distance = 0;
	//��ȡ����ֵ(��Distance>0ʱ,����ֱ�ߵ����²�,���������ϲ�);
	if (LineOrient == 0||LineOrient == PI||LineOrient == 2*PI) 
	{
		Distance = SelDotX-LineX;
	}
	else
	{
		//ֱ�ߵķ���Ϊ:K*(X-X1)+Y1-Y=0;
		double ValueK = tan(PI/2-LineOrient);
		Distance = (ValueK*(SelDotX-LineX)+LineY-SelDotY)/sqrt(ValueK*ValueK+1);
	}
	//��ȡֱ�ߵ���·����;
	int LineForward = GetItemCurForward(LineOrient);
	//�жϵ���ֱ�߻����ߵĲ�����ȷ���¸���·��Ԫ��ƫ��;
	if (Distance > DataPrecision) 
	{
		if (LineForward == VerticalUp||LineForward == ToFirstArea||
			LineForward == HorizontalRight||LineForward == ToSecondArea)  
			return DeflectRigth;
		else return DeflectLeft;
	}
	else if (Distance < DataPrecision) 
	{
		if (LineForward == VerticalUp||LineForward == ToFirstArea||
			LineForward == HorizontalRight||LineForward == ToSecondArea) 
			return DeflectLeft;
		else return DeflectRigth;
	}
	else return NoneDeflect;
}
//���㻺�����ŵĻ�������Aֵ(��ֱ��������);
double BlockComputerFun::GetValueAOfSprial(double StartX,double StartY,double StartOrient,double EndX,double EndY,
										   double &EndR,double &CriLength)
{
	//�����ǰ�Ǵ�ֱ����������������;
	//����Դ�����;
	double OrignX,OrignY,OrignOrient;
	//�������ߵİ뾶���Ʊ���;
	int RadForWard = LargeToSmall;
	//��ȡ���ߵ�ƫת;
	int ItemDeflect;
	ItemDeflect = GetDeflectOfDotToLine(StartX,StartY,StartOrient,EndX,EndY);
	//���巵�ص�Aֵ����;
	double ValueA;
	BOOL GetValueA;
	GetValueA = GetValueAOfSprial(StartX,StartY,1E50,StartOrient,EndX,EndY,
		EndR,OrignX,OrignY,OrignOrient,CriLength,ValueA,RadForWard,ItemDeflect);
	//OrignX,OrignY,OrignOrient=StartX,StartY,StartOrient;
	if (GetValueA) 
		return ValueA;
	else return 1E50;
}

//���㻺�����ŵĻ�������Aֵ(��Բ������������);
BOOL BlockComputerFun::GetValueAOfSprial(double StartX,double StartY,double StartR,double StartOrient,double EndX,double EndY,
										 double &EndR,double &OrignX,double &OrignY,double &OrignOrient,double &CirLength,double &ValueA,
										 int RadForWard,int Deflect)
{
	//�������ߴ�ֱ��������;
	if (RadForWard == LargeToSmall)
	{
		//��ǰֱ�ߵ�Ԫ���յ㼴Ϊ���ߵ����,
		//��LineEndDot=SprialStartDot;
		//���ߵ�ƫ����뾶���ƿ��Զ��б����;		
		//��ȡ���յ���������㴦�ķ�λ��;
		double ValueOrient=ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//�������յ㳤��;
		double Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));		
		//�������ߵ�����ƫ����ƫ��;	
		double DeflectAngle = 0;
		if (Deflect == 1) DeflectAngle = ValueOrient - StartOrient;
		else DeflectAngle = StartOrient - ValueOrient;
		//�ж�ƫ�ǵ���Ч��;
		if (DeflectAngle < 0) DeflectAngle+=2*PI;
		if (DeflectAngle > PI/6.0) return FALSE;
		if (DeflectAngle < DataPrecision) return FALSE;
		//���ҳ�Ϊ��ʼ����;
		double LengthS = Distance;
		ValueA = LengthS/sqrt(6*DeflectAngle);
		//���㵱ǰ���ݾ༰��ǰ�ҳ�;
		double AbsXCor = GetAbsYCorOfSprialCir(ValueA,LengthS);	
		double AbsYCor = GetAbsXCorOfSprialCir(ValueA,LengthS);
		double AbsDist= sqrt(AbsXCor*AbsXCor+AbsYCor*AbsYCor);	
		//��ȡ�����Ŀ�������Ч������Χ;
		double LengthE = LengthS;
		while (AbsDist < Distance) 
		{
			LengthS = LengthE;
			LengthE+=0.1;
			ValueA = LengthE/sqrt(6*DeflectAngle);
			AbsXCor = GetAbsYCorOfSprialCir(ValueA,LengthE);	
			AbsYCor = GetAbsXCorOfSprialCir(ValueA,LengthE);
			AbsDist = sqrt(AbsXCor*AbsXCor+AbsYCor*AbsYCor);
		}
		CirLength = (LengthS+LengthE)/2;
		ValueA = CirLength/sqrt(6*DeflectAngle);
		//���㵱ǰ���ݾ༰��ǰ�ҳ�;	
		AbsXCor = GetAbsXCorOfSprialCir(ValueA,CirLength);	
		AbsYCor = GetAbsYCorOfSprialCir(ValueA,CirLength);
		AbsDist = sqrt(AbsXCor*AbsXCor+AbsYCor*AbsYCor);
		//��ȡ�����Ŀ�������Ч������Χ;
		while (fabs(AbsDist - Distance)>DataPrecision) 
		{
			if (AbsDist > Distance) LengthE = CirLength;
			else LengthS = CirLength;
			CirLength = (LengthS+LengthE)/2;
			ValueA = CirLength/sqrt(6*DeflectAngle);
			//���㵱ǰ���ݾ༰��ǰ�ҳ�;
			AbsXCor = GetAbsXCorOfSprialCir(ValueA,CirLength);	
			AbsYCor = GetAbsYCorOfSprialCir(ValueA,CirLength);
			AbsDist = sqrt(AbsXCor*AbsXCor+AbsYCor*AbsYCor);
		}	
		OrignX = StartX;
		OrignY = StartY;
		OrignOrient = StartOrient;
		if (CirLength < DataPrecision)
		{
			EndR = 1E50;
		}
		else
			EndR = ValueA*ValueA/CirLength;	
		//�жϵ�ǰ�������ĺ����Լ�����������Ƿ���δ����֮ǰ;
		if (EndR < DataPrecision) 
		{
			return FALSE;
		}
		DeflectAngle = ValueA*ValueA/(2*EndR*EndR);
		if (DeflectAngle > PI/2) 
		{
			return FALSE;
		}
		//���ؼ���õ���Aֵ;
		return TRUE;	
	}
	else if (RadForWard == BigToSmall) 
	{
		//��ȡ���յ���������㴦�ķ�λ��;
		double ValueOrient;
		ValueOrient=ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//��ȡ�յ����ߵ�ƫת��;
		double DivertAngle = 0;	
		if (Deflect == 1) DivertAngle = ValueOrient - StartOrient;
		else DivertAngle =  StartOrient - ValueOrient;
		//�ж�ƫ�ǵ���Ч��;
		if (DivertAngle < 0) DivertAngle+=2*PI;
		if (DivertAngle > PI/3.0) return FALSE;
		if (DivertAngle < DataPrecision) return FALSE;	
		//�趨�������߻��������Ĵ��ڷ�Χ;
		double LimValueAS = 0,LimValueAE = 1000000;
		//���������������;
		double InterimA = (LimValueAS+LimValueAE)/2;
		//��ȡ��ǰʵ���ҳ�;
		double Distance;
		Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));
		//��ȡ��ǰ���ߵĳ���;	
		double InterimLen = sqrt(InterimA*InterimA*InterimA*InterimA/(StartR*StartR)
			+6*InterimA*InterimA*DivertAngle)-InterimA*InterimA/StartR;			
		//��ȡָ�������������;
		double InterSX,InterSY;	
		double ValueLen;
		ValueLen = InterimA*InterimA/StartR;
		InterSX = GetAbsXCorOfSprialCir(InterimA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(InterimA,ValueLen);
		//��ȡָ���յ���������;
		double InterEX,InterEY;
		ValueLen = InterimA*InterimA/StartR+InterimLen;
		InterEX = GetAbsXCorOfSprialCir(InterimA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(InterimA,ValueLen);
		//��ȡָ���յ�ļ��賤��;
		double AbsDis;
		AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));
		while (fabs(AbsDis-Distance)>DataPrecision)
		{
			//����ٶ��ҳ�̫��;
			if (AbsDis>Distance) LimValueAE = InterimA;
			else LimValueAS = InterimA;
			//������ز���;
			InterimA = (LimValueAS+LimValueAE)/2;
			InterimLen = sqrt(InterimA*InterimA*InterimA*InterimA/(StartR*StartR)
				+6*InterimA*InterimA*DivertAngle)-InterimA*InterimA/StartR;
			//��ȡָ�������������;
			InterSX = GetAbsXCorOfSprialCir(InterimA,InterimA*InterimA/StartR);
			InterSY = GetAbsYCorOfSprialCir(InterimA,InterimA*InterimA/StartR);
			//��ȡָ���յ���������;
			InterEX = GetAbsXCorOfSprialCir(InterimA,(InterimA*InterimA/StartR+InterimLen));
			InterEY = GetAbsYCorOfSprialCir(InterimA,(InterimA*InterimA/StartR+InterimLen));
			//��ȡָ���յ�ļ��賤��;
			AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));			
		}
		//������������;
		EndR = InterimA*InterimA/(InterimA*InterimA/StartR+InterimLen);
		CirLength = sqrt(InterimA*InterimA*InterimA*InterimA/(StartR*StartR)
			+6*InterimA*InterimA*DivertAngle)-InterimA*InterimA/StartR;
		//�жϵ�ǰ�������ĺ����Լ�����������Ƿ���δ����֮ǰ;
		if (EndR < DataPrecision) 
		{
			return FALSE;
		}
		DivertAngle = ValueA*ValueA/(2*EndR*EndR);
		if (DivertAngle > PI/2) 
		{
			return FALSE;
		}
		//����������Aֵ;
		ValueA = InterimA;
		//����Դ������;
		GetOrignCorOfSprial(StartY,StartX,StartR,StartOrient,ValueA,Deflect,RadForWard,OrignY,OrignX,OrignOrient);
		return TRUE;

	}
	else if (RadForWard == SmallToBig) 
	{
		if (StartR < DataPrecision) 
		{
			return FALSE;
		}
		//��ȡ���յ���������㴦�ķ�λ��;
		double ValueOrient;
		ValueOrient=ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//��ȡ�յ����ߵ�ƫת��;
		double DivertAngle = 0;
		if (Deflect == 1) DivertAngle = ValueOrient - StartOrient;
		else DivertAngle = StartOrient - ValueOrient;
		//�ж�ƫ�ǵ���Ч��;
		if (DivertAngle < 0) DivertAngle+=2*PI;
		if (DivertAngle > PI/3.0) return FALSE;
		if (DivertAngle < DataPrecision) return FALSE;	
		//�趨���߳��ȵĴ��ڷ�Χ;
		//���ں���Ҫ��֤����������,��lֵ��ʼ��Ҫ��;
		double LimValueAS = sqrt(3*DivertAngle*StartR*StartR)+0.001;		
		double LimValueAE = GetValidLimValueAEnd(DivertAngle,StartR);
		//��ȡ��ǰʵ���ҳ�;
		double Distance;
		Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));
		//��������ЧA��Χ�ڼ����AbsDis����������֪��,
		//�����ǰʵ�ʾ������С��ǰAֵ�޽�;
		//////////////////////////////////////////////////////////////////////////
		ValueA =  LimValueAS;
		//��ȡָ�������������;
		double InterSX,InterSY;	
		double ValueLen;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ���������;
		double InterEX,InterEY;
		double InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);
		ValueLen = ValueA*ValueA/StartR-InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ�ļ��賤��;
		double AbsDisE;
		AbsDisE = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));		
		//��������������ʵ�ʾ�С��ǰ�޽�;
		if (AbsDisE < Distance) 
			return FALSE;
		//////////////////////////////////////////////////////////////////////////
		ValueA =  LimValueAE;
		//��ȡָ�������������;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ���������;
		InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);
		ValueLen = ValueA*ValueA/StartR-InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ�ļ��賤��;
		double AbsDisS;		
		AbsDisS = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));
		//�����С���������ʵ�ʾ����ǰ�޽�;
		if (AbsDisS > Distance) 
			return FALSE;				
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��ǰ���ߵĻ�������ֵ;
		ValueA = (LimValueAS+LimValueAE)/2;
		//���㵱ǰ����;
		InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);		
		//��ȡָ�������������;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ���������;
		ValueLen = ValueA*ValueA/StartR-InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ�ļ��賤��;
		double AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));
		while (fabs(AbsDis-Distance)>DataPrecision)
		{
			//����ٶ��ҳ�̫��;
			if (AbsDis>Distance) LimValueAS =ValueA;
			else LimValueAE = ValueA;
			//������ز���;
			ValueA = (LimValueAS+LimValueAE)/2;
			InterimLen = ValueA*ValueA/StartR-
				sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);					
			//��ȡָ�������������;
			ValueLen = ValueA*ValueA/StartR;
			InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
			InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
			//��ȡָ���յ���������;
			ValueLen = ValueA*ValueA/StartR - InterimLen;
			InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
			InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
			//��ȡָ���յ�ļ��賤��;
			AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));									
		}
		//������������;
		EndR = ValueA*ValueA/(ValueA*ValueA/StartR-InterimLen);		
		CirLength = InterimLen;
		//�жϵ�ǰ�������ĺ����Լ�����������Ƿ���δ����֮ǰ;
		DivertAngle = ValueA*ValueA/(2*StartR*StartR);
		if (DivertAngle > PI/2)
		{
			return FALSE;
		}
		//���㷵�ص�Դ������;
		GetOrignCorOfSprial(StartY,StartX,StartR,StartOrient,ValueA,Deflect,RadForWard,OrignY,OrignX,OrignOrient);

		return TRUE;
	}
	else if (RadForWard == SmallToLarge)
	{
		//��ȡ���յ���������㴦�ķ�λ��;
		double ValueOrient;
		ValueOrient = ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//��ȡ�յ����ߵ�ƫת��;
		double DivertAngle = 0;
		if (Deflect == 1) DivertAngle = ValueOrient - StartOrient;
		else DivertAngle =  StartOrient - ValueOrient;
		//�ж�ƫ�ǵ���Ч��;
		if (DivertAngle < 0) DivertAngle += 2*PI;
		if (DivertAngle > PI/3.0) return FALSE;
		if (DivertAngle < DataPrecision) return FALSE;
		//������㷽λ����֪,���յ�ͷ�λ�ǿ�֪;
		//�ڵ�ǰ�յ㼴ΪԴ��;
		OrignOrient = StartOrient + Deflect*3*DivertAngle/2;
		//��ȡ��ǰ�յ������;
		OrignX = EndX;
		OrignY = EndY;
		//��ȡ��ǰ��㵽Դ�����ߵ��ݾ�;
		double Distance;
		Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));
		//���������غ�,���ϲ�����Ԫ������;
		if (Distance < 0.1) return FALSE;
		//����Aֵ�ĵ���������ֵ;
		double LimAS = 0,LimAE = 1000000;
		//////////////////////////////////////////////////////////////////////////
		//��ȡ��ЧAֵ����;
		LimAS = sqrt(Distance*StartR)+0.001;
		//////////////////////////////////////////////////////////////////////////
		ValueA = (LimAS+LimAE)/2;
		//��ȡ���߳���;
		CirLength =ValueA*ValueA/StartR;
		//�����ʱ�ٶ���Aֵʱ�յ㵽ֱ�ߵľ���;
		double InterX = GetAbsXCorOfSprialCir(ValueA,CirLength);
		double InterY = GetAbsYCorOfSprialCir(ValueA,CirLength);
		//����ٶ�����;
		double AbsDis = sqrt(InterX*InterX+InterY*InterY);
		//��������ݾ���ʵ���ݾ��С����(1mm)ʱ;
		while (fabs(AbsDis-Distance) > 0.001)
		{
			//���Aֵ����;
			if (AbsDis>Distance) LimAE = ValueA;
			else LimAS = ValueA;
			//���¼���A��L�Ի�ȡ�µ��ݾ�;			
			ValueA = (LimAS+LimAE)/2;
			CirLength =ValueA*ValueA/StartR;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLength);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLength);
			//����ٶ�����;
			AbsDis = sqrt(InterX*InterX+InterY*InterY);
		}
		//�жϵ�ǰ�������ĺ�����;
		if (ValueA < DataPrecision) 
		{
			return FALSE;
		}
		DivertAngle = CirLength*CirLength/(2*ValueA*ValueA);
		if (DivertAngle > PI/2) 
		{
			return FALSE;
		}
		EndR = 1E50;
		//���㷵�ص�Դ������;
		GetOrignCorOfSprial(StartY,StartX,StartR,StartOrient,ValueA,Deflect,RadForWard,OrignY,OrignX,OrignOrient);
		return TRUE;
	}
	else 
		return FALSE;
}
//��ȡ�����ߵ�Դ������;
void BlockComputerFun::GetOrignCorOfSprial(double SprX,double SprY,double SprRad,
										   double SprOrient,double ValueA,int Deflect,int RadForward,
										   double &OrignX,double &OrignY,double &OrignOrient)
{
	//�����ǰ��������LargeToSmall��BigToSmall;
	if (RadForward==LargeToSmall || RadForward==BigToSmall) 
	{
		//�����ǰ�����㼴ΪԴ��;
		if (SprRad ==1E50)
		{
			OrignX = SprX;
			OrignY = SprY;
			OrignOrient = SprOrient;
		}
		else
		{		
			//������������;
			double CirLen = ValueA*ValueA/SprRad;
			//��ȡ����Դ�����ƫ��;
			double DivAngle = CirLen/(2*SprRad);
			//��ȡԴ��ķ�λ��;
			OrignOrient = SprOrient - Deflect*DivAngle;
			//��ȡԴ�������ת����;
			double Angle = ConvertOrientToAngle(OrignOrient);
			//��ø�������������;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
			//����Դ������;
			OrignX = SprX - InterX*sin(Angle)+Deflect*InterY*cos(Angle);
			OrignY = SprY - InterX*cos(Angle)-Deflect*InterY*sin(Angle);
		}
	}
	//�����ǰ��������SmallToLarge��SmallToBig;
	else 
	{
		if (SprRad ==1E50)
		{
			OrignX = SprX;
			OrignY = SprY;
			OrignOrient = SprOrient;
		}
		else
		{		
			//������������;
			double CirLen = ValueA*ValueA/SprRad;
			//��ȡ����Դ�����ƫ��;
			double DivAngle = CirLen/(2*SprRad);
			//��ȡԴ��ķ�λ��;
			OrignOrient = SprOrient + Deflect*DivAngle;
			//��ȡԴ�������ת����;
			double Angle = ConvertOrientToAngle(OrignOrient)- Deflect*PI;
			//��ø�������������;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
			//����Դ������;
			OrignX = SprX - InterX*sin(Angle)-/*+*/Deflect*InterY*cos(Angle);
			OrignY = SprY - InterX*cos(Angle)+/*-*/Deflect*InterY*sin(Angle);
		}		
	}

	return ;
}
int BlockComputerFun::GetRadiusForward(double StartR,double EndR)
{
	//�����ǰ����Ϊֱ��;
	if ((StartR==EndR)&&(StartR == 1E50)) 
		return NoChange;
	//�����ǰ�Ǵ�ֱ������������������;
	else if ((StartR==1E50)&&(EndR<1E50))
		return LargeToSmall;
	//�����ǰ�Ǵ�����������������������;
	else if ((StartR<1E50)&&(StartR>EndR))
		return BigToSmall;
	//�����ǰ����ΪԲ��;
	else if (StartR==EndR)/*R*/ 
		return NoChange;
	//�����ǰ�Ǵ�����������������������;
	else if ((StartR<EndR)&&(EndR<1E50))
		return SmallToBig;
	//�����ǰ�Ǵ�����������������ֱ��;
	else if ((StartR<1E50)&&(EndR==1E50))	
		return SmallToLarge;
	//�����ǰ����Ϊֱ��;
	else return NoChange;
}

void BlockComputerFun::ShowSpiralCirOnScreen(double StartX,double StartY,double StartR,double StartOrient,
											 double EndR,int ItemDeflect,double ValueA,double CirLen,
											 AcDbObjectId & CreateEntId)
{		
	//����һ���������鼰��ʱ�����;
	AcGePoint2dArray VerticsArray;
	AcGePoint2d Dot;
	//��ն�������;
	int ArraySize = VerticsArray.length();
	if (ArraySize > 0)
		VerticsArray.removeSubArray(0,ArraySize-1);
	//���������ߵĶ�����뵽����������;
	//����ѭ���ṹ��ʼ����������;
	//����������ʱ����;
	double InterLen;
	//���建������������;
	RouteDot SDot;
	SDot.DotX = StartX;
	SDot.DotY = StartY;
	SDot.OrientAngle = StartOrient;
	SDot.CurMiles = 0;//�����ڶ�̬��ƻ���ʱû������;
	SDot.CurRadius = StartR;
	//������ʱ������;
	RouteDot InterDot;
	//���ò���Ϊ5��;
	for(InterLen =0;InterLen<CirLen;InterLen+=2.0)
	{
		ComputerCorOfAnyDotOnItem(SDot,InterDot,InterLen,ValueA,
			EndR,SprialItem,ItemDeflect);
		Dot.set(InterDot.DotY,InterDot.DotX);
		VerticsArray.append(Dot);
	}
	//�����������һ������������;	
	ComputerCorOfAnyDotOnItem(SDot,InterDot,CirLen,ValueA,
		EndR,SprialItem,ItemDeflect);
	Dot.set(InterDot.DotY,InterDot.DotX);
	VerticsArray.append(Dot);
	//����ⲿ���������ʵ�岻����;
	if (CreateEntId == NULL)
	{
		//�����µĶ�����ʵ��;
		AcDbPolyline * pPolyline = new AcDbPolyline();
		//�����㼯�ĵ����Ϊ�����ߵĶ���;
		ArraySize = VerticsArray.length();
		for(int i=0;i<ArraySize;i++)
		{
			pPolyline->addVertexAt(i,VerticsArray[i]);
		}
		//���ö�������ֵ;
		pPolyline->setColorIndex(1);		
		//����������ӵ����ݿ���;
		CreateEntId = AddEntityToDbs(pPolyline);
	}
	else
	{
		AcDbEntity * pEntity = NULL;
		//��д��ʽ��ʵ��
		/* acdbOpenAcDbEntity(pEntity,CreateEntId,AcDb::kForWrite);*/
		if(acdbOpenObject(pEntity,CreateEntId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"��ʵ��ʧ�ܣ�\n");
			return;
		}
		//��ȡʵ��ָ���޸Ķ��㼯;
		if(pEntity->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pPolyline;
			pPolyline=AcDbPolyline::cast(pEntity);
			//ɾ��ԭ�������еĶ�������;
			int VertCount = pPolyline->numVerts();
			for(int i=0; i<VertCount;i++)
			{
				pPolyline->removeVertexAt(0);
			}
			//�������ݼ��뵽��������;
			ArraySize = VerticsArray.length();
			for(int  i=0;i<ArraySize;i++)
			{
				pPolyline->addVertexAt(i,VerticsArray[i]);	
			}
			//���ö�������ֵ;
			pPolyline->setColorIndex(1);
			//ɾ���ߴ�������;
			pPolyline->removeVertexAt(ArraySize);
			//�رն�����ʵ��;	
			pPolyline->close();
		}		
	}
}
Adesk::Boolean  BlockComputerFun::DrawSprialCirInWorldDraw
	(AcGiWorldDraw* pWd,double INDATA[5],double LO,xlpoint *P0,xlpoint *PZ,int icolor)
{

	RouteDot SDot,EDot;
	double CirLen,ValueA,ERadius;
	int ItemType,ItemDeflect;
	SDot.DotX=P0->x;
	SDot.DotY=P0->y;
	SDot.OrientAngle=P0->a;
	//SDot.CurRadius = INDATA[2];
	SDot.CurMiles = P0->lc;

	CirLen = LO;
	ValueA = INDATA[1];
	if (INDATA[0] == 3) 
	{
		SDot.CurRadius = P0->r;
		ERadius = INDATA[2];
	}
	else if (INDATA[0] == 5||INDATA[0] == 6) 
	{
		SDot.CurRadius = INDATA[2];
		ERadius = INDATA[3];
	}
	else if (INDATA[0] == 4) 
	{
		SDot.CurRadius = INDATA[2];
		ERadius = 1E50;
	}
	ItemType = SprialItem;
	ItemDeflect = INDATA[4];
	//�����������;
	int DotCount = 0;
	//���㰴5�ײ���,��ǰ���ߵĵ���;
	if (fmod(CirLen,2.0)<DataPrecision)
		DotCount = int(CirLen/2.0);
	else DotCount = int(CirLen/2.0)+1;
	//�������ͼ��ʱ�Ĵ�ֵ������;	
	AcGePoint3d * Point3dArr = new AcGePoint3d[DotCount];
	//����ѭ���ṹ��ʼ��������;
	int i=0;
	for (double InterLen=0;InterLen<=CirLen;InterLen+=2.0)
	{    
		//�����ǰ���㳤�ȴ���ʵ�ʳ���;
		/*
		if (InterLen>CirLen) 
		InterLen=CirLen;*/

		ComputerCorOfAnyDotOnItem(SDot,EDot,
			InterLen,ValueA,ERadius,SprialItem,ItemDeflect);
		Point3dArr[i].set(EDot.DotY,EDot.DotX,0.0);
		i+=1;
	}
	ComputerCorOfAnyDotOnItem(SDot,EDot,
		CirLen,ValueA,ERadius,SprialItem,ItemDeflect);
	Point3dArr[i].set(EDot.DotY,EDot.DotX,0.0);

	//���û����ߵ�ɫ��;
	if(icolor<0)
		pWd->subEntityTraits().setColor(5);	
	else 
		pWd->subEntityTraits().setColor(icolor);
	//���û���ͼ�κ������ƻ�����;
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
	pWd->geometry().polyline(DotCount,Point3dArr);

	return  0;
}
double BlockComputerFun::GetValidLimValueAEnd(double DivertAngle,double StartR)
{
	//��ʽֻ��Ӧ������Aʱ��SmallToBig;
	//������֪����,��ȡ���������ĵ�����ֵ(��֤���������);
	double LimValueAS = sqrt(3*DivertAngle*StartR*StartR)+0.001;
	//���û�������������ֵ;
	double LimValueAE = 1000000;
	//����AbsDis��L��A�Ĺ�ϵ����֪��AbsDis������ֵʱ��Lֵλ������֮��;
	double ValueA = (LimValueAS+LimValueAE)/2;
	//���㵱ǰ����ֵ;
	double InterimLen = ValueA*ValueA/StartR-
		sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);;	
	//��ȡָ�������������;
	double ValueLen = ValueA*ValueA/StartR;
	double InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
	double InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
	//��ȡָ���յ���������;
	ValueLen = ValueA*ValueA/StartR - InterimLen;
	double InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
	double InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
	//��ȡ����ļ��賤��;	
	double AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));			
	//�����������;
	double LenOfPre = 1E50;
	while (fabs(AbsDis-LenOfPre)>DataPrecision) 
	{
		//��ȡ�ϴε�����;
		LenOfPre = AbsDis;
		//��������������;
		if (AbsDis>InterimLen)
			LimValueAE = ValueA;
		else
			LimValueAS = ValueA;
		//������ز���;
		ValueA = (LimValueAS+LimValueAE)/2;
		InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);			
		//��ȡָ�������������;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡָ���յ���������;
		ValueLen = ValueA*ValueA/StartR - InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//��ȡ����ļ��賤��;	
		AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));			
	}
	return ValueA;
}

//����ǰһ��ԪΪ������,
//�����ΪԲ����ʱ��Բ��Բ�ĵ㼰Բ�뻺���е�����;
double BlockComputerFun::GetCenCorAndSprialDotCor(double SEast,double SNorth,double SOrientAngle,
												  double StartR,double EndR,double ValueA,double ItemDeflect,double CirDeflect,
												  double SelDotX,double SelDotY,double &CenX,double &CenY,double &SprX,double &SprY)
{
	//�����SEast(Y),SNorth(X),SOrientAngle,StartR,EndR�Ⱦ�ȡ�Ե�Ԫ;
	//��ȡ��ǰ�����İ뾶�仯����;
	int RadForward = GetRadiusForward(StartR,EndR);
	//�б�ǰ��������,�Ա�ȷ���뾶�ķ�Χ;
	double LimRadS = 0,LimRadE = 0,ReturnRad = 0;
	//�����ǰ����ΪLargeToSmall;
	if (RadForward == LargeToSmall)
	{
		LimRadS = 0+0.001;
		LimRadE = 1E6;
	}
	else if (RadForward == BigToSmall)
	{
		LimRadS = 0+0.001;
		LimRadE = StartR;
	}
	//�����ǰ��������ΪSmallToBig����SmallToLarge;
	else 
	{
		LimRadS = StartR;
		LimRadE = 1E6;
	}
	//���������뾶�ķ�Χȷ����ǰ�Ƿ���ں��ʵ�Rֵ;
	ReturnRad = LimRadS;

	double Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
		StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
		SelDotX,SelDotY,CenX,CenY,SprX,SprY);

	//ads_printf(L"1:returr=%lf dist=%lf\n",ReturnRad,Dist);   
	//�����ǰ��ͨ����İ뾶�ȵ������޻�С;
	if (ReturnRad > Dist+0.001)
	{
		return 1E50;
	}
	ReturnRad = LimRadE;
	Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
		StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
		SelDotX,SelDotY,CenX,CenY,SprX,SprY);

	//ads_printf(L"2:returr=%lf dist=%lf\n",ReturnRad,Dist);   
	//�����ǰͨ����İ뾶�ȵ������޻���;
	if (ReturnRad < Dist-0.001)
	{
		return 1E50;
	}
	//������ʱ�뾶ֵΪ:
	ReturnRad = (LimRadS+LimRadE)/2;
	//�����ʱ��Բ�ĵ�ѡ�е�ľ���(��������);
	Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
		StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
		SelDotX,SelDotY,CenX,CenY,SprX,SprY);
	//�������õ��ĵ����뾶��ֵ���ھ���;
	while (fabs(Dist-ReturnRad) > DataPrecision)
	{
		//����ǰ뾶ȡ�ýϴ�;
		if (ReturnRad > Dist)
			LimRadE = ReturnRad;
		else LimRadS = ReturnRad;
		//�����������;
		ReturnRad = (LimRadS+LimRadE)/2;
		Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
			StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
			SelDotX,SelDotY,CenX,CenY,SprX,SprY);				
	}
	//��������õİ뾶ֵ;
	return ReturnRad;
}

double BlockComputerFun::GetDistOfCenterAndSelDot(double SEast,double SNorth,double SOrientAngle,
												  double StartR,double EndR,double ValueA,double ItemDeflect,double CirDeflect,double InputRad,
												  double SelDotX,double SelDotY,double &CenX,double &CenY,double &SprX,double &SprY)
{
	//�����SEast,SNorth,SOrientAngle,StartR,EndR�Ⱦ�ȡ�Ե�Ԫ;
	//����İ뾶ֵ����Ч;
	//��ȡ��ǰ�����İ뾶�仯����;
	int RadForward = GetRadiusForward(StartR,EndR);
	//���ݵ�ǰ�������������˻����ߵ�Դ������;
	double OrignX,OrignY,OrignOrient;
	GetOrignCorOfSprial(SEast,SNorth,StartR,
		SOrientAngle,ValueA,ItemDeflect,RadForward,
		OrignY,OrignX,OrignOrient);
	//��ʱ��õ�Դ������Ϊ�ѿ�������ϵ��ֵ(Բ�����е�ֵҲ��Ϊ��������ֵ);
	//�ڻ������϶�Ӧ�뾶���ķ�λ��Ϊ;
	double InterOrient = 0;
	double InterAngle = ValueA*ValueA/(2*InputRad*InputRad);
	if (RadForward == LargeToSmall||RadForward == BigToSmall)
	{
		InterOrient = OrignOrient + ItemDeflect*InterAngle;
	}
	else
	{
		InterOrient = OrignOrient - ItemDeflect*InterAngle;
	}
	//���㻺�����϶�Ӧ���ȴ�������;
	RouteDot StartDot,ReturnDot;
	StartDot.DotX = SEast;
	StartDot.DotY = SNorth;
	StartDot.CurRadius = StartR;
	StartDot.OrientAngle = SOrientAngle;
	StartDot.CurMiles = 0;
	//���㵱ǰ������㵽��Ӧ���ȵ������;
	double InterLen = 0;
	if (RadForward == LargeToSmall)		
		InterLen = ValueA*ValueA/InputRad;
	else if (RadForward == BigToSmall)
		InterLen = ValueA*ValueA/InputRad-ValueA*ValueA/StartR;
	else
		InterLen = ValueA*ValueA/StartR-ValueA*ValueA/InputRad;

	ComputerCorOfAnyDotOnItem(StartDot,ReturnDot,
		InterLen,ValueA,EndR,SprialItem,ItemDeflect);
	SprX = ReturnDot.DotY;
	SprY = ReturnDot.DotX;	
	//ads_printf(L"#### %lf %lf ir=%lf sr=%lf eR=%lf a=%lf l=%lf\n",SprX,SprY,InputRad,StartR,EndR,ValueA,InterLen);
	//��ȡԲ��ƫ��;
	//int CircleDef = GetDeflectOfDotToLine(SprX,SprY,InterOrient,SelDotX,SelDotY);
	//���㵱ǰ�������϶�Ӧ���ȴ�����ȷ����Բ������;
	//�㵽Բ�ĵķ�λ��;
	double OrientOfDotAndCen = InterOrient + CirDeflect*PI/2;
	//�жϷ�λ�ǵ���Ч��;
	if (OrientOfDotAndCen > 2*PI) OrientOfDotAndCen = OrientOfDotAndCen - 2*PI;
	if (OrientOfDotAndCen < 0) OrientOfDotAndCen = OrientOfDotAndCen + 2*PI;
	CenX = SprX + InputRad*sin(OrientOfDotAndCen);
	CenY = SprY + InputRad*cos(OrientOfDotAndCen);
	//���㵱ǰԲ�ĵ���ѡ�е�ľ���;
	double Dist = sqrt((CenX-SelDotX)*(CenX-SelDotX)+(CenY-SelDotY)*(CenY-SelDotY));

	return Dist;
}

double BlockComputerFun::GetRadiusByStartDotAndSelDot(double StartX,double StartY,double StartOrient,double CurDeflect,
													  double SelDotX,double SelDotY,double &GetCenX,double &GetCenY)
{
	//���ݵ�ǰ��Ҫ����Բ�Ļ�ƫ���ȡ��㵽Բ�ĵķ�λ��;
	double OrientOfSC;
	OrientOfSC = StartOrient + CurDeflect*PI/2 +2*PI;
	//��ȡ�����ع���Ĵ�ֱƽ���߲���;
	double MidX,MidY,OrientOfMid;
	MidX = (StartX+SelDotX)/2;
	MidY = (StartY+SelDotY)/2;
	double AngleOfSS = GetAngleOfLineAndXaxes(StartX,StartY,SelDotX,SelDotY);
	OrientOfMid = ConvertAngleToOrient(AngleOfSS) + CurDeflect*PI/2 +2*PI;
	//�����������������λ��,��ȡ��Ӧ��ֱ��ϵ��б��;
	double ValueKSC,ValueKMid;
	ValueKSC = GetValueKByOrient(OrientOfSC*180/PI);
	ValueKMid = GetValueKByOrient(OrientOfMid*180/PI);
	//��ȡ��ֱ�ߵĽ��㼴Բ������;
	GetCrossDotOfLine(ValueKSC,StartX,StartY,ValueKMid,MidX,MidY,GetCenX,GetCenY);
	//����Բ�İ뾶;
	double Radius = sqrt((StartX-GetCenX)*(StartX-GetCenX)+(StartY-GetCenY)*(StartY-GetCenY));

	if (Radius < DataPrecision) return 1E50;
	else return Radius;
}
//����һ��������Բ�����ع�������Բ��ֱ����Բ��;
double BlockComputerFun::GetRadiusOfLineAndTwoDot(double LineX,double LineY,double LineOrient,double FixX,double FixY,
												  double MoveX,double MoveY,double &CenX,double &CenY,double &TanX,double &TanY,BOOL BackOrnot)
{
	//���巵��ֵ;
	double ReturnRad = 0;
	//��ȡ���������ߴ��ߵ�б��;
	double ValueK,ValueVetK;
	ValueK = GetValueKByOrient(LineOrient*180/PI);
	if (ValueK == 0) ValueVetK = 1E50;
	else if (ValueK == 1E50) ValueVetK = 0;
	else ValueVetK = -1/ValueK;
	//�������ֱ�����̫��;
	double Dist1 = GetDistOfLineAndSelDot(ValueK,LineX,LineY,FixX,FixY);
	double Dist2 = GetDistOfLineAndSelDot(ValueK,LineX,LineY,MoveX,MoveY);
	if(fabs(Dist2)<DataPrecision&&fabs(Dist1)<DataPrecision) return 1E50;
	//�����������;
	if (sqrt((FixX-MoveX)*(FixX-MoveX)+(FixY-MoveY)*(FixY-MoveY))<DataPrecision) 
		return 1E50;
	//�ж�Բ��ƫ��;
	int CirDeflect = GetDeflectOfDotToLine(LineX,LineY,LineOrient,FixX,FixY);
	int Deflect = GetDeflectOfDotToLine(LineX,LineY,LineOrient,MoveX,MoveY);
	if (fabs(Dist1)<DataPrecision)
	{
		TanX = FixX;
		TanY = FixY;
		ReturnRad = GetRadiusByStartDotAndSelDot(FixX,FixY,LineOrient,Deflect,MoveX,MoveY,CenX,CenY);		
		return ReturnRad;
	}
	//������㲻����,��ǰ���費�ܳɹ�;
	if (CirDeflect != Deflect) return 1E50;
	//��ȡ�Ҵ�ֱƽ���ߵ�Ҫ��;
	double MidX,MidY,MidOrient,ValueMidK;
	MidX = (FixX+MoveX)/2;
	MidY = (FixY+MoveY)/2;
	MidOrient = ConvertAngleToOrient(
		GetAngleOfLineAndXaxes(FixX,FixY,MoveX,MoveY)) + Deflect*PI/2;
	ValueMidK = GetValueKByOrient(MidOrient*180/PI);
	//����������Ҵ�ֱƽ����ƽ����;
	if (fabs(ValueK-ValueMidK)<DataPrecision) return 1E50;
	//�����������ƽ��ʱ,��ʱ�Ҵ�ֱƽ���߲������ƽ��;
	if (fabs(ValueK) <DataPrecision) 
	{
		//��ʱ�����TanXֵ��CenX���,CenY-TanY=Radius;
		//����Ҵ�ֱƽ��������ᴹֱ;
		if (ValueMidK == 1E50)
		{
			//TanX=MidX=CenX;
			CenX = MidX;
			CenY = ((CenX-FixX)*(CenX-FixX)/(FixY-LineY)+FixY+LineY)/2;
			ReturnRad = sqrt((CenX-FixX)*(CenX-FixX)+(CenY-FixY)*(CenY-FixY));
			TanX = MidX;
			TanY = LineY;

			return ReturnRad;
		}
		else
		{
			//TanY = LineY;
			//���巽��ϵ��;
			double FacA,FacB,FacC;
			FacA = 1;
			FacB = -2*(FixX+ValueMidK*(FixY-LineY));
			FacC = FixX*FixX+FixY*FixY-LineY*LineY-2*(MidY-ValueMidK*MidX)*(FixY-LineY);			
			double ValueBeita = FacB*FacB-4*FacA*FacC;
			//���ValueBeitaС��0�����޽�;
			double InterCenX1,InterCenY1,InterCenX2,InterCenY2,Radius1,Radius2;
			if (ValueBeita < 0) return 1E50;
			else
			{
				//�������Բ��ֵ;
				InterCenX1 = (-FacB+sqrt(ValueBeita))/(2*FacA);
				InterCenY1 = ValueMidK*(InterCenX1-MidX)+MidY;
				Radius1=sqrt((InterCenX1-FixX)*(InterCenX1-FixX)+(InterCenY1-FixY)*(InterCenY1-FixY));
				InterCenX2 = (-FacB-sqrt(ValueBeita))/(2*FacA);
				InterCenY2 = ValueMidK*(InterCenX2-MidX)+MidY;
				Radius2=sqrt((InterCenX2-FixX)*(InterCenX2-FixX)+(InterCenY2-FixY)*(InterCenY2-FixY));
				//�����ǰ���ô�뾶����Բ��;
				if (!BackOrnot)
				{
					if (Radius1>=Radius2) 
					{
						CenX = InterCenX1;
						CenY = InterCenY1;
						TanX = CenX;
						TanY = LineY;
						ReturnRad = Radius1;
					}
					else
					{
						CenX = InterCenX2;
						CenY = InterCenY2;
						TanX = CenX;
						TanY = LineY;
						ReturnRad = Radius2;
					}
				}
				else
				{
					if (Radius2>=Radius1) 
					{
						CenX = InterCenX1;
						CenY = InterCenY1;
						TanX = CenX;
						TanY = LineY;
						ReturnRad = Radius1;
					}
					else
					{
						CenX = InterCenX2;
						CenY = InterCenY2;
						TanX = CenX;
						TanY = LineY;
						ReturnRad = Radius2;
					}
				}
				return ReturnRad;
			}
		}
	}
	else if (ValueK == 1E50) 
	{
		//��ʱ�����TanYֵ��CenY���,CenX-TanX=Radius;
		//����Ҵ�ֱƽ���������ƽ��;
		if (fabs(ValueMidK) < DataPrecision)
		{
			//TanY=MidY=CenY;
			CenX = ((MidY-FixY)*(MidY-FixY)/(FixX-LineX)+FixX+LineX)/2;
			CenY = MidY;
			ReturnRad = sqrt((CenX-FixX)*(CenX-FixX)+(CenY-FixY)*(CenY-FixY));
			TanX = LineX;
			TanY = MidY;

			return ReturnRad;
		}
		else
		{
			//TanX = LineX;
			//���巽��ϵ��;
			double FacA,FacB,FacC;
			FacA = ValueMidK*ValueMidK;
			FacB = 2*(LineX-FixX+ValueMidK*(MidY-ValueMidK*MidX-FixY));
			FacC = FixX*FixX-LineX*LineX+(MidY-ValueMidK*MidX-FixY)*(MidY-ValueMidK*MidX-FixY);			
			double ValueBeita = FacB*FacB-4*FacA*FacC;
			//���ValueBeitaС��0�����޽�;
			double InterCenX1,InterCenY1,InterCenX2,InterCenY2,Radius1,Radius2;
			if (ValueBeita < 0) return 1E50;
			else
			{
				//�������Բ��ֵ;
				InterCenX1 = (-FacB+sqrt(ValueBeita))/(2*FacA);
				InterCenY1 = ValueMidK*(InterCenX1-MidX)+MidY;
				Radius1 = sqrt((InterCenX1-FixX)*(InterCenX1-FixX)+(InterCenY1-FixY)*(InterCenY1-FixY));
				InterCenX2 = (-FacB-sqrt(ValueBeita))/(2*FacA);
				InterCenY2 = ValueMidK*(InterCenX2-MidX)+MidY;
				Radius2 = sqrt((InterCenX2-FixX)*(InterCenX2-FixX)+(InterCenY2-FixY)*(InterCenY2-FixY));
				//�����ǰ���ô�뾶����Բ��;
				if (!BackOrnot)
				{
					if (Radius1>=Radius2) 
					{
						CenX = InterCenX1;
						CenY = InterCenY1;
						TanX = LineX;
						TanY = CenY;
						ReturnRad = Radius1;
					}
					else
					{
						CenX = InterCenX2;
						CenY = InterCenY2;
						TanX = LineX;
						TanY = CenY;
						ReturnRad = Radius2;
					}
				}
				else
				{
					if (Radius2>=Radius1) 
					{
						CenX = InterCenX1;
						CenY = InterCenY1;
						TanX = LineX;
						TanY = CenY;
						ReturnRad = Radius1;
					}
					else
					{
						CenX = InterCenX2;
						CenY = InterCenY2;
						TanX = LineX;
						TanY = CenY;
						ReturnRad = Radius2;
					}
				}
				return ReturnRad;

			}
			return 1E50;
		}		
	}
	else 
	{
		double CroTanX,CroTanY,CroMidX,CroMidY;
		//������������ֱ�ߵĽ���;
		double ValueKother = GetValueKByTwoPoint(FixX,FixY,MoveX,MoveY);
		GetCrossDotOfLine(ValueK,LineX,LineY,ValueKother,FixX,FixY,CroTanX,CroTanY);
		//��ȡ���д��������ߵĽ���;
		GetCrossDotOfLine(ValueK,LineX,LineY,ValueMidK,MidX,MidY,CroMidX,CroMidY);
		//���������ε����ߴ�С;
		double SideC = sqrt((CroTanX-CroMidX)*(CroTanX-CroMidX)+
			(CroTanY-CroMidY)*(CroTanY-CroMidY));
		double SideB = sqrt((MidX-CroMidX)*(MidX-CroMidX)+
			(MidY-CroMidY)*(MidY-CroMidY));
		double SideA = sqrt((MidX-CroTanX)*(MidX-CroTanX)+
			(MidY-CroTanY)*(MidY-CroTanY));
		double PartA = sqrt((MidX-FixX)*(MidX-FixX)+
			(MidY-FixY)*(MidY-FixY));
		//����Բ��Rֵ;
		if (SideC<DataPrecision||
			SideB<DataPrecision||
			SideA<DataPrecision||
			PartA<DataPrecision) return 1E50;
		double FacA,FacB,FacC;		
		FacA = SideC*SideC/(SideA*SideA)-1;
		FacB = -2*SideB*SideC/SideA;
		FacC = SideB*SideB+PartA*PartA;
		double ValueBerta = FacB*FacB-4*FacA*FacC;
		//���ValueBerta�����޽�;
		if (ValueBerta < DataPrecision) return 1E50;
		else
		{
			//�����д��㵽ֱ�ߵľ���;
			double DistOfVet = fabs(GetDistOfLineAndSelDot(ValueK,LineX,LineY,MidX,MidY));
			//�����ǰ���ô�뾶����Բ��;
			if (!BackOrnot)
			{
				ReturnRad = (-FacB+sqrt(ValueBerta))/(2*FacA);	
				//��ǰ�뾶����ʱ�п��޽�;
				if (ReturnRad < DistOfVet) return 1E50;
				int DotDeflect = GetDeflectOfDotToLine(FixX,FixY,LineOrient+CirDeflect*PI/2,MoveX,MoveY);
				if (CirDeflect == 1)
				{
					if (DotDeflect == -1)
					{			
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
					else
					{
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient-PI);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient-PI);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
				}
				else
				{
					if (DotDeflect == 1)
					{			
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
					else
					{
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient+PI);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient+PI);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
				}
			}			
			else
			{				
				ReturnRad = (-FacB-sqrt(ValueBerta))/(2*FacA);
				//��ǰ�뾶����ʱ�п��޽�;
				if (ReturnRad > DistOfVet) return 1E50;
				int DotDeflect = GetDeflectOfDotToLine(FixX,FixY,LineOrient+CirDeflect*PI/2,MoveX,MoveY);
				if (CirDeflect == 1)
				{
					if (DotDeflect == -1)
					{			
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient-PI);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient-PI);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
					else
					{
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
				}
				else
				{
					if (DotDeflect == 1)
					{			
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient+PI);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient+PI);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
					else
					{
						CenX = MidX+sqrt(ReturnRad*ReturnRad-PartA*PartA)*sin(MidOrient);
						CenY = MidY+sqrt(ReturnRad*ReturnRad-PartA*PartA)*cos(MidOrient);
						TanX = CenX-ReturnRad*sin(LineOrient+CirDeflect*PI/2);
						TanY = CenY-ReturnRad*cos(LineOrient+CirDeflect*PI/2);
					}
				}		
			}
		}
		return ReturnRad;
	}
}

void BlockComputerFun::ModifyStartDot(double &StartX,double &StartY,double ValueSide,double OffsetDist,
									  double &StartOrient,double ValueDef,double ValueAngle)
{

	//����ƫ����໹���Ҳ༰ƫ����㵱ǰ���λ��(ValueSide��Ϊ-1��Ϊ1);
	StartX = StartX + OffsetDist*sin(ValueSide*PI/2+StartOrient);
	StartY = StartY + OffsetDist*cos(ValueSide*PI/2+StartOrient);
	//�������߷�����ƫ������ƫ�������ߵķ�λ��;
	StartOrient = StartOrient + ValueDef*ValueAngle*PI/180;
}

void BlockComputerFun::BackTonotModifidOfSDot(double &StartX,double &StartY,double ValueSide,double OffsetDist,
											  double &StartOrient,double ValueDef,double ValueAngle)
{
	//�������߷�����ƫ������ƫ����ԭ���ߵķ�λ��;
	StartOrient = StartOrient - ValueDef*ValueAngle*PI/180;
	//����ƫ����໹���Ҳ༰ƫ�����ԭ���λ��(ValueSide��Ϊ-1��Ϊ1);
	StartX = StartX + OffsetDist*sin(-1*ValueSide*PI/2+StartOrient);
	StartY = StartY + OffsetDist*cos(-1*ValueSide*PI/2+StartOrient);	
}

BOOL BlockComputerFun::GetComItemFromSelEntity(AcDbObjectId SelEntId,struct RampItem & ComItem,
											   double CurDef,BOOL FreeOrnot)
{
	BOOL ContinueOrnot = FALSE;
	//����CAD�ĵ�;
	acedGetAcadDwgView()->SetFocus();
	if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		return FALSE;
	}
	//������ȡ��ʵ��;
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelEntId,AcDb::kForRead)!=Acad::eOk)
	{
		return FALSE;
	}
	if (pEntity == NULL) 
	{
		return FALSE;
	}
	//���ѡ�е���ֱ��;
	if (_tcscmp(pEntity->isA()->name(),L"AcDbLine")==0)
	{
		//��ȡֱ�ߵ����յ�;
		AcDbLine * pLine = AcDbLine::cast(pEntity);
		double LineSX,LineSY,LineEX,LineEY;
		LineSX= (pLine->startPoint())[X];
		LineSY= (pLine->startPoint())[Y];
		LineEX= (pLine->endPoint())[X];
		LineEY= (pLine->endPoint())[Y];
		pEntity->close();
		//��ʼ��������Ԫ;
		double PreItemDeflect = 1;
		if (fabs(CurDef) > DataPrecision) 
		{
			PreItemDeflect = CurDef;
		}
		//else
		//{		
		//��ʾ�û�ָ����ʱ��·������(ƫ��:˳ʱ��-�һ�����ʱ��-��)			
		//	if (acedGetReal(L"\n��ָ����·����(˳ʱ��-��1,��ʱ��-��-1):\n",&PreItemDeflect) == RTCAN)
		//	{				
		//		return FALSE;			
		//	}
		//}
		if ((int)PreItemDeflect == 1)
		{
			ContinueOrnot = TRUE;
		}
		if((int)PreItemDeflect == -1)
		{
			ContinueOrnot = TRUE;
		}
		if (!ContinueOrnot) 
		{
			return FALSE;
		}			
		if (fabs(PreItemDeflect-1)<DataPrecision) 
		{
			ComItem.FDotX = LineEX;
			ComItem.FDotY = LineEY;
			ComItem.FDotOrient = ConvertAngleToOrient
				(GetAngleOfLineAndXaxes(LineSX,LineSY,LineEX,LineEY));			
		}
		else
		{
			ComItem.FDotX = LineSX;
			ComItem.FDotY = LineSY;
			ComItem.FDotOrient = ConvertAngleToOrient
				(GetAngleOfLineAndXaxes(LineEX,LineEY,LineSX,LineSY));
		}							
		//��ȡֱ�ߵ���������;
		ComItem.ItemType = 1.0;             //��ͨCADֱ��;		
		ComItem.ItemDef = PreItemDeflect;   //NoneDeflect,�ʽ����¼����·����;	
	}
	else if (_tcscmp(pEntity->isA()->name(),L"AcDbArc")==0) 
	{
		//��ѡ�е�Բ��ʵ��,��ȡԲ����Ԫ�����Ԫ��;
		AcDbArc * pGotEntArc = AcDbArc::cast(pEntity); 
		XYarray xray;
		int res=GetXdataFromEntity(SelEntId,xray);
		if(res==0)//��ͨcadʵ��
		{
			if (FreeOrnot) //�������϶�
			{			
				//��ʾ�û�ָ����ʱ��·������(ƫ��:˳ʱ��-�һ�����ʱ��-��)
				double PreItemDeflect = 1;
				//	if (acedGetReal(L"\n��ָ����·����(˳ʱ��-��1,��ʱ��-��-1):\n",&PreItemDeflect) == RTCAN)
				//	{
				//		return FALSE;
				//	}
				if ((int)(PreItemDeflect+DataPrecision) == 1)
				{
					ContinueOrnot = TRUE;
				}
				if((int)(PreItemDeflect+DataPrecision) == -1)
				{
					ContinueOrnot = TRUE;
				}
				if (!ContinueOrnot) 
				{
					return FALSE;
				}	
				//��ȡԲ������;
				ComItem.FDotX = (pGotEntArc->center())[X];
				ComItem.FDotY = (pGotEntArc->center())[Y];					
				ComItem.ItemDef = PreItemDeflect;
			}
			else
			{
				//��ʾ�û�ָ����ʱ��·������(ƫ��:˳ʱ��-�һ�����ʱ��-��)
				if (fabs(CurDef) < DataPrecision) 
				{
					if (acedGetReal(L"\n��ָ����·����(˳ʱ��-��1,��ʱ��-��-1):\n",&CurDef) == RTCAN)
					{
						return FALSE;
					}
					if ((int)(CurDef+DataPrecision) == 1)
					{
						ContinueOrnot = TRUE;
					}
					if((int)(CurDef+DataPrecision) == 0)
					{
						ContinueOrnot = TRUE;
					}
					if (!ContinueOrnot) 
					{
						return FALSE;
					}	
				}
				if (fabs(CurDef+1)<DataPrecision)//��ʱ��
				{
					ComItem.FDotX = (pGotEntArc->center())[X] + pGotEntArc->radius()*sin(ConvertAngleToOrient(pGotEntArc->endAngle()));
					ComItem.FDotY = (pGotEntArc->center())[Y] + pGotEntArc->radius()*cos(ConvertAngleToOrient(pGotEntArc->endAngle()));				
					ComItem.FDotOrient = ConvertAngleToOrient(pGotEntArc->endAngle()+PI/2);
				}
				else//˳ʱ��
				{
					ComItem.FDotX = (pGotEntArc->center())[X] + pGotEntArc->radius()*sin(ConvertAngleToOrient(pGotEntArc->startAngle()));
					ComItem.FDotY = (pGotEntArc->center())[Y] + pGotEntArc->radius()*cos(ConvertAngleToOrient(pGotEntArc->startAngle()));				
					ComItem.FDotOrient = ConvertAngleToOrient(pGotEntArc->startAngle()-PI/2);				
				}
				ComItem.ItemDef = CurDef;
			}
			//��ȡ��Ԫ����;
			ComItem.ItemType = 2.0;                //��ͨCADԲ��;
			ComItem.ItemPro = pGotEntArc->radius();//R;
		}
		else
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////
			//��ȡ����ӦԲ����Ԫ��Բ������;
			ComItem.FDotX = xray.xyarr[9]+
				xray.xyarr[1]*sin(xray.xyarr[5]+xray.xyarr[4]*PI/2);
			ComItem.FDotY = xray.xyarr[8]+
				xray.xyarr[1]*cos(xray.xyarr[5]+xray.xyarr[4]*PI/2);
			if (!FreeOrnot) 
			{
				//��ȡ�����յ�����;
				ComItem.FDotOrient = xray.xyarr[5]+xray.xyarr[4]*
					xray.xyarr[2]/xray.xyarr[1];
				ComItem.FDotX = ComItem.FDotX+
					xray.xyarr[1]*sin(ComItem.FDotOrient-xray.xyarr[4]*PI/2);
				ComItem.FDotY = ComItem.FDotY+
					xray.xyarr[1]*cos(ComItem.FDotOrient-xray.xyarr[4]*PI/2);				
			}
			//��ȡ��Ԫ����;
			ComItem.ItemType = 32.0;//�ѵ��е�Բ����Ԫ;
			ComItem.ItemPro = xray.xyarr[1];
			ComItem.ItemDef = xray.xyarr[4];
		}
		pEntity->close();		
	}
	else if (_tcscmp(pEntity->isA()->name(),L"AcDbPolyline")==0) //��������
	{
		XYarray xray;
		int res=GetXdataFromEntity(SelEntId,xray);
		if(res==0)
		{
			ads_printf(L"ѡ��ʵ����Ч!\n");
			pEntity->close();
			return FALSE;
		}
		//��ȡ������������;		
		ComItem.FDotX = xray.xyarr[9];  //->SNorthCor;
		ComItem.FDotY = xray.xyarr[8];  //->SEastCor;
		ComItem.FDotOrient = xray.xyarr[5];
		//��ȡ��Ԫ����;
		ComItem.ItemPro = xray.xyarr[1];//A;
		ComItem.ItemDef = xray.xyarr[4];	
		//�������յ�뾶;
		if (fabs( xray.xyarr[0]-3) < DataPrecision)
		{
			ComItem.ItemType = 33.0;//�ѵ��е�ǰ����Ԫ;			
			ComItem.ItemStartR = 1E50;
			ComItem.ItemEndR = xray.xyarr[2];
			ComItem.RadForWard = LargeToSmall*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemEndR;
		}
		else if (fabs(xray.xyarr[0]-4) < DataPrecision)
		{
			ComItem.ItemType = 34.0;//�ѵ��еĺ���Ԫ;
			ComItem.ItemStartR  = xray.xyarr[2];
			ComItem.ItemEndR = 1E50;
			ComItem.RadForWard = SmallToLarge*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemStartR;
		}
		else if (fabs(xray.xyarr[0]-5) < DataPrecision)
		{
			ComItem.ItemType = 35.0;//�ѵ��е�BS����������Ԫ;
			ComItem.ItemStartR  = xray.xyarr[2];
			ComItem.ItemEndR = xray.xyarr[3];
			ComItem.RadForWard = BigToSmall*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemEndR-1/ComItem.ItemStartR);
		}
		else
		{
			ComItem.ItemType = 36.0;//�ѵ��е�SB����������Ԫ;		
			ComItem.ItemStartR  = xray.xyarr[2];
			ComItem.ItemEndR = xray.xyarr[3];
			ComItem.RadForWard = SmallToBig*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemStartR-1/ComItem.ItemEndR);
		}
		if (!FreeOrnot) 
		{
			//��������ֵ��;
			RouteDot StaDot,EndDot;
			StaDot.DotY = ComItem.FDotX;
			StaDot.DotX = ComItem.FDotY;
			StaDot.CurRadius = ComItem.ItemStartR;
			StaDot.OrientAngle = ComItem.FDotOrient;
			//���㻺���ߵ��յ�
			ComputerCorOfAnyDotOnItem(StaDot,EndDot,ComItem.ItemLen,ComItem.ItemPro,ComItem.ItemEndR,SprialItem,ComItem.ItemDef);
			//����������ֵ;
			ComItem.FDotX = EndDot.DotY;
			ComItem.FDotY = EndDot.DotX;
			//EndDot.CurRadius = ComItem.ItemEndR;
			ComItem.FDotOrient = EndDot.OrientAngle;
		}
		pEntity->close();

	}
	else 
	{

		ads_printf(L"ѡ��ʵ����Ч!\n");
		pEntity->close();
		return FALSE;
	}

	return TRUE;
}

/*
BOOL BlockComputerFun::GetComItemFromSelEntity(AcDbObjectId SelEntId,struct RampItem & ComItem,
double CurDef,BOOL FreeOrnot)
{
BOOL ContinueOrnot = FALSE;
//����CAD�ĵ�;
acedGetAcadDwgView()->SetFocus();
if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
{
return FALSE;
}
//������ȡ��ʵ��;
AcDbEntity * pEntity = NULL;
if (acdbOpenObject(pEntity,SelEntId,AcDb::kForRead)!=Acad::eOk)
{
return FALSE;
}
if (pEntity == NULL) 
{
return FALSE;
}
//���ѡ�е���ֱ��;
if (_tcscmp(pEntity->isA()->name(),L"AcDbLine")==0)
{
//��ȡֱ�ߵ����յ�;
AcDbLine * pLine = AcDbLine::cast(pEntity);
double LineSX,LineSY,LineEX,LineEY;
LineSX= (pLine->startPoint())[X];
LineSY= (pLine->startPoint())[Y];
LineEX= (pLine->endPoint())[X];
LineEY= (pLine->endPoint())[Y];
pEntity->close();
//��ʼ��������Ԫ;
double PreItemDeflect = 1;
if (fabs(CurDef) > DataPrecision) 
{
PreItemDeflect = CurDef;
}
//else
//{		
//��ʾ�û�ָ����ʱ��·������(ƫ��:˳ʱ��-�һ�����ʱ��-��)			
//	if (acedGetReal(L"\n��ָ����·����(˳ʱ��-��1,��ʱ��-��-1):\n",&PreItemDeflect) == RTCAN)
//	{				
//		return FALSE;			
//	}
//}
if ((int)PreItemDeflect == 1)
{
ContinueOrnot = TRUE;
}
if((int)PreItemDeflect == -1)
{
ContinueOrnot = TRUE;
}
if (!ContinueOrnot) 
{
return FALSE;
}			
if (fabs(PreItemDeflect-1)<DataPrecision) 
{
ComItem.FDotX = LineEX;
ComItem.FDotY = LineEY;
ComItem.FDotOrient = ConvertAngleToOrient
(GetAngleOfLineAndXaxes(LineSX,LineSY,LineEX,LineEY));			
}
else
{
ComItem.FDotX = LineSX;
ComItem.FDotY = LineSY;
ComItem.FDotOrient = ConvertAngleToOrient
(GetAngleOfLineAndXaxes(LineEX,LineEY,LineSX,LineSY));
}							
//��ȡֱ�ߵ���������;
ComItem.ItemType = 1.0;             //��ͨCADֱ��;		
ComItem.ItemDef = PreItemDeflect;   //NoneDeflect,�ʽ����¼����·����;	
}
else if (_tcscmp(pEntity->isA()->name(),L"AcDbArc")==0) 
{
//��ѡ�е�Բ��ʵ��,��ȡԲ����Ԫ�����Ԫ��;
AcDbArc * pGotEntArc = AcDbArc::cast(pEntity); 
if (FreeOrnot) 
{			
//��ʾ�û�ָ����ʱ��·������(ƫ��:˳ʱ��-�һ�����ʱ��-��)
double PreItemDeflect = 1;
//	if (acedGetReal(L"\n��ָ����·����(˳ʱ��-��1,��ʱ��-��-1):\n",&PreItemDeflect) == RTCAN)
//	{
//		return FALSE;
//	}
if ((int)(PreItemDeflect+DataPrecision) == 1)
{
ContinueOrnot = TRUE;
}
if((int)(PreItemDeflect+DataPrecision) == -1)
{
ContinueOrnot = TRUE;
}
if (!ContinueOrnot) 
{
return FALSE;
}	
//��ȡԲ������;
ComItem.FDotX = (pGotEntArc->center())[X];
ComItem.FDotY = (pGotEntArc->center())[Y];					
ComItem.ItemDef = PreItemDeflect;
}
else
{
//��ʾ�û�ָ����ʱ��·������(ƫ��:˳ʱ��-�һ�����ʱ��-��)
if (fabs(CurDef) < DataPrecision) 
{
if (acedGetReal(L"\n��ָ����·����(˳ʱ��-��1,��ʱ��-��-1):\n",&CurDef) == RTCAN)
{
return FALSE;
}
if ((int)(CurDef+DataPrecision) == 1)
{
ContinueOrnot = TRUE;
}
if((int)(CurDef+DataPrecision) == -1)
{
ContinueOrnot = TRUE;
}
if (!ContinueOrnot) 
{
return FALSE;
}	
}
if (fabs(CurDef+1)<DataPrecision)
{
ComItem.FDotX = (pGotEntArc->center())[X] + pGotEntArc->radius()*sin(ConvertAngleToOrient(pGotEntArc->endAngle()));
ComItem.FDotY = (pGotEntArc->center())[Y] + pGotEntArc->radius()*cos(ConvertAngleToOrient(pGotEntArc->endAngle()));				
ComItem.FDotOrient = ConvertAngleToOrient(pGotEntArc->endAngle()+PI/2);
}
else
{
ComItem.FDotX = (pGotEntArc->center())[X] + pGotEntArc->radius()*sin(ConvertAngleToOrient(pGotEntArc->startAngle()));
ComItem.FDotY = (pGotEntArc->center())[Y] + pGotEntArc->radius()*cos(ConvertAngleToOrient(pGotEntArc->startAngle()));				
ComItem.FDotOrient = ConvertAngleToOrient(pGotEntArc->startAngle()-PI/2);				
}
ComItem.ItemDef = CurDef;
}
//��ȡ��Ԫ����;
ComItem.ItemType = 2.0;                //��ͨCADԲ��;
ComItem.ItemPro = pGotEntArc->radius();//R;	
pEntity->close();		
}
else if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER")==0) 
{
//��õ�ǰ��ѡ�ѵ�ʵ������һ����Ч��Ԫ����;
JD_CENTER * pGotRamp = JD_CENTER::cast(pEntity);
double ValidItemType = pGotRamp->XYArray[pGotRamp->XYNum-1][0];
//�����ԪΪֱ��;
if (fabs(ValidItemType-1) < DataPrecision)
{			
//���ֱ���յ�����(���㵥Ԫȡ������������Ϊ��Ԫ�յ�����,����λ�������ȡ��㴦����);
ComItem.FDotX = pGotRamp->XYArray[pGotRamp->XYNum-1][9]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*sin(pGotRamp->XYArray[pGotRamp->XYNum-1][5]);
ComItem.FDotY = pGotRamp->XYArray[pGotRamp->XYNum-1][8]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*cos(pGotRamp->XYArray[pGotRamp->XYNum-1][5]);
ComItem.FDotOrient = pGotRamp->XYArray[pGotRamp->XYNum-1][5];
//���õ�Ԫ����;
ComItem.ItemType = 31.0;//�ѵ��е�ֱ����Ԫ;
pEntity->close();
}
else if(fabs(ValidItemType-2) < DataPrecision)
{
//��ȡ����ӦԲ����Ԫ��Բ������;
ComItem.FDotX = pGotRamp->XYArray[pGotRamp->XYNum-1][9]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*sin(pGotRamp->XYArray[pGotRamp->XYNum-1][5]+pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);
ComItem.FDotY = pGotRamp->XYArray[pGotRamp->XYNum-1][8]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*cos(pGotRamp->XYArray[pGotRamp->XYNum-1][5]+pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);
if (!FreeOrnot) 
{
//��ȡ�����յ�����;
ComItem.FDotOrient = pGotRamp->XYArray[pGotRamp->XYNum-1][5]+pGotRamp->XYArray[pGotRamp->XYNum-1][4]*
pGotRamp->XYArray[pGotRamp->XYNum-1][2]/pGotRamp->XYArray[pGotRamp->XYNum-1][1];
ComItem.FDotX = ComItem.FDotX+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*sin(ComItem.FDotOrient-pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);
ComItem.FDotY = ComItem.FDotY+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*cos(ComItem.FDotOrient-pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);				
}
//��ȡ��Ԫ����;
ComItem.ItemType = 32.0;//�ѵ��е�Բ����Ԫ;
ComItem.ItemPro = pGotRamp->XYArray[pGotRamp->XYNum-1][1];
ComItem.ItemDef = pGotRamp->XYArray[pGotRamp->XYNum-1][4];
pEntity->close();
}
else //ValidItemType=3,4,5,6;
{
//��ȡ������������;		
ComItem.FDotX = pGotRamp->XYArray[pGotRamp->XYNum-1][9];  //->SNorthCor;
ComItem.FDotY = pGotRamp->XYArray[pGotRamp->XYNum-1][8];  //->SEastCor;
ComItem.FDotOrient = pGotRamp->XYArray[pGotRamp->XYNum-1][5];
//��ȡ��Ԫ����;
ComItem.ItemPro = pGotRamp->XYArray[pGotRamp->XYNum-1][1];//A;
ComItem.ItemDef = pGotRamp->XYArray[pGotRamp->XYNum-1][4];	
//�������յ�뾶;
if (fabs(ValidItemType-3) < DataPrecision)
{
ComItem.ItemType = 33.0;//�ѵ��е�ǰ����Ԫ;			
ComItem.ItemStartR = 1E50;
ComItem.ItemEndR = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.RadForWard = LargeToSmall*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemEndR;
}
else if (fabs(ValidItemType-4) < DataPrecision)
{
ComItem.ItemType = 34.0;//�ѵ��еĺ���Ԫ;
ComItem.ItemStartR  = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.ItemEndR = 1E50;
ComItem.RadForWard = SmallToLarge*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemStartR;
}
else if (fabs(ValidItemType-5) < DataPrecision)
{
ComItem.ItemType = 35.0;//�ѵ��е�BS����������Ԫ;
ComItem.ItemStartR  = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.ItemEndR = pGotRamp->XYArray[pGotRamp->XYNum-1][3];
ComItem.RadForWard = BigToSmall*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemEndR-1/ComItem.ItemStartR);
}
else
{
ComItem.ItemType = 36.0;//�ѵ��е�SB����������Ԫ;		
ComItem.ItemStartR  = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.ItemEndR = pGotRamp->XYArray[pGotRamp->XYNum-1][3];
ComItem.RadForWard = SmallToBig*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemStartR-1/ComItem.ItemEndR);
}
if (!FreeOrnot) 
{
//��������ֵ��;
RouteDot StaDot,EndDot;
StaDot.DotY = ComItem.FDotX;
StaDot.DotX = ComItem.FDotY;
StaDot.CurRadius = ComItem.ItemStartR;
StaDot.OrientAngle = ComItem.FDotOrient;
//���㻺���ߵ��յ�
ComputerCorOfAnyDotOnItem(StaDot,EndDot,ComItem.ItemLen,ComItem.ItemPro,ComItem.ItemEndR,SprialItem,ComItem.ItemDef);
//����������ֵ;
ComItem.FDotX = EndDot.DotY;
ComItem.FDotY = EndDot.DotX;
//EndDot.CurRadius = ComItem.ItemEndR;
ComItem.FDotOrient = EndDot.OrientAngle;
}
pEntity->close();
}
}
else 
{
ads_printf(L"ѡ��ʵ����Ч!\n");
pEntity->close();
return FALSE;
}
return TRUE;
}*/


void BlockComputerFun::GetOtherParameterOfArc(double StartX,double StartY,double StartOA,double ArcRad,double ArcLen,double ArcDef,
											  double &ArcCenX,double &ArcCenY,double & ArcEndX,double &ArcEndY)
{
	//����Բ����,ƫ��,���뾶ȷ��Բ������;
	ArcCenX = StartX + ArcRad*sin(StartOA + ArcDef*PI/2);
	ArcCenY = StartY + ArcRad*cos(StartOA + ArcDef*PI/2);
	//������������յ㴦�ķ�λ��;
	double EndOA = StartOA + ArcDef*ArcLen/ArcRad;
	//��ȡ�յ��������;
	ArcEndX = ArcCenX + ArcRad*sin(EndOA - ArcDef*PI/2);
	ArcEndY = ArcCenY + ArcRad*cos(EndOA - ArcDef*PI/2);
}

double BlockComputerFun::ChangeDataPrecision(double InputData,int PreLength)
{
	//���徫���ִ�;
	CString InterString;
	//�����ݵ����ִ���ʽ;
	InterString.Format(L"%lf",InputData);
	//���С�����λ��;
	int PosPoint = InterString.Find(L".") + 1;
	//�����Ƚ�ȡС��λ;
	InterString = InterString.Left(PosPoint+PreLength);
	//���ط��Ͼ���Ҫ�������;
	return _wtof(InterString);
}

// ������·ʵ��ID���丽��һ����ȡ�˵��Ӧ����·ͶӰ��̼�ͶӰ��̴�����·�����(��������Ϊ�������)
void BlockComputerFun::ComputerMilesAndCorsOfRoute(AcDbObjectId InputRouteId,
												   ads_point nearPoint,double &ItemType,double &ReturnProMils,xlpoint &RoutePoint)
{
	// ��������CAD�ĵ�;
	acedGetAcadDwgView()->SetFocus();
	if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD�ĵ��򿪴���!\n");
		return ;
	}		
	// ������ȡ��ʵ��
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,InputRouteId,AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"��ʵ��ʧ��!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return ;
	}
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER") != 0) 
	{
		acutPrintf(L"��ѡʵ�岻����·ʵ��!\n");
		pEntity->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		return ;
	}
	JD_CENTER *pGottnRamp = JD_CENTER::cast(pEntity);
	// ���ͶӰ�㴦��·ͳһ���
	ReturnProMils = pGottnRamp->PROJ_ML(nearPoint[Y], nearPoint[X]);
	// ��ô˵��Ӧ��Ԫ������
	int ItemNumber = pGottnRamp->GetXYNoFromTYLC(ReturnProMils);
	ItemType = pGottnRamp->XYArray[ItemNumber][0];
	// ���ͳһ��̵���·��
	RoutePoint.lc = ReturnProMils;	
	pGottnRamp->xlpoint_pz(&RoutePoint);
	pEntity->close();
	// �ͷ�CAD�ĵ�
	acDocManager->unlockDocument(acDocManager->curDocument());
	return ;
}