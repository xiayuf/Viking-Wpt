#include "cvars.h"
#include "client.h"
#include "menu.h"
#include "xorstr.h"
#include "drawing.h"
#include "gui2.h"
extern float flood;
extern float spam;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Â·ï¿½ß¿ï¿½ï¿½ï¿½Ö¸ï¿½ï¿½
extern float wayrec;
extern float wayrecstp;
extern float wayclr1;
extern float wayclr2;
extern float wayclr3;
extern float wayclr4;
extern float wayclr5;
extern float wayclr6;
extern float wayclr7;
extern float wayclr8;
extern float wayclr9;
extern float wayclr10;
extern float wayclrall;
extern float wayload;
extern float savepistol;
extern float avchange_hups;
extern float avchange_hdowns;
extern float avchange_fups;
extern float avchange_fdowns;
extern float avchange_rups;
extern float avchange_rdowns;
extern float avchange_hupd;
extern float avchange_hdownd;
extern float avchange_fupd;
extern float avchange_fdownd;
extern float avchange_rupd;
extern float avchange_rdownd;
extern float avchange_hupj;
extern float avchange_hdownj;
extern float avchange_fupj;
extern float avchange_fdownj;
extern float avchange_rupj;
extern float avchange_rdownj;


extern float avchange_hups1;
extern float avchange_hdowns1;
extern float avchange_fups1;
extern float avchange_fdowns1;
extern float avchange_rups1;
extern float avchange_rdowns1;
extern float avchange_hupd1;
extern float avchange_hdownd1;
extern float avchange_fupd1;
extern float avchange_fdownd1;
extern float avchange_rupd1;
extern float avchange_rdownd1;
extern float avchange_hupj1;
extern float avchange_hdownj1;
extern float avchange_fupj1;
extern float avchange_fdownj1;
extern float avchange_rupj1;
extern float avchange_rdownj1;


extern float avchange_hups2;
extern float avchange_hdowns2;
extern float avchange_fups2;
extern float avchange_fdowns2;
extern float avchange_rups2;
extern float avchange_rdowns2;
extern float avchange_hupd2;
extern float avchange_hdownd2;
extern float avchange_fupd2;
extern float avchange_fdownd2;
extern float avchange_rupd2;
extern float avchange_rdownd2;
extern float avchange_hupj2;
extern float avchange_hdownj2;
extern float avchange_fupj2;
extern float avchange_fdownj2;
extern float avchange_rupj2;
extern float avchange_rdownj2;

extern bool oglSubtractive;

cMenu gMenu;

int cMenu::AddEntry(int n, char title[1000], float* value, float min, float max, float step)
{
	strcpy( menuEntry[n].title,title);
	menuEntry[n].value = value;
	menuEntry[n].min = min;
	menuEntry[n].max = max;
	menuEntry[n].step = step;
	return (n+1);
}

void cMenu::Init()
{
	int i = 0;
	i = AddEntry(i, "=> Wpt", &cvar.waysetting, 0, 1, 1);
	if(cvar.waysetting)
	{
		    i = AddEntry(i, "     Record Start",  &wayrec, 0, 1, 1);
			i = AddEntry(i, "     Record Stop",  &wayrecstp, 0, 1, 1);
		    i = AddEntry(i, "     Clear 1",  &wayclr1, 0, 1, 1);
		    i = AddEntry(i, "     Clear 2",  &wayclr2, 0, 1, 1);
		    i = AddEntry(i, "     Clear 3",  &wayclr3, 0, 1, 1);
		    i = AddEntry(i, "     Clear 4",  &wayclr4, 0, 1, 1);
		    i = AddEntry(i, "     Clear 5",  &wayclr5, 0, 1, 1);
			i = AddEntry(i, "     Clear 6",  &wayclr6, 0, 1, 1);
			i = AddEntry(i, "     Clear 7",  &wayclr7, 0, 1, 1);
			i = AddEntry(i, "     Clear 8",  &wayclr8, 0, 1, 1);
			i = AddEntry(i, "     Clear All ",  &wayclrall, 0, 1, 1);
		}
	i = AddEntry(i, "=> Misc Wpt", &cvar.espshit2, 0, 1, 1);
	if(cvar.espshit2)
	{
		i = AddEntry(i, "     Noall", &cvar.noall, 0, 1, 1);
		i = AddEntry(i, "     Norefresh", &cvar.norefresh, 0, 1, 1);
		i = AddEntry(i, "     Way mode", &cvar.way_mode, 0, 1, 1);
		i = AddEntry(i, "     Runway mode", &cvar.runway_mode, 0, 2, 1);
		i = AddEntry(i, "     Way draw", &cvar.way_draw, 0, 7, 1);
		i = AddEntry(i, "     Auto bq", &cvar.auto_bq, 0, 1, 1);
		i = AddEntry(i, "     Auto br", &cvar.auto_br, 0, 1, 1);
		i = AddEntry(i, "     Way move", &cvar.auto_br, 0, 1, 1);
		i = AddEntry(i, "     Auto jump", &cvar.autojump, 0, 1, 1);
		i = AddEntry(i, "     Nocel", &cvar.nocel, 0, 1, 1);
		i = AddEntry(i, "     Knifeattack", &cvar.knifeattackmode, 0, 1, 1);
		i = AddEntry(i, "     Way lock", &cvar.way_locks, 0, 8, 1);
		i = AddEntry(i, "=> Spin", &cvar.spinsetting, 0, 1, 1);
		if(cvar.spinsetting)
		{
			i = AddEntry(i, "     Type", &cvar.spin, 0, 2, 1);
			i = AddEntry(i, "     Speed", &cvar.spinspeed, 1, 20, 1);
		}
	}
	i = AddEntry(i, "=> Misc", &cvar.espshit, 0, 1, 1);
	if(cvar.espshit)
	{
		i = AddEntry(i, "     Quake guns", &cvar.quake_guns, 0, 1, 1);
		i = AddEntry(i, "     Steps esp", &cvar.soundfilter, 0, 1, 1);
		i = AddEntry(i, "     Chase cam", &cvar.chase_cam, 0, 1, 1);
		i = AddEntry(i, "     Bulletbomb", &cvar.explodingbullets, 0, 1, 1);
		i = AddEntry(i, "     Distance", &cvar.distance, 0, 1, 1);
		i = AddEntry(i, "     Bunny", &cvar.jump, 0, 1, 1);
		i = AddEntry(i, "     Wall", &cvar.walls, 0, 13, 1);
		i = AddEntry(i, "     Nosky", &cvar.nosky, 0, 1, 1);
		i = AddEntry(i, "     Cross", &cvar.cross, 0, 10, 1);
		i = AddEntry(i, "     Namecopy", &cvar.autoname, 0, 1, 1);
		i = AddEntry(i, "     Lambert", &cvar.lambert, 0, 2, 1);
		i = AddEntry(i, "     Nosmoke", &cvar.smoke, 0, 1, 1);
		i = AddEntry(i, "     Noflash", &cvar.flash, 0, 1, 1);
		i = AddEntry(i, "     Esp_line", &cvar.esp_line, 0, 1, 1);
	}
	i = AddEntry(i, "=> Misc2", &cvar.espshit4, 0, 1, 1);
	if(cvar.espshit4)
	{
		i = AddEntry(i, "     Radar", &cvar.radar, 0, 1, 1);
		i = AddEntry(i, "     Weapon", &cvar.weapon, 0, 2, 1);
		i = AddEntry(i, "     Name", &cvar.name, 0, 2, 1);
		i = AddEntry(i, "     Box", &cvar.box, 0, 2, 1);
		i = AddEntry(i, "     Targetline", &cvar.targetline, 0, 1, 1);
		i = AddEntry(i, "     Awpline", &cvar.awpline, 0, 2, 1);
		i = AddEntry(i, "     Grenadetrace", &cvar.grenadetrace, 0, 1, 1);
		i = AddEntry(i, "     Playertrace", &cvar.playertrace, 0, 1, 1);
		i = AddEntry(i, "     Barrel", &cvar.barrel, 0, 1, 1);
		i = AddEntry(i, "     Ghostshells", &cvar.ghostshells, 0, 1, 1);
		i = AddEntry(i, "     Glowshells", &cvar.glowshells, 0, 1, 1);
		i = AddEntry(i, "     Glowshells2", &cvar.glowshells2, 0, 1, 1);
		i = AddEntry(i, "     Localshells", &cvar.localshells, 0, 1, 1);
		i = AddEntry(i, "     Ping", &cvar.ping, 0, 1, 1);
	}
	i = AddEntry(i, "=> Misc3", &cvar.espshit5, 0, 1, 1);
	if(cvar.espshit5)
	{
		i = AddEntry(i, "     Nigger", &cvar.niggermodels, 0, 1, 1);
		i = AddEntry(i, "     Nightmode", &cvar.nightmode, 0, 1, 1);
		i = AddEntry(i, "     Wirewall", &cvar.wireframewall, 0, 1, 1);
		i = AddEntry(i, "     Wiremodels", &cvar.wiremodels, 0, 1, 1);
		i = AddEntry(i, "     Chamz", &cvar.chamz, 0, 1, 1);
		i = AddEntry(i, "     Grenadeshell", &cvar.grenadeshell, 0, 9, 1);
		i = AddEntry(i, "     Bunnyjump", &cvar.jump, 0, 1, 1);
		i = AddEntry(i, "     Sayid", &cvar.sayid, 0, 1, 1);
		i = AddEntry(i, "     Invertedvision", &cvar.invertedvision, 0, 1, 1);
		i = AddEntry(i, "     Camera", &cvar.camera, 0, 1, 1);
		i = AddEntry(i, "     Camerazoom", &cvar.camerazoom, 0, 8, 1);
		i = AddEntry(i, "     Alienvision", &cvar.alienvision, 0, 1, 1);
		i = AddEntry(i, "     Extrablood", &cvar.extrablood, 0, 1, 1);
		i = AddEntry(i, "     Colorblood", &cvar.colorblood, 0, 1, 1);
		i = AddEntry(i, "     Warcrafthuman", &cvar.warcrafthuman, 0, 1, 1);
	}
	i = AddEntry(i, "=> Misc Hud", &cvar.espshit3, 0, 1, 1);
	if(cvar.espshit3)
	{
		i = AddEntry(i, "     Sethud", &cvar.sethud, 0, 1, 1);
		i = AddEntry(i, "     Tophud", &cvar.tophud, 0, 1, 1);
		i = AddEntry(i, "     Vechud", &cvar.vechud, 0, 1, 1);
		i = AddEntry(i, "     Hudbright", &cvar.hud, 0, 1, 1);
	}
	//Ãé×¼Ïà¹Ø
	i = AddEntry(i, "=> Aiming", &cvar.aimingstuff, 0, 1, 1);
	if(cvar.aimingstuff)
	{
		i = AddEntry(i, "=> Aim_mode", &cvar.aim_mode, 1, 3, 1);//1=Hitbox , 2=Bone , 3=vector
		if(cvar.aim_mode == 1)
		{
			i = AddEntry(i, "     Hitbox", &cvar.hitbox, 1, 4, 1); //1=Head , 2=Low Head , 3=Chest , 4=Stomach 
		}
		else if(cvar.aim_mode == 2)
		{
			i = AddEntry(i, "     Bone", &cvar.bone, 1, 7, 1); 
		}
		else if(cvar.aim_mode == 3)
		{
//			i = AddEntry(i, "     VEC-Switch", &cvar.vec_switch, 0, 1, 1);
		}
		if(cvar.aim_mode == 1)
		{
			i = AddEntry(i, "     HitBOX-Height", &cvar.hadj_h, -9, 9, 0.1f);
			i = AddEntry(i, "     HitBOX-Forward", &cvar.hadj_f, -9, 9, 0.1f);
			i = AddEntry(i, "     HitBOX-Right", &cvar.hadj_r, -9, 9, 0.1f);
			i = AddEntry(i, "     H-J-Height", &cvar.judj_h, -9, 9, 0.1f);
			i = AddEntry(i, "     H-J-Forward", &cvar.judj_f, -9, 9, 0.1f);
			i = AddEntry(i, "     H-J-Right", &cvar.judj_r, -9, 9, 0.1f);
		}
		else if(cvar.aim_mode == 2)
		{
			i = AddEntry(i, "     Bone-Height", &cvar.badj_h, -9, 9, 0.1f);
			i = AddEntry(i, "     Bone-Forward", &cvar.badj_f, -9, 9, 0.1f);
			i = AddEntry(i, "     Bone-Right", &cvar.badj_r, -9, 9, 0.1f);
			i = AddEntry(i, "     B-J-Height", &cvar.bjudj_h, -9, 9, 0.1f);
			i = AddEntry(i, "     B-J-Forward", &cvar.bjudj_f, -9, 9, 0.1f);
			i = AddEntry(i, "     B-J-Right", &cvar.bjudj_r, -9, 9, 0.1f);
		}
		else if(cvar.aim_mode == 3)
		{
			i = AddEntry(i, "=> VEC-Maker", &cvar.vecadjust, 0, 1, 1);
			if(cvar.vecadjust)
			{
				i = AddEntry(i, "=> VEC-Stand", &cvar.vecstand, 0, 1, 1);
			    if(cvar.vecstand)
					{
					i = AddEntry(i, "     Height +1", &avchange_hups, 0, 1, 1);
					i = AddEntry(i, "     Height -1", &avchange_hdowns, 0, 1, 1);
					i = AddEntry(i, "     Height +0.1", &avchange_hups1, 0, 1, 1);
					i = AddEntry(i, "     Height -0.1", &avchange_hdowns1, 0, 1, 1);
					i = AddEntry(i, "     Height +0.01", &avchange_hups2, 0, 1, 1);
					i = AddEntry(i, "     Height -0.01", &avchange_hdowns2, 0, 1, 1);
					i = AddEntry(i, "     Forward +1", &avchange_fups, 0, 1, 1);
					i = AddEntry(i, "     Forward -1", &avchange_fdowns, 0, 1, 1);
					i = AddEntry(i, "     Forward +0.1", &avchange_fups1, 0, 1, 1);
					i = AddEntry(i, "     Forward -0.1", &avchange_fdowns1, 0, 1, 1);
					i = AddEntry(i, "     Forward +0.01", &avchange_fups2, 0, 1, 1);
					i = AddEntry(i, "     Forward -0.01", &avchange_fdowns2, 0, 1, 1);
					i = AddEntry(i, "     Right +1", &avchange_rups, 0, 1, 1);
					i = AddEntry(i, "     Right -1", &avchange_rdowns, 0, 1, 1);
					i = AddEntry(i, "     Right +0.1", &avchange_rups1, 0, 1, 1);
					i = AddEntry(i, "     Right -0.1", &avchange_rdowns1, 0, 1, 1);
					i = AddEntry(i, "     Right +0.01", &avchange_rups2, 0, 1, 1);
					i = AddEntry(i, "     Right -0.01", &avchange_rdowns2, 0, 1, 1);
	//				i = AddEntry(i, "     Height 0.01", &cvar.avchange_hups, -10, 10, 1);
	//		        i = AddEntry(i, "     Height 0.1", &cvar.avchange_hups1, -10, 10, 1);
	//		        i = AddEntry(i, "     Height 1", &cvar.avchange_hups2, -10, 10, 1);
	//				i = AddEntry(i, "     Forward 0.01", &cvar.avchange_fups, -10, 10, 1);
	//		        i = AddEntry(i, "     Forward 0.1", &cvar.avchange_fups1, -10, 10, 1);
	//		        i = AddEntry(i, "     Forward 1", &cvar.avchange_fups2, -10, 10, 1);
	//				i = AddEntry(i, "     Right 0.01", &cvar.avchange_rups, -10, 10, 1);
	//		        i = AddEntry(i, "     Right 0.1", &cvar.avchange_rups1, -10, 10, 1);
	//		        i = AddEntry(i, "     Right 1", &cvar.avchange_rups2, -10, 10, 1);
					}
				i = AddEntry(i, "=> VEC-Duck", &cvar.vecduck, 0, 1, 1);
			    if(cvar.vecduck)
					{
					i = AddEntry(i, "     Height +1", &avchange_hupd, 0, 1, 1);
					i = AddEntry(i, "     Height -1", &avchange_hdownd, 0, 1, 1);
					i = AddEntry(i, "     Height +0.1", &avchange_hupd1, 0, 1, 1);
					i = AddEntry(i, "     Height -0.1", &avchange_hdownd1, 0, 1, 1);
					i = AddEntry(i, "     Height +0.01", &avchange_hupd2, 0, 1, 1);
					i = AddEntry(i, "     Height -0.01", &avchange_hdownd2, 0, 1, 1);
					i = AddEntry(i, "     Forward +1", &avchange_fupd, 0, 1, 1);
					i = AddEntry(i, "     Forward -1", &avchange_fdownd, 0, 1, 1);
					i = AddEntry(i, "     Forward +0.1", &avchange_fupd1, 0, 1, 1);
					i = AddEntry(i, "     Forward -0.1", &avchange_fdownd1, 0, 1, 1);
					i = AddEntry(i, "     Forward +0.01", &avchange_fupd2, 0, 1, 1);
					i = AddEntry(i, "     Forward -0.01", &avchange_fdownd2, 0, 1, 1);
					i = AddEntry(i, "     Right +1", &avchange_rupd, 0, 1, 1);
					i = AddEntry(i, "     Right -1", &avchange_rdownd, 0, 1, 1);
					i = AddEntry(i, "     Right +0.1", &avchange_rupd1, 0, 1, 1);
					i = AddEntry(i, "     Right -0.1", &avchange_rdownd1, 0, 1, 1);
					i = AddEntry(i, "     Right +0.01", &avchange_rupd2, 0, 1, 1);
					i = AddEntry(i, "     Right -0.01", &avchange_rdownd2, 0, 1, 1);
	//				i = AddEntry(i, "     Height 0.01", &cvar.avchange_hupd, -10, 10, 1);
	//		        i = AddEntry(i, "     Height 0.1", &cvar.avchange_hupd1, -10, 10, 1);
	//		        i = AddEntry(i, "     Height 1", &cvar.avchange_hupd2, -10, 10, 1);
	//				i = AddEntry(i, "     Forward 0.01", &cvar.avchange_fupd, -10, 10, 1);
	//		        i = AddEntry(i, "     Forward 0.1", &cvar.avchange_fupd1, -10, 10, 1);
	//		        i = AddEntry(i, "     Forward 1", &cvar.avchange_fupd2, -10, 10, 1);
	//				i = AddEntry(i, "     Right 0.01", &cvar.avchange_rupd, -10, 10, 1);
	//		        i = AddEntry(i, "     Right 0.1", &cvar.avchange_rupd1, -10, 10, 1);
	//		        i = AddEntry(i, "     Right 1", &cvar.avchange_rupd2, -10, 10, 1);
					}
				i = AddEntry(i, "=> VEC-Jump", &cvar.vecjump, 0, 1, 1);
			    if(cvar.vecjump)
					{
					i = AddEntry(i, "     Height +1", &avchange_hupj, 0, 1, 1);
					i = AddEntry(i, "     Height -1", &avchange_hdownj, 0, 1, 1);
					i = AddEntry(i, "     Height +0.1", &avchange_hupj1, 0, 1, 1);
					i = AddEntry(i, "     Height -0.1", &avchange_hdownj1, 0, 1, 1);
					i = AddEntry(i, "     Height +0.01", &avchange_hupj2, 0, 1, 1);
					i = AddEntry(i, "     Height -0.01", &avchange_hdownj2, 0, 1, 1);
					i = AddEntry(i, "     Forward +1", &avchange_fupj, 0, 1, 1);
					i = AddEntry(i, "     Forward -1", &avchange_fdownj, 0, 1, 1);
					i = AddEntry(i, "     Forward +0.1", &avchange_fupj1, 0, 1, 1);
					i = AddEntry(i, "     Forward -0.1", &avchange_fdownj1, 0, 1, 1);
					i = AddEntry(i, "     Forward +0.01", &avchange_fupj2, 0, 1, 1);
					i = AddEntry(i, "     Forward -0.01", &avchange_fdownj2, 0, 1, 1);
					i = AddEntry(i, "     Right +1", &avchange_rupj, 0, 1, 1);
					i = AddEntry(i, "     Right -1", &avchange_rdownj, 0, 1, 1);
					i = AddEntry(i, "     Right +0.1", &avchange_rupj1, 0, 1, 1);
					i = AddEntry(i, "     Right -0.1", &avchange_rdownj1, 0, 1, 1);
					i = AddEntry(i, "     Right +0.01", &avchange_rupj2, 0, 1, 1);
					i = AddEntry(i, "     Right -0.01", &avchange_rdownj2, 0, 1, 1);
	//				i = AddEntry(i, "     Height 0.01", &cvar.avchange_hupj, -10, 10, 1);
	//		        i = AddEntry(i, "     Height 0.1", &cvar.avchange_hupj1, -10, 10, 1);
	//		        i = AddEntry(i, "     Height 1", &cvar.avchange_hupj2, -10, 10, 1);
	//				i = AddEntry(i, "     Forward 0.01", &cvar.avchange_fupj, -10, 10, 1);
	//		        i = AddEntry(i, "     Forward 0.1", &cvar.avchange_fupj1, -10, 10, 1);
	//		        i = AddEntry(i, "     Forward 1", &cvar.avchange_fupj2, -10, 10, 1);
	//				i = AddEntry(i, "     Right 0.01", &cvar.avchange_rupj, -10, 10, 1);
	//		        i = AddEntry(i, "     Right 0.1", &cvar.avchange_rupj1, -10, 10, 1);
	//		        i = AddEntry(i, "     Right 1", &cvar.avchange_rupj2, -10, 10, 1);
					}
				i = AddEntry(i, "=> VEC-Save", &cvar.vecmaker, 0, 1, 1);
			    if(cvar.vecmaker)
				{
			        i = AddEntry(i, "     Save Cfg", &savepistol, 0, 1, 1);
				}
			}
		}
		/////////////////////////////////////////////////////////////////////
	        i = AddEntry(i, "=> Mics", &cvar.aimmisc, 0, 1, 1);
			if(cvar.aimmisc)
			{
		        i = AddEntry(i, "=> Pred", &cvar.pred, 0, 1, 1);
	            if(cvar.pred)
				{
			    i = AddEntry(i, "     Predback", &cvar.predback, 0, 5, 0.1f);
			    i = AddEntry(i, "     Pred A Head", &cvar.predahead, 0, 5, 0.1f);
				}
      //	    i = AddEntry(i, "     Bone", &cvar.bone_switch, 0, 1, 1);
      //	    i = AddEntry(i, "     Hitbox", &cvar.hbox_switch, 0, 1, 1);
	            i = AddEntry(i, "     Burstmode", &cvar.burstmode, 1, 5, 1);
	            i = AddEntry(i, "     Rcl_switch", &cvar.rcl_switch, 0, 1, 1);
	            i = AddEntry(i, "     Nospread", &cvar.nospread, 0, 1, 1);
	            i = AddEntry(i, "     Autowall", &cvar.autowall, 0, 1, 1);
	            i = AddEntry(i, "     Vac Wall", &cvar.antiwallblock, 0, 1, 1);
	            i = AddEntry(i, "     Avdraw", &cvar.avdraw, 0, 1, 1);
	            i = AddEntry(i, "     Fov", &cvar.fov, 1, 5, 1);
	            i = AddEntry(i, "     Team attack", &cvar.team, 0, 2, 1);
	            i = AddEntry(i, "     Shield_aim", &cvar.shield_attack, 0, 1, 1);
	            i = AddEntry(i, "     Smooth_aim", &cvar.smooth, 0, 10, 0.5f);
		        i = AddEntry(i, "     Sniper", &cvar.sniper, 0, 1, 1);
			}
	//
	}
    i = AddEntry(i, "=> FuckCD", &cvar.cdon, 0, 1, 1);
	menuItems = i;
}
char pkmenu[] = {'0','.','1',0};
void cMenu::Draw()
{
	if(!Active) return;

	int y = 60;
	int h = 16;
	int w = 160;
	int x = screeninfo.iWidth - w - 2;
	int xx = x+115;
	int offs = 3;
	ColorEntry* color1  = colorList.get(8);
	ColorEntry* color2  = colorList.get(9);
	ColorEntry* color3  = colorList.get(6); 

	for(int b=0; b < menuItems; b++) h+=16; // Font Size

/*	float ubr=cvar.red; float ubg=cvar.green; float ubb=cvar.blue; 
	float lbr=1; float lbg=1; float lbb=1; float ba=0.5;
	float uhr=cvar.red; float uhg=cvar.green; float uhb=cvar.blue; 
	float lhr=1; float lhg=1; float lhb=1; float ha=0.5;
	float bor=1; float bog=1; float bob=1; float boa=1; int line=1;*/
//	gui.Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(x-12,y-3,w+5,h-7,20,ubr,ubg,ubb,lbr,lbg,lbb,ba,uhr,uhg,uhb,lhr,lhg,lhb,ha,bor,bog,bob,boa,line);

	oglSubtractive=true;
	gEngfuncs.pfnFillRGBA(x+0,y+0, 140, 16,255,0,0,128);
	oglSubtractive=false;
	DrawGlowHudString(x+20,y+0,20,20,50,"-Viking Wpt %s-",pkmenu);


	y+=17;
	for(int i=0;i<menuItems;i++)
	{
		ColorEntry * clr;
		if(i == menuSelect)
			clr = colorList.get(21);
		else
			clr = colorList.get(0);

		if(cvar.waysetting || cvar.vecadjust)
		{
			oglSubtractive=true;
	        gEngfuncs.pfnFillRGBA( x, y + (16*i), 140, 16,0,0,255,128);
	        oglSubtractive=false;
			DrawConString( x, y + (16*i), 255, 255, 255,menuEntry[i].title );
			
		}else{
			oglSubtractive=true;
	        gEngfuncs.pfnFillRGBA( x, y + (16*i), 140, 16,0,0,255,128);
	        oglSubtractive=false;
			DrawConString( x, y + (16*i), 255, 255, 255,menuEntry[i].title );
		    DrawConString( xx, y + (16*i), 255, 255, 255, "%0.1f", menuEntry[i].value[0] );
		}


		if( i == menuSelect )
		{
			
			colorBorder( x - 7, y + (16*i)+5,w-4,16,color3->r,color3->g,color3->b,color1->a);
			tintArea( x - 7, y + (16*i)+5,w-4,16,color3->r,color3->g,color3->b,color1->a);
		}
	}
}
int cMenu::KeyEvent(int keynum)
{
	if( keynum == 128 || keynum == 240) //uparrow || mwheelup
	{
		if( menuSelect>0 ) menuSelect--;
		else menuSelect = menuItems - 1;
		return 0;
	}
	else if( keynum == 129 || keynum == 239) //downarrow || mwheeldown
	{
		if( menuSelect<menuItems-1 ) menuSelect++;
		else menuSelect = 0;
		return 0;
	}
	else if( keynum == 130 || keynum == 241 ) //leftarrow || leftbutton
	{
		if( menuEntry[menuSelect].value )
		{
			menuEntry[menuSelect].value[0] -= menuEntry[menuSelect].step;
			if( menuEntry[menuSelect].value[0] < menuEntry[menuSelect].min )
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].max;
		}
		return 0;
	}
	else if( keynum == 131 || keynum == 242 ) //rightarrow || rightbutton
	{
		if( menuEntry[menuSelect].value )
		{
			menuEntry[menuSelect].value[0] += menuEntry[menuSelect].step;
			if( menuEntry[menuSelect].value[0] > menuEntry[menuSelect].max )
				menuEntry[menuSelect].value[0] = menuEntry[menuSelect].min;
		}
		return 0;
	}
	return 1;
}
