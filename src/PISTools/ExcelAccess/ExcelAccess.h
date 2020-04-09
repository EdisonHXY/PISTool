
#ifndef	_EXCELACCESS_H_OC
#define	_EXCELACCESS_H_OC

#include <afxdb.h>
#include <odbcinst.h>


class CExcelAccess
{
//方法及过程函数 -----------------------------------------------------------------
public:
	CExcelAccess(void);
	~CExcelAccess(void);

	BOOL OpenExcel(CString strExcelFile);
	void CloseExcel(void);
	int GetFieldValue(CStringArray &arryValue, CString strTable, int iFieldIndex);

	BOOL CreateExcel(CString strExcelFile);
	void CreateTable(...);

private:
	CString GetExcelDriver(void);
	
private:
	CDatabase m_dbExcel;
	CString m_strFile;
	
};

#endif



