#ifndef AIMBOT_H
#define AIMBOT_H

#include <windows.h>
#include <vector>
#include "cvars.h"

using namespace std;

struct aimvec
{
	float h;
	float f;
	float r;
};

class CAimbot
{
protected:
	void init() 
	{ 
		iTarget = -1;
		active = false;
	}

public:
	int iTarget;
	bool active;
	vec3_t aim_viewangles;	
	vector<aimvec> AimVecsS;
	vector<aimvec> AimVecsJ;
	vector<aimvec> AimVecsD;
	vector<aimvec> AimVecsR;
	int CanPenetrate( float *start, float *end, int power );
	int CorrectGun();
	void Draw();
	void Draw_Player_Vecs(int ax);
	void CalculateHitbox( cl_entity_s *pEnt );
	void CalculateAimingView( void );
	inline void SetTarget( int iIndex ){ iTarget = iIndex; }
	bool HasTarget( void ){ return iTarget != -1; }
	void FindTarget( void );
	bool CheckTeam(int ax);
	bool IsShielded(int ax);
	float calcFovAngle(const float* origin_viewer, const float* angle_viewer, const float* origin_target);
	void calcFovangleAndVisibility(int ax);
	bool pathFree(float* xfrom,float* xto);
	aimvec* TargetRegion(int ax);

public:
	CAimbot() { init(); }
};

extern CAimbot Aimbot;

#endif