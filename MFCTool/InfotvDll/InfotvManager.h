#pragma once
/*!
 * \class 管理infotv模块类
 *
 * \brief 
 *
 * \author HXY10
 * \date 十月 2020
 */
#include "InfotvHandle_Parent.h"
#include "ConfigDlg_Parent.h"

class CInfotvManager
{
public:
	CInfotvManager();
	~CInfotvManager();

	//设置继承Infotv_Parent对象
	static void SetInstance(CInfotvHandle_Parent *infotv);
	
	//设置继承SYSTEM_PARAM_Parent对象
	static void SetParamInstance(SYSTEM_PARAM_Parent *param);

	//设置继承ConfigDlg_Parent对象
	static void SetDlgInstance(CConfigDlg_Parent *dlg);

	//设置最大创建实例数
	static void SetCreatMaxNum(UINT maxNum);

	//设置 dll 的所在路径和 dll名字
	static void SetCurDirAndModueName(CString dir,CString name);

	//设置 模块的名称 和  三个图标
	static void SetDescription(HBITMAP * pHBmp, char * szBuf);

	static bool IsExistFile(LPCTSTR szFile);

	/******************
		
			下面的参数和方法内部使用，不用设置和调用

	*******************************************/
public:
	//获取实例 自己使用
	static CInfotvHandle_Parent *GetInstance();
	static CConfigDlg_Parent *GetDlgInstance();
	static SYSTEM_PARAM_Parent *GetParamInstance();

public:
	static 	HBITMAP __hBmp[3];   //模块的图标
	static char __szTempName[50]; //模块名
	static CInfotvHandle_Parent *__infotv;
	static CConfigDlg_Parent * __cfgDlg;
	static SYSTEM_PARAM_Parent *__sysParam;
	static char __szInfotvVersion[50]; //记录infotv的版本号
	static UINT __maxInstanceNum;//记录最大创建的数量
	static CUIntArray		__guInstance; //记录所有创建的实例
	static UINT			__guMaxNum;      //记录实例的总数

	static UINT				__giHeight;	// 屏幕高
	static UINT				__giWidth;	// 屏幕宽
	static BOOL			__gbPlayMode;	// 判断主程序是否是支持播放的程序

	static CString __strCurDir; //运行的目录
	static CString __strModuleName; //模块名

};

