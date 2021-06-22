// DialogSetDirectRamp.h : header file
// ֱ��ʽ���ٳ�����ƶԻ���
// 1.���Ƕγ��Ȱ���������Ľ��ȡֵ,���������
// 2.���ٳ����ܳ���Ҫ���ڹ淶ֵ
// 3.���Ƕ��뽥���ͬ��,�������ǽ���ε�˵��
// 4.���ٳ���������������һ��(�μ��淶˵��)
// 5.�ڱ��ٳ������ʱ�����û��ѡ����Բ,����ٳ�����������淶������������Ҫ�󼴿�
// 6.���Ƕε���ռ���ɴ����߽����һ���ѵ���λ��ȷ��
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIALOGSETDIRECTRAMP_H__BD905B50_44BB_4F2F_ACA9_A90EB1A9D676__INCLUDED_)
#define AFX_DIALOGSETDIRECTRAMP_H__BD905B50_44BB_4F2F_ACA9_A90EB1A9D676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
#include "BlockComputerFun.h"
#include "JD_CENTER.H"
#include "math.h"
#include "ZYLink.h"
#include "YYLuanLink.h"
#include "YYSLink.h"
#include "LLTuLink.h"

/////////////////////////////////////////////////////////////////////////////
// ֱ��ʽ���ٳ�����ƶԻ���

class CDialogSetDirectRamp : public CDialog, public BlockComputerFun
{
// Construction
public:
	CDialogSetDirectRamp(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSetDirectRamp)
	enum { IDD = IDD_DIALOGDIRECTRAMP };
	CString	m_EditChangedata;   // ������
	CString	m_EditRampTriMiles; // �����(���Ӷ�)���׮��
	CString	m_EditConnectLen;   // ���Ӷγ���
	CString	m_EditDRampSMiles;  // ���ٳ������׮��
	CString	m_EditDRampEMiles;  // ���ٳ����յ�׮��
	CString	m_EditDRampLen;     // ���ٳ�����
	CString	m_EditDRampERad;    // ����Բ�뾶
	CString	m_AorL;             // �����߲�������
	int		m_HLinkModel;       // ����Բ����ֹ��Ԫ����������
	double	m_dA1L1;            // A1 or L1
	double	m_dA2L2;            // A2 or L2
	double	m_dA3L3;            // A3 or L3
	double	m_dA4L4;            // A4 or L4
	CString	m_strW1234;         // W1,W2,W3,W4
	CString	m_fldR;             // ���������ʰ뾶
	CString	m_fldA;             // ������Aֵ
	double	m_dOffsetW;         // ��ֹ��Ԫƫ��
	//}}AFX_DATA

	double W1, W2, W3, W4;    // ���ٳ������Ƶ����
	double A1, A2, A3, A4;    // �������߲���
	
	double MainVect,RampVect;   // ���ߡ��ѵ�����Ƴ���
	JD_CENTER*   pGottnRamp;    // ����ʵ��ָ��
	AcDbObjectId SelEntId;      // ��ѡ���ߵ�ID
	double ValidItemType;       // �����б�ѡ�д�����Ԫ
	double ProTriMiles;	        // ���������Ӧ���������
	double m_dTriLen;           // ����γ���
	double DRampTriX,DRampTriY,TriSOrient,TriRad;  // �����������ꡢ��λ�Ǽ��뾶
	double TriMainX,TriMainY,TriMainOA,TriMainRad; // ����ζ�Ӧ�����ߵ����ꡢ��λ�Ǽ��뾶
	
	double ProDRampSMiles,ProDRampEMiles;         // ���ٳ������յ��Ӧ����·���
	AcGePoint3d DRampSDot,DRampEDot,OrignDot;     // ���ٳ������յ����꼰�����ߵ��������
	double DRampSOrient,DRampEOrient,DROriOrient; // ���ٳ������յ㼰�����ߵ����ķ�λ��
	double DRampSMainX,DRampSMainY,DRampSMainOA,DRampSMainRad; // ���ٳ������յ��Ӧ�����ߵ����ꡢ��λ�Ǽ��뾶
	double SCirX, SCirY;         // ���ٳ�����㴦��Բ������
	double DRampDef;             // ���ٳ�����ƫ��
	double DRampSRad, DRampERad; // ���ٳ������յ㴦�İ뾶
	
	AcDbObjectId TriRampId;      // ����ε�ͼ��ID
	AcDbObjectId ConnectLineID;  // ���Ӷ�(�����������)Ϊֱ��ʱ��ͼ��ID
	AcDbObjectId ConnectCirID;   // ���Ӷ�(�����������)Ϊ����ʱ��ͼ��ID

	AcDbObjectId SelCircleId;                 // ����ԲID
	double ProDRampCirMiles;	              // ����Բ��Բ�Ķ�Ӧ���������
	bool   m_bCirSelAgain;                    // ����Բ�Ƿ�����ʰȡ
	double DRampCirX, DRampCirY, DRampCirRad; // ����Բ��Բ�����꼰�뾶	

	AcDbObjectId StartElementId;              // ��ʼ��ԪID
	AcDbObjectId EndElementId;                // ��ֹ��ԪID
	AcDbObjectId OffsetElementId;             // ƫ�õ�ԪID
	
	double ChangeValue; // �����ʱ���(ƫת��)

	AcDbObjectId SprialID1,SprialID2;             // ����������ID	
	AcGePoint3d  FirstSprialEnd, SecondSprialEnd; // �������ߵ��յ�����
	double       FirstSprialEOA, SecondSprialEOA; // �������ߵ��յ㷽λ��
	double       FirstSprialLen, SecondSprialLen; // ���������߳�
	
	double LCLindSide; // ԲԲ���ӵ�ƫ��(��ʱ��:-1 ˳ʱ��:1) 
	double CCLineDef;  // ����ԲԲ���ڽ����������ߵĲ���
	double m_RampPosLeftOrRight;  // ���ٳ���λ�����ߵĲ���(����ԲԲ�����������ǰ������) ��:-1 ��:1
	double m_RampAddOrSub;        // ���ٳ���������(����:1 ����:-1)
	double flag;                  // ���ٳ����������Ƿ�ͬ��
	
	double m_dNoseCmlOnZX, m_dNoseCmlOnYX; // �Ƕ������ߺ������ϵ�ͶӰ���
	double m_NosePt[2];                    // �Ƕ�Բ�ĵ�
	AcDbObjectId W1Id, W2Id, W3Id, W4Id;   // ����W1,W2,W3,W4��Ӧ��ʶֱ��ID
	
	double RouteItemArrayOld[10][10]; // �ѵ���Ԫ(����acutGrDraw)
	double RouteItemArray[10][10]; // �ѵ���Ԫ
	JD_CENTER* RampJD_CENTER;      // �ѵ�ʵ��ָ��
	AcDbObjectId RampJD_CENTERID;  // �ѵ�ʵ��ID
	AcDbObjectId XYId[11];         // �ѵ�����ԪID
	// 
	int ItemNum;    
	int m_iXYMax;   // �����Ԫ��
	int m_iSelElem; // ѡ��Ԫ����
	bool   m_bFitConnectLen; // �Ƿ�ָ�����Ӷγ�
	double ConnectLen; // ���Ӷγ�(�����������)
	double SLength, SLength1, SLength2; // �������߶γ�

	int m_LinkModel;  // ���ٳ������Ϊ����ʱ,���ٳ���������Բ�����ӷ�ʽ

	// 
	double MainValueA;
	// 
	AcDbObjectId AddCirId;

	AcGePoint3d PickDotOld, PickDotNew; // ���ʰȡ��
	ads_point   PickDotEnd;   // ��ֹ��Ԫʰȡ����
	bool   m_bJD_CENTER;      // ��ֹ��Ԫ�Ƿ�ΪJD_CENTER
	bool   m_bConnectDirect;  // �Ƿ�ֱ��������ֹ��Ԫ
	bool   m_bDragConnectCir; // �Ƿ����϶����ӵ�Ԫ
	int    m_iConnectCirXY;   // ����Բ��Ԫ���
	double m_dEndElemR;       // ��ֹ��Ԫ�뾶
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSetDirectRamp)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	// ���ݽ�����������Ϣ���㽥��γ������ٳ�������
	void ComputerDRampParameters();
	// ���㻺�����ϵ�ָ��ֱ�����ΪD�ĵ�
	void GetEndPointOfDRamp();
	// ���Բ���ڽ����������ߵĲ���ԲԲ������ƫ��(˳ʱ�뻹����ʱ��)
	bool GetCenterSideAndLinkDef();
	// Բ�������㼰Բ���������������Բ������
	double GetArcLength(double CenX,double CenY,double ArcX1,double ArcY1,
	double ArcX2,double ArcY2,double ArcRad);
		
	void AutoModel();
	
protected:
	bool GetParameter();          // ��ý����������������������� 2005.07.21 ldf
	bool GetW1234();              // ��ÿ��Ƶ����W1,W2,W3,W4
	void PutA1A2();               // ��ͳһ��A1A2ֵ��������������
	void GetA1A2();               // �ɽ�������������ͳһ��A1A2ֵ
	bool SelectMainElement();     // ѡȡ���ߵ�Ԫ
	bool SelectConnectElement();  // ѡȡ���ӵ�Ԫ(Բ��Բ��)
	bool SelectEndElement();      // ѡȡ��ֹ��Ԫ(��·ʵ�塢ֱ�ߡ�Բ��Բ��)
	bool GetMainElement();        // ʵʱ������ߵ�Ԫ
	bool GetConnectCirCoordRad(); // ʵʱ�������ԲԲ������Ͱ뾶
	bool SetConnectCirRad(double dRadius);         // ��������Բ�뾶	
	bool SetConnectCirCoord(AcGePoint3d centerPt); // ��������ԲԲ������
	bool DragConnectCir();        // ��̬�϶�����Բ
	bool GetOffsetElement();      // ʵʱ�����ֹƫ�õ�Ԫ
	bool CheckSprial();           // ��黺�����ߵĺ�����
	bool CreateRampJD_CENTER(double XYArrayTmp[][10],int iXYNum);   // �������ٳ�����·ʵ��
	void Draw();                  // ���������·  2005.07.22 ldf
	// Generated message map functions
	//{{AFX_MSG(CDialogSetDirectRamp)
	afx_msg LONG OnAcadKeepFocus(UINT,LONG);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtontoknowmeaning();	
	afx_msg void OnButtongetendrad();
	afx_msg void OnButtonupdatedramp();
	afx_msg void OnCheck1();
	afx_msg void OnSelchangeAl();
	afx_msg void OnSelendokAl();
	afx_msg void OnButtonImply();
	afx_msg void OnCHECKConnectDirect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETDIRECTRAMP_H__BD905B50_44BB_4F2F_ACA9_A90EB1A9D676__INCLUDED_)
