#include <windows.h>
#include <gl/gl.h>

//extern SCREENINFO screeninfo;

class mousept
{
public:
	float x;
	float y;
	void init(void);
	void leftclick(void);
	void rightclick(void);
	void mouseover(void);
	void mousemove(float xm, float ym);
	void draw(void);

private:
	bool bound(void);
};
extern mousept mouse;

class GUI
{
public:
	void Init();

	void MenuDraw();
	int  MenuKey(int keynum);
	int  MenuIndex;

	void WinampDraw();
	int  WinampKey(int keynum);
	int  WinampIndex;
	bool WinampPlaylist;
	
	void DrawFlatBox(int x, int y, int width, int height, float br, float bg, float bb, float ba);
	void DrawSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba);
	void Draw3DSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba);
	void DrawFlatBorder(int x, int y, int width, int height, float bor, float bog, float bob, float boa, int line);
	void Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line);
	void Newhud(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line);
private:
};
extern GUI gui;

	