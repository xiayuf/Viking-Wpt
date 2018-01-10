//==============================================================================
// drawing.h
//==============================================================================

int DrawLen(char *fmt);
void DrawHudStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... );
void DrawHudString (int x, int y, int r, int g, int b, const char *fmt, ... );
void DrawHudString1 (int x, int y, int r, int g, int b, const char *fmt, ... );
void DrawConStringCenter (int x, int y, int r, int g, int b, const char *fmt, ... );
void DrawConString (int x, int y, int r, int g, int b, const char *fmt, ... );
void tintArea(int x,int y,int w,int h, struct ColorEntry* clr);
void tintArea(int x,int y,int w,int h,int r,int g,int b,int a);
void gDrawBoxAtScreenXY( int x, int y, int r, int g, int b, int alpha, int radius=1);
void colorBorder(int x,int y,int w, int h,int r,int g,int b,int a);
void dddBorder(int x,int y,int w, int h, int a);
void dddBorder2(int x,int y,int w, int h, int a);
void gDrawFilledBoxAtLocation( float* origin, DWORD color, int radius);
