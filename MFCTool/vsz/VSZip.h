#pragma once

#pragma pack(1)

#define VSZIP_EXPORT extern "C" _declspec(dllexport)

enum VSZ_CODE
{
	VSZ_OK = 0,
	VSZ_SRC_ERROR,
	VSZ_DST_ERROR,
	VSZ_MEM_ERROR,
};

typedef struct
{
	char szFile[MAX_PATH];
	ULONGLONG ullFileSize;
	FILETIME  ftEditTime;
	ULONGLONG ullStartPos;
	ULONGLONG ullStopPos;

}	FILE_DESC, * PFILE_DESC;

//压缩句柄
typedef void * HVSZ;

//进度回调函数
//iPercent: 0～100
typedef void (* lpProgressFunc)(LPVOID pVoid, const char * szFile, int iPercent);

#pragma pack()

//压缩一个目录，输出到szOutFile文件
//szPath：需要压缩的目录的完整路径
//szOutFile：压缩后的文件完整路径，如果为NULL，则压缩后的文件位于szPath相同的目录下，扩展名为vsz
//返回0，表示压缩成功，否则表示错误代码
VSZIP_EXPORT int  VS_Compress(const char * szPath, const char * szOutFile, lpProgressFunc ProgressFunc, LPVOID pVoid);

//压缩单个文件，输出到szOutFile文件
//szInFile：需要压缩的单个文件的完整路径
//szOutFile：压缩后的文件完整路径，如果为NULL，则压缩后的文件位于szInFile相同的目录下，扩展名为vsz
//返回0，表示压缩成功，否则表示错误代码
VSZIP_EXPORT int  VS_CompressFile(const char * szInFile, const char * szOutFile, lpProgressFunc ProgressFunc, LPVOID pVoid);

//解压缩一个文件，输出到szOutPath目录
//szInFile：需要解压缩的文件的完整路径
//szOutPath：输出目录，如果为NULL，直接使用压缩文件所在的目录
//返回0，表示解压缩成功，否则表示错误代码
VSZIP_EXPORT int  VS_Decompress(const char * szInFile, const char * szOutPath, lpProgressFunc ProgressFunc, LPVOID pVoid);

//初始化压缩文件内的文件列表，以获取文件列表
//szInFile：压缩文件的完整路径
//iFileNum：返回压缩文件内的文件数量
//返回压缩句柄，用于逐个获取文件列表，返回NULL，表示失败
VSZIP_EXPORT HVSZ VS_ParseVSZ(const char * szInFile, int & iFileNum);
//获取压缩文件内包含的文件名
//hVSZ：需要释放的压缩句柄
//iIndex：需要获取文件名的文件序号，从0开始，一直递增
//pFileDesc：压缩文件单元的信息，包括文件名、原始文件大小、压缩文件大小
//返回true，表示获取成功，否则表示已经没有文件
VSZIP_EXPORT BOOL VS_GetNextFile(HVSZ hVSZ, int iIndex, PFILE_DESC pFileDesc);
//释放压缩句柄
//hVSZ：需要释放的压缩句柄
VSZIP_EXPORT void VS_FreeVSZ(HVSZ hVSZ);

//计算文件的md5
//szInFile：需要计算MD5的文件名
//szMD5：保存计算好的32个字节的MD5码
//返回true表示成功，否则失败
VSZIP_EXPORT BOOL VS_GetMD5(const char * szInFile, char szMD5[36]);
