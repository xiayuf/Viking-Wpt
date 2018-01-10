//==============================================================================
// Console.cpp
//==============================================================================

#include <windows.h>
#include <vector>
#include "console.h"
#include "client.h"
#include "stringfinder.h"
#include "interpreter.h"
#include "color.h"
#include "cvars.h"
#include "drawing.h"
#include "gui2.h"
// global
Console gConsole;

extern bool oglSubtractive;
void DrawBox(int x, int y);
void DrawMove(int x, int y);



//========================================================================================
void Console::draw(int con_x, int con_y, int con_w, int con_h)
{
	if( !active ) return;

	ColorEntry* color;
	oglSubtractive=true;
	gEngfuncs.pfnFillRGBA(con_x,con_y-16,con_w, 16,255,0,0,128);
	gEngfuncs.pfnFillRGBA(con_x,con_y-0,con_w,con_h,0,0,255,128);
	oglSubtractive=false;
//	gui.Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(con_x,con_y-16,con_w,con_h,20,ubr,ubg,ubb,lbr,lbg,lbb,ba,uhr,uhg,uhb,lhr,lhg,lhb,ha,bor,bog,bob,boa,line);
	DrawGlowHudString(con_x + con_w/3, con_y-18,20,20,50,"   - Viking Wpt -"); 

	int x = con_x+3;
	int y = con_y+con_h-14;

	// advance blink:
	if(blinkTimer.expired())
	{
		blink = !blink;
		if(blink) { blinkTimer.countdown(0.2); }
		else      { blinkTimer.countdown(0.2); }
	}
	
	// draw cursor
	if(blink)
	{
		// get len of text to cursor.
		int length, height, ch_length;
		char  save;
		save = cursorpos[0]; cursorpos[0]=0;
		gEngfuncs.pfnDrawConsoleStringLen( editbuf, &length, &height );
		cursorpos[0]=save;

		// get cursor size:
		save = cursorpos[1]; cursorpos[1]=0;
		gEngfuncs.pfnDrawConsoleStringLen( cursorpos, &ch_length, &height );
		cursorpos[1]=save;
		if(!*cursorpos) ch_length=5;

        gEngfuncs.pfnDrawSetTextColor(1,1,1);
		gEngfuncs.pfnDrawConsoleString(x+length,y+1,"|"); // "con_text2"

//        tintArea(x+length,y+2,ch_length,height-5, colorList.get(11)); // "con_text2"
	}

	color = colorList.get(24); // "con_edit"
	tintArea(con_x, con_y+con_h-13,con_w,1,255,255,255,150);
	gEngfuncs.pfnDrawSetTextColor(color->onebased_r,color->onebased_g,color->onebased_b);
	gEngfuncs.pfnDrawConsoleString(x,y,editbuf);

	lines.reset();
	for(;;)
	{
		y-=14;
		if(y<con_y) break;
		
		string& curLine = lines.read(); 
		lines.prev();

		drawConsoleLine(curLine,x,y);
	}
}

//========================================================================================
void Console::echo(const char *fmt, ... )
{
	va_list va_alist;
	char buf[384];

	va_start (va_alist, fmt);
	_vsnprintf (buf, sizeof(buf), fmt, va_alist);
	va_end (va_alist);

	lines.add( buf );
}

//========================================================================================
void Console::say(const char* text, const char* name, int team )
{
	if(team==1) { echo("&r%s :", name); echo(" %s",text); }
	else        { echo("&b%s :", name); echo(" %s",text); }
}


//========================================================================================
void Console::setcolortag(unsigned char ch, int r, int g, int b)
{
	ch -= 'a';
	if(ch>=26) { echo("color tags must be labeled \'a\' to \'z\'"); return; }
	
	colorTags[ch].r = r;
	colorTags[ch].g = g;
	colorTags[ch].b = b;
	colorTags[ch].fill_missing();
}

//========================================================================================
void Console::drawConsoleLine( const string& str, int x, int y )
{
	const char* line = str.c_str();
	char  buf[256];
	char* bufpos;

	for(;;)
	{
		// extract string part
		bufpos=buf;
		for(;;) { *bufpos=*line; if(!*line||*line=='&')break;  ++line; ++bufpos; };
		bufpos[0]=0;bufpos[1]=0;

		// draw
		int length, height;
		gEngfuncs.pfnDrawConsoleStringLen( buf, &length, &height );
		gEngfuncs.pfnDrawSetTextColor(colorTags[curColorTag].onebased_r, 
										 colorTags[curColorTag].onebased_g, 
										 colorTags[curColorTag].onebased_b);
		gEngfuncs.pfnDrawConsoleString(x,y,buf);

		// advance
		x+=length;
		if(*line=='&')
		{
			unsigned char ch = *++line - 'a';
			if(ch<26) curColorTag=ch;
			else         break;
			if(!*++line) break;
		}
		else 
		{
			break;
		}
	}
	curColorTag=0;
}

//========================================================================================
void Console::key(int ch)
{
	char* pos;
	switch(ch)
	{
	case -1: // backspace
		if(cursorpos==editline) return;
		pos = --cursorpos;
		while(pos[0]) { pos[0]=pos[1]; ++pos; }
		return;
		
	case -2: // uparrow
		if(hist_direction!=DIR_BACK) { history.prev(); history.prev(); hist_direction=DIR_BACK;}
		strcpy(editline, history.read().c_str());
		cursorpos = editline + strlen(editline);
		history.prev();
		return;

	case -3: // downarrow
		if(hist_direction==DIR_BACK) { history.next(); history.next(); hist_direction=DIR_FORWARD;}
		strcpy(editline, history.read().c_str());
		cursorpos = editline + strlen(editline);
		history.next();
		return;
	
	case -4: // leftarrow
		if(cursorpos!=editline) --cursorpos;
		return;

	case -5: // leftarrow
		if(cursorpos!=(editline+strlen(editline))) ++cursorpos;
		return;

	case '\n':
			if( !strcmp(editline,"===") )
			{
				if(mode==MODE_EXECUTE) { mode=MODE_CHAT;    echo("&b*** &aCONSOLE: &wCHAT MODE &b***"); }
				else                   { mode=MODE_EXECUTE; echo("&b*** &aCONSOLE: &wEXEC MODE &b***"); }
			}
			else if(mode==MODE_EXECUTE)
			{
				echo    ( "&x%s",editbuf );
				if(editline[0])
				{
					cmd.exec( editline );
					history.add(editline);
					history.reset();
				}
			} 
			else if(mode==MODE_CHAT)
			{
				char* text = editline; while(*text==' ')++text;
				char buf[256];sprintf(buf,"say \"%s\"",text);
				gEngfuncs.pfnClientCmd(buf);
			}	
			editline[0]=0;
			cursorpos = editline;
			return;
	default:
		// insert character
		if(strlen(editbuf)>(EDIT_MAX-4)) return;
		if(!cursorpos[0]) { cursorpos[0]=ch; ++cursorpos; cursorpos[0]=0; return; }

		pos = editbuf+strlen(editbuf)+1; 
		while(pos>cursorpos) { pos[0]=pos[-1]; --pos; }

		*cursorpos = ch;
		++cursorpos;
		return;
	}
}