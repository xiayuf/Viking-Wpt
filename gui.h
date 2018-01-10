class GUI
{
private:
public:
	void DrawFlatBox(int x, int y, int width, int height, float br, float bg, float bb, float ba);
	void DrawSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba);
	void Draw3DSmoothBox(int x, int y, int width, int height, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba);
	void DrawFlatBorder(int x, int y, int width, int height, float bor, float bog, float bob, float boa, int line);
	void Draw3DSmoothBoxAndSmoothHeaderWithFlatBorder(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line);
	void Newhud(int x, int y, int bw, int bh, int hh, float ubr, float ubg, float ubb, float lbr, float lbg, float lbb, float ba, float uhr, float uhg, float uhb, float lhr, float lhg, float lhb, float ha, float bor, float bog, float bob, float boa, int line);
};
extern GUI cGui;