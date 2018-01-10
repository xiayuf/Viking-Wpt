/************************************************************************************************************************************
*
*
* 007 HOOK
* Code By: 007 + boy_scout
* msn: david_bs@live.com
* (c)2011
* www.etalking.com.ar
*
*
************************************************************************************************************************************/

#ifndef WAYPOINT_H
#define WAYPOINT_H

//**********************************************************************************************************************************

class WAYPOINT
{
public:
	float relativeSpeed();
	void FindTarget(struct usercmd_s* usercmd);
	void draw_text();
	void draw_text2();
	void draw_line();
	void save();
	void load();
	void add(int index);
	void clear(int index);
	void clearAll();
	void record();
	int target_point;
	int cur_way_mode;
	bool findFirstPoint;
	bool isRunning;
	bool runContinue;
	bool isRecord;
	bool CalcRecordIndex;
	bool wayLock;
	void Debug();


	bool freeInGroup;
	bool runToPoint;

	bool isRunningToPoint(){
		return runToPoint;
	}

	bool getRunning(){
		return isRunning;
	}

	bool isFreeInGroup(){
		return freeInGroup;
	}
	bool getFree(){

		return (firstGroup==-1&&firstPoint==-1&&!isRunning);
	}

protected:
	float fmove;
	float smove;
	//-----
	char file_name[256];
	char map_name[256];
	void getFileName();
	bool pathFree(float* from,float* to);
	float getDist(float* point);
	void vectorAngles(const float * forward, float * angles);
	void aimAngleCalc(float* point,float* viewangles);
    void aimAt(float* point);
	float getAngle(float* point);
	float MoveAnglesSub(float *point);
	void MoveTo(float *point, float* forwardmove, float* sidemove);
	void StopMove(float* forwardmove, float* sidemove);
	void runDirection(int cur_direction,int cur_group,struct usercmd_s* usercmd);
	void drawline(float *from, float *to,int r,int g,int b,float d,float amplitude);
	void groupCountCalc(void);
	
	void setRunning(bool state){
		isRunning=state;
	}

	int group_count;
	int point_count[9];
	int direction;
	int beforeGroup;
	int firstGroup;
	int firstPoint;
	float firstDist;
	float nextDist;
	int beforeDirection;
public:
	WAYPOINT(){ 
	target_point=-1; 
	group_count=0; 
	point_count[1]=0; 
	point_count[2]=0; 
	point_count[3]=0; 
	point_count[4]=0;
	point_count[5]=0; 
	point_count[6]=0; 
	point_count[7]=0; 
	point_count[8]=0;
	isRunning=false; 
	runContinue=false; 
	direction=-1; 
	beforeGroup=-1; 
	firstGroup=-1; 
	firstPoint=-1; 
	isRecord=false; 
	beforeDirection=-1;
	findFirstPoint=false;
	CalcRecordIndex=false;
	freeInGroup=false;
	runToPoint=false;
	}
};

//**********************************************************************************************************************************

extern WAYPOINT waypoint;

//**********************************************************************************************************************************

#endif

//**********************************************************************************************************************************
