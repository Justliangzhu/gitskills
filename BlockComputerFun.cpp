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
//当已知直线的方位角时求解直线的斜率K;
double BlockComputerFun::GetValueKByOrient(double ValueOrient)
{
	//定义斜率变量;
	double ValueK;
	//判断方位角是否是0或180度;
	if (fmod(ValueOrient,360) == 0||
		fmod(ValueOrient,360) == 180)
	{
		ValueK = 1E50;//无穷大;
	}
	else ValueK = tan(PI/2-ValueOrient*PI/180);
	//返回所求的K值;
	return ValueK;
}
//已知两点获取两点间距离(或直线长度);
double BlockComputerFun::GetPreciseLength(double x1,double y1,double x2,double y2)
{
	//定义距离变量,纵横增量变量以及纵横增量绝对值变量;
	double Distance,DistanceX,DistanceY,InterimX,InterimY;
	//获取纵横增量以及纵横增量绝对值;
	DistanceX = x1-x2;
	DistanceY = y1-y2;
	InterimX = fabs(DistanceX);
	InterimY = fabs(DistanceY);
	//获取纵纵横增量绝对值中最大者与最小者;
	double ValueMax,ValueMin;
	ValueMax = (InterimX>InterimY)?InterimX:InterimY;
	ValueMin = (InterimX<InterimY)?InterimX:InterimY;
	//获取纵纵横增量绝对值中最小者与最大者的比值;
	double ValueScale;
	ValueScale = ValueMin/ValueMax;
	//获取两点间的距离;
	Distance = ValueMax*sqrt(1.0+ValueScale*ValueScale);
	//返回两点间距离;
	return Distance;	
}
//求直线外一点在直线上的投影点坐标;
void BlockComputerFun::GetProjectDotOfLine(double LineX1,double LineY1,double ValueK,
										   double SelDotX,double SelDotY,double &ProjX,double &ProjY)
{
	//如果当前直线与Y轴平行即ValueK的值为无穷大;
	if (ValueK == 1E50)
	{
		//投影点的坐标可直接看出;
		ProjX = LineX1;   //(或ProjX = LineX2);
		ProjY = SelDotY;  
	}
	else
	{
		double ValueA,ValueB;
		ValueA = ValueK/sqrt(ValueK*ValueK+1);
		ValueB = 1/sqrt(ValueK*ValueK+1);
		//获取选取点与直线的距离;
		double Dist;
		Dist = ValueA*SelDotX-ValueB*SelDotY-(ValueA*LineX1-ValueB*LineY1);
		//如果当前点在直线上则;
		if (Dist == 0) 
		{
			//投影点即为选取点;
			ProjX = SelDotX;   
			ProjY = SelDotY; 
		}
		else 
		{
			//获取选取点投影点坐标;
			ProjX = SelDotX - Dist*ValueA;
			ProjY = SelDotY + Dist*ValueB;
		}
	}
}
double BlockComputerFun::ConvertAngleToOrient(double AngleOfLineAndXaxes)
{
	AngleOfLineAndXaxes = AngleOfLineAndXaxes*180/PI;
	//检测辐角值是否是辐角主值(即在0~360度间的正角);
	if (fabs(AngleOfLineAndXaxes)>360) 
	{
		//将所给的角值换算成绝对值在0~360间度的角值;
		AngleOfLineAndXaxes = fmod(AngleOfLineAndXaxes,360);
	}
	//将所给的角值换算成绝对值在0~360间度的正角;
	if (AngleOfLineAndXaxes<0) 
		AngleOfLineAndXaxes = AngleOfLineAndXaxes+360;
	//如果辐角主值在0~90度之间(相当于方位角的一象限);
	if (AngleOfLineAndXaxes>=0&&AngleOfLineAndXaxes<90) 
		return (90-AngleOfLineAndXaxes)*PI/180;
	//如果辐角主值在90~360度之间(相当于方位角的四,三,二象限);
	else return (450-AngleOfLineAndXaxes)*PI/180;
	//返回角值的单位是度;
}

void BlockComputerFun::GetCrossDotOfLine(double ValueK1,double LineX1,double LineY1,
										 double ValueK2,double LineX2,double LineY2,
										 double &CrossX,double &CrossY)
{
	//如果第一条直线与Y轴平行;
	if (ValueK1 == 1E50)
	{
		CrossX = LineX1;
		CrossY = ValueK2*(LineX1-LineX2)+LineY2;
		return ;
	}
	//如果第二条直线与Y轴平行;
	else if (ValueK2 == 1E50)
	{
		CrossX = LineX2;
		CrossY = ValueK1*(LineX2-LineX1)+LineY1;
		return ;
	}
	//如果两直线平行;
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

//求直线外一点到直线的距离;
double BlockComputerFun::GetDistOfLineAndSelDot(double ValueK,double LineX,double LineY,
												double SelDotX,double SelDotY)
{
	//如果直线与Y轴平行;
	if (ValueK ==1E40)
		return SelDotX-LineX;
	else
		return (ValueK*SelDotX-SelDotY-(ValueK*LineX-LineY))/sqrt(ValueK*ValueK+1);
}


BOOL BlockComputerFun::GetCenterCorAndRadius(double CurCenX,double CurCenY,double CurRadius,double PreDeflect,double &ItemDeflect,
											 double FixDotX,double FixDotY,double MoveDotX,double MoveDotY,
											 double &ComCenX,double &ComCenY,double &ComRadius)
{
	//计算圆心与第一点间的长度;
	double Length,Angle1;
	Length = distfwj(CurCenX,CurCenY,FixDotX,FixDotY,&Angle1);
	//如果当前圆在所求圆外或内;
	if (Length > CurRadius+0.0001)
	{	
		//ItemDeflect表示布线时采用左偏布线还是右偏布线;
		//判明第二点的位置;
		//判断点是否落于当前圆内;
		double InterAngle,InterLen;
		InterLen = distfwj(CurCenX,CurCenY,MoveDotX,MoveDotY,&InterAngle);
		if (fabs(InterLen - CurRadius)<DataPrecision) 
		{
			return FALSE;	
		}
		//由所求圆上两点求此两点的中垂线方程;
		double MidDotX,MidDotY,ValueK;
		MidDotX = (FixDotX+MoveDotX)/2;
		MidDotY = (FixDotY+MoveDotY)/2;
		ValueK = GetValueKByTwoPoint(FixDotX,FixDotY,MoveDotX,MoveDotY);
		//如果中垂线与纵轴垂直;
		if (fabs(ValueK)<0.00000001) ValueK = 1E50;
		else if (fabs(ValueK- 1E50)<0.00000001) ValueK = 0;
		else ValueK = -1/ValueK;
		//求解此时通过已知两点且与已知圆相切的圆其半径与圆心坐标;
		if (fabs(ValueK- 1E50)<0.00000001)
		{
			//则所求圆的圆心坐标横坐标为;
			ComCenX = MidDotX;
			//计算所求圆的圆心坐标纵坐标;
			//定义三个系数变量;
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
			//求解Y值大小;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double Y1,Y2;
				Y1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				//判断有效圆心值;
				if (PreDeflect == 1)//前一线元是右偏;
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
				else//前一线元是左偏;
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
				//求取返回圆的半径;
				ComRadius = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+
					(FixDotY-ComCenY)*(FixDotY-ComCenY));
			}		
		}
		else
		{
			//此时已知圆心横坐标与纵坐标之间的关系Y=K(X-X1)+Y1;;
			//定义五个系数变量;
			double FacA,FacB,FacC,FacD,FacE;
			FacE = 2*(ValueK*(CurCenY-FixDotY)+(CurCenX-FixDotX));
			FacD = FixDotX*FixDotX+(MidDotY-ValueK*MidDotX-FixDotY)*(MidDotY-ValueK*MidDotX-FixDotY)
				-(CurCenX*CurCenX+CurRadius*CurRadius+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY));

			FacA = 4*CurRadius*CurRadius*(1+ValueK*ValueK)-FacE*FacE;
			FacB = 8*CurRadius*CurRadius*(ValueK*(MidDotY-ValueK*MidDotX-CurCenY)-CurCenX)-2*FacD*FacE;
			FacC = 4*CurRadius*CurRadius*(CurCenX*CurCenX+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY))-FacD*FacD;
			//求解X值大小;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double X1,X2,Y1,Y2;
				X1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y1 = ValueK*(X1-MidDotX)+MidDotY;
				X2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				Y2 = ValueK*(X2-MidDotX)+MidDotY;
				//判断有效圆心值;
				if (PreDeflect == 1)//前一线元是右偏;
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
				else//前一线元是左偏;
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
				//求取返回圆的半径;
				ComRadius = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+
					(FixDotY-ComCenY)*(FixDotY-ComCenY));
			}		
		}
		return TRUE;
	}
	//如果当前圆圆内切;
	else if (Length < CurRadius-0.0001)
	{
		//ItemDeflect表示布线时采用大半径圆(1)布形还是小半径(-1)圆形布线;
		//判明第二点的位置;
		//判断点是否落于当前圆内;
		double InterAngle,InterLen;
		InterLen = distfwj(CurCenX,CurCenY,MoveDotX,MoveDotY,&InterAngle);
		if (InterLen >= CurRadius) return FALSE;
		//由所求圆上两点求此两点的中垂线方程;
		double MidDotX,MidDotY,ValueK;
		MidDotX = (FixDotX+MoveDotX)/2;
		MidDotY = (FixDotY+MoveDotY)/2;
		ValueK = GetValueKByTwoPoint(FixDotX,FixDotY,MoveDotX,MoveDotY);
		//如果中垂线与纵轴垂直;
		if (fabs(ValueK)<0.00000001) ValueK = 1E50;
		else if (ValueK == 1E50) ValueK = 0;
		else ValueK = -1/ValueK;
		//求解此时通过已知两点且与已知圆相切的圆其半径与圆心坐标;
		if (ValueK == 1E50)
		{
			//则所求圆的圆心坐标横坐标为;
			ComCenX = MidDotX;
			//计算所求圆的圆心坐标纵坐标;
			//定义三个系数变量;
			double FacA,FacB,FacC;
			FacA = 4*((FixDotY-CurCenY)*(FixDotY-CurCenY)-CurRadius*CurRadius);
			FacB = 4*(CurCenY-FixDotY)*((ComCenX-FixDotX)*(ComCenX-FixDotX)-
				(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+
				FixDotY*FixDotY-CurCenY*CurCenY)+8*CurRadius*CurRadius*CurCenY;
			FacC = -4*CurRadius*CurRadius*((ComCenX-CurCenX)*(ComCenX-CurCenX)+CurCenY*CurCenY)
				+((ComCenX-FixDotX)*(ComCenX-FixDotX)-(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+
				FixDotY*FixDotY-CurCenY*CurCenY)*((ComCenX-FixDotX)*(ComCenX-FixDotX)
				-(ComCenX-CurCenX)*(ComCenX-CurCenX)-CurRadius*CurRadius+FixDotY*FixDotY-CurCenY*CurCenY);
			//求解Y值大小;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double Y1,Y2;
				Y1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				//判断有效圆心值;
				double Radius1,Radius2;
				Radius1 = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y1)*(FixDotY-Y1));
				Radius2 = sqrt((FixDotX-ComCenX)*(FixDotX-ComCenX)+(FixDotY-Y2)*(FixDotY-Y2));
				//右偏进行布线;
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
				//左偏进行布线;
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
			//此时已知圆心横坐标与纵坐标之间的关系Y=K(X-X1)+Y1;;
			//定义五个系数变量;
			double FacA,FacB,FacC,FacD,FacE;
			FacE = 2*(ValueK*(CurCenY-FixDotY)+(CurCenX-FixDotX));
			FacD = FixDotX*FixDotX+(MidDotY-ValueK*MidDotX-FixDotY)*(MidDotY-ValueK*MidDotX-FixDotY)
				-(CurCenX*CurCenX+CurRadius*CurRadius+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY));

			FacA = 4*CurRadius*CurRadius*(1+ValueK*ValueK)-FacE*FacE;
			FacB = 8*CurRadius*CurRadius*(ValueK*(MidDotY-ValueK*MidDotX-CurCenY)-CurCenX)-2*FacD*FacE;
			FacC = 4*CurRadius*CurRadius*(CurCenX*CurCenX+(MidDotY-ValueK*MidDotX-CurCenY)
				*(MidDotY-ValueK*MidDotX-CurCenY))-FacD*FacD;
			//求解X值大小;
			double InterDerta = FacB*FacB-4*FacA*FacC;
			if (InterDerta <0.00000001) return FALSE;
			else
			{
				double X1,X2,Y1,Y2;
				X1 = ((-FacB-sqrt(InterDerta)))/(2*FacA);
				Y1 = ValueK*(X1-MidDotX)+MidDotY;
				X2 = ((-FacB+sqrt(InterDerta)))/(2*FacA);
				Y2 = ValueK*(X2-MidDotX)+MidDotY;
				//判断有效圆心值;
				double Radius1,Radius2;
				Radius1 = sqrt((FixDotX-X1)*(FixDotX-X1)+(FixDotY-Y1)*(FixDotY-Y1));
				Radius2 = sqrt((FixDotX-X2)*(FixDotX-X2)+(FixDotY-Y2)*(FixDotY-Y2));
				//右偏进行布线;
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
				//左偏进行布线;
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
									   double ItemDeflect,double ArcDotX1,double ArcDotY1,/*切点*/
									   double ArcDotX2,double ArcDotY2/*活动点*/,AcDbObjectId &ArcId,AcDbArc * pArc)
{
	//求取圆弧上三点相应处的辐角;
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
	//创建圆弧;
	if (pArc == NULL)
	{
		pArc = new AcDbArc;
		ArcId = AddEntityToDbs(pArc);
	}
	//定义圆弧起终辐角变量;
	double StartAngle,EndAngle;
	//如果当前圆弧是右偏则;
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
	//修改圆弧;
	//acdbOpenObject(pArc,ArcId,AcDb::kForWrite);
	if(acdbOpenObject(pArc,ArcId,AcDb::kForWrite)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
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
	//求两点圆心之间的距离;
	double Dist,Angle;
	Dist = distfwj(CenX1,CenY1,CenX2,CenY2,&Angle);
	Angle = angleX(AcGePoint3d(CenX1,CenY1,0.0),AcGePoint3d(CenX2,CenY2,0.0));
	//如果当前两圆是外切关系;
	if (fabs(Dist-CenRadius1-CenRadius2)<0.00000001)
	{
		TanX = CenX1+CenRadius1*sin(ConvertAngleToOrient(Angle));
		TanY = CenY1+CenRadius1*cos(ConvertAngleToOrient(Angle));
	}
	//2圆包含1圆;
	else if (fabs(Dist+CenRadius1-CenRadius2)<0.00000001)
	{
		Angle+=PI;
		TanX = CenX1+CenRadius1*sin(ConvertAngleToOrient(Angle));
		TanY = CenY1+CenRadius1*cos(ConvertAngleToOrient(Angle));
	}
	//1圆包含2圆;
	else
	{
		TanX = CenX1+CenRadius1*sin(ConvertAngleToOrient(Angle));
		TanY = CenY1+CenRadius1*cos(ConvertAngleToOrient(Angle));
	}
}
double BlockComputerFun::GetValueKByTwoPoint(double LineX1,double LineY1,double LineX2,double LineY2)
{
	//定义横纵差距变量;
	double DistX,DistY;
	DistX = LineX1 - LineX2;
	DistY = LineY1 - LineY2;
	//判断DistX是否为0;
	if (fabs(DistX) < 0.00000001) return 1E50;
	else return DistY/DistX;
}
double BlockComputerFun::GetDistOfLineAndDot(double LineX1,double LineY1,double LineX2,
											 double LineY2,double DotX,double DotY)
{
	//获取直线的斜率;
	double ValueK  = GetValueKByTwoPoint(LineX1,LineY1,LineX2,LineY2);
	//计算点到直线的实际距离;
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
	//获取点到直线的距离;
	double CurDist;
	CurDist = GetDistOfLineAndDot(LineX1,LineY1,LineX2,
		LineY2,DotX,DotY);
	//如果点在直线的左侧或上方时;
	if (CurDist < -0.00000001) return -1;
	//如果点在直线上则;
	else if (fabs(CurDist) <= 0.00000001) return 0;
	else return 1;
}

int BlockComputerFun::DotAtMiddleOrOut(double LineX1,double LineY1,double LineX2,
									   double LineY2,double DotX,double DotY)
{
	//获取当前直线某一无穷远点;
	double InterX,InterY;
	double ValueK = GetValueKByTwoPoint(LineX1,LineY1,LineX2,LineY2);
	//计算无穷远点的坐标;
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
	//计算选定点在直线上的投影点;
	double ProjX,ProjY;
	GetProjectDotOfLine(LineX1,LineY1,ValueK,DotX,DotY,ProjX,ProjY);
	//计算直线上第一点第二点及选定点的投影点与无穷远点的距离;
	double Dist1,Dist2,DistP,A;
	Dist1 = distfwj(InterX,InterY,LineX1,LineY1,&A);
	Dist2 = distfwj(InterX,InterY,LineX2,LineY2,&A);
	DistP = distfwj(InterX,InterY,ProjX,ProjY,&A);
	//如果投影点在直线第一第二点之间;
	if (((Dist1<=DistP)&&(DistP<=Dist2))||
		((Dist2<=DistP)&&(DistP<=Dist1)))
		return 1;
	//如果投影点在直线第一第二点之外;
	else return -1;
}

////将方位角转化成辐角;
double BlockComputerFun::ConvertOrientToAngle(double ValueOrient)
{
	ValueOrient = ValueOrient*180/PI;
	//方位角的范围是:0~360度;
	//如果给定的方位角在0~90度间(相当于辐角的第一象限);
	if (ValueOrient>0 && ValueOrient<=90) 
		return (90-ValueOrient)*PI/180;
	//如果给定的方位角在90~360度间(相当于辐角的第四,三,二象限);
	else 
		return (450-ValueOrient)*PI/180;
}

//根据某一线元的数据,计算此线元上任一点的坐标;
int BlockComputerFun::ComputerCorOfAnyDotOnItem(RouteDot &SDot,RouteDot &EDot,double CirLen,
												double ValueA,double ERadius,int ItemType,int ItemDeflect)
{  
	//如果当前线元是直线则;
	if (ItemType == LineItem) 
	{
		EDot.DotX = SDot.DotX+CirLen*sin(SDot.OrientAngle);
		EDot.DotY = SDot.DotY+CirLen*cos(SDot.OrientAngle);
		EDot.OrientAngle = SDot.OrientAngle;
		EDot.CurRadius = 1E50;
		EDot.CurMiles = SDot.CurMiles+CirLen;

		return 1;
	}
	//如果当前线元是圆弧;
	else if (ItemType == CirOrArcItem)
	{
		////起点处的半径即为圆弧半径;
		//计算圆心的坐标;
		double CenX,CenY;
		//获取起点到圆心的方位角;
		double InterOrient = SDot.OrientAngle + ItemDeflect*PI/2;
		if (InterOrient<DataPrecision) 
			InterOrient+=2*PI;
		CenX = SDot.DotX + SDot.CurRadius*sin(InterOrient);
		CenY = SDot.DotY + SDot.CurRadius*cos(InterOrient);
		//获取终点处的坐标方位角;
		EDot.OrientAngle = SDot.OrientAngle + ItemDeflect*CirLen/SDot.CurRadius;
		//获取圆心到终点方向的方位角;
		InterOrient = EDot.OrientAngle - ItemDeflect*PI/2;
		//终点半径即为起点半径;
		EDot.CurRadius = SDot.CurRadius;
		//计算终点的坐标;
		EDot.DotX = CenX + EDot.CurRadius*sin(InterOrient);
		EDot.DotY = CenY + EDot.CurRadius*cos(InterOrient);
		//终点处的里程为;
		EDot.CurMiles = SDot.CurMiles + CirLen;

		return 1;
	}
	else if (ItemType == SprialItem) 
	{
		int RadForward = GetRadiusForward(SDot.CurRadius,ERadius);
		if (RadForward == LargeToSmall) 
		{	
			//SDot.CurRadius = 1E50;
			//获取曲线长度;
			double RealLen;
			RealLen = ValueA*ValueA/ERadius;

			//当前指定长度处的曲线偏转角;
			double DeflectAngle;
			DeflectAngle = CirLen*CirLen/(2*ERadius*RealLen);
			//定义相对坐标变量;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
			//定义曲线的源点变量;
			double OrignX,OrignY,OrignOrient;
			OrignX = SDot.DotX;
			OrignY = SDot.DotY;
			OrignOrient = SDot.OrientAngle;

			//获取源点处的辐角;
			double AngleOfOrign;
			AngleOfOrign = ConvertOrientToAngle(OrignOrient);
			EDot.OrientAngle = SDot.OrientAngle+ItemDeflect*DeflectAngle;
			//求指定长度处点的坐标;
			EDot.DotX = OrignX+InterX*sin(AngleOfOrign)-ItemDeflect*InterY*cos(AngleOfOrign);
			EDot.DotY = OrignY+InterX*cos(AngleOfOrign)+ItemDeflect*InterY*sin(AngleOfOrign);
			//初始化所求点处的半径;
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
			//获取源点坐标及相应处的辐角;
			//获取起点到源点的曲线长;
			double CurLength = ValueA*ValueA/SDot.CurRadius;
			//获取从源点到起点的偏角;
			double DeflectAngle = CurLength/(2*SDot.CurRadius);
			//获取源点处的方位角;
			double OrignOrient = SDot.OrientAngle - ItemDeflect*DeflectAngle;			
			//获取源点处的辐角;
			double OrignAngle = ConvertOrientToAngle(OrignOrient);
			//获取源点处的坐标;
			double OrignX,OrignY;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CurLength);
			InterY = GetAbsYCorOfSprialCir(ValueA,CurLength);
			OrignX = SDot.DotX - InterX*sin(OrignAngle)+ItemDeflect*InterY*cos(OrignAngle);
			OrignY = SDot.DotY - InterX*cos(OrignAngle)-ItemDeflect*InterY*sin(OrignAngle);
			//当前半径是由大到小;
			double ForLength = CurLength+CirLen;
			//求取指定长度处的相对坐标;
			InterX = GetAbsXCorOfSprialCir(ValueA,ForLength);
			InterY = GetAbsYCorOfSprialCir(ValueA,ForLength);			
			//求取指定长度处的绝对坐标;
			EDot.DotX = OrignX+InterX*sin(OrignAngle)-ItemDeflect*InterY*cos(OrignAngle);
			EDot.DotY = OrignY+InterX*cos(OrignAngle)+ItemDeflect*InterY*sin(OrignAngle);
			//求取指定长度处的半径;
			EDot.CurRadius = ValueA*ValueA/ForLength;
			//求取指定长度处的方位角;
			EDot.OrientAngle = OrignOrient+ItemDeflect*ForLength*ForLength/(2*ValueA*ValueA);
			//所求处的里程;
			EDot.CurMiles = SDot.CurMiles+CirLen;

			return 1;
		}
		else if (RadForward == SmallToBig) 
		{
			//当前ValueA不等于0,CirLen大于等于0,Big<1E50;
			if (ValueA <DataPrecision) 
				return -1;
			//获取源点坐标;
			double OrignX,OrignY,OrignOrient;
			//当前曲线完整长为;
			double CurCirLen = ValueA*ValueA/SDot.CurRadius;
			//曲线的偏转角为;
			double DivertAngle = CurCirLen/(2*SDot.CurRadius);
			//源点的坐标方位角为;
			OrignOrient = SDot.OrientAngle + ItemDeflect*DivertAngle;
			//求取坐标转换角;
			double Angle = ConvertOrientToAngle(OrignOrient) - ItemDeflect*PI;
			double InterX = GetAbsXCorOfSprialCir(ValueA,CurCirLen);
			double InterY = GetAbsYCorOfSprialCir(ValueA,CurCirLen);
			//求得源点坐标;
			OrignX = SDot.DotX - InterX*sin(Angle)-ItemDeflect*InterY*cos(Angle);
			OrignY = SDot.DotY - InterX*cos(Angle)+ItemDeflect*InterY*sin(Angle);			
			//当前所求点对应的曲线长为;
			double CurLen = CurCirLen - CirLen;
			//求取所求点在缓曲线上对应的相对坐标;
			InterX = GetAbsXCorOfSprialCir(ValueA,CurLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CurLen);			
			//获取所求点的坐标;
			EDot.DotX = OrignX+InterX*sin(Angle)+/*-*/ItemDeflect*InterY*cos(Angle);
			EDot.DotY = OrignY+InterX*cos(Angle)-/*+*/ItemDeflect*InterY*sin(Angle);
			//获取所求点处的方位角,半径及里程;
			EDot.CurRadius = ValueA*ValueA/CurLen;
			EDot.OrientAngle = SDot.OrientAngle + ItemDeflect*(ValueA*ValueA/(2*SDot.CurRadius*SDot.CurRadius)-
				ValueA*ValueA/(2*EDot.CurRadius*EDot.CurRadius));
			EDot.CurMiles  = SDot.CurMiles+CirLen;

			return 1;
		}
		else if (RadForward == SmallToLarge)
		{
			//当前ValueA不等于0,CirLen大于等于0;
			if (ValueA < DataPrecision) 
				return FALSE;
			//获取源点坐标;
			double OrignX,OrignY,OrignOrient;
			//当前曲线完整长为;
			double CurCirLen = ValueA*ValueA/SDot.CurRadius;
			//曲线的偏转角为;
			double DivertAngle = CurCirLen/(2*SDot.CurRadius);
			//源点的坐标方位角为;
			OrignOrient = SDot.OrientAngle + ItemDeflect*DivertAngle;
			//求取坐标转换角;
			double Angle = ConvertOrientToAngle(OrignOrient) - ItemDeflect*PI;

			double InterX = GetAbsXCorOfSprialCir(ValueA,CurCirLen);
			double InterY = GetAbsYCorOfSprialCir(ValueA,CurCirLen);
			//求得源点坐标;
			OrignX = SDot.DotX - InterX*sin(Angle)-/*+*/ItemDeflect*InterY*cos(Angle);
			OrignY = SDot.DotY - InterX*cos(Angle)+/*-*/ItemDeflect*InterY*sin(Angle);
			//当前所求点对应的曲线长为;
			double CurLen = CurCirLen - CirLen;
			//求取所求点在缓曲线上对应的相对坐标;
			InterX = GetAbsXCorOfSprialCir(ValueA,CurLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CurLen);			
			//获取所求点的坐标;
			EDot.DotX = OrignX+InterX*sin(Angle)+/*-*/ItemDeflect*InterY*cos(Angle);
			EDot.DotY = OrignY+InterX*cos(Angle)-/*+*/ItemDeflect*InterY*sin(Angle);
			//获取所求点处的方位角,半径及里程;
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
//获取n阶乘(Factorial)值大小;
long  BlockComputerFun::GetValueOfFactorial(long ValueNum)
{
	//判断当前阶乘是否是0!的形式;
	if (ValueNum == 0) return 1;
	//定义基值变量;
	long ReturnValue = 1;
	//利用循环获取n阶乘值;
	for(long i=1;i<ValueNum+1;i++)
		ReturnValue = ReturnValue*i;
	//返回所求值;
	return ReturnValue;
}
//获取n的m次方值大小;
double  BlockComputerFun::GetValueOfMonWithNumPower(double ValuePar,long NumPower)
{   
	//定义基值变量;
	double BaseVar = 1.0;
	//如果次方值NumPower大于0; 
	if (NumPower>0)
	{				
		for (long i=1;i<=NumPower;i++)
			BaseVar = BaseVar*ValuePar;
	}
	//如果次方值NumPower等于0; 
	else if (NumPower == 0) return 1;
	//如果次方值NumPower小于0; 
	else
	{
		for (long i=1;i<=fabs(1.0*NumPower);i++)
			BaseVar = BaseVar*ValuePar;	
		BaseVar = 1/BaseVar;
	}
	//返回所求值;
	return BaseVar;
}
//根据传入的回旋参数与曲线长度获取缓和曲线在相对坐标系中的横距;
double BlockComputerFun::GetAbsXCorOfSprialCir(double ValueA,double CirLength)
{
	//如果A值为0则;
	if(ValueA <= DataPrecision) return 0.0;
	//定义基值变量与中间迭加变量;
	double BaseSum=0.0,InterimX=0.0;
	//利用循环获取所求精度值;
	for(long n=0;n<6;n++)
	{
		InterimX = GetValueOfMonWithNumPower(-1.0,n+2)*
			GetValueOfMonWithNumPower(CirLength,4*n+1)/
			((4*n+1)*GetValueOfMonWithNumPower(2*ValueA*ValueA,2*n)*GetValueOfFactorial(2*n));
		BaseSum = BaseSum + InterimX;
	}
	//返回所求值;
	return BaseSum;
}
//根据传入的回旋参数与曲线长度获取缓和曲线在相对坐标系中的纵距;
double BlockComputerFun::GetAbsYCorOfSprialCir(double ValueA,double CirLength)
{
	//如果A值为0则;
	if(ValueA <= DataPrecision) return 0.0;
	//定义基值变量与中间迭加变量;
	double BaseSum=0.0,InterimY=0.0;
	//利用循环获取所求精度值;	
	for(long n=1;n<7;n++)
	{
		InterimY = GetValueOfMonWithNumPower(-1.0,n+1)*
			GetValueOfMonWithNumPower(CirLength,4*n-1)/
			((4*n-1)*GetValueOfMonWithNumPower(2*ValueA*ValueA,2*n-1)*
			GetValueOfFactorial(2*n-1));				
		BaseSum = BaseSum + InterimY;
	}
	//返回所求值;	
	return BaseSum;		
}

//计算有向直线与X轴的辐角;
double BlockComputerFun::GetAngleOfLineAndXaxes(double StartX,double StartY,double EndX,double EndY)
{
	//定义两个ads_point点变量;
	ads_point SDot,EDot;
	//指定直线的起终点;
	SDot[X]=StartX;
	SDot[Y]=StartY;
	EDot[X]=EndX;
	EDot[Y]=EndY;
	//返回直线与X轴的夹角;
	return ads_angle(SDot,EDot);
}

//根据线路单元上某点的坐标方位角获取线路走向;
int BlockComputerFun::GetItemCurForward(double DotOrientAngle)
{
	//如果给出的角度大于2*PI;
	if (DotOrientAngle > 2*PI + DataPrecision) 
	{
		DotOrientAngle = DotOrientAngle - 2*PI;
	}
	//如果给出的角度小于0;
	if (DotOrientAngle < 0)
	{
		DotOrientAngle = DotOrientAngle + 2*PI;
	}
	//判别点的走向;
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
//由点在有向直线(或切线)的侧向,确定下个线路单元的偏向;
int BlockComputerFun::GetDeflectOfDotToLine(double LineX,double LineY,double LineOrient,
											double SelDotX,double SelDotY)
{
	//定义点到直线的距离变量;
	double Distance = 0;
	//获取距离值(当Distance>0时,点在直线的右下侧,否则在左上侧);
	if (LineOrient == 0||LineOrient == PI||LineOrient == 2*PI) 
	{
		Distance = SelDotX-LineX;
	}
	else
	{
		//直线的方程为:K*(X-X1)+Y1-Y=0;
		double ValueK = tan(PI/2-LineOrient);
		Distance = (ValueK*(SelDotX-LineX)+LineY-SelDotY)/sqrt(ValueK*ValueK+1);
	}
	//获取直线的线路走向;
	int LineForward = GetItemCurForward(LineOrient);
	//判断点在直线或切线的侧向以确定下个线路单元的偏向;
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
//计算缓和曲张的回旋参数A值(从直线上引出);
double BlockComputerFun::GetValueAOfSprial(double StartX,double StartY,double StartOrient,double EndX,double EndY,
										   double &EndR,double &CriLength)
{
	//如果当前是从直线上引出缓和曲线;
	//定义源点变量;
	double OrignX,OrignY,OrignOrient;
	//定义曲线的半径趋势变量;
	int RadForWard = LargeToSmall;
	//获取曲线的偏转;
	int ItemDeflect;
	ItemDeflect = GetDeflectOfDotToLine(StartX,StartY,StartOrient,EndX,EndY);
	//定义返回的A值变量;
	double ValueA;
	BOOL GetValueA;
	GetValueA = GetValueAOfSprial(StartX,StartY,1E50,StartOrient,EndX,EndY,
		EndR,OrignX,OrignY,OrignOrient,CriLength,ValueA,RadForWard,ItemDeflect);
	//OrignX,OrignY,OrignOrient=StartX,StartY,StartOrient;
	if (GetValueA) 
		return ValueA;
	else return 1E50;
}

//计算缓和曲张的回旋参数A值(从圆或缓曲线上引出);
BOOL BlockComputerFun::GetValueAOfSprial(double StartX,double StartY,double StartR,double StartOrient,double EndX,double EndY,
										 double &EndR,double &OrignX,double &OrignY,double &OrignOrient,double &CirLength,double &ValueA,
										 int RadForWard,int Deflect)
{
	//缓和曲线从直线上引出;
	if (RadForWard == LargeToSmall)
	{
		//当前直线单元的终点即为曲线的起点,
		//即LineEndDot=SprialStartDot;
		//曲线的偏向与半径趋势可自动判别出来;		
		//获取起终点连线在起点处的方位角;
		double ValueOrient=ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//定义起终点长度;
		double Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));		
		//根据曲线的左右偏计算偏角;	
		double DeflectAngle = 0;
		if (Deflect == 1) DeflectAngle = ValueOrient - StartOrient;
		else DeflectAngle = StartOrient - ValueOrient;
		//判断偏角的有效性;
		if (DeflectAngle < 0) DeflectAngle+=2*PI;
		if (DeflectAngle > PI/6.0) return FALSE;
		if (DeflectAngle < DataPrecision) return FALSE;
		//以弦长为初始曲长;
		double LengthS = Distance;
		ValueA = LengthS/sqrt(6*DeflectAngle);
		//计算当前横纵距及当前弦长;
		double AbsXCor = GetAbsYCorOfSprialCir(ValueA,LengthS);	
		double AbsYCor = GetAbsXCorOfSprialCir(ValueA,LengthS);
		double AbsDist= sqrt(AbsXCor*AbsXCor+AbsYCor*AbsYCor);	
		//获取曲长的可能且有效迭代范围;
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
		//计算当前横纵距及当前弦长;	
		AbsXCor = GetAbsXCorOfSprialCir(ValueA,CirLength);	
		AbsYCor = GetAbsYCorOfSprialCir(ValueA,CirLength);
		AbsDist = sqrt(AbsXCor*AbsXCor+AbsYCor*AbsYCor);
		//获取曲长的可能且有效迭代范围;
		while (fabs(AbsDist - Distance)>DataPrecision) 
		{
			if (AbsDist > Distance) LengthE = CirLength;
			else LengthS = CirLength;
			CirLength = (LengthS+LengthE)/2;
			ValueA = CirLength/sqrt(6*DeflectAngle);
			//计算当前横纵距及当前弦长;
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
		//判断当前计算结果的合理性即布设段曲线是否在未回旋之前;
		if (EndR < DataPrecision) 
		{
			return FALSE;
		}
		DeflectAngle = ValueA*ValueA/(2*EndR*EndR);
		if (DeflectAngle > PI/2) 
		{
			return FALSE;
		}
		//返回计算得到的A值;
		return TRUE;	
	}
	else if (RadForWard == BigToSmall) 
	{
		//获取起终点连线在起点处的方位角;
		double ValueOrient;
		ValueOrient=ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//获取终点切线的偏转角;
		double DivertAngle = 0;	
		if (Deflect == 1) DivertAngle = ValueOrient - StartOrient;
		else DivertAngle =  StartOrient - ValueOrient;
		//判断偏角的有效性;
		if (DivertAngle < 0) DivertAngle+=2*PI;
		if (DivertAngle > PI/3.0) return FALSE;
		if (DivertAngle < DataPrecision) return FALSE;	
		//设定缓向曲线回旋参数的存在范围;
		double LimValueAS = 0,LimValueAE = 1000000;
		//定义回旋参数变量;
		double InterimA = (LimValueAS+LimValueAE)/2;
		//获取当前实际弦长;
		double Distance;
		Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));
		//获取当前曲线的长度;	
		double InterimLen = sqrt(InterimA*InterimA*InterimA*InterimA/(StartR*StartR)
			+6*InterimA*InterimA*DivertAngle)-InterimA*InterimA/StartR;			
		//获取指定起点的相对坐标;
		double InterSX,InterSY;	
		double ValueLen;
		ValueLen = InterimA*InterimA/StartR;
		InterSX = GetAbsXCorOfSprialCir(InterimA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(InterimA,ValueLen);
		//获取指定终点的相对坐标;
		double InterEX,InterEY;
		ValueLen = InterimA*InterimA/StartR+InterimLen;
		InterEX = GetAbsXCorOfSprialCir(InterimA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(InterimA,ValueLen);
		//获取指定终点的假设长度;
		double AbsDis;
		AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));
		while (fabs(AbsDis-Distance)>DataPrecision)
		{
			//如果假定弦长太大;
			if (AbsDis>Distance) LimValueAE = InterimA;
			else LimValueAS = InterimA;
			//重算相关参数;
			InterimA = (LimValueAS+LimValueAE)/2;
			InterimLen = sqrt(InterimA*InterimA*InterimA*InterimA/(StartR*StartR)
				+6*InterimA*InterimA*DivertAngle)-InterimA*InterimA/StartR;
			//获取指定起点的相对坐标;
			InterSX = GetAbsXCorOfSprialCir(InterimA,InterimA*InterimA/StartR);
			InterSY = GetAbsYCorOfSprialCir(InterimA,InterimA*InterimA/StartR);
			//获取指定终点的相对坐标;
			InterEX = GetAbsXCorOfSprialCir(InterimA,(InterimA*InterimA/StartR+InterimLen));
			InterEY = GetAbsYCorOfSprialCir(InterimA,(InterimA*InterimA/StartR+InterimLen));
			//获取指定终点的假设长度;
			AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));			
		}
		//返回其它参数;
		EndR = InterimA*InterimA/(InterimA*InterimA/StartR+InterimLen);
		CirLength = sqrt(InterimA*InterimA*InterimA*InterimA/(StartR*StartR)
			+6*InterimA*InterimA*DivertAngle)-InterimA*InterimA/StartR;
		//判断当前计算结果的合理性即布设段曲线是否在未回旋之前;
		if (EndR < DataPrecision) 
		{
			return FALSE;
		}
		DivertAngle = ValueA*ValueA/(2*EndR*EndR);
		if (DivertAngle > PI/2) 
		{
			return FALSE;
		}
		//返回所求解的A值;
		ValueA = InterimA;
		//计算源点坐标;
		GetOrignCorOfSprial(StartY,StartX,StartR,StartOrient,ValueA,Deflect,RadForWard,OrignY,OrignX,OrignOrient);
		return TRUE;

	}
	else if (RadForWard == SmallToBig) 
	{
		if (StartR < DataPrecision) 
		{
			return FALSE;
		}
		//获取起终点连线在起点处的方位角;
		double ValueOrient;
		ValueOrient=ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//获取终点切线的偏转角;
		double DivertAngle = 0;
		if (Deflect == 1) DivertAngle = ValueOrient - StartOrient;
		else DivertAngle = StartOrient - ValueOrient;
		//判断偏角的有效性;
		if (DivertAngle < 0) DivertAngle+=2*PI;
		if (DivertAngle > PI/3.0) return FALSE;
		if (DivertAngle < DataPrecision) return FALSE;	
		//设定曲线长度的存在范围;
		//由于后面要保证均方有意义,故l值起始有要求;
		double LimValueAS = sqrt(3*DivertAngle*StartR*StartR)+0.001;		
		double LimValueAE = GetValidLimValueAEnd(DivertAngle,StartR);
		//获取当前实际弦长;
		double Distance;
		Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));
		//由于在有效A范围内计算的AbsDis其下限是已知的,
		//如果当前实际距离比其小则当前A值无解;
		//////////////////////////////////////////////////////////////////////////
		ValueA =  LimValueAS;
		//获取指定起点的相对坐标;
		double InterSX,InterSY;	
		double ValueLen;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的相对坐标;
		double InterEX,InterEY;
		double InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);
		ValueLen = ValueA*ValueA/StartR-InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的假设长度;
		double AbsDisE;
		AbsDisE = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));		
		//如果最大计算曲距比实际距小则当前无解;
		if (AbsDisE < Distance) 
			return FALSE;
		//////////////////////////////////////////////////////////////////////////
		ValueA =  LimValueAE;
		//获取指定起点的相对坐标;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的相对坐标;
		InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);
		ValueLen = ValueA*ValueA/StartR-InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的假设长度;
		double AbsDisS;		
		AbsDisS = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));
		//如果最小计算曲距比实际距大则当前无解;
		if (AbsDisS > Distance) 
			return FALSE;				
		//////////////////////////////////////////////////////////////////////////
		//获取当前曲线的回旋参数值;
		ValueA = (LimValueAS+LimValueAE)/2;
		//计算当前曲长;
		InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);		
		//获取指定起点的相对坐标;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的相对坐标;
		ValueLen = ValueA*ValueA/StartR-InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的假设长度;
		double AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));
		while (fabs(AbsDis-Distance)>DataPrecision)
		{
			//如果假定弦长太大;
			if (AbsDis>Distance) LimValueAS =ValueA;
			else LimValueAE = ValueA;
			//重算相关参数;
			ValueA = (LimValueAS+LimValueAE)/2;
			InterimLen = ValueA*ValueA/StartR-
				sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);					
			//获取指定起点的相对坐标;
			ValueLen = ValueA*ValueA/StartR;
			InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
			InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
			//获取指定终点的相对坐标;
			ValueLen = ValueA*ValueA/StartR - InterimLen;
			InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
			InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
			//获取指定终点的假设长度;
			AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));									
		}
		//返回其它参数;
		EndR = ValueA*ValueA/(ValueA*ValueA/StartR-InterimLen);		
		CirLength = InterimLen;
		//判断当前计算结果的合理性即布设段曲线是否在未回旋之前;
		DivertAngle = ValueA*ValueA/(2*StartR*StartR);
		if (DivertAngle > PI/2)
		{
			return FALSE;
		}
		//计算返回的源点数据;
		GetOrignCorOfSprial(StartY,StartX,StartR,StartOrient,ValueA,Deflect,RadForWard,OrignY,OrignX,OrignOrient);

		return TRUE;
	}
	else if (RadForWard == SmallToLarge)
	{
		//获取起终点连线在起点处的方位角;
		double ValueOrient;
		ValueOrient = ConvertAngleToOrient(GetAngleOfLineAndXaxes(StartX,StartY,EndX,EndY));
		//获取终点切线的偏转角;
		double DivertAngle = 0;
		if (Deflect == 1) DivertAngle = ValueOrient - StartOrient;
		else DivertAngle =  StartOrient - ValueOrient;
		//判断偏角的有效性;
		if (DivertAngle < 0) DivertAngle += 2*PI;
		if (DivertAngle > PI/3.0) return FALSE;
		if (DivertAngle < DataPrecision) return FALSE;
		//由于起点方位角已知,故终点和方位角可知;
		//在当前终点即为源点;
		OrignOrient = StartOrient + Deflect*3*DivertAngle/2;
		//获取当前终点的坐标;
		OrignX = EndX;
		OrignY = EndY;
		//获取当前起点到源点切线的纵距;
		double Distance;
		Distance = sqrt((StartX-EndX)*(StartX-EndX)+(StartY-EndY)*(StartY-EndY));
		//如果两点快重合,不合布设线元的意义;
		if (Distance < 0.1) return FALSE;
		//设置A值的迭代上下限值;
		double LimAS = 0,LimAE = 1000000;
		//////////////////////////////////////////////////////////////////////////
		//获取有效A值下限;
		LimAS = sqrt(Distance*StartR)+0.001;
		//////////////////////////////////////////////////////////////////////////
		ValueA = (LimAS+LimAE)/2;
		//获取曲线长度;
		CirLength =ValueA*ValueA/StartR;
		//计算此时假定了A值时终点到直线的距离;
		double InterX = GetAbsXCorOfSprialCir(ValueA,CirLength);
		double InterY = GetAbsYCorOfSprialCir(ValueA,CirLength);
		//计算假定曲距;
		double AbsDis = sqrt(InterX*InterX+InterY*InterY);
		//如果计算纵距与实际纵距大小精度(1mm)时;
		while (fabs(AbsDis-Distance) > 0.001)
		{
			//如果A值过大;
			if (AbsDis>Distance) LimAE = ValueA;
			else LimAS = ValueA;
			//重新计算A与L以获取新的纵距;			
			ValueA = (LimAS+LimAE)/2;
			CirLength =ValueA*ValueA/StartR;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLength);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLength);
			//计算假定曲距;
			AbsDis = sqrt(InterX*InterX+InterY*InterY);
		}
		//判断当前计算结果的合理性;
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
		//计算返回的源点数据;
		GetOrignCorOfSprial(StartY,StartX,StartR,StartOrient,ValueA,Deflect,RadForWard,OrignY,OrignX,OrignOrient);
		return TRUE;
	}
	else 
		return FALSE;
}
//获取缓曲线的源点坐标;
void BlockComputerFun::GetOrignCorOfSprial(double SprX,double SprY,double SprRad,
										   double SprOrient,double ValueA,int Deflect,int RadForward,
										   double &OrignX,double &OrignY,double &OrignOrient)
{
	//如果当前缓曲线是LargeToSmall或BigToSmall;
	if (RadForward==LargeToSmall || RadForward==BigToSmall) 
	{
		//如果当前给定点即为源点;
		if (SprRad ==1E50)
		{
			OrignX = SprX;
			OrignY = SprY;
			OrignOrient = SprOrient;
		}
		else
		{		
			//定义曲长变量;
			double CirLen = ValueA*ValueA/SprRad;
			//获取曲线源定点的偏角;
			double DivAngle = CirLen/(2*SprRad);
			//获取源点的方位角;
			OrignOrient = SprOrient - Deflect*DivAngle;
			//获取源点的坐标转换角;
			double Angle = ConvertOrientToAngle(OrignOrient);
			//获得给定点的相对坐标;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
			//计算源点坐标;
			OrignX = SprX - InterX*sin(Angle)+Deflect*InterY*cos(Angle);
			OrignY = SprY - InterX*cos(Angle)-Deflect*InterY*sin(Angle);
		}
	}
	//如果当前缓曲线是SmallToLarge或SmallToBig;
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
			//定义曲长变量;
			double CirLen = ValueA*ValueA/SprRad;
			//获取曲线源定点的偏角;
			double DivAngle = CirLen/(2*SprRad);
			//获取源点的方位角;
			OrignOrient = SprOrient + Deflect*DivAngle;
			//获取源点的坐标转换角;
			double Angle = ConvertOrientToAngle(OrignOrient)- Deflect*PI;
			//获得给定点的相对坐标;
			double InterX,InterY;
			InterX = GetAbsXCorOfSprialCir(ValueA,CirLen);
			InterY = GetAbsYCorOfSprialCir(ValueA,CirLen);
			//计算源点坐标;
			OrignX = SprX - InterX*sin(Angle)-/*+*/Deflect*InterY*cos(Angle);
			OrignY = SprY - InterX*cos(Angle)+/*-*/Deflect*InterY*sin(Angle);
		}		
	}

	return ;
}
int BlockComputerFun::GetRadiusForward(double StartR,double EndR)
{
	//如果当前曲线为直线;
	if ((StartR==EndR)&&(StartR == 1E50)) 
		return NoChange;
	//如果当前是从直线上引出完整缓曲线;
	else if ((StartR==1E50)&&(EndR<1E50))
		return LargeToSmall;
	//如果当前是从曲线上引出不完整缓曲线;
	else if ((StartR<1E50)&&(StartR>EndR))
		return BigToSmall;
	//如果当前曲线为圆或弧;
	else if (StartR==EndR)/*R*/ 
		return NoChange;
	//如果当前是从曲线上引出不完整缓曲线;
	else if ((StartR<EndR)&&(EndR<1E50))
		return SmallToBig;
	//如果当前是从完整缓曲线上引出直线;
	else if ((StartR<1E50)&&(EndR==1E50))	
		return SmallToLarge;
	//如果当前曲线为直线;
	else return NoChange;
}

void BlockComputerFun::ShowSpiralCirOnScreen(double StartX,double StartY,double StartR,double StartOrient,
											 double EndR,int ItemDeflect,double ValueA,double CirLen,
											 AcDbObjectId & CreateEntId)
{		
	//定义一个顶点数组及临时点变量;
	AcGePoint2dArray VerticsArray;
	AcGePoint2d Dot;
	//清空顶点数组;
	int ArraySize = VerticsArray.length();
	if (ArraySize > 0)
		VerticsArray.removeSubArray(0,ArraySize-1);
	//将缓和曲线的顶点加入到顶点数组中;
	//利用循环结构初始化顶点数组;
	//定义曲线临时长度;
	double InterLen;
	//定义缓和曲线起点变量;
	RouteDot SDot;
	SDot.DotX = StartX;
	SDot.DotY = StartY;
	SDot.OrientAngle = StartOrient;
	SDot.CurMiles = 0;//此项在动态设计缓曲时没有意义;
	SDot.CurRadius = StartR;
	//定义临时计算结果;
	RouteDot InterDot;
	//设置步长为5米;
	for(InterLen =0;InterLen<CirLen;InterLen+=2.0)
	{
		ComputerCorOfAnyDotOnItem(SDot,InterDot,InterLen,ValueA,
			EndR,SprialItem,ItemDeflect);
		Dot.set(InterDot.DotY,InterDot.DotX);
		VerticsArray.append(Dot);
	}
	//将缓曲线最后一点加入点数组中;	
	ComputerCorOfAnyDotOnItem(SDot,InterDot,CirLen,ValueA,
		EndR,SprialItem,ItemDeflect);
	Dot.set(InterDot.DotY,InterDot.DotX);
	VerticsArray.append(Dot);
	//如果外部传入多义线实体不存在;
	if (CreateEntId == NULL)
	{
		//创建新的多义线实体;
		AcDbPolyline * pPolyline = new AcDbPolyline();
		//将顶点集的点添加为多义线的顶点;
		ArraySize = VerticsArray.length();
		for(int i=0;i<ArraySize;i++)
		{
			pPolyline->addVertexAt(i,VerticsArray[i]);
		}
		//设置多义线属值;
		pPolyline->setColorIndex(1);		
		//将多义线添加到数据库中;
		CreateEntId = AddEntityToDbs(pPolyline);
	}
	else
	{
		AcDbEntity * pEntity = NULL;
		//以写方式打开实体
		/* acdbOpenAcDbEntity(pEntity,CreateEntId,AcDb::kForWrite);*/
		if(acdbOpenObject(pEntity,CreateEntId,AcDb::kForWrite)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		//获取实体指针修改顶点集;
		if(pEntity->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pPolyline;
			pPolyline=AcDbPolyline::cast(pEntity);
			//删除原有线所有的顶点数据;
			int VertCount = pPolyline->numVerts();
			for(int i=0; i<VertCount;i++)
			{
				pPolyline->removeVertexAt(0);
			}
			//将新数据加入到多义线中;
			ArraySize = VerticsArray.length();
			for(int  i=0;i<ArraySize;i++)
			{
				pPolyline->addVertexAt(i,VerticsArray[i]);	
			}
			//设置多义线属值;
			pPolyline->setColorIndex(1);
			//删除线创建基点;
			pPolyline->removeVertexAt(ArraySize);
			//关闭多义线实体;	
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
	//定义点数变量;
	int DotCount = 0;
	//计算按5米步长,当前曲线的点数;
	if (fmod(CirLen,2.0)<DataPrecision)
		DotCount = int(CirLen/2.0);
	else DotCount = int(CirLen/2.0)+1;
	//定义绘制图形时的传值点数组;	
	AcGePoint3d * Point3dArr = new AcGePoint3d[DotCount];
	//利用循环结构初始化点数组;
	int i=0;
	for (double InterLen=0;InterLen<=CirLen;InterLen+=2.0)
	{    
		//如果当前计算长度大于实际长则;
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

	//设置缓曲线的色彩;
	if(icolor<0)
		pWd->subEntityTraits().setColor(5);	
	else 
		pWd->subEntityTraits().setColor(icolor);
	//利用基层图形函数缓制缓曲线;
	pWd->subEntityTraits().setLineWeight(AcDb::kLnWt030);
	pWd->geometry().polyline(DotCount,Point3dArr);

	return  0;
}
double BlockComputerFun::GetValidLimValueAEnd(double DivertAngle,double StartR)
{
	//此式只适应于求缓曲A时的SmallToBig;
	//根据已知条件,求取回旋参数的的下限值(保证求解有意义);
	double LimValueAS = sqrt(3*DivertAngle*StartR*StartR)+0.001;
	//设置回旋参数的上限值;
	double LimValueAE = 1000000;
	//根据AbsDis与L及A的关系可以知道AbsDis发生极值时的L值位于上述之间;
	double ValueA = (LimValueAS+LimValueAE)/2;
	//计算当前曲长值;
	double InterimLen = ValueA*ValueA/StartR-
		sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);;	
	//获取指定起点的相对坐标;
	double ValueLen = ValueA*ValueA/StartR;
	double InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
	double InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
	//获取指定终点的相对坐标;
	ValueLen = ValueA*ValueA/StartR - InterimLen;
	double InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
	double InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
	//获取计算的假设长度;	
	double AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));			
	//定义迭代长度;
	double LenOfPre = 1E50;
	while (fabs(AbsDis-LenOfPre)>DataPrecision) 
	{
		//获取上次的曲长;
		LenOfPre = AbsDis;
		//如果曲距大于曲长;
		if (AbsDis>InterimLen)
			LimValueAE = ValueA;
		else
			LimValueAS = ValueA;
		//重算相关参数;
		ValueA = (LimValueAS+LimValueAE)/2;
		InterimLen = ValueA*ValueA/StartR-
			sqrt(ValueA*ValueA*ValueA*ValueA/(StartR*StartR)-3*DivertAngle*ValueA*ValueA);			
		//获取指定起点的相对坐标;
		ValueLen = ValueA*ValueA/StartR;
		InterSX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterSY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取指定终点的相对坐标;
		ValueLen = ValueA*ValueA/StartR - InterimLen;
		InterEX = GetAbsXCorOfSprialCir(ValueA,ValueLen);
		InterEY = GetAbsYCorOfSprialCir(ValueA,ValueLen);
		//获取计算的假设长度;	
		AbsDis = sqrt((InterEX-InterSX)*(InterEX-InterSX)+(InterEY-InterSY)*(InterEY-InterSY));			
	}
	return ValueA;
}

//根据前一单元为缓曲线,
//求解后接为圆曲线时此圆的圆心点及圆与缓的切点坐标;
double BlockComputerFun::GetCenCorAndSprialDotCor(double SEast,double SNorth,double SOrientAngle,
												  double StartR,double EndR,double ValueA,double ItemDeflect,double CirDeflect,
												  double SelDotX,double SelDotY,double &CenX,double &CenY,double &SprX,double &SprY)
{
	//传入的SEast(Y),SNorth(X),SOrientAngle,StartR,EndR等均取自单元;
	//获取当前缓曲的半径变化趋势;
	int RadForward = GetRadiusForward(StartR,EndR);
	//判别当前缓曲类型,以便确定半径的范围;
	double LimRadS = 0,LimRadE = 0,ReturnRad = 0;
	//如果当前缓曲为LargeToSmall;
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
	//如果当前缓曲类型为SmallToBig或者SmallToLarge;
	else 
	{
		LimRadS = StartR;
		LimRadE = 1E6;
	}
	//根据上述半径的范围确定当前是否存在合适的R值;
	ReturnRad = LimRadS;

	double Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
		StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
		SelDotX,SelDotY,CenX,CenY,SprX,SprY);

	//ads_printf(L"1:returr=%lf dist=%lf\n",ReturnRad,Dist);   
	//如果当前能通过点的半径比迭代下限还小;
	if (ReturnRad > Dist+0.001)
	{
		return 1E50;
	}
	ReturnRad = LimRadE;
	Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
		StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
		SelDotX,SelDotY,CenX,CenY,SprX,SprY);

	//ads_printf(L"2:returr=%lf dist=%lf\n",ReturnRad,Dist);   
	//如果当前通过点的半径比迭代上限还大;
	if (ReturnRad < Dist-0.001)
	{
		return 1E50;
	}
	//假设临时半径值为:
	ReturnRad = (LimRadS+LimRadE)/2;
	//计算此时的圆心到选中点的距离(计算曲径);
	Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
		StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
		SelDotX,SelDotY,CenX,CenY,SprX,SprY);
	//如果计算得到的点距与半径差值大于精度;
	while (fabs(Dist-ReturnRad) > DataPrecision)
	{
		//如果是半径取得较大;
		if (ReturnRad > Dist)
			LimRadE = ReturnRad;
		else LimRadS = ReturnRad;
		//重算相关数据;
		ReturnRad = (LimRadS+LimRadE)/2;
		Dist = GetDistOfCenterAndSelDot(SEast,SNorth,SOrientAngle,
			StartR,EndR,ValueA,ItemDeflect,CirDeflect,ReturnRad,
			SelDotX,SelDotY,CenX,CenY,SprX,SprY);				
	}
	//返回所求得的半径值;
	return ReturnRad;
}

double BlockComputerFun::GetDistOfCenterAndSelDot(double SEast,double SNorth,double SOrientAngle,
												  double StartR,double EndR,double ValueA,double ItemDeflect,double CirDeflect,double InputRad,
												  double SelDotX,double SelDotY,double &CenX,double &CenY,double &SprX,double &SprY)
{
	//传入的SEast,SNorth,SOrientAngle,StartR,EndR等均取自单元;
	//传入的半径值须有效;
	//获取当前缓曲的半径变化趋势;
	int RadForward = GetRadiusForward(StartR,EndR);
	//根据当前缓曲线类型求解此缓曲线的源点坐标;
	double OrignX,OrignY,OrignOrient;
	GetOrignCorOfSprial(SEast,SNorth,StartR,
		SOrientAngle,ValueA,ItemDeflect,RadForward,
		OrignY,OrignX,OrignOrient);
	//此时获得的源点坐标为笛卡尔坐标系中值(圆心与切点值也均为笛氏坐标值);
	//在缓曲线上对应半径处的方位角为;
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
	//计算缓曲线上对应长度处的坐标;
	RouteDot StartDot,ReturnDot;
	StartDot.DotX = SEast;
	StartDot.DotY = SNorth;
	StartDot.CurRadius = StartR;
	StartDot.OrientAngle = SOrientAngle;
	StartDot.CurMiles = 0;
	//计算当前曲线起点到对应长度点的曲长;
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
	//获取圆的偏向;
	//int CircleDef = GetDeflectOfDotToLine(SprX,SprY,InterOrient,SelDotX,SelDotY);
	//计算当前缓曲线上对应长度处点所确定的圆心坐标;
	//点到圆心的方位角;
	double OrientOfDotAndCen = InterOrient + CirDeflect*PI/2;
	//判断方位角的有效性;
	if (OrientOfDotAndCen > 2*PI) OrientOfDotAndCen = OrientOfDotAndCen - 2*PI;
	if (OrientOfDotAndCen < 0) OrientOfDotAndCen = OrientOfDotAndCen + 2*PI;
	CenX = SprX + InputRad*sin(OrientOfDotAndCen);
	CenY = SprY + InputRad*cos(OrientOfDotAndCen);
	//计算当前圆心到被选中点的距离;
	double Dist = sqrt((CenX-SelDotX)*(CenX-SelDotX)+(CenY-SelDotY)*(CenY-SelDotY));

	return Dist;
}

double BlockComputerFun::GetRadiusByStartDotAndSelDot(double StartX,double StartY,double StartOrient,double CurDeflect,
													  double SelDotX,double SelDotY,double &GetCenX,double &GetCenY)
{
	//根据当前将要布设圆的弧偏向获取起点到圆心的方位角;
	double OrientOfSC;
	OrientOfSC = StartOrient + CurDeflect*PI/2 +2*PI;
	//获取起点与必过点的垂直平分线参数;
	double MidX,MidY,OrientOfMid;
	MidX = (StartX+SelDotX)/2;
	MidY = (StartY+SelDotY)/2;
	double AngleOfSS = GetAngleOfLineAndXaxes(StartX,StartY,SelDotX,SelDotY);
	OrientOfMid = ConvertAngleToOrient(AngleOfSS) + CurDeflect*PI/2 +2*PI;
	//根据上述所求的两方位角,求取对应的直线系的斜率;
	double ValueKSC,ValueKMid;
	ValueKSC = GetValueKByOrient(OrientOfSC*180/PI);
	ValueKMid = GetValueKByOrient(OrientOfMid*180/PI);
	//求取两直线的交点即圆心坐标;
	GetCrossDotOfLine(ValueKSC,StartX,StartY,ValueKMid,MidX,MidY,GetCenX,GetCenY);
	//返回圆的半径;
	double Radius = sqrt((StartX-GetCenX)*(StartX-GetCenX)+(StartY-GetCenY)*(StartY-GetCenY));

	if (Radius < DataPrecision) return 1E50;
	else return Radius;
}
//根据一条切线与圆的两必过点求解此圆的直径与圆心;
double BlockComputerFun::GetRadiusOfLineAndTwoDot(double LineX,double LineY,double LineOrient,double FixX,double FixY,
												  double MoveX,double MoveY,double &CenX,double &CenY,double &TanX,double &TanY,BOOL BackOrnot)
{
	//定义返回值;
	double ReturnRad = 0;
	//获取切线与切线垂线的斜率;
	double ValueK,ValueVetK;
	ValueK = GetValueKByOrient(LineOrient*180/PI);
	if (ValueK == 0) ValueVetK = 1E50;
	else if (ValueK == 1E50) ValueVetK = 0;
	else ValueVetK = -1/ValueK;
	//如果点与直线相距太近;
	double Dist1 = GetDistOfLineAndSelDot(ValueK,LineX,LineY,FixX,FixY);
	double Dist2 = GetDistOfLineAndSelDot(ValueK,LineX,LineY,MoveX,MoveY);
	if(fabs(Dist2)<DataPrecision&&fabs(Dist1)<DataPrecision) return 1E50;
	//如果两点相重;
	if (sqrt((FixX-MoveX)*(FixX-MoveX)+(FixY-MoveY)*(FixY-MoveY))<DataPrecision) 
		return 1E50;
	//判断圆的偏向;
	int CirDeflect = GetDeflectOfDotToLine(LineX,LineY,LineOrient,FixX,FixY);
	int Deflect = GetDeflectOfDotToLine(LineX,LineY,LineOrient,MoveX,MoveY);
	if (fabs(Dist1)<DataPrecision)
	{
		TanX = FixX;
		TanY = FixY;
		ReturnRad = GetRadiusByStartDotAndSelDot(FixX,FixY,LineOrient,Deflect,MoveX,MoveY,CenX,CenY);		
		return ReturnRad;
	}
	//如果两点不共边,当前布设不能成功;
	if (CirDeflect != Deflect) return 1E50;
	//获取弦垂直平分线的要素;
	double MidX,MidY,MidOrient,ValueMidK;
	MidX = (FixX+MoveX)/2;
	MidY = (FixY+MoveY)/2;
	MidOrient = ConvertAngleToOrient(
		GetAngleOfLineAndXaxes(FixX,FixY,MoveX,MoveY)) + Deflect*PI/2;
	ValueMidK = GetValueKByOrient(MidOrient*180/PI);
	//如果切线与弦垂直平分线平行则;
	if (fabs(ValueK-ValueMidK)<DataPrecision) return 1E50;
	//当切线与横轴平行时,此时弦垂直平分线不与横轴平行;
	if (fabs(ValueK) <DataPrecision) 
	{
		//此时垂足的TanX值与CenX相等,CenY-TanY=Radius;
		//如果弦垂直平分线与横轴垂直;
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
			//定义方程系数;
			double FacA,FacB,FacC;
			FacA = 1;
			FacB = -2*(FixX+ValueMidK*(FixY-LineY));
			FacC = FixX*FixX+FixY*FixY-LineY*LineY-2*(MidY-ValueMidK*MidX)*(FixY-LineY);			
			double ValueBeita = FacB*FacB-4*FacA*FacC;
			//如果ValueBeita小于0方程无解;
			double InterCenX1,InterCenY1,InterCenX2,InterCenY2,Radius1,Radius2;
			if (ValueBeita < 0) return 1E50;
			else
			{
				//获得两个圆心值;
				InterCenX1 = (-FacB+sqrt(ValueBeita))/(2*FacA);
				InterCenY1 = ValueMidK*(InterCenX1-MidX)+MidY;
				Radius1=sqrt((InterCenX1-FixX)*(InterCenX1-FixX)+(InterCenY1-FixY)*(InterCenY1-FixY));
				InterCenX2 = (-FacB-sqrt(ValueBeita))/(2*FacA);
				InterCenY2 = ValueMidK*(InterCenX2-MidX)+MidY;
				Radius2=sqrt((InterCenX2-FixX)*(InterCenX2-FixX)+(InterCenY2-FixY)*(InterCenY2-FixY));
				//如果当前采用大半径布设圆曲;
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
		//此时垂足的TanY值与CenY相等,CenX-TanX=Radius;
		//如果弦垂直平分线与横轴平行;
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
			//定义方程系数;
			double FacA,FacB,FacC;
			FacA = ValueMidK*ValueMidK;
			FacB = 2*(LineX-FixX+ValueMidK*(MidY-ValueMidK*MidX-FixY));
			FacC = FixX*FixX-LineX*LineX+(MidY-ValueMidK*MidX-FixY)*(MidY-ValueMidK*MidX-FixY);			
			double ValueBeita = FacB*FacB-4*FacA*FacC;
			//如果ValueBeita小于0方程无解;
			double InterCenX1,InterCenY1,InterCenX2,InterCenY2,Radius1,Radius2;
			if (ValueBeita < 0) return 1E50;
			else
			{
				//获得两个圆心值;
				InterCenX1 = (-FacB+sqrt(ValueBeita))/(2*FacA);
				InterCenY1 = ValueMidK*(InterCenX1-MidX)+MidY;
				Radius1 = sqrt((InterCenX1-FixX)*(InterCenX1-FixX)+(InterCenY1-FixY)*(InterCenY1-FixY));
				InterCenX2 = (-FacB-sqrt(ValueBeita))/(2*FacA);
				InterCenY2 = ValueMidK*(InterCenX2-MidX)+MidY;
				Radius2 = sqrt((InterCenX2-FixX)*(InterCenX2-FixX)+(InterCenY2-FixY)*(InterCenY2-FixY));
				//如果当前采用大半径布设圆曲;
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
		//切线与弦所在直线的交点;
		double ValueKother = GetValueKByTwoPoint(FixX,FixY,MoveX,MoveY);
		GetCrossDotOfLine(ValueK,LineX,LineY,ValueKother,FixX,FixY,CroTanX,CroTanY);
		//获取弦中垂线与切线的交点;
		GetCrossDotOfLine(ValueK,LineX,LineY,ValueMidK,MidX,MidY,CroMidX,CroMidY);
		//计算三角形的三边大小;
		double SideC = sqrt((CroTanX-CroMidX)*(CroTanX-CroMidX)+
			(CroTanY-CroMidY)*(CroTanY-CroMidY));
		double SideB = sqrt((MidX-CroMidX)*(MidX-CroMidX)+
			(MidY-CroMidY)*(MidY-CroMidY));
		double SideA = sqrt((MidX-CroTanX)*(MidX-CroTanX)+
			(MidY-CroTanY)*(MidY-CroTanY));
		double PartA = sqrt((MidX-FixX)*(MidX-FixX)+
			(MidY-FixY)*(MidY-FixY));
		//计算圆的R值;
		if (SideC<DataPrecision||
			SideB<DataPrecision||
			SideA<DataPrecision||
			PartA<DataPrecision) return 1E50;
		double FacA,FacB,FacC;		
		FacA = SideC*SideC/(SideA*SideA)-1;
		FacB = -2*SideB*SideC/SideA;
		FacC = SideB*SideB+PartA*PartA;
		double ValueBerta = FacB*FacB-4*FacA*FacC;
		//如果ValueBerta方程无解;
		if (ValueBerta < DataPrecision) return 1E50;
		else
		{
			//计算中垂点到直线的距离;
			double DistOfVet = fabs(GetDistOfLineAndSelDot(ValueK,LineX,LineY,MidX,MidY));
			//如果当前采用大半径布设圆曲;
			if (!BackOrnot)
			{
				ReturnRad = (-FacB+sqrt(ValueBerta))/(2*FacA);	
				//当前半径布线时有可无解;
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
				//当前半径布线时有可无解;
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

	//根据偏向左侧还是右侧及偏距计算当前起点位置(ValueSide左为-1右为1);
	StartX = StartX + OffsetDist*sin(ValueSide*PI/2+StartOrient);
	StartY = StartY + OffsetDist*cos(ValueSide*PI/2+StartOrient);
	//根据引线方向左偏还是右偏计算引线的方位角;
	StartOrient = StartOrient + ValueDef*ValueAngle*PI/180;
}

void BlockComputerFun::BackTonotModifidOfSDot(double &StartX,double &StartY,double ValueSide,double OffsetDist,
											  double &StartOrient,double ValueDef,double ValueAngle)
{
	//根据引线方向左偏还是右偏计算原引线的方位角;
	StartOrient = StartOrient - ValueDef*ValueAngle*PI/180;
	//根据偏向左侧还是右侧及偏距计算原起点位置(ValueSide左为-1右为1);
	StartX = StartX + OffsetDist*sin(-1*ValueSide*PI/2+StartOrient);
	StartY = StartY + OffsetDist*cos(-1*ValueSide*PI/2+StartOrient);	
}

BOOL BlockComputerFun::GetComItemFromSelEntity(AcDbObjectId SelEntId,struct RampItem & ComItem,
											   double CurDef,BOOL FreeOrnot)
{
	BOOL ContinueOrnot = FALSE;
	//激活CAD文档;
	acedGetAcadDwgView()->SetFocus();
	if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		return FALSE;
	}
	//打开所获取的实体;
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelEntId,AcDb::kForRead)!=Acad::eOk)
	{
		return FALSE;
	}
	if (pEntity == NULL) 
	{
		return FALSE;
	}
	//如果选中的是直线;
	if (_tcscmp(pEntity->isA()->name(),L"AcDbLine")==0)
	{
		//获取直线的起终点;
		AcDbLine * pLine = AcDbLine::cast(pEntity);
		double LineSX,LineSY,LineEX,LineEY;
		LineSX= (pLine->startPoint())[X];
		LineSY= (pLine->startPoint())[Y];
		LineEX= (pLine->endPoint())[X];
		LineEY= (pLine->endPoint())[Y];
		pEntity->close();
		//初始化计算线元;
		double PreItemDeflect = 1;
		if (fabs(CurDef) > DataPrecision) 
		{
			PreItemDeflect = CurDef;
		}
		//else
		//{		
		//提示用户指定此时线路的走向(偏向:顺时针-右还是逆时针-左)			
		//	if (acedGetReal(L"\n请指明线路走向(顺时针-右1,逆时针-左-1):\n",&PreItemDeflect) == RTCAN)
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
		//获取直线的属性数据;
		ComItem.ItemType = 1.0;             //普通CAD直线;		
		ComItem.ItemDef = PreItemDeflect;   //NoneDeflect,故将其记录成线路走向;	
	}
	else if (_tcscmp(pEntity->isA()->name(),L"AcDbArc")==0) 
	{
		//打开选中的圆弧实体,获取圆曲单元的起点元素;
		AcDbArc * pGotEntArc = AcDbArc::cast(pEntity); 
		XYarray xray;
		int res=GetXdataFromEntity(SelEntId,xray);
		if(res==0)//普通cad实体
		{
			if (FreeOrnot) //如果起点拖动
			{			
				//提示用户指定此时线路的走向(偏向:顺时针-右还是逆时针-左)
				double PreItemDeflect = 1;
				//	if (acedGetReal(L"\n请指明线路走向(顺时针-右1,逆时针-左-1):\n",&PreItemDeflect) == RTCAN)
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
				//获取圆心坐标;
				ComItem.FDotX = (pGotEntArc->center())[X];
				ComItem.FDotY = (pGotEntArc->center())[Y];					
				ComItem.ItemDef = PreItemDeflect;
			}
			else
			{
				//提示用户指定此时线路的走向(偏向:顺时针-右还是逆时针-左)
				if (fabs(CurDef) < DataPrecision) 
				{
					if (acedGetReal(L"\n请指明线路走向(顺时针-右1,逆时针-左-1):\n",&CurDef) == RTCAN)
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
				if (fabs(CurDef+1)<DataPrecision)//逆时针
				{
					ComItem.FDotX = (pGotEntArc->center())[X] + pGotEntArc->radius()*sin(ConvertAngleToOrient(pGotEntArc->endAngle()));
					ComItem.FDotY = (pGotEntArc->center())[Y] + pGotEntArc->radius()*cos(ConvertAngleToOrient(pGotEntArc->endAngle()));				
					ComItem.FDotOrient = ConvertAngleToOrient(pGotEntArc->endAngle()+PI/2);
				}
				else//顺时针
				{
					ComItem.FDotX = (pGotEntArc->center())[X] + pGotEntArc->radius()*sin(ConvertAngleToOrient(pGotEntArc->startAngle()));
					ComItem.FDotY = (pGotEntArc->center())[Y] + pGotEntArc->radius()*cos(ConvertAngleToOrient(pGotEntArc->startAngle()));				
					ComItem.FDotOrient = ConvertAngleToOrient(pGotEntArc->startAngle()-PI/2);				
				}
				ComItem.ItemDef = CurDef;
			}
			//获取单元参数;
			ComItem.ItemType = 2.0;                //普通CAD圆弧;
			ComItem.ItemPro = pGotEntArc->radius();//R;
		}
		else
		{
			/////////////////////////////////////////////////////////////////////////////////////////////////
			//获取所对应圆弧单元的圆心坐标;
			ComItem.FDotX = xray.xyarr[9]+
				xray.xyarr[1]*sin(xray.xyarr[5]+xray.xyarr[4]*PI/2);
			ComItem.FDotY = xray.xyarr[8]+
				xray.xyarr[1]*cos(xray.xyarr[5]+xray.xyarr[4]*PI/2);
			if (!FreeOrnot) 
			{
				//获取曲线终点数据;
				ComItem.FDotOrient = xray.xyarr[5]+xray.xyarr[4]*
					xray.xyarr[2]/xray.xyarr[1];
				ComItem.FDotX = ComItem.FDotX+
					xray.xyarr[1]*sin(ComItem.FDotOrient-xray.xyarr[4]*PI/2);
				ComItem.FDotY = ComItem.FDotY+
					xray.xyarr[1]*cos(ComItem.FDotOrient-xray.xyarr[4]*PI/2);				
			}
			//获取单元参数;
			ComItem.ItemType = 32.0;//匝道中的圆弧线元;
			ComItem.ItemPro = xray.xyarr[1];
			ComItem.ItemDef = xray.xyarr[4];
		}
		pEntity->close();		
	}
	else if (_tcscmp(pEntity->isA()->name(),L"AcDbPolyline")==0) //缓和曲线
	{
		XYarray xray;
		int res=GetXdataFromEntity(SelEntId,xray);
		if(res==0)
		{
			ads_printf(L"选中实体无效!\n");
			pEntity->close();
			return FALSE;
		}
		//获取曲线起点的数据;		
		ComItem.FDotX = xray.xyarr[9];  //->SNorthCor;
		ComItem.FDotY = xray.xyarr[8];  //->SEastCor;
		ComItem.FDotOrient = xray.xyarr[5];
		//获取单元参数;
		ComItem.ItemPro = xray.xyarr[1];//A;
		ComItem.ItemDef = xray.xyarr[4];	
		//设置起终点半径;
		if (fabs( xray.xyarr[0]-3) < DataPrecision)
		{
			ComItem.ItemType = 33.0;//匝道中的前缓线元;			
			ComItem.ItemStartR = 1E50;
			ComItem.ItemEndR = xray.xyarr[2];
			ComItem.RadForWard = LargeToSmall*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemEndR;
		}
		else if (fabs(xray.xyarr[0]-4) < DataPrecision)
		{
			ComItem.ItemType = 34.0;//匝道中的后缓线元;
			ComItem.ItemStartR  = xray.xyarr[2];
			ComItem.ItemEndR = 1E50;
			ComItem.RadForWard = SmallToLarge*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemStartR;
		}
		else if (fabs(xray.xyarr[0]-5) < DataPrecision)
		{
			ComItem.ItemType = 35.0;//匝道中的BS不完整缓线元;
			ComItem.ItemStartR  = xray.xyarr[2];
			ComItem.ItemEndR = xray.xyarr[3];
			ComItem.RadForWard = BigToSmall*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemEndR-1/ComItem.ItemStartR);
		}
		else
		{
			ComItem.ItemType = 36.0;//匝道中的SB不完整缓线元;		
			ComItem.ItemStartR  = xray.xyarr[2];
			ComItem.ItemEndR = xray.xyarr[3];
			ComItem.RadForWard = SmallToBig*1.0; 
			ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemStartR-1/ComItem.ItemEndR);
		}
		if (!FreeOrnot) 
		{
			//定义两传值点;
			RouteDot StaDot,EndDot;
			StaDot.DotY = ComItem.FDotX;
			StaDot.DotX = ComItem.FDotY;
			StaDot.CurRadius = ComItem.ItemStartR;
			StaDot.OrientAngle = ComItem.FDotOrient;
			//计算缓曲线的终点
			ComputerCorOfAnyDotOnItem(StaDot,EndDot,ComItem.ItemLen,ComItem.ItemPro,ComItem.ItemEndR,SprialItem,ComItem.ItemDef);
			//重设特征点值;
			ComItem.FDotX = EndDot.DotY;
			ComItem.FDotY = EndDot.DotX;
			//EndDot.CurRadius = ComItem.ItemEndR;
			ComItem.FDotOrient = EndDot.OrientAngle;
		}
		pEntity->close();

	}
	else 
	{

		ads_printf(L"选中实体无效!\n");
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
//激活CAD文档;
acedGetAcadDwgView()->SetFocus();
if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
{
return FALSE;
}
//打开所获取的实体;
AcDbEntity * pEntity = NULL;
if (acdbOpenObject(pEntity,SelEntId,AcDb::kForRead)!=Acad::eOk)
{
return FALSE;
}
if (pEntity == NULL) 
{
return FALSE;
}
//如果选中的是直线;
if (_tcscmp(pEntity->isA()->name(),L"AcDbLine")==0)
{
//获取直线的起终点;
AcDbLine * pLine = AcDbLine::cast(pEntity);
double LineSX,LineSY,LineEX,LineEY;
LineSX= (pLine->startPoint())[X];
LineSY= (pLine->startPoint())[Y];
LineEX= (pLine->endPoint())[X];
LineEY= (pLine->endPoint())[Y];
pEntity->close();
//初始化计算线元;
double PreItemDeflect = 1;
if (fabs(CurDef) > DataPrecision) 
{
PreItemDeflect = CurDef;
}
//else
//{		
//提示用户指定此时线路的走向(偏向:顺时针-右还是逆时针-左)			
//	if (acedGetReal(L"\n请指明线路走向(顺时针-右1,逆时针-左-1):\n",&PreItemDeflect) == RTCAN)
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
//获取直线的属性数据;
ComItem.ItemType = 1.0;             //普通CAD直线;		
ComItem.ItemDef = PreItemDeflect;   //NoneDeflect,故将其记录成线路走向;	
}
else if (_tcscmp(pEntity->isA()->name(),L"AcDbArc")==0) 
{
//打开选中的圆弧实体,获取圆曲单元的起点元素;
AcDbArc * pGotEntArc = AcDbArc::cast(pEntity); 
if (FreeOrnot) 
{			
//提示用户指定此时线路的走向(偏向:顺时针-右还是逆时针-左)
double PreItemDeflect = 1;
//	if (acedGetReal(L"\n请指明线路走向(顺时针-右1,逆时针-左-1):\n",&PreItemDeflect) == RTCAN)
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
//获取圆心坐标;
ComItem.FDotX = (pGotEntArc->center())[X];
ComItem.FDotY = (pGotEntArc->center())[Y];					
ComItem.ItemDef = PreItemDeflect;
}
else
{
//提示用户指定此时线路的走向(偏向:顺时针-右还是逆时针-左)
if (fabs(CurDef) < DataPrecision) 
{
if (acedGetReal(L"\n请指明线路走向(顺时针-右1,逆时针-左-1):\n",&CurDef) == RTCAN)
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
//获取单元参数;
ComItem.ItemType = 2.0;                //普通CAD圆弧;
ComItem.ItemPro = pGotEntArc->radius();//R;	
pEntity->close();		
}
else if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER")==0) 
{
//获得当前所选匝道实体的最后一个有效线元类型;
JD_CENTER * pGotRamp = JD_CENTER::cast(pEntity);
double ValidItemType = pGotRamp->XYArray[pGotRamp->XYNum-1][0];
//如果线元为直线;
if (fabs(ValidItemType-1) < DataPrecision)
{			
//获得直线终点坐标(计算单元取的特征点坐标为线元终点坐标,而方位角与里程取起点处数据);
ComItem.FDotX = pGotRamp->XYArray[pGotRamp->XYNum-1][9]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*sin(pGotRamp->XYArray[pGotRamp->XYNum-1][5]);
ComItem.FDotY = pGotRamp->XYArray[pGotRamp->XYNum-1][8]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*cos(pGotRamp->XYArray[pGotRamp->XYNum-1][5]);
ComItem.FDotOrient = pGotRamp->XYArray[pGotRamp->XYNum-1][5];
//设置单元属性;
ComItem.ItemType = 31.0;//匝道中的直线线元;
pEntity->close();
}
else if(fabs(ValidItemType-2) < DataPrecision)
{
//获取所对应圆弧单元的圆心坐标;
ComItem.FDotX = pGotRamp->XYArray[pGotRamp->XYNum-1][9]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*sin(pGotRamp->XYArray[pGotRamp->XYNum-1][5]+pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);
ComItem.FDotY = pGotRamp->XYArray[pGotRamp->XYNum-1][8]+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*cos(pGotRamp->XYArray[pGotRamp->XYNum-1][5]+pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);
if (!FreeOrnot) 
{
//获取曲线终点数据;
ComItem.FDotOrient = pGotRamp->XYArray[pGotRamp->XYNum-1][5]+pGotRamp->XYArray[pGotRamp->XYNum-1][4]*
pGotRamp->XYArray[pGotRamp->XYNum-1][2]/pGotRamp->XYArray[pGotRamp->XYNum-1][1];
ComItem.FDotX = ComItem.FDotX+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*sin(ComItem.FDotOrient-pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);
ComItem.FDotY = ComItem.FDotY+
pGotRamp->XYArray[pGotRamp->XYNum-1][1]*cos(ComItem.FDotOrient-pGotRamp->XYArray[pGotRamp->XYNum-1][4]*PI/2);				
}
//获取单元参数;
ComItem.ItemType = 32.0;//匝道中的圆弧线元;
ComItem.ItemPro = pGotRamp->XYArray[pGotRamp->XYNum-1][1];
ComItem.ItemDef = pGotRamp->XYArray[pGotRamp->XYNum-1][4];
pEntity->close();
}
else //ValidItemType=3,4,5,6;
{
//获取曲线起点的数据;		
ComItem.FDotX = pGotRamp->XYArray[pGotRamp->XYNum-1][9];  //->SNorthCor;
ComItem.FDotY = pGotRamp->XYArray[pGotRamp->XYNum-1][8];  //->SEastCor;
ComItem.FDotOrient = pGotRamp->XYArray[pGotRamp->XYNum-1][5];
//获取单元参数;
ComItem.ItemPro = pGotRamp->XYArray[pGotRamp->XYNum-1][1];//A;
ComItem.ItemDef = pGotRamp->XYArray[pGotRamp->XYNum-1][4];	
//设置起终点半径;
if (fabs(ValidItemType-3) < DataPrecision)
{
ComItem.ItemType = 33.0;//匝道中的前缓线元;			
ComItem.ItemStartR = 1E50;
ComItem.ItemEndR = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.RadForWard = LargeToSmall*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemEndR;
}
else if (fabs(ValidItemType-4) < DataPrecision)
{
ComItem.ItemType = 34.0;//匝道中的后缓线元;
ComItem.ItemStartR  = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.ItemEndR = 1E50;
ComItem.RadForWard = SmallToLarge*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro/ComItem.ItemStartR;
}
else if (fabs(ValidItemType-5) < DataPrecision)
{
ComItem.ItemType = 35.0;//匝道中的BS不完整缓线元;
ComItem.ItemStartR  = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.ItemEndR = pGotRamp->XYArray[pGotRamp->XYNum-1][3];
ComItem.RadForWard = BigToSmall*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemEndR-1/ComItem.ItemStartR);
}
else
{
ComItem.ItemType = 36.0;//匝道中的SB不完整缓线元;		
ComItem.ItemStartR  = pGotRamp->XYArray[pGotRamp->XYNum-1][2];
ComItem.ItemEndR = pGotRamp->XYArray[pGotRamp->XYNum-1][3];
ComItem.RadForWard = SmallToBig*1.0; 
ComItem.ItemLen =  ComItem.ItemPro*ComItem.ItemPro*(1/ComItem.ItemStartR-1/ComItem.ItemEndR);
}
if (!FreeOrnot) 
{
//定义两传值点;
RouteDot StaDot,EndDot;
StaDot.DotY = ComItem.FDotX;
StaDot.DotX = ComItem.FDotY;
StaDot.CurRadius = ComItem.ItemStartR;
StaDot.OrientAngle = ComItem.FDotOrient;
//计算缓曲线的终点
ComputerCorOfAnyDotOnItem(StaDot,EndDot,ComItem.ItemLen,ComItem.ItemPro,ComItem.ItemEndR,SprialItem,ComItem.ItemDef);
//重设特征点值;
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
ads_printf(L"选中实体无效!\n");
pEntity->close();
return FALSE;
}
return TRUE;
}*/


void BlockComputerFun::GetOtherParameterOfArc(double StartX,double StartY,double StartOA,double ArcRad,double ArcLen,double ArcDef,
											  double &ArcCenX,double &ArcCenY,double & ArcEndX,double &ArcEndY)
{
	//根据圆弧点,偏向,及半径确定圆心坐标;
	ArcCenX = StartX + ArcRad*sin(StartOA + ArcDef*PI/2);
	ArcCenY = StartY + ArcRad*cos(StartOA + ArcDef*PI/2);
	//根据曲长算得终点处的方位角;
	double EndOA = StartOA + ArcDef*ArcLen/ArcRad;
	//获取终点坐标变量;
	ArcEndX = ArcCenX + ArcRad*sin(EndOA - ArcDef*PI/2);
	ArcEndY = ArcCenY + ArcRad*cos(EndOA - ArcDef*PI/2);
}

double BlockComputerFun::ChangeDataPrecision(double InputData,int PreLength)
{
	//定义精度字串;
	CString InterString;
	//将数据导入字串形式;
	InterString.Format(L"%lf",InputData);
	//获得小数点的位置;
	int PosPoint = InterString.Find(L".") + 1;
	//按精度截取小数位;
	InterString = InterString.Left(PosPoint+PreLength);
	//返回符合精度要求的数据;
	return _wtof(InterString);
}

// 根据线路实体ID及其附近一点求取此点对应的线路投影里程及投影里程处的线路坐标点(传出坐标为大地坐标)
void BlockComputerFun::ComputerMilesAndCorsOfRoute(AcDbObjectId InputRouteId,
												   ads_point nearPoint,double &ItemType,double &ReturnProMils,xlpoint &RoutePoint)
{
	// 激活锁定CAD文档;
	acedGetAcadDwgView()->SetFocus();
	if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return ;
	}		
	// 打开所获取的实体
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,InputRouteId,AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"打开实体失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return ;
	}
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER") != 0) 
	{
		acutPrintf(L"所选实体不是线路实体!\n");
		pEntity->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		return ;
	}
	JD_CENTER *pGottnRamp = JD_CENTER::cast(pEntity);
	// 获得投影点处线路统一里程
	ReturnProMils = pGottnRamp->PROJ_ML(nearPoint[Y], nearPoint[X]);
	// 获得此点对应单元的类型
	int ItemNumber = pGottnRamp->GetXYNoFromTYLC(ReturnProMils);
	ItemType = pGottnRamp->XYArray[ItemNumber][0];
	// 获得统一里程的线路点
	RoutePoint.lc = ReturnProMils;	
	pGottnRamp->xlpoint_pz(&RoutePoint);
	pEntity->close();
	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());
	return ;
}