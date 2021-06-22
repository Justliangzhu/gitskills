// ORI_SCALE.h: interface for the ORI_SCALE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORI_SCALE_H__99790D39_0E2F_4FDF_B476_8FF5E9B378FB__INCLUDED_)
#define AFX_ORI_SCALE_H__99790D39_0E2F_4FDF_B476_8FF5E9B378FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ORI_SCALE  
{
public:
	ORI_SCALE();
	virtual ~ORI_SCALE();
	double X_ORI,Y_ORI;//�ݶ����������
	double HSCALE,VSCALE,HBVSCALE;//������
	double STARTLC,ENDLC;//��·��ʼ��̣���ֹ���
	double ZMIN;//�ݶ������߳�
	double ZMTH;//�ݶ���ͼ��
	double LCBTEXTH;//��̱��ָߣ�
	double PQXHIGH;//ƽ��������
	double DESHIGH;//��Ƹ�����
	double EARHIGH;//���������
	double PQXTEXTH;
	double LCBHigh;
	double QZZ;//���µ����ȡΪ��10m,��50m,��Ҫ��
	double XSW;//�¶�ȡΪһλС��,��λС������λС����������С��
	double DesHXSW;//��Ʊ��ȡΪһλС��,��λС������λС����������С��
//	int LJorGM;//·���»������ 0-·���� 1-������
	int TDFS;//�϶���ʽ 1:�����϶�  2:ǰ�¶Ȳ����϶�  3:���¶Ȳ����϶�
	int VerRadius;//�����߱仯�������� 0 ��Լ�� 1 1m������ 2 10��������
	double PDTextH;//�¶��ָ�
//	int m_DXorRXD;//���߻����ж��ݶ���
//	int RXDno;//�ڼ����ж�
//	CString RXDname;//���ж���

};

#endif // !defined(AFX_ORI_SCALE_H__99790D39_0E2F_4FDF_B476_8FF5E9B378FB__INCLUDED_)
