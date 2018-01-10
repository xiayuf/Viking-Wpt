/////
///// CBOBY V2 [Revision by ETK]
/////
///// By [boy_scout][boyscout_etk@hotmail.com]
/////
///// [2011][www.etalking.com.ar]
/////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "players.h"

class IdHook
{
public:
	int FirstKillPlayer[MAX_VPLAYERS];
	int SameCheat[MAX_VPLAYERS];
	void ClearPlayer();
	void RelistPlayer();
	void AddPlayer(int ax);
	struct Player;
	struct PlayerEntry{
		PlayerEntry() :player(0){}

		char name[256];
		char content[256];
		Player*  player;
	};

	struct Player {
		Player():parent(0),selection(0),seekselection(0){}
		void boundSelection()
		{ 
			// wrap around
			if(selection<0) { selection = items.size()-1; seekselection = 2;}
			else  if(selection >= (int)items.size()) { selection = 0; seekselection = 0;}
			if(selection==items.size()-1){seekselection = 2;}
			else if(selection==items.size()-2){seekselection = 1;}
			else {seekselection = 0;}
		}
		void boundSelection1()
		{ 
			// wrap around
			if(selection<0) { selection = 0; }
			else  if(selection >= (int)items.size()-3) 
			{ 
				if(seekselection == 0)
					selection = items.size()-3; 
				else if(seekselection == 1)
					selection = items.size()-2; 
				else if(seekselection == 2)
					selection = items.size()-1; 
			}
			else
			{
				if(seekselection == 1)
					selection = items.size()-2; 
				else if(seekselection == 2)
					selection = items.size()-1; 			}
		}

		Player*  parent;
		string name; 
		int    selection;
		int seekselection;
		vector<PlayerEntry> items;
	};

	IdHook():basePlayer(0){}
	void init();

	struct Player* basePlayer;
};

void func_relistplayer();
void func_clearallplayer();
void func_addplayer();
void player_describe_current();
void func_player_list();
void func_player_select();
void func_player_back();
void func_player_up();
void func_player_down();
void relistplayermenu();
bool gPlayerActive();
void drawPlayer(int x, int y, int w);
void func_first_kill_mode();
extern bool  player_active;
extern char  gHudMessage[256];

extern IdHook idhook;