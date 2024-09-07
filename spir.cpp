#include <ata/win.h>
#include <vector>
#include <math.h>

#define N 20

using namespace ata;
using namespace win;

std::vector<int> a;

void Init(int w,int h) {
    static int W=0,H=0;
    if(w!=W||h!=H) {
    	a.resize(2*(W=w)*(H=h));
        int cx=w>>1,cy=h>>1,x,y,*q=&a[0];
    	for(y=-cy;y<cy;++y)
        	for(x=-cx;x<cx;++x,q+=2) {
				if(!x&&!y) continue;
          		float a=0.5/M_PI*N*atan2(y,x),r=0.005/M_PI*N*sqrt(x*x+y*y);
                q[0]=(a+r)*65536;q[1]=(a-r)*65536;
            }
	}
}

void Draw(int w,int h,int *p) {
	Init(w,h);
    int t=Time(),t1=-0.0001/M_PI*32768*N*t,t2=0.0005/M_PI*32768*N*t;
	int cx=w>>1,cy=h>>1,c[]={RGB(0,0,255),RGB(0,255,0)};
    int *q=&a[0];
    for(int i=4*cx*cy;--i>=0;++p,q+=2)
        *p=c[(((q[0]+t1)^(q[1]+t2))>>16)&1];
}

int main(int narg,char** args) {
    Window("Spiral",600,400,32);
    while(!Closed()) {
    	Draw(W(),H(),(int*)Data());
        Update();
		Flip();
		Wait(20);
	}
	return 0;
}

