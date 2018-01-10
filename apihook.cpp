//==============================================================================
// apihook.cpp
// credits to ogc
//==============================================================================


//==============================================================================
#include "apihook.h"
#define MakePtr(cast, ptr, addValue)(cast)((DWORD)(ptr)+(DWORD)(addValue))
//==============================================================================


//==============================================================================
void *InterceptDllCall(HMODULE hModule, char *szDllName, char *szFunctionName, DWORD pNewFunction)
{
  PIMAGE_DOS_HEADER pDosHeader;
  PIMAGE_NT_HEADERS pNTHeader;
  PIMAGE_IMPORT_DESCRIPTOR pImportDesc;
  PIMAGE_THUNK_DATA pThunk;
  DWORD dwOldProtect;
  DWORD dwOldProtect2;
  void *pOldFunction;

  if (!(pOldFunction=GetProcAddress(GetModuleHandle(szDllName), szFunctionName)))
  {
    return 0;
  }

  pDosHeader=(PIMAGE_DOS_HEADER)hModule;
  if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
  {
    return NULL;
  }

  pNTHeader=MakePtr(PIMAGE_NT_HEADERS, pDosHeader, pDosHeader->e_lfanew);
  if (pNTHeader->Signature!=IMAGE_NT_SIGNATURE || (pImportDesc=MakePtr(PIMAGE_IMPORT_DESCRIPTOR, pDosHeader, pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress))==(PIMAGE_IMPORT_DESCRIPTOR)pNTHeader)
  {
    return NULL;
  }

  while (pImportDesc->Name)
  {
    char *szModuleName=MakePtr(char *, pDosHeader, pImportDesc->Name);
    if (!stricmp(szModuleName, szDllName))
    {
      break;
    }
    pImportDesc++;
  }
  if (pImportDesc->Name==NULL)
  {
    return NULL;
  }

  pThunk=MakePtr(PIMAGE_THUNK_DATA, pDosHeader, pImportDesc->FirstThunk);
  while (pThunk->u1.Function)
  {
    if (pThunk->u1.Function==(PDWORD)pOldFunction)
    {
      VirtualProtect((void *)&pThunk->u1.Function, sizeof(PDWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect);
      pThunk->u1.Function=(PDWORD)pNewFunction;
      VirtualProtect((void *)&pThunk->u1.Function, sizeof(PDWORD), dwOldProtect, &dwOldProtect2);
      return pOldFunction;
    }
    pThunk++;
  }
  return NULL;
}
//==============================================================================