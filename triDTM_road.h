// triDTM.h: interface for the triDTM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIDTM_road_H__3F2F6E3F_1CBD_46E9_9A8D_DB8441C8FD2A__INCLUDED_)
#define AFX_TRIDTM_road_H__3F2F6E3F_1CBD_46E9_9A8D_DB8441C8FD2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include  "stdafx.h"
#include <io.h>
#include <process.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define X 0
//#define Y 1 
//#define Z 2
#define FindOppTri(j,ajtno,trino)  j=0;\
	      if(ajtno!=-1)\
		 {\
	       while(j<3 && trino!=tarray[ajtno].Ajt[j])\
	    	j++;\
		}\


struct EdgeNode {
	             double pt[3];
				 struct EdgeNode *next;
   };


class triDTM_road  
{

public:
	int PTSUM;
    struct PT{
 	double pt[3];
	int cur;
	char att;
	int trino;
   } *parray;

	bool IsDtm;//Dtm是否存在
	double DefaultH;//缺省高程
	double LeftLowPt[2],RightUpPt[2];
 
protected:
	 HANDLE mapfileHandleP,MEMmapfileHandleP,mapfileHandleT,MEMmapfileHandleT,mapfileHandleG,MEMmapfileHandleG;
     LPVOID FilePtrP,FilePtrT,FilePtrG;    

     int NCol,NRow;
     
     int GridWidth;
     struct GridNode{
	      int Pfcur;//第一个落在该网格的点
		  int Ptcur;//最后落在该网格的点
		  int Pncur;//当前网格的待插点号
          int PtSum;//当前网格的总点数
	 } *GridArray;

    struct Triangle{
	     int vertex[3];
	     int Ajt[3];//*prior,*next;
	     char EdgeInf[3];
         char Attribute;//属性：代表三角形是否有效,'Y'有效;'N'无效
	}  *tarray; 

  
 
public:
	int DTMHasRead;
	triDTM_road();
	virtual ~triDTM_road();

	int Read_Dtm();//读数模
	double interpolate(double pt[2]);//传入某点X,Y坐标，返回Z

	struct EdgeNode * FindInterPt(double pt1[3], double pt2[3],int &Pnum);//传入两点定出直线，求出直线与三角网边的交点X,Y,Z链表
    
    void ReleaseDtm();//释放数模

protected://请不要修改
   int LocatetrlNew(double pt[3]);
   double chkd(double pt11[3],double pt22[3],double pt33[3]);
   int sign(double result);
   double CCW2(double pt1[2],double pt2[2],double pt3[2]); 
   double interpt(double pt[3],int l);
   int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
   int CheckPtInTri(int trino, double pt[2]);
   int FindFirstNoEmptyGridNo(int crow,int ccol);

};

#endif // !defined(AFX_TRIDTM_H__3F2F6E3F_1CBD_46E9_9A8D_DB8441C8FD2A__INCLUDED_)
