//==============================================================================
// color.cpp
//==============================================================================

#include <vector>
#include "stringfinder.h"
#include "color.h"

#define BOUND_INTEGER(var,min,max) if((var)>(max)){(var)=(max);};if((var)<(min)){(var)=(min);}

ColorManager colorList;

//====================================================================
void ColorManager::init()
{
	indexes.clear();
	list.clear();
	insertpos = 0;

	add("cross"      ,255,0,0,200 ); // index: 0
	add("cross2"     ,255,255,255,200  ); // index: 1
	add("terror"     ,255, 40, 40      ); // index: 2
	add("counter"    , 60,100,255      ); // index: 3
	add("esp"        ,0  ,255,  0      ); // index: 4
	add("special"    ,155,155,155      ); // index: 5
	add("target"     ,0,255,0,50       ); // index: 6
	add("radar"      ,80,150, 30, 110  ); // index: 7
	add("menu1"      , 51,51,204,130   ); // index: 8
	add("menu2"      , 51,51,204,123   ); // index: 9
	add("con_text"   ,255,255, 30,255  ); // index: 10
	add("con_text2"  ,160,160,222,128  ); // index: 11
	add("con_edit"   ,255,255,255,128  ); // index: 12
	add("tsound"     ,255,144,144      ); // index: 13
	add("ctsound"    ,144,144,255      ); // index: 14
	add("snd_step"   ,255,255,255, 255 ); // index: 15
	add("snd_weapon" ,144,155,255, 255 ); // index: 16
	add("snd_hit"    ,222,100, 61, 254 ); // index: 17
	add("snd_special",222,111,111, 254 ); // index: 18
	add("snd_default",222,255,222, 255 ); // index: 19
	add("sound"      , 68, 68,255      ); // index: 20
	add("wpt"        ,000,000,255, 60  ); // index: 21
	add("yellow"     ,255,255,0, 255   ); // index: 22
	add("pink"       ,100,8,58         ); // index: 23
	add("green"      ,0,255,0          ); // index: 24
	add("norefresh"      ,0,0,0        ); // index: 25
	add("red"      ,255,0,0        ); // index: 26
	add("hud"        ,0,255,0      ); // index: 27
} 

//==================================================================== 
static void FillColorEntry(ColorEntry& tmp)
{
	tmp.compiled = (tmp.r<<24) + (tmp.g<<16) + (tmp.b<<8) + tmp.a;
	tmp.fr= float(tmp.r)/255.0/30.0;
	tmp.fg= float(tmp.g)/255.0/30.0;
	tmp.fb= float(tmp.b)/255.0/30.0;

	tmp.onebased_r=float(tmp.r)/255.0;
	tmp.onebased_g=float(tmp.g)/255.0;
	tmp.onebased_b=float(tmp.b)/255.0;
	tmp.onebased_a=float(tmp.a)/255.0;
}

//====================================================================
void ColorManager::add(const char* name,int r, int g, int b, int a)
{
	indexes.add(name,insertpos);

	ColorEntry tmp;
	tmp.r=r;
	tmp.g=g;
	tmp.b=b;
	tmp.a=a;
	FillColorEntry(tmp);

	list.push_back(tmp);

	insertpos++;
}

//====================================================================
void ColorManager::set(char* name, char* red, char* green, char* blue, char* alpha)
{
	if( !indexes.find(name) ){ return; }

	int index = indexes.num;
	char format[]="%d";

	ColorEntry tmp;
	sscanf(red   ,format,&tmp.r);
	sscanf(green ,format,&tmp.g);
	sscanf(blue  ,format,&tmp.b);
	sscanf(alpha ,format,&tmp.a);
	
	FillColorEntry(tmp);

	BOUND_INTEGER(index,0,(list.size()-1));
	list[index] = tmp;
}


