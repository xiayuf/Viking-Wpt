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
#include "bind.h"
#include "common/net_api.h"
#include "players.h"
#include "xorstr.h"
#include "menu.h"
#include "misc\defs.h"
#include <gl/gl.h>
#include "waypoint.h"

WAYPOINT waypoint;

#define Dbg_Echo(a) 

#define POINT_MAX 999
#define GROUP_MAX 9

struct WAY
{
	float  point[3];
};

WAY ways[GROUP_MAX][POINT_MAX];
void FixupAngleDifference(usercmd_t* usercmd,float forward,float right,float up);
void FixupAngleDifference(usercmd_t* usercmd);
#define PI 3.14159265359
double pisobre180 = (PI/180.0f);
double _180sobrepi = (180.0f/PI);
extern char hackdir[256];
//**********************************************************************************************************************************

void WAYPOINT::getFileName(void)
{
	char buf[256];
	strcpy(buf,gEngfuncs.pfnGetLevelName());
	strcpy(buf,strstr(buf,"/"));
	strcpy(map_name,buf+1);
	*strstr(map_name,".")=0;
	if (me.team!=1)
	{
	strcpy(strstr(buf,"."),/*.ct*/XorStr<0xB5,4,0x9B6EA184>("\x9B\xD5\xC3"+0x9B6EA184).s);
	}
	if (me.team==1)
	{
	strcpy(strstr(buf,"."),/*.t*/XorStr<0x22,3,0x423C6B65>("\x0C\x57"+0x423C6B65).s);
	}
	strcpy(file_name,hackdir);
	strcat(file_name,/*wpt*/XorStr<0xDA,4,0x4839124A>("\xAD\xAB\xA8"+0x4839124A).s);
	strcat(file_name,buf);
}

//**********************************************************************************************************************************

void WAYPOINT::vectorAngles(const float * forward, float * angles)
{
	float temp = 0.0, yaw = 0.0;
	
	if (forward[1] == 0.0f && forward[0] == 0.0f)
	{
		yaw = 0.0f;
	}
	else
	{
		yaw = (float)(atan2(forward[1], forward[0]) * _180sobrepi);

		if (yaw < 0.0f)
		{
			yaw += 360.0f;
		}
	}
	angles[0] = 0.0f;
	angles[1] = yaw;
	angles[2] = 0.0f;
}

//**********************************************************************************************************************************

bool WAYPOINT::pathFree(float* from,float* to)
{
	float newFrom[3];
	float newTo[3];
	newFrom[0] = from[0];
	newFrom[1] = from[1];
	newFrom[2] = from[2]-20;
	newTo[0] = to[0];
	newTo[1] = to[1];
	newTo[2] = to[2]-20;
	pmtrace_t tr; 
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 ); 
//	gEngfuncs.pEventAPI->EV_PlayerTrace( newFrom, newTo, PM_NORMAL, -1, &tr );
//	int iEntity = gEngfuncs.pEventAPI->EV_IndexFromTrace ( &tr );
//	cl_entity_s* ent = gEngfuncs.GetEntityByIndex ( iEntity );
//	if(ent->index == me.ent->index) return true;

	//ignorar puertas y ventanas (IMPORTANTE!)
	gEngfuncs.pEventAPI->EV_PlayerTrace( newFrom, newTo, PM_GLASS_IGNORE, -1, &tr );// me.ent->index
	return  (tr.fraction == 1.0); 
}

//**********************************************************************************************************************************

/*
#define PM_NORMAL			0x00000000
#define PM_STUDIO_IGNORE	0x00000001		// Skip studio models
#define PM_STUDIO_BOX		0x00000002		// Use boxes for non-complex studio models (even in traceline)
#define PM_GLASS_IGNORE		0x00000004		// Ignore entities with non-normal rendermode
#define PM_WORLD_ONLY		0x00000008		// Only trace against the world

// Values for flags parameter of PM_TraceLine
#define PM_TRACELINE_ANYVISIBLE		0
#define PM_TRACELINE_PHYSENTSONLY	1


bool WAYPOINT::pathFree(float* from,float* to){

	float newFrom[3];
	float newTo[3];
	newFrom[0] = from[0];
	newFrom[1] = from[1];
	newFrom[2] = from[2]-20;
	newTo[0] = to[0];
	newTo[1] = to[1];
	newTo[2] = to[2]-20;
	pmtrace_t* vis = gEngfuncs.PM_TraceLine(newFrom, newTo, 0, 2, -1);    
	return (vis->fraction == 1.0f);
}*/

//**********************************************************************************************************************************

float WAYPOINT::getDist(float* point)
{
	float a = point[0] - me.pmEyePos[0];
	float b = point[1] - me.pmEyePos[1];
	float c = point[2] - me.pmEyePos[2];
	return sqrt(a*a + b*b + c*c);
}

//**********************************************************************************************************************************

/*
void WAYPOINT::aimAngleCalc(float* point,float* viewangles)
{
	float src[3],dst[3];
	float dx,dy,b;
	src[0]=me.pmEyePos[0];
	src[1]=me.pmEyePos[1];
	dst[0]=point[0];
	dst[1]=point[1];
	dx = dst[0]-src[0];
	dy = dst[1]-src[1];
	if( dx==0&&dy==0 ){ b=0; }
	else if( dx==0 && dy>0  )b=90;
	else if( dx==0 && dy<0  )b=-90;
	else if( dx>0  && dy==0 )b=0;
	else if( dx<0  && dy==0 )b=180;
	else if( dx>0  && dy>0  )b=atan(dy/dx)*_180sobrepi;
	else if( dx>0  && dy<0  )b=-atan(-dy/dx)*_180sobrepi;
	else if( dx<0  && dy>0  )b=180-(atan(dy/-dx)*_180sobrepi);
	else if( dx<0  && dy<0  )b=-(180-(atan(-dy/-dx)*_180sobrepi));
	viewangles[0]=0;
	viewangles[1]=b;
	viewangles[2]=0;
}*/

//**********************************************************************************************************************************

void WAYPOINT::aimAt(float* point)
{
	float dodgeDir[2], dodgeAngles[3];
	dodgeDir[0] = point[0] - me.pmEyePos[0];
	dodgeDir[1] = point[1] - me.pmEyePos[1];
	vectorAngles(dodgeDir,dodgeAngles);
	
	float angles[3];
	float aimAngle[3];
//	aimAngleCalc(point,angles);

	float angle = dodgeAngles[1] - mainViewAngles[1];

	angles[0]=0;
	angles[1]=angle;
	angles[2]=0;
	aimAngle[0]=angles[0];
	aimAngle[1]=angles[1];
	aimAngle[2]=angles[2];
	gEngfuncs.SetViewAngles(aimAngle);
}

//**********************************************************************************************************************************

/*
float WAYPOINT::getAngle(float* point)
{
	float angles[3];
	float angle;
	aimAngleCalc(point,angles);
	angles[0]=0;
	angles[2]=0;
//	angle=angles[1]-me.viewAngles[1];// si se usa spin modificado
	angle=angles[1]-mainViewAngles[1];
	if(angle>=180)angle-=360;
	if(angle<=-180)angle+=360;
    return angle;
}*/

//**********************************************************************************************************************************

float WAYPOINT::getAngle(float* point)
{
	float dodgeDir[2], dodgeAngles[3];
	dodgeDir[0] = point[0] - me.pmEyePos[0];
	dodgeDir[1] = point[1] - me.pmEyePos[1];
	vectorAngles(dodgeDir,dodgeAngles);
	float angle = dodgeAngles[1] - mainViewAngles[1];
	while(angle<0)   { angle+=360; }
	while(angle>360) { angle-=360; }
	return angle;
}

//**********************************************************************************************************************************

float WAYPOINT::MoveAnglesSub(float* point)
{
	float moveDir[2], moveAngles[3];
	float pointDir[2], pointAngles[3];
	float angles_sub;
	moveDir[0] = me.pmVelocity[0];
	moveDir[1] = me.pmVelocity[1];
	pointDir[0] = point[0] - me.pmEyePos[0];
	pointDir[1] = point[1] - me.pmEyePos[1];
	vectorAngles(moveDir,moveAngles);
	vectorAngles(pointDir,pointAngles);
	angles_sub = pointAngles[1] - moveAngles[1];
	if(angles_sub > 180.0) angles_sub -= 360.0;
	if(angles_sub < -180.0) angles_sub += 360.0;
	if(angles_sub < 0.0) angles_sub *= -1;
	return angles_sub;
}

//**********************************************************************************************************************************

float WAYPOINT::relativeSpeed(void)
{
	float speed = sqrt(me.pmVelocity[0] * me.pmVelocity[0] + me.pmVelocity[1] * me.pmVelocity[1]);
	return speed;
}

//**********************************************************************************************************************************

void WAYPOINT::MoveTo(float* point, float* forwardmove, float* sidemove)
{
	float units_per_second = 250.0f;
	if(cvar.nocel&& (me.pmFlags & FL_ONGROUND)){

		float speed = relativeSpeed();
//		float aceleration = speed-me.weaponspeed;
		float a= getDist(point);
		float b=0;
		float ramp=0;
		if (me.pmEyePos[2]>point[2]) b=me.pmEyePos[2]-point[2];
		if(b>0) ramp= sqrt(a*a+b*b);

	   units_per_second -= ramp;
//	   units_per_second -= aceleration;
	}
	fmove = (float) cos(getAngle(point)*(pisobre180))*units_per_second;
	smove = (float)-sin(getAngle(point)*(pisobre180))*units_per_second;
	*forwardmove = fmove;
	*sidemove    = smove;
}

//**********************************************************************************************************************************

void WAYPOINT::StopMove(float* forwardmove, float* sidemove)
{
	*forwardmove = 0;
	*sidemove    = 0;
}

//**********************************************************************************************************************************

void WAYPOINT::runDirection(int cur_direction,int cur_group,struct usercmd_s* usercmd)
{
	target_point=-1;
    wayLock = false;
	beforeDirection=-1;

	switch(cur_direction)
	{
	case 1:
	{
		for(int i=0;i<point_count[cur_group];i++)
		{
			if(getDist(ways[cur_group][i].point)<cvar.way_maxdist)
			{
				target_point=i;
			}
		}
		if(target_point!=-1)
		{
			if(target_point!=point_count[cur_group]-1)
			{
				if(cvar.runway_mode){

					if( relativeSpeed() < 20.0 )// se detiene por freeze, por bloqueo.
					{
						if( !pathFree(me.pmEyePos,ways[cur_group][target_point].point) )
						{
							if(cvar.auto_br==2)
							{
							freeInGroup=true;
									 return;//libre para autobq/autobr
							}

							if(cvar.runway_mode==1){
							findFirstPoint=false;isRunning=false;runContinue=true;firstGroup=-1;firstPoint=-1;
							beforeGroup=-1;
							beforeDirection=direction;
							}
							else if(cvar.runway_mode==2){
								freeInGroup=true;
									 return;//libre para autobq/autobr
							}
						}
					}

					MoveTo(ways[cur_group][target_point].point, &usercmd->forwardmove, &usercmd->sidemove);
				}
				else
				{
					if( pathFree(me.pmEyePos,ways[cur_group][target_point].point) )
						MoveTo(ways[cur_group][target_point].point, &usercmd->forwardmove, &usercmd->sidemove);
				}
			}
			else //Punto extremo (hace k salga, pero todavia falta para llegar a este punto (600 units))
			{
				findFirstPoint=false;isRunning=false;runContinue=true;beforeGroup=cur_group;
				firstGroup=-1;firstPoint=-1;wayLock = false;
				beforeDirection=direction;
			} 
		}
		else
		{
			if(cvar.way_restore)
			{
			findFirstPoint=false;isRunning=false;runContinue=true;firstGroup=-1;firstPoint=-1;
			beforeGroup=-1;
			beforeDirection=direction;
			}
			else
				freeInGroup=true;// se sale de la ruta pero queda atrapado en runDirection. (auto_direction)
		}
	}
	break;
	case 2:
	{
		for(int i=point_count[cur_group]-1;i>=0;i--)
		{
			if(getDist(ways[cur_group][i].point)<cvar.way_maxdist)
			{
				target_point=i;
			}
		}
		if(target_point!=-1)
		{
			if(target_point!=0)
			{
				if(cvar.runway_mode){

					if( relativeSpeed() < 20.0 )// se detiene por freeze, por bloqueo.
					{
						if( !pathFree(me.pmEyePos,ways[cur_group][target_point].point) )
						{
							if(cvar.auto_br==2)
							{
							freeInGroup=true;
									 return;//libre para autobq/autobr
							}

							if(cvar.runway_mode==1){
							findFirstPoint=false;isRunning=false;runContinue=true;firstGroup=-1;firstPoint=-1;
							beforeGroup=-1;
							beforeDirection=direction;
							}
							else if(cvar.runway_mode==2){
								freeInGroup=true;
									 return;//libre para autobq/autobr
							}
						}
					}

					MoveTo(ways[cur_group][target_point].point, &usercmd->forwardmove, &usercmd->sidemove);
				}
				else
				{
					if( pathFree(me.pmEyePos,ways[cur_group][target_point].point) )
						MoveTo(ways[cur_group][target_point].point, &usercmd->forwardmove, &usercmd->sidemove);
				}
			}
			else //Punto extremo
			{
				findFirstPoint=false;isRunning=false;runContinue=true;beforeGroup=cur_group;
				firstGroup=-1;firstPoint=-1;wayLock = false;
				beforeDirection=direction;
			}  
		}
		else
		{
			if(cvar.way_restore)
			{
			findFirstPoint=false;isRunning=false;runContinue=true;firstGroup=-1;firstPoint=-1;
			beforeGroup=-1;
			beforeDirection=direction;
			}
			else
				freeInGroup=true;// se sale de la ruta pero queda atrapado en runDirection. (auto_direction)
		}
	}
	break;
	}
}

//**********************************************************************************************************************************

void WAYPOINT::FindTarget(struct usercmd_s* usercmd)
{
	if(!me.alive){
		findFirstPoint=false;isRunning=false;runContinue=false;beforeGroup=-1;firstGroup=-1;firstPoint=-1; 
		beforeDirection=-1; 
		freeInGroup=false;
		return;
	}
	if(cur_way_mode==1){
		if(Aimbot.iTarget!=-1){
			findFirstPoint=false;isRunning=false;runContinue=false;beforeGroup=-1;firstGroup=-1;firstPoint=-1; 
			beforeDirection=-1; 
			freeInGroup=false;
			return;
		}
	}
	if(!cvar.way_on){
		findFirstPoint=false;isRunning=false;runContinue=false;beforeGroup=-1;firstGroup=-1;firstPoint=-1; 
		beforeDirection=-1; 
		freeInGroup=false;
		return;
	}

	///////////////////////
	freeInGroup=false;
	runToPoint=false;
	/////////////////////////

	if(cvar.way_lock == 0) { wayLock = false; }

	float nearDist, minAngles;

	//if(firstGroup==-1 && firstPoint==-1  && !isRunning)
	if(!findFirstPoint && !isRunning)
	{
		firstDist=0;
		nextDist=0;

		/////////////////////////////////

		if(wayLock)
		{
			nearDist=10000.0f;

			if(point_count[cvar.way_lock]>0)
			{
				for(int i=0;i<point_count[cvar.way_lock];i++)
				{
					if(getDist(ways[cvar.way_lock][i].point)<nearDist){
						if(pathFree(me.pmEyePos,ways[cvar.way_lock][i].point))
						{
							firstGroup=cvar.way_lock;
							firstPoint=i;
							nearDist=getDist(ways[firstGroup][firstPoint].point);
						}
					}
				}
			}			
		}
		else
		{
			nearDist=cvar.way_neardist;
			minAngles = cvar.way_findangle;
			//

			if(!runContinue)
			{
				for(int i=1;i<=group_count;i++)
				{
					if(point_count[i]>0)
					{
						for(int j=0;j<point_count[i];j++)
						{
							if(getDist(ways[i][j].point)<nearDist && getDist(ways[i][j].point)>cvar.way_mindist){
								if(getAngle(ways[i][j].point)<cvar.way_fov && getAngle(ways[i][j].point)>-cvar.way_fov){
									if(pathFree(me.pmEyePos,ways[i][j].point)){
										firstGroup=i;
										firstPoint=j;
										nearDist=getDist(ways[firstGroup][firstPoint].point);
									}
								}
							}
						}
					}
				}

				//- Hace posible el avance hacia adelante (direction=1)
				if(firstPoint != 0 && firstPoint != point_count[firstGroup]-1)
				{
					firstDist=getDist(ways[firstGroup][firstPoint].point);
					nextDist=getDist(ways[firstGroup][firstPoint+1].point);
				}
			}
			else //runContinue: buscador de puntos
			{
					
				for(int i=1;i<=group_count;i++){
							
					if(i!=beforeGroup && point_count[i]>0)
					{
						for(int j=0;j<point_count[i];j++){
							if(getDist(ways[i][j].point)<nearDist){
								if(MoveAnglesSub(ways[i][j].point) < minAngles){
									if(pathFree(me.pmEyePos,ways[i][j].point))
									{
										firstGroup=i;
										firstPoint=j;
										minAngles=MoveAnglesSub(ways[firstGroup][firstPoint].point);
									}
								}
							}
						}
					}

					if(firstGroup==-1 && firstPoint==-1){ beforeGroup=-1;} // da la oportunidad de seguir por la misma ruta
					                                                       // deberia tambien ser la misma direccion ¿¿¿¿¿
				}

				//- Hace posible el avance hacia adelante (direction=1)
				if(firstPoint != 0 && firstPoint != point_count[firstGroup]-1)
				{
					firstDist=getDist(ways[firstGroup][firstPoint].point);
					nextDist=getDist(ways[firstGroup][firstPoint+1].point);
				}
			}
		}		
	}

	if(firstGroup!=-1 && firstPoint!=-1 && !isRunning)
	{
		direction = -1;
		findFirstPoint=true;

		// if( relativeSpeed() == 0.0 )
		runToPoint=true;
		//else
		//runToPoint=false;

		//-

		if(!pathFree(me.pmEyePos,ways[firstGroup][firstPoint].point))//h233333.png
		{
			firstGroup=-1;
			firstPoint=-1;
			findFirstPoint=false;
			isRunning=false;
			runContinue=true;
			beforeGroup=-1;
			beforeDirection=-1;
			return;// facilita auto_br
		}

		//Con esto se tratara de mover al nuevo punto
		MoveTo(ways[firstGroup][firstPoint].point, &usercmd->forwardmove, &usercmd->sidemove);

		if(cvar.way_on && cvar.way_move)
		{
			//if( relativeSpeed() < 20.0 )
			if( getAngle(ways[firstGroup][firstPoint].point)<10 && getAngle(ways[firstGroup][firstPoint].point)>-10);
			else
			{
				// para que actue el autojump
				if(cvar.autojump && Aimbot.iTarget==-1){
					if(!cvar.spin)
						aimAt(ways[firstGroup][firstPoint].point);
				}
			}
		}

		//Si la distancia del nuevo punto es cercana entonces se procede a ingresar a la ruta
		if(getDist(ways[firstGroup][firstPoint].point) < cvar.way_mindist)
		{
			if(firstPoint == 0)
				direction=1;
			else if(firstPoint == point_count[firstGroup]-1)
				direction=2;
			else if(firstDist<nextDist)
				direction=1;
			else
				direction=2;

			if(wayLock)
			{
			if((pathFree(me.pmEyePos,ways[cvar.way_lock][0].point) || pathFree(me.pmEyePos,ways[cvar.way_lock][point_count[cvar.way_lock]-1].point)))
			{
				if(getDist(ways[cvar.way_lock][0].point)<getDist(ways[cvar.way_lock][point_count[cvar.way_lock]-1].point))
					direction=1;
				else
					direction=2;
			}
			}
		}

		if(direction != -1)
			isRunning=true;
	}

	if(isRunning){
		runDirection(direction,firstGroup,usercmd);
	}

	// Se trata de mover al punto nuevo pero esta trabado (debe buscar en el mismo grupo pero salir de la ruta)
	// En este caso se tiene firstpoint, firstgroup, findfirstpoint, pero no se tiene nunca direction ni isrunning
/*	if(!isRunning && me.pmVelocity[0]==0 && me.pmVelocity[1]==0  && me.pmVelocity[2]==0){
		runDirection(beforeDirection,firstGroup,usercmd);
	}*/
}

//**********************************************************************************************************************************

void WAYPOINT::Debug()
{
	if(!me.alive) return;

	DrawGlowHudString(100, 100, 0, 255, 0, "getRunning : %d", getRunning());
	DrawGlowHudString(100, 120, 0, 255, 0, "isFreeInGroup : %d", isFreeInGroup() );
	DrawGlowHudString(100, 140, 0, 255, 0, "getFree : %d", getFree() );
	DrawGlowHudString(100, 160, 0, 255, 0, "isRunningToPoint : %d", isRunningToPoint() );
	DrawGlowHudString(100, 180, 0, 255, 0, "firstGroup : %d", firstGroup );
	DrawGlowHudString(100, 200, 0, 255, 0, "firstPoint : %d", firstPoint );
	DrawGlowHudString(100, 220, 0, 255, 0, "beforeGroup : %d", beforeGroup );
	DrawGlowHudString(100, 240, 0, 255, 0, "direction : %d", direction );
	DrawGlowHudString(100, 260, 0, 255, 0, "isRunning : %d", isRunning );
	DrawGlowHudString(100, 280, 0, 255, 0, "target_point : %d", target_point );
	DrawGlowHudString(100, 300, 0, 255, 0, "runContinue : %d", runContinue );
	DrawGlowHudString(100, 320, 0, 255, 0, "findFirstPoint : %d", findFirstPoint );
}

//**********************************************************************************************************************************

void text3d(float *origin,int r,int g,int b,const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	float vecScreen[2];
	if (NewCalcScreen(origin,vecScreen))
	{
		DrawGlowHudString(vecScreen[0],vecScreen[1],r,g,b,"%s",buf);
	}
}

//**********************************************************************************************************************************


void WAYPOINT::draw_text2(void)
{
	if(cvar.rush > 0.0) return;
	if (!me.alive) return;
	for(int i=1;i<=group_count;i++)
	{
		if(point_count[i]>0)
		{
			for(int j=0;j<point_count[i];j++)
			{
				float vecScreen[2];
				float newPoint[3];
				newPoint[0]=ways[i][j].point[0];
				newPoint[1]=ways[i][j].point[1];
				newPoint[2]=ways[i][j].point[2]-20;
				if (NewCalcScreen(newPoint,vecScreen))
				{
					char buf1[256];
					char buf2[256];
					int r=0,g=0,b=255;
					sprintf(buf1,"*",i);
					sprintf(buf2,"%d",i);
//=========================================================================================
					if(cvar.way_draw == 4)
					{
//						if(getDist(ways[i][j].point)<1000)
						{
							DrawGlowHudString((int)vecScreen[0],(int)vecScreen[1],r,g,b,buf1);
						}
						int r=255,g=0,b=0;
						if(j==0 || j==point_count[i]-1)
						{
							DrawGlowHudString((int)vecScreen[0],(int)vecScreen[1]+12,r,g,b,buf2);
						}
					}
////////////////////////////////////////////////////////////////////////////
					if(cvar.way_draw == 5)
					{
//						if(getDist(ways[i][j].point)<1000)
						{
							gDrawBoxAtScreenXY(vecScreen[0],vecScreen[1],255,0,0,255, 3);//}
						}
						if(j==0 || j==point_count[i]-1)
						{
							gDrawBoxAtScreenXY(vecScreen[0],vecScreen[1],255,0,0,255, 3);//}
						}
					}
					////////////////////////////////////////////////////////////////////////////
					if(cvar.way_draw == 6)
					{
						float from[3];
						float to[3];
						float vecScreen[2];
						from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]-40;
						to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]-40;
	//					if(pathFree(from,to))
						{
	//						if(getDist(ways[i][j].point)<1500)
							{
								drawline(from,to,255,0,0,1,0.50f);
								if (NewCalcScreen(from,vecScreen))
								{
									gDrawBoxAtScreenXY(vecScreen[0],vecScreen[1],0,0,255,255, 4);//}
								}
							}
						}
					}
					////////////////////////////////////////////////////////////////////////////
					if(cvar.way_draw == 7)
					{
						float from[3];
						float to[3];
						float vecScreen[2];
						register DWORD color;
	                    register int rad;
						float pos[3];
					    pos[0]=ways[i][j].point[0];
					    pos[1]=ways[i][j].point[1];
					    pos[2]=ways[i][j].point[2]-40;
						from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]-40;
						to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]-40;
	//					if(pathFree(from,to))
						{
	//						if(getDist(ways[i][j].point)<1500)
							{
								drawline(from,to,0,0,255,1,0.50f);
								if (NewCalcScreen(from,vecScreen))
								{
									color = 0xFF0000FF;
							        rad = 5;
									//gDrawFilledBoxAtLocation(pos,color,rad);
									gDrawFilledBoxAtLocation(pos,color,rad);//}
								}
							}
						}
					}
					if(cvar.way_draw == 8)
					{
						float from[3];
						float to[3];
						float vecScreen[2];
						register DWORD color;
	                    register int rad;
						float pos[3];
					    pos[0]=ways[i][j].point[0];
					    pos[1]=ways[i][j].point[1];
					    pos[2]=ways[i][j].point[2]-40;
						from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]+40;
						to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]-40;

	//					if(pathFree(from,to))
						{
							if(getDist(ways[i][j].point)<800)
							{
								drawline(from,to,0,0,255,1,0.50f);
								if (NewCalcScreen(from,vecScreen))
								{
									color = 0xFF0000FF;
							        rad = 5;
									//gDrawFilledBoxAtLocation(pos,color,rad);
									gDrawFilledBoxAtLocation(pos,color,rad);//}
								}
							}
						}
						from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]-40;
						to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]+40;
						drawline(from,to,255,0,0,1,0.50f);
					}
				}
			}
		}
	}
}

//**********************************************************************************************************************************

void WAYPOINT::draw_text(void)
{
	if(cvar.rush > 0.0) return;
	register DWORD color;
	register int rad;

	if(cvar.way_draw==1)
	{
		for(int i=1;i<=group_count;i++)
		{
			if(point_count[i]>0)
			{
				int count=0;

				for(int j=0;j<point_count[i];j++)
				{
					float vecScreen[2];
					float pos[3];
					pos[0]=ways[i][j].point[0];
					pos[1]=ways[i][j].point[1];
					pos[2]=ways[i][j].point[2]-40;

					if(getDist(ways[i][j].point)<800)
					{



					if(j==0 || j==point_count[i]-1)
					{
						if (NewCalcScreen(pos,vecScreen))
						{
							//if(getDist(ways[i][j].point)<500)
							{
								char buf2[256];
								sprintf(buf2,"%d",i);
								int r=0,g=0,b=255;
								DrawGlowHudString((int)vecScreen[0],(int)vecScreen[1],r,g,b,buf2);
							}
						}
					}

					else
					{

						if(cvar.way_draw==1)
						{


						count++;

						if(count==2)
						{
							count=0;


							color = 0xFF0000FF;
							rad = 5;

							gDrawFilledBoxAtLocation(pos,color,rad);


						}

						if(count && count <2)
						{
							char buf1[256];
							
							sprintf(buf1,"*",i);
							

							text3d(pos,0,0,255,buf1);

						}


						}
					}



					}
				}
			}
		}
	}
}

//**********************************************************************************************************************************

void WAYPOINT::drawline(float *from, float *to,int r,int g,int b,float d,float amplitude)
{
	int beamindex = 0;
	if(!beamindex)
		beamindex = gEngfuncs.pEventAPI->EV_FindModelIndex(/*sprites/laserbeam.spr*/XorStr<0x11,22,0x4C9F5102>("\x62\x62\x61\x7D\x61\x73\x64\x37\x75\x7B\x68\x79\x6F\x7C\x7A\x41\x4C\x0C\x50\x54\x57"+0x4C9F5102).s);
	gEngfuncs.pEfxAPI->R_BeamPoints(from,to,beamindex,d,amplitude,0,32,2,0,0,r/255,g/255,b/255);
}
//**********************************************************************************************************************************

void WAYPOINT::draw_line()
{
	if(cvar.rush > 0.0) return;

	for(int i=1;i<=group_count;i++)
	{
		if(point_count[i]>0)
		{
			for(int j=0;j<point_count[i]-1;j++)
			{
				switch((int)cvar.way_draw)
				{
					case 2:case 3:
					{
						float newPoint[3];
						newPoint[0]=ways[i][j].point[0];
						newPoint[1]=ways[i][j].point[1];
						newPoint[2]=ways[i][j].point[2]-2;

						float from[3];
						float to[3];						
						from[0]=ways[i][j].point[0];from[1]=ways[i][j].point[1];from[2]=ways[i][j].point[2]-40;
						to[0]=ways[i][j+1].point[0];to[1]=ways[i][j+1].point[1];to[2]=ways[i][j+1].point[2]-40;
						if(getDist(ways[i][j].point)<1500)
						{
							drawline(from,to,(cvar.way_draw==3 && j%2==0)?255:0,0,(cvar.way_draw==3 && j%2==0)?0:255,1,1.2f);
							
						}
					}
					break;
				}
			}
		}
	}
}

//**********************************************************************************************************************************

void WAYPOINT::groupCountCalc(void)
{
	int index_max = 0;
	for(int i=1;i<GROUP_MAX;i++)
	{
		if(point_count[i]>0)
		index_max = i;
	}
	group_count = index_max;
}

//**********************************************************************************************************************************

void WAYPOINT::save(void)
{
	getFileName();
	FILE* fp;
	fp = fopen(file_name,"w");
	if(fp==NULL)return;
	fprintf(fp,"// way File(%s)\n",map_name);
	groupCountCalc();
	for(int i=1;i<=group_count;i++)
	{
		if(point_count[i] > 0)
		{
			for(int j=0;j<point_count[i];j++)
			{
				fprintf(fp,/*P(%d)(%d):%f %f %f\n*/XorStr<0xAA,20,0x84AA6768>("\xFA\x83\x89\xC9\x87\x87\x95\xD5\x9B\x89\x91\xD3\x96\x92\xDE\x99\x9F\xDD\xB6"+0x84AA6768).s,i,j,ways[i][j].point[0],ways[i][j].point[1],ways[i][j].point[2]);
			}
		}
	}
	fclose(fp); 
}

//**********************************************************************************************************************************

void WAYPOINT::load(void)
{
	char buf[256];
	getFileName();
	for(int i=1;i<GROUP_MAX;i++)
	{
		point_count[i] = 0;
	}
	group_count = 0;
	FILE* fp;
	fp = fopen(file_name,"r");
	if(fp==NULL)return;

	while(fgets(buf,256,fp))
	{
		int tmp;

		if(buf[0]!='P')continue;

		for(int i=1;i<GROUP_MAX;i++)
		{
			if(buf[2] == i+'0')
			{
				sscanf(buf,/*P(%d)(%d):%f %f %f*/XorStr<0xDB,19,0x1215832B>("\x8B\xF4\xF8\xBA\xF6\xC8\xC4\x86\xCA\xDE\xC0\x80\xC7\xCD\x8F\xCA\xCE\x8A"+0x1215832B).s,&i,&tmp,&ways[i][point_count[i]].point[0],&ways[i][point_count[i]].point[1],&ways[i][point_count[i]].point[2]);
				point_count[i]++;
			}
		}
	}

	fclose(fp);

	groupCountCalc();
}

//**********************************************************************************************************************************

void WAYPOINT::add(int index)
{
	ways[index][point_count[index]].point[0]= me.pmEyePos[0];
	ways[index][point_count[index]].point[1] = me.pmEyePos[1];
	ways[index][point_count[index]].point[2] = me.pmEyePos[2];

	point_count[index]++;
	save();
}

//**********************************************************************************************************************************

void WAYPOINT::clear(int index)
{
	point_count[index] = 0;
	save();
}

//**********************************************************************************************************************************

void WAYPOINT::clearAll(void)
{
	for(int i=1;i<GROUP_MAX;i++)
	{
		point_count[i] = 0;
	}
	group_count = 0;
	save();
}

//**********************************************************************************************************************************

void WAYPOINT::record(void)
{
	if(!me.alive) return;
	static int recordIndex = 1;

	if(CalcRecordIndex)
	{
		for(int i=1;i<GROUP_MAX;i++)
		{
			if(point_count[i] == 0)
			{
				recordIndex = i;
				break;
			}
		}
	}
	CalcRecordIndex = false;

	if(point_count[recordIndex] == 0)
	{
		add(recordIndex);
	}
	else
	if(point_count[recordIndex]<POINT_MAX)
	{
		if(getDist(ways[recordIndex][point_count[recordIndex]-1].point)>cvar.record_dist && getDist(ways[recordIndex][point_count[recordIndex]-1].point)<2*cvar.record_dist)
		{
			add(recordIndex);
		}
	}
	else
	{
 		CalcRecordIndex = true;
		return;
	}
}

//**********************************************************************************************************************************


