#include <ata/win.h>
#include <vector>
#include <math.h>

#define D 32

using namespace win;

void Draw(int *p,int w,int h) {
    int cx=w>>1,cy=h>>1;
    int cc[2]={RGB(0,0,255),RGB(0,255,0)};
	for(int y=-cy;y<cy;++y)
		for(int x=-cx;x<cx;++x) {
			float r=std::max(sqrt(sqrt(x*x+y*y)),1.);
			float tt=-0.5*M_PI/2/w*r + 0.0002*t;
			float c=cos(tt),s=sin(tt);
			r=30./r;
			int x1=cx+(c*x-s*y)*r+0.07*t,y1=cy+(s*x+c*y)*r+0.07*t;
            int ii=(((x1&D)>D/2)==((y1&D)>D/2));
			*(p++)=cc[ii];
		}
}

int main(int narg,char* args) {
	int w=600,h=400;
    int* p=(int*)Window("Warp",w,h,32);
    while(!Terminated()) {
    	ProcessMessages();
        p=(int*)Resize(w=W(),h=H());
    	Draw(p,w,h);
		Flip();
	}
	return 0;
}

