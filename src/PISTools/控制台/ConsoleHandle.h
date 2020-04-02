#pragma once
class CConsoleHandle
{
public:
	CConsoleHandle();
	~CConsoleHandle();
	
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

	CString m_formatStr;

};

