#include <ata/win.h>
#include <math.h>

#define N 128
#define CW 0.3
#define CW2 -0.3
#define R2 50.
#define VR 3.e1
#define VG 5.e1
#define VB 7.e1

using namespace ata;
using namespace win;

unsigned char sina[N+1];

void Init() {
    for(int i=0;i<=N;++i)
        sina[i]=128+sin(2.*M_PI*i/(N+1))*127;
}

inline unsigned char Sin(float x) { return sina[(int)(frac(x)*N)]; }

double Z(int x,int y,float c,float s) { return (1./(2.*M_PI))*0.00002*(c*x-s*y)*(s*x+c*y); }

void Draw(int w,int h,int *p) {
    float t=0.001*Time(),ca=cos(CW*t),sa=sin(CW*t);
    int cx=w/2+R2*cos(CW2*t),cy=h/2+R2*sin(CW2*t);t*=1.5;
    for(int y=0;y<h;++y) {
        // считаем квадр.форму, достаточно dz и ddz
        double z=Z(-cx,y-cy,ca,sa),z1=Z(1-cx,y-cy,ca,sa),z2=Z(2-cx,y-cy,ca,sa),dz=z1-z,ddz=z2-z1-dz;
        for(int j=w;--j>=0;z+=dz,dz+=ddz)
            *(p++)=RGB(Sin(VR*z-t),Sin(VG*z-t),Sin(VB*z-t));
    }
}

int main(int narg,char* args[]) {
    Init();
    if(Window("Bola",600,400,32))
    while(!Closed()) {
    	Draw(W(),H(),(int*)Data());
        Update();
		Flip();
        Wait(10);
	}
    return 0;
}

