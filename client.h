//==============================================================================
// client.h
//==============================================================================

//==============================================================================
#include "engine/wrect.h"
#include "engine/cl_dll.h"
#include "engine/cdll_int.h"
#include "engine/const.h"
#include "engine/progdefs.h"
#include "engine/eiface.h"
#include "engine/edict.h"
#include "engine/studio_event.h"
#include "engine/entity_types.h"
#include "engine/r_efx.h"
#include "engine/pmtrace.h"
#include "engine/studio.h"
#include "common/ref_params.h"
#include "common/screenfade.h"
#include "common/event_api.h"
#include "common/com_model.h"
#include "misc/parsemsg.h"
#include "misc/sprites.h"
#include "misc/r_studioint.h"
#include "engine/triangleapi.h"
#include "engine/pm_defs.h"

#include <windows.h>
#include <mmsystem.h>
#include <memory.h>
#include <vector>
#include <string>
#include <tlhelp32.h>
#include <fstream>
#include <iostream>
#include "color.h"

using namespace std;
float relativeSpeed();
//==============================================================================
extern struct cl_enginefuncs_s gEngfuncs;
extern struct engine_studio_api_s oEngStudio;
//==============================================================================
#ifndef CDLL_INT_H
typedef struct
{
	char *name;
	char *model;

} hud_player_info_t;
struct cl_entity_s * getEnt() { return gEngfuncs.GetEntityByIndex(entindex); }
#endif

void DrawCrosshair(void);
int getSeqInfo(int ax);
static void AddEntityPlayer(struct cl_entity_s *ent);
static void AddEntityWeapon (struct cl_entity_s *ent, const char *mdlname, bool special);
static void AddEntityHostage(struct cl_entity_s *ent);
static void updateLocalPlayer();
static void drawPlayerEsp(int ax);
static void drawEsp(int ab);
int	HookUserMsg (char *szMsgName, pfnUserMsgHook pfn);
void hlexec (const char *fmt, ... );
int DoSpeed(int eventcode);
void AtMapChange();
void InitMenu();
static ColorEntry* PlayerColor(int ax);
int KeyMenuEvent(int keynum);
void FixupAngleDifference(usercmd_t *cmd);
void Con_Echo(const char *fmt, ... );
void HandleCvarFloat(char* name, float* value);
void HandleCvarInt(char* name, int* value);
void HlEngineCommand(const char* command);
bool isHlCvar(char* name);
void HandleHlCvar(char* name);
bool fileExists(const char* filename);
inline std::string getHackDirFile(const char* basename);
char* gGetWeaponName( int weaponmodel );
client_sprite_t* __stdcall hookpfnSPR_GetList(char *psz, int *piCount);
string avadd(int i, float h, float f, float r);
void exec(const char* filename);
bool __fastcall CalcScreen(float *origin, float *vecScreen);
extern SCREENINFO screeninfo;
void glFillArea(float x, float y, int w, int h, int r, int g, int b, int a, int rr, int gg, int bb, int aa);
void SPR_DrawAdditive( int frame, int x, int y, const wrect_t *prc );
void SPR_Set (HSPRITE hPic, int r, int g, int b);
int	DrawCharacter( int x, int y, int number, int r, int g, int b );
void FillRGBA( int x, int y, int width, int height, int r, int g, int b, int a );
void StudioDrawPoints();
extern bool ActivateEngineStudio( void );
void DrawHudStringLimit(int x, int y, int lim, int r, int g, int b, const char *fmt, ... );
void DrawGlowHudString(int x, int y, int r, int g, int b, const char *fmt, ...);
//==============================================================================
