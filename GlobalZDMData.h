// GlobalZDMData.h: interface for the GlobalZDMData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALZDMDATA_H__1ACF23C3_2A4B_49B7_B4E7_79C7794B7B0E__INCLUDED_)
#define AFX_GLOBALZDMDATA_H__1ACF23C3_2A4B_49B7_B4E7_79C7794B7B0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JD_CENTER.h"
#include "GTZDM.h"
#include "DMX.h"
#include "bri.h"
#include "Tun.h"

class GlobalZDMData  //���ڱ���ʱ�Զ��б���뵥�߻������������ݱ�
{
public:
	GlobalZDMData();
	virtual ~GlobalZDMData();

	struct ZdmBox
	{
		AcGePoint2d lpt,rpt;//�ݶ���ͼ���µ㣬���ϵ�
		int DXorRXD;//1���߻� 2���������� 3����������
		int RXDno;//�����߱��
		CString RXDname;//����������
		CString mdbname;//���ݶ����Ӧ�����ݿ���
		double X0,Y0,H0;
		JD_CENTER *pm;//ƽ��ָ��
		GTZDM_ROAD *zdm;
		DMX_ROAD *dmx;
		BRI_ROAD *pbri;
		TUN_ROAD *ptun;
	} ZDMTK[100];
	int NZDM;//��¼��ǰ�ݶ�����Ŀ
	int CurZDM;//��¼��ǰ�ݶ����
	int FindWichZdm(AcGePoint3d pt);
	void IsRXDOrDX(AcGePoint3d pt,int &DXorRXD,int &rxdno,CString &RXDname);
	JD_CENTER *GetPM(AcGePoint3d pt); 
    GTZDM_ROAD *GetZDM(AcGePoint3d pt);
	DMX_ROAD *GetDMX(AcGePoint3d pt);
};

#endif // !defined(AFX_GLOBALZDMDATA_H__1ACF23C3_2A4B_49B7_B4E7_79C7794B7B0E__INCLUDED_)
