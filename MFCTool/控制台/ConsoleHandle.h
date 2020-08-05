#pragma once
class CConsoleHandle
{
public:
	CConsoleHandle();
	~CConsoleHandle();
	
	//����̨����
	enum WRITETPYE
	{
		WRITETPYE_INFO, //������Ϣ
		WRITETPYE_WARN, //������Ϣ
		WRITETPYE_ERROR //������Ϣ
	};

	//�򿪿���̨
	bool OpenConsole(CString titleStr);
	
	//�ر�
	bool CloseConsole();

	//�������
	void WriteConsole(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

	//���������ʽ
	void SetOutPattern(CString timeFormatStr);

	//���ò�ͬ�����µ���ɫ
	void SetColor(WRITETPYE wp, WORD colorValue);


private:
	bool m_enableWriteConsole;
	WORD m_infoColor;
	WORD m_warnColor;
	WORD m_errorColor;

	CString m_formatStr;

};

