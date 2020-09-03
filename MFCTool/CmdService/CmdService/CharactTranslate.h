/******************************************************************************************
*	��Ȩ����(c) 2018 �����ڻ����������
*	�ļ�����:	CharactTranslate.h
*	�ļ�˵��:	�ַ����뻻�������ļ�
*	��ǰ�汾:	1.0.20180714
*	����:		��Ρ��
*	��������:	2018-07-14
******************************************************************************************/
#ifndef __CHARACTTRANSLATE_H__   
#define __CHARACTTRANSLATE_H__   
  
class CCharTrans  
{  
public:  
    CCharTrans(void);  
    virtual ~CCharTrans(void);  

	CString GBKToUtf8(const CString &strGBK);
	CString Utf8ToGBK(const CString &strUtf8);
	int GBKToUnicode(const CString &strGBK, wchar_t *pUnicodeBuf);
	CString UnicodeToGBK(const WCHAR *szUnicode);

	
};  


#endif
