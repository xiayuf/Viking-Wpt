//==============================================================================
// Aimbot.cpp
//==============================================================================

#include <windows.h>
#include <math.h>
#include "client.h"
#include "players.h"
#include "Aimbot.h"
#include "weaponslist.h"
#include "misc/defs.h"
#include "idhook.h"

using namespace std;

void gDrawFilledBoxAtLocation( float* origin, DWORD color, int radius);

typedef float TransformMatrix[MAXSTUDIOBONES][3][4];

CAimbot Aimbot;

#define M_PI		3.14159265358979323846

float fov = 0;

extern bool VISIBLE;

//==============================================================================

void VectorAngles( const float *forward, float *angles )
{
	float tmp, yaw, pitch;
	
	if( forward[1] == 0 && forward[0] == 0 )
	{
		yaw = 0;

		if( forward[2] > 0 )
			pitch = 90.0f;
		else
			pitch = 270.0f;
	}
	else
	{
		yaw = ( float )( ( atan2( forward[1], forward[0] ) * 180 / M_PI ) );

		if( yaw < 0 )
			yaw += 360.0f;

		tmp = sqrt( forward[0] * forward[0] + forward[1] * forward[1] );
		
		pitch = ( float )( ( atan2( forward[2], tmp ) * 180 / M_PI ) );
	}
	
	angles[0] = pitch;
	angles[1] = yaw;
	angles[2] = 0;
}

//==============================================================================

void VectorTransform (float *in1, float in2[3][4], float *out)
{
    out[0] = DotProduct(in1, in2[0]) + in2[0][3];
    out[1] = DotProduct(in1, in2[1]) + in2[1][3];
    out[2] = DotProduct(in1, in2[2]) + in2[2][3];
}

//==============================================================================
#include <gl\gl.h>

void CAimbot::CalculateHitbox( cl_entity_s *pEnt )
{
	if( !cvar.aim_mode )
		return;
	if(cvar.aim_mode == 1)
	{
		if( !vPlayers[pEnt->index].bGotHead )
		{
			int iIndex = pEnt->index;

			model_s		    *pModel			= oEngStudio.SetupPlayerModel( iIndex );
			studiohdr_t     *pStudioHeader	= ( studiohdr_t* )oEngStudio.Mod_Extradata( pModel );
			mstudiobbox_t   *pStudioBox;
			TransformMatrix *pBoneTransform = ( TransformMatrix* )oEngStudio.StudioGetBoneTransform( );	

			vec3_t vMin, vMax;
			pStudioBox = ( mstudiobbox_t* )( ( byte* )pStudioHeader + pStudioHeader->hitboxindex );

			//Head 11 bone 7 | Low Head 9 bone 5 | Chest 8 bone 4 | Stomach 7 bone 3
			int i = 11;
			if (cvar.hitbox == 1) i = 11;
			else if (cvar.hitbox == 2) i = 9;
			else if (cvar.hitbox == 3) i = 8;	
			else if (cvar.hitbox == 4) i = 7;	
			VectorTransform(pStudioBox[i].bbmin, (*pBoneTransform)[pStudioBox[i].bone], vMin);
			VectorTransform(pStudioBox[i].bbmax, (*pBoneTransform)[pStudioBox[i].bone], vMax);
			vPlayers[iIndex].vHitbox	= ( vMin + vMax ) * 0.5f;
			vPlayers[iIndex].bGotHead	= true;		
		}
	}
	else if(cvar.aim_mode == 2)
	{
		int ax = pEnt->index;
		if(!vPlayers[ax].bGotHead)
		{
			vec3_t pos;
			TransformMatrix*  pbonetransform = (TransformMatrix*)oEngStudio.StudioGetBoneTransform();
			//Head 11 bone 7 | Low Head 9 bone 5 | Chest 8 bone 4 | Stomach 7 bone 3
			int i = cvar.bone;
			pos[ 0 ] = (*pbonetransform)[ i ][ 0 ][ 3 ];
			pos[ 1 ] = (*pbonetransform)[ i ][ 1 ][ 3 ];
			pos[ 2 ] = (*pbonetransform)[ i ][ 2 ][ 3 ];
			VectorCopy(pos, vPlayers[ax].vHitbox);
			vPlayers[ax].bGotHead = true;
		}
	}

	if (cvar.pred)
	{
		int index = pEnt->index;
		cl_entity_s* ent = vPlayers[index].getEnt();
		vec3_t vFromOrigin, vToOrigin, delta;

		int historyIndex = ((int)(ent->current_position+HISTORY_MAX-cvar.predback))%HISTORY_MAX;
			
		vFromOrigin = ent->ph[historyIndex].origin;
		vToOrigin   = ent->ph[ent->current_position].origin;
  
		delta[0] = vToOrigin[0] - vFromOrigin[0];
		delta[1] = vToOrigin[1] - vFromOrigin[1];
		delta[2] = vToOrigin[2] - vFromOrigin[2];

		delta[0] *= cvar.predahead;
		delta[1] *= cvar.predahead;
		delta[2] *= cvar.predahead;

		vPlayers[index].vHitbox[0] += delta[0];
		vPlayers[index].vHitbox[1] += delta[1];
		vPlayers[index].vHitbox[2] += delta[2];
	}
}

//==============================================================================

void CAimbot::CalculateAimingView( void )
{
	float view[3];
	vec3_t vecEnd, up, right, forward, EntViewOrg, playerAngles;

	if (!vPlayers[iTarget].fixHbAim) {VectorCopy(vPlayers[iTarget].vHitbox,EntViewOrg);}	
	else							 {VectorCopy(vPlayers[iTarget].origin(),EntViewOrg);}	
	playerAngles[0]=0;
	playerAngles[1]=vPlayers[iTarget].getEnt()->angles[1];
	playerAngles[2]=0;
	gEngfuncs.pfnAngleVectors (playerAngles, forward, right, up);
	forward[2] = -forward[2];

	if(cvar.aim_mode==3)
	{
		aimvec* si = TargetRegion(iTarget);
		float h = 0.0, r = 0.0, f = 0.0;
		if (si)
		{
			f = si->f;
			h = si->h;
			r = si->r;
		}
		else 
		{
			if (vPlayers[iTarget].getEnt()->curstate.usehull == 0)
			{
				if (AimVecsS.size() > 0.0f)
				{
					f = AimVecsS[0].f;
					h = AimVecsS[0].h;
					r = AimVecsS[0].r;
				}
			} else {
				if (AimVecsD.size() > 0.0f)
				{
					f = AimVecsD[0].f;
					h = AimVecsD[0].h;
					r = AimVecsD[0].r;
				}
			}
			if (vPlayers[iTarget].getEnt()->curstate.gaitsequence==6 && AimVecsJ.size() > 0.0f) 
			{ 
				f = AimVecsJ[0].f; 
				h = AimVecsJ[0].h; 
				r = AimVecsJ[0].r; 
			}
		}

		EntViewOrg[0] = EntViewOrg[0] + forward[0]*f + up[0]*h + right[0]*r;
		EntViewOrg[1] = EntViewOrg[1] + forward[1]*f + up[1]*h + right[1]*r;
		EntViewOrg[2] = EntViewOrg[2] + forward[2]*f + up[2]*h + right[2]*r;
	}
	else if(cvar.aim_mode==1)
	{
		EntViewOrg = EntViewOrg + forward * cvar.hadj_f;
		EntViewOrg = EntViewOrg + up * cvar.hadj_h;
		EntViewOrg = EntViewOrg + right * cvar.hadj_r;

		if (iTarget !=-1 && vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6)
		{
			EntViewOrg = EntViewOrg + forward * cvar.judj_f;
			EntViewOrg = EntViewOrg + up * cvar.judj_h;
			EntViewOrg = EntViewOrg + right * cvar.judj_r;
		}
	}
	else if(cvar.aim_mode==2)
	{
		EntViewOrg = EntViewOrg + forward * cvar.badj_f;
		EntViewOrg = EntViewOrg + up * cvar.badj_h;
		EntViewOrg = EntViewOrg + right * cvar.badj_r;

		if (iTarget !=-1 && vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6)
		{
			EntViewOrg = EntViewOrg + forward * cvar.bjudj_f;
			EntViewOrg = EntViewOrg + up * cvar.bjudj_h;
			EntViewOrg = EntViewOrg + right * cvar.bjudj_r;
		}
	}

	view[0] = EntViewOrg[0] - me.pmEyePos[0];
	view[1] = EntViewOrg[1] - me.pmEyePos[1];
	view[2] = EntViewOrg[2] - me.pmEyePos[2];

	VectorAngles(view,aim_viewangles);
	aim_viewangles[0] *= -1;

	if (aim_viewangles[0]>180) aim_viewangles[0]-=360;
	if (aim_viewangles[1]>180) aim_viewangles[1]-=360;
	if(cvar.smooth!=0.0)
	{
		float diff[3];
		diff[0] = aim_viewangles[0]-me.viewAngles[0];
		diff[1] = aim_viewangles[1]-me.viewAngles[1];
		
		if (diff[0]>180)  diff[0]-=360;
		if (diff[1]>180)  diff[1]-=360;
		if (diff[0]<-180) diff[0]+=360;
		if (diff[1]<-180) diff[1]+=360;

		if     ( diff[0] >  cvar.smooth ) { aim_viewangles[0] = me.viewAngles[0]+cvar.smooth; }
		else if( diff[0] < -cvar.smooth ) { aim_viewangles[0] = me.viewAngles[0]-cvar.smooth; }
		if     ( diff[1] >  cvar.smooth ) { aim_viewangles[1] = me.viewAngles[1]+cvar.smooth; }
		else if( diff[1] < -cvar.smooth ) { aim_viewangles[1] = me.viewAngles[1]-cvar.smooth; }

		if (aim_viewangles[0]>180)  aim_viewangles[0]-=360;
		if (aim_viewangles[1]>180)  aim_viewangles[1]-=360;
		if (aim_viewangles[0]<-180) aim_viewangles[0]+=360;
		if (aim_viewangles[1]<-180) aim_viewangles[1]+=360;
	}
}

//==============================================================================

pmtrace_t pmtrace;
int CAimbot::CanPenetrate( float *start, float *end, int power ) 
{ 
	pmtrace_t* tr = (pmtrace_t*) &pmtrace;

	float view[3]; 
	float dir[3]; 

	view[0] = end[0] - start[0]; 
	view[1] = end[1] - start[1]; 
	view[2] = end[2] - start[2]; 

	float length = VectorLength(view); 

	dir[0] = view[0] / length; 
	dir[1] = view[1] / length; 
	dir[2] = view[2] / length; 

	float position[3]; 
	position[0] = start[0]; 
	position[1] = start[1]; 
	position[2] = start[2]; 
	tr->startsolid = true; 

	while( power ) 
	{ 
	   if( !tr->startsolid ) 
	      power--; 
	   tr = gEngfuncs.PM_TraceLine( position, end, PM_TRACELINE_PHYSENTSONLY, 2, -1); 
        
	   if( tr->fraction==1.0f ) 
	      return 1; 
	   if( tr->allsolid ) 
	      return 0; 

	   position[0] = tr->endpos[0] + dir[0] * 8.0f; 
	   position[1] = tr->endpos[1] + dir[1] * 8.0f; 
	   position[2] = tr->endpos[2] + dir[2] * 8.0f; 
	} 
	return 0; 
} 

//==============================================================================

int CAimbot::CorrectGun()
{
	if (GetCurWeaponId() == WEAPONLIST_SG550 || GetCurWeaponId() == WEAPONLIST_G3SG1 || GetCurWeaponId() == WEAPONLIST_SCOUT || GetCurWeaponId() == WEAPONLIST_AWP) 
		return 3; 
	if (GetCurWeaponId() == WEAPONLIST_AUG || GetCurWeaponId() == WEAPONLIST_M249 || GetCurWeaponId() == WEAPONLIST_M4A1 || GetCurWeaponId() == WEAPONLIST_DEAGLE || GetCurWeaponId() == WEAPONLIST_SG552 || GetCurWeaponId() == WEAPONLIST_AK47|| GetCurWeaponId() == WEAPONLIST_FAMAS || GetCurWeaponId() == WEAPONLIST_GALIL) 
		return 2; 

	return 0; 
}

//==============================================================================

#pragma warning (disable:4800)
bool CAimbot::pathFree(float* xfrom,float* xto)
{
	int pathtest; 
	pmtrace_t tr; 
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 ); 
	gEngfuncs.pEventAPI->EV_PlayerTrace( xfrom, xto, PM_GLASS_IGNORE, me.entindex, &tr ); 
	pathtest = (tr.fraction == 1.0); 
	if (!pathtest && cvar.autowall && CorrectGun()) 
	{ 
	   pathtest = CanPenetrate(xfrom, xto, CorrectGun()); 
	} 
	return pathtest; 
}

//====================================================================================

aimvec* CAimbot::TargetRegion(int ax)
{
	vec3_t vecEnd, up, right, forward, EntViewOrg,playerAngles;
	cl_entity_s* ent = vPlayers[ax].getEnt();
	playerAngles[0]=0;
	playerAngles[1]=ent->angles[1];
	playerAngles[2]=0;
    gEngfuncs.pfnAngleVectors (playerAngles, forward, right, up);
	forward[2] = -forward[2];

	if (vPlayers[ax].bGotHead)	{VectorCopy(vPlayers[ax].vHitbox,EntViewOrg);vPlayers[ax].fixHbAim=false;}	
	else						{VectorCopy(vPlayers[ax].origin(),EntViewOrg);vPlayers[ax].fixHbAim=true;}		

	vector<aimvec>::iterator si, end;
	static struct aimvec temp = {0,0,0};

	if(cvar.aim_mode == 3)
	{
		if (vPlayers[iTarget].getEnt()->curstate.usehull == 0) 
		{ 
			si = AimVecsS.begin(); end = AimVecsS.end(); 
		} else { 
			si = AimVecsD.begin(); end = AimVecsD.end(); 
		} 
		if (vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6) 
		{
			si = AimVecsJ.begin(); end = AimVecsJ.end();
		}
		
   		for (;si!=end;++si) 
   		{ 
   		   EntViewOrg = EntViewOrg + forward * si->f; 
   		   EntViewOrg = EntViewOrg + up * si->h; 
   		   EntViewOrg = EntViewOrg + right * si->r; 
   		   if(pathFree(me.pmEyePos,EntViewOrg)) return &(*si); 
   		} 
	}
	else if(cvar.aim_mode==1)
	{
		EntViewOrg = EntViewOrg + forward * cvar.hadj_f;
		EntViewOrg = EntViewOrg + up * cvar.hadj_h;
		EntViewOrg = EntViewOrg + right * cvar.hadj_r;

		if (iTarget !=-1 && vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6)
		{
			EntViewOrg = EntViewOrg + forward * cvar.judj_f;
			EntViewOrg = EntViewOrg + up * cvar.judj_h;
			EntViewOrg = EntViewOrg + right * cvar.judj_r;
		}
	}
	else if(cvar.aim_mode==2)
	{
		EntViewOrg = EntViewOrg + forward * cvar.badj_f;
		EntViewOrg = EntViewOrg + up * cvar.badj_h;
		EntViewOrg = EntViewOrg + right * cvar.badj_r;

		if (iTarget !=-1 && vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6)
		{
			EntViewOrg = EntViewOrg + forward * cvar.bjudj_f;
			EntViewOrg = EntViewOrg + up * cvar.bjudj_h;
			EntViewOrg = EntViewOrg + right * cvar.bjudj_r;
		}
	}
		
	if(pathFree(me.pmEyePos,EntViewOrg)) return (aimvec*)true;

	return false;
}
//===================================================================================

//==============================================================================
void CAimbot::FindTarget( void )
{
	if (!me.alive) return;

	SetTarget(-1);

	for (int ax=0;ax<MAX_VPLAYERS;ax++)
	{
		if  ( vPlayers[ax].isUpdatedAddEnt() && vPlayers[ax].canAim && ( idhook.FirstKillPlayer[ax]==1 || cvar.id_mode==0 ) )
		{
			if( !HasTarget() )											{ SetTarget(ax); continue; }

			switch(cvar.find_mode)
			{
			default:
			case 1:
				if( vPlayers[ax].distance < vPlayers[iTarget].distance )  {  SetTarget(ax);  break;  }
				break;
			case 2:
				if( vPlayers[ax].fovangle < vPlayers[iTarget].fovangle )  {  SetTarget(ax);  break;  }
				break;
			}
		}
	}

	if(cvar.id_mode==2)return;

	if( (cvar.id_mode==0 && !HasTarget()) || (cvar.id_mode==1 && !HasTarget()) || (cvar.id_mode==2 && !HasTarget() ))
	{
		for (int ax=0;ax<MAX_VPLAYERS;ax++)
		{
			if (vPlayers[ax].isUpdatedAddEnt() && vPlayers[ax].canAim && idhook.FirstKillPlayer[ax]<2 )
			{
				if( !HasTarget() )											{ SetTarget(ax); continue; }

				switch(cvar.find_mode)
				{
				default:
				case 1:
					if( vPlayers[ax].distance < vPlayers[iTarget].distance )  {  SetTarget(ax);  break;  }
					break;
				case 2:
					if( vPlayers[ax].fovangle < vPlayers[iTarget].fovangle )  {  SetTarget(ax);  break;  }
					break;
				}
			}
		}
	}
}
//==============================================================================

void CAimbot::Draw()
{
	for (int ax=0;ax<MAX_VPLAYERS;ax++) 
		if (vPlayers[ax].isUpdated() && vPlayers[ax].isAlive())  Draw_Player_Vecs(ax);
}

//==============================================================================

void CAimbot::Draw_Player_Vecs(int ax)
{
	vec3_t up, right, forward, EntViewOrg,playerAngles;
	vec3_t aim_location;
	cl_entity_s* ent = vPlayers[ax].getEnt();

	playerAngles[0]=0;
	playerAngles[1]=ent->angles[1];
	playerAngles[2]=0;
    gEngfuncs.pfnAngleVectors (playerAngles, forward, right, up);
	forward[2] = -forward[2];

	if (vPlayers[ax].bGotHead)	{VectorCopy(vPlayers[ax].vHitbox,EntViewOrg);vPlayers[ax].fixHbAim=false;}	
	else						{VectorCopy(vPlayers[ax].origin(),EntViewOrg);vPlayers[ax].fixHbAim=true;}		

	if(cvar.aim_mode==3)
	{
		vector<aimvec>::iterator pos, end;
		if (vPlayers[iTarget].getEnt()->curstate.usehull == 0)
		{
				pos = AimVecsS.begin();
				end = AimVecsS.end();
		} else {
				pos = AimVecsD.begin();
				end = AimVecsD.end();
		}
		if (vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6) 
		{ 
			pos = AimVecsJ.begin(); 
			end = AimVecsJ.end(); 
		}

		for (;pos!=end;++pos) 
		{
			EntViewOrg = EntViewOrg + forward * pos->f;
			EntViewOrg = EntViewOrg + up * pos->h;
			EntViewOrg = EntViewOrg + right * pos->r;
		}
	}
	else if(cvar.aim_mode==1)
	{
		EntViewOrg = EntViewOrg + forward * cvar.hadj_f;
		EntViewOrg = EntViewOrg + up * cvar.hadj_h;
		EntViewOrg = EntViewOrg + right * cvar.hadj_r;

		if (iTarget !=-1 && vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6)
		{
			EntViewOrg = EntViewOrg + forward * cvar.judj_f;
			EntViewOrg = EntViewOrg + up * cvar.judj_h;
			EntViewOrg = EntViewOrg + right * cvar.judj_r;
		}
	}
	else if(cvar.aim_mode==2)
	{
		EntViewOrg = EntViewOrg + forward * cvar.badj_f;
		EntViewOrg = EntViewOrg + up * cvar.badj_h;
		EntViewOrg = EntViewOrg + right * cvar.badj_r;

		if (iTarget !=-1 && vPlayers[iTarget].getEnt()->curstate.gaitsequence == 6)
		{
			EntViewOrg = EntViewOrg + forward * cvar.bjudj_f;
			EntViewOrg = EntViewOrg + up * cvar.bjudj_h;
			EntViewOrg = EntViewOrg + right * cvar.bjudj_r;
		}
	}

	register DWORD color = 0xFF0000FF;
	
	gDrawFilledBoxAtLocation(EntViewOrg,color,2);
}

//==============================================================================

bool CAimbot::CheckTeam(int ax)
{
	if (!cvar.team && (me.team != vPlayers[ax].team)) return true; 
	else if (cvar.team==1 &&  (me.team == vPlayers[ax].team)) return true;
	else if (cvar.team==2) return true;
	return false;
}

//==============================================================================

bool CAimbot::IsShielded(int ax)
{
	int seqinfo = getSeqInfo(ax);
	if (seqinfo & SEQUENCE_RELOAD)
		return false;
	if (seqinfo & SEQUENCE_SHIELD)
		return true;
	return false;
}
//===================================================================================
/*double CAimbot::GetAimingFov(int ax)
{
	float vec[3], view[3];
	float dot;	
	float* origin = vPlayers[ax].getEnt()->origin;
	view[0] = origin[0] - me.pmEyePos[0];
	view[1] = origin[1] - me.pmEyePos[1];
	view[2] = origin[2] - me.pmEyePos[2];
	dot = sqrt(view[0] * view[0] + view[1] * view[1] + view[2] * view[2]);
	dot = 1/dot;
	vec[0] = view[0] * dot;
	vec[1] = view[1] * dot;
	vec[2] = view[2] * dot;
	view[0] = sin((me.viewAngles[1] + 90) * (0.0174532922));
	view[1] = -cos((me.viewAngles[1] + 90) * (0.0174532922));
	view[2] = -sin(me.viewAngles[0] * (0.0174532922));	
	dot = view[0] * vec[0] + view[1] * vec[1] + view[2] * vec[2];	
	return dot;
}*/
//==============================================================================
float CAimbot::calcFovAngle(const float* origin_viewer, const float* angle_viewer, const float* origin_target)
{
	double vec[3], view[3];
	double dot;	
	
	view[0] = origin_target[0] - origin_viewer[0];
	view[1] = origin_target[1] - origin_viewer[1];
	view[2] = origin_target[2] - origin_viewer[2];

	dot = sqrt(view[0] * view[0] + view[1] * view[1] + view[2] * view[2]);
	dot = 1/dot;

	vec[0] = view[0] * dot;
	vec[1] = view[1] * dot;
	vec[2] = view[2] * dot;
	
	view[0] =  sin((angle_viewer[1] + 90) * (M_PI / 180));
	view[1] = -cos((angle_viewer[1] + 90) * (M_PI / 180));
	view[2] = -sin( angle_viewer[0] * (M_PI / 180));	
	
	dot = view[0] * vec[0] + view[1] * vec[1] + view[2] * vec[2];
	
	// dot to angle:
	return (float)((1.0-dot)*180.0); 
}
void CAimbot::calcFovangleAndVisibility(int ax)
{ 
	PlayerInfo& r = vPlayers[ax];
	r.fovangle    = calcFovAngle(me.pmEyePos, me.viewAngles, r.origin() );
	
	if (cvar.fov == 1) fov = 1;
	else if (cvar.fov == 2) fov = 5;
	else if (cvar.fov == 3) fov = 30;
	else if (cvar.fov == 4) fov = 90;
	else if (cvar.fov == 5) fov = 360;

	r.visible = TargetRegion(ax);

	if(0) {}
	else if (!cvar.team && !CheckTeam(ax))					{ r.canAim = 0; }
	else if (IsCurWeaponNade())					            { r.canAim = 0; }
	else if (IsCurWeaponC4())					            { r.canAim = 0; }
	else if (vPlayers[ax].USEID)					        { r.canAim = 0; }
	else if (!cvar.shield_attack && IsShielded(ax))			{ r.canAim = 0; }
	else if (r.fovangle>fov)					            { r.canAim = 0; }
	else										            { r.canAim = r.visible; }
}
//===================================================================================

//==============================================================================
