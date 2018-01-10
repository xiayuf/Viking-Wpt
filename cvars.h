#ifndef CVARS_H
#define CVARS_H

typedef unsigned long DWORD;

class CVARS // cvars (of course ;P)
{
public:
	void Init();
//	void Save();
//	void Load();

public:

 	float name; 
	float weapon; 
	float radar; 
	float radar_x; 
	float radar_y; 
	float radar_size; 
	float flash;
	float chase_cam;
	float avdraw;
	float noall;
	float lambert;
	float antiwallblock;
	float rcl_switch;
	float nosky;
	float vec_switch;
//	float bone_switch;
//	float hbox_switch;
	float spin;
	float menukey;
	float consolekey;
	float jump;
	// menu sub shit
	float aimingstuff;
	float waysetting;
	float espshit;
	float espshit2;
	float espshit3;
	float espshit4;
	float espshit5;
	float vecadjust;
	float aimmisc;
	float vecduck;
	float vecjump;
	float vecstand;
	float vecmaker;
	float spinsetting;
	float speedx;
	float speedset;
	//AIMBOT cvars
	float nospread;
	float recoil;
	float autowall;
	float aim;
	float hitbox;
	float hadj_h;
	float hadj_f;
	float hadj_r;
	float judj_h;
	float judj_f;
	float judj_r;
	float badj_h;
	float badj_f;
	float badj_r;
	float bjudj_h;
	float bjudj_f;
	float bjudj_r;
	float shoot;
	float bone;
	float aim_mode;
	float pred;
	float predahead;
	float predback;
	float team;
	float reload;
/*	float avchange_hups;
	float avchange_hups1;
	float avchange_hups2;
	float avchange_hupd;
	float avchange_hupd1;
	float avchange_hupd2;
	float avchange_hupj;
	float avchange_hupj1;
	float avchange_hupj2;
	float avchange_fups;
	float avchange_fups1;
	float avchange_fups2;
	float avchange_fupd;
	float avchange_fupd1;
	float avchange_fupd2;
	float avchange_fupj;
	float avchange_fupj1;
	float avchange_fupj2;
	float avchange_rups;
	float avchange_rups1;
	float avchange_rups2;
	float avchange_rupd;
	float avchange_rupd1;
	float avchange_rupd2;
	float avchange_rupj;
	float avchange_rupj1;
	float avchange_rupj2;*/

	//Speed cvars
	float speed;
	float aspeed;
	//way
	float way_mindist;
	float way_maxdist;
	float way_fov;
    float way_enable;
	float way_draw;
	float way;
    float way_mode; 
	float record_dist;
	//
	float sayid;
	float cdon;
	float rush;
	float smallview;

	float colors;
	float red;
	float green;
	float blue;
	float height_correction;
	float autoburst;    
	float ftime;
	float dtime; 
	float sdelay;
	float rdelay;
	float sglow;
	float tglow;
	float fov;
	float shield_attack;
	float autojump;
	float walls;
	float cross;
	float quake_guns;
	float box;
	float explodingbullets;
	float burstmode;
	float smoke;
	float norefresh;
	float autoname;
	float awpline;
//	float way_for;
//	float finddist;
//	float way_dist;
//	float route_mode;
//	float route_draw;
//	float way_switch;
	float tophud;
	float sethud;
	float niggermodels;
	float nightmode;
	float wireframewall;
	float wiremodels;
	float vechud;
	float playertrace;
	float glowshells;
	float glowshells2;
	float localshells;
	float spinspeed;
	float autoway;
	float invertedvision;
	float camera;
	float camerazoom;
	float alienvision;
	float extrablood;
	float colorblood;
	float targetline;
	float sniper;
	float smooth;
	float sounddisplay;
	float soundradar;
	float soundtol;
	float soundfilter;
	float soundtime;
	float distance;
	float knifeattackdist;
	float knifeattackmode;
	float hud;
	float way_neardist;
	float way_findangle;
	float rush_step;
	float way_locks;
	float auto_br;
	float auto_bq;
	float way_move;
	float runway_mode;
	float nocel;
	float barrel;
	float grenadetrace;
	float weaponname;
	float ghostshells;
	float grenadeshell;
	float chamz;
	float warcrafthuman;
	float ping;
	float esp_line;

	
    int crosshair;
	int burstflags;
	int find_mode;
    int id_mode;
	int wall;
/*	int linecount;
	int pointcount;
	int waypursue;*/
	int way_moder;
	int shield_attackr;
	int noallr;
	int way_on;
	int way_lock;
	int action_step;
	int stop_auto_bq;
	int way_restore;
	int direction_step;
	int jump_dist;
	int burstmoder;
	int norefreshr;
};

extern CVARS cvar;

#endif
