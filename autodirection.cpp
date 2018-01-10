/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <windows.h>

#include "client.h"
#include "autodirection.h"
#include "players.h"
#include "cvars.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HURRICANE autodirection.cpp [by cs_007]
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

AutoDirection autodirection;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


extern double pisobre180;
float relativeSpeed(void);

//**********************************************************************************************************************************

#define PM_NORMAL			0x00000000
#define PM_STUDIO_IGNORE	0x00000001		// Skip studio models
#define PM_STUDIO_BOX		0x00000002		// Use boxes for non-complex studio models (even in traceline)
#define PM_GLASS_IGNORE		0x00000004		// Ignore entities with non-normal rendermode
#define PM_WORLD_ONLY		0x00000008		// Only trace against the world

// Values for flags parameter of PM_TraceLine
#define PM_TRACELINE_ANYVISIBLE		0
#define PM_TRACELINE_PHYSENTSONLY	1

//**********************************************************************************************************************************

bool AutoDirection::wayFree1(float* from,float* to)
{
	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull( 2 );
//	gEngfuncs.pEventAPI->EV_PlayerTrace( from, to, PM_NORMAL, 2, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_STUDIO_BOX, 2, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_STUDIO_BOX, -1, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_STUDIO_BOX, me.ent->index, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace( from, to, PM_NORMAL, me.ent->index, &tr );


	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_NORMAL, -1, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_GLASS_IGNORE, -1, &tr );// para ignorar puertas?

//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_GLASS_IGNORE, me.ent->index, &tr );

	return ( tr.fraction == 1.0 ); 
}

//**********************************************************************************************************************************

bool AutoDirection::wayFree2(float* from,float* to)
{
	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull ( 2 );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_STUDIO_BOX, -1, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_NORMAL, me.ent->index, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_NORMAL, -1, &tr );
	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_STUDIO_BOX, -1, &tr );
	int iEntity = gEngfuncs.pEventAPI->EV_IndexFromTrace ( &tr );
	cl_entity_s* ent = gEngfuncs.GetEntityByIndex ( iEntity );
	if(!ent->player || ent->index == me.ent->index) return true;// true=libre
	return ( tr.fraction == 1.0 ); 
}

//**********************************************************************************************************************************

bool AutoDirection::wayFree3(float* from,float* to)
{
	pmtrace_t tr;
	gEngfuncs.pEventAPI->EV_SetTraceHull ( 2 );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_NORMAL, me.ent->index, &tr );
//	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_NORMAL, 2, &tr );
	gEngfuncs.pEventAPI->EV_PlayerTrace ( from, to, PM_NORMAL, -1, &tr );
	
	/*
	int iEntity = gEngfuncs.pEventAPI->EV_IndexFromTrace ( &tr );
	cl_entity_s* ent = gEngfuncs.GetEntityByIndex ( iEntity );
	if(ent->player || ent->index == me.ent->index) return true;*/

	return ( tr.fraction == 1.0 ); 
}

//**********************************************************************************************************************************

void AutoDirection::NextStepPos(float* nowpos,float* viewangles,float step,float* newpos)
{
	float dx,dy,dz,dxy;
	dx=0;
	dy=0;
	//dz=(float)-sin(viewangles[0]*pisobre180)*step;
	dz=0;
	//dxy=(float)cos(viewangles[0]*pisobre180)*step;
	dxy=step;

	if( viewangles[1]>90 && viewangles[1]<=180 )
		dx = dxy * -cos((180.0-viewangles[1])*pisobre180);
	else if( viewangles[1]>=0 && viewangles[1]<=90 )
		dx = dxy * cos(viewangles[1]*pisobre180);
	else if( viewangles[1]>=-90 && viewangles[1]<0 )
		dx = dxy * cos((-viewangles[1])*pisobre180);
	else if( viewangles[1]>=-180 && viewangles[1]<-90)
		dx = dxy * -cos((180.0+viewangles[1])*pisobre180);
	if( viewangles[1]>90 && viewangles[1]<=180 )
		dy = dxy * sin((180.0-viewangles[1])*pisobre180);
	else if( viewangles[1]>=0 && viewangles[1]<=90 )
		dy = dxy * sin(viewangles[1]*pisobre180);
	else if( viewangles[1]>=-90 && viewangles[1]<0 )
		dy = dxy * -sin((-viewangles[1])*pisobre180);
	else if( viewangles[1]>=-180 && viewangles[1]<-90)
		dy = dxy * -sin((180.0+viewangles[1])*pisobre180);

	newpos[0]=nowpos[0]+dx;
	newpos[1]=nowpos[1]+dy;
	newpos[2]=nowpos[2]+dz;
}

//**********************************************************************************************************************************

void AutoDirection::SetAngle(float* viewangles)
{
	if( viewangles[0]>180 )
		viewangles[0]=viewangles[0]-360;
	else if( viewangles[0]<-180 )
		viewangles[0]=viewangles[0]+360;	
	if( viewangles[1]>180 )
		viewangles[1]=viewangles[1]-360;
	else if( viewangles[1]<-180 )
		viewangles[1]=viewangles[1]+360;
	if( viewangles[2]>180 )
		viewangles[2]=viewangles[2]-360;
	else if( viewangles[2]<-180 )
		viewangles[2]=viewangles[2]+360;
}

//**********************************************************************************************************************************

bool AutoDirection::IsMyWayFree1(float step,float userangle)
{
	float src[3],src1[3],src2[3];
	float dst[3],dst1[3],dst2[3];
	float angles[3];
	bool free,free1,free2;
	src[0]=me.ent->origin[0];
	src[1]=me.ent->origin[1];
	src[2]=me.ent->origin[2];
	angles[0]=0;angles[2]=0;
	angles[1]=userangle;
	NextStepPos(src,angles,step,dst);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle+90;
	NextStepPos(src,angles,cvar.direction_step,src1);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle;
	NextStepPos(src1,angles,step,dst1);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle-90;
	NextStepPos(src,angles,cvar.direction_step,src2);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle;
	NextStepPos(src2,angles,step,dst2);
	free=wayFree1(src,dst);
	free1=wayFree1(src1,dst1);
	free2=wayFree1(src2,dst2);
	if( free&free1&free2 )return true;
	return false;
}

//**********************************************************************************************************************************

bool AutoDirection::IsMyWayFree2(float step,float userangle)
{
	float src[3],src1[3],src2[3];
	float dst[3],dst1[3],dst2[3];
	float angles[3];
	bool free,free1,free2;
	src[0]=me.ent->origin[0];
	src[1]=me.ent->origin[1];
	src[2]=me.ent->origin[2];
	angles[0]=0;angles[2]=0;
	angles[1]=userangle;
	NextStepPos(src,angles,step,dst);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle+90;
	NextStepPos(src,angles,cvar.direction_step,src1);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle;
	NextStepPos(src1,angles,step,dst1);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle-90;
	NextStepPos(src,angles,cvar.direction_step,src2);
	angles[0]=0;angles[2]=0;
	angles[1]=userangle;
	NextStepPos(src2,angles,step,dst2);
	free=wayFree2(src,dst);
	free1=wayFree2(src1,dst1);
	free2=wayFree2(src2,dst2);
	if( free&free1&free2 )return true;
	return false;
}

//**********************************************************************************************************************************

inline void adjustAngle(float *angle){
	if( (*angle)>180 )(*angle)=(*angle)-360;
	else if( (*angle)<-180 )(*angle)=(*angle)+360;
}

//**********************************************************************************************************************************

bool AutoDirection::adjustMyDirection1(struct usercmd_s* usercmd)
{
	float userangle;
	float stepangle=0;// no hace falta inicializar..

    if(usercmd->forwardmove==0){
		if(usercmd->sidemove>=0){
			stepangle = 90;}
		else{
			stepangle = -90;}}
	else{
		if(usercmd->forwardmove>=0){
			stepangle = atan(usercmd->sidemove/usercmd->forwardmove);}
		else{
			stepangle = 180-atan(usercmd->sidemove/(-(usercmd->forwardmove)));}}

	/////////////////////////////////////////////////////
	//stepangle siempre es inicializado arriba..!!
	///////////////////////////////////////////////////////////
	userangle = usercmd->viewangles[1]-stepangle;
	adjustAngle(&userangle);
	float newangle;
	float newangle1,newangle2;
	float da;
	bool free1,free2;
	int step=cvar.action_step;
	if( IsMyWayFree1(step,userangle) )return false;

	//for(da=15;da<=165;da=da+15)
	//for(da=45;da<=135;da=da+45)

#define ANGLE_ADD 45
#define ANGLE_LIMIT 135
	for(da=ANGLE_ADD;da<=ANGLE_LIMIT;da=da+ANGLE_ADD){// no tiene sentido ser cero porque luego se debe sumar
		
		newangle1=userangle+da;
		adjustAngle(&newangle1);
		if( IsMyWayFree1(step,newangle1) ) 
			free1=true; 
		else free1=false;
		
		newangle2=userangle-da;
		adjustAngle(&newangle2);
		if( IsMyWayFree1(step,newangle2) )
			free2=true; 
		else free2=false;
		
		if( free1 && !free2 )goto aimnewangle1;
		if( !free1 && free2 )goto aimnewangle2;
		if( free1 && free2 )goto aimnewangle1;
	}

	//////////////////////////////////////////////////
	// si los dos lados estan trabados..
	newangle=userangle-180;
	goto adjust;
	///////////////////////////////////
aimnewangle1:
	newangle=newangle1;
	goto adjust;
aimnewangle2:
	newangle=newangle2;
	goto adjust;
adjust:
	float angle;
	angle=newangle-usercmd->viewangles[1];
	if(angle>=180) angle-=360;
	if(angle<=-180) angle+=360;
	float fm,sm;
	fm = (float)cos(angle*pisobre180)*cvar.rush_step;
	sm = (float)-sin(angle*pisobre180)*cvar.rush_step;
	usercmd->forwardmove = fm;
	usercmd->sidemove = sm;
	return true;
}

//**********************************************************************************************************************************

bool AutoDirection::adjustMyDirection2(struct usercmd_s* usercmd)
{
	float userangle;
	float stepangle;

    if(usercmd->forwardmove==0){
		if(usercmd->sidemove>=0){
			stepangle = 90;}
		else{
			stepangle = -90;}}
	else{
		if(usercmd->forwardmove>=0){
			stepangle = atan(usercmd->sidemove/usercmd->forwardmove);}
		else{
			stepangle = 180-atan(usercmd->sidemove/(-(usercmd->forwardmove)));}}

	userangle = usercmd->viewangles[1]-stepangle;
	adjustAngle(&userangle);
	float newangle;
	float newangle1,newangle2;
	float da;
	bool free1,free2;
	int step=cvar.action_step;
	if( IsMyWayFree2(step,userangle) )return false;
	for(da=45;da<=135;da=da+45){
		newangle1=userangle+da;
		adjustAngle(&newangle1);
		if( IsMyWayFree2(step,newangle1) ) free1=true; else free1=false;
		newangle2=userangle-da;
		adjustAngle(&newangle2);
		if( IsMyWayFree2(step,newangle2) )free2=true; else free2=false;
		if( free1 && !free2 )goto aimnewangle1;
		if( !free1 && free2 )goto aimnewangle2;
		if( free1 && free2 )goto aimnewangle1;}
	newangle=userangle-180;
	goto adjust;
aimnewangle1:
	newangle=newangle1;
	goto adjust;
aimnewangle2:
	newangle=newangle2;
	goto adjust;
adjust:
	float angle;
	angle=newangle-usercmd->viewangles[1];
	if(angle>=180) angle-=360;
	if(angle<=-180) angle+=360;
	float fm,sm;
	fm = cvar.rush_step*cos(angle*pisobre180);
	sm = -cvar.rush_step*sin(angle*pisobre180);
	usercmd->forwardmove = fm;
	usercmd->sidemove = sm;
	return true;
}

//**********************************************************************************************************************************

bool AutoDirection::IsMyWayFree3(float step,float* viewangles)
{
	float src[3],src1[3],src2[3];
	float dst[3],dst1[3],dst2[3];
	float angles[3];
	bool free,free1,free2;
	src[0]=me.pmEyePos[0];
	src[1]=me.pmEyePos[1];
	src[2]=me.pmEyePos[2];
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextStepPos(src,angles,step,dst);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]+90;
	SetAngle(angles);
	NextStepPos(src,angles,cvar.direction_step,src1);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextStepPos(src1,angles,step,dst1);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]-90;
	SetAngle(angles);
	NextStepPos(src,angles,cvar.direction_step,src2);
	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextStepPos(src2,angles,step,dst2);
	free=wayFree3(src,dst);
	free1=wayFree3(src1,dst1);
	free2=wayFree3(src2,dst2);
	if( free&free1&free2 )return true;
	return false;
}

//**********************************************************************************************************************************

bool AutoDirection::adjustMyAngles(float* viewangles)
{
	float oldviews[3],newview1[3],newview2[3];
	float da;
	bool free1,free2;
	int step=30;
	oldviews[0]=viewangles[0];
	oldviews[1]=viewangles[1];
	oldviews[2]=viewangles[2];
	if( IsMyWayFree3(step,oldviews) )return false;
	for(da=15;da<=165;da=da+15)
	{
		newview1[0]=oldviews[0];
		newview1[1]=oldviews[1]+da;
		newview1[2]=oldviews[2];
		SetAngle(newview1);
		if( IsMyWayFree3(step,newview1) ) free1=true; else free1=false;

		newview2[0]=oldviews[0];
		newview2[1]=oldviews[1]-da;
		newview2[2]=oldviews[2];
		SetAngle(newview2);
		if( IsMyWayFree3(step,newview2) )free2=true; else free2=false;
		if( free1 && !free2 )goto aimnewview1;
		if( !free1 && free2 )goto aimnewview2;
		if( free1 && free2 )
				goto aimnewview1; 
	}
	viewangles[0]=oldviews[0];
	viewangles[1]=oldviews[1]-180;
	viewangles[2]=oldviews[2];
	SetAngle(viewangles);
	return true;
    aimnewview1:
	viewangles[0]=newview1[0];
	viewangles[1]=newview1[1];
	viewangles[2]=newview1[2];
	return true;
    aimnewview2:
	viewangles[0]=newview2[0];
	viewangles[1]=newview2[1];
	viewangles[2]=newview2[2];
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
bool AutoDirection::IsJumpFree(float step,float* viewangles)
{
	float src[3],src1[3],src2[3];
	float dst[3],dst1[3],dst2[3];
	float angles[3];
	bool free,free1,free2;
	src[0]=me.pmEyePos[0];
	src[1]=me.pmEyePos[1];
	src[2]=me.pmEyePos[2]-35;

	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextStepPos(src,angles,step,dst);

	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]+90;
	SetAngle(angles);
	NextStepPos(src,angles,cvar.direction_step,src1);

	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextStepPos(src1,angles,step,dst1);

	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1]-90;
	SetAngle(angles);
	NextStepPos(src,angles,cvar.direction_step,src2);

	angles[0]=0;angles[2]=0;
	angles[1]=viewangles[1];
	SetAngle(angles);
	NextStepPos(src2,angles,step,dst2);

	free=wayFree1(src,dst);
	free1=wayFree1(src1,dst1);
	free2=wayFree1(src2,dst2);
	if( free&free1&free2 )return true;
	return false;
}*/

//**********************************************************************************************************************************

/*
bool AutoDirection::Jump(float* viewangles)
{
	//if (me.ent->curstate.usehull) return;

	float oldviews[3];
	oldviews[0]=viewangles[0];
	oldviews[1]=viewangles[1];
	oldviews[2]=viewangles[2];
	if(IsMyWayFree3(cvar.jump_dist,oldviews))
	{
		if(!IsJumpFree(cvar.jump_dist,oldviews))
			return true;
		else
			return false;
	}
	else
		return false;
}*/

//**********************************************************************************************************************************
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**********************************************************************************************************************************

void AutoDirection::adjustJump(struct usercmd_s* usercmd){
	
	if (me.ent->curstate.usehull) return;	// in duck
	
	int auto_jump_h1=23;
	int auto_jump_h2=-28;
	int auto_jump_h3=0;
	int auto_jump_distance=cvar.jump_dist+20;
	bool free1,free2;
	float src[3],src1[3],src2[3],dst[3],dst1[3],dst2[3];
	float angles[3];
	src[0]=me.ent->origin[0];
	src[1]=me.ent->origin[1];
	src[2]=me.ent->origin[2];
	src1[0]=src[0];
	src1[1]=src[1];
	src1[2]=src[2]+auto_jump_h1;
	src2[0]=src[0];
	src2[1]=src[1];
	src2[2]=src[2]+auto_jump_h2;
	angles[0]=0;
	angles[1]=me.viewAngles[1];//usercmd->viewangles[1];
	angles[2]=0;
	NextStepPos(src,angles,auto_jump_distance,dst);
	dst1[0]=dst[0];
	dst1[1]=dst[1];
	dst1[2]=dst[2]+auto_jump_h1;
	dst2[0]=dst[0];
	dst2[1]=dst[1];
	dst2[2]=dst[2]+auto_jump_h3;
	free1=wayFree1(src1,dst1);
	free2=wayFree1(src2,dst2);	
	if( free1&&!free2 )
	{
		usercmd->buttons |= IN_JUMP;
		
		if ( (usercmd->buttons & IN_JUMP) && !(me.pmFlags & FL_ONGROUND) ) usercmd->buttons &= ~IN_JUMP;
	}
}

//**********************************************************************************************************************************

/*
void AutoDirection::pos_nextStep(float* origin,float* point,float* viewangles,int type)
{
	int dis=0;
	if(type==1) dis = cvar.jump_dist;
	else if(type==2) dis = cvar.contact_dist;
	point[0] = dis * cos(PI*viewangles[1]/180) + origin[0];
	point[1] = dis * sin(PI*viewangles[1]/180) + origin[1];
	point[2] = origin[2];
}*/

//**********************************************************************************************************************************

/*
int AutoDirection::wallType(struct usercmd_s* usercmd, int type)
{
	float nextPos[3];
	float nextPos_h[3];
	float nextPos_l[3];

	//pos_nextStep(me.pmEyePos,nextPos,me.viewAngles);
	pos_nextStep(me.pmEyePos,nextPos,mainViewAngles,type);
	//pos_nextStep(me.pmEyePos,nextPos,usercmd->viewangles);

	//high
	nextPos_h[0] = nextPos[0];
	nextPos_h[1] = nextPos[1];
	nextPos_h[2] = nextPos[2];

	//low
	nextPos_l[0] = nextPos[0];
	nextPos_l[1] = nextPos[1];
	nextPos_l[2] = nextPos[2]-35.0f;

	if(!wayFree(me.pmEyePos,nextPos_h))
	{
		return 2;
	}
	else if(!wayFree(me.pmEyePos,nextPos_l))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}*/

//**********************************************************************************************************************************

/*
void AutoDirection::createAction(struct usercmd_s* usercmd)
{
	int wall_type = wallType(usercmd,2);

	if(wall_type == 0 || !me.alive) return;

	if(wall_type == 2)
	{
		//if (usercmd->buttons&IN_FORWARD && ( cvar.auto_bq==1 || (cvar.auto_bq==2 && !cvar.way_on) ))
		if (usercmd->buttons&IN_FORWARD)
		{
			float angles[3];

			//angles[0] = usercmd->viewangles[0];
			//angles[2] = usercmd->viewangles[2];
			angles[0] = mainViewAngles[0];
			angles[2] = mainViewAngles[2];
			//angles[0] = me.viewAngles[0];
			//angles[2] = me.viewAngles[2];

			if(relativeSpeed() > 20.0f)
			{
				if(me.pmVelocity[0] == 0)
				{    
					if( (0.0 < me.viewAngles[1] && me.viewAngles[1] <= 90.0) || (-180.0 < me.viewAngles[1] && me.viewAngles[1] <= -90.0) )    
					angles[1] = me.viewAngles[1] + cvar.direction_fov;
					//if( (0.0 < usercmd->viewangles[1] && usercmd->viewangles[1] <= 90.0) || (-180.0 < usercmd->viewangles[1] && usercmd->viewangles[1] <= -90.0) )
					//angles[1] = usercmd->viewangles[1] + cvar.direction_fov;
					//if( (0.0 < mainViewAngles[1] && mainViewAngles[1] <= 90.0) || (-180.0 < mainViewAngles[1] && mainViewAngles[1] <= -90.0) )
					//angles[1] = mainViewAngles[1] + cvar.direction_fov;


					if( (90.0 <= me.viewAngles[1] && me.viewAngles[1] < 180.0) || (-90.0 <= me.viewAngles[1] && me.viewAngles[1] < 0.0) )
					angles[1] = me.viewAngles[1] - cvar.direction_fov;
					//if( (90.0 <= usercmd->viewangles[1] && usercmd->viewangles[1] < 180.0) || (-90.0 <= usercmd->viewangles[1] && usercmd->viewangles[1] < 0.0) )
					//angles[1] = usercmd->viewangles[1] - cvar.direction_fov;
					//if( (90.0 <= mainViewAngles[1] && mainViewAngles[1] < 180.0) || (-90.0 <= mainViewAngles[1] && mainViewAngles[1] < 0.0) )
					//angles[1] = mainViewAngles[1] - cvar.direction_fov;


					gEngfuncs.SetViewAngles(angles);
				}
				if(me.pmVelocity[1] == 0)
				{
					
					if( (0.0 < me.viewAngles[1] && me.viewAngles[1] <= 90.0) || (-180.0 < me.viewAngles[1] && me.viewAngles[1] <= -90.0) )
					angles[1] = me.viewAngles[1] - cvar.direction_fov;
					//if( (0.0 < usercmd->viewangles[1] && usercmd->viewangles[1] <= 90.0) || (-180.0 < usercmd->viewangles[1] && usercmd->viewangles[1] <= -90.0) )
					//angles[1] = usercmd->viewangles[1] - cvar.direction_fov;
					//if( (0.0 < mainViewAngles[1] && mainViewAngles[1] <= 90.0) || (-180.0 < mainViewAngles[1] && mainViewAngles[1] <= -90.0) )
					//angles[1] = mainViewAngles[1] - cvar.direction_fov;


					if( (90.0 <= me.viewAngles[1] && me.viewAngles[1] < 180.0) || (-90.0 <= me.viewAngles[1] && me.viewAngles[1] < 0.0) )
					angles[1] = me.viewAngles[1] + cvar.direction_fov;
					//if( (90.0 <= usercmd->viewangles[1] && usercmd->viewangles[1] < 180.0) || (-90.0 <= usercmd->viewangles[1] && usercmd->viewangles[1] < 0.0) )
					//angles[1] = usercmd->viewangles[1] + cvar.direction_fov;
					//if( (90.0 <= mainViewAngles[1] && mainViewAngles[1] < 180.0) || (-90.0 <= mainViewAngles[1] && mainViewAngles[1] < 0.0) )
					//angles[1] = mainViewAngles[1] + cvar.direction_fov;


					if(angles[1] >= 180.0) angles[1] -= 360.0;
					if(angles[1] <= -180.0) angles[1] += 360.0;

					gEngfuncs.SetViewAngles(angles);
				}
			}
			else
			{
				if( (0.0 < me.viewAngles[1] && me.viewAngles[1] < 180.0) ) 
				angles[1] = me.viewAngles[1] - 3 * cvar.direction_fov;
				//if( (0.0 < usercmd->viewangles[1] && usercmd->viewangles[1] < 180.0) )
				//angles[1] = usercmd->viewangles[1] - 3 * cvar.direction_fov;
				//if( (0.0 < mainViewAngles[1] && mainViewAngles[1] < 180.0) )
				//angles[1] = mainViewAngles[1] - 3 * cvar.direction_fov;

				if( (-180.0 < me.viewAngles[1] && me.viewAngles[1] < 0.0) )
				angles[1] = me.viewAngles[1] + 3 * cvar.direction_fov;
				//if( (-180.0 < usercmd->viewangles[1] && usercmd->viewangles[1] < 0.0) )
				//angles[1] = usercmd->viewangles[1] + 3 * cvar.direction_fov;
				//if( (-180.0 < mainViewAngles[1] && mainViewAngles[1] < 0.0) )
				//angles[1] = mainViewAngles[1] + 3 * cvar.direction_fov;

				gEngfuncs.SetViewAngles(angles);
			}
		}
	}
}*/

//**********************************************************************************************************************************

/*
bool AutoDirection::Jump(struct usercmd_s* usercmd)
{
	if(wallType(usercmd,1) == 0 || !me.alive) return false;
	
	if(wallType(usercmd,1) == 1)
	{
	   return true;
	}

	return false;
}*/

//**********************************************************************************************************************************
