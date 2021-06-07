#pragma once
/*!
 * \class infotv模块实现的父类
 *
 * \brief 使用的时候需要基础该类 并且基础 SYSTEM_PARAM_Parent 实现自己结构体
 *
 * \author HXY10
 * \date 十月 2020
 */
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include <vector>
#include <string>
using namespace std;
typedef struct
{
	char szModule[64];				//模块名称
	//位置大小
	int  iPosX, iPosY;
	int  iWidth, iHeight;

	//背景
	int  iBkType;
	COLORREF crPure, crBlend1, crBlend2;
	int  iPureAlpha, iBlendAlpha1, iBlendAlpha2;
	int  iBlendColorType;							//渐变色类型
	char szBkFile[MAX_PATH];


	BOOL bStartByOther;				// 是否由其它模块启动

}	SYSTEM_PARAM_Parent;

typedef struct __GHG_PARAM
{
	//系统参数
	SYSTEM_PARAM_Parent *sysParam;

	HCONTEXT context;
	int		iOldType;
	BOOL	bPlaying;
	BOOL	bPause;
	BOOL	bCleanScreenOnStop;	// 停止时是否清屏属性
	UINT	uIndex;
	char	szErrorString[MAX_PATH];
	char	szCurPath[MAX_PATH];
	char	szOldPath[MAX_PATH];

	__GHG_PARAM()
	{

		context = 0;
		iOldType = 0;
		bPlaying = FALSE;
		bPause = FALSE;
		bCleanScreenOnStop = FALSE;
		uIndex = 0;
		memset(szOldPath, 0, MAX_PATH);
		memset(szCurPath, 0, MAX_PATH);
		memset(szErrorString, 0, MAX_PATH);
	}

}	GHG_PARAM, *PGHG_PARAM;

class CInfotvHandle_Parent
{

public:
	CInfotvHandle_Parent();
	~CInfotvHandle_Parent();

	//设置默认的参数
	virtual void SetDefaultParam(GHG_PARAM *) = 0;

	//保存参数到文件
	virtual bool SaveParamToFile(const SYSTEM_PARAM_Parent *param,CString szIniFile, CString szXmlFile) = 0;

	//从配置文件中读取参数
	virtual void ReadParamFromFile(SYSTEM_PARAM_Parent *param, CString szIniFile) = 0;

	//播放前的验证操作
	virtual bool VerifyPlayData(GHG_PARAM *param) = 0;

	//开始播放 可以绘制背景 开启组播等
	virtual void StarPlay(GHG_PARAM * hGhg) = 0;

	//停止播放 关闭组播等
	virtual void EndPlay(GHG_PARAM * pParam) = 0;

	//绘制内容 定时器会定时调用
	virtual void DrawContent(GHG_PARAM * hGhg, SYSTEMTIME sysTime, UINT uInterval) = 0;

	//获取需要添加的文件 例如添加的背景图片等 将本地路径的文件 拷贝到 模块中
	virtual void GetNeedAddFiles(GHG_PARAM * pParam,vector<string>  &picList) = 0;

	//针对添加的文件 进行 更新配置文件的参数 更新 参数中 指定文件的路径
	virtual void UpdateAddFileParam(GHG_PARAM * pParam,CStringArray &newFilePath) = 0;


public:
	//获取模块的大小
	void GetModuleSize(UINT &nWidth, UINT &nHeight);

	//设置默认字体
	void SetDefaultFont(FONT_PARAM &font, char * szFont, int iSize);
	
	//获取需要播放的文件路径 
	CString GetPicFilePath(GHG_PARAM *pParam, CString strFileName);


	//设置字体
	void SetFontParam(HCONTEXT hContext, FONT_PARAM font);
	

	void SetColorParam(HCONTEXT context, FONT_PARAM font, FILL_TYPE fillType);
	

	//创建字体的xml元素
	TiXmlElement * CreateFontElement(LPCTSTR szName, FONT_PARAM * pFont);
	//设置背景参数并绘制
	void  SetBackParamAndDraw(GHG_PARAM * pParam);

	//获取模块的运行目录和名
	void GetModelDirAndFileName(CString &dir, CString &fileName);
};

