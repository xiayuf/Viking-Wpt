//==============================================================================
// gateways.cpp
//==============================================================================

#pragma warning(disable:4800)
#include <windows.h>
#include <mmsystem.h>
#include <memory.h>
#include <tlhelp32.h>
#include <fstream>
#include <iostream>
#include "client.h"
#include "gateways.h"
#include "cvars.h"
#include "console.h"
#include "bind.h"
#include "xorstr.h"
#include "gpatch.h"
#include "players.h"
//==============================================================================
int AddEntResult=1;
int KeyEventResult=1;

//==============================================================================

extern GenericPatch soundPatch;

extern bool detour_S_DynamicSound();

////////////////////////////
cl_enginefuncs_s *pEngfuncs;
engine_studio_api_s *pstudio;
DWORD *ltfxslots;
DWORD ltfx_loc;
DWORD *RetartPtr;
DWORD *tempslots=(DWORD*)0x01700000; 
DWORD *mempatcher=0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AutoOffsets() 
{ 
   static bool NotPatched = true; 
   if (NotPatched) 
   { 
      int a; 
      for(a=0x01D00000;a<0x01D0FFFF;a++) 
      { 
         if(*(PBYTE)(a) == 0x68 && *(PBYTE)(a + 0x05) == 0xE8 && 
         *(PBYTE)(a + 0x0A) == 0x6A && *(PBYTE)(a + 0x0B) == 0x07 && 
         *(PBYTE)(a + 0x0C) == 0x68 && *(PBYTE)(a + 0x11) == 0xFF && 
         *(PBYTE)(a + 0x12) == 0x15 && *(PBYTE)(a + 0x17) == 0x68) 
         { 
            pEngfuncs = (cl_enginefunc_t*)*(DWORD*)(a+0x0D); 
         } 
      }
      int c; 
      for(c=0x01D00000;c<0x01D0FFFF;c++) 
      { 
         if(*(PBYTE)(c) == 0x68 && *(PBYTE)(c + 0x05) == 0x68 && 
         *(PBYTE)(c + 0x0A) == 0x6A && *(PBYTE)(c + 0x0B) == 0x01 && 
         *(PBYTE)(c + 0x0C) == 0xFF && *(PBYTE)(c + 0x0D) == 0xD0 && 
         *(PBYTE)(c + 0x0E) == 0x83 && *(PBYTE)(c + 0x27) == 0xC3) 
         { 
            pstudio = (engine_studio_api_s*)*(DWORD*)(c+0x01); 
         } 
      } 
      int d; 
      for(d=0x01D60000;d<0x01D6FFFF;d++) 
      { 
         if(*(PBYTE)(d) == 0x68 && *(PBYTE)(d + 0x05) == 0xFF && 
         *(PBYTE)(d + 0x06) == 0x15 && *(PBYTE)(d + 0x0B) == 0x68 && 
         *(PBYTE)(d + 0x10) == 0xFF && *(PBYTE)(d + 0x11) == 0x15 && 
         *(PBYTE)(d + 0x16) == 0xA1 && *(PBYTE)(d + 0x1B) == 0x83) 
         { 
             ltfxslots = (DWORD*)(*(DWORD*)(d+0x01)); 
			 ltfx_loc = (DWORD)ltfxslots;
         } 
      } 

	  if( ((DWORD*)pEngfuncs <= (DWORD*)(0x01EB52B8)) )//RetartPtr
	  {
	      RetartPtr = (DWORD*)0x01A179D0;
	  }
	  else
	  {
		  RetartPtr = (DWORD*)0x01A179F0;
	  } 
        
      NotPatched = false; 
   } 
}

//==============================================================================
DWORD retaddress;
__declspec(naked) void Gateway2_HUD_Init(void)
{
	__asm
	{
		call HUD_Init;
		jmp retaddress;
	}
}
//==============================================================================
DWORD Init=(DWORD)&Gateway2_HUD_Init;
__declspec(naked) void Gateway1_HUD_Init(void)
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x08];
		mov retaddress,esi;
		push Init
		pop esi;
		mov dword ptr ss:[esp+0x08],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_V_CalcRefdef(void)
{
	__asm
	{
		call PostV_CalcRefdef;
		jmp retaddress;
	}
}
//==============================================================================
DWORD CalcRef=(DWORD)&Gateway2_V_CalcRefdef;
__declspec(naked) void Gateway1_V_CalcRefdef(void)
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		push esi;
		call PreV_CalcRefdef;
		add esp,4;
		mov esi,dword ptr ss:[esp+0x0c];
		mov retaddress,esi;
		push CalcRef;
		pop esi;
		mov dword ptr ss:[esp+0x0c],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_CL_CreateMove(void)
{
	__asm
	{
		call CL_CreateMove;
		jmp retaddress;
	}
}
//==============================================================================
DWORD CreateMove=(DWORD)&Gateway2_CL_CreateMove;
__declspec(naked) void Gateway1_CL_CreateMove(void)
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x28];
		mov retaddress,esi;
		push CreateMove;
		pop esi;
		mov dword ptr ss:[esp+0x28],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_HUD_Redraw(void)
{
	__asm
	{
		call HUD_Redraw;
		jmp retaddress;
	}
}
//==============================================================================
DWORD Redraw=(DWORD)&Gateway2_HUD_Redraw;
__declspec(naked) void Gateway1_HUD_Redraw(void)
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		mov retaddress,esi;
		push Redraw
		pop esi;
		mov dword ptr ss:[esp+0x10],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_HUD_PlayerMove(void)
{
	__asm
	{
		call HUD_PlayerMove;
		jmp retaddress;
	}
}
//==============================================================================
DWORD PlayerMove=(DWORD)&Gateway2_HUD_PlayerMove;
__declspec(naked) void Gateway1_HUD_PlayerMove(void)
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x10];
		mov retaddress,esi;
		push PlayerMove
		pop esi;
		mov dword ptr ss:[esp+0x10],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_HUD_AddEntity(void)
{
	__asm
	{
		mov AddEntResult, eax;
		call HUD_AddEntity;
		mov eax, AddEntResult;
		jmp retaddress;
	}
}
//==============================================================================
DWORD AddEnt=(DWORD)&Gateway2_HUD_AddEntity;
__declspec(naked) void Gateway1_HUD_AddEntity(void)
{	
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x14];
		mov retaddress,esi;
		push AddEnt
		pop esi;
		mov dword ptr ss:[esp+0x14],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_HUD_Key_Event(void)
{
	__asm
	{
		mov KeyEventResult, eax;
		call HUD_Key_Event;
		mov eax, KeyEventResult;
		jmp retaddress;
	}
}
//==============================================================================
DWORD KeyEvent=(DWORD)&Gateway2_HUD_Key_Event;
__declspec(naked) void Gateway1_HUD_Key_Event(void)
{
	__asm
	{
		push esi;
		mov esi,dword ptr ss:[esp+0x14];
		mov retaddress,esi;
		push KeyEvent
		pop esi;
		mov dword ptr ss:[esp+0x14],esi;
		pop esi;
		ret;
	}
}
//==============================================================================	
__declspec(naked) void Gateway2_HUD_PostRunCmd(void)
{
	__asm
	{
		nop;
		nop; 
		nop;
		call HUD_PostRunCmd;
		nop;
		nop;
		nop;
		jmp retaddress;
	}
}
//==============================================================================
DWORD PostRunCmd=(DWORD)&Gateway2_HUD_PostRunCmd;
__declspec(naked) void Gateway1_HUD_PostRunCmd(void)
{
	__asm
	{
		nop;
		nop;
		nop;
		push esi;
		mov esi,dword ptr ss:[esp+0x38];
		mov retaddress,esi;
		push PostRunCmd
		pop esi;
		mov dword ptr ss:[esp+0x38],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
__declspec(naked) void Gateway2_HUD_TempEntUpdate(void)
{
	__asm
	{
		nop;
		nop;
		nop;
		call HUD_TempEntUpdate;
		nop;
		nop;
		nop;
		jmp retaddress;
	}
}
//==============================================================================
DWORD TempEntUpdate=(DWORD)&Gateway2_HUD_TempEntUpdate;
__declspec(naked) void Gateway1_HUD_TempEntUpdate(void)
{
	__asm
	{
		nop;
		nop;
		nop;
		push esi;
		mov esi,dword ptr ss:[esp+0x100];
		mov retaddress,esi;
		push TempEntUpdate;
		pop esi;
		mov dword ptr ss:[esp+0x100],esi;
		pop esi;
		ret;
	}
}
//==============================================================================
int iUpdateResult;
DWORD dwRetAddr;
__declspec( naked ) void Gateway2_HUD_UpdateClientData()
{
	_asm	
	{
		mov	[iUpdateResult],eax;
		call HUD_UpdateClientData;
		mov	eax,[iUpdateResult];
		jmp	[dwRetAddr];
	}
}
//==============================================================================
DWORD dwUpdateClD = (DWORD)&Gateway2_HUD_UpdateClientData;
__declspec( naked ) void Gateway1_HUD_UpdateClientData()
{
	_asm	
	{
		push esi;
		mov	esi,[esp+0x10];
		mov	[dwRetAddr],esi;
		push [dwUpdateClD];
		pop	esi;
		mov	[esp+0x10],esi;
		pop	esi;
		retn;
	}
}
//==============================================================================
void ApplyClientPatch(void)
{
	tempslots[1]=(DWORD)&Gateway1_HUD_Init;
	tempslots[3]=(DWORD)&Gateway1_HUD_Redraw;
	tempslots[4]=(DWORD)&Gateway1_HUD_UpdateClientData;
	tempslots[6]=(DWORD)&Gateway1_HUD_PlayerMove;
	tempslots[14]=(DWORD)&Gateway1_CL_CreateMove;
	tempslots[19]=(DWORD)&Gateway1_V_CalcRefdef;
	tempslots[20]=(DWORD)&Gateway1_HUD_AddEntity;
	tempslots[25]=(DWORD)&Gateway1_HUD_PostRunCmd;
	tempslots[34]=(DWORD)&Gateway1_HUD_Key_Event;
	tempslots[35]=(DWORD)&Gateway1_HUD_TempEntUpdate;
}
//==============================================================================
bool bFirstTime=true;
bool bStudioFirst=true;
bool bEventApi=true;
//==============================================================================
void CB_KeyManager_ConType(char ch);
void CB_KeyManager_Exec(const string& commandlist);
void CommandInit();
void ApplyClientGateways(void)
{
    AutoOffsets();

	if (pEngfuncs->pfnHookUserMsg && bFirstTime)
	{
		memcpy(&gEngfuncs, pEngfuncs, sizeof(cl_enginefunc_t));	
		pEngfuncs->pfnHookUserMsg = &HookUserMsg;
		pEngfuncs->pfnSPR_Set = &SPR_Set;
		pEngfuncs->pfnSPR_DrawAdditive = &SPR_DrawAdditive;
		pEngfuncs->pfnDrawCharacter = &DrawCharacter;
		pEngfuncs->pfnFillRGBA = &FillRGBA;
		keyBindManager.init();
		keyBindManager.CallBack_Execute = CB_KeyManager_Exec;
		keyBindManager.CallBack_ConType = CB_KeyManager_ConType;
		keyBindManager.con_visible = gEngfuncs.Con_IsVisible;

		gConsole.setcolortag( 'b', 128,128,255);
		gConsole.setcolortag( 'r', 255,128,128);
		gConsole.setcolortag( 'g',	80,222,140);
		gConsole.setcolortag( 'w', 255,255,255);
		gConsole.setcolortag( 'y', 255,255, 50);
		gConsole.setcolortag( 'o', 255,132,	0);
		detour_S_DynamicSound();
		soundPatch.apply();
		CommandInit();

		bFirstTime=false;
	}

	if (pstudio->GetModelByIndex && bStudioFirst)
	{
		bStudioFirst=false;
		memcpy(&oEngStudio, pstudio, sizeof(engine_studio_api_t));
	}
	
	static bool patched = false;
	static bool bDoneSlots = false;
	unsigned char *c;

	if (!patched && !bFirstTime)
	{
		patched = true;
	
		if (bDoneSlots)
		{
			DWORD *memp = mempatcher;
			if (memp[0] == ltfx_loc)
			{
				bDoneSlots = false;
			}
			else
			{
				patched = false;
			}
		}
		else
		{
			for (int s = 0; s < 40; s++)
			{
				c = (unsigned char *) ltfxslots[s];
				if (c[0] != 0xc3)
				{
					patched = false;
					break;
				}
			}
		}
	}

	if (patched)
	{
		if (RetartPtr[0] == ltfx_loc)
		{
			mempatcher = RetartPtr;
		}
			
		if (mempatcher)
		{
			memcpy(tempslots, ltfxslots, 43*4);
			mempatcher[0] = (DWORD)tempslots;
			bDoneSlots = true;
		}

		ApplyClientPatch();
	}

	patched = false;
}
//==============================================================================


