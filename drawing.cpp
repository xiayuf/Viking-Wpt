//==============================================================================
// drawing.cpp
//==============================================================================

//========================================================================================
#include <windows.h>
#include <vector>
#include "client.h"
#include "calcscreen.h"
#include "color.h"
#include "cvars.h"

extern SCREENINFO screeninfo;
extern bool oglSubtractive;

//========================================================================================
int DrawLen(char *fmt)
{
	int len=0;
	for ( char * p = fmt; *p; p++ ) len+=screeninfo.charWidths[*p]; 
	return len;
}
//==============================================================================
void DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	// y-check
	int borderY = displayCenterY*2-18;
	if( y<0 || y>borderY ) { return; }

	int drawLen = DrawLen(buf);
	x = x-drawLen/2;

	int borderX = displayCenterX*2-11;
	gEngfuncs.pfnDrawSetTextColor(/*(float)*/r/255.0,/*(float)*/g/255.0,/*(float)*/b/255.0);
	gEngfuncs.pfnDrawConsoleString(x,y,buf);
}

//========================================================================================
void DrawHudString1 (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	gEngfuncs.pfnDrawSetTextColor(/*(float)*/r/255.0,/*(float)*/g/255.0,/*(float)*/b/255.0);
	gEngfuncs.pfnDrawConsoleString(x,y,buf);
}
void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);
	
	// y-check
	int borderY = displayCenterY*2-18;
	if( y<0 || y>borderY ) { return; }
	
	bool needSingleCheck=false;
	int borderX = displayCenterX*2-11;
		
	int drawLen = DrawLen(buf);
	if( x<1 ){ needSingleCheck=true; }
	else
	{
		int maxX = x+drawLen;
		needSingleCheck = (maxX>borderX);
	}

	if( needSingleCheck )
	{
		for ( char * p = buf; *p; p++ )
		{
			int next = x + screeninfo.charWidths[*p];
			// IMPORTANT NOTE: when drawing admin-mod style charactters
			//    you MAY NOT provide x/y coordinates that cause drawing
			//    off screen. This causes HL to crash or just quit
			if( x>0 && x<borderX )
				gEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
			x = next;
		}
	}else {
		for ( char * p = buf; *p; p++ )
		{
			int next = x + screeninfo.charWidths[*p];
			// IMPORTANT NOTE: when drawing admin-mod style charactters
			//    you MAY NOT provide x/y coordinates that cause drawing
			//    off screen. This causes HL to crash or just quit
			gEngfuncs.pfnDrawCharacter (x, y, *p, r, g, b);
			x = next;
		}
	}	
}
//========================================================================================

void DrawConString (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	int length, height;

	y += 4;
	gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
	gEngfuncs.pfnDrawSetTextColor(r/255.0f, g/255.0f, b/255.0f);
	gEngfuncs.pfnDrawConsoleString(x,y,buf);
}

//========================================================================================

void DrawConStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... )
{
	va_list va_alist;
	char buf[256];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	int length, height;

	gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
	x -= length/2;
	y += 4;
	gEngfuncs.pfnDrawSetTextColor(r/255.0f, g/255.0f, b/255.0f);
	gEngfuncs.pfnDrawConsoleString(x,y,buf);
}
void gDrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius=1)
{
	int radius2 = radius<<1;
	gEngfuncs.pfnFillRGBA(x-radius+2, y-radius, radius2-2,2,r,g,b,alpha);
	gEngfuncs.pfnFillRGBA(x-radius, y-radius, 2,radius2,r,g,b,alpha);
	gEngfuncs.pfnFillRGBA(x-radius, y+radius, radius2,2,r,g,b,alpha);
	gEngfuncs.pfnFillRGBA(x+radius, y-radius, 2,radius2+2,r,g,b,alpha);
}

//========================================================================================

void tintArea(int x,int y,int w,int h, ColorEntry* clr)
{
	oglSubtractive = true;
	gEngfuncs.pfnFillRGBA(x,y,w,h,clr->r,clr->g,clr->b,clr->a);
	oglSubtractive = false;
}

//========================================================================================

void tintArea(int x,int y,int w,int h,int r,int g,int b,int a)
{
	oglSubtractive = true;
	gEngfuncs.pfnFillRGBA(x,y,w,h,r,g,b,a);
	oglSubtractive = false;
}

//========================================================================================

void colorBorder(int x,int y,int w, int h,int r,int g,int b,int a)
{
	oglSubtractive = true;
	gEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,r,g,b,a ); //top
	gEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,r,g,b,a );	//left
	gEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,r,g,b,a );	//right
	gEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,r,g,b,a ); //bottom
	oglSubtractive = false;
}

//========================================================================================

void dddBorder(int x,int y,int w, int h, int a)
{
	oglSubtractive = true;
	gEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,255,255,255,a ); //top
	gEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,255,255,255,a );	//left
	gEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,0,0,0,a );	//right
	gEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,0,0,0,a ); //bottom
	oglSubtractive = false;
}
void gDrawFilledBoxAtLocation( float* origin, DWORD color, int radius)
{
	float vecScreen[2]; 
	if( !CalcScreen(origin, vecScreen) ) { return; }

	int red   = (color>>24);
	int green = (color>>16)&0xFF;
	int blue  = (color>>8)&0xFF;
	int alpha = (color)&0xFF;
	int radius2 = radius<<1;

	gEngfuncs.pfnFillRGBA(vecScreen[0]-radius,vecScreen[1]-radius,radius2,radius2,red,green,blue,alpha);
}
//========================================================================================

void dddBorder2(int x,int y,int w, int h, int a)
{
	oglSubtractive = true;
	gEngfuncs.pfnFillRGBA( x-1, y-1, w+2, 1 ,0,0,0,a ); //top
	gEngfuncs.pfnFillRGBA( x-1, y, 1, h-1 ,0,0,0,a );	//left
	gEngfuncs.pfnFillRGBA( x+w, y, 1, h-1 ,255,255,255,a );	//right
	gEngfuncs.pfnFillRGBA( x-1, y+h-1, w+2, 1 ,255,255,255,a ); //bottom
	oglSubtractive = false;
}

//========================================================================================
