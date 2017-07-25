#ifndef __COLOURSET
#define __COLOURSET

struct CColourSet
{
	float ambr;
	float ambg;
	float ambb;
	float ambobjr;
	float ambobjg;
	float ambobjb;
	float ambBeforeBrightnessr;
	float ambBeforeBrightnessg;
	float ambBeforeBrightnessb;
	short skytopr;
	short skytopg;
	short skytopb;
	short skybotr;
	short skybotg;
	short skybotb;
	short suncorer;
	short suncoreg;
	short suncoreb;
	short suncoronar;
	short suncoronag;
	short suncoronab;
	float sunsz;
	float sprsz;
	float sprbght;
	short shd;
	short lightshd;
	short poleshd;
	float farclp;
	float fogst;
	float lightonground;
	short lowcloudr;
	short lowcloudg;
	short lowcloudb;
	short fluffycloudr;
	short fluffycloudg;
	short fluffycloudb;
	float waterr;
	float waterg;
	float waterb;
	float watera;
	float postfx1r;
	float postfx1g;
	float postfx1b;
	float postfx1a;
	float postfx2r;
	float postfx2g;
	float postfx2b;
	float postfx2a;
	float cloudalpha;
	int intensityLimit;
	short waterfogalpha;
	float directionalmult;
	float lodDistMult;

	// VCS
/*
	float ambr_bl;
	float ambg_bl;
	float ambb_bl;
	float ambobjr_bl;
	float ambobjg_bl;
	float ambobjb_bl;
*/
	float dirr;
	float dirg;
	float dirb;
	short radiosityIntensity;
	short radiosityLimit;
	short fluffycloudtopr;
	short fluffycloudtopg;
	short fluffycloudtopb;
	short blurr;
	short blurg;
	short blurb;
	float blura;
	float bluroffset;

	void ctor(int h, int w);
	CColourSet(void) {}
	CColourSet(int h, int w) { ctor(h, w); }
	void Interpolate(CColourSet *a, CColourSet *b, float fa, float fb, bool ignoreSky);
	void convertToSA(void);
};

#endif COLOURSET