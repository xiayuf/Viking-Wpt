//=============================================================================================================================
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//=============================================================================================================================
//
// 007 HOOK
// 2011(c)
// Code By: 007 + boy_scout
// Team: ETK
// Website: etalking.com.ar
//
//=============================================================================================================================
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
//=============================================================================================================================

//#pragma optimize("gsy", on)
//#pragma optimize("gtp",on)
//#pragma optimize("p",on)

// Set section alignment to be nice and small
//#pragma comment(linker, "/FILEALIGN:0x200")

// Make section alignment really small
//#pragma comment(linker, "/FILEALIGN:16")//se encontr?la directiva '/FILEALIGN:16' no v醠ida; se omiti?
//#pragma comment(linker, "/ALIGN:20")
//#pragma comment(linker, "/ALIGN:20")//valor de '/ALIGN' no v醠ido 20 (debe ser potencia de 2); se supone el valor predeterminado

//#pragma comment(linker, "/FILEALIGN:16")
//#pragma comment(linker, "/ALIGN:16")

//#pragma comment(linker, "/FILEALIGN:0x200")
//#pragma comment(linker, "/ALIGN:512")
//#pragma comment(linker, "/SAFESEH:NO")

// Merge all default sections into the .text (code) section.
#pragma comment(linker,"/merge:.rdata=.data")
#pragma comment(linker,"/merge:.text=.data")
//#pragma comment(linker,"/merge:.reloc=.data")

#ifdef _WIN64
#pragma comment(lib, "VMProtectSDK64.lib")
#endif

//#define  VA_SHARED 0x8000000

///////////////////////////////////////

//Defining that macro before including windows.h causes only commonly used parts of the Windows API to be included. 
//It usually doesn't make the final executable any smaller, but it speeds the compilling time.
#define  WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
//#include <fstream>
#include <commdlg.h>
#include "resource.h"
//#include "md5.h"
//#include "utils/VMProtectSDK.h"
//#include "utils/WinlicenseSDK.h"

//#pragma pack(1)
//#pragma pack (push ,1)
//#pragma pack (pop, 1)

STARTUPINFO          SI;
PROCESS_INFORMATION  PI;

using namespace std;

//===========================================================================================================================================================

inline void removeTrailingCRLF(char* a)
{
	while(a[strlen(a)-1]=='\x0D' || a[strlen(a)-1]=='\x0A' ) a[strlen(a)-1]=0;
}

//===========================================================================================================================================================

inline bool fileExists(const char* filename)
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}

//===========================================================================================================================================================

inline BOOL bInjectLibrary(HANDLE hProcess, char* szDllToInjectPath)
{
    LPVOID lpRemoteAddress = VirtualAllocEx(hProcess, NULL, strlen(szDllToInjectPath), MEM_COMMIT, PAGE_READWRITE);
    if( !lpRemoteAddress )
        return FALSE;
    if( !WriteProcessMemory(hProcess, lpRemoteAddress, (LPVOID)szDllToInjectPath, strlen(szDllToInjectPath), NULL) )
        return FALSE;
    HANDLE hThread = NULL;
    if( !(hThread = CreateRemoteThread(hProcess, NULL, NULL, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("KERNEL32.DLL"), "LoadLibraryA"), lpRemoteAddress, NULL, NULL)) )
        return FALSE;
    WaitForSingleObject(hThread, INFINITE);
    if( !VirtualFreeEx(hProcess, lpRemoteAddress, 0, MEM_RELEASE) )
        return FALSE;
    CloseHandle(hThread);
    return TRUE;
}

//===========================================================================================================================================================
//void DbgPrint(const char* format, ...){static char tmp[2048];va_list p;va_start(p, format);vsprintf(tmp,format,p);OutputDebugString(tmp);va_end(p);}

/*
char ogcdir [256];
inline std::string getOgcDirFile(const char* basename)
{
	if(strstr(basename,"..")){ return ":*?\\/<>\""; }
	string ret = ogcdir;
	return (ret+basename);
}*/

//===========================================================================================================================================================

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nShowCmd)
{

#define CH_ERROR_DLL1 "请重新拷贝"
#define CH_ERROR_DLL2 "DLL丢失"
#define CH_MSG_CS1  "CS目录" 
#define CH_ERROR_INI1 "ini写入错误" 
#define CH_ERROR_EXE1 "不能启动"
#define CH_ERROR_EXE2 "错误"
#define CH_ERROR_HOOK1 "无法注入"
#define CH_ERROR_HOOK2 "[飓风路线]"
#define CH_ERROR_HOOK3 "注入失败");
#define CH_ERROR_HOOK4 "[飓风路线]"

//	CODEREPLACE_START
//	VMProtectBegin
	char dll_name[] = {'e','V','O','L','.','d','l','l',0};
//	VMProtectEnd
//	CODEREPLACE_END

//	VMProtectBegin
//	CODEREPLACE_START
	if(!fileExists(dll_name))
	{


		//CODEREPLACE_START
//		char szFailMsg[400] = { 0 };
		//VMProtectBegin
//		sprintf( szFailMsg, "DLL not found\n\n\"%s\"", dll_name );
//		MessageBox( NULL, szFailMsg, EN_ERROR_MSG, MB_ICONERROR | MB_OK );

		MessageBox(0,"请重新拷贝","DLL丢失",MB_ICONERROR|MB_TOPMOST);

		//VMProtectEnd
		//CODEREPLACE_END
		return -1;
	}
//	CODEREPLACE_END
//	VMProtectEnd


	HINSTANCE mainInstance;
	mainInstance = hInstance;
	char hookEXE  [4000] = ""; 
	char hookDLL  [400] = ""; 
	char hookINI  [400] = "";
	char hlCmdLine[400] = "";
	char hlEXE    [400] = "";
	
	GetModuleFileName(0,hookEXE,390); 
	int  len = strlen(hookEXE);
	strcpy(hookDLL,hookEXE); 
	strcpy(hookINI,hookEXE); 
	hookDLL[len-3]='d';hookDLL[len-2]='l';hookDLL[len-1]='l';
	hookINI[len-3]='i';hookINI[len-2]='n';hookINI[len-1]='i';

	bool success = false;
	do{
		//VMProtectBegin
		FILE* file = fopen(hookINI,"r");
		if(file)
		{
			fgets(hlEXE    ,390,file);
			hlEXE[398]=0;     
			removeTrailingCRLF(hlEXE);
			fclose(file);
		}
		//VMProtectEnd

		//WIN32_FIND_DATA finddata;
	    //HANDLE handle = FindFirstFile(hlEXE,&finddata);

		if(!fileExists(hlEXE))
		//if(handle==INVALID_HANDLE_VALUE)
		{
			//VMProtectBegin
			if(hlEXE)
			{
				/////////////////////////////

//				CODEREPLACE_START

				char* retFile=hlEXE;//hlEXE
				const char* tip="hl.exe.cstrike.exe\0";//"hl.exe.cstrike.exe"
				char* filter="hl.exe or cstrike.exe\0hl.exe;cstrike.exe\0";//"hl.exe or cstrike.exe\0hl.exe;cstrike.exe\0"
				char* title="CS 1.6 Location\0";//"CS目錄"
				bool save=false;//false
				HWND hWnd=0;//0
				//DbgPrint("retFile %s",retFile);
				//DbgPrint("tip %s",tip);
				//DbgPrint("filter %s",filter);
				//DbgPrint("title %s",title);
				OPENFILENAME ofn;
                memset(&ofn, 0, sizeof(OPENFILENAME));
                ofn.lStructSize = sizeof(OPENFILENAME);
                ofn.hwndOwner = hWnd;
                ofn.hInstance = mainInstance;
                ofn.nFilterIndex = 1;
                ofn.lpstrFile = retFile;
                ofn.nMaxFile = 256;
                ofn.lpstrInitialDir = tip;
                //====
                ofn.lpstrFilter = filter;
                ofn.lpstrTitle  = title;
                strcpy( retFile, tip );
                bool status;
                if( save )
				{
					ofn.Flags = OFN_OVERWRITEPROMPT;
					status =  (GetSaveFileName(&ofn)!=0);
				} 
				else
				{
					ofn.Flags = OFN_FILEMUSTEXIST;
					status = (GetOpenFileName(&ofn)!=0);
				}

				if(!status)
				{

					return 0; 
				}

//				CODEREPLACE_END

				//////////////////////////////
			}
			//VMProtectEnd
			
			//VMProtectBegin
			file = fopen(hookINI,"w");
			//VMProtectEnd


			if(file)
			{
				fprintf(file,"%s\x0D\x0A",hlEXE);
				fclose(file);
			} 
			else 
			{
				//VMProtectBegin
				//MessageBox(0,hookINI,"ini写入错误",0);//ini寫入錯誤
				//VMProtectEnd
				break;
			}

			//VMProtectBegin
//			if(handle!=INVALID_HANDLE_VALUE)
			{
				success = true;
			}
			//VMProtectEnd
		}
		else
		{
			success = true;
		}

	} while(!success);

	//VMProtectBegin
	if( !strcmpi(hlEXE,hookEXE) )
	{
		MessageBox(0,"不能启动","错误",MB_ICONEXCLAMATION);//"不能啟動"
		DeleteFile(hookINI);
		return 1;
	}
	//VMProtectEnd


	///////////////////////////////////////////

//	GetModuleFileName(0,ogcdir,255);
//	char* position = ogcdir+strlen(ogcdir);
//	while(position>=ogcdir && *position!='\\') --position;
//	position[1]=0;

	char* position = hookEXE+len;
	while(position>=hookEXE && *position!='\\') --position;
	position[1]=0;

	char filepath[400];
	//CODEREPLACE_START
//	sprintf(filepath,"%s\\%s",ogcdir,dll_name);
	sprintf(filepath,"%s\\%s",hookEXE,dll_name);
	//CODEREPLACE_END



	strcpy(hlCmdLine," -game cstrike ");


	strcat(hlCmdLine,lpCmdLine);
	ZeroMemory(&SI,sizeof(STARTUPINFO));
	ZeroMemory(&PI,sizeof(PROCESS_INFORMATION));
	SI.cb = sizeof(STARTUPINFO);


	///////////////////////////////////////////////////

	char hlDir[400];
	strcpy(hlDir,hlEXE);
	char* pos = hlDir+strlen(hlDir);
	while(pos>=hlDir && *pos!='\\') --pos;
	*pos = 0;
	char* hlBaseFileName = pos+1;
	SetCurrentDirectory(hlDir);


	/////////////////////

	if (!CreateProcess(hlBaseFileName,hlCmdLine,NULL,NULL,FALSE,CREATE_SUSPENDED,NULL,NULL,&SI,&PI))
	{
		//VMProtectBegin
		//MessageBox(0,"无法注入","[飓风路线]",MB_ICONERROR|MB_TOPMOST);//無法注入","[幽靈路線]
		//VMProtectEnd
		//VMProtectBegin
		return -1;
		//VMProtectEnd
	}

	//VMProtectBegin
	if( !bInjectLibrary(PI.hProcess, filepath ))
	{
		//VMProtectBegin
		//MessageBox(0,"注入失败","[飓风路线]",MB_ICONERROR|MB_TOPMOST);//注入失敗","[幽靈路線]
		//VMProtectEnd
		//VMProtectBegin
		TerminateProcess(PI.hProcess,-1);
		//VMProtectEnd
		//VMProtectBegin
		return -1;
		//VMProtectEnd
	}
	//VMProtectEnd

	//VMProtectBegin
	ResumeThread(PI.hThread);
	//VMProtectEnd
	//VMProtectBegin
	CloseHandle(PI.hProcess);
	//VMProtectEnd
	CloseHandle(PI.hThread);
	return 0;
}

//===========================================================================================================================================================

