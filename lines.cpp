// вращать вокруг плавно меняющейся оси

#include <ata/win.h>
#include <ata/gl.h>
#include <ata/vec.h>
#include <math.h>
#include <ata/rand.h>

#define N 35
#define NC 20

//???using namespace ata;

struct {
	vec3f l;
	float u,du;
} s[3];

vec3f p[4],q[4];
vec2f r[4][N];
vec3f c[N],cc,dc;
float d=3.5;
mat3f M,mm;
int x,y,X,Y;

void Init() {
	randomize();
	for(int i=0;i<3;++i) {
		s[i].l=vec3f(rnd()-0.5f,rnd()-0.5f,rnd()-0.5f);
		s[i].du=0.07*rnd();
    }
	for(int i=0;i<4;++i)
		p[i]=vec3f(rnd()-0.5f,rnd()-0.5f,rnd()-0.5f);
	c[N-1]=vec3f(rnd(),rnd(),rnd());
	cc=vec3f(rnd(),rnd(),rnd());
	dc=(cc-c[N-1])*(1./NC);
}

void LoadMatrix(const mat34f& m) {
	float a[16];
	a[0] =m.rot.m[0][0];a[1] =m.rot.m[1][0];a[2] =m.rot.m[2][0];a[3] =0;
	a[4] =m.rot.m[0][1];a[5] =m.rot.m[1][1];a[6] =m.rot.m[2][1];a[7] =0;
	a[8] =m.rot.m[0][2];a[9] =m.rot.m[1][2];a[10]=m.rot.m[2][2];a[11]=0;
	a[12]=m.pos.x;		a[13]=m.pos.y;		a[14]=m.pos.z;		a[15]=1;
    glLoadMatrixf(a);
};

void Draw() {
	glViewport(0,0,WinW(),WinH());
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,1,-1,-1,1);
	glMatrixMode(GL_MODELVIEW);
	//???    glLoadIdentity();
    LoadMatrix(mat34f(M,vec3f(0,0,0.2)));
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	for(int i=0;i<3;++i) {
    	glBegin(GL_QUAD_STRIP);
		for(int j=0;j<N;++j) {
			glColor3fv(&c[j].x);
			glVertex3fv(&r[i][j].x);
			glVertex3fv(&r[i+1][j].x);
        }
		glEnd();
    }

	//??? optimize
	mat34f m[4];
	m[0]=m[1]=rot(s[0].l,s[0].u);
	s[0].u+=s[0].du;
	m[2]=mat34f(rot(s[1].l,s[1].u))*mat34f(p[0])*m[1];s[1].u+=s[1].du;
	m[3]=mat34f(rot(s[2].l,s[2].u))*mat34f(p[1])*m[2];s[2].u+=s[2].du;

	for(int j=1;j<N;++j) {
		for(int i=0;i<4;++i)
			r[i][j-1]=r[i][j];
        c[j-1]=c[j];
    }

	c[N-1]+=dc;
	if((c[N-1]-cc).len2()<1e-2) {
		cc=vec3f(rnd(),rnd(),rnd());
		dc=(cc-c[N-1])*(1./NC);
    }

	for(int i=0;i<4;++i)
		r[i][N-1]=m[i]*p[i];
}

void ProcMouse() {
    M=mm.rot(vec3f(y-Y,X-x,0))*M;
    X=x;Y=y;
}

int main(int narg,char* args[]) {
	Window("Lines",0,0);
	Init();
    GetMouse(X,Y);
	while(Run()) {
        int m=GetMouse(x,y);
		if(m&1) Init();
        if(m&4) GetMouse(X,Y);
        //??? ProcMouse();
		if(GetKey(VK_ESCAPE)) break;
		Draw();
		Flip();
		Wait(10);
	}
    return 0;
}

