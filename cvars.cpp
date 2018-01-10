/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <fstream>
#include <windows.h>
#include "cvars.h"
#include "interpreter.h"
#include "client.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CVARS cvar;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CVARS::Init()
{
	memset((char*)this, 0, sizeof(*this)); 
	#define REGISTER_CVAR_FLOAT(name,defaultvalue) cmd.AddCvarFloat(#name, &##name );name=defaultvalue;
	REGISTER_CVAR_FLOAT(recoil,1)
	REGISTER_CVAR_FLOAT(aspeed,32)	
	REGISTER_CVAR_FLOAT(weapon,0)	
	REGISTER_CVAR_FLOAT(name,1)
	REGISTER_CVAR_FLOAT(spin,0)
	REGISTER_CVAR_FLOAT(radar,1)
	REGISTER_CVAR_FLOAT(radar_x,90)
	REGISTER_CVAR_FLOAT(radar_y,100)
	REGISTER_CVAR_FLOAT(radar_size,120)
	REGISTER_CVAR_FLOAT(flash,0)	
	REGISTER_CVAR_FLOAT(aim,1)
	REGISTER_CVAR_FLOAT(hitbox,1)	 
	REGISTER_CVAR_FLOAT(hadj_h,3.0)
	REGISTER_CVAR_FLOAT(hadj_f,0.5)
	REGISTER_CVAR_FLOAT(hadj_r,0.5)	
	REGISTER_CVAR_FLOAT(shoot,1)	
	REGISTER_CVAR_FLOAT(autowall,1)
	REGISTER_CVAR_FLOAT(antiwallblock,0)
	REGISTER_CVAR_FLOAT(speed,512)
	REGISTER_CVAR_FLOAT(nospread,1)
	REGISTER_CVAR_FLOAT(chase_cam,0)
	REGISTER_CVAR_FLOAT(avdraw,1)			
	REGISTER_CVAR_FLOAT(lambert,0)
	REGISTER_CVAR_FLOAT(bone,7)
	REGISTER_CVAR_FLOAT(jump,1)
	REGISTER_CVAR_FLOAT(aim_mode,3)
	REGISTER_CVAR_FLOAT(pred,0)
	REGISTER_CVAR_FLOAT(predback,0)
	REGISTER_CVAR_FLOAT(predahead,0)	
	REGISTER_CVAR_FLOAT(judj_f,0)	
	REGISTER_CVAR_FLOAT(judj_r,0)	
	REGISTER_CVAR_FLOAT(judj_h,0)
	REGISTER_CVAR_FLOAT(team,0)
	REGISTER_CVAR_FLOAT(bjudj_f,0)	
	REGISTER_CVAR_FLOAT(bjudj_r,0)	
	REGISTER_CVAR_FLOAT(bjudj_h,0)
	REGISTER_CVAR_FLOAT(badj_f,0)	
	REGISTER_CVAR_FLOAT(badj_r,0)	
	REGISTER_CVAR_FLOAT(badj_h,0)
	REGISTER_CVAR_FLOAT(rcl_switch,1)
	REGISTER_CVAR_FLOAT(nosky,1)
	REGISTER_CVAR_FLOAT(vec_switch,1)
//	REGISTER_CVAR_FLOAT(bone_switch,0)
//	REGISTER_CVAR_FLOAT(hbox_switch,0)
	REGISTER_CVAR_FLOAT(menukey,147)
	REGISTER_CVAR_FLOAT(consolekey,148)
	REGISTER_CVAR_FLOAT(reload,1)
	REGISTER_CVAR_FLOAT(way_mindist,60.0);
	REGISTER_CVAR_FLOAT(way_maxdist,120.0);
	REGISTER_CVAR_FLOAT(record_dist ,50.0);
	REGISTER_CVAR_FLOAT(way_fov,20.0);
	REGISTER_CVAR_FLOAT(way_enable ,1);
	REGISTER_CVAR_FLOAT(way_draw ,8);
    REGISTER_CVAR_FLOAT(way_mode ,1);
	REGISTER_CVAR_FLOAT(sayid ,0);
	REGISTER_CVAR_FLOAT(cdon ,0);
	REGISTER_CVAR_FLOAT(rush ,0);
	REGISTER_CVAR_FLOAT(smallview ,0);
	REGISTER_CVAR_FLOAT(colors,0);
	REGISTER_CVAR_FLOAT(red,0);
	REGISTER_CVAR_FLOAT(green,0);
	REGISTER_CVAR_FLOAT(blue,0);
	REGISTER_CVAR_FLOAT(height_correction,0.0);
    REGISTER_CVAR_FLOAT( autoburst,0);
	REGISTER_CVAR_FLOAT( ftime     ,1.0)
	REGISTER_CVAR_FLOAT( dtime     ,0.00)
	REGISTER_CVAR_FLOAT( sdelay    ,0.00)
	REGISTER_CVAR_FLOAT( rdelay    ,0.00)
	REGISTER_CVAR_FLOAT( sglow                  ,0)
	REGISTER_CVAR_FLOAT( tglow                  ,0)
	REGISTER_CVAR_FLOAT( fov                  ,5)
	REGISTER_CVAR_FLOAT( shield_attack                  ,1)
	REGISTER_CVAR_FLOAT( autojump                  ,1)
	REGISTER_CVAR_FLOAT( walls                  ,1)//for menu is wall
	REGISTER_CVAR_FLOAT( cross                  ,7)//for menu is crosshair
	REGISTER_CVAR_FLOAT( quake_guns                  ,0)
	REGISTER_CVAR_FLOAT( box                  ,0)
	REGISTER_CVAR_FLOAT( explodingbullets                  ,0)
	REGISTER_CVAR_FLOAT( burstmode                  ,1)
	REGISTER_CVAR_FLOAT( smoke                  ,0)
	REGISTER_CVAR_FLOAT( noall                  ,1)
	REGISTER_CVAR_FLOAT( norefresh                  ,0)
	REGISTER_CVAR_FLOAT( autoname                  ,0)
	REGISTER_CVAR_FLOAT( awpline                 ,0)
//	REGISTER_CVAR_FLOAT( way_dist, 48.0); //点之间距离
//	REGISTER_CVAR_FLOAT( finddist, 120.0);  //最大寻点距离
//	REGISTER_CVAR_FLOAT( way_for, 80.0); //捕获角度
//	REGISTER_CVAR_FLOAT( route_mode, 1); //捕获角度
//	REGISTER_CVAR_FLOAT( route_draw, 1); //捕获角度
//	REGISTER_CVAR_FLOAT( way_switch, 1); //捕获角度
//  REGISTER_CVAR_FLOAT( autoway, 0); //捕获角度
    REGISTER_CVAR_FLOAT( tophud, 0);
    REGISTER_CVAR_FLOAT( sethud, 0);
    REGISTER_CVAR_FLOAT( niggermodels, 0);
    REGISTER_CVAR_FLOAT( nightmode, 0);
    REGISTER_CVAR_FLOAT( wireframewall, 0);
    REGISTER_CVAR_FLOAT( wiremodels, 0);
    REGISTER_CVAR_FLOAT( vechud, 1);
    REGISTER_CVAR_FLOAT( playertrace, 0);
    REGISTER_CVAR_FLOAT( glowshells, 0);
    REGISTER_CVAR_FLOAT( glowshells2, 0);
    REGISTER_CVAR_FLOAT( localshells, 0);
    REGISTER_CVAR_FLOAT( spinspeed, 5);
    REGISTER_CVAR_FLOAT( invertedvision, 0);
    REGISTER_CVAR_FLOAT( camera, 0);
    REGISTER_CVAR_FLOAT( camerazoom, 0);
    REGISTER_CVAR_FLOAT( alienvision, 0);
    REGISTER_CVAR_FLOAT( extrablood, 0);
    REGISTER_CVAR_FLOAT( colorblood, 0);
 /*   REGISTER_CVAR_FLOAT( avchange_hups, 0);
    REGISTER_CVAR_FLOAT( avchange_hups1, 0);
    REGISTER_CVAR_FLOAT( avchange_hups2, 0);
    REGISTER_CVAR_FLOAT( avchange_hupd, 0);
    REGISTER_CVAR_FLOAT( avchange_hupd1, 0);
    REGISTER_CVAR_FLOAT( avchange_hupd2, 0);
    REGISTER_CVAR_FLOAT( avchange_hupj, 0);
    REGISTER_CVAR_FLOAT( avchange_hupj1, 0);
    REGISTER_CVAR_FLOAT( avchange_hupj2, 0);
	REGISTER_CVAR_FLOAT( avchange_fups, 0);
    REGISTER_CVAR_FLOAT( avchange_fups1, 0);
    REGISTER_CVAR_FLOAT( avchange_fups2, 0);
    REGISTER_CVAR_FLOAT( avchange_fupd, 0);
    REGISTER_CVAR_FLOAT( avchange_fupd1, 0);
    REGISTER_CVAR_FLOAT( avchange_fupd2, 0);
    REGISTER_CVAR_FLOAT( avchange_fupj, 0);
    REGISTER_CVAR_FLOAT( avchange_fupj1, 0);
    REGISTER_CVAR_FLOAT( avchange_fupj2, 0);
    REGISTER_CVAR_FLOAT( avchange_rups, 0);
    REGISTER_CVAR_FLOAT( avchange_rups1, 0);
    REGISTER_CVAR_FLOAT( avchange_rups2, 0);
    REGISTER_CVAR_FLOAT( avchange_rupd, 0);
    REGISTER_CVAR_FLOAT( avchange_rupd1, 0);
    REGISTER_CVAR_FLOAT( avchange_rupd2, 0);
    REGISTER_CVAR_FLOAT( avchange_rupj, 0);
    REGISTER_CVAR_FLOAT( avchange_rupj1, 0);
    REGISTER_CVAR_FLOAT( avchange_rupj2, 0);*/
    REGISTER_CVAR_FLOAT( targetline, 0);
    REGISTER_CVAR_FLOAT( sniper, 0);
    REGISTER_CVAR_FLOAT( smooth, 0);
    REGISTER_CVAR_FLOAT( sounddisplay, 1);
    REGISTER_CVAR_FLOAT( soundradar, 1);
	REGISTER_CVAR_FLOAT( soundtol             ,200)
	REGISTER_CVAR_FLOAT( soundfilter            ,1)
    REGISTER_CVAR_FLOAT( soundtime          ,1.0)
    REGISTER_CVAR_FLOAT( distance          ,0)
    REGISTER_CVAR_FLOAT( knifeattackdist          ,2000.0)
    REGISTER_CVAR_FLOAT( knifeattackmode          ,1)
    REGISTER_CVAR_FLOAT( hud          ,1)
	REGISTER_CVAR_FLOAT(way_neardist,500.0)
	REGISTER_CVAR_FLOAT(way_findangle,180.0)
	REGISTER_CVAR_FLOAT(rush_step,500.0)//500
	REGISTER_CVAR_FLOAT(way_locks,0)//for menu is wau_lock
	REGISTER_CVAR_FLOAT(auto_br,1)
	REGISTER_CVAR_FLOAT(auto_bq,1)
	REGISTER_CVAR_FLOAT(way_move,1)
	REGISTER_CVAR_FLOAT(runway_mode,1)
	REGISTER_CVAR_FLOAT(barrel,0)
	REGISTER_CVAR_FLOAT(nocel,0)
	REGISTER_CVAR_FLOAT(grenadetrace,0)
	REGISTER_CVAR_FLOAT(ghostshells,0)
	REGISTER_CVAR_FLOAT(grenadeshell,0)
	REGISTER_CVAR_FLOAT(chamz,0)
	REGISTER_CVAR_FLOAT(warcrafthuman,0)
	REGISTER_CVAR_FLOAT(ping,0)
	REGISTER_CVAR_FLOAT(esp_line,0)

	#define REGISTER_CVAR_INT(name,defaultvalue) cmd.AddCvarInt(#name, &##name );name=defaultvalue;
	REGISTER_CVAR_INT(crosshair,7);
    REGISTER_CVAR_INT( burstflags,0)
	REGISTER_CVAR_INT( find_mode                  ,0)
	REGISTER_CVAR_INT( id_mode                ,1)
	REGISTER_CVAR_INT( wall                ,1)
/*	REGISTER_CVAR_INT( pointcount               ,510);  //路线长度
	REGISTER_CVAR_INT( linecount                ,5);    //允许录制路线X条
	REGISTER_CVAR_INT( waypursue                ,0);    //脱线开关*/
    REGISTER_CVAR_INT( way_moder, 1); //for newhud
    REGISTER_CVAR_INT( shield_attackr, 1); //for newhud
    REGISTER_CVAR_INT( noallr, 1); //for newhud
	REGISTER_CVAR_INT(way_on ,0);
	REGISTER_CVAR_INT(way_lock,0);
	REGISTER_CVAR_INT(action_step,50)
	REGISTER_CVAR_INT(stop_auto_bq,0)
	REGISTER_CVAR_INT(way_restore,1)
	REGISTER_CVAR_INT(direction_step,18.0)// 0.1=10 ; 007=18 ; cboby=15
	REGISTER_CVAR_INT(jump_dist,30)
	REGISTER_CVAR_INT(burstmoder,1)
	REGISTER_CVAR_INT(norefreshr,0)

	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

