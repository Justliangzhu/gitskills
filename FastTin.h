#if !defined(AFX_FASTTIN_H__7D5F0143_7BAC_4669_AE49_6ACF159A512E__INCLUDED_)
#define AFX_FASTTIN_H__7D5F0143_7BAC_4669_AE49_6ACF159A512E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FastTin.h : header file
//
#include "vect3d.h"
#include "DrawPM.h"
#include "BAS_DRAW_FUN.h"
#include "XLdataBase.h"
#include "resource.h"
//#include "TextProgressCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CFastTin dialog

class CFastTin : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CFastTin(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	CDrawPM *RoadHole[500];
	int NRoad;
// Dialog Data
	//{{AFX_DATA(CFastTin)
	enum { IDD = IDD_addJD1 };
//	CTextProgressCtrl	m_progress;
	//}}AFX_DATA
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFastTin)
public:		
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
	XLDataBase xlmdb;
	
	struct PtVect
	{
		Vect3d pvec; //���㷨����,��λ��
		float costhita; //���㷨������ƽ�淨�����Ƕȵ�����
	} *ptvec;
	struct TriVect{
		Vect3d trivec; //�����η�����,���ڼ��㶥�㷨����,���꼴ɾ��
	} *trivectmp;
	
    struct Triangle{
	   int vertex[3];
	   int Ajt[3];//*prior,*next;
	   ACHAR EdgeInf[4];//'f' fixed ,'a' active
       ACHAR Attribute;//���ԣ������������Ƿ���Ч,'Y'��Ч;'N'��Ч
	} *TriListHead, *TriList; 
    int TriListTailNo;
	
    struct PT{
	   double pt[3];
	   int cur;
	   ACHAR att;//����
	   int trino;
	} *ptdata;

    struct GridNode{
	      int Pfcur;//��һ�����ڸ�����ĵ�
		  int Ptcur;//������ڸ�����ĵ�
		  int Pncur;//��ǰ����Ĵ�����
          int PtSum;//��ǰ������ܵ���
	} *GridArray;

   struct GridIndex{
	         int GridNo;
			 int ptsum;			 
	      } *GridIdx;

     struct StackNode{  //�ڴ���������У�Vno����VerListǰ�±꣬inf����VerList���±�
         int trino;	        
	    	int Vno;  
		  int inf;//�Ƿ�ǰ������Ϊ��: 1:��, -1:����
		  struct StackNode *next;
	 }  *StackTop,*ImpactTriHead;

     struct StackNode *headptr,*tailptr;
     struct ImpactArea{
		int vf,vt;//Ӱ����߽�߶����
		int Vno;
		int trino;
		struct ImpactArea *next;
	 } *ImpactHead,*ImpactTail;

    struct TEdgeNode{
		int vf,vt;//Ӱ����߽�߶����
		int Vno;
		int trino;
		struct TEdgeNode *next;
	} *EdgeListHead,*EdgeListTail;

struct VerList//���������Ӱ��߽�ṹ
{
	int ptno; //���
	int	Vno;//�߽���ڽ������ζ�Ӧ��ţ�0��1��2��
	int trino;//�߽���ڽ������κ�
} *LeftVerList, *RightVerList;

int *ImpactTris; //��������Ӱ�������κ�
int PtZero[500], PtZeroNum; //������ͨ�������

struct record {
                struct record *llink;
                char edge[30];
                int trino; //�߽���ڽ������κ�
                int Vno;//�߽���ڽ������ζ�Ӧ��ţ�0��1��2��
                int bf;//ƽ������
                int mak;//
                struct record *rlink;
              };
struct record *Root;
double LeftLowPt[2],RightUpPt[2];
int PTSUM,RealPTSum;
int NRow,NCol,IdxSum,NCrs;
void FastCreateTin();
HANDLE mapfileHandlePp,mapfileHandleTt,MEMmapfileHandlePp,MEMmapfileHandleTt;
LPVOID FilePtrPp,FilePtrTt;    

void Cal_PtNormal();
void Cal_TinNormal();
void AdjustBriBj();
void FastSort(struct GridIndex R[],int s1,int t1);
int PARTITION(struct GridIndex R[],int l,int h);
int CreateGridIndex();
void AddList(struct Triangle *p);
int CreateFirstTri();
int IncrementInsertPt(int ptnum);//���������
int LocateTri(int ptnum);
double chkd(double pt11[3],double pt22[3],double pt33[3]);
int sign(double result);
double HCircle(int v1, int v2, int v3, int vl);
double CCW(int v1,int v2,int v3) ;
void  ExpandProgress(int trino,int vno);
int FindImpactArea(int trino,int ptnum);
void PUSHSTACK(int trino,int vno,int inf);
struct StackNode  *POPSTACK();
int ThrowPtInGrid();
//void DeleteTris();
void DeleteATri(struct Triangle *tri);
//void CreateAFace(double p0[3],double p1[3],double p2[3],Adesk::UInt16 color);
int FormHole();
int FindVerNo(double ver1[2]);
int AjustTIN(double ver1[2],double ver2[2]);
void DealHole(int i,int k);
int FormVerList(int StartTriNo,int vno,int StartVerNo,int EndVerNo,int &lpcur,int &rpcur,int &trcur);
double distance(double *,double *);
double coc(double *,double *,double *);
void ReConstructTin(struct VerList *verlist, int pcur, int &trcur);
int LocateTriOnXY(double pt[2]);
void  DigHole();
void enqueue(int);
struct StackNode *dequeue(void);
void init_queue(void);
struct StackNode *notempty(void);
int  SetEdgeInf(double ver1[2],double ver2[2]);
void detree(struct record *);
struct record *insert(int v1,int v2,int &trino ,int &Vno);
double CCW2(double pt1[2],double pt2[2],double pt3[2]) ;
double interpolate(double pt[2],int l);
int CheckPtInTri(int trino, double pt[2]);
int FindFirstNoEmptyGridNo(int crow,int ccol);
int CheckPtinViewCone(double pt[2]);
int read_FDfile();
int  AllocateFD(int nfend);
void WritePtIn3DFile(FILE *fpr,FILE *fpw,int &NumAll);
int FormZXHole();//����
void FormInitEgdes();
void DealEgdes();
void DeleteTris();
void SelectPoint();
void DealBriBjx();//�������߽��ߴ��� �õ��߳�����������߽�
void SetATinAtt(int itri);
void SetTriAtt(int ntri);
//ɾ���ϵ�,����дSPT�ļ�,����3dmodel�ļ�
int ThrowOffPt();
void AdjustBjx();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFastTin)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FASTTIN_H__7D5F0143_7BAC_4669_AE49_6ACF159A512E__INCLUDED_)
