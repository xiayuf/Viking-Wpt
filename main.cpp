//==============================================================================
// main.cpp
//==============================================================================


//==============================================================================
#include <windows.h>
#include <tlhelp32.h>
#include "apihook.h"
#include "gpatch.h"
#include "client.h"
#include "opengl.h"
#include "bind.h"
#include "xorstr.h"
//==============================================================================


//==============================================================================
void ApplyClientGateways(void);
typedef BOOL (WINAPI *QueryPerformanceCounterFunc)(LARGE_INTEGER*);
QueryPerformanceCounterFunc QueryPerformanceCounterPtr=NULL;
FARPROC (WINAPI *pGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);
extern char hackdir[256];
//==============================================================================


//==============================================================================
BOOL WINAPI newQueryPerformanceCounter(LARGE_INTEGER *lp)
{
	BOOL  ret=(*QueryPerformanceCounterPtr)(lp);
	ApplyClientGateways();

	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

GenericPatch soundPatch;
static void* stackret=0; 
static DWORD sound_reloc_addr1=0;
void PreS_DynamicSound(const DWORD a,const DWORD entindex,const DWORD c,const char*const sample,const float*const origin,DWORD f,const DWORD g,const DWORD h);

////////////////////////////////////////////////////////////////////////////////////////////////
FARPROC WINAPI newGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{
	FARPROC nResult;
	nResult=GetProcAddress(hModule, lpProcName);

	static bool ptr_done=false;
	if (!ptr_done)
	{
		ptr_done=StoreOffsets();
	}

	if (HIWORD(lpProcName))
	{
		if (!lstrcmp(lpProcName, "GetProcAddress"))
		{
			return (FARPROC) &newGetProcAddress;
		}
		else if (!strcmp(lpProcName, "QueryPerformanceCounter"))
		{
		  QueryPerformanceCounterPtr=(QueryPerformanceCounterFunc)GetProcAddress(hModule, lpProcName);
			return (FARPROC) &newQueryPerformanceCounter;
    }
		if (ptr_done)
		{
      ApplyOpenGLHook(&nResult, lpProcName);
    }
	}
	return nResult;
}
//==============================================================================
__declspec(naked) void JumpGate_DynamicSound()
{
    __asm{
        // 8 args on stack
		call PreS_DynamicSound;  

        // exec caller instructions & jump back
        sub esp,0x48;
        
        mov  eax, soundPatch.patchloc;
        add  eax, soundPatch.patchsize;
        push eax;
        
        mov  eax,sound_reloc_addr1;
        mov  eax,[eax];

        ret;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//void OGCmessage(const char *fmt, ... );
bool bDataCompare(const BYTE* pData, const BYTE* bMask, const char* szMask) 
{ 
    for(;*szMask;++szMask,++pData,++bMask) 
        if(*szMask=='x' && *pData!=*bMask ) 
            return false; 
    return (*szMask) == NULL; 
} 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DWORD dwFindPattern(DWORD dwAddress,DWORD dwLen,BYTE *bMask,char * szMask) 
{ 
    for(DWORD i=0; i < dwLen; i++) 
        if( bDataCompare( (BYTE*)( dwAddress+i ),bMask,szMask) ) 
            return (DWORD)(dwAddress+i); 
    
    return 0; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool detour_S_DynamicSound()
{
        static bool applied = false;
        if( applied )   { return true; }
		applied = true;
	    //assert(stackret);

		DWORD SOUND_ADDR = dwFindPattern(0x1D90000,0x30000,(BYTE*)"\x83\xEC\x48\xA1\x00\x00\x00\x00","xxxx???");
		BYTE* offs = (BYTE*) SOUND_ADDR;
 
		BYTE* sig   = (BYTE*)"\x83\xEC\x48\xA1\x00\x00\x00\x00";
		char* mask  = "xxxx????";

		if(!soundPatch.find_location(offs,sig,mask))
		{
           //OGCmessage("&rsound patch failed. 0x%x", stackret);
           return false;
		}
		
		// prepare patch:
		soundPatch.patchdata[0]=0x68;
		*(DWORD*)(soundPatch.patchdata+1) = (DWORD)JumpGate_DynamicSound;
		soundPatch.patchdata[5]=0xC3;
		soundPatch.patchdata[6]=0x90;
		soundPatch.patchdata[7]=0x90;

        // relocated addr needed in detour
		sound_reloc_addr1 = *(DWORD*)(soundPatch.backup+4);

		soundPatch.apply();
        //OGCmessage( "&gsound patch applied successfully\n");
        return true;
}

//==============================================================================
bool WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	char tmpName[255];
	GetModuleFileName(hinstDLL, tmpName, 255);
	strlwr(tmpName);
	if (!strstr(tmpName, /*eVOL.dll*/XorStr<0xE4,9,0x216A025C>("\x81\xB3\xA9\xAB\xC6\x8D\x86\x87"+0x216A025C).s))
	{
		char tmpa[100];
		sprintf(tmpa,"Sif change the dam name bitch, change it back NOW \n(%s)",tmpName);
//		MessageBox(NULL, tmpa, "Myth2 Hook v0.01", MB_OK);
//		ExitProcess(0);
	}

	if (fdwReason==DLL_PROCESS_ATTACH) 
	{ 
		GetModuleFileName((HINSTANCE)hinstDLL,hackdir,255);
		char* pos = hackdir+strlen(hackdir);
		while(pos>=hackdir && *pos!='\\') --pos;
		pos[1]=0;

		pGetProcAddress=(FARPROC(WINAPI *)(HMODULE, LPCSTR))InterceptDllCall(GetModuleHandle(NULL), "Kernel32.dll", "GetProcAddress", (DWORD)&newGetProcAddress);
	}
	return true;
}
//==============================================================================