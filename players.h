#ifndef _PLAYERS_
#define _PLAYERS_

#include "client.h"
#include "timehandling.h" //soundesp
#include <string.h>

struct spread_info
{
	unsigned int random_seed;
	int recoil;
	float gtime;
	float prevtime;
	float brokentime; // This is only supposed to be set to zero when you buy the sg550 or the g3sg1
						// not when you reload, switch weapons or pick up a weapon, this is do to the
						// cs bugs for these guns (valve fix your code please)
	float spreadvar;
	float recoiltime;
	bool firing;
	int WeaponState;
	int prcflags;
};


struct local_player_info
{
	struct cl_entity_s *ent;
	float punchangle[3];
	int entindex;
	bool alive;
	int pmFlags; 
	int pmMoveType;
	float pmVelocity[3];
	float pmEyePos[3];
	float viewAngles[3];
	int iClip;
	int iFOV;
	bool inZoomMode;
	int team;
	float sin_yaw, minus_cos_yaw;
	spread_info spread;
	int prcflags;
	int currentWeaponID;

	int headshots;
	float hspercent;
};

enum 
{ 
     SEQUENCE_IDLE     = 0, 
     SEQUENCE_SHOOT    = 1, 
     SEQUENCE_RELOAD   = 2, 
     SEQUENCE_DIE      = 4, 
     SEQUENCE_THROW    = 8, 
     SEQUENCE_ARM_C4   = 16, 
     SEQUENCE_SHIELD   = 32,
	 SEQUENCE_SHIELD_SIDE = 64
};
enum //soundesp
{ 
	UPDATE_MISSING=0, 
	UPDATE_ADDENT=1, 
	UPDATE_SOUND_RADAR=2
};
//Players Info
class PlayerInfo 
{
protected:
	friend class VecPlayers;
	void init( int _entindex) 
	{ 
		team=2;
		entinfo.name="\\missing-name\\";
		entinfo.model="missing-model";
		alive=false;
		entindex = _entindex;
		distance = 1000.0;
		visible  = 0;
		strcpy(m_weapon,"N-A");
		m_updated = false;
		bGotHead = false;
		fixHbAim = false;
		canAim = false;
		frags=0;
		deaths=0;
		ratio=0;
		bestplayer=false;
        USEID=false;
	}
public:
	hud_player_info_t entinfo;

	void updateEntInfo();
	void SuspectNextOrigin(int index,float* pflOutOrigin );
	const char* getName() const { return entinfo.name; }
	short GetPing() { return entinfo.ping; }

	int    team;
	int    iInfo;
	float  distance;
	bool    visible;
	float  fovangle;   // minimum fov a player is in
	vec3_t vHitbox;
	bool bGotHead;
	bool fixHbAim;
    bool USEID;
	int canAim;
	int entindex;

	int    frags;
	int    deaths;
	float  ratio;
	bool   bestplayer; // based on ratio, not frags!

	void setAlive() { alive = true;  }
	void setDead() { alive = false;  }
	bool isAlive () { return  alive; }

	struct cl_entity_s * getEnt() { return gEngfuncs.GetEntityByIndex(entindex); }

	// weapon update:
	
	const char* getWeapon() { return m_weapon; }
	void setWeapon(const char* newname)
	{
		register int len = strlen(newname);
		if(len && len<30) { strcpy(m_weapon,newname); }
	}

	void   updateAddEntity (const float* neworg)
	{ 
		if(alive)
		{			
			VectorCopy(neworg,m_origin); 
			m_updated = true;
		}
	}	
	// position update reporting
	void   updateClear    ()
	{ 
		m_updated = false; 
	}

	bool   isUpdated       ()    { return m_updated!=UPDATE_MISSING; }
    bool   isUpdatedAddEnt ()    { return m_updated==UPDATE_ADDENT;  }
	float * origin()       { return m_origin; }

private:
	float   m_origin[3];
	char    m_weapon[32];
	bool    m_updated;

	bool alive;	

	int     m_lastUpdateType;
	double  m_lastUpdateTime;
	DWORD dwUpdatetime;
	float flSoundpos[3];
	float flOffset[3];
	float flLastSoundpos[3];

public:
	PlayerInfo() { init(0); }
};

enum{  MAX_VPLAYERS =36 };
class VecPlayers
{
private:
	PlayerInfo* players;
public:
	~VecPlayers() { delete[] players; }
	VecPlayers() 
	{ 
		players = new PlayerInfo[MAX_VPLAYERS];
		for(int i=0;i<MAX_VPLAYERS;i++) { players[i].init(i); }
	}

	inline PlayerInfo& operator [] (unsigned int i)
	{
		if(i>=MAX_VPLAYERS) {return players[0];}
		else                {return players[i];}
	}
	
	inline unsigned int size() { return MAX_VPLAYERS; }
};
extern VecPlayers vPlayers;
//==============================================================================
extern local_player_info me;

#endif