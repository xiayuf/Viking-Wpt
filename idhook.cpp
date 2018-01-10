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
#include "color.h"
#include "idhook.h"
#include "cvars.h"
#include "interpreter.h"
#include "misc/defs.h"
#include "calcscreen.h"
#include "players.h"
#include "drawing.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IdHook idhook;
typedef IdHook::Player Player;
typedef IdHook::PlayerEntry PlayerEntry;

static IdHook::Player* curPlayer = idhook.basePlayer;
bool player_active = false;
extern bool oglSubtractive;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_relistplayer() //ˢ�������б�
{
	idhook.RelistPlayer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_clearallplayer() //��������רɱ/ר��ɱ����
{
	idhook.ClearPlayer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_addplayer() //����רɱ/ר��ɱ����
{
	if(!cmd.argS(1).empty())
		idhook.AddPlayer(cmd.argI(1));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void player_describe_current()
{
	if(!player_active||!curPlayer) return;
	IdHook::PlayerEntry& entry = curPlayer->items[curPlayer->selection];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int selection=0;
int seekselection=0;

void func_player_list() //����רɱ/ר��ɱ�����б��˵�
{
	if(!player_active)
	{
		func_player_select();
	}
	else
	{
		selection=curPlayer->selection;
		seekselection=curPlayer->seekselection;
		player_active=false;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_player_select() //ѡ��
{
	if(!player_active)
	{
		idhook.init();
	}
//	if(!curPlayer) //����BUG
//	{
		curPlayer = idhook.basePlayer;
//		player_active = true;
		curPlayer->boundSelection();
		player_describe_current();
//		return;
//	}
	if(player_active)
	{
		IdHook::PlayerEntry& entry = curPlayer->items[curPlayer->selection];
		if(entry.player)
		{
			curPlayer = entry.player;
			curPlayer->boundSelection();
			player_describe_current();
		}
		else
		{
			int i=curPlayer->selection;
			int j=curPlayer->seekselection;
			cmd.exec(const_cast<char*>(entry.content));
			player_active=false;
			func_player_select();
			curPlayer->selection=i;
			curPlayer->seekselection=j;
			curPlayer->boundSelection1();
			player_describe_current();
		}
	}
	else
	{
		curPlayer->selection=selection;
		curPlayer->seekselection=seekselection;
		curPlayer->boundSelection1();
		player_describe_current();
	}
	player_active=true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_player_back() //���ϼ�
{
	if(!player_active || !curPlayer) return;
	curPlayer = curPlayer->parent;
	if(!curPlayer) 
	{ 
		curPlayer = idhook.basePlayer; 
		player_active=false; 
	}
	selection=curPlayer->selection;
	seekselection=curPlayer->seekselection;
	curPlayer->boundSelection();
	player_describe_current();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_player_up() //��������һ��
{
	if(!player_active || !curPlayer) return;
	--curPlayer->selection;
	curPlayer->boundSelection();
	player_describe_current();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_player_down() //��������һ��
{
	if(!player_active || !curPlayer) return;
	++curPlayer->selection;
	curPlayer->boundSelection();
	player_describe_current();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void relistplayermenu() //�Զ�ˢ�²˵�
{
	if(player_active)
	{
		int i=curPlayer->selection;
		int j=curPlayer->seekselection;
		idhook.RelistPlayer();
		player_active=false;
		func_player_select();
		curPlayer->selection=i;
		curPlayer->seekselection=j;
		curPlayer->boundSelection1();
		player_describe_current();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool gPlayerActive()
{ 
	return player_active; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void drawPlayer(int x, int y, int w)
{
	if (!player_active)
		return;

	if (!curPlayer)
		curPlayer = idhook.basePlayer;

	vector<IdHook::PlayerEntry>& items = curPlayer->items;

	int player_y = 30;
	int player_w = 350;

	int cW = screeninfo.iWidth/2.3;
	x = displayCenterX - cW/2;
	y = player_y;
	w = player_w;

	enum{ TEXT_H = 16 };
	int title_h = TEXT_H-1; //��������λ��
	int frame_y = player_y - title_h;
	int frame_h = items.size()*TEXT_H + title_h;

	ColorEntry* color1 = colorList.get(8); // "menu1"
	ColorEntry* color2 = colorList.get(9); // "menu2"
	ColorEntry* color3 = colorList.get(6);

	oglSubtractive = true;
	gEngfuncs.pfnFillRGBA(x,y-17,w,17,255,0,0,128);
//	glFillArea(x,y-17,w,17,color1->r,color1->g,color1->b,color1->a,color2->r,color2->g,color2->b,color2->a); //����ɫ��
//	gEngfuncs.pfnFillRGBA(x, frame_y-3, w, 1, 255, 255, 255, 255); //���ⶥ�ϱ�ɫ��
	oglSubtractive = false;

	int h1 = curPlayer->selection*TEXT_H;
	if(h1)
	{
		oglSubtractive = true;
		gEngfuncs.pfnFillRGBA(x,y,w,h1,0,0,255,128);
//		glFillArea(x,y,w,h1,color1->r,color1->g,color1->b,color1->a,color2->r,color2->g,color2->b,color2->a);//�����µ�����ɫ��
		oglSubtractive = false;
		y+=h1;
	}

	tintArea(x,y,w,TEXT_H,color3->r,color3->g,color3->b,color3->a); //ѡ����
	colorBorder(x,y,w,TEXT_H,color3->r,color3->g,color3->b,color3->a); //ѡ����
	y+=TEXT_H;

	int numItemsLeft = items.size() - curPlayer->selection - 1;
	if(numItemsLeft>0)
	{
		oglSubtractive = true;
		gEngfuncs.pfnFillRGBA(x,y,w,numItemsLeft*TEXT_H,255,0,0,128);
//		glFillArea(x,y,w,numItemsLeft*TEXT_H,color1->r,color1->g,color1->b,color1->a,color2->r,color2->g,color2->b,color2->a);//�����µ�����ɫ��
		oglSubtractive = false;
	}

	DrawGlowHudString(x+80,frame_y+1,20,20,50,const_cast<char*>(curPlayer->name.c_str()));
	y=player_y;
	x+=4;
	ColorEntry* clr;
	for(int i=0;i<items.size();i++)
	{
		IdHook::PlayerEntry& item = items[i];
		
		if(strstr(item.name," T"))
			clr = colorList.get(2); // "T_alive"
		else if(strstr(item.name,"CT"))
			clr = colorList.get(3); // "CT_alive"
		else
			clr = colorList.get(15); //����ɫ��

		gEngfuncs.pfnDrawSetTextColor(clr->onebased_r,clr->onebased_g,clr->onebased_b);
		gEngfuncs.pfnDrawConsoleString(x,y,const_cast<char*>(item.name));
		y+=TEXT_H;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void func_first_kill_mode()
{
	if(cvar.id_mode==0)
		cvar.id_mode=1; //��ɱ��Ұ�е�רɱ����
	else if(cvar.id_mode==1)
		cvar.id_mode=2; //ֻɱרɱ������������ɱ
	else
		cvar.id_mode=0; //��ͨģʽ
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void listPlayer(Player* pPlayer)
{
	PlayerEntry newEntry;

	sprintf(newEntry.content,"relistplayer");

	sprintf(newEntry.name,"Attack ID Mode ( %d )",cvar.id_mode);
	sprintf(newEntry.content,"first_kill_mode");
	newEntry.player = 0;
	pPlayer->items.push_back(newEntry);

	sprintf(newEntry.name,"Clear ID");
	sprintf(newEntry.content,"clearallplayer");
	newEntry.player = 0;
	pPlayer->items.push_back(newEntry);

	char *teamname;
//	char *deathstate; bug
	for (int ax=0;ax<vPlayers.size();ax++)
	{
		PlayerInfo& r = vPlayers[ax];
		r.updateEntInfo();
		if(strcmp(vPlayers[ax].entinfo.name,"\\missing-name\\") && vPlayers[ax].entinfo.name[0]!='\0' && vPlayers[ax].team>0 && vPlayers[ax].team==1)
		{
			teamname=" T";//T
		/*	if(!vPlayers[ax].isAlive())bug
				deathstate="死亡";
			else
				deathstate="     ";*/
			if(idhook.FirstKillPlayer[ax]==0)
				sprintf(newEntry.name,"              %s      %s",teamname,vPlayers[ax].entinfo.name);
			else if(idhook.FirstKillPlayer[ax]==1)
				sprintf(newEntry.name," On>      %s      %s",teamname,vPlayers[ax].entinfo.name);
			else
				sprintf(newEntry.name," Off>      %s      %s",teamname,vPlayers[ax].entinfo.name);
			sprintf(newEntry.content,"addplayer %d",ax);
			newEntry.player = 0;
			pPlayer->items.push_back(newEntry);
		}
	}
	for (int x=0;x<vPlayers.size();x++)
	{
		PlayerInfo& r = vPlayers[x];
		r.updateEntInfo();
		if(strcmp(vPlayers[x].entinfo.name,"\\missing-name\\") && vPlayers[x].entinfo.name[0]!='\0' && vPlayers[x].team>0 && vPlayers[x].team==2)
		{
			teamname="CT";//CT
	/*		if(!vPlayers[x].isAlive())bug
				deathstate="死亡";
			else
				deathstate="     ";*/
			if(idhook.FirstKillPlayer[x]==0)
				sprintf(newEntry.name,"              %s      %s",teamname,vPlayers[x].entinfo.name);
			else if(idhook.FirstKillPlayer[x]==1)
				sprintf(newEntry.name," on>      %s      %s",teamname,vPlayers[x].entinfo.name);
			else
				sprintf(newEntry.name," Off>      %s      %s",teamname,vPlayers[x].entinfo.name);
			sprintf(newEntry.content,"addplayer %d",x);
			newEntry.player = 0;
			pPlayer->items.push_back(newEntry);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IdHook::init()
{
	if(basePlayer) delete basePlayer;
	basePlayer = new Player;
	basePlayer->name = "     Viking Wpt IDhook";
	listPlayer(basePlayer);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IdHook::AddPlayer(int ax)
{
	if(FirstKillPlayer[ax]==0)
		FirstKillPlayer[ax]=1;
	else if(FirstKillPlayer[ax]==1)
		FirstKillPlayer[ax]=2;
	else
		FirstKillPlayer[ax]=0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IdHook::ClearPlayer()
{
	for(int i=0;i<MAX_VPLAYERS;i++)
	{
		FirstKillPlayer[i]=0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IdHook::RelistPlayer()
{
	for(int i=0;i<MAX_VPLAYERS;i++)
	{
		if(i>=vPlayers.size())
			return;
		if(vPlayers[i].entinfo.name[0]=='\0')
			FirstKillPlayer[i]=0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



