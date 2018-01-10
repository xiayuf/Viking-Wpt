//==============================================================================
// opengl32.cpp
// credits to panzer ;>
//==============================================================================


//==============================================================================
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")
#pragma comment(lib,"GLaux.lib")
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "client.h"
#include "cvars.h"
#include "aimbot.h"
#include "players.h"
//==============================================================================
bool routeDraw = false;
//=========================
bool oglSubtractive=false;
bool bDrawingFlash=false;
bool bDrawingSmoke=false;
bool bDrawingSky=false;
bool bDrawingScope=false;
bool bStartedDrawingEnts=false;
bool bDrawnWorld=false;
int viewport_c=0;
int nAngles=0;
extern float displayCenterX, displayCenterY;
extern cl_enginefuncs_s *pEngfuncs;
extern bool VISIBLE;
float curcol[4];
bool bPolygon = false; 
int polyCoords = 0; 
float polyArray[32][3];
int iCurrentTeam = -1;
//==============================================================================
bool nodrawentities = false;
#define PI 3.1416
//==============================================================================
void opengl_ClearBackground()
{
	if(&glClearColor && &glClear)
	{
		ColorEntry* color = colorList.get(25);
		float r = float(color->r)/255.0f;
		float g = float(color->g)/255.0f;
		float b = float(color->b)/255.0f;
		float a = float(color->a)/255.0f;
		glClearColor(r,g,b,a);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
void BlockFunc() 
{ 
    cl_entity_s *pme = pEngfuncs->GetLocalPlayer(); 
    cl_entity_s *ent; 
    if(me.alive)
	{
		for( int ax = 1; ax < 33; ax++ ) 
		{ 
			ent = pEngfuncs->GetEntityByIndex( ax ); 
			pmtrace_t *vis; 

			if( ( ent != NULL ) && ( ent->index != pEngfuncs->GetLocalPlayer()->index ) && ( ent->curstate.solid ) && ( ent->player ) && ( ent->curstate.weaponmodel ) && ( !ent->curstate.spectator ) && ( ent->curstate.messagenum + 10 > pEngfuncs->GetLocalPlayer()->curstate.messagenum ) && pEngfuncs->GetLocalPlayer()->curstate.solid ) //isValidEnt( ent ) ) 
			{ 

			vis = pEngfuncs->PM_TraceLine(pme->origin, ent->origin, 0, 2, -1); 
             
				if(vis->fraction < 1.0f) 
				{ 
					pEngfuncs->CL_CreateVisibleEntity(ET_NORMAL, ent); 
				} 
			} 
		} 
	}
} 
//==============================================================================

//==============================================================================
bool isValidEnt(cl_entity_s *ent)
{
	if(ent && (ent != pEngfuncs->GetLocalPlayer()) && !(ent->curstate.effects & EF_NODRAW) && ent->player && !ent->curstate.spectator && ent->curstate.solid && !(ent->curstate.messagenum < pEngfuncs->GetLocalPlayer()->curstate.messagenum))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//==============================================================================


//==============================================================================
int GetTeamByModel(char *mdl)
{
	if( strstr(mdl,"leet")		||
		strstr(mdl,"terror")	||
		strstr(mdl,"arctic")	||
		strstr(mdl,"guerilla") )
	{
		return 0;
	}

	    if( strstr(mdl,"gign")	||
		strstr(mdl,"gsg9")	||
		strstr(mdl,"sas")	||
		strstr(mdl,"urban") ||
		strstr(mdl,"vip") )
	{
		return 1;
	}

	return -1;
}
//==============================================================================


//==============================================================================
void SolidModel(void) //by RobotFood
{  
   glDisable(GL_TEXTURE_2D);
   glBegin(GL_POLYGON);
   glEnd(); 
} 
//==============================================================================


//==============================================================================
int Vis(float* xfrom,float* xto)
{
	int pathtest; 
	pmtrace_t tr; 
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 ); 
	gEngfuncs.pEventAPI->EV_PlayerTrace( xfrom, xto, PM_GLASS_IGNORE, me.entindex, &tr ); 
	pathtest = (tr.fraction == 1.0); 
	return pathtest; 
}
//==============================================================================


//==============================================================================
bool Vis2(int ax)
{
	if(Vis(me.pmEyePos,vPlayers[ax].origin())) return true;

	return false;
}
//==============================================================================


//==============================================================================
#define PTR_CREATE(glPtr) FARPROC pOrig_##glPtr;
PTR_CREATE(glBegin)
PTR_CREATE(glBlendFunc)
PTR_CREATE(glClear)
PTR_CREATE(glEnable)
PTR_CREATE(glEnd)
PTR_CREATE(glPopMatrix)
PTR_CREATE(glPushMatrix)
PTR_CREATE(glVertex2f)
PTR_CREATE(glVertex3f)
PTR_CREATE(glVertex3fv)
PTR_CREATE(glViewport)
PTR_CREATE(wglSwapBuffers)
PTR_CREATE(glRotatef)
PTR_CREATE(glFrustum)
//==============================================================================


//==============================================================================
#define PTR_STORE(function) \
	pOrig_##function = (FARPROC)(GetProcAddress(GetModuleHandle("opengl32.dll"),#function)); \
	if (*pOrig_##function == NULL) \
		pResult = false;
bool StoreOffsets(void)
{
	bool pResult=true;
	PTR_STORE(glBegin)			//else logme("Found glBegin at 0x%x"		,*pOrig_glBegin);
	PTR_STORE(glBlendFunc)		//else logme("Found glBlendFunc at 0x%x"	,*pOrig_glBlendFunc);
	PTR_STORE(glClear)			//else logme("Found glClear at 0x%x"		,*pOrig_glClear);
	PTR_STORE(glEnable)			//else logme("Found glEnable at 0x%x"		,*pOrig_glEnable);
	PTR_STORE(glEnd)			//else logme("Found glEnd at 0x%x"			,*pOrig_glEnd);
	PTR_STORE(glPopMatrix)		//else logme("Found glPopMatrix at 0x%x"	,*pOrig_glPopMatrix);
	PTR_STORE(glPushMatrix)		//else logme("Found glPushMatrix at 0x%x"	,*pOrig_glPushMatrix);
	PTR_STORE(glVertex2f)		//else logme("Found glVertex2f at 0x%x"		,*pOrig_glVertex2f);
    PTR_STORE(glVertex3f)		//else logme("Found glVertex3f at 0x%x"		,*pOrig_glVertex3f);
	PTR_STORE(glVertex3fv)		//else logme("Found glVertex3fv at 0x%x"	,*pOrig_glVertex3fv);
	PTR_STORE(glViewport)		//else logme("Found glViewport at 0x%x"		,*pOrig_glViewport);
	PTR_STORE(wglSwapBuffers)	//else logme("Found wglSwapBuffers at 0x%x"	,*pOrig_wglSwapBuffers);
	PTR_STORE(glRotatef)		//else logme("Found glRotatef at 0x%x"		,*pOrig_glRotatef);
	PTR_STORE(glFrustum)		//else logme("Found glFrustum at 0x%x"		,*pOrig_glFrustum);
	return pResult;
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glBegin(GLenum mode)
{ 
	glGetFloatv(GL_CURRENT_COLOR, curcol); //开枪火焰及烟雾效果
//=====================================================================
//屏蔽人物模型
//=====================================================================
	if((cvar.niggermodels && mode==GL_TRIANGLE_STRIP)||(cvar.nightmode && mode==GL_POLYGON))
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		cl_entity_s *pEnt=oEngStudio.GetCurrentEntity();
	if(cvar.wireframewall || cvar.wiremodels)
		{
			if(cvar.wireframewall && mode==GL_POLYGON)
			{
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1);
				glColor3f(0.0f,0.8f,1.0f);
			}
			else if(cvar.wiremodels && (mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN))
			{
				glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
				glLineWidth(1);
			}
			else
				glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		}
	if (cvar.noall && me.alive && cvar.way_on == 1 && (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN))
		nodrawentities = true;
	else
		nodrawentities = false;
	switch (cvar.wall) 
	{
	case 0:
		break;
	case 1: { if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN) (*glDisable)(GL_DEPTH_TEST);break;}
	case 2:
		if (!(mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			float curcolor[4];
			glGetFloatv(GL_CURRENT_COLOR, curcolor);
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(GL_ONE, GL_SRC_ALPHA_SATURATE);
			glColor4f(curcolor[0], curcolor[1], curcolor[2], colorList.get(10)->onebased_a );
		}
		break;
	case 3:
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(1,2),(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			glColor4f(0.8f,0.7f,0.6f,0.7f);
			glClearColor(0.7f,0.9f,0.6f,0.8f);	
		}
		break;
	case 4:	
		if  (!(mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			glColor4f(0.6f,0.5f,0.4f,0.5f);			
			glClearColor(0.5f,0.6f,0.6f,0.4f);
		}
		break;	
	 case 5:
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(GL_ONE, GL_SRC_ALPHA_SATURATE);
			glColor4f(0.9f,0.6f,0.6f,0.5f);			
			glClearColor(0.7f,0.6f,0.5f,0.7f);
		}	
		break;
	case 6:	
		if  (!(mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			glColor4f(0.6f,0.7f,0.6f,0.6f);			
			glClearColor(0.6f,0.6f,0.8f,0.7f);
		}
		break;
	case 7:
		if  (!(mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
		{
			ColorEntry* c = colorList.get(16);
			glColor3f( c->onebased_r, c->onebased_g, c->onebased_b);
			float curcolor[4];
			glGetFloatv(GL_CURRENT_COLOR, curcolor);
			glDisable(GL_DEPTH_TEST);
			glEnable (GL_BLEND);
			glBlendFunc(1,2),(GL_DEPTH_TEST, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
			glColor4f(curcolor[0],curcolor[1],curcolor[2], .673f);
			glClearColor(0.5f,0.6f,0.7f,0.6f);
		}
		break;
	case 8:   // Doesnt mess up the gun when used like xqz style does 
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS)) 
		{ 
			ColorEntry* c = colorList.get(21); 
			glColor3f( c->onebased_r, c->onebased_g, c->onebased_b); 
			float curcolor[4]; 
			glGetFloatv(GL_CURRENT_COLOR, curcolor); 
			glDisable(GL_DEPTH_TEST); 
			glEnable (GL_BLEND); 
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE); 
			glColor4f(curcolor[0], curcolor[1], curcolor[2], 0.2f ); 
//			cvar.trans=0; 
		} 
		else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN) 
		{ 
			glEnable(GL_DEPTH_TEST); 
			glDisable(GL_BLEND); 
		 } 
		break;
	case 9: 
		if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
			glDisable(GL_DEPTH_TEST);
		else if(mode == GL_POLYGON) 
			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);    
		break;
	case 10:   // Doesnt mess up the gun when used like xqz style does 
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS)) 
		{ 
			ColorEntry* c = colorList.get(1); 
			glColor3f( c->onebased_r, c->onebased_g, c->onebased_b); 
			float curcolor[4]; 
			glGetFloatv(GL_CURRENT_COLOR, curcolor); 
			glDisable(GL_DEPTH_TEST); 
			glEnable (GL_BLEND); 
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE); 
			glColor4f(curcolor[0], curcolor[1], curcolor[2], 0.2f ); 
//			cvar.trans=0; 
		} 
		else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN) 
		{ 
			glEnable(GL_DEPTH_TEST); 
			glDisable(GL_BLEND); 
		 } 
		break;
	case 11:   // Doesnt mess up the gun when used like xqz style does 
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS)) 
		{ 
			ColorEntry* c = colorList.get(22); 
			glColor3f( c->onebased_r, c->onebased_g, c->onebased_b); 
			float curcolor[4]; 
			glGetFloatv(GL_CURRENT_COLOR, curcolor); 
			glDisable(GL_DEPTH_TEST); 
			glEnable (GL_BLEND); 
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE); 
			glColor4f(curcolor[0], curcolor[1], curcolor[2], 0.2f ); 
//			cvar.trans=0; 
		} 
		else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN) 
		{ 
			glEnable(GL_DEPTH_TEST); 
			glDisable(GL_BLEND); 
		 } 
		break;
	case 12:   // Doesnt mess up the gun when used like xqz style does 
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS)) 
		{ 
			ColorEntry* c = colorList.get(23); 
			glColor3f( c->onebased_r, c->onebased_g, c->onebased_b); 
			float curcolor[4]; 
			glGetFloatv(GL_CURRENT_COLOR, curcolor); 
			glDisable(GL_DEPTH_TEST); 
			glEnable (GL_BLEND); 
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE); 
			glColor4f(curcolor[0], curcolor[1], curcolor[2], 0.2f ); 
//			cvar.trans=0; 
		} 
		else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN) 
		{ 
			glEnable(GL_DEPTH_TEST); 
			glDisable(GL_BLEND); 
		 } 
		break;
	case 13:   // Doesnt mess up the gun when used like xqz style does 
		if (!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS)) 
		{ 
			ColorEntry* c = colorList.get(24); 
			glColor3f( c->onebased_r, c->onebased_g, c->onebased_b); 
			float curcolor[4]; 
			glGetFloatv(GL_CURRENT_COLOR, curcolor); 
			glDisable(GL_DEPTH_TEST); 
			glEnable (GL_BLEND); 
			glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE); 
			glColor4f(curcolor[0], curcolor[1], curcolor[2], 0.2f ); 
//			cvar.trans=0; 
		} 
		else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN) 
		{ 
			glEnable(GL_DEPTH_TEST); 
			glDisable(GL_BLEND); 
		 } 
		break;
	}	

	if (mode==GL_QUADS && cvar.nosky)
	{
		bDrawingSky=true;
	}
	else
	{
		bDrawingSky=false;
	}

    if ( mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN )
    {
        routeDraw = true;
    }
		if(cvar.lambert == 2)
		{
			if(mode == GL_TEXTURE_ENV || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			}
		}
	if (cvar.sniper &&  (curcol[0]==curcol[1] && curcol[0]==curcol[2]) && (curcol[0]==0.0 || curcol[0]==1.0)
			&& me.inZoomMode && curcol[3]==1.0)
		bDrawingScope = true;	
	else
		bDrawingScope = false;	

	if (bDrawingScope) glColor4f(curcol[0],curcol[1],curcol[2],0.0f);
  __asm
	{
		push mode
		call dword ptr [pOrig_glBegin]
	}
	if (me.alive && cvar.way_on == 1 && (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN))
		nodrawentities = true;
	else
		nodrawentities = false;

	if (mode==GL_QUADS)
	{		 
		if ((curcol[0]==curcol[1])&&(curcol[0]==curcol[2]) && (curcol[0]!=0.0) && (curcol[0]!=1.0) && 
			cvar.flash==1)
			bDrawingSmoke = true;	
	}
	else 
	{
		bDrawingSmoke = false; 		
	}

	polyCoords = 0;

  __asm
	{
		push mode
		call dword ptr [pOrig_glBegin]
	}
  if(cvar.chamz)
	{
		cl_entity_s * ent = oEngStudio.GetCurrentEntity();
		if(isValidEnt(ent))
		{
			if(ent->player)
			{ 
				SolidModel();
			} 
		}

		if(mode == GL_TRIANGLES || mode == GL_POLYGON)
		{
			glEnable(GL_TEXTURE_2D);
		}
	}

  __asm
	{
		push mode
		call dword ptr [pOrig_glBegin]
	}
}
//==============================================================================

//==============================================================================
void Excellentsetting()
{
	hlexec("net_graph 0;cl_showfps 1");
	hlexec("cl_bob 0;cl_bobup 0;cl_bobcycle 0.0");//关闭手臂摆动
	hlexec("gl_zmax 0");//可看及射穿墙壁,gl_zmax 100看穿墙壁 gl_zmax 3600让墙壁恢复原状
	hlexec("ex_interp 0.02");
	hlexec("gl_dither 1"); // 只渲染可见目标
	hlexec("gl_cull 1"); // 只渲染可见目标
	hlexec("gl_keeptjunctions 0"); // 显示纹理的裂纹
//	hlexec("gl_max_size 4"); //this is hud shit--------------------------------------------------------------
	hlexec("gl_clear 0"); // 纹理质量
	hlexec("d_spriteskip 0"); // 不清楚，似乎能提高性能
	hlexec("r_dynamic 0"); // 固定动态光源
	hlexec("r_mirroralpha 0"); // 关闭反射图片
	hlexec("gl_palette_tex 0"); // 使纹理平滑
	hlexec("gl_wireframe 0");
	hlexec("gl_round_down 4"); // 纹理降级等级固定(1-99越高质量越低)
	hlexec("cl_weather 0"); // 关闭天气(de_aztec)
	hlexec("max_shells 0"); // 关闭子弹退镗
	hlexec("gl_texturemode \"GL_LINEAR_MIPMAP_NEAREST\""); // 设置纹理模式
	hlexec("gl_wateramp 0"); // 不显示水波
	hlexec("fastsprites 2"); // 烟雾细节(0-3，数值越高质量越低)，对白云没有影响，但对手雷爆炸冒出的烟雾有影响
	hlexec("cl_shadows 0"); // 关闭玩家阴影
//	hlexec("gl_picmip 3"); // this is also hud shit-------------------------------------------------------------
	hlexec("gl_playermip 3"); //人物模型贴图效果(0-3)
	hlexec("joystick 0"); // 关闭游戏操纵杆
	hlexec("max_shells 0"); // 不显示弹壳
	hlexec("max_smokepuffs 0"); // 关闭烟雾扩散效果(违规命令，比赛勿用)
	hlexec("brightness 30"); // 允许使用CPU MMX指令集
	hlexec("gamma 3"); // 允许使用CPU MMX指令集
	hlexec("r_mmx 1"); // 允许使用CPU MMX指令集
	hlexec("r_decals 0"); // 设定血花或者墙上的弹孔数目
	hlexec("cl_crosshair_color \"50 250 50\"");
	hlexec("fps_max 999"); 
	hlexec("fps_modem 0");
	hlexec("developer 1"); //与FPS提高有关
	hlexec("mat_vsync 0");//垂直同步
	hlexec("cl_updaterate 99999");
	hlexec("cl_cmdrate 99999");
	hlexec("rate 99999");
}
//==============================================================================
void __stdcall pHooked_glBlendFunc(GLenum sfactor,  GLenum dfactor)
{
  if (oglSubtractive)
	{
		__asm
		{ 
			push 0x0303 // GL_ONE_MINUS_SRC_ALPHA
			push 0x0302	// GL_SRC_ALPHA
			call dword ptr [pOrig_glBlendFunc]
		}
	}
	else
	{
		__asm
		{ 
			push dfactor
			push sfactor
			call dword ptr [pOrig_glBlendFunc]
		}
	}
}
//==============================================================================

//==============================================================================
void __stdcall pHooked_glClear(GLbitfield mask)
{

	if ((mask==GL_DEPTH_BUFFER_BIT) && cvar.nosky)	// clear buffer if nosky is 1 otherwise sky is fucked
	{											// also nescessary if u add e.g. asus wh
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
		glClear(mask);
	}

   __asm
	{
		push mask
		call dword ptr [pOrig_glClear]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glEnable(GLenum cap)
{
	
  __asm
	{
		push cap
		call dword ptr [pOrig_glEnable]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glEnd(void)
{

	__asm
	{
		call dword ptr [pOrig_glEnd]
	}
	bPolygon = false;
	polyCoords = 0;

}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glPopMatrix(void)
{
//==============================================================================
	iCurrentTeam = -1;

    cl_entity_t *pEnt;
    // check to see if we are in a studio rendering phase, and if a player is being rendered, get the head position from the bones
    pEnt = oEngStudio.GetCurrentEntity();
    if(pEnt)
		if (pEnt->player)
			Aimbot.CalculateHitbox(pEnt);

	__asm
	{
		call dword ptr [pOrig_glPopMatrix]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glPushMatrix(void)
{

	cl_entity_t *pEnt;
	pEnt = oEngStudio.GetCurrentEntity();
	if(pEnt && pEnt->player)
	{
		if(pEnt->index > 0)
		{
			hud_player_info_t EntInfo;
			pEngfuncs->pfnGetPlayerInfo( pEnt->index, &EntInfo );
			iCurrentTeam = GetTeamByModel( EntInfo.model );
		}
		else
		{
			iCurrentTeam = -1;
		}
	}
	else 
	{
		iCurrentTeam = -1;
	}

	__asm
	{
		call dword ptr [pOrig_glPushMatrix]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glVertex2f(GLfloat x, GLfloat y)
{


  __asm
	{
		push y
		push x
		call dword ptr [pOrig_glVertex2f]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	
	if (cvar.lambert)
	{
		glColor3f( 1.0f,1.0f,1.0f );
	}

	cl_entity_s *ent = oEngStudio.GetCurrentEntity();
	if(cvar.chamz)
	{
		if(isValidEnt(ent))
		{
			VISIBLE = Vis2(ent->index);
			if(ent->index == me.entindex)
			{
				VISIBLE = false;
			}

			if( VISIBLE && iCurrentTeam == 0 )
			{
				glColor3f( 0.0f,1.0f,0.0f );
			}
			else if( !VISIBLE && iCurrentTeam == 0 )
			{
				glColor3f( 1.0f,0.0f,0.0f );
			}
			else if( VISIBLE && iCurrentTeam == 1 )
			{
				glColor3f( 0.0f,1.0f,0.0f );
			}
			else if( !VISIBLE && iCurrentTeam == 1 )
			{
				glColor3f( 0.0f,0.0f,1.0f );
			}
		}
	}
  __asm
	{
		push z
		push y
		push x
		call dword ptr [pOrig_glVertex3f]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glVertex3fv(const GLfloat *var)
{
	if (bDrawingSmoke) return;

	bDrawnWorld=true; 

	if(var && bPolygon) 
	{ 
		polyArray[polyCoords][0] = var[0]; 
		polyArray[polyCoords][1] = var[1]; 
		polyArray[polyCoords++][2] = var[2]; 
	}  

	if (bDrawingSky==true && cvar.nosky && var[2]>3000)
	{
		return;
	}

  __asm
	{
		push var
		call dword ptr [pOrig_glVertex3fv]
	}
}
//==============================================================================
void __stdcall pHooked_glFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
//	if(hHackOptions.Features.bCheatActivado)
	{
		//bool modified=false;
		if(cvar.invertedvision)
		{
			if(cvar.camera)
			{
				top=zNear*tan((50-(20+(cvar.camerazoom*-1))*5)*PI/360);
				bottom=-top;
				left=bottom*4/3;
				right=top*4/3;
				//modified=true;
			}
			else
			{
				top=zNear*tan((50-(25)*5)*PI/360);
				bottom=-top;
				left=bottom*4/3;
				right=top*4/3;
				//modified=true;
			}
		}
		if(cvar.camera) //&& !modified)
		{
			top=zNear*tan((50-cvar.camerazoom*5)*PI/360);
			bottom=-top;
			left=bottom*4/3;
			right=top*4/3;
			//modified=true;
		}
		if(cvar.alienvision && me.alive/* && !modified*/)
		{
			top=zNear*tan((50-(-21)*5)*PI/360);
			bottom=-top;
			left=bottom*4/3;
			right=top*4/3;
		}
	}

	glFrustum(left,right,bottom,top,zNear,zFar);
}

//==============================================================================
void __stdcall pHooked_glViewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
	
	viewport_c++;

	if(viewport_c == 1 && cvar.antiwallblock)
	{
		BlockFunc();
	}

	static bool bStart=false;
	if (!bStart)
	{
		
		gEngfuncs.pfnConsolePrint( "\nViking Wpt Multihack" );
		gEngfuncs.pfnConsolePrint( "\n      Creadits: FT5, 85, ASD and 17 " );
		gEngfuncs.pfnConsolePrint( "\n      Code by: MYTH, CB, GYJ and 007" );
		gEngfuncs.pfnConsolePrint( "\n      Coder: eVOL" );
		gEngfuncs.pfnConsolePrint( "\n      Www.cstrikenoobhackers.com\n" );
		gEngfuncs.pfnConsolePrint( "                               03-05-2017\n\n\n\n\n\n\n\n\n" );
		gEngfuncs.pfnClientCmd("fs_lazy_precache 1");
		gEngfuncs.pfnClientCmd("toggleconsole");
		Excellentsetting();
		bStart=true;
	}
//=================================================================================
//窗口模式
    GLint smallx = (width-200)/2, smally = (height-150)/2;
	GLsizei smallwidth = 200, smallheight = 150;

	if(cvar.smallview && GL_DEPTH_BUFFER_BIT && cvar.way_on == 1 && me.alive)
	{
		__asm
		{
		    push smallheight
		    push smallwidth
		    push smally
		    push smallx
		    call dword ptr [pOrig_glViewport]
		}
	}
	else
	{
		__asm
		{
		    push height
		    push width
		    push y
		    push x
		    call dword ptr [pOrig_glViewport]
		}
	}
    if(cvar.smallview==0)
	{
		__asm
		{
		    push height
		    push width
		    push y
		    push x
		    call dword ptr [pOrig_glViewport]
		}
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_wglSwapBuffers(HDC hDC)
{
	viewport_c = 0;
	nAngles = 0;
	bDrawnWorld=false;
	bStartedDrawingEnts=false;

  __asm
	{
		push hDC
		call dword ptr [pOrig_wglSwapBuffers]
	}
}
//==============================================================================


//==============================================================================
void __stdcall pHooked_glRotatef ( GLfloat angle, GLfloat x,  GLfloat y,  GLfloat z ) 
{ 
	float angles[3];

	nAngles++;

	if( nAngles == 4 )
		angles[0] = angle;
	if( nAngles == 5 )
		angles[1] = angle;
	if( nAngles == 3 )
	{
		angles[2] = angle;
		angle = 0.0;
	}

  __asm
	{
		push z
		push y
		push x
		push angle
		call dword ptr [pOrig_glRotatef]
	}
}
//==============================================================================


//==============================================================================
#define HOOK_APPLY(func_name) if(!lstrcmp(lpProcName,#func_name)) { \
				*pProc = (FARPROC) &pHooked_##func_name; \
				return; }
void ApplyOpenGLHook (FARPROC* pProc, LPCSTR lpProcName)
{
	HOOK_APPLY(wglSwapBuffers)
	HOOK_APPLY(glRotatef)
	HOOK_APPLY(glBegin)
	HOOK_APPLY(glBlendFunc)
	HOOK_APPLY(glClear)
	HOOK_APPLY(glEnable)
	HOOK_APPLY(glEnd)
	HOOK_APPLY(glPopMatrix)
	HOOK_APPLY(glPushMatrix)
	HOOK_APPLY(glVertex2f)
	HOOK_APPLY(glVertex3f)
	HOOK_APPLY(glVertex3fv)
	HOOK_APPLY(glViewport)
	HOOK_APPLY(glFrustum);
}
//===================================================================================

//===================================================================================
