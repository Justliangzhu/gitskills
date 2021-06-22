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
	double X_ORI,Y_ORI;//纵断面基点坐标
	double HSCALE,VSCALE,HBVSCALE;//比例尺
	double STARTLC,ENDLC;//线路起始里程，终止里程
	double ZMIN;//纵断面基点高程
	double ZMTH;//纵断面图高
	double LCBTEXTH;//里程表字高；
	double PQXHIGH;//平曲线栏高
	double DESHIGH;//设计高栏高
	double EARHIGH;//地面高栏高
	double PQXTEXTH;
	double LCBHigh;
	double QZZ;//变坡点里程取为整10m,整50m,无要求
	double XSW;//坡度取为一位小数,两位小数，三位小数，不保留小数
	double DesHXSW;//设计标高取为一位小数,两位小数，三位小数，不保留小数
//	int LJorGM;//路肩坡或轨面坡 0-路肩坡 1-轨面坡
	int TDFS;//拖动方式 1:任意拖动  2:前坡度不变拖动  3:后坡度不变拖动
	int VerRadius;//竖曲线变化步长控制 0 无约束 1 1m整倍数 2 10米整倍数
	double PDTextH;//坡度字高
//	int m_DXorRXD;//单线或绕行段纵断面
//	int RXDno;//第几绕行段
//	CString RXDname;//绕行段名

};

#endif // !defined(AFX_ORI_SCALE_H__99790D39_0E2F_4FDF_B476_8FF5E9B378FB__INCLUDED_)
