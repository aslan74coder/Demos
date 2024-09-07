#include <ata/win.h>
#include <math.h>

#define N 10000
#define Z 20
#define D 0
#define SX 600
#define SY 400

using namespace win;

int* pix;

struct Star {
	int x,y,z,c;
} stars[N];

void InitStar(Star* star) {
	star->x=(rand()%(SX-D*2)-SX/2+D)*Z<<16;
	star->y=(rand()%(SY-D*2)-SY/2+D)*Z<<16;
	star->z=Z;
	star->c=RGB(rand(),rand(),rand());
}

void Init() {
	randomize();
	Star* star=stars;
	for(int i=N;--i>=0;++star)
		InitStar(star);
}

void Run() {
	// clear screen
	int* p=(int*)pix;
	for(int i=SX*SY;--i>=0;++p)
		*p=0;
	// draw stars
	p=(int*)pix;
	Star* star=stars;
	for(int i=N;--i>=0;++star) {
		int x=star->x,y=star->y,z=star->z;
		p[SX*(y/z>>16)+(x/z>>16)+(SX*(SY/2)+SX/2)]=0;
		if(--z<=0)
			InitStar(star);
		else {
			x=x/z>>16;y=y/z>>16;
			if(abs(x)>SX/2-D||abs(y)>SY/2-D)
				InitStar(star);
			else {
				p[SX*y+x+(SX*(SY/2)+SX/2)]=star->c;
				star->z=z;
			}
		}
	}
}

int main(int narg,char* args) {
	pix=(int*)Window("Starfly",SX,SY,32);
	Init();
    while(!Terminated()) {
        Update();
    	if(GetKey(VK_ESCAPE)) break;
    	Run();
        Flip();
    	Wait(10);
	}
	return 0;
}

