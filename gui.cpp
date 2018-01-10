#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")
#pragma comment(lib,"GLaux.lib")
#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "gui.h"
GUI cGui;
void GUI::DrawFlatBox(int x, int y, int width, int height, float br, float bg, float bb, float ba)
{
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_FLAT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(br,bg,bb,ba);
	glBegin(GL_QUADS);
	glVertex2i(x,y);
	glVertex2i(x+width,y);
	glVertex2i(x+width,y+height);
	glVertex2i(x,y+height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GUI::DrawSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba)
{
	glDisable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUADS);
	glColor4f(ubr,ubg,ubb,ba);
	glVertex2i(x,y);
	glColor4f(ubr,ubg,ubb,ba);
	glVertex2i(x+width,y);
	glColor4f(lbr,lbg,lbb,ba);
	glVertex2i(x+width,y+height);
	glColor4f(lbr,lbg,lbb,ba);
	glVertex2i(x,y+height);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void GUI::Draw3DSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba)
{
	GUI::DrawSmoothBox(x,y,width,height,ubr,ubg,ubb,lbr,lbg,lbb,ba);
	GUI::DrawFlatBox(x,y,width,1,0.78f,0.78f,0.78f,0.78f);
	GUI::DrawFlatBox(x+width,y,-1,height,0.22f,0.22f,0.22f,0.78f);
	GUI::DrawFlatBox(x+width,y+height,-width,-1,0.22f,0.22f,0.22f,0.78f);
	GUI::DrawFlatBox(x,y+height,1,-height,0.78f,0.78f,0.78f,0.78f);
}
void GUI::DrawFlatBorder(int x, int y, int width, int height, float bor, float bog, float bob, float boa, int line)
{
	//I know i can do this with GL_LINES and GL_LINE_LOOP, but y should i xD
	GUI::DrawFlatBox(x,y,width,line,bor,bog,bob,boa);
	GUI::DrawFlatBox(x+width,y,-line,height,bor,bog,bob,boa);
	GUI::DrawFlatBox(x+width,y+height,-width,-line,bor,bog,bob,boa);
	GUI::DrawFlatBox(x,y+height,line,-height,bor,bog,bob,boa);
}
void GUI::Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line)
{
	GUI::Draw3DSmoothBox(x+line,y+line,bw-(line*2),hh-(line*2),uhr,uhg,uhb,lhr,lhg,lhb,ha);
	GUI::DrawFlatBorder(x,y,bw,hh,bor,bog,bob,boa,line);
	GUI::Draw3DSmoothBox(x+line,y+hh+line,bw-(line*2),bh-(line*2),ubr,ubg,ubb,lbr,lbg,lbb,ba);
	GUI::DrawFlatBorder(x,y+hh,bw,bh,bor,bog,bob,boa,line);
}
void GUI::Newhud(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line)
{
//	GUI::Draw3DSmoothBox(x+line,y+line,bw-(line*2),hh-(line*2),uhr,uhg,uhb,lhr,lhg,lhb,ha);
//	GUI::DrawFlatBorder(x,y,bw,hh,bor,bog,bob,boa,line);
	GUI::Draw3DSmoothBox(x+line,y+hh+line,bw-(line*2),bh-(line*2),ubr,ubg,ubb,lbr,lbg,lbb,ba);
	GUI::DrawFlatBorder(x,y+hh,bw,bh,bor,bog,bob,boa,line);
}