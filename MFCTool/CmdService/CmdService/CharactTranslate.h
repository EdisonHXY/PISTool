/******************************************************************************************
*	版权所有(c) 2018 北京冠华天视软件部
*	文件名称:	CharactTranslate.h
*	文件说明:	字符编码换换定义文件
*	当前版本:	1.0.20180714
*	作者:		贾巍云
*	创建日期:	2018-07-14
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
