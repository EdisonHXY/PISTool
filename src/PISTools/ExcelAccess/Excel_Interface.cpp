#include "stdafx.h"
#include "..\对外接口\Excel_Interface.h"
#include "ExcelAccess.h"

 int OC_GetExcelContent(CString filePath, CStringArray &contentList, CString strTable, int nFieldIndex)
{
	 CFileFind fileFind;
	 if (!fileFind.FindFile(filePath) || nFieldIndex < 0)
	 {
		 return OC_ERROR_CODE_PARAM;
	 }
	 CExcelAccess readExcel;
	 if (!readExcel.CreateExcel(filePath))
		 return OC_ERROR_CODE_FAIL;

	 contentList.RemoveAll();

	 readExcel.GetFieldValue(contentList,strTable, nFieldIndex);
	 readExcel.CloseExcel();

	 return OC_ERROR_CODE_NONE;

}
