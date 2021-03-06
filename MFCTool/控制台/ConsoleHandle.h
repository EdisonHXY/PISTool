#pragma once
class CConsoleHandle
{
public:
	CConsoleHandle();
	~CConsoleHandle();
	
	//控制台类型
	enum WRITETPYE
	{
		WRITETPYE_INFO, //正常信息
		WRITETPYE_WARN, //警告信息
		WRITETPYE_ERROR //错误信息
	};

	//打开控制台
	bool OpenConsole(CString titleStr);
	
	//关闭
	bool CloseConsole();

	//输出文字
	void WriteConsole(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

	//设置输出格式
	void SetOutPattern(CString timeFormatStr);

	//设置不同类型下的颜色
	void SetColor(WRITETPYE wp, WORD colorValue);


private:
	bool m_enableWriteConsole;
	WORD m_infoColor;
	WORD m_warnColor;
	WORD m_errorColor;
	CRITICAL_SECTION m_lock;
	CString m_formatStr;

};

