//==============================================================================
// client.cpp
//==============================================================================

#pragma warning(disable:4800)
#include <list>
#include <time.h>
#include "client.h"
#include "calcscreen.h"
#include "cvars.h"
#include "drawing.h"
#include "color.h"
#include "timehandling.h"
#include "Aimbot.h"
#include "weaponslist.h"
#include "nospread.h"
#include "console.h"
#include "interpreter.h"
#include "tinystrings.h"
#include "gpatch.h"
#include "bind.h"
#include "common/net_api.h"
#include "players.h"
#include "xorstr.h"
#include "menu.h"
#include "misc\defs.h"
#include <gl/gl.h>
#include "engine\cdll_int.h"
#include "waypoint.h"
//#include "autoroute.h" //max
#include "attack.h"
#include "idhook.h"
#include "autodirection.h"
#include "gui2.h"
#define PI		3.14159265358979323846
//==============================================================================
struct cl_enginefuncs_s gEngfuncs;
struct engine_studio_api_s oEngStudio;
engine_studio_api_s* pStudio=NULL;
extern cl_enginefuncs_s *pEngfuncs;
double* globalTime;
//==============================================================================
int beamindex = 0;
//==============================================================================
//==============================================================================
extern GenericPatch soundPatch;
//==============================================================================
SCREENINFO screeninfo;
char hackdir[256] = "";
extern int AddEntResult;
extern int KeyEventResult;
extern bool oglSubtractive;
//==============================================================================
enum { FIRESEQ_INACTIVE     = 0, 
       FIRESEQ_STARTDELAY   = 1, 
	   FIRESEQ_ACTIVE       = 2,
	   FIRESEQ_RELEASEDELAY = 3 };
BurstInfo burstInfo;
static bool bAttacking = false;
static bool bAttack2 = false;
static int  nFireSequence = FIRESEQ_INACTIVE;
static bool bFireCycle  = true;
bool bStopMovement=false;
float mainViewAngles_SinYaw;
float mainViewAngles_CosYaw;
//==============================================================================
int Cstrike_SequenceInfo[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0,  
	1, 2, 0, 1, 1, 2, 0, 1, 1, 2,  
	0, 1, 2, 0, 1, 2, 0, 1, 2, 0,  
	1, 2, 0, 1, 2, 0, 1, 2, 0, 1,  
	2, 0, 1, 2, 0, 0, 0, 4, 0, 4, 
	0, 5, 0, 5, 0, 0, 1, 1, 2, 0, 
	1, 1, 2, 0, 1, 0, 1, 0, 1, 2, 
	0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
	3
};

//gait sequence stuff 
int Cstrike_GaitSequenceInfo[] = { 0,0,1,2,3,4,5,5,6,6 };

enum {
	SEQUENCE_STANDING = 0, 
	SEQUENCE_CROUCHING = 1, 
	SEQUENCE_WALKING = 2, 
	SEQUENCE_RUNNING = 3, 
	SEQUENCE_CROUCH_RUNNING = 4, 
	SEQUENCE_JUMPING = 5, 
	SEQUENCE_SWIMMING = 6 
};

// 85/86 == Grenade Throw with shield
// 91/92 == Shoot with shield
// 89/93/94/95/96 == Shield to side
// 97/98 == Shield in front

// esp Info Structure
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
#define MAX_VALUE(a,b,c) ( a>b? (a>c?a:(b>c?b:c)) : (b>c?b:c) )
void overview_calcRadarPoint(const float *origin,int &screenx,int &screeny);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static ColorEntry dummycolor;

class SoundMarker
{
public:
	SoundMarker() : color(&dummycolor),priority(0) {}
	float        origin[3];
	char        description[16];
	int         team;
	int         index;
	StopTimer       timer;
	ColorEntry*       color;
	int         priority;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef vector<SoundMarker>  SoundMarkerVector;
SoundMarkerVector vecSoundMarker(65);
int soundMarkerCurrentIndex = 0;
struct esp_info
{
	esp_info() { memset(this,0,sizeof(*this)); }

	char        name[16];
	float       origin[3];
	bool        blink;
	ColorEntry* color;
};
// esp Info vector
enum { MAX_ESPINFO=32 };
static esp_info vEspInfo[MAX_ESPINFO];
static int      espInfoCount=0;
bool menu_active = false;
bool sniper=false;
bool gMenuActive(){ return menu_active; }
//∂≥Ω·
bool isInFrozenTime = false;
float wayrec = 0;
float wayrecstp = 0;
float wayclr1 = 0;
float wayclr2 = 0;
float wayclr3 = 0;
float wayclr4 = 0;
float wayclr5 = 0;
float wayclr6 = 0;
float wayclr7 = 0;
float wayclr8 = 0;
float wayclr9 = 0;
float wayclr10 = 0;
float wayclrall = 0;
float savepistol = 0;
float avchange_hups = 0;
float avchange_hdowns = 0;
float avchange_fups = 0;
float avchange_fdowns = 0;
float avchange_rups = 0;
float avchange_rdowns = 0;
float avchange_hupd = 0;
float avchange_hdownd = 0;
float avchange_fupd = 0;
float avchange_fdownd = 0;
float avchange_rupd = 0;
float avchange_rdownd = 0;
float avchange_hupj = 0;
float avchange_hdownj = 0;
float avchange_fupj = 0;
float avchange_fdownj = 0;
float avchange_rupj = 0;
float avchange_rdownj = 0;

float avchange_hups1 = 0;
float avchange_hdowns1 = 0;
float avchange_fups1 = 0;
float avchange_fdowns1 = 0;
float avchange_rups1 = 0;
float avchange_rdowns1 = 0;
float avchange_hupd1 = 0;
float avchange_hdownd1 = 0;
float avchange_fupd1 = 0;
float avchange_fdownd1 = 0;
float avchange_rupd1 = 0;
float avchange_rdownd1 = 0;
float avchange_hupj1 = 0;
float avchange_hdownj1 = 0;
float avchange_fupj1 = 0;
float avchange_fdownj1 = 0;
float avchange_rupj1 = 0;
float avchange_rdownj1 = 0;

float avchange_hups2 = 0;
float avchange_hdowns2 = 0;
float avchange_fups2 = 0;
float avchange_fdowns2 = 0;
float avchange_rups2 = 0;
float avchange_rdowns2 = 0;
float avchange_hupd2 = 0;
float avchange_hdownd2 = 0;
float avchange_fupd2 = 0;
float avchange_fdownd2 = 0;
float avchange_rupd2 = 0;
float avchange_rdownd2 = 0;
float avchange_hupj2 = 0;
float avchange_hdownj2 = 0;
float avchange_fupj2 = 0;
float avchange_fdownj2 = 0;
float avchange_rupj2 = 0;
float avchange_rdownj2 = 0;
// Blinking for Radar/Esp
StopTimer blinkTimer;
bool blinkPhase = false;
//
static char currentMap[100];

// Scheduled Commands (later x blabla)
struct ScheduledCommand
{
	string           cmd;
	RunningStopTimer timer;
};
typedef list<ScheduledCommand> ScheduleList;
ScheduleList scheduleList;

float chaseofs = 0;

static bool bGotHeadshot=false;

char gWelcomeMessage[256]="";
StopTimer tWelcomeMessage;

bool VISIBLE;

char  gHudMessage2[256]="";
StopTimer gHudTimer2;

//Handle Speed
float gSpeed;

// spam and flood scripts;
bool floodoff = true;
bool spamoff = true;
float flood = 0;
float spam = 0;

//Hack

usercmd_t g_Originalcmd;

//==============================================================================

//==============================================================================
#define BOUND_VALUE(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}
//==============================================================================

//==============================================================================
bool __fastcall CalcScreen(float *origin, float *vecScreen)
{
	if(!cvar.chase_cam)
	{
		return NewCalcScreen(origin, vecScreen);
	}
	else if(cvar.chase_cam)
	{
		int cResult = 0;
		if( gEngfuncs.GetLocalPlayer() != NULL )
			cResult = gEngfuncs.pTriAPI->WorldToScreen( origin, vecScreen );
		if( vecScreen[0] < 1 && vecScreen[1] < 1 && vecScreen[0] > -1 && vecScreen [1] > -1 && !cResult )
		{
			vecScreen[0] = vecScreen[0] * displayCenterX + displayCenterX;
			vecScreen[1] = -vecScreen[1] * displayCenterY + displayCenterY;
			return true;
		}
		return NULL;
	}
	return NULL;
}
//==============================================================================


//==============================================================================
int HUD_Init(void)
{
  return 1;
}
//==============================================================================

//Scroll Bar Sys ===============================================================
void UpdateScrollbarCon()
{
	if( GetAsyncKeyState(VK_ADD) )
	{
		gConsole.scrollbar++;
	}

	if( GetAsyncKeyState(VK_SUBTRACT) )
	{
		gConsole.scrollbar--;
	}
}	
//==============================================================================
void exec(const char* filename)
{
	string file = getHackDirFile(filename);
	cmd.execFile( file.c_str() );
}
void overview_calcRadarPoint(const float* origin, int& screenx, int& screeny)
{
	float dx  = origin[0] - me.pmEyePos[0];
	float dy  = origin[1] - me.pmEyePos[1];
	float range = 2500.0f;

    // rotate
	float x =  dx*  mainViewAngles_SinYaw   -  dy*mainViewAngles_CosYaw;
	float y =  dx*(-mainViewAngles_CosYaw)  -  dy*mainViewAngles_SinYaw;

	
	screenx = displayCenterX+int(x/range*float(cvar.radar_size));
	screeny = displayCenterY+int(y/range*float(cvar.radar_size));
	
}
//==============================================================================
void drawSound()
{
	for(int pri=0;pri<3;pri++)
	for(int i=0;i<12;i++)
	{
		SoundMarker& ref = vecSoundMarker[i];
		if( ref.priority==pri && ref.timer.running() )
		{  
			ColorEntry* clr = colorList.get(20);

			if(cvar.sounddisplay)
			{
				float vecScreen[2];
				if (CalcScreen(ref.origin,vecScreen))
				{
					DrawConStringCenter(vecScreen[0],vecScreen[1],ref.color->r,ref.color->g,ref.color->b,ref.description);
				}
			}
			if(cvar.soundradar)
			{
				int screenx, screeny;
				overview_calcRadarPoint(ref.origin, screenx,screeny);
				gEngfuncs.pfnFillRGBA(screenx,screeny,2,2,ref.color->r,ref.color->g,ref.color->b,255);
			}
		}
	}
}
void DrawRadar()
{
	int size    = (int)cvar.radar_size;

	cvar.radar_x = displayCenterX;
	cvar.radar_y = displayCenterY;
	ColorEntry* clr = colorList.get(7);	
	gEngfuncs.pfnFillRGBA(cvar.radar_x,cvar.radar_y-size,1,2*size,clr->r,clr->g,clr->b,180);
	gEngfuncs.pfnFillRGBA(cvar.radar_x-size,cvar.radar_y,2*size,1,clr->r,clr->g,clr->b,180);
}
//==============================================================================
void calcRadarPoint(float* origin, int& screenx, int& screeny)
{
	float dx  = origin[0] - me.pmEyePos[0];
	float dy  = origin[1] - me.pmEyePos[1];

	float yaw = mainViewAngles[1]*(PI/180.0);
	me.sin_yaw       =  sin(yaw); 
	me.minus_cos_yaw = -cos(yaw);

	float x =  dy*me.minus_cos_yaw + dx*me.sin_yaw;
	float y =  dx*me.minus_cos_yaw - dy*me.sin_yaw;

	float range = 3500.0f;
	if(fabs(x)>range || fabs(y)>range)
	{ 
		// clipping
		if(y>x)
		{
			if(y>-x) {
				x = range*x/y;
				y = range;
			}  else  {
				y = -range*y/x; 
				x = -range; 
			}
		} else {
			if(y>-x) {
				y = range*y/x; 
				x = range; 
			}  else  {
				x = -range*x/y;
				y = -range;
			}
		}
	}
	screenx = displayCenterX+int(x/range*float(cvar.radar_size));
	screeny = displayCenterY+int(y/range*float(cvar.radar_size));		
}
void overview_calcRadarPoint(const float *origin,int &screenx,int &screeny);
void drawRadarPoint(float* origin,int r,int g,int b,int a,bool blink=false,int boxsize=3)
{
	int screenx;
	int screeny;
	{
		float dx  = origin[0] - me.pmEyePos[0];
		float dy  = origin[1] - me.pmEyePos[1];

		float yaw = mainViewAngles[1]*(3.1415/180.0);
		me.sin_yaw       =  sin(yaw); 
		me.minus_cos_yaw = -cos(yaw);

		// rotate
		float x =  dy*me.minus_cos_yaw + dx*me.sin_yaw;
		float y =  dx*me.minus_cos_yaw - dy*me.sin_yaw;

		float range = 2500.0f;
		if(fabs(x)>range || fabs(y)>range)
		{ 
			// clipping
			if(y>x)
			{
				if(y>-x) {
					x = range*x/y;
					y = range;
				}  else  {
					y = -range*y/x; 
					x = -range; 
				}
			} else {
				if(y>-x) {
					y = range*y/x; 
					x = range; 
				}  else  {
					x = -range*x/y;
					y = -range;
				}
			}
		}
		screenx = cvar.radar_x+int(x/range*float(cvar.radar_size));
		screeny = cvar.radar_y+int(y/range*float(cvar.radar_size));		
	}
		
	if(!blink || blinkPhase)
	{ 
		oglSubtractive = true;
		gEngfuncs.pfnFillRGBA(screenx-1,screeny-1,boxsize,boxsize,r,g,b,a);
		oglSubtractive = false;
	}
}
//==============================================================================
void DrawGlowHudString(int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	DrawHudString1(x-2,y,r,g,b,buf);
	DrawHudString1(x+2,y,r,g,b,buf);
	DrawHudString1(x,y+2,r,g,b,buf);
	DrawHudString1(x,y-2,r,g,b,buf);
	DrawHudString1(x-1,y-1,r,g,b,buf);
	DrawHudString1(x+1,y+1,r,g,b,buf);
	DrawHudString1(x+1,y-1,r,g,b,buf);
	DrawHudString1(x-1,y+1,r,g,b,buf);
	DrawHudString1(x,y,255,255,255,buf);
}
float relativeSpeed(void)
{
	float speed = sqrt(me.pmVelocity[0] * me.pmVelocity[0] + me.pmVelocity[1] * me.pmVelocity[1]);
	return speed;
}
//==============================================================================
static void gStopMovement()
{
	bStopMovement = true;
}
static void gFreeMovement()
{
	bStopMovement = false;	
}
void gAttackHandling()
{
	static double timer=0;

	if(!( me.alive && me.iClip && cvar.aim && cvar.shoot ) ){ return; }
    if(cvar.burstflags & BurstInfo::BURSTFLAG_NOAIM)
	{
		bAttacking = false;
		bFireCycle = true;
		nFireSequence = FIRESEQ_INACTIVE;
		return;
	}
 	if( !cvar.ftime ) { cvar.ftime = 0.1f; }
	if( cvar.burstflags&BurstInfo::BURSTFLAG_GROUND )
	{
		if(!(me.pmFlags&FL_ONGROUND))
		{
			bAttacking = false;
			nFireSequence = FIRESEQ_INACTIVE;
		}
	}
	if (Aimbot.iTarget!=-1)
	{
		switch (nFireSequence)
		{
		case FIRESEQ_INACTIVE:
			if( cvar.sdelay )
			{
				nFireSequence =  FIRESEQ_STARTDELAY;
				timer = ClientTime::current;
				gFreeMovement();

			} else {
				nFireSequence =  FIRESEQ_ACTIVE;
				timer = ClientTime::current;
				bFireCycle  = true;
				bAttacking = true;
			}
			break;
		case FIRESEQ_STARTDELAY:
			if( ClientTime::current >= (timer+cvar.sdelay) )
			{
				nFireSequence =  FIRESEQ_ACTIVE;
				timer = ClientTime::current;
				bFireCycle  = true;
				bAttacking = true;
			}
			break;
		case FIRESEQ_ACTIVE:
			if( bFireCycle )
			{
				if( (ClientTime::current >= (timer+cvar.ftime)) && (cvar.dtime!=0))
				{
					bAttacking = false;
					timer = ClientTime::current;
					bFireCycle = false;
					gFreeMovement();
				}

			} else {
				if( ClientTime::current >= (timer+cvar.dtime) )
				{
					bAttacking = true;
					timer = ClientTime::current;
					bFireCycle = true;
				}
			}
			break;
		case FIRESEQ_RELEASEDELAY:
			nFireSequence =  FIRESEQ_ACTIVE;
			timer = ClientTime::current;
			bFireCycle  = true;
			bAttacking = true;
			break;
		}
	} else {
		switch (nFireSequence)
		{
		case FIRESEQ_INACTIVE:
			break;
		case FIRESEQ_STARTDELAY:
			nFireSequence =  FIRESEQ_INACTIVE;
			gFreeMovement();
			break;
		case FIRESEQ_ACTIVE:
			gFreeMovement();
			if( !bFireCycle || (cvar.rdelay==0) )
			{
				bAttacking = false;
				nFireSequence =  FIRESEQ_INACTIVE;

			} else {
				timer = ClientTime::current;
				nFireSequence = FIRESEQ_RELEASEDELAY;
			}
			break;
		case FIRESEQ_RELEASEDELAY:
			gFreeMovement();
			if( ClientTime::current >= (timer+cvar.rdelay) )
			{
				bAttacking = false;
				nFireSequence =  FIRESEQ_INACTIVE;
			}
			break;
		}
	}
}

void DrawHudStringLimit(int x, int y, int lim, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];
	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	oglSubtractive = true;

	int length, height;
	gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
	x -= length/2;
	y += 4;
	//gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
	gEngfuncs.pfnDrawSetTextColor(r/lim, g/lim, b/lim);
	gEngfuncs.pfnDrawConsoleString(x,y,buf);

	oglSubtractive = false;
}
void SmokeEvent( event_args_t *args ) 
{ 
	return;
}
float fFade = 2.4f;;
bool bDisplayScroll = true;
void HUD_Redraw(float x, int y)
{
	if(cvar.way_enable) {waypoint.draw_text();waypoint.draw_text2();}
	if(waypoint.isRecord)
	{
		waypoint.record();
	}
//	if (cvar.way_switch && cvar.route_draw == 1){autoroute.DrawRouteLine();} //max

	//-------------------------¬∑ÔøΩﬂøÔøΩÔøΩÔøΩ Begin-------------------------------
	static bool a = false;
	if(!a)
	{
		int b = 32;
		BOUND_VALUE  (b,0,64);
		for( int i=0;i<b;i++)
		{
			vecSoundMarker[i].timer.setExpired();
		}
		a = true;
	}
	if(cvar.vecmaker)
	{
		if(savepistol)
		{
			cmd.exec("avsave VEC-Maker VEC");
			savepistol=0;
		}
	}
	/////
	if(cvar.waysetting)
	{
		if(wayrec)
		{
			cmd.exec("stop_auto_bq 1");
			cmd.exec("way_record");
			wayrec=0;
		}
		if(wayrecstp)
		{
			cmd.exec("stop_auto_bq 0");
			cmd.exec("way_stop");
			wayrecstp=0;
		}
		if(wayclr1)
		{
			cmd.exec("way_clear 1");
			wayclr1=0;
		}
		if(wayclr2)
		{
			 cmd.exec("way_clear 2");
			 wayclr2=0;
		}
		if(wayclr3)
		{
			 cmd.exec("way_clear 3");
			 wayclr3=0;
		}
		if(wayclr4)
		{
			 cmd.exec("way_clear 4");
			 wayclr4=0;
		}
		if(wayclr5)
		{
			 cmd.exec("way_clear 5");
		 	 wayclr5=0;
		}
		if(wayclr6)
		{
			 cmd.exec("way_clear 6");
		 	 wayclr6=0;
		}
		if(wayclr7)
		{
			 cmd.exec("way_clear 7");
		 	 wayclr7=0;
		}
		if(wayclr8)
		{
			 cmd.exec("way_clear 8");
		 	 wayclr8=0;
		}
		if(wayclr9)
		{
			 cmd.exec("way_clear 9");
		 	 wayclr9=0;
		}
		if(wayclr10)
		{
			 cmd.exec("way_clear 10");
		 	    wayclr10=0;
		}
		if(wayclrall)
		{
			 cmd.exec("way_clear");
			 wayclrall=0;
		}
	}
	//stand height
	//avadd 0 +0.01
/*	if (cvar.avchange_hups == 1 )
	{ 
		cmd.exec("avchange_h 0 0.01");
	}
	if (cvar.avchange_hups == 2 )
	{ 
		cmd.exec("avchange_h 0 0.02");
	}
	if (cvar.avchange_hups == 3 )
	{ 
		cmd.exec("avchange_h 0 0.03");
	}
	if (cvar.avchange_hups == 4 )
	{ 
		cmd.exec("avchange_h 0 0.04");
	}
	if (cvar.avchange_hups == 5 )
	{ 
		cmd.exec("avchange_h 0 0.05");
	}
	if (cvar.avchange_hups == 6 )
	{ 
		cmd.exec("avchange_h 0 0.06");
	}
	if (cvar.avchange_hups == 7 )
	{ 
		cmd.exec("avchange_h 0 0.07");
	}
	if (cvar.avchange_hups == 8 )
	{ 
		cmd.exec("avchange_h 0 0.08");
	}
	if (cvar.avchange_hups == 9 )
	{ 
		cmd.exec("avchange_h 0 0.09");
	}
	if (cvar.avchange_hups == 10 )
	{ 
		cmd.exec("avchange_h 0 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_hups1 == 1 )
	{ 
		cmd.exec("avchange_h 0 0.1");
	}
	if (cvar.avchange_hups1 == 2 )
	{ 
		cmd.exec("avchange_h 0 0.2");
	}
	if (cvar.avchange_hups1 == 3 )
	{ 
		cmd.exec("avchange_h 0 0.3");
	}
	if (cvar.avchange_hups1 == 4 )
	{ 
		cmd.exec("avchange_h 0 0.4");
	}
	if (cvar.avchange_hups1 == 5 )
	{ 
		cmd.exec("avchange_h 0 0.5");
	}
	if (cvar.avchange_hups1 == 6 )
	{ 
		cmd.exec("avchange_h 0 0.6");
	}
	if (cvar.avchange_hups1 == 7 )
	{ 
		cmd.exec("avchange_h 0 0.7");
	}
	if (cvar.avchange_hups1 == 8 )
	{ 
		cmd.exec("avchange_h 0 0.8");
	}
	if (cvar.avchange_hups1 == 9 )
	{ 
		cmd.exec("avchange_h 0 0.9");
	}
	if (cvar.avchange_hups1 == 10 )
	{ 
		cmd.exec("avchange_h 0 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_hups2 == 1 )
	{ 
		cmd.exec("avchange_h 0 1");
	}
	if (cvar.avchange_hups2 == 2 )
	{ 
		cmd.exec("avchange_h 0 2");
	}
	if (cvar.avchange_hups2 == 3 )
	{ 
		cmd.exec("avchange_h 0 3");
	}
	if (cvar.avchange_hups2 == 4 )
	{ 
		cmd.exec("avchange_h 0 4");
	}
	if (cvar.avchange_hups2 == 5 )
	{ 
		cmd.exec("avchange_h 0 5");
	}
	if (cvar.avchange_hups2 == 6 )
	{ 
		cmd.exec("avchange_h 0 6");
	}
	if (cvar.avchange_hups2 == 7 )
	{ 
		cmd.exec("avchange_h 0 7");
	}
	if (cvar.avchange_hups2 == 8 )
	{ 
		cmd.exec("avchange_h 0 8");
	}
	if (cvar.avchange_hups2 == 9 )
	{ 
		cmd.exec("avchange_h 0 9");
	}
	if (cvar.avchange_hups2 == 10 )
	{ 
		cmd.exec("avchange_h 0 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_hups == -1 )
	{ 
		cmd.exec("avchange_h 0 -0.01");
	}
	if (cvar.avchange_hups == -2 )
	{ 
		cmd.exec("avchange_h 0 -0.02");
	}
	if (cvar.avchange_hups == -3 )
	{ 
		cmd.exec("avchange_h 0 -0.03");
	}
	if (cvar.avchange_hups == -4 )
	{ 
		cmd.exec("avchange_h 0 -0.04");
	}
	if (cvar.avchange_hups == -5 )
	{ 
		cmd.exec("avchange_h 0 -0.05");
	}
	if (cvar.avchange_hups == -6 )
	{ 
		cmd.exec("avchange_h 0 -0.06");
	}
	if (cvar.avchange_hups == -7 )
	{ 
		cmd.exec("avchange_h 0 -0.07");
	}
	if (cvar.avchange_hups == -8 )
	{ 
		cmd.exec("avchange_h 0 -0.08");
	}
	if (cvar.avchange_hups == -9 )
	{ 
		cmd.exec("avchange_h 0 -0.09");
	}
	if (cvar.avchange_hups == -10 )
	{ 
		cmd.exec("avchange_h 0 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_hups1 == -1 )
	{ 
		cmd.exec("avchange_h 0 -0.1");
	}
	if (cvar.avchange_hups1 == -2 )
	{ 
		cmd.exec("avchange_h 0 -0.2");
	}
	if (cvar.avchange_hups1 == -3 )
	{ 
		cmd.exec("avchange_h 0 -0.3");
	}
	if (cvar.avchange_hups1 == -4 )
	{ 
		cmd.exec("avchange_h 0 -0.4");
	}
	if (cvar.avchange_hups1 == -5 )
	{ 
		cmd.exec("avchange_h 0 -0.5");
	}
	if (cvar.avchange_hups1 == -6 )
	{ 
		cmd.exec("avchange_h 0 -0.6");
	}
	if (cvar.avchange_hups1 == -7 )
	{ 
		cmd.exec("avchange_h 0 -0.7");
	}
	if (cvar.avchange_hups1 == -8 )
	{ 
		cmd.exec("avchange_h 0 -0.8");
	}
	if (cvar.avchange_hups1 == -9 )
	{ 
		cmd.exec("avchange_h 0 -0.9");
	}
	if (cvar.avchange_hups1 == -10 )
	{ 
		cmd.exec("avchange_h 0 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_hups2 == -1 )
	{ 
		cmd.exec("avchange_h 0 -1");
	}
	if (cvar.avchange_hups2 == -2 )
	{ 
		cmd.exec("avchange_h 0 -2");
	}
	if (cvar.avchange_hups2 == -3 )
	{ 
		cmd.exec("avchange_h 0 -3");
	}
	if (cvar.avchange_hups2 == -4 )
	{ 
		cmd.exec("avchange_h 0 -4");
	}
	if (cvar.avchange_hups2 == -5 )
	{ 
		cmd.exec("avchange_h 0 -5");
	}
	if (cvar.avchange_hups2 == -6 )
	{ 
		cmd.exec("avchange_h 0 -6");
	}
	if (cvar.avchange_hups2 == -7 )
	{ 
		cmd.exec("avchange_h 0 -7");
	}
	if (cvar.avchange_hups2 == -8 )
	{ 
		cmd.exec("avchange_h 0 -8");
	}
	if (cvar.avchange_hups2 == -9 )
	{ 
		cmd.exec("avchange_h 0 -9");
	}
	if (cvar.avchange_hups2 == -10 )
	{ 
		cmd.exec("avchange_h 0 -10");
	}
	//duck height
	//avadd 1 +0.01
	if (cvar.avchange_hupd == 1 )
	{ 
		cmd.exec("avchange_h 1 0.01");
	}
	if (cvar.avchange_hupd == 2 )
	{ 
		cmd.exec("avchange_h 1 0.02");
	}
	if (cvar.avchange_hupd == 3 )
	{ 
		cmd.exec("avchange_h 1 0.03");
	}
	if (cvar.avchange_hupd == 4 )
	{ 
		cmd.exec("avchange_h 1 0.04");
	}
	if (cvar.avchange_hupd == 5 )
	{ 
		cmd.exec("avchange_h 1 0.05");
	}
	if (cvar.avchange_hupd == 6 )
	{ 
		cmd.exec("avchange_h 1 0.06");
	}
	if (cvar.avchange_hupd == 7 )
	{ 
		cmd.exec("avchange_h 1 0.07");
	}
	if (cvar.avchange_hupd == 8 )
	{ 
		cmd.exec("avchange_h 1 0.08");
	}
	if (cvar.avchange_hupd == 9 )
	{ 
		cmd.exec("avchange_h 1 0.09");
	}
	if (cvar.avchange_hupd == 10 )
	{ 
		cmd.exec("avchange_h 1 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_hupd1 == 1 )
	{ 
		cmd.exec("avchange_h 1 0.1");
	}
	if (cvar.avchange_hupd1 == 2 )
	{ 
		cmd.exec("avchange_h 1 0.2");
	}
	if (cvar.avchange_hupd1 == 3 )
	{ 
		cmd.exec("avchange_h 1 0.3");
	}
	if (cvar.avchange_hupd1 == 4 )
	{ 
		cmd.exec("avchange_h 1 0.4");
	}
	if (cvar.avchange_hupd1 == 5 )
	{ 
		cmd.exec("avchange_h 1 0.5");
	}
	if (cvar.avchange_hupd1 == 6 )
	{ 
		cmd.exec("avchange_h 1 0.6");
	}
	if (cvar.avchange_hupd1 == 7 )
	{ 
		cmd.exec("avchange_h 1 0.7");
	}
	if (cvar.avchange_hupd1 == 8 )
	{ 
		cmd.exec("avchange_h 1 0.8");
	}
	if (cvar.avchange_hupd1 == 9 )
	{ 
		cmd.exec("avchange_h 1 0.9");
	}
	if (cvar.avchange_hupd1 == 10 )
	{ 
		cmd.exec("avchange_h 1 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_hupd2 == 1 )
	{ 
		cmd.exec("avchange_h 1 1");
	}
	if (cvar.avchange_hupd2 == 2 )
	{ 
		cmd.exec("avchange_h 1 2");
	}
	if (cvar.avchange_hupd2 == 3 )
	{ 
		cmd.exec("avchange_h 1 3");
	}
	if (cvar.avchange_hupd2 == 4 )
	{ 
		cmd.exec("avchange_h 1 4");
	}
	if (cvar.avchange_hupd2 == 5 )
	{ 
		cmd.exec("avchange_h 1 5");
	}
	if (cvar.avchange_hupd2 == 6 )
	{ 
		cmd.exec("avchange_h 1 6");
	}
	if (cvar.avchange_hupd2 == 7 )
	{ 
		cmd.exec("avchange_h 1 7");
	}
	if (cvar.avchange_hupd2 == 8 )
	{ 
		cmd.exec("avchange_h 1 8");
	}
	if (cvar.avchange_hupd2 == 9 )
	{ 
		cmd.exec("avchange_h 1 9");
	}
	if (cvar.avchange_hupd2 == 10 )
	{ 
		cmd.exec("avchange_h 1 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_hupd == -1 )
	{ 
		cmd.exec("avchange_h 1 -0.01");
	}
	if (cvar.avchange_hupd == -2 )
	{ 
		cmd.exec("avchange_h 1 -0.02");
	}
	if (cvar.avchange_hupd == -3 )
	{ 
		cmd.exec("avchange_h 1 -0.03");
	}
	if (cvar.avchange_hupd == -4 )
	{ 
		cmd.exec("avchange_h 1 -0.04");
	}
	if (cvar.avchange_hupd == -5 )
	{ 
		cmd.exec("avchange_h 1 -0.05");
	}
	if (cvar.avchange_hupd == -6 )
	{ 
		cmd.exec("avchange_h 1 -0.06");
	}
	if (cvar.avchange_hupd == -7 )
	{ 
		cmd.exec("avchange_h 1 -0.07");
	}
	if (cvar.avchange_hupd == -8 )
	{ 
		cmd.exec("avchange_h 1 -0.08");
	}
	if (cvar.avchange_hupd == -9 )
	{ 
		cmd.exec("avchange_h 1 -0.09");
	}
	if (cvar.avchange_hupd == -10 )
	{ 
		cmd.exec("avchange_h 1 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_hupd1 == -1 )
	{ 
		cmd.exec("avchange_h 1 -0.1");
	}
	if (cvar.avchange_hupd1 == -2 )
	{ 
		cmd.exec("avchange_h 1 -0.2");
	}
	if (cvar.avchange_hupd1 == -3 )
	{ 
		cmd.exec("avchange_h 1 -0.3");
	}
	if (cvar.avchange_hupd1 == -4 )
	{ 
		cmd.exec("avchange_h 1 -0.4");
	}
	if (cvar.avchange_hupd1 == -5 )
	{ 
		cmd.exec("avchange_h 1 -0.5");
	}
	if (cvar.avchange_hupd1 == -6 )
	{ 
		cmd.exec("avchange_h 1 -0.6");
	}
	if (cvar.avchange_hupd1 == -7 )
	{ 
		cmd.exec("avchange_h 1 -0.7");
	}
	if (cvar.avchange_hupd1 == -8 )
	{ 
		cmd.exec("avchange_h 1 -0.8");
	}
	if (cvar.avchange_hupd1 == -9 )
	{ 
		cmd.exec("avchange_h 1 -0.9");
	}
	if (cvar.avchange_hupd1 == -10 )
	{ 
		cmd.exec("avchange_h 1 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_hupd2 == -1 )
	{ 
		cmd.exec("avchange_h 1 -1");
	}
	if (cvar.avchange_hupd2 == -2 )
	{ 
		cmd.exec("avchange_h 1 -2");
	}
	if (cvar.avchange_hupd2 == -3 )
	{ 
		cmd.exec("avchange_h 1 -3");
	}
	if (cvar.avchange_hupd2 == -4 )
	{ 
		cmd.exec("avchange_h 1 -4");
	}
	if (cvar.avchange_hupd2 == -5 )
	{ 
		cmd.exec("avchange_h 1 -5");
	}
	if (cvar.avchange_hupd2 == -6 )
	{ 
		cmd.exec("avchange_h 1 -6");
	}
	if (cvar.avchange_hupd2 == -7 )
	{ 
		cmd.exec("avchange_h 1 -7");
	}
	if (cvar.avchange_hupd2 == -8 )
	{ 
		cmd.exec("avchange_h 1 -8");
	}
	if (cvar.avchange_hupd2 == -9 )
	{ 
		cmd.exec("avchange_h 1 -9");
	}
	if (cvar.avchange_hupd2 == -10 )
	{ 
		cmd.exec("avchange_h 1 -10");
	}
	//jump height
	//avadd 2 +0.01
*//*	if (cvar.avchange_hupj == 1 )
	{ 
		cmd.exec("avchange_h 2 0.01");
	}
	if (cvar.avchange_hupj == 2 )
	{ 
		cmd.exec("avchange_h 2 0.02");
	}
	if (cvar.avchange_hupj == 3 )
	{ 
		cmd.exec("avchange_h 2 0.03");
	}
	if (cvar.avchange_hupj == 4 )
	{ 
		cmd.exec("avchange_h 2 0.04");
	}
	if (cvar.avchange_hupj == 5 )
	{ 
		cmd.exec("avchange_h 2 0.05");
	}
	if (cvar.avchange_hupj == 6 )
	{ 
		cmd.exec("avchange_h 2 0.06");
	}
	if (cvar.avchange_hupj == 7 )
	{ 
		cmd.exec("avchange_h 2 0.07");
	}
	if (cvar.avchange_hupj == 8 )
	{ 
		cmd.exec("avchange_h 2 0.08");
	}
	if (cvar.avchange_hupj == 9 )
	{ 
		cmd.exec("avchange_h 2 0.09");
	}
	if (cvar.avchange_hupj == 10 )
	{ 
		cmd.exec("avchange_h 2 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_hupj1 == 1 )
	{ 
		cmd.exec("avchange_h 2 0.1");
	}
	if (cvar.avchange_hupj1 == 2 )
	{ 
		cmd.exec("avchange_h 2 0.2");
	}
	if (cvar.avchange_hupj1 == 3 )
	{ 
		cmd.exec("avchange_h 2 0.3");
	}
	if (cvar.avchange_hupj1 == 4 )
	{ 
		cmd.exec("avchange_h 2 0.4");
	}
	if (cvar.avchange_hupj1 == 5 )
	{ 
		cmd.exec("avchange_h 2 0.5");
	}
	if (cvar.avchange_hupj1 == 6 )
	{ 
		cmd.exec("avchange_h 2 0.6");
	}
	if (cvar.avchange_hupj1 == 7 )
	{ 
		cmd.exec("avchange_h 2 0.7");
	}
	if (cvar.avchange_hupj1 == 8 )
	{ 
		cmd.exec("avchange_h 2 0.8");
	}
	if (cvar.avchange_hupj1 == 9 )
	{ 
		cmd.exec("avchange_h 2 0.9");
	}
	if (cvar.avchange_hupj1 == 10 )
	{ 
		cmd.exec("avchange_h 2 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_hupj2 == 1 )
	{ 
		cmd.exec("avchange_h 2 1");
	}
	if (cvar.avchange_hupj2 == 2 )
	{ 
		cmd.exec("avchange_h 2 2");
	}
	if (cvar.avchange_hupj2 == 3 )
	{ 
		cmd.exec("avchange_h 2 3");
	}
	if (cvar.avchange_hupj2 == 4 )
	{ 
		cmd.exec("avchange_h 2 4");
	}
	if (cvar.avchange_hupj2 == 5 )
	{ 
		cmd.exec("avchange_h 2 5");
	}
	if (cvar.avchange_hupj2 == 6 )
	{ 
		cmd.exec("avchange_h 2 6");
	}
	if (cvar.avchange_hupj2 == 7 )
	{ 
		cmd.exec("avchange_h 2 7");
	}
	if (cvar.avchange_hupj2 == 8 )
	{ 
		cmd.exec("avchange_h 2 8");
	}
	if (cvar.avchange_hupj2 == 9 )
	{ 
		cmd.exec("avchange_h 2 9");
	}
	if (cvar.avchange_hupj2 == 10 )
	{ 
		cmd.exec("avchange_h 2 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_hupj == -1 )
	{ 
		cmd.exec("avchange_h 2 -0.01");
	}
	if (cvar.avchange_hupj == -2 )
	{ 
		cmd.exec("avchange_h 2 -0.02");
	}
	if (cvar.avchange_hupj == -3 )
	{ 
		cmd.exec("avchange_h 2 -0.03");
	}
	if (cvar.avchange_hupj == -4 )
	{ 
		cmd.exec("avchange_h 2 -0.04");
	}
	if (cvar.avchange_hupj == -5 )
	{ 
		cmd.exec("avchange_h 2 -0.05");
	}
	if (cvar.avchange_hupj == -6 )
	{ 
		cmd.exec("avchange_h 2 -0.06");
	}
	if (cvar.avchange_hupj == -7 )
	{ 
		cmd.exec("avchange_h 2 -0.07");
	}
	if (cvar.avchange_hupj == -8 )
	{ 
		cmd.exec("avchange_h 2 -0.08");
	}
	if (cvar.avchange_hupj == -9 )
	{ 
		cmd.exec("avchange_h 2 -0.09");
	}
	if (cvar.avchange_hupj == -10 )
	{ 
		cmd.exec("avchange_h 2 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_hupj1 == -1 )
	{ 
		cmd.exec("avchange_h 2 -0.1");
	}
	if (cvar.avchange_hupj1 == -2 )
	{ 
		cmd.exec("avchange_h 2 -0.2");
	}
	if (cvar.avchange_hupj1 == -3 )
	{ 
		cmd.exec("avchange_h 2 -0.3");
	}
	if (cvar.avchange_hupj1 == -4 )
	{ 
		cmd.exec("avchange_h 2 -0.4");
	}
	if (cvar.avchange_hupj1 == -5 )
	{ 
		cmd.exec("avchange_h 2 -0.5");
	}
	if (cvar.avchange_hupj1 == -6 )
	{ 
		cmd.exec("avchange_h 2 -0.6");
	}
	if (cvar.avchange_hupj1 == -7 )
	{ 
		cmd.exec("avchange_h 2 -0.7");
	}
	if (cvar.avchange_hupj1 == -8 )
	{ 
		cmd.exec("avchange_h 2 -0.8");
	}
	if (cvar.avchange_hupj1 == -9 )
	{ 
		cmd.exec("avchange_h 2 -0.9");
	}
	if (cvar.avchange_hupj1 == -10 )
	{ 
		cmd.exec("avchange_h 2 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_hupj2 == -1 )
	{ 
		cmd.exec("avchange_h 2 -1");
	}
	if (cvar.avchange_hupj2 == -2 )
	{ 
		cmd.exec("avchange_h 2 -2");
	}
	if (cvar.avchange_hupj2 == -3 )
	{ 
		cmd.exec("avchange_h 2 -3");
	}
	if (cvar.avchange_hupj2 == -4 )
	{ 
		cmd.exec("avchange_h 2 -4");
	}
	if (cvar.avchange_hupj2 == -5 )
	{ 
		cmd.exec("avchange_h 2 -5");
	}
	if (cvar.avchange_hupj2 == -6 )
	{ 
		cmd.exec("avchange_h 2 -6");
	}
	if (cvar.avchange_hupj2 == -7 )
	{ 
		cmd.exec("avchange_h 2 -7");
	}
	if (cvar.avchange_hupj2 == -8 )
	{ 
		cmd.exec("avchange_h 2 -8");
	}
	if (cvar.avchange_hupj2 == -9 )
	{ 
		cmd.exec("avchange_h 2 -9");
	}
	if (cvar.avchange_hupj2 == -10 )
	{ 
		cmd.exec("avchange_h 2 -10");
	}
	///////////////
	//stand forward
	//avadd 0 +0.01
	if (cvar.avchange_fups == 1 )
	{ 
		cmd.exec("avchange_f 0 0.01");
	}
	if (cvar.avchange_fups == 2 )
	{ 
		cmd.exec("avchange_f 0 0.02");
	}
	if (cvar.avchange_fups == 3 )
	{ 
		cmd.exec("avchange_f 0 0.03");
	}
	if (cvar.avchange_fups == 4 )
	{ 
		cmd.exec("avchange_f 0 0.04");
	}
	if (cvar.avchange_fups == 5 )
	{ 
		cmd.exec("avchange_f 0 0.05");
	}
	if (cvar.avchange_fups == 6 )
	{ 
		cmd.exec("avchange_f 0 0.06");
	}
	if (cvar.avchange_fups == 7 )
	{ 
		cmd.exec("avchange_f 0 0.07");
	}
	if (cvar.avchange_fups == 8 )
	{ 
		cmd.exec("avchange_f 0 0.08");
	}
	if (cvar.avchange_fups == 9 )
	{ 
		cmd.exec("avchange_f 0 0.09");
	}
	if (cvar.avchange_fups == 10 )
	{ 
		cmd.exec("avchange_f 0 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_fups1 == 1 )
	{ 
		cmd.exec("avchange_f 0 0.1");
	}
	if (cvar.avchange_fups1 == 2 )
	{ 
		cmd.exec("avchange_f 0 0.2");
	}
	if (cvar.avchange_fups1 == 3 )
	{ 
		cmd.exec("avchange_f 0 0.3");
	}
	if (cvar.avchange_fups1 == 4 )
	{ 
		cmd.exec("avchange_f 0 0.4");
	}
	if (cvar.avchange_fups1 == 5 )
	{ 
		cmd.exec("avchange_f 0 0.5");
	}
	if (cvar.avchange_fups1 == 6 )
	{ 
		cmd.exec("avchange_f 0 0.6");
	}
	if (cvar.avchange_fups1 == 7 )
	{ 
		cmd.exec("avchange_f 0 0.7");
	}
	if (cvar.avchange_fups1 == 8 )
	{ 
		cmd.exec("avchange_f 0 0.8");
	}
	if (cvar.avchange_fups1 == 9 )
	{ 
		cmd.exec("avchange_f 0 0.9");
	}
	if (cvar.avchange_fups1 == 10 )
	{ 
		cmd.exec("avchange_f 0 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_fups2 == 1 )
	{ 
		cmd.exec("avchange_f 0 1");
	}
	if (cvar.avchange_fups2 == 2 )
	{ 
		cmd.exec("avchange_f 0 2");
	}
	if (cvar.avchange_fups2 == 3 )
	{ 
		cmd.exec("avchange_f 0 3");
	}
	if (cvar.avchange_fups2 == 4 )
	{ 
		cmd.exec("avchange_f 0 4");
	}
	if (cvar.avchange_fups2 == 5 )
	{ 
		cmd.exec("avchange_f 0 5");
	}
	if (cvar.avchange_fups2 == 6 )
	{ 
		cmd.exec("avchange_f 0 6");
	}
	if (cvar.avchange_fups2 == 7 )
	{ 
		cmd.exec("avchange_f 0 7");
	}
	if (cvar.avchange_fups2 == 8 )
	{ 
		cmd.exec("avchange_f 0 8");
	}
	if (cvar.avchange_fups2 == 9 )
	{ 
		cmd.exec("avchange_f 0 9");
	}
	if (cvar.avchange_fups2 == 10 )
	{ 
		cmd.exec("avchange_f 0 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_fups == -1 )
	{ 
		cmd.exec("avchange_f 0 -0.01");
	}
	if (cvar.avchange_fups == -2 )
	{ 
		cmd.exec("avchange_f 0 -0.02");
	}
	if (cvar.avchange_fups == -3 )
	{ 
		cmd.exec("avchange_f 0 -0.03");
	}
	if (cvar.avchange_fups == -4 )
	{ 
		cmd.exec("avchange_f 0 -0.04");
	}
	if (cvar.avchange_fups == -5 )
	{ 
		cmd.exec("avchange_f 0 -0.05");
	}
	if (cvar.avchange_fups == -6 )
	{ 
		cmd.exec("avchange_f 0 -0.06");
	}
	if (cvar.avchange_fups == -7 )
	{ 
		cmd.exec("avchange_f 0 -0.07");
	}
	if (cvar.avchange_fups == -8 )
	{ 
		cmd.exec("avchange_f 0 -0.08");
	}
	if (cvar.avchange_fups == -9 )
	{ 
		cmd.exec("avchange_f 0 -0.09");
	}
	if (cvar.avchange_fups == -10 )
	{ 
		cmd.exec("avchange_f 0 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_fups1 == -1 )
	{ 
		cmd.exec("avchange_f 0 -0.1");
	}
	if (cvar.avchange_fups1 == -2 )
	{ 
		cmd.exec("avchange_f 0 -0.2");
	}
	if (cvar.avchange_fups1 == -3 )
	{ 
		cmd.exec("avchange_f 0 -0.3");
	}
	if (cvar.avchange_fups1 == -4 )
	{ 
		cmd.exec("avchange_f 0 -0.4");
	}
	if (cvar.avchange_fups1 == -5 )
	{ 
		cmd.exec("avchange_f 0 -0.5");
	}
	if (cvar.avchange_fups1 == -6 )
	{ 
		cmd.exec("avchange_f 0 -0.6");
	}
	if (cvar.avchange_fups1 == -7 )
	{ 
		cmd.exec("avchange_f 0 -0.7");
	}
	if (cvar.avchange_fups1 == -8 )
	{ 
		cmd.exec("avchange_f 0 -0.8");
	}
	if (cvar.avchange_fups1 == -9 )
	{ 
		cmd.exec("avchange_f 0 -0.9");
	}
	if (cvar.avchange_fups1 == -10 )
	{ 
		cmd.exec("avchange_f 0 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_fups2 == -1 )
	{ 
		cmd.exec("avchange_f 0 -1");
	}
	if (cvar.avchange_fups2 == -2 )
	{ 
		cmd.exec("avchange_f 0 -2");
	}
	if (cvar.avchange_fups2 == -3 )
	{ 
		cmd.exec("avchange_f 0 -3");
	}
	if (cvar.avchange_fups2 == -4 )
	{ 
		cmd.exec("avchange_f 0 -4");
	}
	if (cvar.avchange_fups2 == -5 )
	{ 
		cmd.exec("avchange_f 0 -5");
	}
	if (cvar.avchange_fups2 == -6 )
	{ 
		cmd.exec("avchange_f 0 -6");
	}
	if (cvar.avchange_fups2 == -7 )
	{ 
		cmd.exec("avchange_f 0 -7");
	}
	if (cvar.avchange_fups2 == -8 )
	{ 
		cmd.exec("avchange_f 0 -8");
	}
	if (cvar.avchange_fups2 == -9 )
	{ 
		cmd.exec("avchange_f 0 -9");
	}
	if (cvar.avchange_fups2 == -10 )
	{ 
		cmd.exec("avchange_f 0 -10");
	}
	//duck firward
*//*	//avadd 1 +0.01
	if (cvar.avchange_fupd == 1 )
	{ 
		cmd.exec("avchange_f 1 0.01");
	}
	if (cvar.avchange_fupd == 2 )
	{ 
		cmd.exec("avchange_f 1 0.02");
	}
	if (cvar.avchange_fupd == 3 )
	{ 
		cmd.exec("avchange_f 1 0.03");
	}
	if (cvar.avchange_fupd == 4 )
	{ 
		cmd.exec("avchange_f 1 0.04");
	}
	if (cvar.avchange_fupd == 5 )
	{ 
		cmd.exec("avchange_f 1 0.05");
	}
	if (cvar.avchange_fupd == 6 )
	{ 
		cmd.exec("avchange_f 1 0.06");
	}
	if (cvar.avchange_fupd == 7 )
	{ 
		cmd.exec("avchange_f 1 0.07");
	}
	if (cvar.avchange_fupd == 8 )
	{ 
		cmd.exec("avchange_f 1 0.08");
	}
	if (cvar.avchange_fupd == 9 )
	{ 
		cmd.exec("avchange_f 1 0.09");
	}
	if (cvar.avchange_fupd == 10 )
	{ 
		cmd.exec("avchange_f 1 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_fupd1 == 1 )
	{ 
		cmd.exec("avchange_f 1 0.1");
	}
	if (cvar.avchange_fupd1 == 2 )
	{ 
		cmd.exec("avchange_f 1 0.2");
	}
	if (cvar.avchange_fupd1 == 3 )
	{ 
		cmd.exec("avchange_f 1 0.3");
	}
	if (cvar.avchange_fupd1 == 4 )
	{ 
		cmd.exec("avchange_f 1 0.4");
	}
	if (cvar.avchange_fupd1 == 5 )
	{ 
		cmd.exec("avchange_f 1 0.5");
	}
	if (cvar.avchange_fupd1 == 6 )
	{ 
		cmd.exec("avchange_f 1 0.6");
	}
	if (cvar.avchange_fupd1 == 7 )
	{ 
		cmd.exec("avchange_f 1 0.7");
	}
	if (cvar.avchange_fupd1 == 8 )
	{ 
		cmd.exec("avchange_f 1 0.8");
	}
	if (cvar.avchange_fupd1 == 9 )
	{ 
		cmd.exec("avchange_f 1 0.9");
	}
	if (cvar.avchange_fupd1 == 10 )
	{ 
		cmd.exec("avchange_f 1 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_fupd2 == 1 )
	{ 
		cmd.exec("avchange_f 1 1");
	}
	if (cvar.avchange_fupd2 == 2 )
	{ 
		cmd.exec("avchange_f 1 2");
	}
	if (cvar.avchange_fupd2 == 3 )
	{ 
		cmd.exec("avchange_f 1 3");
	}
	if (cvar.avchange_fupd2 == 4 )
	{ 
		cmd.exec("avchange_f 1 4");
	}
	if (cvar.avchange_fupd2 == 5 )
	{ 
		cmd.exec("avchange_f 1 5");
	}
	if (cvar.avchange_fupd2 == 6 )
	{ 
		cmd.exec("avchange_f 1 6");
	}
	if (cvar.avchange_fupd2 == 7 )
	{ 
		cmd.exec("avchange_f 1 7");
	}
	if (cvar.avchange_fupd2 == 8 )
	{ 
		cmd.exec("avchange_f 1 8");
	}
	if (cvar.avchange_fupd2 == 9 )
	{ 
		cmd.exec("avchange_f 1 9");
	}
	if (cvar.avchange_fupd2 == 10 )
	{ 
		cmd.exec("avchange_f 1 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_fupd == -1 )
	{ 
		cmd.exec("avchange_f 1 -0.01");
	}
	if (cvar.avchange_fupd == -2 )
	{ 
		cmd.exec("avchange_f 1 -0.02");
	}
	if (cvar.avchange_fupd == -3 )
	{ 
		cmd.exec("avchange_f 1 -0.03");
	}
	if (cvar.avchange_fupd == -4 )
	{ 
		cmd.exec("avchange_f 1 -0.04");
	}
	if (cvar.avchange_fupd == -5 )
	{ 
		cmd.exec("avchange_f 1 -0.05");
	}
	if (cvar.avchange_fupd == -6 )
	{ 
		cmd.exec("avchange_f 1 -0.06");
	}
	if (cvar.avchange_fupd == -7 )
	{ 
		cmd.exec("avchange_f 1 -0.07");
	}
	if (cvar.avchange_fupd == -8 )
	{ 
		cmd.exec("avchange_f 1 -0.08");
	}
	if (cvar.avchange_fupd == -9 )
	{ 
		cmd.exec("avchange_f 1 -0.09");
	}
	if (cvar.avchange_fupd == -10 )
	{ 
		cmd.exec("avchange_f 1 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_fupd1 == -1 )
	{ 
		cmd.exec("avchange_f 1 -0.1");
	}
	if (cvar.avchange_fupd1 == -2 )
	{ 
		cmd.exec("avchange_f 1 -0.2");
	}
	if (cvar.avchange_fupd1 == -3 )
	{ 
		cmd.exec("avchange_f 1 -0.3");
	}
	if (cvar.avchange_fupd1 == -4 )
	{ 
		cmd.exec("avchange_f 1 -0.4");
	}
	if (cvar.avchange_fupd1 == -5 )
	{ 
		cmd.exec("avchange_f 1 -0.5");
	}
	if (cvar.avchange_fupd1 == -6 )
	{ 
		cmd.exec("avchange_f 1 -0.6");
	}
	if (cvar.avchange_fupd1 == -7 )
	{ 
		cmd.exec("avchange_f 1 -0.7");
	}
	if (cvar.avchange_fupd1 == -8 )
	{ 
		cmd.exec("avchange_f 1 -0.8");
	}
	if (cvar.avchange_fupd1 == -9 )
	{ 
		cmd.exec("avchange_f 1 -0.9");
	}
	if (cvar.avchange_fupd1 == -10 )
	{ 
		cmd.exec("avchange_f 1 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_fupd2 == -1 )
	{ 
		cmd.exec("avchange_f 1 -1");
	}
	if (cvar.avchange_fupd2 == -2 )
	{ 
		cmd.exec("avchange_f 1 -2");
	}
	if (cvar.avchange_fupd2 == -3 )
	{ 
		cmd.exec("avchange_f 1 -3");
	}
	if (cvar.avchange_fupd2 == -4 )
	{ 
		cmd.exec("avchange_f 1 -4");
	}
	if (cvar.avchange_fupd2 == -5 )
	{ 
		cmd.exec("avchange_f 1 -5");
	}
	if (cvar.avchange_fupd2 == -6 )
	{ 
		cmd.exec("avchange_f 1 -6");
	}
	if (cvar.avchange_fupd2 == -7 )
	{ 
		cmd.exec("avchange_f 1 -7");
	}
	if (cvar.avchange_fupd2 == -8 )
	{ 
		cmd.exec("avchange_f 1 -8");
	}
	if (cvar.avchange_fupd2 == -9 )
	{ 
		cmd.exec("avchange_f 1 -9");
	}
	if (cvar.avchange_fupd2 == -10 )
	{ 
		cmd.exec("avchange_f 1 -10");
	}
	//jump forward
	//avadd 2 +0.01
	if (cvar.avchange_fupj == 1 )
	{ 
		cmd.exec("avchange_f 2 0.01");
	}
	if (cvar.avchange_fupj == 2 )
	{ 
		cmd.exec("avchange_f 2 0.02");
	}
	if (cvar.avchange_fupj == 3 )
	{ 
		cmd.exec("avchange_f 2 0.03");
	}
	if (cvar.avchange_fupj == 4 )
	{ 
		cmd.exec("avchange_f 2 0.04");
	}
	if (cvar.avchange_fupj == 5 )
	{ 
		cmd.exec("avchange_f 2 0.05");
	}
	if (cvar.avchange_fupj == 6 )
	{ 
		cmd.exec("avchange_f 2 0.06");
	}
	if (cvar.avchange_fupj == 7 )
	{ 
		cmd.exec("avchange_f 2 0.07");
	}
	if (cvar.avchange_fupj == 8 )
	{ 
		cmd.exec("avchange_f 2 0.08");
	}
	if (cvar.avchange_fupj == 9 )
	{ 
		cmd.exec("avchange_f 2 0.09");
	}
	if (cvar.avchange_fupj == 10 )
	{ 
		cmd.exec("avchange_f 2 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_fupj1 == 1 )
	{ 
		cmd.exec("avchange_f 2 0.1");
	}
	if (cvar.avchange_fupj1 == 2 )
	{ 
		cmd.exec("avchange_f 2 0.2");
	}
	if (cvar.avchange_fupj1 == 3 )
	{ 
		cmd.exec("avchange_f 2 0.3");
	}
	if (cvar.avchange_fupj1 == 4 )
	{ 
		cmd.exec("avchange_f 2 0.4");
	}
	if (cvar.avchange_fupj1 == 5 )
	{ 
		cmd.exec("avchange_f 2 0.5");
	}
	if (cvar.avchange_fupj1 == 6 )
	{ 
		cmd.exec("avchange_f 2 0.6");
	}
	if (cvar.avchange_fupj1 == 7 )
	{ 
		cmd.exec("avchange_f 2 0.7");
	}
	if (cvar.avchange_fupj1 == 8 )
	{ 
		cmd.exec("avchange_f 2 0.8");
	}
	if (cvar.avchange_fupj1 == 9 )
	{ 
		cmd.exec("avchange_f 2 0.9");
	}
	if (cvar.avchange_fupj1 == 10 )
	{ 
		cmd.exec("avchange_f 2 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_fupj2 == 1 )
	{ 
		cmd.exec("avchange_f 2 1");
	}
	if (cvar.avchange_fupj2 == 2 )
	{ 
		cmd.exec("avchange_f 2 2");
	}
	if (cvar.avchange_fupj2 == 3 )
	{ 
		cmd.exec("avchange_f 2 3");
	}
	if (cvar.avchange_fupj2 == 4 )
	{ 
		cmd.exec("avchange_f 2 4");
	}
	if (cvar.avchange_fupj2 == 5 )
	{ 
		cmd.exec("avchange_f 2 5");
	}
	if (cvar.avchange_fupj2 == 6 )
	{ 
		cmd.exec("avchange_f 2 6");
	}
	if (cvar.avchange_fupj2 == 7 )
	{ 
		cmd.exec("avchange_f 2 7");
	}
	if (cvar.avchange_fupj2 == 8 )
	{ 
		cmd.exec("avchange_f 2 8");
	}
	if (cvar.avchange_fupj2 == 9 )
	{ 
		cmd.exec("avchange_f 2 9");
	}
	if (cvar.avchange_fupj2 == 10 )
	{ 
		cmd.exec("avchange_f 2 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_fupj == -1 )
	{ 
		cmd.exec("avchange_f 2 -0.01");
	}
	if (cvar.avchange_fupj == -2 )
	{ 
		cmd.exec("avchange_f 2 -0.02");
	}
	if (cvar.avchange_fupj == -3 )
	{ 
		cmd.exec("avchange_f 2 -0.03");
	}
	if (cvar.avchange_fupj == -4 )
	{ 
		cmd.exec("avchange_f 2 -0.04");
	}
	if (cvar.avchange_fupj == -5 )
	{ 
		cmd.exec("avchange_f 2 -0.05");
	}
	if (cvar.avchange_fupj == -6 )
	{ 
		cmd.exec("avchange_f 2 -0.06");
	}
	if (cvar.avchange_fupj == -7 )
	{ 
		cmd.exec("avchange_f 2 -0.07");
	}
	if (cvar.avchange_fupj == -8 )
	{ 
		cmd.exec("avchange_f 2 -0.08");
	}
	if (cvar.avchange_fupj == -9 )
	{ 
		cmd.exec("avchange_f 2 -0.09");
	}
	if (cvar.avchange_fupj == -10 )
	{ 
		cmd.exec("avchange_f 2 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_fupj1 == -1 )
	{ 
		cmd.exec("avchange_f 2 -0.1");
	}
	if (cvar.avchange_fupj1 == -2 )
	{ 
		cmd.exec("avchange_f 2 -0.2");
	}
	if (cvar.avchange_fupj1 == -3 )
	{ 
		cmd.exec("avchange_f 2 -0.3");
	}
	if (cvar.avchange_fupj1 == -4 )
	{ 
		cmd.exec("avchange_f 2 -0.4");
	}
	if (cvar.avchange_fupj1 == -5 )
	{ 
		cmd.exec("avchange_f 2 -0.5");
	}
	if (cvar.avchange_fupj1 == -6 )
	{ 
		cmd.exec("avchange_f 2 -0.6");
	}
	if (cvar.avchange_fupj1 == -7 )
	{ 
		cmd.exec("avchange_f 2 -0.7");
	}
	if (cvar.avchange_fupj1 == -8 )
	{ 
		cmd.exec("avchange_f 2 -0.8");
	}
	if (cvar.avchange_fupj1 == -9 )
	{ 
		cmd.exec("avchange_f 2 -0.9");
	}
	if (cvar.avchange_fupj1 == -10 )
	{ 
		cmd.exec("avchange_f 2 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_fupj2 == -1 )
	{ 
		cmd.exec("avchange_f 2 -1");
	}
	if (cvar.avchange_fupj2 == -2 )
	{ 
		cmd.exec("avchange_f 2 -2");
	}
	if (cvar.avchange_fupj2 == -3 )
	{ 
		cmd.exec("avchange_f 2 -3");
	}
	if (cvar.avchange_fupj2 == -4 )
	{ 
		cmd.exec("avchange_f 2 -4");
	}
	if (cvar.avchange_fupj2 == -5 )
	{ 
		cmd.exec("avchange_f 2 -5");
	}
	if (cvar.avchange_fupj2 == -6 )
	{ 
		cmd.exec("avchange_f 2 -6");
	}
	if (cvar.avchange_fupj2 == -7 )
	{ 
		cmd.exec("avchange_f 2 -7");
	}
	if (cvar.avchange_fupj2 == -8 )
	{ 
		cmd.exec("avchange_f 2 -8");
	}
	if (cvar.avchange_fupj2 == -9 )
	{ 
		cmd.exec("avchange_f 2 -9");
	}
	if (cvar.avchange_fupj2 == -10 )
	{ 
		cmd.exec("avchange_f 2 -10");
	}
	//stand right
	//avadd 0 +0.01
*//*	if (cvar.avchange_rups == 1 )
	{ 
		cmd.exec("avchange_r 0 0.01");
	}
	if (cvar.avchange_rups == 2 )
	{ 
		cmd.exec("avchange_r 0 0.02");
	}
	if (cvar.avchange_rups == 3 )
	{ 
		cmd.exec("avchange_r 0 0.03");
	}
	if (cvar.avchange_rups == 4 )
	{ 
		cmd.exec("avchange_r 0 0.04");
	}
	if (cvar.avchange_rups == 5 )
	{ 
		cmd.exec("avchange_r 0 0.05");
	}
	if (cvar.avchange_rups == 6 )
	{ 
		cmd.exec("avchange_r 0 0.06");
	}
	if (cvar.avchange_rups == 7 )
	{ 
		cmd.exec("avchange_r 0 0.07");
	}
	if (cvar.avchange_rups == 8 )
	{ 
		cmd.exec("avchange_r 0 0.08");
	}
	if (cvar.avchange_rups == 9 )
	{ 
		cmd.exec("avchange_r 0 0.09");
	}
	if (cvar.avchange_rups == 10 )
	{ 
		cmd.exec("avchange_r 0 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_rups1 == 1 )
	{ 
		cmd.exec("avchange_r 0 0.1");
	}
	if (cvar.avchange_rups1 == 2 )
	{ 
		cmd.exec("avchange_r 0 0.2");
	}
	if (cvar.avchange_rups1 == 3 )
	{ 
		cmd.exec("avchange_r 0 0.3");
	}
	if (cvar.avchange_rups1 == 4 )
	{ 
		cmd.exec("avchange_r 0 0.4");
	}
	if (cvar.avchange_rups1 == 5 )
	{ 
		cmd.exec("avchange_r 0 0.5");
	}
	if (cvar.avchange_rups1 == 6 )
	{ 
		cmd.exec("avchange_r 0 0.6");
	}
	if (cvar.avchange_rups1 == 7 )
	{ 
		cmd.exec("avchange_r 0 0.7");
	}
	if (cvar.avchange_rups1 == 8 )
	{ 
		cmd.exec("avchange_r 0 0.8");
	}
	if (cvar.avchange_rups1 == 9 )
	{ 
		cmd.exec("avchange_r 0 0.9");
	}
	if (cvar.avchange_rups1 == 10 )
	{ 
		cmd.exec("avchange_r 0 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_rups2 == 1 )
	{ 
		cmd.exec("avchange_r 0 1");
	}
	if (cvar.avchange_rups2 == 2 )
	{ 
		cmd.exec("avchange_r 0 2");
	}
	if (cvar.avchange_rups2 == 3 )
	{ 
		cmd.exec("avchange_r 0 3");
	}
	if (cvar.avchange_rups2 == 4 )
	{ 
		cmd.exec("avchange_r 0 4");
	}
	if (cvar.avchange_rups2 == 5 )
	{ 
		cmd.exec("avchange_r 0 5");
	}
	if (cvar.avchange_rups2 == 6 )
	{ 
		cmd.exec("avchange_r 0 6");
	}
	if (cvar.avchange_rups2 == 7 )
	{ 
		cmd.exec("avchange_r 0 7");
	}
	if (cvar.avchange_rups2 == 8 )
	{ 
		cmd.exec("avchange_r 0 8");
	}
	if (cvar.avchange_rups2 == 9 )
	{ 
		cmd.exec("avchange_r 0 9");
	}
	if (cvar.avchange_rups2 == 10 )
	{ 
		cmd.exec("avchange_r 0 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_rups == -1 )
	{ 
		cmd.exec("avchange_r 0 -0.01");
	}
	if (cvar.avchange_rups == -2 )
	{ 
		cmd.exec("avchange_r 0 -0.02");
	}
	if (cvar.avchange_rups == -3 )
	{ 
		cmd.exec("avchange_r 0 -0.03");
	}
	if (cvar.avchange_rups == -4 )
	{ 
		cmd.exec("avchange_r 0 -0.04");
	}
	if (cvar.avchange_rups == -5 )
	{ 
		cmd.exec("avchange_r 0 -0.05");
	}
	if (cvar.avchange_rups == -6 )
	{ 
		cmd.exec("avchange_r 0 -0.06");
	}
	if (cvar.avchange_rups == -7 )
	{ 
		cmd.exec("avchange_r 0 -0.07");
	}
	if (cvar.avchange_rups == -8 )
	{ 
		cmd.exec("avchange_r 0 -0.08");
	}
	if (cvar.avchange_rups == -9 )
	{ 
		cmd.exec("avchange_r 0 -0.09");
	}
	if (cvar.avchange_rups == -10 )
	{ 
		cmd.exec("avchange_r 0 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_rups1 == -1 )
	{ 
		cmd.exec("avchange_r 0 -0.1");
	}
	if (cvar.avchange_rups1 == -2 )
	{ 
		cmd.exec("avchange_r 0 -0.2");
	}
	if (cvar.avchange_rups1 == -3 )
	{ 
		cmd.exec("avchange_r 0 -0.3");
	}
	if (cvar.avchange_rups1 == -4 )
	{ 
		cmd.exec("avchange_r 0 -0.4");
	}
	if (cvar.avchange_rups1 == -5 )
	{ 
		cmd.exec("avchange_r 0 -0.5");
	}
	if (cvar.avchange_rups1 == -6 )
	{ 
		cmd.exec("avchange_r 0 -0.6");
	}
	if (cvar.avchange_rups1 == -7 )
	{ 
		cmd.exec("avchange_r 0 -0.7");
	}
	if (cvar.avchange_rups1 == -8 )
	{ 
		cmd.exec("avchange_r 0 -0.8");
	}
	if (cvar.avchange_rups1 == -9 )
	{ 
		cmd.exec("avchange_r 0 -0.9");
	}
	if (cvar.avchange_rups1 == -10 )
	{ 
		cmd.exec("avchange_r 0 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_rups2 == -1 )
	{ 
		cmd.exec("avchange_r 0 -1");
	}
	if (cvar.avchange_rups2 == -2 )
	{ 
		cmd.exec("avchange_r 0 -2");
	}
	if (cvar.avchange_rups2 == -3 )
	{ 
		cmd.exec("avchange_r 0 -3");
	}
	if (cvar.avchange_rups2 == -4 )
	{ 
		cmd.exec("avchange_r 0 -4");
	}
	if (cvar.avchange_rups2 == -5 )
	{ 
		cmd.exec("avchange_r 0 -5");
	}
	if (cvar.avchange_rups2 == -6 )
	{ 
		cmd.exec("avchange_r 0 -6");
	}
	if (cvar.avchange_rups2 == -7 )
	{ 
		cmd.exec("avchange_r 0 -7");
	}
	if (cvar.avchange_rups2 == -8 )
	{ 
		cmd.exec("avchange_r 0 -8");
	}
	if (cvar.avchange_rups2 == -9 )
	{ 
		cmd.exec("avchange_r 0 -9");
	}
	if (cvar.avchange_rups2 == -10 )
	{ 
		cmd.exec("avchange_r 0 -10");
	}
	//duck right
	//avadd 1 +0.01
	if (cvar.avchange_rupd == 1 )
	{ 
		cmd.exec("avchange_r 1 0.01");
	}
	if (cvar.avchange_rupd == 2 )
	{ 
		cmd.exec("avchange_r 1 0.02");
	}
	if (cvar.avchange_rupd == 3 )
	{ 
		cmd.exec("avchange_r 1 0.03");
	}
	if (cvar.avchange_rupd == 4 )
	{ 
		cmd.exec("avchange_r 1 0.04");
	}
	if (cvar.avchange_rupd == 5 )
	{ 
		cmd.exec("avchange_r 1 0.05");
	}
	if (cvar.avchange_rupd == 6 )
	{ 
		cmd.exec("avchange_r 1 0.06");
	}
	if (cvar.avchange_rupd == 7 )
	{ 
		cmd.exec("avchange_r 1 0.07");
	}
	if (cvar.avchange_rupd == 8 )
	{ 
		cmd.exec("avchange_r 1 0.08");
	}
	if (cvar.avchange_rupd == 9 )
	{ 
		cmd.exec("avchange_r 1 0.09");
	}
	if (cvar.avchange_rupd == 10 )
	{ 
		cmd.exec("avchange_r 1 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_rupd1 == 1 )
	{ 
		cmd.exec("avchange_r 1 0.1");
	}
	if (cvar.avchange_rupd1 == 2 )
	{ 
		cmd.exec("avchange_r 1 0.2");
	}
	if (cvar.avchange_rupd1 == 3 )
	{ 
		cmd.exec("avchange_r 1 0.3");
	}
	if (cvar.avchange_rupd1 == 4 )
	{ 
		cmd.exec("avchange_r 1 0.4");
	}
	if (cvar.avchange_rupd1 == 5 )
	{ 
		cmd.exec("avchange_r 1 0.5");
	}
	if (cvar.avchange_rupd1 == 6 )
	{ 
		cmd.exec("avchange_r 1 0.6");
	}
	if (cvar.avchange_rupd1 == 7 )
	{ 
		cmd.exec("avchange_r 1 0.7");
	}
	if (cvar.avchange_rupd1 == 8 )
	{ 
		cmd.exec("avchange_r 1 0.8");
	}
	if (cvar.avchange_rupd1 == 9 )
	{ 
		cmd.exec("avchange_r 1 0.9");
	}
	if (cvar.avchange_rupd1 == 10 )
	{ 
		cmd.exec("avchange_r 1 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_rupd2 == 1 )
	{ 
		cmd.exec("avchange_r 1 1");
	}
	if (cvar.avchange_rupd2 == 2 )
	{ 
		cmd.exec("avchange_r 1 2");
	}
	if (cvar.avchange_rupd2 == 3 )
	{ 
		cmd.exec("avchange_r 1 3");
	}
	if (cvar.avchange_rupd2 == 4 )
	{ 
		cmd.exec("avchange_r 1 4");
	}
	if (cvar.avchange_rupd2 == 5 )
	{ 
		cmd.exec("avchange_r 1 5");
	}
	if (cvar.avchange_rupd2 == 6 )
	{ 
		cmd.exec("avchange_r 1 6");
	}
	if (cvar.avchange_rupd2 == 7 )
	{ 
		cmd.exec("avchange_r 1 7");
	}
	if (cvar.avchange_rupd2 == 8 )
	{ 
		cmd.exec("avchange_r 1 8");
	}
	if (cvar.avchange_rupd2 == 9 )
	{ 
		cmd.exec("avchange_r 1 9");
	}
	if (cvar.avchange_rupd2 == 10 )
	{ 
		cmd.exec("avchange_r 1 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_rupd == -1 )
	{ 
		cmd.exec("avchange_r 1 -0.01");
	}
	if (cvar.avchange_rupd == -2 )
	{ 
		cmd.exec("avchange_r 1 -0.02");
	}
	if (cvar.avchange_rupd == -3 )
	{ 
		cmd.exec("avchange_r 1 -0.03");
	}
	if (cvar.avchange_rupd == -4 )
	{ 
		cmd.exec("avchange_r 1 -0.04");
	}
	if (cvar.avchange_rupd == -5 )
	{ 
		cmd.exec("avchange_r 1 -0.05");
	}
	if (cvar.avchange_rupd == -6 )
	{ 
		cmd.exec("avchange_r 1 -0.06");
	}
	if (cvar.avchange_rupd == -7 )
	{ 
		cmd.exec("avchange_r 1 -0.07");
	}
	if (cvar.avchange_rupd == -8 )
	{ 
		cmd.exec("avchange_r 1 -0.08");
	}
	if (cvar.avchange_rupd == -9 )
	{ 
		cmd.exec("avchange_r 1 -0.09");
	}
	if (cvar.avchange_rupd == -10 )
	{ 
		cmd.exec("avchange_r 1 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_rupd1 == -1 )
	{ 
		cmd.exec("avchange_r 1 -0.1");
	}
	if (cvar.avchange_rupd1 == -2 )
	{ 
		cmd.exec("avchange_r 1 -0.2");
	}
	if (cvar.avchange_rupd1 == -3 )
	{ 
		cmd.exec("avchange_r 1 -0.3");
	}
	if (cvar.avchange_rupd1 == -4 )
	{ 
		cmd.exec("avchange_r 1 -0.4");
	}
	if (cvar.avchange_rupd1 == -5 )
	{ 
		cmd.exec("avchange_r 1 -0.5");
	}
	if (cvar.avchange_rupd1 == -6 )
	{ 
		cmd.exec("avchange_r 1 -0.6");
	}
	if (cvar.avchange_rupd1 == -7 )
	{ 
		cmd.exec("avchange_r 1 -0.7");
	}
	if (cvar.avchange_rupd1 == -8 )
	{ 
		cmd.exec("avchange_r 1 -0.8");
	}
	if (cvar.avchange_rupd1 == -9 )
	{ 
		cmd.exec("avchange_r 1 -0.9");
	}
	if (cvar.avchange_rupd1 == -10 )
	{ 
		cmd.exec("avchange_r 1 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_rupd2 == -1 )
	{ 
		cmd.exec("avchange_r 1 -1");
	}
	if (cvar.avchange_rupd2 == -2 )
	{ 
		cmd.exec("avchange_r 1 -2");
	}
	if (cvar.avchange_rupd2 == -3 )
	{ 
		cmd.exec("avchange_r 1 -3");
	}
	if (cvar.avchange_rupd2 == -4 )
	{ 
		cmd.exec("avchange_r 1 -4");
	}
	if (cvar.avchange_rupd2 == -5 )
	{ 
		cmd.exec("avchange_r 1 -5");
	}
	if (cvar.avchange_rupd2 == -6 )
	{ 
		cmd.exec("avchange_r 1 -6");
	}
	if (cvar.avchange_rupd2 == -7 )
	{ 
		cmd.exec("avchange_r 1 -7");
	}
	if (cvar.avchange_rupd2 == -8 )
	{ 
		cmd.exec("avchange_r 1 -8");
	}
	if (cvar.avchange_rupd2 == -9 )
	{ 
		cmd.exec("avchange_r 1 -9");
	}
	if (cvar.avchange_rupd2 == -10 )
	{ 
		cmd.exec("avchange_r 1 -10");
	}
	//jump right
*//*	//avadd 2 +0.01
	if (cvar.avchange_rupj == 1 )
	{ 
		cmd.exec("avchange_r 2 0.01");
	}
	if (cvar.avchange_rupj == 2 )
	{ 
		cmd.exec("avchange_r 2 0.02");
	}
	if (cvar.avchange_rupj == 3 )
	{ 
		cmd.exec("avchange_r 2 0.03");
	}
	if (cvar.avchange_rupj == 4 )
	{ 
		cmd.exec("avchange_r 2 0.04");
	}
	if (cvar.avchange_rupj == 5 )
	{ 
		cmd.exec("avchange_r 2 0.05");
	}
	if (cvar.avchange_rupj == 6 )
	{ 
		cmd.exec("avchange_r 2 0.06");
	}
	if (cvar.avchange_rupj == 7 )
	{ 
		cmd.exec("avchange_r 2 0.07");
	}
	if (cvar.avchange_rupj == 8 )
	{ 
		cmd.exec("avchange_r 2 0.08");
	}
	if (cvar.avchange_rupj == 9 )
	{ 
		cmd.exec("avchange_r 2 0.09");
	}
	if (cvar.avchange_rupj == 10 )
	{ 
		cmd.exec("avchange_r 2 0.1");
	}
	//avadd 0 +0.1
	if (cvar.avchange_rupj1 == 1 )
	{ 
		cmd.exec("avchange_r 2 0.1");
	}
	if (cvar.avchange_rupj1 == 2 )
	{ 
		cmd.exec("avchange_r 2 0.2");
	}
	if (cvar.avchange_rupj1 == 3 )
	{ 
		cmd.exec("avchange_r 2 0.3");
	}
	if (cvar.avchange_rupj1 == 4 )
	{ 
		cmd.exec("avchange_r 2 0.4");
	}
	if (cvar.avchange_rupj1 == 5 )
	{ 
		cmd.exec("avchange_r 2 0.5");
	}
	if (cvar.avchange_rupj1 == 6 )
	{ 
		cmd.exec("avchange_r 2 0.6");
	}
	if (cvar.avchange_rupj1 == 7 )
	{ 
		cmd.exec("avchange_r 2 0.7");
	}
	if (cvar.avchange_rupj1 == 8 )
	{ 
		cmd.exec("avchange_r 2 0.8");
	}
	if (cvar.avchange_rupj1 == 9 )
	{ 
		cmd.exec("avchange_r 2 0.9");
	}
	if (cvar.avchange_rupj1 == 10 )
	{ 
		cmd.exec("avchange_r 2 1.0");
	}
	//avadd 0 +1
	if (cvar.avchange_rupj2 == 1 )
	{ 
		cmd.exec("avchange_r 2 1");
	}
	if (cvar.avchange_rupj2 == 2 )
	{ 
		cmd.exec("avchange_r 2 2");
	}
	if (cvar.avchange_rupj2 == 3 )
	{ 
		cmd.exec("avchange_r 2 3");
	}
	if (cvar.avchange_rupj2 == 4 )
	{ 
		cmd.exec("avchange_r 2 4");
	}
	if (cvar.avchange_rupj2 == 5 )
	{ 
		cmd.exec("avchange_r 2 5");
	}
	if (cvar.avchange_rupj2 == 6 )
	{ 
		cmd.exec("avchange_r 2 6");
	}
	if (cvar.avchange_rupj2 == 7 )
	{ 
		cmd.exec("avchange_r 2 7");
	}
	if (cvar.avchange_rupj2 == 8 )
	{ 
		cmd.exec("avchange_r 2 8");
	}
	if (cvar.avchange_rupj2 == 9 )
	{ 
		cmd.exec("avchange_r 2 9");
	}
	if (cvar.avchange_rupj2 == 10 )
	{ 
		cmd.exec("avchange_r 2 10");
	}
	//avadd 0 -0.01
	if (cvar.avchange_rupj == -1 )
	{ 
		cmd.exec("avchange_r 2 -0.01");
	}
	if (cvar.avchange_rupj == -2 )
	{ 
		cmd.exec("avchange_r 2 -0.02");
	}
	if (cvar.avchange_rupj == -3 )
	{ 
		cmd.exec("avchange_r 2 -0.03");
	}
	if (cvar.avchange_rupj == -4 )
	{ 
		cmd.exec("avchange_r 2 -0.04");
	}
	if (cvar.avchange_rupj == -5 )
	{ 
		cmd.exec("avchange_r 2 -0.05");
	}
	if (cvar.avchange_rupj == -6 )
	{ 
		cmd.exec("avchange_r 2 -0.06");
	}
	if (cvar.avchange_rupj == -7 )
	{ 
		cmd.exec("avchange_r 2 -0.07");
	}
	if (cvar.avchange_rupj == -8 )
	{ 
		cmd.exec("avchange_r 2 -0.08");
	}
	if (cvar.avchange_rupj == -9 )
	{ 
		cmd.exec("avchange_r 2 -0.09");
	}
	if (cvar.avchange_rupj == -10 )
	{ 
		cmd.exec("avchange_r 2 -0.1");
	}
	//avadd 0 -0.1
	if (cvar.avchange_rupj1 == -1 )
	{ 
		cmd.exec("avchange_r 2 -0.1");
	}
	if (cvar.avchange_rupj1 == -2 )
	{ 
		cmd.exec("avchange_r 2 -0.2");
	}
	if (cvar.avchange_rupj1 == -3 )
	{ 
		cmd.exec("avchange_r 2 -0.3");
	}
	if (cvar.avchange_rupj1 == -4 )
	{ 
		cmd.exec("avchange_r 2 -0.4");
	}
	if (cvar.avchange_rupj1 == -5 )
	{ 
		cmd.exec("avchange_r 2 -0.5");
	}
	if (cvar.avchange_rupj1 == -6 )
	{ 
		cmd.exec("avchange_r 2 -0.6");
	}
	if (cvar.avchange_rupj1 == -7 )
	{ 
		cmd.exec("avchange_r 2 -0.7");
	}
	if (cvar.avchange_rupj1 == -8 )
	{ 
		cmd.exec("avchange_r 2 -0.8");
	}
	if (cvar.avchange_rupj1 == -9 )
	{ 
		cmd.exec("avchange_r 2 -0.9");
	}
	if (cvar.avchange_rupj1 == -10 )
	{ 
		cmd.exec("avchange_r 2 -1.0");
	}
	//avadd 0 -1
	if (cvar.avchange_rupj2 == -1 )
	{ 
		cmd.exec("avchange_r 2 -1");
	}
	if (cvar.avchange_rupj2 == -2 )
	{ 
		cmd.exec("avchange_r 2 -2");
	}
	if (cvar.avchange_rupj2 == -3 )
	{ 
		cmd.exec("avchange_r 2 -3");
	}
	if (cvar.avchange_rupj2 == -4 )
	{ 
		cmd.exec("avchange_r 2 -4");
	}
	if (cvar.avchange_rupj2 == -5 )
	{ 
		cmd.exec("avchange_r 2 -5");
	}
	if (cvar.avchange_rupj2 == -6 )
	{ 
		cmd.exec("avchange_r 2 -6");
	}
	if (cvar.avchange_rupj2 == -7 )
	{ 
		cmd.exec("avchange_r 2 -7");
	}
	if (cvar.avchange_rupj2 == -8 )
	{ 
		cmd.exec("avchange_r 2 -8");
	}
	if (cvar.avchange_rupj2 == -9 )
	{ 
		cmd.exec("avchange_r 2 -9");
	}
	if (cvar.avchange_rupj2 == -10 )
	{ 
		cmd.exec("avchange_r 2 -10");
	}*/

	//stand +-1
	if(avchange_hups)
	{
		cmd.exec("avchange_h 0 1.0");
		avchange_hups=0;
	}
	if(avchange_hdowns)
	{
		cmd.exec("avchange_h 0 -1.0");
		avchange_hdowns=0;
	}
	if(avchange_fups)
	{
		cmd.exec("avchange_f 0 1.0");
		avchange_fups=0;
	}
	if(avchange_fdowns)
	{
		cmd.exec("avchange_f 0 -1.0");
		avchange_fdowns=0;
	}
	if(avchange_rups)
	{
		cmd.exec("avchange_r 0 1");
		avchange_rups=0;
	}
	if(avchange_rdowns)
	{
		cmd.exec("avchange_r 0 -1");
		avchange_rdowns=0;
	}
	//duck +-1
	if(avchange_hupd)
	{
		cmd.exec("avchange_h 1 1.0");
		avchange_hupd=0;
	}
	if(avchange_hdownd)
	{
		cmd.exec("avchange_h 1 -1.0");
		avchange_hdownd=0;
	}
	if(avchange_fupd)
	{
		cmd.exec("avchange_f 1 1.0");
		avchange_fupd=0;
	}
	if(avchange_fdownd)
	{
		cmd.exec("avchange_f 1 -1.0");
		avchange_fdownd=0;
	}
	if(avchange_rupd)
	{
		cmd.exec("avchange_r 1 1");
		avchange_rupd=0;
	}
	if(avchange_rdownd)
	{
		cmd.exec("avchange_r 1 -1");
		avchange_rdownd=0;
	}
	//jump +-1
	if(avchange_hupj)
	{
		cmd.exec("avchange_h 2 1.0");
		avchange_hupj=0;
	}
	if(avchange_hdownj)
	{
		cmd.exec("avchange_h 2 -1.0");
		avchange_hdownj=0;
	}
	if(avchange_fupj)
	{
		cmd.exec("avchange_f 2 1.0");
		avchange_fupj=0;
	}
	if(avchange_fdownj)
	{
		 cmd.exec("avchange_f 2 -1.0");
		avchange_fdownj=0;
	}
	if(avchange_rupj)
	{
		cmd.exec("avchange_r 2 1");
		avchange_rupj=0;
	}
	if(avchange_rdownj)
	{
		cmd.exec("avchange_r 2 -1");
		avchange_rdownj=0;
	}
	//stand +-0.1
	if(avchange_hups1)
	{
		cmd.exec("avchange_h 0 0.1");
		avchange_hups1=0;
	}
	if(avchange_hdowns1)
	{
		cmd.exec("avchange_h 0 -0.1");
		avchange_hdowns1=0;
	}
	if(avchange_fups1)
	{
		cmd.exec("avchange_f 0 0.1");
		avchange_fups1=0;
	}
	if(avchange_fdowns1)
	{
		cmd.exec("avchange_f 0 -0.1");
		avchange_fdowns1=0;
	}
	if(avchange_rups1)
	{
		cmd.exec("avchange_r 0 0.1");
		avchange_rups1=0;
	}
	if(avchange_rdowns1)
	{
		cmd.exec("avchange_r 0 -0.1");
		avchange_rdowns1=0;
	}
	//duck +-0.1
	if(avchange_hupd1)
	{
		cmd.exec("avchange_h 1 0.1");
		avchange_hupd1=0;
	}
	if(avchange_hdownd1)
	{
		cmd.exec("avchange_h 1 -0.1");
		avchange_hdownd1=0;
	}
	if(avchange_fupd1)
	{
		cmd.exec("avchange_f 1 0.1");
		avchange_fupd1=0;
	}
	if(avchange_fdownd1)
	{
		cmd.exec("avchange_f 1 -0.1");
		avchange_fdownd1=0;
	}
	if(avchange_rupd1)
	{
		cmd.exec("avchange_r 1 0.1");
		avchange_rupd1=0;
	}
	if(avchange_rdownd1)
	{
		cmd.exec("avchange_r 1 -0.1");
		avchange_rdownd1=0;
	}
	//jump +-0.1
	if(avchange_hupj1)
	{
		cmd.exec("avchange_h 2 0.1");
		avchange_hupj1=0;
	}
	if(avchange_hdownj1)
	{
		cmd.exec("avchange_h 2 -0.1");
		avchange_hdownj1=0;
	}
	if(avchange_fupj1)
	{
		cmd.exec("avchange_f 2 0.1");
		avchange_fupj1=0;
	}
	if(avchange_fdownj1)
	{
		 cmd.exec("avchange_f 2 -0.1");
		avchange_fdownj1=0;
	}
	if(avchange_rupj1)
	{
		cmd.exec("avchange_r 2 0.1");
		avchange_rupj1=0;
	}
	if(avchange_rdownj1)
	{
		cmd.exec("avchange_r 2 -0.1");
		avchange_rdownj1=0;
	}
	//stand +-0.01
	if(avchange_hups2)
	{
		cmd.exec("avchange_h 0 0.01");
		avchange_hups2=0;
	}
	if(avchange_hdowns2)
	{
		cmd.exec("avchange_h 0 -0.01");
		avchange_hdowns2=0;
	}
	if(avchange_fups2)
	{
		cmd.exec("avchange_f 0 0.01");
		avchange_fups2=0;
	}
	if(avchange_fdowns2)
	{
		cmd.exec("avchange_f 0 -0.01");
		avchange_fdowns2=0;
	}
	if(avchange_rups2)
	{
		cmd.exec("avchange_r 0 0.01");
		avchange_rups2=0;
	}
	if(avchange_rdowns2)
	{
		cmd.exec("avchange_r 0 -0.01");
		avchange_rdowns2=0;
	}
	//duck +-0.01
	if(avchange_hupd2)
	{
		cmd.exec("avchange_h 1 0.01");
		avchange_hupd2=0;
	}
	if(avchange_hdownd2)
	{
		cmd.exec("avchange_h 1 -0.01");
		avchange_hdownd2=0;
	}
	if(avchange_fupd2)
	{
		cmd.exec("avchange_f 1 0.01");
		avchange_fupd2=0;
	}
	if(avchange_fdownd2)
	{
		cmd.exec("avchange_f 1 -0.01");
		avchange_fdownd2=0;
	}
	if(avchange_rupd2)
	{
		cmd.exec("avchange_r 1 0.01");
		avchange_rupd2=0;
	}
	if(avchange_rdownd2)
	{
		cmd.exec("avchange_r 1 -0.01");
		avchange_rdownd2=0;
	}
	//jump +-0.01
	if(avchange_hupj2)
	{
		cmd.exec("avchange_h 2 0.01");
		avchange_hupj2=0;
	}
	if(avchange_hdownj2)
	{
		cmd.exec("avchange_h 2 -0.01");
		avchange_hdownj2=0;
	}
	if(avchange_fupj2)
	{
		cmd.exec("avchange_f 2 0.01");
		avchange_fupj2=0;
	}
	if(avchange_fdownj2)
	{
		 cmd.exec("avchange_f 2 -0.01");
		avchange_fdownj2=0;
	}
	if(avchange_rupj2)
	{
		cmd.exec("avchange_r 2 0.01");
		avchange_rupj2=0;
	}
	if(avchange_rdownj2)
	{
		cmd.exec("avchange_r 2 -0.01");
		avchange_rdownj2=0;
	}
	if(me.alive && cvar.vec_switch == 1 && cvar.vecadjust == 1)
	{
		cmd.exec("vec_switch 0");
	}
	if(me.alive && cvar.vecadjust == 0)
	{
		cmd.exec("vec_switch 1");
	}
	//-------------------------¬∑ÔøΩﬂøÔøΩÔøΩÔøΩ End-------------------------------
	//∂≥Ω·
	static bool previous_frozen_state=false;
	if(relativeSpeed() > 10.0 && (me.pmFlags & FL_ONGROUND))
	{
		isInFrozenTime = false;
	}
	if(previous_frozen_state && !isInFrozenTime)  //∂≥Ω· ±º‰∏’π˝À≤º‰(≈–∂œ«∞Ã·:“™”–“∆∂Øµƒ∂Ø◊˜)
	{
		cvar.speed = 512.0;
	}
	previous_frozen_state = isInFrozenTime;
	//
	static bool bInit=false;
	if (!bInit)
	{
		globalTime = (double*)*(PDWORD)((DWORD)pEngfuncs->pNetAPI->SendRequest+0x51);
		screeninfo.iSize=sizeof(SCREENINFO);
		gEngfuncs.pfnGetScreenInfo(&screeninfo);
		displayCenterX=screeninfo.iWidth/2;
		displayCenterY=screeninfo.iHeight/2;
		//
		cvar.Init();
		gConsole.echo("\n");
		gConsole.echo("\n");
		gConsole.echo("\n");
		exec("Main.cfg");
		exec("Vec-.cfg");
		exec("Burst.cfg");
		exec("Recoil.cfg");
 //◊‘∂Ø¬Ú«π
	/*	cmd.exec("bind l \"sayx\"");
//◊‘Ñ”õ_¥Ã
//MOUSE1¬∑œﬂ≥Â¥Ã
//==============================================================
		cmd.exec("bind = \"way_draw change\"");
		cmd.exec("bind f \"chase_cam change\"");
		cmd.exec("bind r \"+rrr\"");*/
		cmd.exec("alias d \"avclear1;avadd 1 25.66 5.75 1.78\"");   //Squatting
		cmd.exec("alias dk \"avclear1;avadd 1 22.66 10.75 -3\"");   //Squatting the other side
		cmd.exec("alias dr \"avclear1;avadd 1 25.66 5.75 1.78\"");  //The other squatting bullets
		cmd.exec("alias z \"avclear2;avadd 0 21.27 7.32 2.019\"");  //The other station
		cmd.exec("alias zk \"avclear2;avadd 0 21.27 10.32 5.01\""); //The other side of the station shot
		cmd.exec("alias zr \"avclear2;avadd 0 21.27 7.32 2.019\""); //The other station for bullets
//==============================================================
		gConsole.echo("&oInfo:");
		gConsole.echo("");
		char maker[] = {'V','i','k','i','n','g',' ','W','p','t',0};
		gConsole.echo("&w + &oVersion:&w %s",maker);
		gConsole.echo("");
		char myqq[] = {'2','7','3','7','4','6','6','2','7','1',0};
		Con_Echo("&w + &oMy QQ:&w %s",myqq);
		gConsole.echo("");
		char myid[] = {'e','V','O','L',0};
		Con_Echo("&w + &oCS1.6 My ID:&w %s",myid);
		gConsole.echo("");
		const char sDateStamp[255] = __DATE__;
		const char sCompileStamp[255] = __TIME__;
		gConsole.echo("&w + &oDate:&w %s %s",sDateStamp,sCompileStamp);
		gConsole.echo("");
		gConsole.echo(/*&o-=[eVOL]=- &w> all*/XorStr<0x88,21,0xCE3B6838>("\xAE\xE6\xA7\xB6\xD7\xE8\xD8\xC0\xDC\xCC\xAF\xBE\xB4\xB3\xE1\xA9\xB8\xF8\xF6\xF7"+0xCE3B6838).s);
		gConsole.echo("");
//		exec(/*-=[CB]=- Hook V9*/XorStr<0xEC,17,0x1D758306>("\xC1\xD0\xB5\xAC\xB2\xAC\xCF\xDE\xD4\xBD\x99\x98\x93\xD9\xAC\xC2"+0x1D758306).s);
//		exec(/*Myth2 Hook v9*/XorStr<0x6E,14,0x3A9F9DCA>("\x23\x16\x04\x19\x40\x53\x3C\x1A\x19\x1C\x58\x0F\x43"+0x3A9F9DCA).s);

		bInit=true;
	}

	if (cvar.crosshair) DrawCrosshair();
	// menu init/draw
	gMenu.Init();
	gMenu.Draw();

	updateLocalPlayer();
	ClientTime::reportMapTime(gEngfuncs.GetClientTime());
	if(cvar.cdon)
	{
		cmd.exec("#cd_version;#cd_fps;#cd_sound");
	}
	if(cvar.chase_cam) 
	{	
		cmd.exec("chase_active hide 1;r_drawviewmodel hide 0");
		chaseofs = -100;
	} else {
		cmd.exec("chase_active hide 0;r_drawviewmodel hide 1");
		chaseofs = 0.0;
	}
	if( tWelcomeMessage.running() )
	{
		DrawHudStringCenter(displayCenterX,displayCenterY-70,235,138,0,gWelcomeMessage );
	}

	if( gHudTimer2.running() )
	{
		DrawHudString1(10,screeninfo.iHeight - 300,255,255,255,gHudMessage2 );
	}

	cl_entity_t *wpn = gEngfuncs.GetViewModel();

	// === blink 
	if(blinkTimer.expired())
	{
		blinkPhase = !blinkPhase;
		if(blinkPhase){ blinkTimer.countdown(0.2); }
		else          { blinkTimer.countdown(0.1); }
	}

	// === check for scheduled commands:
	for( ScheduleList::iterator pos = scheduleList.begin();
		 pos != scheduleList.end();
		 ++pos)
	{
		if( (*pos).timer.expired() )
		{
			cmd.exec( const_cast<char*>((*pos).cmd.c_str()) );
			scheduleList.erase(pos);
			break;
		}	
	}
    if (cvar.way_on == 1 && cvar.norefresh == 1 )
	{ 
		hlexec("r_norefresh 1");
	}
	else
	if (cvar.way_on == 0)
	{
		hlexec("r_norefresh 0");
	}
	if (cvar.norefresh == 1 )
	{ 
		cvar.norefreshr = 1;
	}
	if (cvar.norefresh == 0)
	{
		cvar.norefreshr = 0;
	}

	if (cvar.burstmode == 1 )
	{ 
		cvar.autoburst = 1;
//		cmd.exec("txt Auto burst ON");
	}
	if (cvar.burstmode == 2 )
	{ 
		cvar.autoburst = 0;
		cvar.sdelay = 0.03f;
		cvar.rdelay = 0.0f;
		cvar.dtime = 0.0f;
		cvar.ftime = 1.0f;
		cvar.burstflags = 0;
//		cmd.exec("txt Auto burst OFF");
	}
	if (cvar.burstmode == 3 )
	{ 
		cvar.autoburst = 0;
		cvar.sdelay = 0.05f;
		cvar.rdelay = 0.0f;
		cvar.dtime = 0.5f;
		cvar.ftime = 0.1f;
		cvar.burstflags = 0;
//		cmd.exec("txt Short bursts ON");
	}
	if (cvar.burstmode == 4 )
	{ 
		cvar.autoburst = 0;
		cvar.sdelay = 0.05f;
		cvar.rdelay = 0.0f;
		cvar.dtime = 0.4f;
		cvar.ftime = 0.2f;
		cvar.burstflags = 0;
//		cmd.exec("txt Medium bursts ON");
	}
	if (cvar.burstmode == 5 )
	{ 
		cvar.autoburst = 0;
		cvar.sdelay = 0.05f;
		cvar.rdelay = 0.0f;
		cvar.dtime = 0.25f;
		cvar.ftime = 0.1f;
		cvar.burstflags = 0;
//		cmd.exec("txt Fast bursts ON");
	}
	if (cvar.burstmode == 1)
	{
		cvar.burstmoder = 1;
	}
	if (cvar.burstmode == 2)
	{
		cvar.burstmoder = 2;
	}
	if (cvar.burstmode == 3)
	{
		cvar.burstmoder = 3;
	}
	if (cvar.burstmode == 4)
	{
		cvar.burstmoder = 4;
	}
	if (cvar.burstmode == 5)
	{
		cvar.burstmoder = 5;
	}

	/////////////////////
	if (cvar.cross == 0 )
	{ 
		cvar.crosshair = 0;
	}
	if (cvar.cross == 1 )
	{ 
		cvar.crosshair = 1;
	}
	if (cvar.cross == 2 )
	{ 
		cvar.crosshair = 2;
	}
	if (cvar.cross == 3 )
	{ 
		cvar.crosshair = 3;
	}
	if (cvar.cross == 4 )
	{ 
		cvar.crosshair = 4;
	}
	if (cvar.cross == 5 )
	{ 
		cvar.crosshair = 5;
	}
	if (cvar.cross == 6 )
	{ 
		cvar.crosshair = 6;
	}
	if (cvar.cross == 7 )
	{ 
		cvar.crosshair = 7;
	}
	if (cvar.cross == 8 )
	{ 
		cvar.crosshair = 8;
	}
	if (cvar.cross == 9 )
	{ 
		cvar.crosshair = 9;
	}
	if (cvar.cross == 10 )
	{ 
		cvar.crosshair = 10;
	}
	//////////////////////////////
	if (cvar.way_lock == 0 )
	{ 
		cvar.way_lock = 0;
	}if (cvar.way_locks == 1 )
	{ 
		cvar.way_lock = 1;
	}if (cvar.way_locks == 2 )
	{ 
		cvar.way_lock = 2;
	}if (cvar.way_locks == 3 )
	{ 
		cvar.way_lock = 3;
	}if (cvar.way_locks == 4 )
	{ 
		cvar.way_lock = 4;
	}if (cvar.way_locks == 5 )
	{ 
		cvar.way_lock = 5;
	}if (cvar.way_locks == 6 )
	{ 
		cvar.way_lock = 6;
	}if (cvar.way_locks == 7 )
	{ 
		cvar.way_lock = 7;
	}if (cvar.way_locks == 8 )
	{ 
		cvar.way_lock = 8;
	}

	////////////////////

	////////////////////
	if (cvar.walls == 0 )
	{ 
		cvar.wall = 0;
	}
	if (cvar.walls == 1 )
	{ 
		cvar.wall = 1;
	}
	if (cvar.walls == 2 )
	{ 
		cvar.wall = 2;
	}
	if (cvar.walls == 3 )
	{ 
		cvar.wall = 3;
	}
	if (cvar.walls == 4 )
	{ 
		cvar.wall = 4;
	}
	if (cvar.walls == 5 )
	{ 
		cvar.wall = 5;
	}
	if (cvar.walls == 6 )
	{ 
		cvar.wall = 6;
	}
	if (cvar.walls == 7 )
	{ 
		cvar.wall = 7;
	}
	if (cvar.walls == 8 )
	{ 
		cvar.wall = 8;
	}
	if (cvar.walls == 9 )
	{ 
		cvar.wall = 9;
	}
	if (cvar.walls == 10 )
	{ 
		cvar.wall = 10;
	}
	if (cvar.walls == 11 )
	{ 
		cvar.wall = 11;
	}
	if (cvar.walls == 12 )
	{ 
		cvar.wall = 12;
	}
	if (cvar.walls == 13 )
	{ 
		cvar.wall = 13;
	}
	///////////////
	if (cvar.way_mode == 0 )
	{ 
		cvar.way_moder = 0;
	}
	if (cvar.way_mode == 1 )
	{ 
		cvar.way_moder = 1;
	}
	//////////////////////////////////
	//////////////////////////////////
	
	if (cvar.shield_attack == 0 )
	{ 
		cvar.shield_attackr = 0;
	}
	if (cvar.shield_attack == 1 )
	{ 
		cvar.shield_attackr = 1;
	}
	/////////////////////////////////
	
	if (cvar.noall == 0 )
	{ 
		cvar.noallr = 0;
	}
	if (cvar.noall == 1 )
	{ 
		cvar.noallr = 1;
	}
	
	//////////////
	// === flash removal
	if (cvar.flash)
	{
		static screenfade_t sf = { 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0 }; 
		gEngfuncs.pfnSetScreenFade( &sf ); 
	}
	if(cvar.smoke)	{gEngfuncs.pfnHookEvent("events/createsmoke.sc",SmokeEvent);}
	
	if (cvar.crosshair) DrawCrosshair();

	int cW = screeninfo.iWidth/1.7;
	int cH = screeninfo.iHeight/1.3;
	int cX = displayCenterX - cW/2;
	int cY = displayCenterY - cH/2;
	gConsole.draw(cX,cY,cW-20,cH);//Draw the console
	
	if (player_active)    { drawPlayer          (cX,cY,cW-20); }
	
	UpdateScrollbarCon();
	if (cvar.soundfilter == 1)	{drawSound();}
	if (cvar.radar == 1)	{DrawRadar();}

//=======================================================================================
    if(cvar.tophud && !cvar.rush)
	{
		PlayerInfo& r = vPlayers[me.ent->index];
		static int hit_x = 4;
		static int hit_y = 50;
		char message_name[] = {'V','i','k','i','n','g',' ','W','p','t',0};
		
		oglSubtractive=true;
		gEngfuncs.pfnFillRGBA(hit_x, hit_y, 160, 16,255,0,0,128);
		gEngfuncs.pfnFillRGBA(hit_x, hit_y+16, 160, 48,0,0,255,128);
		oglSubtractive=false;

		DrawGlowHudString(hit_x, hit_y, 20, 20, 50, "%s Info panel",message_name );
     	
		SYSTEMTIME SysTime;
	    GetLocalTime(&SysTime);
		char* formatstring   =   (SysTime.wMinute<10) ?  "Time:%d:0%d" :  "Systime: %d:%d";
        
		DrawHudString1(hit_x, hit_y+20, 255, 255, 255, formatstring, SysTime.wHour, SysTime.wMinute);
		DrawHudString1(hit_x, hit_y+36,255,255,255,"Kills:%d", r.frags);
		DrawHudString1(hit_x, hit_y+52,255,255,255,"Deaths:%d", r.deaths);
	}
	if(cvar.vechud && !cvar.rush)
	{
		PlayerInfo& r = vPlayers[me.ent->index];
		static int hit_x = 4;
		static int hit_y = 50;

		oglSubtractive=true;
		gEngfuncs.pfnFillRGBA(hit_x, hit_y+64, 240, 130,0,0,255,128);
		oglSubtractive=false;

        DrawHudString1(hit_x, hit_y+68, 255,255,255, "Aimvecs:" );
     	
		DrawHudString1(hit_x, hit_y+84, 255,255,255, "%s",gGetWeaponName(me.ent->curstate.weaponmodel));
		
		DrawHudString1(hit_x, hit_y+100, 255,255,255,"  Standing");
		for (vector<aimvec>::iterator si = Aimbot.AimVecsS.begin(); si != Aimbot.AimVecsS.end();++si) 
		{ 
			DrawHudString1(hit_x, hit_y+116,255,255,255," %1.5f | %1.5f | %1.5f ",si->h,si->f,si->r);
		}

		DrawHudString1(hit_x, hit_y+132,255,255,255,"  Ducking");
		for (vector<aimvec>::iterator si2 = Aimbot.AimVecsD.begin(); si2 != Aimbot.AimVecsD.end();++si2) 
		{ 
			DrawHudString1(hit_x, hit_y+148,255,255,255," %1.5f | %1.5f | %1.5f ",si2->h,si2->f,si2->r);
		}

		DrawHudString1(hit_x, hit_y+164,255,255,255,"  Jumping");
		for (vector<aimvec>::iterator si3 = Aimbot.AimVecsJ.begin(); si3 != Aimbot.AimVecsJ.end();++si3) 
		{ 
			DrawHudString1(hit_x, hit_y+180,255,255,255," %1.5f | %1.5f | %1.5f ",si3->h,si3->f,si3->r);
		}
	}
//=======================================================================================
	if(cvar.sethud && !cvar.rush){

		
		float ubr=cvar.red; float ubg=cvar.green; float ubb=cvar.blue; 
	    float lbr=1; float lbg=1; float lbb=1; float ba=0.5;
	    float uhr=cvar.red; float uhg=cvar.green; float uhb=cvar.blue; 
	    float lhr=1; float lhg=1; float lhb=1; float ha=0.5;
	    float bor=1; float bog=1; float bob=1; float boa=1; int line=1;


		static int xx = ((displayCenterX*2)- 60);
		static int yy = (screeninfo.iHeight-155);
		static int val=5;
		static int h=72+36+8;
		static int X=xx-49;
		static int Y=yy+10-1;
		static int W=100+1;
		static int H=h+1+5;
		oglSubtractive=true;
		gEngfuncs.pfnFillRGBA((displayCenterX*2)- 120,screeninfo.iHeight-141, 115, 16,255,0,0,128);
		gEngfuncs.pfnFillRGBA((displayCenterX*2)- 120,screeninfo.iHeight-125, 115, 124,0,0,255,128);
		oglSubtractive=false;
		DrawGlowHudString(xx-45,yy+10+val,0,1,0,"Viking Wpt");
//		gui.Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder
			( xx-49-1, 
		           yy+10-1, 
			       100+1+1+1-1+1, 
			       h+12, 
			       0, 
			       ubr, 
			       ubg, 
			       ubb, 
			       lbr, 
			       lbg, 
			       lbb, 
			       ba, 
			       uhr, 
			       uhg, 
			       uhb, 
			       lhr, 
			       lhg, 
			       lhb, 
			       ha, 
			       bor, 
			       bog, 
			       bob, 
			       boa, 
			       line
			);

		/*if(!cvar.language)*/{

			switch(cvar.noallr){
			case 0:
			{
				DrawConString(xx-45,yy+48+val,255,255,255,"Noall: 0");
			}
			break;
			case 1:
			{
				DrawConString(xx-45,yy+48+val,255,255,255,"Noall: 1");
			}
			break;
			default:
			break;
			}

			switch(cvar.id_mode){
			case 0:
			{
				DrawConString(xx-45,yy+66+val,255,255,255,"Id_mode: 0");
			}
			break;
			case 1:
			{
				DrawConString(xx-45,yy+66+val,255,255,255,"Id_mode: 1");
			}
			break;
			case 2:
			{
				DrawConString(xx-45,yy+66+val,255,255,255,"Id_mode: 2");
			}
			break;
			default:
			break;
			}

			switch(cvar.way_moder){
			case 0:
			{
				DrawConString(xx-45,yy+84+val,255,255,255,"Way_mode: 0");
			}
			break;
			case 1:
			{
				DrawConString(xx-45,yy+84+val,255,255,255,"Way_mode: 1");
			}
			break;
			default:
			break;
			}

			switch(cvar.norefreshr){
			case 0:
			{
				DrawConString(xx-45,yy+102+val,255,255,255,"Norefresh: 0");
			}
			break;
			case 1:
			{
				DrawConString(xx-45,yy+102+val,255,255,255,"Norefresh: 1");
			}
			break;
			default:
			break;
			}

			switch(cvar.shield_attackr){
			case 0:
			{
				DrawConString(xx-45,yy+120+val,255,255,255,"Shield_aim: 0");
			}
			break;
			case 1:
			{
				DrawConString(xx-45,yy+120+val,255,255,255,"Shield_aim: 1");
			}
			break;
			default:
			break;
			}

			switch(cvar.burstmoder){
			case 1:
			{
				DrawConString(xx-45,yy+30+val,255,255,255,"Autoburst ON");
			}
			break;
			case 2:
			{
				DrawConString(xx-45,yy+30+val,255,255,255,"Burst Off");
			}
			break;
			case 3:
			{
				DrawConString(xx-45,yy+30+val,255,255,255,"Burst Short");
			}
			break;
			case 4:
			{
				DrawConString(xx-45,yy+30+val,255,255,255,"Burst Medium");
			}
			break;
			case 5:
			{
				DrawConString(xx-45,yy+30+val,255,255,255,"Burst Fast");
			}
			break;
			default:
			break;
			}/*
		}
		else
		{
			switch(cvar.followmode){
			case 0:
			{
				DrawGlowHudString(xx+2,yy+20+val,0,1,0,  "ÂæÖÊú∫Ê®°Âºè: 0");
			}
			break;
			case 1:
			{
				DrawGlowHudString(xx+2,yy+20+val,0,1,0,  "ÂæÖÊú∫Ê®°Âºè: 1");
			}
			break;
			case 2:
			{
				DrawGlowHudString(xx+2,yy+20+val,0,1,0,  "ÂæÖÊú∫Ê®°Âºè: 2");
			}
			break;
			default:
			break;
			}

			switch(cvar.vecmode){
			case 0:
			{
				DrawGlowHudString(xx+2,yy+40+val,0,1,0,"ÁûÑÂáÜÊ®°Âºè: 0");
			}
			break;
			case 1:
			{
				DrawGlowHudString(xx+2,yy+40+val,0,1,0,"ÁûÑÂáÜÊ®°Âºè: 1");
			}
			break;
			case 2:
			{
				DrawGlowHudString(xx+2,yy+40+val,0,1,0,"ÁûÑÂáÜÊ®°Âºè: 2");
			}
			break;
			default:
			break;
			}

			switch(cvar.way_mode){
			case 0:
			{
				DrawGlowHudString(xx+2,yy+60+val,0,1,0,  "Ë∑ØÁ∫øÊ®°Âºè: 0");
			}
			break;
			case 1:
			{
				DrawGlowHudString(xx+2,yy+60+val,0,1,0,  "Ë∑ØÁ∫øÊ®°Âºè: 1");
			}
			break;
			case 2:
			{
				DrawGlowHudString(xx+2,yy+60+val,0,1,0,  "Ë∑ØÁ∫øÊ®°Âºè: 2");
			}
			break;
			default:
			break;
			}

			switch(cvar.shield_attack){
			case 0:
			{
				DrawGlowHudString(xx+2,yy+80+val,0,1,0,"ÊîªÂáªÁõæÁâå: 0");
			}
			break;
			case 1:
			{
				DrawGlowHudString(xx+2,yy+80+val,0,1,0,"ÊîªÂáªÁõæÁâå: 1");
			}
			break;
			default:
			break;
			}

			switch(cvar.auto_switch){
			case 0:
			{
				DrawGlowHudString(xx+2,yy+100+val,0,1,0,"Êç¢Êû™ÂàáÊç¢: 0");
			}
			break;
			case 1:
			{
				DrawGlowHudString(xx+2,yy+100+val,0,1,0,"Êç¢Êû™ÂàáÊç¢: 1");
			}
			break;
			default:
			break;
			}*/
		}
//	    Background(X,Y,W,H);
	}
	if(bDisplayScroll)
	{

		float ubr=cvar.red; float ubg=cvar.green; float ubb=cvar.blue; //Ω•±‰—’…´
		float lbr=1; float lbg=1; float lbb=1; float ba=0.5;
		float uhr=cvar.red; float uhg=cvar.green; float uhb=cvar.blue; //Ω•±‰—’…´
		float lhr=1; float lhg=1; float lhb=1; float ha=0.5;
		float bor=1; float bog=1; float bob=1; float boa=1; int line=1;
		static int hit_x = 1040;
		gEngfuncs.pfnFillRGBA(-1,-1,displayCenterX*2+2,35,0,0,255,128);
//		gui.Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(-1,-1,displayCenterX*2+2,35,16,ubr,ubg,ubb,lbr,lbg,lbb,ba,uhr,uhg,uhb,lhr,lhg,lhb,ha,bor,bog,bob,boa,line);
		//
		char buf[255];
		sprintf(buf,"Viking Wpt 0.1 by -=eVOL=- my QQ:2737466271 visit www.cstrikenoobhackers.com");
		DrawGlowHudString(hit_x, 10,0,1,0, buf);	
		
		if(hit_x < 0 - screeninfo.charWidths['0'] * 63)
		{
			fFade = fFade - 1;
		} else 
		{
			hit_x = hit_x - 1.0f;
			if(fFade < 0.4f)
				fFade = fFade + 0.005f;
		}
		if(fFade <= 0)
		{
			bDisplayScroll = false;
//			cmd.exec("#developer 1");
//			cmd.exec("#cl_showfps 1");

//			UpdateScreenInfo();
		}
		//	int abcd =screeninfo.iWidth;
		//	char buf2[255];
		//	sprintf(buf2,abcd);
		//	if (screeninfo.iWidth = displayCenterX*2)
		//	{
		//		DrawHudStringLimit(200, 400,20,128,128,128,"1111111111111111");
		//	}

	}


	///////////////////////////////////////
/*	if(cvar.newhud){

		static int hud_x = 100;
		static int hud_y = (screeninfo.iHeight-30);//20
		static int x = (displayCenterX+1);
		static int y = (screeninfo.iHeight-30);
		static int w = 4;
		static int h = 12;
//		if (!cvar.language){
			DrawHudStringLimit(hud_x-45, hud_y,60, 255,255,255,"Health: %d", playerItems.health);
			DrawHudStringLimit(hud_x+100-57, hud_y,60, 255,255,255,"Armor: %d", playerItems.armor);
			DrawHudStringLimit(hud_x+200-60, hud_y,60, 255,255,255,"Money: %d", playerItems.money);
		}/*
		else
		{
			DrawHudStringLimit(hud_x-50, hud_y,60, 255,255,255,"ÁîüÂëΩÂÄºÔ: %d", playerItems.health);
			DrawHudStringLimit(hud_x+100-62, hud_y,60, 255,255,255,"Èò≤ÂºπË°£Ô: %d", playerItems.armor);
			DrawHudStringLimit(hud_x+200-73, hud_y,60, 255,255,255,"ÈáëÈí±: %d", playerItems.money);
	}*/

	/////////////////////////////////////////////////////////
	// player esp
	for (int ax=0;ax<MAX_VPLAYERS;ax++) if (vPlayers[ax].isUpdated() && vPlayers[ax].isAlive()) { drawPlayerEsp(ax); }	

	// Search for Target !
	Aimbot.FindTarget();

	if(!cvar.rush && cvar.avdraw) Aimbot.Draw();
		
	espInfoCount = 0; // reset esp info

	for(int i=0;i<MAX_VPLAYERS;i++) 
	{ 		
		vPlayers[i].updateClear();
	}	

	PlayerInfo& r = vPlayers[me.entindex];
	// == handle autoshoot
	gAttackHandling();
}
void func_setburst()
{
	if(!strcmp(cmd.argC(1),"copy"))
	{ 
		bool status = burstInfo.copy(cmd.argC(2),cmd.argC(3)); 
		if(!status) { Con_Echo( "&rcopy failed.\n"); }
		return;
	}
	char* weaponString = cmd.argC(1);
	int weapon = burstInfo.getWeaponID( weaponString );
	if( weapon==0 ){ Con_Echo( "&w%s&a no such weapon.\n",weaponString); return; }
	if( !*cmd.argC(2) ) // just weapon given, then enlist
	{    
		Con_Echo( "Listing for weapon: &w%s\n", weaponString);
		for(int i=0;i<5;i++)
		{
			char* desc = burstInfo.description(weapon,i);
			Con_Echo( " &w%d&a: %s",i+1,desc);
		}
		return;
	}
	int index=0, range=0, flags=0;
	float ftime=1,dtime=0,sdelay=0.05f;
	index  = cmd.argI(2)-1;
	range  = cmd.argI(3);
	ftime  = cmd.argF(4);
	dtime  = cmd.argF(5);
	sdelay = cmd.argF(6);
	flags  = cmd.argI(7);
	if(index>5||index<0) { index = 0; }
	BurstInfo::RangeInfo r;
	r.range  = range;
	r.ftime  = ftime;
	r.dtime  = dtime;
	r.sdelay = sdelay;
	r.flags  = flags;
	burstInfo.set( weapon,&r, index );
}
//==============================================================================
void PreV_CalcRefdef(struct ref_params_s *pparams)
{
	VectorCopy(pparams->punchangle,me.punchangle);	
	if (cvar.recoil) for (int i=0; i<3; i++) pparams->punchangle[i] = 0;  
	if( pparams->nextView == 0 )
	{
		Aimbot.FindTarget();
		if(Aimbot.iTarget!=-1 && cvar.autoburst)
		{
			// == update burst settings
			int dist = vPlayers[Aimbot.iTarget].distance/22;
			BurstInfo::RangeInfo* range = burstInfo.get(GetCurWeaponId(),dist );

			// update burst variables
			cvar.ftime  = range->ftime;
			cvar.dtime  = range->dtime;
			cvar.sdelay = range->sdelay;
			cvar.burstflags = range->flags;
		} 
		
		if ( me.alive )
		if ( Aimbot.iTarget!=-1 )
	    if ( cvar.aim )
	    if ( !IsCurWeaponNade() )
	    if ( !IsCurWeaponC4() )
	    if ( me.iClip )
		if ( pparams->nextView == 0 )
		{
		Aimbot.CalculateAimingView();
		VectorCopy(Aimbot.aim_viewangles, pparams->cl_viewangles);
		VectorCopy(Aimbot.aim_viewangles, pparams->viewangles);
		}
	}
}

//==============================================================================


//==============================================================================

float GetGunOffset(cl_entity_s* vm){
	if(!vm->model)
		return 0;

	char* gunname = vm->model->name;
	
	if(!gunname || !gunname[0])
		return 0;

	gunname += 9;

#define CHECKGUNOFFSET(a,b) if(!strcmp(a,gunname)) return b;
	CHECKGUNOFFSET("glock18.mdl", -4.55f);
	CHECKGUNOFFSET("usp.mdl", -4.64f);
	CHECKGUNOFFSET("p228.mdl", -4.65f);
	CHECKGUNOFFSET("deagle.mdl", -4.71f);
	CHECKGUNOFFSET("fiveseven.mdl", -4.84f);
	
	CHECKGUNOFFSET("m3.mdl", -5.03f);
	CHECKGUNOFFSET("xm1014.mdl", -5.82f);

	CHECKGUNOFFSET("mac10.mdl", -5.05f);
	CHECKGUNOFFSET("tmp.mdl", -6.47f);
	CHECKGUNOFFSET("mp5.mdl", -5.53f);
	CHECKGUNOFFSET("ump45.mdl", -6.07f);
	CHECKGUNOFFSET("p90.mdl", -4.32f);

	CHECKGUNOFFSET("scout.mdl", -5.14f);
	CHECKGUNOFFSET("awp.mdl", -6.02f);

	CHECKGUNOFFSET("famas.mdl", -4.84f);
	CHECKGUNOFFSET("aug.mdl", -6.22f);
	CHECKGUNOFFSET("m4a1.mdl", -6.74f);
	CHECKGUNOFFSET("sg550.mdl", -7.11f);
	CHECKGUNOFFSET("ak47.mdl", -6.79f);

	CHECKGUNOFFSET("g3sg1.mdl", -6.19f);
	CHECKGUNOFFSET("sg552.mdl", -5.27f);
	CHECKGUNOFFSET("galil.mdl", -4.78f);

	CHECKGUNOFFSET("m249.mdl", -5.13f);

	return 0;
}

//==============================================================================
/*float GetGunOffset2(cl_entity_s* vm){
	if(!vm->model)
		return 0;

	char* gunname = vm->model->name;
	
	if(!gunname || !gunname[0])
		return 0;

	gunname += 9;

#define CHECKGUNOFFSET(a,b) if(!strcmp(a,gunname)) return b;
	CHECKGUNOFFSET("glock18.mdl", -2.05f);
	CHECKGUNOFFSET("usp.mdl", -2.14f);
	CHECKGUNOFFSET("p228.mdl", -2.15f);
	CHECKGUNOFFSET("deagle.mdl", -2.21f);
	CHECKGUNOFFSET("fiveseven.mdl", -2.34f);

	CHECKGUNOFFSET("m3.mdl", -2.53f);
	CHECKGUNOFFSET("xm1014.mdl", -3.32f);

	CHECKGUNOFFSET("mac10.mdl", -2.55f);
	CHECKGUNOFFSET("tmp.mdl", -3.97f);
	CHECKGUNOFFSET("mp5.mdl", -3.03f);
	CHECKGUNOFFSET("ump45.mdl", -3.57f);
	CHECKGUNOFFSET("p90.mdl", -1.82f);

	CHECKGUNOFFSET("scout.mdl", -2.94f);
	CHECKGUNOFFSET("awp.mdl", -3.52f);

	CHECKGUNOFFSET("famas.mdl", -2.34f);
	CHECKGUNOFFSET("aug.mdl", -3.72f);
	CHECKGUNOFFSET("m4a1.mdl", -4.24f);
	CHECKGUNOFFSET("sg550.mdl", -4.91f);
	CHECKGUNOFFSET("ak47.mdl", -4.29f);

	CHECKGUNOFFSET("g3sg1.mdl", -3.99f);
	CHECKGUNOFFSET("sg552.mdl", -2.87f);
	CHECKGUNOFFSET("galil.mdl", -2.28f);

	CHECKGUNOFFSET("m249.mdl", -2.93f);

	return 0;
}*/

//==============================================================================
void QuakeGuns(){
cl_entity_s* vm = gEngfuncs.GetViewModel();

if(!vm)
return;

float gunoffsetr = GetGunOffset(vm);

if(gunoffsetr == 0)
return;

float* org = vm->origin;
float* ang = vm->angles;

VectorCopy(mainViewAngles, ang);

vec3_t forward, right, up;
gEngfuncs.pfnAngleVectors(mainViewAngles, forward, right, up);

org[0] += forward[0] + up[0] + right[0]*gunoffsetr;
org[1] += forward[1] + up[1] + right[1]*gunoffsetr;
org[2] += forward[2] + up[2] + right[2]*gunoffsetr;
}
void PostV_CalcRefdef(struct ref_params_s *pparams)
{
	if( pparams->nextView == 0 )
	{
		VectorCopy(pparams->viewangles,mainViewAngles);
		VectorCopy(pparams->vieworg,mainViewOrigin);	
	}
	if(cvar.quake_guns) 
{
    QuakeGuns(); 
}
	if( pparams->nextView == 0 )
	{
		Aimbot.aim_viewangles[0] -= pparams->punchangle[0];
		Aimbot.aim_viewangles[1] -= pparams->punchangle[1];
		Aimbot.aim_viewangles[2] = 0.0f;
		Aimbot.CalculateAimingView();
		VectorCopy(pparams->viewangles,mainViewAngles);
		VectorCopy(pparams->vieworg,mainViewOrigin);
		if(chaseofs)
		{
			vec3_t vecEnd, up, right, forward, EntViewOrg;
			gEngfuncs.pfnAngleVectors (mainViewAngles, forward, right, up);
			mainViewOrigin[0] += chaseofs*forward[0];
			mainViewOrigin[1] += chaseofs*forward[1];
			mainViewOrigin[2] -= chaseofs*forward[2];
		}
	}	
}
inline void drawawpLine(float*from, float*to, ColorEntry*color)
{
	int beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr");
	gEngfuncs.pEfxAPI->R_BeamPoints(from,to,beamindex,0.001f,0.9f,0,   32,2,0,10,0,0,255);
}
void opengl_ClearBackground();
//==============================================================================
void CL_CreateMove(float frametime, struct usercmd_s *cmd, int active)
{
    // copy the original cmd structure for reference after it has been changed by the aimbot/other hacks 
    memcpy(&g_Originalcmd, cmd, sizeof(usercmd_t)); 
    if(cvar.way_enable) {waypoint.FindTarget(cmd);}
	relistplayermenu();// µ ±À¢–¬ID¡–±Ìidhook
//	autoroute.CreateMove(usercmd);
	//∂≥Ω·
	if (cvar.wall >= 0)  { opengl_ClearBackground(); }
    if(isInFrozenTime)
		{
			if(cvar.way_on)
			{
				cvar.speed = 0;
			}
		}
	// Speed	
	float spd = 0.0;
	if (cvar.rush)		spd = cvar.speed;
	if (cvar.speed) *globalTime += frametime * spd/5;
	//
	// aim
	Aimbot.active = false;
	float distance = 0;
	for (int ax=0;ax<MAX_VPLAYERS;ax++) if (vPlayers[ax].isUpdated()) 
	{ distance = vPlayers[ax].distance/22.0f; }	
	if ( me.alive )
	if ( cvar.aim )
	if ( !IsCurWeaponNade() )
	if ( !IsCurWeaponC4() )
	if ( me.iClip )
	if ( distance <0)
		return;
	else if ( Aimbot.HasTarget() )
	{
		// auto aim	
		Aimbot.CalculateAimingView();
		VectorCopy(Aimbot.aim_viewangles, cmd->viewangles);	
		Aimbot.active = true;
	}
	// auto shoot
	if (cvar.shoot && me.iClip && Aimbot.active && bAttacking) 
	{
		if (cvar.burstflags & BurstInfo::BURSTFLAG_ZOOM && !me.inZoomMode)
		  {
		  	cmd->buttons |= IN_ATTACK2;
		    if (cmd->buttons&IN_ATTACK2 && cvar.aspeed && !IsCurWeaponNade() && !IsCurWeaponC4()) 
		{
		    spd = cvar.aspeed;
		    *globalTime += frametime * spd/8;
		  }
			}
          else
		  {
			  if(IsCurWeaponKnife())
			  {
			    if(vPlayers[Aimbot.iTarget].distance<=cvar.knifeattackdist)  //≥÷µ∂…±»À
				{
			    if(cvar.knifeattackmode)
				cmd->buttons |= IN_ATTACK2;
		        if (cmd->buttons&IN_ATTACK2 && cvar.aspeed && !IsCurWeaponNade() && !IsCurWeaponC4()) 
				{
		        spd = cvar.aspeed;
		        *globalTime += frametime * spd/8;
				}
			    else
			    cmd->buttons |= IN_ATTACK;
		        if (cmd->buttons&IN_ATTACK && cvar.aspeed && !IsCurWeaponNade() && !IsCurWeaponC4()) 
				{
		        spd = cvar.aspeed;
		        *globalTime += frametime * spd/8;
				}
				}
			  }
		else
			cmd->buttons |= IN_ATTACK;
		    if (cmd->buttons&IN_ATTACK && cvar.aspeed && !IsCurWeaponNade() && !IsCurWeaponC4()) 
			{
		    spd = cvar.aspeed;
		    *globalTime += frametime * spd/8;
		  }
		}
	}

	if (cmd->buttons&IN_ATTACK)
	{	
		if(IsCurWeaponSec() && cvar.reload)
		{
			static bool flipFlop = true;
			if(flipFlop){ cmd->buttons |= IN_ATTACK;    }
			else        { cmd->buttons &= (~IN_ATTACK); }

			flipFlop = !flipFlop;
		}
	}
	if(cvar.way_on && cvar.autojump) autodirection.adjustJump(cmd);
	switch(cvar.way_on){

			case 1:
			{
				if (cvar.auto_br && !cvar.stop_auto_bq && cvar.auto_bq>0){

					// runway_mode 0 : se queda quieto en el lugar y no puede moverse hasta no tener target del curgroup
					// runway_mode 1 : busca otro posible target, si no lo encuentra queda libre..
					// runway_mode 2 : queda libre directamente

					if(waypoint.getFree())
					{
						if(cvar.way_move)
						{

						if(Aimbot.iTarget==-1){
							if(autodirection.adjustMyAngles(cmd->viewangles))
								gEngfuncs.SetViewAngles(me.viewAngles);
						}
						}
					}

					//else if(waypoint.isFreeInGroup())
					//else if(waypoint.isRunningToPoint())
					//else if(waypoint.getRunning())
					else
					{
						if(cvar.auto_br==1)
						{
						if (IsCurWeaponKnife() /*&& !IsFrozen()*/) autodirection.adjustMyDirection2(cmd);
						}
						else if(cvar.auto_br==2)
						{
						if (IsCurWeaponKnife() /*&& !IsFrozen()*/) autodirection.adjustMyDirection1(cmd);
						}
					}
				}
			}
			break;
			case 0:
			{
				if (cvar.auto_br && !cvar.stop_auto_bq && cvar.auto_bq>0){
					//for( unsigned int ax=0;ax<vPlayers.size();ax++ )
						//if (vPlayers[ax].getPVS())
						if(me.alive)
						{
							if (IsCurWeaponKnife() /*&& !IsFrozen()*/)
							{
								autodirection.adjustMyDirection1(cmd);
							}
						}
				}
			}
			break;
			default:
			break;
		}
    if (me.alive)
    {
        if(cvar.way_mode == 0)
        {
        	if(IsCurWeaponKnife())
        	{
        		waypoint.cur_way_mode=1;
        	}
        	else
           	{
	        	waypoint.cur_way_mode=0;
       		}
	    }
    	if(cvar.way_mode == 1)
    	{
    	waypoint.cur_way_mode=1;	
    	}
    }

	//nospread
	if (cvar.nospread == 1 && me.alive && Aimbot.iTarget!=-1 && me.iClip)
	{
		float offset[3];
		gNoSpread.GetSpreadOffset(me.spread.random_seed, 1, cmd->viewangles, me.pmVelocity, offset);
		cmd->viewangles[0] += offset[0];
		cmd->viewangles[1] += offset[1];
		cmd->viewangles[2] += offset[2];
	}
	// anti-recoil
	if( cvar.recoil )
	{
		 cmd->viewangles[0] -= (me.punchangle[0] * cvar.recoil);
		 cmd->viewangles[1] -= (me.punchangle[1] * cvar.recoil);
	}
	if (cvar.jump && (cmd->buttons & IN_JUMP) && !(me.pmFlags & FL_ONGROUND)) cmd->buttons &= ~IN_JUMP;
	if (cvar.spin) { FixupAngleDifference(cmd); } //spin
	if (cvar.explodingbullets && me.alive && Aimbot.iTarget!=-1 && me.iClip)
		{
			int seqinfo=getSeqInfo(me.ent->index);
			if(seqinfo & SEQUENCE_SHOOT)
			if(cmd->buttons&IN_ATTACK)
			{
				pmtrace_t tr;
				vec3_t vTrace, vAngles, vForward;
				gEngfuncs.GetViewAngles(vAngles);
				gEngfuncs.pfnAngleVectors(vAngles, vForward, NULL, NULL );
				vTrace = vAngles + vForward * 8092;
				gEngfuncs.pEventAPI->EV_SetTraceHull ( 2 );
				gEngfuncs.pEventAPI->EV_PlayerTrace ( me.pmEyePos, vTrace, 0, -1, &tr );
				gEngfuncs.pEfxAPI->R_Explosion( tr.endpos, gEngfuncs.pEventAPI->EV_FindModelIndex (/*sprites/zerogxplode.spr*/XorStr<0xE7,24,0x34A2C4D9>("\x94\x98\x9B\x83\x9F\x89\x9E\xC1\x95\x95\x83\x9D\x94\x8C\x85\x9A\x98\x9C\x9C\xD4\x88\x8C\x8F"+0x34A2C4D9).s), 2.5, 15, TE_EXPLFLAG_NONE );
				gEngfuncs.pEfxAPI->R_ParticleExplosion2( tr.endpos, 111, 8 );
			}
		}
	}
// =============================================================================
void HUD_PlayerMove(struct playermove_s *a, int b)
{
	me.pmFlags = a->flags;
	me.pmMoveType = a->movetype;
    VectorCopy(a->velocity,me.pmVelocity);
	
	// copy origin+angles
	gEngfuncs.pEventAPI->EV_LocalPlayerViewheight(me.pmEyePos);	
	if(me.alive) me.pmEyePos[2] += cvar.height_correction;

    me.pmEyePos[0] += a->origin[0];
	me.pmEyePos[1] += a->origin[1];
	me.pmEyePos[2] += a->origin[2];

	VectorCopy(a->angles,me.viewAngles);	
}
//==============================================================================
void HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *usercmd, int runfuncs, double time, unsigned int random_seed)
{
	gNoSpread.HUD_PostRunCmd(from,to,usercmd,runfuncs,time,random_seed);

	if (cvar.vec_switch && me.alive) 
	{
		char signal[256];
		sprintf(signal, "vec_%s",gGetWeaponName(me.ent->curstate.weaponmodel));
		cmd.exec(signal);
	}
	if( cvar.rcl_switch  && me.alive )
	{
		char signal[256];
		sprintf( signal , "rcl_%s",gGetWeaponName(me.ent->curstate.weaponmodel) );
		cmd.exec( signal );
	}

/*	if (cvar.bone_switch && me.alive) 
	{
		char signal[128];
		sprintf(signal, "bone_%s",gGetWeaponName(me.ent->curstate.weaponmodel));
		cmd.exec(signal);
	}

	if (cvar.hbox_switch && me.alive) 
	{
		char signal[128];
		sprintf(signal, "hbox_%s",gGetWeaponName(me.ent->curstate.weaponmodel));
		cmd.exec(signal);
	}*/
}
//==============================================================================
void HUD_TempEntUpdate(double *pframetime, double *pclient_time, double *pcl_gravity, TEMPENTITY*** pppTempEntFree, TEMPENTITY*** pppTempEntActive, int( **pCallback_AddVisibleEntity )( cl_entity_t *pEntity ), void( **pCallback_TempEntPlaySound )( TEMPENTITY *pTemp, float damp ) ) 
{
	
}
//==============================================================================
static void change_name(char *str)
{
	if(strstr(str,"  ")!=NULL)
	{
		strcpy(strstr(str,"  "),strstr(str,"  ")+1);
		return;
	}
	if(str[0]==str[strlen(str)-1])
	{
		char buf[256];
		char sbuf[2]="A";
		sbuf[0]=str[0];
		strcpy(buf,sbuf);
		strcat(buf,str);
		strcpy(str,buf);
		return;
	}
	if(strlen(str)>16)
	{
		str[strlen(str)-1]=0;
		return;
	}
	if(strstr(str,"0")!=NULL)
	{
		*strstr(str,"0")='O';
		return;
	}
	if(strstr(str,"O")!=NULL)
	{
		*strstr(str,"O")='0';
		return;
	}
	if(strstr(str,".")!=NULL)
	{
		*strstr(str,".")=',';
		return;
	}
	if(strstr(str,",")!=NULL)
	{
		*strstr(str,",")='.';
		return;
	}
	if(strstr(str,"|")!=NULL)
	{
		*strstr(str,"|")='l';
		return;
	}
	if(strstr(str,"l")!=NULL)
	{
		*strstr(str,"l")='|';
		return;
	}
	if(strstr(str," ")!=NULL)
	{
		char buf[256];
		strcpy(buf,str);
		strcpy(strstr(buf," ")+1,strstr(str," "));
		strcpy(str,buf);
		return;
	}
	if(strstr(str,"-")!=NULL)
	{
		*strstr(str,"-")='=';
		return;
	}
	if(strstr(str,"=")!=NULL)
	{
		*strstr(str,"=")='-';
		return;
	}
	if(strstr(str,"o")!=NULL)
	{
		*strstr(str,"o")='0';
		return;
	}
	if(strstr(str,"0")!=NULL)
	{
		*strstr(str,"0")='o';
		return;
	}
	if(str[strlen(str)-1]==str[strlen(str)-2])
	{
		str[strlen(str)-1]=0;
		return;
	}
	strcat(str,".");
}
bool __fastcall isEnemy(int ax)
{
	return (me.team != vPlayers[ax].team);
}
/////////////////////////////////////////

int GetTargetWeaponID(int ax)
{
	if(ax==-1)
		return -1;
	else
		return burstInfo.getWeaponID(gGetWeaponName(vPlayers[ax].getEnt()->curstate.weaponmodel));
}
bool TargeHaveDangeWeapon(int ax)
{
	int weaponid=GetTargetWeaponID(ax);
	if((weaponid==WEAPON_SCOUT||weaponid==WEAPON_AWP||weaponid==WEAPON_G3SG1||weaponid==WEAPON_SG550))
		return true;
	else if(weaponid==WEAPON_AWP)
		return true;
	else
		return false;
}
//==============================================================================
static void playerRenderOptions(cl_entity_s* ent)
{
	ColorEntry* color = PlayerColor(ent->index);
	if (isEnemy(ent->index) && TargeHaveDangeWeapon(ent->index))
	{
		if( (!cvar.rush && cvar.awpline == 1 &&
			ent->curstate.gaitsequence != 3 &&
			ent->curstate.gaitsequence != 4 &&
			ent->curstate.gaitsequence != 5 &&
			ent->curstate.gaitsequence != 6 ) || !cvar.rush &&
			cvar.awpline == 2
			)
		{
			vec3_t vecEnd, up, right, forward, EntViewOrg;
			VectorCopy(ent->origin,EntViewOrg);
			
			if (ent->curstate.usehull == 0)
				EntViewOrg[2] += 17;
			else if (ent->curstate.usehull == 1)
				EntViewOrg[2] += 12;
			
			gEngfuncs.pfnAngleVectors (ent->angles, forward, right, up);
			forward[2] = -forward[2];
			vec3_t vecBegin(EntViewOrg);
			
			vecBegin[0]+=forward[0]*22;
			vecBegin[1]+=forward[1]*22;
			vecBegin[2]+=forward[2]*22;
			
			vecEnd = vecBegin + forward * 99999;
			drawawpLine(vecBegin,vecEnd,color);
		}
	}
}
void DrawTrace(cl_entity_s *pEnt,ColorEntry *pClr,float life,float width)
{
	if( pEnt )
	{
		int beamindex;
		beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex(/*sprites/laserbeam.spr*/XorStr<0xAA,22,0x5754D1D0>("\xD9\xDB\xDE\xC4\xDA\xCA\xC3\x9E\xDE\xD2\xC7\xD0\xC4\xD5\xDD\xD8\xD7\x95\xCF\xCD\xCC"+0x5754D1D0).s);
		gEngfuncs.pEfxAPI->R_BeamFollow(pEnt->index, beamindex, life, width, pClr->r/255, pClr->g/255, pClr->b/255, 255);
	}
}
void DrawLine2(float *from, float *to,int r,int g,int b,float d)
{
	int beamindex = 0;
	if(!beamindex)
		beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex(/*sprites/laserbeam.spr*/XorStr<0x11,22,0x4C9F5102>("\x62\x62\x61\x7D\x61\x73\x64\x37\x75\x7B\x68\x79\x6F\x7C\x7A\x41\x4C\x0C\x50\x54\x57"+0x4C9F5102).s);
	gEngfuncs.pEfxAPI->R_BeamPoints(from,to,beamindex,d,1.2f,0,32,2,0,0,r/255,g/255,b/255);
}
void HUD_AddEntity(int type, struct cl_entity_s *ent, const char *modelname)
{
	//AddEntResult=1;
	char* pos;
	if(0){}
	else if( ent->player)
	{ 
		AddEntityPlayer(ent); 
		for (int ax=0;ax<MAX_VPLAYERS;ax++)
		{
			if(me.alive && cvar.way_on == 1 && cvar.noall)
			{
				ent->curstate.rendermode=kRenderTransAlpha;
			}
	}
	} 
	else if( strlen(modelname)>5 )
	{
		if(0){}
		else if( (pos=strstr(modelname, "w_"       )) !=0 ) { AddEntityWeapon(ent,pos+2,false); }
		else if( (pos=strstr(modelname, "hostage"  )) !=0 ) { AddEntityHostage(ent); }
		else if( (pos=strstr(modelname, "scientist")) !=0 ) { AddEntityHostage(ent); }
		else if( (pos=strstr(modelname, "sentry"   )) !=0 ) { AddEntityWeapon(ent,pos+2,true); }
	}
	else if (type==ET_NORMAL && cvar.wall == 1 && ent->curstate.solid==SOLID_BSP)
	{
			ent->curstate.rendermode=2;
			ent->curstate.renderamt=80;
	}
	playerRenderOptions(ent);
	cl_entity_s *myEnt = gEngfuncs.GetViewModel();
	for (int ax=0;ax<MAX_VPLAYERS;ax++)
	if (!cvar.rush && vPlayers[ax].isUpdated() && vPlayers[ax].isAlive() && ent && cvar.glowshells)
			{	
				if (ax == Aimbot.iTarget)
				{
					ent->curstate.renderamt = 5;
					ent->curstate.rendercolor.r = 33;
					ent->curstate.rendercolor.g = 169;
					ent->curstate.rendercolor.b = 49;
					ent->curstate.renderfx = kRenderFxGlowShell;
				}
				else if (vPlayers[ax].team == 1)
				{ 
					ent->curstate.renderamt = 5;
					ent->curstate.rendercolor.r = 255;
					ent->curstate.rendercolor.g = 40;
					ent->curstate.rendercolor.b = 40;
					ent->curstate.renderfx = kRenderFxGlowShell;
				}
				else
				{ 
					ent->curstate.renderamt = 5;
					ent->curstate.rendercolor.r = 60;
					ent->curstate.rendercolor.g = 100;
					ent->curstate.rendercolor.b = 255;
					ent->curstate.renderfx = kRenderFxGlowShell;
				}
			}
	if(cvar.warcrafthuman)
	{
		{
			if((strstr(modelname, "leet")||strstr(modelname, "guerilla")||strstr(modelname, "terror")||strstr(modelname, "arctic")||strstr(modelname, "gsg9")||strstr(modelname, "gign")||strstr(modelname, "urban")||strstr(modelname, "sas")))
			{
				ent->curstate.rendermode = kRenderNormal;
				ent->curstate.renderamt = 0;
			}
		}
	}
	if(!cvar.rush && cvar.glowshells2)
			{
				if(ent->player)
				{
					color24 color;
					ColorEntry *clr = PlayerColor(ent->index);
					color.r = clr->r;
					color.g = clr->g;
					color.b = clr->b;
					ent->curstate.renderamt = 1;
					ent->curstate.renderfx |= kRenderFxGlowShell;
					ent->curstate.rendercolor = color;
				}
			}
	if(!cvar.rush && cvar.ghostshells)
			{
				ent->curstate.rendermode=kRenderTransAlpha;
				cvar.glowshells = 1;
			}
	if(!cvar.rush && cvar.barrel)
			{
				ColorEntry *pClr = PlayerColor(ent->index);
				vec3_t vecBegin,vecEnd,up,right,forward;
				VectorCopy(ent->origin,vecBegin);
				if(ent->curstate.usehull)
					vecBegin[2]+=12;
				else
					vecBegin[2]+=17;
				gEngfuncs.pfnAngleVectors(ent->angles,forward,right,up);
				forward[2]=-forward[2];
				vecBegin[0]+=forward[0]*10;
				vecBegin[1]+=forward[1]*10;
				vecBegin[2]+=forward[2]*10;
				vecEnd=vecBegin+forward*999;
				DrawLine2(vecBegin,vecEnd,pClr->r,pClr->g,pClr->b,0.001f);
			}
	if (!cvar.rush && cvar.barrel)
		{
		    int r =255;
	        int g =255;
	        int b =0;
			int beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr");
			vec3_t vecEnd, up, right, forward, EntViewOrg;
			VectorCopy(ent->origin,EntViewOrg);
			
			if (ent->curstate.usehull == 0)
				EntViewOrg[2] += 17;
			else if (ent->curstate.usehull == 1)
				EntViewOrg[2] += 12;
			
			gEngfuncs.pfnAngleVectors (ent->angles, forward, right, up);
			forward[2] = -forward[2];
			vec3_t vecBegin(EntViewOrg);
			
			vecEnd = vecBegin + forward * cvar.barrel;
			gEngfuncs.pEfxAPI->R_BeamPoints(vecBegin,vecEnd,beamindex,0.001f,0.9f,0,32,2,0,0,r,g,b);
		}
	if(!cvar.rush && cvar.grenadeshell)
			{
		        ColorEntry* color = PlayerColor(oEngStudio.GetCurrentEntity()->index);
				float sizes = 0;
				if(cvar.grenadeshell == 1) { sizes = 100; }
				else if(cvar.grenadeshell == 2) { sizes = 200; }
				else if(cvar.grenadeshell == 3) { sizes = 300; }
				else if(cvar.grenadeshell == 4) { sizes = 400; }
				else if(cvar.grenadeshell == 5) { sizes = 500; }
				else if(cvar.grenadeshell == 6) { sizes = 600; }
				else if(cvar.grenadeshell == 7) { sizes = 700; }
				else if(cvar.grenadeshell == 8) { sizes = 800; }
				else if(cvar.grenadeshell == 9) { sizes = 900; }
				ent->curstate.renderamt = sizes;
				ent->curstate.rendercolor.r = color->r;
				ent->curstate.rendercolor.b = color->b;
				ent->curstate.rendercolor.g = color->g;
				ent->curstate.renderfx = kRenderFxGlowShell;
			}
	if(!cvar.rush && cvar.grenadetrace)
				if(ent->index >= MAX_VPLAYERS)
				{
					cl_entity_s * pMe = gEngfuncs.GetLocalPlayer();
					//cl_entity_s *pEnt = gEngfuncs.GetEntityByIndex(i);
					if(ent && (ent->curstate.messagenum+10 > pMe->curstate.messagenum) && ent->model && ent->model->name && !ent->player)
					{
						if(strstr(ent->model->name, /*hegrenade*/XorStr<0xB4,10,0xFBF7F52E>("\xDC\xD0\xD1\xC5\xDD\xD7\xDB\xDF\xD9"+0xFBF7F52E).s))
							DrawTrace(ent,colorList.get(2),1,2);
						else if(strstr(ent->model->name, /*smokegrenade*/XorStr<0xFE,13,0x0C5E3497>("\x8D\x92\x6F\x6A\x67\x64\x76\x60\x68\x66\x6C\x6C"+0x0C5E3497).s))
							DrawTrace(ent,colorList.get(4),1,2);
						else if(strstr(ent->model->name, /*flashbang*/XorStr<0x0D,10,0xFFF6EFFF>("\x6B\x62\x6E\x63\x79\x70\x72\x7A\x72"+0xFFF6EFFF).s))
							DrawTrace(ent,colorList.get(22),1,2);
					}
				}
	if(!cvar.rush && cvar.localshells)
					{
						ColorEntry* color = PlayerColor(oEngStudio.GetCurrentEntity()->index);
						myEnt->curstate.renderamt = 1;
						myEnt->curstate.rendercolor.r = color->r;
						myEnt->curstate.rendercolor.b = color->b;
						myEnt->curstate.rendercolor.g = color->g;
						myEnt->curstate.renderfx = kRenderFxGlowShell;
					}
					else
					{
						myEnt->curstate.rendermode = kRenderNormal;
						myEnt->curstate.renderfx = kRenderFxNone;
					}
    if(!cvar.rush && cvar.colorblood)
		{
			if( strstr(ent->model->name, /*blood.spr*/XorStr<0x17,10,0xEDF506A9>("\x75\x74\x76\x75\x7F\x32\x6E\x6E\x6D"+0xEDF506A9).s))
			{
				ent->curstate.renderamt = 1;
				switch((int)ent->curstate.frame)
				{
					case 0:
						ent->curstate.rendercolor.r = 255;
						ent->curstate.rendercolor.g = 0;
						ent->curstate.rendercolor.b = 0;
						break;
					case 1:
						ent->curstate.rendercolor.r = 0;
						ent->curstate.rendercolor.g = 255;
						ent->curstate.rendercolor.b = 0;
						break;
					case 2:
						ent->curstate.rendercolor.r = 0;
						ent->curstate.rendercolor.g = 0;
						ent->curstate.rendercolor.b = 255;
						break;
					case 3:
						ent->curstate.rendercolor.r = 255;
						ent->curstate.rendercolor.g = 255;
						ent->curstate.rendercolor.b = 0;
						break;
					case 4:
						ent->curstate.rendercolor.r = 0;
						ent->curstate.rendercolor.g = 255;
						ent->curstate.rendercolor.b = 255;
						break;
					case 5:
						ent->curstate.rendercolor.r = 255;
						ent->curstate.rendercolor.g = 0;
						ent->curstate.rendercolor.b = 255;
						break;
					case 6:
						ent->curstate.rendercolor.r = 20;
						ent->curstate.rendercolor.g = 20;
						ent->curstate.rendercolor.b = 255;
						break;
					case 7:
						ent->curstate.rendercolor.r = 20;
						ent->curstate.rendercolor.g = 255;
						ent->curstate.rendercolor.b = 20;
						break;
				}
			}
		}
	if(!cvar.rush && cvar.playertrace && (ent->player || (ent->index>0 && ent->index<33)))
				if(vPlayers[ent->index].isAlive())
					DrawTrace(ent,PlayerColor(ent->index),2,2);
			/*else if(!cvar.playertrace && (ent->player || (ent->index>0 && ent->index<33)))
				gEngfuncs.pEfxAPI->R_BeamKill(ent->index);*/
	if(cvar.way_enable){waypoint.draw_line();} 
//	if(cvar.way_switch && cvar.way_draw == 2){autoroute.DrawRouteLine();} //max
	static char names[32][256];
	static unsigned long rand=0;
	static double last_timer=0;
	int names_count=0;
	int n;
	rand++;
	if(cvar.autoname && me.alive)
	{
		if(ClientTime::current - last_timer<1.0)return;

		for (int ax=0;ax<MAX_VPLAYERS;ax++)
		{
			char name[256];
			memcpy(name,vPlayers[ax].entinfo.name,128);
		    name[128]=0;
		    if(name[0]=='\\')continue;
			//if(strlen(name)<8)continue;
			strcpy(names[names_count++],name);
		}

		if(names_count>0)
		{
			char cmdstr[256];
		    char name[256];
		    char buf[2]="A";
		    n=rand%names_count;//UTIL_SharedRandomLong(me.spread.random_seed,0,names_count-1);
		    strcpy(cmdstr,"#name \"");
		    strcpy(name,names[n]);
		    change_name(name);
		    strcat(cmdstr,name);
		    strcat(cmdstr,"\"");
		    cmd.exec(cmdstr);
		}
		last_timer = ClientTime::current;
	}
}		
//==============================================================================
void HUD_Key_Event(int eventcode, int keynum, const char *pszCurrentBinding)
{
	KeyEventResult=1;

	//Handle Menu on INS
	if (keynum == cvar.menukey && eventcode) {gMenu.Active = !gMenu.Active; KeyEventResult=0;return;}
	if( gMenu.Active && eventcode )
	{
		KeyEventResult = gMenu.KeyEvent(keynum);
		if (!KeyEventResult) return;		
	}

	//Handle Console on DEL
	if (keynum == cvar.consolekey && eventcode) {gConsole.active = !gConsole.active;return;}
	//Handle Speed
	if (cvar.rush)
		if (keynum == (241)) {KeyEventResult = DoSpeed(eventcode);}
//		if (keynum == (242)) {KeyEventResult = DoSpeed(eventcode);}

	//-----------------------------------------------------------------------------//
	const char PROC_NAME[50] = "HUD_Key_Event_Hook\0";
	int retval = 1;
	if (keynum >= 239 && keynum <= 244)
	{
		switch (keynum) {
			case 239:
				keyBindManager.notifyMouseEvent("mwheeldown", (eventcode == 1));
				break;
			case 240:
				keyBindManager.notifyMouseEvent("mwheelup", (eventcode ==1));
				break;
			case 241:
				keyBindManager.notifyMouseEvent("mouse1", (eventcode == 1));
				break;
			case 242:
				keyBindManager.notifyMouseEvent("mouse2", (eventcode == 1));
				break;
			case 243:
				keyBindManager.notifyMouseEvent("mouse3", (eventcode == 1));
				break;
			case 244:
				keyBindManager.notifyMouseEvent("mouse4", (eventcode == 1));
				break;
			case 245:
				keyBindManager.notifyMouseEvent("mouse5", (eventcode == 1));
				break;
		}
	}
	else
		// don't pass event code straight in, for some reason it b0rks
		if (eventcode)
			keyBindManager.notifyKeyEvent(keynum, true, false);
		else
			keyBindManager.notifyKeyEvent(keynum, false, false);

	retval = (keyBindManager.keyBlocked(keynum, (eventcode) ? true : false)) ? 0 : 1;

		if( gMenuActive() && (keynum >= 239 && keynum <= 244))
		retval = 0;

	KeyEventResult = retval;
	if( gPlayerActive() && (keynum >= 239 && keynum <= 245))
		retval = 0;

	KeyEventResult = retval;
}
//==============================================================================
inline float GetPseudoDistance(const float* const pos1, const float* const pos2)
{
	return
		fabs(pos1[0]-pos2[0])+
		fabs(pos1[1]-pos2[1])+
		fabs(pos1[2]-pos2[2]);
}
bool passesSoundFilter(const float* const origin,const int bx)
{
	if(cvar.soundfilter==4)
		return true;
	if( GetPseudoDistance(origin,me.pmEyePos)<cvar.soundtol){ return false; }
	int ax;
	if(cvar.soundfilter == 1)
	{
		for (ax=0;ax<vPlayers.size();ax++)
		{
	//		if( vPlayers[ax].isUpdated()  ) 
			{
				if( GetPseudoDistance(vPlayers[ax].getEnt()->origin, origin)<cvar.soundtol )
				{
					return false;
				}
			}
		}
	}
	else if(cvar.soundfilter == 2)
	{
		return (vPlayers[bx].team!=me.team);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void strcpy_x(char* dest, const char* pos)
{
	do{ 
		while( *pos>='0' && *pos<='9' )  ++pos; // ignore numbers
		
		if(*pos=='.')				{*dest=0;break; } // cut off ".wav"
		*dest=*pos;
		++dest;
		++pos; 
	}
	while(*pos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void strcpy_x2(char* dest, const char* pos)
{
	do{ 
		if(*pos=='.'||*pos=='-'||*pos=='_') {*dest=0;break; } // cut off ".wav"
		*dest=*pos;
		++dest;
		++pos; 
	}
	while(*pos);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void PreS_DynamicSound(const DWORD a,const DWORD b,const DWORD c,const char*const sample, const float*const origin,DWORD f,const DWORD g,const DWORD h)
{
	if(cvar.soundfilter == -1)
		return;
	BOUND_VALUE(soundMarkerCurrentIndex,0,64);
	SoundMarker& ref = vecSoundMarker[soundMarkerCurrentIndex];

	string theMessage;
/*
	if(b>0 && b<=36)
	{
		theMessage = vPlayers[b].entinfo.name;
		ref.team = vPlayers[b].team;

	}
*/
	if(b>0 && b<=36)
	{
		vPlayers[b].entindex = b;
		vPlayers[b].updateEntInfo();
		theMessage = vPlayers[b].getName();
		ref.team = vPlayers[b].team;
	}
	else return;

	// setup color and description of sound marker depending on it's type
	// or drop it if it doesn't pass the filter
	if(sample[0]=='p' && sample[3]=='y' && sample[6]=='/')  // "player/pl_step1.wav"
	{
		if(!passesSoundFilter(origin,b)) return;
		const char* pos = sample+7;
		if(pos[0]=='p' && pos[2]=='_') // pl_xx
		{
			if(pos[3]=='s' && pos[4]=='h' && pos[5]=='e' && pos[6]=='l' )  return; // pl_shell#      
	
			pos+=3;

			ref.color = colorList.get(1+ref.team);

			ref.priority = 0;
		}
		strcpy_x(ref.description,pos);
	}
	else
	if( sample[0]=='w' && sample[1]=='e' && sample[7]=='/' ) // weapons/ric1.wav
	{
		const char* pos = sample+8;
		if(   (pos[0]=='r' && pos[1]=='i')    // ricxxx
			||(pos[0]=='b' && pos[1]=='u') )  // bullet-hit...
		{
			return;
		}
		if(!passesSoundFilter(origin,b)) return;

		ref.priority = 1;
		ref.color    = colorList.get(1+ref.team);   // snd_weapon
		strcpy_x2(ref.description,pos);
		for(char *pos1=ref.description;*pos1;++pos1){ *pos1 = toupper(*pos1); }// uppercase
	}
	else 
	{
		return;
	}

	if(!cvar.soundfilter) { return; }

//	if (cvar.soundesp) {vPlayers[b].Quick_Update( origin, sample );return;}

	// activate sound marker
	VectorCopy( origin, ref.origin );
	ref.timer.countdown( cvar.soundtime );
	
	// advance to next sound marker
	++soundMarkerCurrentIndex;
	if(soundMarkerCurrentIndex>=12){ soundMarkerCurrentIndex=0;}
}
//==============================================================================
void gSetHudMessage2(const char* message)
{
	strcpy(gHudMessage2,message);
	gHudTimer2.countdown(4);
}
//==============================================================================

//==============================================================================
void CB_KeyManager_ConType(char ch)
{
	gConsole.key(ch);
}
//==============================================================================

//==============================================================================
void CB_KeyManager_Exec(const string& commandlist)
{
	cmd.exec( const_cast<char*>( commandlist.c_str() ) );
}
void ogc_exec(const char* filename)
{
	string file = getHackDirFile(filename);
	file += ".cfg";
	cmd.execFile( file.c_str() );
}
//Func Commands ================================================================
void func_quit()
{
	cmd.exec("#exit");
}
void func_exec()     { ogc_exec( cmd.argC(1) ); }
void func_way_record()
{
	waypoint.isRecord = true;
	waypoint.CalcRecordIndex = true;
}
//
void func_way_stop()
{
	waypoint.isRecord = false;
}
void func_way_load(void)
{
	waypoint.load();
}

void func_way_save(void)
{
	waypoint.save();
}

void func_way_clear()
{
	if(cmd.argS(1).empty())
	{
		waypoint.clearAll();
		return;
	}
	int index = cmd.argI(1);
	waypoint.clear(index);
}
void func_color()
{
	int arg=1;
	while(*cmd.argC(arg))
	{
		colorList.set( cmd.argC(arg  ), cmd.argC(arg+1), 
			 cmd.argC(arg+2),  cmd.argC(arg+3),  cmd.argC(arg+4));
		arg+=5;
	}
}
/////////////2 way max
/*void func_way_load2(void)
{
	autoroute.LoadFromFile();
}

void func_way_save2(void)
{
	autoroute.SaveToFile();
}

void func_way_add_point2()
{
	autoroute.AddPoint();
}

void func_way_record_line2()
{
	int index = cmd.argI(1) - 1; 
	autoroute.RecordLine(index);
}

void func_way_clear_line2()
{
	int index = cmd.argI(1) - 1; 
	autoroute.ClearLine(index);
}

void func_way_clear2()
{
	autoroute.ClearRoute();
}

void func_way_record_stop2()
{
	autoroute.RecordStop();
}

void func_way_record_add2()
{
	autoroute.AddPoint();
}*/
//===================================================================================
bool func_eval_condition(int& refArgIndex)
{
	char* test  = cmd.argC(refArgIndex++);
	char* szArg = cmd.argC(refArgIndex++);
	if(!*test||!*szArg){ return false; }
	if( !strcmp(test,"is"    ))  
	{ 
		if(!strcmp(szArg,"t"    )) { return me.team==1; }
		if(!strcmp(szArg,"ct"   )) { return me.team!=1; }
	    if(!strcmp(szArg,"alive")) { return (me.alive); }
	}
	return false;
}

//===================================================================================
void func_if()
{
	int  pos = 1;
	bool final_condition = func_eval_condition(pos);
	while(1){ 
		if( *cmd.argC(pos)=='&' ) {
			final_condition &= func_eval_condition(pos);
		} else if( *cmd.argC(pos)=='|' ) {
			final_condition |= func_eval_condition(pos);
		} else {
			break;
		} 
	}
	int truepos = pos;
	int elsepos = 0;
	while(1){
		char* arg = cmd.argC(pos);
		if(!*arg) { break; }
		if(!strcmp(arg,"else")) { elsepos = pos; break; }
		pos++;
	}
	string command;
	if( !elsepos )
	{
		if(final_condition)
		{
			cmd.collectArguments(command,truepos);
			cmd.exec( const_cast<char*>( command.c_str() ) );
		}
	} else {
		if(final_condition)
		{
			cmd.collectArguments(command,truepos,elsepos-1);
			cmd.exec( const_cast<char*>( command.c_str() ) );
		} else {
			cmd.collectArguments(command,elsepos+1);
			cmd.exec( const_cast<char*>( command.c_str() ) );
		}
	}
}
void func_later()
{
	if(cmd.argS(1).empty())
	{
		list<ScheduledCommand>::iterator pos;
		for(pos=scheduleList.begin();pos!=scheduleList.end();++pos)
		{
			Con_Echo("%6.2f sec: \"%s\"", (*pos).timer.timeleft(), (*pos).cmd.c_str() );
		}

		return;
	}

	if(cmd.argS(1)=="clear")
	{
		scheduleList.erase(scheduleList.begin(),scheduleList.end());
		return;
	}

	float time = cmd.argF(1);

	ScheduledCommand tmp;
	cmd.collectArguments( tmp.cmd, 2); 
	tmp.timer.countdown(time);
	if(scheduleList.size()<32) scheduleList.push_back(tmp);
}

void func_alias()
{
	const char* name = cmd.argC(1);
	string& content = cmd.argS(2);
	cmd.AddAlias(name,content);
}

void func_bind()
{
	keyBindManager.addBind(cmd.argC(1),cmd.argC(2));
}

void func_unbind()
{
	keyBindManager.removeBind(cmd.argC(1));
}
void func_load()
{
	exec( cmd.argC(1) ); 
}
void func_avadd(void)
{
	aimvec av;
	int duck = cmd.argI(1);
	int arg = 2;
	while(*cmd.argC(arg))
	{
		av.h = cmd.argF(arg++);
		av.f = cmd.argF(arg++);
		av.r = cmd.argF(arg++);
		if (duck == 1)  
			Aimbot.AimVecsD.push_back(av); 
		if (duck == 0) 
			Aimbot.AimVecsS.push_back(av); 
		if (duck == 2)
			Aimbot.AimVecsJ.push_back(av);
	}
}

void func_avclear(void)
{
	Aimbot.AimVecsS.clear(); 
	Aimbot.AimVecsD.clear();
	Aimbot.AimVecsJ.clear();
}
void func_avclear0(void) 
{ 
   Aimbot.AimVecsS.clear();
}
void func_avclear1(void) 
{ 
   Aimbot.AimVecsD.clear(); 
}
void func_avclear2(void) 
{ 
   Aimbot.AimVecsJ.clear();
}
void func_standvec()
{
	srand(static_cast<unsigned>(time(0)));
	int height = rand() % (22 -19 + 1) + 19;
	int forward = rand() % (5 -0 + 1) + 0;
	int right = rand() % (4 -0 + 1) + 0;
	aimvec av;
	av.h = height;
	av.f = forward;
	av.r = right;
	func_avclear0();
	Aimbot.AimVecsS.push_back(av);
	gConsole.echo("Generating Stand Vec: Created avadd 0 %i %i %i", height,forward,right);
}
void func_duckvec()
{
	srand(static_cast<unsigned>(time(0)));
	int height1 = rand() %  (28 -24 + 1) + 24;
	int forward1 = rand() % (5 -0 + 1) + 0;
	int right1 = rand() % (4 -0 + 1) + 0;
	aimvec av;
	av.h = height1;
	av.f = forward1;
	av.r = right1;
	func_avclear1();
	Aimbot.AimVecsD.push_back(av);
	gConsole.echo("Generating Duck Vec: Created avadd 1 %i %i %i", height1,forward1,right1);
}
void func_jumpvec()
{
	srand(static_cast<unsigned>(time(0)));
	int height2 = rand() %  (14 -10 + 1) + 10;
	int forward2 = rand() % (5 -0 + 1) + 0;
	int right2 = rand() % (4 -0 + 1) + 0;
	aimvec av;
	av.h = height2;
	av.f = forward2;
	av.r = right2;
	func_avclear2();
	Aimbot.AimVecsJ.push_back(av);
	gConsole.echo("Generating Jump Vec: Created avadd 2 %i %i %i", height2,forward2,right2);
}
void func_avlist(void)
{
	int dmmy = cmd.argI(1);
	if (dmmy==0)
	{
		Con_Echo("Listing AimVecs(0) Standing\n");
		for (vector<aimvec>::iterator si = Aimbot.AimVecsS.begin(); si != Aimbot.AimVecsS.end();++si)
		{
			Con_Echo("HeighT: &w%f&a ForwarD: &w%f&a RighT: &w%f&a\n",si->h,si->f,si->r);
		}
	}
	if (dmmy==1)
	{
		Con_Echo("Listing AimVecs(1) Ducking\n");
		for (vector<aimvec>::iterator si = Aimbot.AimVecsD.begin(); si != Aimbot.AimVecsD.end();++si)
		{
			Con_Echo("HeighT: &w%f&a ForwarD: &w%f&a RighT: &w%f&a\n",si->h,si->f,si->r);
		}
	}
	if (dmmy==2)
	{
		Con_Echo("Listing AimVecs(2) Jumping\n");
		for (vector<aimvec>::iterator si = Aimbot.AimVecsJ.begin(); si != Aimbot.AimVecsJ.end();++si)
		{
			Con_Echo("HeighT: &w%f&a ForwarD: &w%f&a RighT: &w%f&a\n",si->h,si->f,si->r);
		}
	}
}
void func_avchange_h(void)
{
   int argX = cmd.argI(1);
   float add = cmd.argF(2);

   if(argX==0)
      Aimbot.AimVecsS[0].h+=add;
   else if(argX==1)
      Aimbot.AimVecsD[0].h+=add;
   else if(argX==2)
      Aimbot.AimVecsJ[0].h+=add;
}

void func_avchange_f(void)
{
   int argX = cmd.argI(1);
   float add = cmd.argF(2);

   if(argX==0)
      Aimbot.AimVecsS[0].f+=add;
   else if(argX==1)
      Aimbot.AimVecsD[0].f+=add;
   else if(argX==2)
      Aimbot.AimVecsJ[0].f+=add;
}

void func_avchange_r(void)
{
   int argX = cmd.argI(1);
   float add = cmd.argF(2);

   if(argX==0)
      Aimbot.AimVecsS[0].r+=add;
   else if(argX==1)
      Aimbot.AimVecsD[0].r+=add;
   else if(argX==2)
      Aimbot.AimVecsJ[0].r+=add;
}
void func_avsave() 
{ 
   string filename = getHackDirFile(cmd.argC(1));  // first arg: filename 
   string vecalias = cmd.argC(2); 
   float recoil = cvar.recoil;
 
   filename += ".cfg"; 
   const char* file = filename.c_str(); 
   const char* vec = vecalias.c_str(); 

   if (!file || strlen(file) == 0 || !vec || strlen(vec) == 0) 
   { 
       Con_Echo("Format: avsave <filename> <vecalias>"); 
       return; 
   } 

   ofstream ofs(file, ios::binary | ios::app); 
   if (!ofs) 
   { 
       return; 
   } 

   ofs << "                 -=Viking Wpt=-" << (char)0x0D << (char)0x0A; 
   ofs << "      -=Copy your new VEC to VEC-.cfg=-" << (char)0x0D << (char)0x0A; 

   ofs << "Stand " << vec << " 0 \""; 
   for (vector<aimvec>::iterator si = Aimbot.AimVecsS.begin(); si != Aimbot.AimVecsS.end();++si) 
   { 
        ofs << "avadd 0 " << si->h << ' ' << si->f << ' ' << si->r << ""; 
   } 

   ofs << "\"" << (char)0x0D << (char)0x0A << "Duck  " << vec << " 1 \""; 
   for (vector<aimvec>::iterator si2 = Aimbot.AimVecsD.begin(); si2 != Aimbot.AimVecsD.end();++si2) 
   { 
        ofs << "avadd 1 " << si2->h << ' ' << si2->f << ' ' << si2->r << ""; 
   } 

   ofs << "\"" << (char)0x0D << (char)0x0A << "Jump  " << vec << " 2 \""; 
  for (vector<aimvec>::iterator si3 = Aimbot.AimVecsJ.begin(); si3 != Aimbot.AimVecsJ.end();++si3) 
  { 
       ofs << "avadd 2 " << si3->h << ' ' << si3->f << ' ' << si3->r << ""; 
  }   

   ofs << "\"" << (char)0x0D << (char)0x0A << (char)0x0D << (char)0x0A << (char)0x0D << (char)0x0A; // print the alias that starts the vec, with recoil and a txt message
   if (ofs) // success message 
	{
	   gConsole.active = !gConsole.active;
	   Con_Echo("&rYour new VEC has been saved to VEC-Maker.cfg", filename.c_str()); 
	}
   ofs.close(); // close file
}


void func_txt()
{
	string s;
	cmd.collectArguments(s);
	gSetHudMessage2( s.c_str() );
}
//==============================================================================
void CommandInit()
{
	cmd.init();

	#define REG_COMMAND(name) cmd.AddCommand( #name, func_##name);

	REG_COMMAND(quit)
	REG_COMMAND(color)
	REG_COMMAND(load)
	REG_COMMAND(alias)
	REG_COMMAND(bind)
	REG_COMMAND(unbind)
	REG_COMMAND(later)
	REG_COMMAND(avlist)
	REG_COMMAND(avsave)
	REG_COMMAND(avclear)
	REG_COMMAND(avadd)
	REG_COMMAND(avclear0    )
	REG_COMMAND(avclear1    )
	REG_COMMAND(avclear2    )
	REG_COMMAND(standvec      )
	REG_COMMAND(duckvec      )
	REG_COMMAND(jumpvec       )  
	REG_COMMAND(avchange_f    )
	REG_COMMAND(avchange_r    )
	REG_COMMAND(avchange_h    )
	REG_COMMAND(txt)
    REG_COMMAND(way_load)
	REG_COMMAND(way_save)
	REG_COMMAND(way_clear)
	REG_COMMAND(way_record)
	REG_COMMAND(way_stop)
	REG_COMMAND(setburst   ) 
	REG_COMMAND(first_kill_mode) 
    REG_COMMAND(addplayer)    
    REG_COMMAND(relistplayer) 
    REG_COMMAND(clearallplayer)
    REG_COMMAND(player_list)
/*	REG_COMMAND(way_load2)//max
	REG_COMMAND(way_save2)
	REG_COMMAND(way_record_line2)
	REG_COMMAND(way_clear_line2)
	REG_COMMAND(way_clear2)
	REG_COMMAND(way_record_stop2)
	REG_COMMAND(way_record_add2)*/
	REG_COMMAND(exec)
    REG_COMMAND(if        )
}
//==============================================================================

//==============================================================================
void HUD_UpdateClientData(client_data_t *cdata, float flTime)
{
	WeaponListUpdate(cdata->iWeaponBits);
}
//==============================================================================

//Entity Handling ==============================================================
float GetDistanceFrom(float* pos) //¬∑µ„
{	
	register float a = pos[0] - me.pmEyePos[0];
	register float b = pos[1] - me.pmEyePos[1];
	register float c = pos[2] - me.pmEyePos[2];
	return sqrt(a*a + b*b + c*c);
}

void PlayerInfo::updateEntInfo()
{	
	gEngfuncs.pfnGetPlayerInfo(entindex, &entinfo);

	if(!entinfo.name ) { entinfo.name  = "\\missing-name\\"; }
	if(!entinfo.model) { entinfo.model = "unknown model";    } 
}

#define MAXWEAPONS 31
enum
{
	W_NULL = 0,
	W_P228,
	W_UNKNOWN,
	W_SCOUT,
	W_HEGRENADE,
	W_XM1014,
	W_C4,
	W_MAC10,
	W_AUG,
	W_SMOKEGRENADE,
	W_ELITES,
	W_FIVESEVEN,
	W_UMP,
	W_SG550,
	W_GALIL,
	W_FAMAS,
	W_USP,
	W_GLOCK,
	W_AWP,
	W_MP5,
	W_PARA,
	W_M3,
	W_M4A1,
	W_TMP,
	W_G3SG1,
	W_FLASHBANG,
	W_DEAGLE,
	W_SG552,
	W_AK47,
	W_KNIFE,
	W_P90,
};

char *pszWeapon[MAXWEAPONS] = 
{ 
	"Null",
	"228 Compact",
	"Unknown",
	"Schmidt Scout",
	"HE Grenade",
	"Leone YG1265 Auto Shotgun",
	"C4 Explosives",
	"Ingram MAC-10",
	"Bullpup",
	"Smoke Grenade",
	".40 Dual Elites",
	"ES Five-Seven",
	"KM UMP45",
	"Krieg 550 Commando",
	"IDF Defender",
	"Clarion 5.56",
	"KM .45 Tactical",
	"9x19mm Sidearm",
	"Magnum Sniper Rifle",
	"KM Sub-Machine Gun",
	"M249",
	"Leone 12 Gauge Super",
	"Maverick M4A1 Carbine",
	"Schmidt Machine Pistol",
	"D3/AU1",
	"Flashbang",
	"Night Hawk .50C",
	"Krieg 552",
	"CV-47",
	"Knife",
	"ES C90"
};
char* gGetWeaponName( int weaponmodel )
{
	static char weapon[50];
	weapon[0]=0;

	model_s* mdl = oEngStudio.GetModelByIndex( weaponmodel );
	if( !mdl ){ return weapon; }

	char* name = mdl->name;  if( !name )          { return weapon; }
	int len = strlen(name);  if( len>48 || len<10){ return weapon; }

    if (len > 25)//to catch shield string 
    { 
        strcpy(weapon,name+23); len -= 23; 
    } 
    else//no shield 
    { 
        strcpy(weapon,name+9); len -=9; 
    } 
	if(len>4)weapon[len-4]=(char)0;

	return weapon;
}

static void playerCalcExtraData(int ax, cl_entity_s* ent)
{
	PlayerInfo& r = vPlayers[ax];

	// playername and model
	r.updateEntInfo();

	if( cvar.weapon && oEngStudio.GetModelByIndex )
	{
		r.setWeapon(gGetWeaponName(r.getEnt()->curstate.weaponmodel));
	}

	// distance
	r.distance  = GetDistanceFrom (r.origin()); if(r.distance<1) r.distance=1; // avoid division by zero
	Aimbot.calcFovangleAndVisibility(ax);
	vPlayers[ax].bGotHead = false;
}

static void AddEntityPlayer(struct cl_entity_s *ent)
{
	int px = ent->index;

	if(ent->curstate.solid) { vPlayers[px].setAlive();}

	// filter local player out out and set me.alive
	if(ent->index == me.entindex) 
	{ 
		vPlayers[px].distance = 100000.0;
		vPlayers[px].visible  = false;
		vPlayers[px].updateClear();
		return;
	}

	if( vPlayers[px].isAlive() )
	{
		vPlayers[px].updateAddEntity(ent->origin);
		playerCalcExtraData(px,ent);
	}
}
//==============================================================================

static void updateLocalPlayer()
{
	me.entindex = 0;

	me.ent = gEngfuncs.GetLocalPlayer();

	cl_entity_s * itsme = gEngfuncs.GetLocalPlayer();
	if(itsme)
	{
		me.entindex = itsme->index;
	}
}
//==============================================================================


////////////////////////////
static ColorEntry* PlayerColor(int ax)
{
	static ColorEntry colorDefault;
	if ((cvar.sglow) && ((vPlayers[ax].iInfo & 2) || (vPlayers[ax].iInfo & 4))) { return colorList.get(0); }
	if (cvar.tglow && ax == Aimbot.iTarget)   { return colorList.get(24); }
	if (vPlayers[ax].team == 1) { return colorList.get(2); }
	else                        { return colorList.get(3); }
}

//==============================================================================

int getSeqInfo(int ax)
{
	return Cstrike_SequenceInfo[vPlayers[ax].getEnt()->curstate.sequence];
}

//==============================================================================

#define MAX_SPRITES 212
typedef struct
{
   char szName[256];
   char szSprite[256];
   HSPRITE hspr;
   wrect_t rc;
   int iRes;
} spriteinfo_t;

//==============================================================================


////////////////////////////////////
void DrawLine( int ax, int ay, int bx, int by, int width, int r, int g, int b, int a )
{
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4ub( r, g, b, a);

    glLineWidth(width);
    glBegin(GL_LINES);
    glVertex2i( ax, ay);
    glVertex2i( bx, by);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
}
//==============================================================================
typedef struct
{
	int iSpritesLoaded;
	spriteinfo_t spriteinfo[MAX_SPRITES];
}spritedata_t;

spritedata_t spritedata;
static void drawPlayerEsp(int ax)
{
	ColorEntry* color = PlayerColor(ax);
	if (cvar.radar){drawRadarPoint(vPlayers[ax].origin(),color->r,color->g,color->b,255);}

	// rest of code is only for players on screen:
	float vecScreen[2];
	if( !CalcScreen(vPlayers[ax].origin(),vecScreen) ) { return; }	

	// distance and boxradius also needed for esp offset
	float distance = vPlayers[ax].distance/22.0f;
	int   boxradius = (300.0*90.0) / (distance*fCurrentFOV);	 
	BOUND_VALUE(boxradius,1,200);

	if (cvar.box == 1)
		gDrawBoxAtScreenXY(vecScreen[0],vecScreen[1],color->r,color->g,color->b,color->a,boxradius);
	else if (cvar.box == 2)
		gDrawBoxAtScreenXY(vecScreen[0],vecScreen[1],color->r,color->g,color->b,color->a,10);	
	
	// ====== show visibility
	const char* format_string; 
	const char* format_int;
	if (vPlayers[ax].visible) { format_string="%s";format_int="%i"; }
	else                      { format_string="%s";format_int="%i"; }

	/*
	// ===== prepare text esp:
	enum{ CHAR_HEIGHT = 16 };
	int text_dist = (int)(boxradius);
	int ystep = -CHAR_HEIGHT;

	int x = vecScreen[0];
	int y = vecScreen[1] -30;
	if (me.alive) y = y-text_dist-CHAR_HEIGHT+10;
	*/

	enum{ CHAR_HEIGHT = 16 };
	int text_dist = (int)(boxradius);
	int ystep = CHAR_HEIGHT;

	enum{ CH = 9 };
	int fstep = CHAR_HEIGHT;

	int x = vecScreen[0];	
	int y = vecScreen[1]-40;

	if (cvar.esp_line)
	{
		if(ystep>0) // stack down
		{
			gEngfuncs.pfnFillRGBA(x,vecScreen[1],1,text_dist,color->r,color->g,color->b,255);
		}
		else
		{
			gEngfuncs.pfnFillRGBA(x,y+CHAR_HEIGHT,1,text_dist,color->r,color->g,color->b,255);
		}
	}

	if(cvar.ping)
	{
		DrawConStringCenter(x,y,color->r,color->g,color->b,format_int,  vPlayers[ax].GetPing());
		y += ystep;
	}

	if(cvar.targetline && vPlayers[ax].team != me.team)
	{
	DrawLine(displayCenterX,displayCenterX,x,y,3,color->r,color->g,color->b,255);
	}

	if (cvar.weapon==1 || (cvar.weapon==2 && idhook.FirstKillPlayer[ax]))
	{
//	   if( isEnemy(ax))
       {
		   const char* displayname = vPlayers[ax].getWeapon();

		   int seqinfo = getSeqInfo(ax);
		   const char* tmpSeq;

		   if (seqinfo & SEQUENCE_ARM_C4)
			   tmpSeq = "- ARMING C4 -";
		   else if (seqinfo & SEQUENCE_RELOAD)
			   tmpSeq = "- RELOADING -";
		   else if (seqinfo & SEQUENCE_THROW)
			   tmpSeq = "- THROWING - ";
		   else if (seqinfo & SEQUENCE_SHIELD)
			   tmpSeq = "- SHIELDED -";
		   else if (seqinfo & SEQUENCE_SHIELD_SIDE)
			   tmpSeq = "- SHIELD TO SIDE -";
		   else if (seqinfo & SEQUENCE_DIE)
			   tmpSeq = "";

		   if ( (seqinfo & SEQUENCE_ARM_C4) || (seqinfo & SEQUENCE_THROW) ||
			   (seqinfo & SEQUENCE_SHIELD) || (seqinfo & SEQUENCE_SHIELD_SIDE) ||
			   ( seqinfo & SEQUENCE_RELOAD ) )
		   {
			   displayname = tmpSeq;
		   }
		   
		   DrawHudStringCenter(x,y,color->r,color->g,color->b,format_string,displayname);
		   y += ystep;
	   }
	}

	if (!cvar.rush && cvar.name==1 || (cvar.name==2 && idhook.FirstKillPlayer[ax]))
	{
//	   if( isEnemy(ax))
       {
		   char displayname[32];
//		   int len = (cvar.name==1) ? 8 : cvar.name;
//		   if(len<0 || len>30) { len = 30; }
		   strncpy(displayname,vPlayers[ax].getName(),32);
		   displayname[32] = 0;
		   
		   DrawHudStringCenter(x,y,color->r,color->g,color->b,format_string,displayname);
		   y += ystep;
	   }
	}	
	if (!cvar.rush && cvar.distance==1 || (cvar.distance==2 && idhook.FirstKillPlayer[ax]))
	{
		DrawHudStringCenter(x,y,color->r,color->g,color->b,format_int,  (short int)distance);
		y += ystep;
	}
}
//OTHERS THAN PLAYERS ==========================================================
static inline void AddEspInfo(esp_info& r)
{
	if(espInfoCount < (MAX_ESPINFO-1))
	{
		assert(r.color);
		vEspInfo[espInfoCount] = r;
		++espInfoCount;
	}
}

//========================================================================================

static void AddEntityWeapon (struct cl_entity_s *ent, const char *mdlname, bool special=false)
{
	esp_info esp;
	
	VectorCopy(ent->origin,esp.origin);
	esp.color = colorList.get(4); // "esp"

	// get entity name from model name
	int len = strlen(mdlname);   
	len -= 4;  // strip ".mdl"
	if( len>15 ) len=15; // prevent buffer overflow
	strncpy(esp.name,mdlname,len); 
	esp.name[len]=0;

	if(!strcmp(esp.name,"backpack")) { special = true; } 
	if(!strcmp(esp.name,"c4"      )) { special = true; }

	if(special) { esp.blink = true; esp.color = colorList.get(5); /* "special" */ }
	AddEspInfo(esp);
}

//========================================================================================

static void AddEntityHostage(struct cl_entity_s *ent)
{
	if(!ent->curstate.solid) { return; }  // dead hostage

	esp_info esp;
	
	VectorCopy(ent->origin, esp.origin);
	esp.origin[2] += 75;
	
	strcpy(esp.name,"hostage");
	esp.color = colorList.get(5); // "special"
	esp.blink = true;

	AddEspInfo(esp);
}

//========================================================================================
static void drawEsp(int ab)
{
	esp_info& esp = vEspInfo[ab];
	
	if (cvar.radar && esp.blink){drawRadarPoint(esp.origin,255,255,255,255,esp.blink);}

	float vecScreen[2];
	if(CalcScreen(esp.origin,vecScreen) && (!esp.blink||blinkPhase) )
	{
		DrawHudStringCenter(vecScreen[0], vecScreen[1], esp.color->r, esp.color->g, esp.color->b, esp.name);
	}
}
void glFillArea(float x, float y, int w, int h, int r, int g, int b, int a, int rr, int gg, int bb, int aa)
{
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_STRIP);
	glColor4ub(rr,gg,bb,aa);
	glVertex2f(x,y + h);
	glVertex2f(x + w,y + h);
	glColor4ub(r,g,b,a);
	glVertex2f(x,y);
	glVertex2f(x + w,y);
	glEnd();
	
	glDisable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
void opengl_ClearBackground();
HSPRITE sprset_hsprite = 0;
int     sprset_r, sprset_g, sprset_b;
extern bool oglSubtractive;

////////////////////////////////////////////////////////////////////////////////////////////////////////

void SPR_DrawAdditive( int frame, int x, int y, const wrect_t *prc )
{
	if(cvar.hud) gEngfuncs.pfnSPR_Set (sprset_hsprite,sprset_r,sprset_g,sprset_b);
	gEngfuncs.pfnSPR_DrawAdditive(frame,x,y,prc);

}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void FillRGBA( int x, int y, int width, int height, int r, int g, int b, int a )
{
	if(cvar.hud)
	{
		register ColorEntry* color = colorList.get(27);
		gEngfuncs.pfnFillRGBA(x,y,width,height,color->r,color->g,color->b,a);
	} else {
		gEngfuncs.pfnFillRGBA(x,y,width,height,r,g,b,a);
	}
}
	
////////////////////////////////////////////////////////////////////////////////////////////////////////

int	DrawCharacter( int x, int y, int number, int r, int g, int b )
{
	if( b==0 && g>0 && cvar.hud)
	{
		register double ratio = (double(r)/double(g));
		if( ratio>=1.5 && ratio<=1.7 )
		{
			register ColorEntry* color = colorList.get(27);
			return gEngfuncs.pfnDrawCharacter(x,y,number,color->r*r/255,color->g*r/255,color->b*r/255);
		}
	}
	return gEngfuncs.pfnDrawCharacter(x,y,number,r,g,b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////

void SPR_Set (HSPRITE hPic, int r, int g, int b)
{
	if(cvar.hud)
	{
		register ColorEntry* color = colorList.get(27);
		sprset_hsprite=hPic;
		int intensity = MAX_VALUE(r,g,b);
		sprset_r = color->r*intensity/255;
		sprset_g = color->g*intensity/255;
		sprset_b = color->b*intensity/255;
	}
	gEngfuncs.pfnSPR_Set (hPic,r,g,b);
}
//==============================================================================
int roundcounter = 0;
void AtRoundStart(void)
{
	if( strcmp(currentMap,gEngfuncs.pfnGetLevelName()) )
	{
		strcpy(currentMap,gEngfuncs.pfnGetLevelName());
		AtMapChange();
	}
	if(cvar.way_lock) waypoint.wayLock = true;
	if(me.team>0) me.alive = true;

	gEngfuncs.pfnGetPlayerInfo(me.ent->index, &vPlayers[me.ent->index].entinfo);
	beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex("sprites/laserbeam.spr");
	roundcounter++;
	if(cvar.sayid)
	{
	char tmp[128];
	if(me.alive)
    sprintf(tmp,"say Viking Wpt 2017");
	gEngfuncs.pfnClientCmd(tmp);
	}
    cmd.exec("#hud_fastswitch 1");
	cmd.exec("fakelag -99999999999");
	cmd.exec("#cl_bobup 0");
	cmd.exec("#cl_bob 0");
	cmd.exec("way_load");
//	cmd.exec("way_load2"); max
	cmd.exec("#impulse 101;later 0.5 buystuff");
	for(int i=0;i<MAX_VPLAYERS;i++) 
	{ 
		vPlayers[i].updateClear();
		vPlayers[i].bGotHead = false;
		vPlayers[i].setAlive();
	}	
}

//USER MESSAGES HANDLING =======================================================

#define USER_MSG_INTERCEPT(name) \
	static pfnUserMsgHook name ## Org = NULL; \
	static int UserMsg ## name ( const char *pszName, int iSize, void *pbuf)
#define USER_MSG_CALL(name) (*name ## Org)(pszName,iSize,pbuf)

//==============================================================================

USER_MSG_INTERCEPT(CurWeapon)
{
	BEGIN_READ( pbuf, iSize );
	int iState = READ_BYTE();
	int iID    = READ_CHAR();
	int iClip  = READ_CHAR();

	static int currentWeapon = -1;

	if (iState) 
	{
		me.iClip = iClip;
		if (currentWeapon != iID)
		{
			currentWeapon = iID;
		}
	}

	WeaponListCurWeapon(iState, iID, iClip);

	return USER_MSG_CALL(CurWeapon);
}

//==============================================================================

USER_MSG_INTERCEPT(TeamInfo)
{
	static bool         done=false;
	static StringFinder teamNumber;
	if(!done)
	{
		teamNumber.add("terrorist"  ,1);teamNumber.add("ct"         ,2);
		teamNumber.add("unassigned" ,0);teamNumber.add("spectator"  ,0);
		done = true;
	}

	updateLocalPlayer();

	BEGIN_READ(pbuf,iSize);
	int px = READ_BYTE();
	char * teamtext = READ_STRING();

	if(!*teamtext) { vPlayers[px].team = 0; }
	else
	{
		_strlwr(teamtext);
		if(teamNumber.find(teamtext))
		{
			vPlayers[px].team = teamNumber.num;
			if(px==me.entindex){ me.team = teamNumber.num; }
		}
		else
		{
			vPlayers[px].team = -1;
			if(px==me.entindex){ me.team = -1; }
		}
	}	
	return USER_MSG_CALL(TeamInfo);
}

//==============================================================================

USER_MSG_INTERCEPT(SetFOV)
{	
	BEGIN_READ( pbuf, iSize );
	me.iFOV = READ_BYTE();

	if(!me.iFOV)
		me.iFOV=90;
	if(me.iFOV==90)
		me.inZoomMode=false;
	else
		me.inZoomMode=true;

	fCurrentFOV = me.iFOV;	
	return USER_MSG_CALL(SetFOV);
}

//==============================================================================

USER_MSG_INTERCEPT(ResetHUD)
{
	AtRoundStart();
	    //∂≥Ω·
	isInFrozenTime=true;
	return USER_MSG_CALL(ResetHUD);
}

//==============================================================================

USER_MSG_INTERCEPT(WeaponList)
{
	char *weaponname;
	int ammo1type, max1, ammo2type, max2, slot, slotpos, id, flags;
	BEGIN_READ(pbuf, iSize);
	weaponname = READ_STRING();
	ammo1type = READ_CHAR();
	max1 = READ_BYTE();
	ammo2type = READ_CHAR();
	max2 = READ_BYTE();
	slot = READ_CHAR();
	slotpos = READ_CHAR();
	id = READ_CHAR();
	flags = READ_BYTE();
	WeaponListAdd(weaponname, ammo1type, max1, ammo2type, max2, slot, slotpos, id, flags);
	return USER_MSG_CALL(WeaponList);
}

//==============================================================================

USER_MSG_INTERCEPT(DeathMsg)
{
	BEGIN_READ( pbuf, iSize );
	int killer = READ_BYTE();
	int victim = READ_BYTE();
	int headshot = 0;
	headshot = READ_BYTE();
    char* weaponName = READ_STRING();	

	if( killer == me.entindex && headshot)
	{
		bGotHeadshot=true;
	}

	if (victim == me.entindex) {
		me.alive = false;
		Aimbot.SetTarget(-1);
	}	
	int px = victim;
	vPlayers[px].setDead();
	vPlayers[victim].updateClear();
	if(cvar.extrablood && headshot)
	{
		for(int spray_count = 0;spray_count<5;spray_count++)
		{
			cl_entity_s *pEnt = vPlayers[victim].getEnt();
			vec3_t blood_origin;
			vec3_t blood_direction;
			int speed;

			blood_origin[0] = pEnt->origin[0];
			blood_origin[1] = pEnt->origin[1];
			blood_origin[2] = pEnt->origin[2]+30;

			blood_direction[0] = (float)(rand()%41)-20;
			blood_direction[1] = (float)(rand()%41)-20;
			blood_direction[2] = (float)(rand()%251)+50;

			speed = (rand()%101)+100;

			gEngfuncs.pEfxAPI->R_BloodStream
			( 
				blood_origin,
				blood_direction, 
				70, 
				speed
			);
		}
	}
	return USER_MSG_CALL(DeathMsg);
}

//==============================================================================

USER_MSG_INTERCEPT(ScoreInfo)
{
	BEGIN_READ(pbuf, iSize);
	int idx = READ_BYTE();
	PlayerInfo& r = vPlayers[idx];

	r.frags  = READ_SHORT();
	r.deaths = READ_SHORT();

	if(r.frags<=0 )   { r.ratio=0; }
	else              { r.ratio = (double)r.frags / ((r.deaths<=0)?1:r.deaths); }

	// update bestplayer
	r.bestplayer = false;
	bool foundbetter = false;
	for(int i=0;i<MAX_VPLAYERS;i++)
	{
		if     ( r.ratio < vPlayers[i].ratio ) foundbetter = true;
		else if( r.ratio > vPlayers[i].ratio ) vPlayers[i].bestplayer=false;
	}
	if(!foundbetter) r.bestplayer = true;

	
	// score handling:
	if(idx==me.entindex)
	{
		static int lastfrags=0;
		static int lastdeaths=0;

		if(me.headshots>r.frags || r.frags==0) 
		{ 
			// reset statistics
			me.headshots = 0; 
			me.hspercent = 0;
			bGotHeadshot = false; 
			lastfrags    = r.frags;
			lastdeaths   = r.deaths;
		}
		else
		{
			if(bGotHeadshot)         { ++me.headshots; }

			double den = r.frags ? (double)r.frags : 1.0;
			me.hspercent = (double)me.headshots/den*100.0;

		}
		bGotHeadshot = false;
		lastfrags    = r.frags;
		lastdeaths   = r.deaths;
	}

	return USER_MSG_CALL(ScoreInfo);
}

//==============================================================================
USER_MSG_INTERCEPT(AmmoX)
{
	int ID, Count;
	BEGIN_READ(pbuf, iSize);
	ID = READ_BYTE();
	Count = READ_BYTE();
	WeaponListAmmoX(ID, Count);
	return USER_MSG_CALL(AmmoX);
}

//==============================================================================

int	HookUserMsg (char *szMsgName, pfnUserMsgHook pfn)
{
	int retval = gEngfuncs.pfnHookUserMsg(szMsgName, pfn);

	#define REDIRECT_MESSAGE(name) \
		else if (!strcmp(szMsgName,#name)) \
		{ \
			name##Org = pfn; \
			return gEngfuncs.pfnHookUserMsg (szMsgName, UserMsg##name ); \
		}	

	if(0){}
	REDIRECT_MESSAGE( TeamInfo    ) REDIRECT_MESSAGE( CurWeapon   )
	REDIRECT_MESSAGE( SetFOV      ) REDIRECT_MESSAGE( DeathMsg    )
	REDIRECT_MESSAGE( ResetHUD    ) REDIRECT_MESSAGE( WeaponList  )
	REDIRECT_MESSAGE( ScoreInfo	  ) REDIRECT_MESSAGE( AmmoX		  )
	return retval;
}

//==============================================================================

inline std::string getHackDirFile(const char* basename)
{
	if(strstr(basename,"..")){ return ":*?\\/<>\""; }
	string ret = hackdir;
	return (ret+basename);
}

//TimeHandling =================================================================

float  ClientTime::mapTime    = 1;
double ClientTime::current    = 1;
unsigned int ClientTime::current_ms = 1;

//==============================================================================

void AtMapChange()
{

    tWelcomeMessage.setExpired();

	roundcounter = 0;

	gHudTimer2.setExpired();
}

//==============================================================================

void ClientTime::reportMapTime(float mapTimeNew)
{
	mapTime = mapTimeNew;
	updateCurrentTime();
}

//===============================================================================

void ClientTime::updateCurrentTime()
{
	// timer update
	current_ms = timeGetTime();

	current = (double)current_ms / 1000.0;
}

//===============================================================================

void hlexec (const char *fmt, ... )
{
	va_list va_alist;
	char buf[384];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	gEngfuncs.pfnClientCmd(buf);
}
void DrawCrosshair(void)
{
    ColorEntry* color  = colorList.get(9);  // "cross"
	ColorEntry* colo2  = colorList.get(10); // "cross2"

	int r=color->r, g=color->g, b=color->b, a=color->a;
	int R=colo2->r, G=colo2->g, B=colo2->b, A=colo2->a;

	oglSubtractive = true;

	int tcross = cvar.crosshair;
	if (me.inZoomMode) cvar.crosshair = 6;


	if(cvar.crosshair == 1)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX - 14, displayCenterY, 9, 1,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX +5,   displayCenterY, 9, 1,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY - 14, 1, 9,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY +  5, 1, 9,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY     , 1, 1,R,G,B,A); // center
	}
	else if(cvar.crosshair == 2)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX - 14, displayCenterY, 9,2,r,g,b,a); // left
		gEngfuncs.pfnFillRGBA(displayCenterX +6,   displayCenterY, 9,2,r,g,b,a); // right
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY - 14, 2,9,r,g,b,a); // top
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY +  7, 2,9,r,g,b,a); // bottom
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY     , 2,2,R,G,B,A); // center
	}
	else if(cvar.crosshair == 3)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX-25,displayCenterY,50,1,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY-25,1,50,r,g,b,a);

		gEngfuncs.pfnFillRGBA(displayCenterX - 5, displayCenterY, 10, 1,R,G,B,A);
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY - 5,  1,10,R,G,B,A);
	}
	else if(cvar.crosshair == 4)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX-25,displayCenterY,50,2,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY-25,2,50,r,g,b,a);

		gEngfuncs.pfnFillRGBA(displayCenterX - 5, displayCenterY, 10, 2,R,G,B,A);
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY - 5,  2,10,R,G,B,A);
	}
	else if(cvar.crosshair == 5)
	{
		int iX=int(displayCenterX), iY=int(displayCenterY), iSize=25;
		// border top
		tintArea(iX-iSize, iY-iSize, 2*iSize, 1, 255, 255, 255, 180);
		//border bottom
		tintArea(iX-iSize, iY+iSize, 2*iSize, 1, 255, 255, 255, 180);
		//border left
		tintArea(iX-iSize, iY-iSize+1, 1, 2*iSize-1, 255, 255, 255, 180);
		//border right
		tintArea(iX+iSize, iY-iSize, 1, 2*iSize+1, 255, 255, 255, 180);

		//cross
		tintArea(iX, iY-iSize+1, 1, 2*iSize-1, 0, 160, 0, 180);
		tintArea(iX-iSize+1, iY, 2*iSize-1, 1, 0, 160, 0, 180);
	}
	else if(cvar.crosshair == 6)
	{
		gEngfuncs.pfnFillRGBA(0,displayCenterY,2*displayCenterX,1,r,g,b,a);
		gEngfuncs.pfnFillRGBA(displayCenterX,0,1,2*displayCenterY,r,g,b,a);

		gEngfuncs.pfnFillRGBA(displayCenterX - 5, displayCenterY, 10, 1,R,G,B,A);
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY - 5,  1,10,R,G,B,A);
	}
	else if(cvar.crosshair == 7)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX, displayCenterY     , 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 1, displayCenterY + 1, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 2, displayCenterY + 2, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 3, displayCenterY + 3, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 5, displayCenterY + 5, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 6, displayCenterY + 6, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 7, displayCenterY + 7, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 8, displayCenterY + 8, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 9, displayCenterY + 9, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 10, displayCenterY + 10, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 11, displayCenterY + 11, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 12, displayCenterY + 12, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 13, displayCenterY + 13, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 14, displayCenterY + 14, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 15, displayCenterY + 15, 1, 1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX - 1, displayCenterY - 1, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 2, displayCenterY - 2, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 3, displayCenterY - 3, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 5, displayCenterY - 5, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 6, displayCenterY - 6, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 7, displayCenterY - 7, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 8, displayCenterY - 8, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 9, displayCenterY - 9, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 10, displayCenterY - 10, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 11, displayCenterY - 11, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 12, displayCenterY - 12, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 13, displayCenterY - 13, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 14, displayCenterY - 14, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 15, displayCenterY - 15, 1, 1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX + 1, displayCenterY + 1, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 2, displayCenterY + 2, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 3, displayCenterY + 3, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 5, displayCenterY + 5, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 6, displayCenterY + 6, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 7, displayCenterY + 7, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 8, displayCenterY + 8, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 9, displayCenterY + 9, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 10, displayCenterY + 10, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 11, displayCenterY + 11, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 12, displayCenterY + 12, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 13, displayCenterY + 13, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 14, displayCenterY + 14, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 15, displayCenterY + 15, 1, 1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX + 1, displayCenterY - 1, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 2, displayCenterY - 2, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 3, displayCenterY - 3, 1, 1,255,0,0,255); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 5, displayCenterY - 5, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 6, displayCenterY - 6, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 7, displayCenterY - 7, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 8, displayCenterY - 8, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 9, displayCenterY - 9, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 10, displayCenterY - 10, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 11, displayCenterY - 11, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 12, displayCenterY - 12, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 13, displayCenterY - 13, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 14, displayCenterY - 14, 1, 1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 15, displayCenterY - 15, 1, 1,r,g,b,a);

	}
	else if(cvar.crosshair == 8)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX - 14,displayCenterY,9,2,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 5 ,displayCenterY,9,2,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 14,displayCenterY,9,-1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 5 ,displayCenterY,9,-1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY - 14,2,9,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY + 6 ,2,9,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY - 14,-1,9,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY + 6 ,-1,9,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX - 3,displayCenterY,1,1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 3,displayCenterY,1,1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY - 3,1,1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY + 3,1,1,r,g,b,a);
	}
	else if(cvar.crosshair == 9)
	{
		gEngfuncs.pfnFillRGBA(displayCenterX - 14,displayCenterY,9,2,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 5 ,displayCenterY,9,2,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX - 14,displayCenterY,9,-1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 5 ,displayCenterY,9,-1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY - 14,2,9,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY + 6 ,2,9,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY - 14,-1,9,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY + 6 ,-1,9,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX - 3,displayCenterY,1,1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX + 3,displayCenterY,1,1,r,g,b,a); 

		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY - 3,1,1,r,g,b,a); 
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY + 3,1,1,r,g,b,a);

		gEngfuncs.pfnFillRGBA(displayCenterX-1,displayCenterY,3,1,R,G,B,A);
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY-1,1,3,R,G,B,A);
	}
	else if(cvar.crosshair == 10)
	{
		//Lineas ppales
		gEngfuncs.pfnFillRGBA(displayCenterX-20,displayCenterY,40,2,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY-20,2,40,0,0,0,255);
		//Izquierda
		gEngfuncs.pfnFillRGBA(displayCenterX-20,displayCenterY-10,1,22,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX-15,displayCenterY-5,1,12,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX-10,displayCenterY-2,1,6,0,0,0,255);
		//Derecha
		gEngfuncs.pfnFillRGBA(displayCenterX+20,displayCenterY-10,1,22,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX+15,displayCenterY-5,1,12,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX+10,displayCenterY-2,1,6,0,0,0,255);
		//Arriba
		gEngfuncs.pfnFillRGBA(displayCenterX-10,displayCenterY-20,22,1,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX-5,displayCenterY-15,12,1,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX-2,displayCenterY-10,6,1,0,0,0,255);
		//Abajo
		gEngfuncs.pfnFillRGBA(displayCenterX-10,displayCenterY+20,22,1,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX-5,displayCenterY+15,12,1,0,0,0,255);
		gEngfuncs.pfnFillRGBA(displayCenterX-2,displayCenterY+10,6,1,0,0,0,255);
		//Punto blanco
		gEngfuncs.pfnFillRGBA(displayCenterX,displayCenterY,2,2,R,G,B,A);
	}
	oglSubtractive = false;

	cvar.crosshair = tcross;
}
//===============================================================================

int DoSpeed(int eventcode)
{
	if (eventcode == 1)
	{
	//	if (IsCurWeaponKnife())	gSpeed = 20.0f;
	//	else										
		gSpeed = cvar.speed;		
	}
	else
	{
		gSpeed = 0.0;
	}
	return 0;
}
//==============================================================================

//Spin Hack
#define SPIN_REVS_PER_SECOND cvar.spinspeed

void FixupAngleDifference(usercmd_t *cmd)
{
	if (IsCurWeaponNade()) { return; }
	else if (IsCurWeaponC4()) { return; }

    cl_entity_t *pLocal;
    Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vTemp;
    float newforward, newright, newup, fTime;
    float forward = cmd->forwardmove;
    float right = cmd->sidemove;
    float up = cmd->upmove;

    pLocal = gEngfuncs.GetLocalPlayer();
    if(!pLocal)
        return;
    if(pLocal->curstate.movetype == MOVETYPE_WALK)
    {
        gEngfuncs.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), viewforward, viewright, viewup);
    }
    else
    {
        gEngfuncs.pfnAngleVectors(cmd->viewangles, viewforward, viewright, viewup);
    }
    int iHasShiftHeld = GetAsyncKeyState(VK_LSHIFT);
    if(pLocal->curstate.movetype == MOVETYPE_WALK && !iHasShiftHeld && !(cmd->buttons & IN_ATTACK) && !(cmd->buttons & IN_USE))
    {
        fTime = gEngfuncs.GetClientTime();

		if(cvar.spin == 2)
		{      	
			cmd->viewangles.x = cos(fTime * SPIN_REVS_PER_SECOND * 3.1415926535897932384626433832795) * 360.0f;
      		cmd->viewangles.y = sin(fTime * SPIN_REVS_PER_SECOND * 3.1415926535897932384626433832795) * 360.0f;
			cmd->viewangles.x = cos(fTime * SPIN_REVS_PER_SECOND * 3.1415926535897932384626433832795) * 90.0f;
		}
		else
			cmd->viewangles.y = fmod(fTime * SPIN_REVS_PER_SECOND * 360.0f, 360.0f);
    }
    if(pLocal->curstate.movetype == MOVETYPE_WALK)
    {
        gEngfuncs.pfnAngleVectors(Vector(0.0f, cmd->viewangles.y, 0.0f), aimforward, aimright, aimup);
    }
    else
    {
        gEngfuncs.pfnAngleVectors(cmd->viewangles, aimforward, aimright, aimup);
    }

        newforward = DotProduct(forward * viewforward.Normalize(), aimforward) + DotProduct(right * viewright.Normalize(), aimforward) + DotProduct(up * viewup.Normalize(), aimforward);
        newright = DotProduct(forward * viewforward.Normalize(), aimright) + DotProduct(right * viewright.Normalize(), aimright) + DotProduct(up * viewup.Normalize(), aimright);
        newup = DotProduct(forward * viewforward.Normalize(), aimup) + DotProduct(right * viewright.Normalize(), aimup) + DotProduct(up * viewup.Normalize(), aimup);

    cmd->forwardmove = newforward;
    cmd->sidemove = newright;
    cmd->upmove = newup;
}
//==============================================================================

void Con_Echo(const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	// echo + scroll up:
	gConsole.echo("%s",buf);
}

//==============================================================================

void HlEngineCommand(const char* command)
{
	if(!gEngfuncs.pfnClientCmd) { return; }
	gEngfuncs.pfnClientCmd( const_cast<char*>(command) );
}

//==============================================================================
void HandleCvarInt(char* name, int* value)
{
	char* arg1 = cmd.argC(1);  
	if (!strcmp(arg1,"change")) 
	{
		if (*value) *value=0; else *value=1; 
		if(!strcmp(name,"autoburst") && !*value)
		{
			cvar.ftime=1;
			cvar.dtime=0;
			cvar.sdelay=0;
		}
		return;
	}
	if (!strcmp(arg1,"up"))     
	{ 
		*value += cmd.argF(2); 
		return;
	}
    if (!strcmp(arg1,"down"))   
	{ 
		*value -= cmd.argF(2); 
		return;
	}
	if (!strcmp(arg1,"hide")) 
    { 
        *value = cmd.argF(2);
        return;
    }
	if (!*arg1) 
	{ 
		Con_Echo( "CVAR &w%s&r = %i\n",name,*value); 
		return;
	}
    *value = cmd.argF(1);
}
//////////////////////////////////////////////////////
void HandleCvarFloat(char* name, float* value)
{
	char* arg1 = cmd.argC(1);  
	if (!strcmp(arg1,"change")) 
	{
		if (*value) *value=0; else *value=1; 
		return;
	}
	if (!strcmp(arg1,"up"))     
	{ 
		*value += cmd.argF(2); 
		return;
	}
    if (!strcmp(arg1,"down"))   
	{ 
		*value -= cmd.argF(2); 
		return;
	}
	if (!strcmp(arg1,"hide")) 
    { 
        *value = cmd.argF(2);
        return;
    }
	if (!*arg1) 
	{ 
		Con_Echo( "CVAR &w%s&r = %f\n",name,*value); 
		return;
	}
    *value = cmd.argF(1);
}

//==============================================================================

bool isHlCvar(char* name)
{
	if(!gEngfuncs.pfnGetCvarPointer) { return false; }
	
	cvar_s* test = gEngfuncs.pfnGetCvarPointer(name);
	return (test!=NULL);
}

//==============================================================================

void HandleHlCvar(char* name)
{
	if(!gEngfuncs.pfnGetCvarPointer) { return; }

	cvar_s* ptr = gEngfuncs.pfnGetCvarPointer(name);
	if(!ptr) return;

	HandleCvarFloat(name,&ptr->value);
}

//==============================================================================

bool fileExists(const char* filename)
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}

//==============================================================================
