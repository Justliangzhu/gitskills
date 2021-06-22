// CROSS.h
// by Puhao
// "Programming AutoCAD with ObjectARX"

#include "DesignLine.h"
#include "LTGou.h"
#include "Hdmtext.h"
#include "earthline.h"
#include "BAS_DRAW_FUN.h"
#include "JSGou.h"
#include "DesLineReact.h"
#include "LTGouReactor.h"
#include "JSGouReactor.h"	//20190927����

class CROSS : public BAS_DRAW_FUN//������
{
	public:
    	CROSS();	// Constructor
		~CROSS();	// Destructor
		void FreeRoom();

	    void DrawCrossCenLine(AcDbObjectId layerId,double NoteTextH=4.0);
		void Draw();
		void NoteDist(AcGePoint3d Spt,AcGePoint3d Ept,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,AcDbObjectId LayerId);
		AcDbObjectId AddEntityToModal(AcDbObjectId layerId,AcDbEntity *entity);
	  	AcDbObjectId H_maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,AcDbObjectId layerId);
		void OutDraw(double TabTextH=5.0,double NoteTextH=4.0);
	public:
		int BzTWInf;//-1 ��ע��������
		ACHAR  m_ckml[256];
		ACHAR  m_FLDckml[256];
		double m_cml;
		double m_ZTWH,m_YTWH,m_YZTWH,m_YYTWH;//���ڸ�
		double m_FillArea;//����
		double m_CutArea;//�ڷ����
		double m_LeftZdk;//���ռ�ؿ�
		double m_RightZdk;//�Ҳ�ռ�ؿ�
		double m_BTinf;
		double m_YBTinf;
		double m_EarH;
		double Ymin,Ymax;
		double xjj;//�߼��
		double offset,Yoffset;

		//20190822����	�Ĺ�����
		bool m_IsModifyGutter;		//�Ƿ��ǸĹ�ģʽ
		CString m_GutterPath;		//�Ĺ�ͼ��·��
		double m_GutterThick;		//ˮ�����׺�

		//20190829����	�Ƿ����ռ��׮
		bool m_IsDrawCrossEarthPile;

		//20190903����	������������
		double m_NetCenterY;

		//20190910����	ˮ��ͼ������
		ACHAR m_GutterBlockName[256];
	
		int DorS;//��·���˫·��
		int IfSetDQ;
		AcGePoint3d m_OriPoint;//��ͼԭ��
		double m_Scale;	//������
		double DX;
		//�����
		EarthLine_ROAD   *LeftEarthLinePart,*RightEarthLinePart;
		//��Ƶ�
		DesignLine_ROAD  *LeftDesignLinePart,*RightDesignLinePart;
		DesignLine_ROAD  *Y_LeftDesignLinePart,*Y_RightDesignLinePart;
		//ˮ��
		LTGOU_ROAD       *LeftLTGouPart,*RightLTGouPart;
		LTGOU_ROAD       *Y_LeftLTGouPart,*Y_RightLTGouPart;
		JSGOU_ROAD       *LeftJSGouPart,*RightJSGouPart;
		JSGOU_ROAD       *Y_LeftJSGouPart,*Y_RightJSGouPart;

		//ע��
		HdmText_ROAD     *Note;
	   AcDbObjectId LearthId,RearthId,LgouId,RgouId,LJsgId,RJsgId,LdesignId,RdesignId,NoteId;
	   AcDbObjectId Y_LgouId,Y_RgouId,Y_LJsgId,Y_RJsgId,Y_LdesignId,Y_RdesignId;
	   
	   DesLineReact *LDesReactor;
	   DesLineReact *RDesReactor;
	   DesLineReact * Y_RDesReactor;

	   LTGouReactor *LLTGouReactor;
	   LTGouReactor *RLTGouReactor;
	   LTGouReactor *Y_RLTGouReactor;

	   //20190927����	��ˮ����Ӧ��
	   JSGouReactor* LJSGouReactor;
	   JSGouReactor* RJSGouReactor;
	   JSGouReactor* Y_RJSGouReactor;

	   bool IsLinkHdm;

	   void wr_BG();
	   void wr_TW();
	   void DrawCrossDQLine(AcGePoint3d pjpt);

	   void DrawCrossInfor(double TextH=5.0);	//���ƺ������Ϣ	20190815����
	   void CalSlopeRateACHAR(double m, ACHAR Str[]);
	   double Round(double Value);

	   void DrawGutterBlock(AcGePoint3d OriPt);

	   void DrawZDK();	//����ռ�ؿ���Ϣ	20190823����

	   void DrawCrossInfor_Gutter();	//���ƸĹ��������Ϣ	20190829

	   double OffsetByXLZX;//·�������������·���ߵ�ƫ����
	   double m_EarHOnLJZX;//·�����ߵ�����

	   double m_SuphRate_L, m_SuphRate_R;
};
