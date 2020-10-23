#include "stdafx.h"
#include "InfotvHandle_Parent.h"
#include "InfotvManager.h"

CInfotvHandle_Parent::CInfotvHandle_Parent()
{
}


CInfotvHandle_Parent::~CInfotvHandle_Parent()
{
}

void CInfotvHandle_Parent::GetModuleSize(UINT &nWidth, UINT &nHeight)
{
	nWidth = CInfotvManager::__giWidth;
	nHeight = CInfotvManager::__giHeight;
}

TiXmlElement * CInfotvHandle_Parent::CreateFontElement(LPCTSTR szName, FONT_PARAM * pFont)
{
	char szBuf[MAX_PATH];
	WCHAR wszBuf[MAX_PATH];
	TiXmlElement * pXmlModule, *pXmlItem;

	pXmlModule = new TiXmlElement(szName);
	//font name
	MultiByteToWideChar(CP_ACP, 0, pFont->szFont, -1, wszBuf, MAX_PATH);
	WideCharToMultiByte(CP_UTF8, 0, wszBuf, -1, szBuf, MAX_PATH, NULL, NULL);
	pXmlModule->SetAttribute("Name", szBuf);
	//大小、宽度、字距、行距
	pXmlModule->SetAttribute("Size", pFont->iSize);
	pXmlModule->SetAttribute("Width", pFont->iWidth);
	pXmlModule->SetAttribute("Kerning", pFont->iKerning);
	pXmlModule->SetAttribute("Leading", pFont->iLeading);

	//倾斜度、旋转、粗体、斜体、下划线
	pXmlItem = new TiXmlElement("Style");
	pXmlItem->SetAttribute("Slant", pFont->iSlant);
	pXmlItem->SetAttribute("Rotation", pFont->iRotation);
	pXmlItem->SetAttribute("Bold", pFont->bBold ? 1 : 0);
	pXmlItem->SetAttribute("Italic", pFont->bItalic ? 1 : 0);
	pXmlItem->SetAttribute("Underline", pFont->bUnderline ? 1 : 0);
	pXmlModule->LinkEndChild(pXmlItem);

	//面参数
	pXmlItem = new TiXmlElement("Face");
	pXmlItem->SetAttribute("Color", pFont->crFace);
	pXmlItem->SetAttribute("Alpha", pFont->iFaceAlpha);
	pXmlModule->LinkEndChild(pXmlItem);

	//边参数
	pXmlItem = new TiXmlElement("Edge");
	pXmlItem->SetAttribute("Type", pFont->iEdgeType);
	pXmlItem->SetAttribute("Width", pFont->iEdgeWidth);
	pXmlItem->SetAttribute("Color", pFont->crEdge);
	pXmlItem->SetAttribute("Alpha", pFont->iFaceAlpha);
	pXmlModule->LinkEndChild(pXmlItem);

	//影参数
	pXmlItem = new TiXmlElement("Shadow");
	pXmlItem->SetAttribute("Type", pFont->iShadowType);
	pXmlItem->SetAttribute("Width", pFont->iShadowWidth);
	pXmlItem->SetAttribute("Color", pFont->crShadow);
	pXmlItem->SetAttribute("Alpha", pFont->iShadowAlpha);
	pXmlItem->SetAttribute("Angle", pFont->iShadowAngle);
	pXmlItem->SetAttribute("Softness", pFont->iSoftness);
	pXmlModule->LinkEndChild(pXmlItem);
	return pXmlModule;
}


void CInfotvHandle_Parent::SetBackParamAndDraw(GHG_PARAM * pParam)
{

	HCONTEXT context = pParam->context;
	SYSTEM_PARAM_Parent * pSysParam = (pParam->sysParam);

	ULONG ulColors[2];
	USHORT usScales[2];

	switch (pSysParam->iBkType)
	{
	case FILL_TYPE_PURE:
		GDK_SetBkType(context, FILL_TYPE_PURE);
		GDK_SetBkColor(context, GetRGBA(pSysParam->iPureAlpha, pSysParam->crPure));
		break;
	case FILL_TYPE_GRADUAL:
		//set draw blend color
		GDK_SetBkType(context, FILL_TYPE_GRADUAL);
		ulColors[0] = GetRGBA(pSysParam->iBlendAlpha1, pSysParam->crBlend1);
		ulColors[1] = GetRGBA(pSysParam->iBlendAlpha2, pSysParam->crBlend2);
		usScales[0] = 0;
		usScales[1] = 255;  //以255为最大值的分界线，表示每种渐变颜色所处位置
		GDK_SetBkColors(context, ulColors, usScales, 2, pSysParam->iBlendColorType);
		break;
	case FILL_TYPE_PICTURE:
	{
		GDK_SetBkType(context, FILL_TYPE_PICTURE);
		CString bkFile = GetPicFilePath(pParam, pParam->sysParam->szBkFile);
		GDK_SetBkPicFileName(context, bkFile.GetBuffer(), PICTURE_TGA);
		break;
	}
	
	case FILL_TYPE_NONE:
	default:
		GDK_SetBkType(context, FILL_TYPE_PURE);
		GDK_SetBkColor(context, RGBA(0, 0, 0, 0));
	}

	GDK_InvalBk(context);
}

void CInfotvHandle_Parent::GetModelDirAndFileName(CString &dir, CString &fileName)
{
	dir = CInfotvManager::__strCurDir;
	fileName = CInfotvManager::__strModuleName;
}

void CInfotvHandle_Parent::SetDefaultFont(FONT_PARAM &font, char * szFont, int iSize)
{
	sprintf(font.szFont, "%s", szFont);
	font.iSize = iSize;
	font.iWidth = 0;
	font.iSlant = 0;
	font.iRotation = 0;
	font.iKerning = 0;
	font.iLeading = 2;
	font.bBold = false;
	font.bItalic = false;
	font.bUnderline = false;
	//face
	font.crFace = RGB(255, 255, 255);
	font.iFaceAlpha = 255;
	//shadow
	font.iShadowWidth = 0;
	font.crShadow = RGB(0, 0, 0);
	font.iShadowAlpha = 255;
	font.iSoftness = 4;
	font.iShadowType = 0;
	font.iShadowAngle = 7 * 45;
	//edge
	font.iEdgeWidth = 0;
	font.crEdge = RGB(200, 200, 200);
	font.iEdgeAlpha = 255;
	font.iEdgeType = 0;
}

CString CInfotvHandle_Parent::GetPicFilePath(GHG_PARAM *pParam,CString strFileName)
{
	CString strPath = pParam->szCurPath;

	if (strFileName.Find("\\") == -1)
	{
		if (strPath.GetLength() == 0)
		{
			sprintf(pParam->szErrorString, "没有指定当前路径");
			return "";
		}
		if (strPath.Right(1) != "\\") strPath += "\\";
		strFileName = strPath + strFileName;
	}

	return strFileName;
}


void CInfotvHandle_Parent::SetFontParam(HCONTEXT hContext, FONT_PARAM font)
{
	//仅仅修改了字体的名称、高、宽、加粗、斜体、下划线等属性
	LOGFONT logFont;

	GDK_GetFont(hContext, logFont);
	strcpy(logFont.lfFaceName, font.szFont);
	logFont.lfHeight = font.iSize;
	logFont.lfWidth = font.iWidth;
	logFont.lfEscapement = font.iRotation * 10;
	logFont.lfOrientation = font.iSlant * 10;
	logFont.lfWeight = font.bBold ? FW_BOLD : FW_NORMAL;
	logFont.lfItalic = font.bItalic;
	logFont.lfUnderline = font.bUnderline;

	GDK_SetFont(hContext, logFont);
	//set edge width
	GDK_SetEdgeWidth(hContext, font.iEdgeWidth);
	//set shadow width
	GDK_SetShadowWidth(hContext, font.iShadowWidth);
}

void CInfotvHandle_Parent::SetColorParam(HCONTEXT context, FONT_PARAM font, FILL_TYPE fillType)
{
	BYTE R, G, B;
	R = GetRValue(font.crFace);
	G = GetGValue(font.crFace);
	B = GetBValue(font.crFace);
	//set face color
	GDK_SetFaceType(context, fillType);
	GDK_SetFaceColor(context, RGBA(B, G, R, font.iFaceAlpha));
	//set shadow color
	R = GetRValue(font.crShadow);
	G = GetGValue(font.crShadow);
	B = GetBValue(font.crShadow);
	GDK_SetShadowType(context, SHADOW_TYPE_OFFSET, font.iSoftness);  //not complete other type
	GDK_SetShadowColor(context, RGBA(B, G, R, font.iShadowAlpha));
	GDK_SetShadowAngle(context, font.iShadowAngle);

	//set edge color
	R = GetRValue(font.crEdge);
	G = GetGValue(font.crEdge);
	B = GetBValue(font.crEdge);

	GDK_SetEdgeType(context, fillType);
	GDK_SetEdgeColor(context, RGBA(B, G, R, font.iEdgeAlpha));
}


