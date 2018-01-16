#include "stdio.h"
#include <iostream>
#include <GL/freeglut.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include "math.h"
#define _USE_MATH_DEFINES
#define worldx 300.0
#define worldy 300.0
using namespace std;
void *font=GLUT_BITMAP_TIMES_ROMAN_24;
void *font_10=GLUT_BITMAP_TIMES_ROMAN_10;
void *font_12=GLUT_BITMAP_HELVETICA_12;
void *font_18=GLUT_BITMAP_HELVETICA_18;
void plotString(float a,float b,float sy1);
void plotCircle(float a, float b);
void plot_Half_Circle(float a,float b,float w);
void plotArrow(float a,float b);
void plotrArrow(float a,float b,int stop);
void plottarget(double x,double y);
void Ground();
void ScoreBar(int zs,int ls);
void zshot(double t,double t_start, double t_end,double zs, double ls);

void tctr(double *t_start, double *t_end, double t_add){
	*t_start=*t_end;
	*t_end+=t_add;
}
void output(double x, double y,char *string,void *f)
{
	int len,i;
	glRasterPos2f(x,y);
	len=strlen(string);
	for(i=0;i<len;i++)
	{
		glutBitmapCharacter(f,string[i]);
	}
}

class Component{
public:
	double parent_x, parent_y;
	double r;
	double shift;
	double width;
	double height;
	double sy1;
	Component* component=NULL;
	Component(){;}
	Component(double r){this->r=r;}
	virtual void plot()=0;
	virtual void reset(float r)=0;
};

class Limbs:public Component{
public:
	Limbs(double parent_x, double parent_y, double r){
		this->parent_x=parent_x;
		this->parent_y=parent_y;
		this->r=r;
	}
	virtual void reset(float r)=0;
	Limbs(){;}
	Limbs(double r){this->r=r;}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(139.0/255.0, 69.0/255.0, 19.0/255.0);
			glRectf(-width/2-0.8, shift+0.8, width/2+0.8, shift-height-0.8);
			glColor3f(1.0, 215.0/255.0, 200.0/255.0);
			glRectf(-width/2, shift, width/2, shift-height);
			if(component!=NULL){
				component->plot();
			}
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
};

class Arm:public Limbs{
public:
	Arm(){;}
	Arm(double parent_x, double parent_y, double r): Limbs(parent_x, parent_y, r){
		shift=1.0;
		width=5.0;
		height=13.0;
	}	
	void reset(float r){}
	void reset(double parent_x, double parent_y, double r){
		this->parent_x=parent_x;
		this->parent_y=parent_y;
		this->r=r;
		shift=1.0;
		width=5.0;
		height=13.0;
	}
};

class ForeArm:public Limbs{
public:
	ForeArm(){;}
	ForeArm(double r): Limbs(r){
		parent_x=0.0;
		parent_y=-12.0;
		shift=1.0;
		width=5.0;
		height=16.0;
	}
	void reset(float r){
		this->r=r;
		parent_x=0.0;
		parent_y=-12.0;
		shift=1.0;
		width=5.0;
		height=13.0;
		if(component!=NULL)
				component->reset(r);
	}
};

class Thigh:public Limbs{
public:
	Thigh();
	Thigh(double parent_x, double parent_y, double r): Limbs(parent_x, parent_y, r){
		shift=3.0;
		width=8.0;
		height=20.0;
	}
	void reset(float r){}	
	void reset(double parent_x, double parent_y, double r){
		this->parent_x=parent_x;
		this->parent_y=parent_y;
		//this->parent_x=
		this->r=r;
		shift=3.0;
		width=8.0;
		height=20.0;
	}
};

class Calf:public Limbs{
public:
	Calf(){;}
	Calf(double r): Limbs(r){
		parent_x=0.0;
		parent_y=-17.0;
		shift=3.0;
		width=5.0;
		height=20.0;
	}
	void reset(float r){
		this->r=r;
		parent_x=0.0;
		parent_y=-17.0;
		shift=3.0;
		width=5.0;
		height=18.0;
	}
};

class Head:public Component{
public:
	Head(){;}
	Head(double r): Component(r){
		parent_x=0.0;
		parent_y=45.0;
		shift=0.0;
		width=15.0;
		height=18.0;
	}	
	void reset(float r){
		this->r=r;
		parent_x=0.0;
		parent_y=45.0;
		shift=0.0;
		width=15.0;
		height=18.0;
	}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(0.0, 0.0, 0.0);
		//	plotCircle(width+0.8, height+0.8);
			glColor3f(1.0, 215.0/255.0, 200.0/255.0);
			plotCircle(width, height);
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
};

class Body{
public:
	double width;
	
	double R, G, B;
	Body(){
		width=15.0;
		R=1.0;
		G=1.0;
		B=0.0;
	}
	void plot(){
		glColor3f(0, 0, 0);
		//glRectf(-width/2-0.8, 30.0+0.8, width/2+0.8, 0.0-0.8);
		glColor3f(R,G,B);
		glRectf(-width/2, 30.0, width/2, 0.0);
	}
	void reset(){
		width=15.0;
	}
};
class Person{
public:
	bool canreset;
	double x,y;
	double r;
	bool front;
	Head* head;
	Body* body;
	Arm* right_arm;
	Arm* left_arm;
	ForeArm* right_forearm;
	ForeArm* left_forearm;
	Thigh* right_thigh;
	Thigh* left_thigh;
	Calf* right_calf;
	Calf* left_calf;

	Person(double x, double y){
		this->x=x;
		this->y=y;
		r=0.0;
		front=true;
		head=new Head(0.0);
		body=new Body();
		right_arm=new Arm(-10,25.0,0.0);
		left_arm=new Arm(10,25.0,0.0);
		right_forearm=new ForeArm(0.0);
		left_forearm=new ForeArm(0.0);
		right_thigh=new Thigh(-4.0, 0.0, 0.0);
		left_thigh=new Thigh(4.0, 0.0, 0.0);
		right_calf=new Calf(0.0);
		left_calf=new Calf(0.0);
		right_arm->component=right_forearm;
		left_arm->component=left_forearm;
		right_thigh->component=right_calf;
		left_thigh->component=left_calf;
		canreset=false;
	}

	void setfront(double x, double y){
		this->x=x;
		this->y=y;
		r=0.0;
		front=true;
		head->reset(0.0);
		body->reset();
		right_arm->reset(-10,20.0,0.0);
		left_arm->reset(10,20.0,0.0);
		right_forearm->reset(0.0);
		left_forearm->reset(0.0);
		right_thigh->reset(-4.0, 0.0, 0.0);
		left_thigh->reset(4.0, 0.0, 0.0);
		right_calf->reset(0.0);
		left_calf->reset(0.0);
		canreset=false;
	}

	void setside(double x, double y){
		setfront(x, y);
		bodyToside(1.0, 1.0);
		front=false;
	}

	void take(ForeArm* forearm, Component* comp){
		forearm->component=comp;
	}

	void bend(double dr){
		r+=dr;
		oLimbsrotate(left_thigh, -dr);
		oLimbsrotate(right_thigh, -dr);
	}
	
	void oLimbsrotate(Component* limbs, double dr){
		limbs->r+=dr;
	}

	void Limbsrotate(Component* limbs, double dr){
		limbs->r+=dr;
		if(limbs->component!=NULL) Limbsrotate(limbs->component, -dr);
	}

	void Limbshift(Component* limbs, double dy){
		limbs->parent_y+=dy;
		if(limbs->component!=NULL) Limbshift(limbs->component, dy);
	}
	void bodyMove(double dx, double dy, double dr){
		x+=dx;
		y+=dy;
		r+=dr;
	}

	void bodyTofront(double t_r, double t, double dt){
		body->width+=6.0*dt/t;
		head->r+=20.0*dt/t;
		head->width+=5.0*dt/t;
		right_arm->parent_x+=-10.0*dt/t;
		left_arm->parent_x+=10.0*dt/t;
		right_thigh->parent_x+=-4.0*dt/t;
		left_thigh->parent_x+=4.0*dt/t;
		if(fabs(t_r-t)<0.01) front=true;
	}
	void bodyToside(double t, double dt){
		front=false;
		body->width+=-6.0*dt/t;
		head->r+=-20.0*dt/t;
		head->width+=-5.0*dt/t;
		right_arm->parent_x+=10.0*dt/t;
		left_arm->parent_x+=-10.0*dt/t;
		right_thigh->parent_x+=4.0*dt/t;
		left_thigh->parent_x+=-4.0*dt/t;
		if(left_forearm->component!=NULL){
			left_forearm->component->r+=-10*dt/t;
		}
	}
	void headToside(double t,double dt)
	{
		//front=false;
		//body->width+=-6.0*dt/t;
		head->r+=20.0*dt/t;
		head->width+=-3.0*dt/t;
	}
	void istep(double t, double t_start, double dt, double scale){
		if(t>=t_start&&t<t_start+1/scale){	
			t-=t_start;
			dt=dt*scale;
			if(t<0.5/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(right_thigh, -30*2*dt);
				Limbsrotate(right_calf, 20*2*dt);
				oLimbsrotate(left_thigh, 10*2*dt);
				oLimbsrotate(right_arm, 10*2*dt);
			}
			else if(t<0.75/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(right_thigh, 10*4*dt);
				Limbsrotate(right_calf, -20*4*dt);
				oLimbsrotate(left_thigh, 5*4*dt);
				oLimbsrotate(right_arm, 10*2*dt);
			}
			else if(t<1.0/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(right_thigh, 0*4*dt);
				Limbsrotate(right_calf, -20*4*dt);
				oLimbsrotate(left_thigh, 5*4*dt);
				oLimbsrotate(right_arm, 10*2*dt);
			}
		} 
	}
	void rstep(double t, double t_start, double dt, double scale){
		if(t>=t_start&&t<t_start+1.5/scale){	
			t-=t_start;
			dt*=scale;
			if(t<1.0/scale){
				bodyMove(-10.0*dt, 0.0, 0.0);
				Limbsrotate(right_thigh, -50*dt);
				Limbsrotate(right_calf, 0*dt);
				oLimbsrotate(left_thigh, 30*dt);
				oLimbsrotate(right_arm, 10*3*dt);
			}
			else if(t<1.25/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(right_thigh, 10*4*dt);
				Limbsrotate(right_calf, -20*4*dt);
				oLimbsrotate(left_thigh, 5*4*dt);
				oLimbsrotate(right_arm, 10*2*dt);
			}
			else if(t<1.5/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(right_thigh, 0*4*dt);
				Limbsrotate(right_calf, -20*4*dt);
				oLimbsrotate(left_thigh, 5*4*dt);
				oLimbsrotate(right_arm, 10*2*dt);
			}
		} 
	}


	void lstep(double t, double t_start, double dt, double scale){
		if(t>=t_start&&t<t_start+1.5/scale){	
			t-=t_start;
			dt*=scale;
			if(t<1.0/scale){
				bodyMove(-10.0*dt, 0.0, 0.0);
				Limbsrotate(left_thigh, -50*dt);
				Limbsrotate(left_calf, 0*dt);
				oLimbsrotate(right_thigh, 30*dt);
				oLimbsrotate(right_arm, -10*3*dt);
			}
			else if(t<1.25/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(left_thigh, 10*4*dt);
				Limbsrotate(left_calf, -20*4*dt);
				oLimbsrotate(right_thigh, 5*4*dt);
				oLimbsrotate(right_arm, -10*2*dt);
			}
			else if(t<1.5/scale){
				bodyMove(-5.0*2*dt, 0.0, 0.0);
				Limbsrotate(left_thigh, 0*4*dt);
				Limbsrotate(left_calf, -20*4*dt);
				oLimbsrotate(right_thigh, 5*4*dt);
				oLimbsrotate(right_arm, -10*2*dt);
			}
		} 
	}

	void squat(double t, double t_start, double dt){
		if(t>=t_start&&t<t_start+10.0){	
			t-=t_start;
			double fbend=80.0;
			double sbend=45.0-fbend;
			if(t<1.0){
				bend(fbend*dt);
				oLimbsrotate(right_arm, -90*dt);
				Limbsrotate(right_forearm->component, -(fbend-90.0)*dt);
				Limbsrotate(left_forearm->component, -fbend*dt);
				
			}
			else if(t<1.5){
				head->r+=-60.0*dt;
			}
			else if(t<3.0){;}
			else if(t<4.0){
				double my;
				double thigh=right_thigh->height-right_thigh->shift;
				double calf=right_calf->height-right_calf->shift;
				my=thigh*fabs(cos((right_thigh->r+fbend+sbend*dt)*M_PI/180.0)-cos((right_thigh->r+50.0*dt+fbend+sbend*dt)*M_PI/180.0))+calf*fabs(cos((right_calf->r+20.0+fbend+sbend*dt)*M_PI/180.0)-cos((right_calf->r+20.0+60.0*dt+fbend+sbend*dt)*M_PI/180.0));
				bodyMove(9.0*dt-50*dt, -my+5*dt, 0.0);
				bend(sbend*dt);
				Limbsrotate(left_forearm, -90.0*dt);
				Limbsrotate(left_thigh, -75*dt);
				Limbsrotate(left_calf, 40*dt);
				oLimbsrotate(right_thigh, 50.0*dt);
				Limbsrotate(right_calf, 10.0*dt);
				Limbsrotate(right_forearm->component, -sbend*dt);
				Limbsrotate(left_forearm->component, -sbend*dt);
			}
			else if(t<5.0){
				
				bodyMove(-60*dt,0.0, 0.0);
			}
			else{
				gluLookAt((5-t)*100.0, 0, 1, (5-t)*100.0, 0, 0, 0, 1, 0);
				bodyMove(-60*dt*(10-t)/5,0.0, 0.0);
				right_forearm->component->parent_x+=-60/1.414*dt;
				right_forearm->component->parent_y+=-60/1.414*dt;
			}
			//else if(t<0.75){
			//	bodyMove(-5.0*2*dt, 0.0, 0.0);
			//	Limbsrotate(right_thigh, 10*4*dt);
			//	Limbsrotate(right_calf, -20*4*dt);
			//	oLimbsrotate(left_thigh, 5*4*dt);
			//}
			//else if(t<1.0){
			//	bodyMove(-5.0*2*dt, 0.0, 0.0);
			//	Limbsrotate(right_thigh, 0*4*dt);
			//	Limbsrotate(right_calf, -20*4*dt);
			//	oLimbsrotate(left_thigh, 5*4*dt);
			//}
		} 
	}

	void plot(){
		glTranslatef(x,y,0.0);
		glRotatef(r,0,0,1);
			body->plot();
			head->plot();
			left_thigh->plot();

			if(!front){
				body->plot();
				head->plot();
				right_thigh->plot();
			}
			right_thigh->plot();
			left_arm->plot();
			right_arm->plot();
		glRotatef(-r,0,0,1);
		glTranslatef(-x,-y,0.0);
	}
};


class BowString:public Component{
public:
	//float sy1=0.0;
	BowString(){;}
	BowString(double r): Component(r){
		parent_x=0.0;
		parent_y=0.0;
		shift=10.0;
		width=35.0;
		height=50.0;
	}
	void reset(float r){
		parent_x=0.0;
		parent_y=0.0;
		sy1=0.0;
		shift=10.0;
		width=35.0;
		height=50.0;
		if(component!=NULL)
				component->reset(r);
	}
	void plot(){
		//puts("Plot String");
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(1.0, 97.0/255.0, 0.0);
			//plot_Half_Circle(height,width,width);
			plotString(height,width,sy1);
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
};

class Arrow:public Component{
public:
	Arrow(){;}
	Arrow(double r): Component(r){
		parent_x=0.0;
		parent_y=-12.0;
		shift=10.0;
		width=35.0;
		height=50.0;
		component=new BowString(r);
	//	bs=component;
	}
	void reset(float r){
		parent_x=0.0;
		parent_y=-12.0;
		shift=10.0;
		width=35.0;
		height=50.0;
		if(component!=NULL)
				component->reset(r);
	}
	void Shift(float dy)
	{
		parent_y-=dy;
	}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(0.1, 0.1, 0.1);
			plotArrow(height,width);	
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
};

class Bow:public Component{
public:
	Bow(){;}
	Bow(double r): Component(r){
		parent_x=0.0;
		parent_y=-12.0;
		shift=10.0;
		width=35.0;
		height=50.0;
		component=new BowString(r);
	//	bs=component;
	}
	void reset(float r){
	
		parent_x=0.0;
		parent_y=-12.0;
		shift=10.0;
		width=35.0;
		height=50.0;
		if(component!=NULL)
			component->reset(r);
	}

	void plot(){
		glTranslatef(parent_x, parent_y+width, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(1.0, 97.0/255.0, 0.0);
			plot_Half_Circle(height,width,width);	
			if(component!=NULL){
				component->plot();
			}
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y-width, 0.0);
	}
};

class rArrow:public Component{
public:
	int stop;
	rArrow(){;}
	rArrow(double r): Component(r){
		reset(0);
	}
	void reset(float r){
		parent_x=0.0;
		parent_y=0.0;
		shift=10.0;
		width=35.0;
		height=80.0;
		stop=0;
		
		if(component!=NULL)
				component->reset(r);
	}
	void Shift(float ds)
	{
	//	parent_y+=(ds*sin(M_PI/6.0));
	//	parent_x+=(ds*cos(M_PI/6.0));
//		parent_y+=ds;
		parent_x+=ds;
	}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(0.1, 0.1, 0.1);
			plotrArrow(height,width,stop);	
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
};

class Target{
public:
	rArrow* arrow1;
	rArrow* arrow2;
	rArrow* arrow3;
	float x;
	float y;

	Target(){
		arrow1=new rArrow(0);
		arrow2=new rArrow(0);
		arrow3=new rArrow(0);
		x=150;
		y=150;
	}
	void reset(){
		arrow1->reset(0);
		arrow2->reset(0);
		arrow3->reset(0);
		
	}
	void reset2()
	{
		arrow2->reset(0);
	}
	void reset3()
	{
		arrow3->reset(0);
	}
	void plot(){
		plottarget(x,y);
		arrow1->plot();
		arrow2->plot();
		arrow3->plot();
	}
	void shift1(double dy,double tx,double ty)
	{
		arrow1->parent_y=ty;
		if(arrow1->parent_x<=tx)				
			arrow1->Shift(dy);
		else
			arrow1->stop=1;
	}
	void shift2(double dy,double tx,double ty)
	{
		arrow2->parent_y=ty;
		if(arrow2->parent_x<=tx)				
			arrow2->Shift(dy);
		else
			arrow2->stop=1;
	}
	
	void shift3(double dy,double tx,double ty)
	{
		arrow3->parent_y=ty;
		if(arrow3->parent_x<=tx)				
			arrow3->Shift(dy);
		else
			arrow3->stop=1;
	}
	

};



double t_pre=0.0;
float centx,centy,topx,topy;
Person* zmin=NULL;
Person* leo=NULL;
Bow* bw=NULL;
Bow* bw2=NULL;
BowString *bs=NULL;
BowString *bs2=NULL;
Arrow *ar=NULL;
Arrow *ar2=NULL;
Target* ztarget=NULL;
Target* ltarget=NULL;
void init(void){
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, worldx,0.0, worldy);
}
void plotArrow(float a,float b)
{
	glBegin(GL_LINE_STRIP);
	glVertex2i(0,0);
	glVertex2i(-a,0);
	glEnd();
}

void plotrArrow(float a,float b,int stop)
{
	glBegin(GL_POLYGON);
	glColor3f(255.0/255.0,140.0/255.0,0.0/255.0);
	glVertex2i(-a,2);
	glVertex2i(-a+10,0);
	glVertex2i(-a,-2);
	glEnd();
	glBegin(GL_POLYGON);
	if(stop==1)
		glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
	else
		glColor3f(211.0/255.0,211.0/255.0,211.0/255.0);
	glVertex2i(0,0);
	glVertex2i(0,1.5);
	glVertex2i(1.5,0);
	glVertex2i(0,-1.5);
	glEnd();

	glColor3f(192.0/255.0,192.0/255.0,192.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(0,0.6);
	glVertex2f(-a,0.6);
	glVertex2f(-a,-0.6);
	glVertex2f(0.0,-0.6);

	glEnd();
}

void plotString(float a,float b,float sy1)
{
	glBegin(GL_LINE_STRIP);
	glVertex2i(a,b*cos(75.0/360.0*2.0*M_PI));
	glVertex2i(0.0,sy1+b*cos(75.0/360.0*2.0*M_PI));
	glVertex2i(-a,b*cos(75.0/360.0*2.0*M_PI));
	glEnd();
}
void plotCircle(float a, float b){
	float n_side=30;
	glBegin(GL_POLYGON);
	for(int i=0;i<n_side;i++){
		glVertex2i(a*cos(i/n_side*2*M_PI), b*sin(i/n_side*2*M_PI));
	}
	glEnd();
}
void plotCircleLine(float a, float b){
	float n_side=30;
	glBegin(GL_LINE_LOOP);
	for(int i=0;i<n_side;i++){
		glVertex2i(a*cos(i/n_side*2*M_PI), b*sin(i/n_side*2*M_PI));
	}
	glEnd();
}

void plot_Half_Circle(float a, float b,float w){
	float n_side=360.0;
	for(int i=-75;i<=75;i++){
		float x=b*cos(i/n_side*2.0*M_PI);
		float y=a*sin(i/n_side*2.0*M_PI);

		glRectf(x-1,y-1,x+1,y+1);
	}
}

void plotCurling(){
	plotCircle(7.0,7.0);
	glColor3f(0.0,0.0,0.0);
	glRectf(0.0,1.0,5.0,-1.0);
}

int i=0;
void lwalkpic(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		//gluLookAt(280.0, 150.0, 10.0, 280.0, 150.0, 0.0, 0.0, 1.0, 0.0);
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(leo==NULL) {
			leo=new Person(150,100);
			leo->body->R=0.0;
			leo->body->G=0.0;
			leo->body->B=0.0;
		}
		if(bw2==NULL) {
			bw2=new Bow(-90.0);
	/*		bw2->R=200.0/255.0;
			bw2->G=200.0/255.0;
			bw2->B=255.0/255.0;
	*/	}
		
		//if(leo->left_forearm->component==NULL) leo->take(leo->left_forearm, bw2);
		//if(leo->right_forearm->component==NULL) leo->take(leo->right_forearm, curling_2);
		
		
		if(t<1) ;
		else if(t<2)leo->bodyToside(1.0, dt);
		zmin->setfront(100,200);
		zmin->plot();
		leo->istep(t, 2.0, dt, 2);
		leo->lstep(t, 2.5, dt, 2);
		leo->rstep(t, 3.25, dt, 2);
		leo->lstep(t, 4.0, dt, 2);
		leo->rstep(t, 4.75, dt, 2);
		leo->lstep(t, 5.5, dt, 2);
		leo->rstep(t, 6.25, dt, 2);
		leo->lstep(t, 7.0, dt, 2);
		if(t>=7.75&&t<8.25)leo->oLimbsrotate(leo->right_arm, 10*4*dt);
//		background();
		leo->plot();
	}
}


void finalscene(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		//double dt=t-t_pre;
		//t-=t_start;
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		Ground();
		if(zmin==NULL) {zmin=new Person(100,200);zmin->body->R=1.0;zmin->body->G=1.0;zmin->body->B=1.0;}
		if(leo==NULL){leo=new Person(80,100);leo->body->R=0.0;leo->body->G=0.0;leo->body->B=0.0;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(bw2==NULL) 
				bw2=new Bow(-90.0);
		
		if(ar==NULL)
				ar=new Arrow(90.0);

		if(ar2==NULL)
				ar2=new Arrow(90.0);

		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
				zmin->take(zmin->right_forearm,ar);
		}
		if(leo->left_forearm->component==NULL) {
				leo->take(leo->left_forearm, bw2);
				leo->take(leo->right_forearm,ar2);
		}
		if(t<1)
		{

		}
		else if(t<2)
		{
				leo->setfront(80,100);
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
		}
		else if(t<3)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
		}
		else if(t<4)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
				glColor3f(0,0,0);
				output(160,190,"27",font_18);
				glColor3f(1,0,0);
				output(140,90,"28",font_18);
		}
		else if(t<5)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
				glColor3f(0,0,0);
				output(160,190,"27",font_18);
				glColor3f(1,0,0);
				output(140,90,"28",font_18);
				glColor3f(0,0,0);
				output(170,190,"27",font_18);
		}
		else if(t<6)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
				glColor3f(0,0,0);
				output(160,190,"27",font_18);
				glColor3f(1,0,0);
				output(140,90,"28",font_18);
				glColor3f(0,0,0);
				output(170,190,"27",font_18);
				glColor3f(1,0,0);
				output(150,90,"28",font_18);
		}
		else if(t<11)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
				glColor3f(0,0,0);
				output(160,190,"27",font_18);
				glColor3f(1,0,0);
				output(140,90,"28",font_18);
				glColor3f(0,0,0);
				output(170,190,"27",font_18);
				glColor3f(1,0,0);
				output(150,90,"28",font_18);
				glColor3f(1,0,0);
				output(160,90,"Win!!",font);
		}
			zmin->plot();
			leo->plot();
	}

}

void firstscene(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		//double dt=t-t_pre;
		//t-=t_start;
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		Ground();
		if(zmin==NULL) {zmin=new Person(100,200);zmin->body->R=1.0;zmin->body->G=1.0;zmin->body->B=1.0;}
		if(leo==NULL){leo=new Person(80,100);leo->body->R=0.0;leo->body->G=0.0;leo->body->B=0.0;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(bw2==NULL) 
				bw2=new Bow(-90.0);
		
		if(ar==NULL)
				ar=new Arrow(90.0);

		if(ar2==NULL)
				ar2=new Arrow(90.0);

		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
				zmin->take(zmin->right_forearm,ar);
		}
		if(leo->left_forearm->component==NULL) {
				leo->take(leo->left_forearm, bw2);
				leo->take(leo->right_forearm,ar2);
		}
		if(t<1)
		{

		}
		else if(t<2)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
		}
		else if(t<3)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
		}
		else if(t<4){
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
		}
		else if(t<5)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
		}
		else if(t<6)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
				glColor3f(0,0,0);
				output(160,190,"27",font_18);
		}
		else if(t<7)
		{
				glColor3f(0,0,0);
				output(150,200,"TWN",font);
				glColor3f(0,0,0);
				output(130,100,"MEX",font);
				glColor3f(0,0,0);
				output(150,190,"26",font_18);
				glColor3f(1,0,0);
				output(130,90,"28",font_18);
				glColor3f(0,0,0);
				output(160,190,"27",font_18);
				glColor3f(1,0,0);
				output(140,90,"28",font_18);
		}
		
			zmin->plot();
			leo->plot();
	}

}

void zupset(double t,double t_start, double t_end,double zs,double ls){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		
		Ground();
		ScoreBar(zs,ls);
		if(zmin==NULL) {zmin=new Person(100,200);zmin->body->R=1.0;zmin->body->G=1.0;zmin->body->B=1.0;}
		if(leo==NULL){leo=new Person(80,100);leo->body->R=0.0;leo->body->G=0.0;leo->body->B=0.0;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(bw2==NULL) 
				bw2=new Bow(-90.0);
		
		if(ar==NULL)
				ar=new Arrow(90.0);

		if(ar2==NULL)
				ar2=new Arrow(90.0);

		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
				zmin->take(zmin->right_forearm,ar);
		}
		if(leo->left_forearm->component==NULL) {
				leo->take(leo->left_forearm, bw2);
				leo->take(leo->right_forearm,ar2);
		}
		if(t<1)
		{
			zmin->setfront(100,200);
			leo->setfront(80,100);
		}
		else if(t<2)
		{
			float dangle=30.0/1.0;
			zmin->oLimbsrotate(zmin->head, dt*dangle);
		}
		else if(t<3){
			float dangle=-90.0/1.0;
			zmin->oLimbsrotate(zmin->right_arm, dt*dangle);
			zmin->oLimbsrotate(zmin->right_forearm, dt*(-120));
		}
		else if(t<4)
		{
		
		}
		else if(t<5)
		{
			float dangle=-30.0/1.0;
			zmin->oLimbsrotate(zmin->head, dt*dangle);
			dangle=90.0/1.0;
			zmin->oLimbsrotate(zmin->right_arm, dt*dangle);
			zmin->oLimbsrotate(zmin->right_forearm, dt*(120));
		}

			zmin->plot();	
			leo->plot();
	}

}
void zcheer(double t,double t_start, double t_end,double zs,double ls){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		
		Ground();
		ScoreBar(zs,ls);
		if(zmin==NULL) {zmin=new Person(100,200);zmin->body->R=1.0;zmin->body->G=1.0;zmin->body->B=1.0;}
		if(leo==NULL){leo=new Person(80,100);leo->body->R=0.0;leo->body->G=0.0;leo->body->B=0.0;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(bw2==NULL) 
				bw2=new Bow(-90.0);
		
		if(ar==NULL)
				ar=new Arrow(90.0);

		if(ar2==NULL)
				ar2=new Arrow(90.0);

		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
				zmin->take(zmin->right_forearm,ar);
		}
		if(leo->left_forearm->component==NULL) {
				leo->take(leo->left_forearm, bw2);
				leo->take(leo->right_forearm,ar2);
		}
		if(t<1)
		{
			zmin->setfront(100,200);
			leo->setfront(80,100);
		}
		else if(t<2)
		{
			float dangle=120.0/1.0;
			zmin->oLimbsrotate(zmin->right_forearm, dt*dangle);
		}
		else if(t<3){
			float dangle=90.0/1.0;
			zmin->oLimbsrotate(zmin->right_arm, dt*dangle);
			zmin->oLimbsrotate(zmin->right_forearm, dt*(-30));
		}
		else if(t<4)
		{
			float dangle=-90.0/1.0;
			zmin->oLimbsrotate(zmin->right_arm, dt*dangle);
			zmin->oLimbsrotate(zmin->right_forearm, dt*(30));
		}
		else if(t<5)
		{
		}
		else if(t<6)
		{
			float dangle=120.0/1.0;
			zmin->oLimbsrotate(zmin->right_forearm, -dt*dangle);
		}

			zmin->plot();	
			leo->plot();
	}

}
void zshot(double t,double t_start, double t_end,double zs, double ls){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		
		Ground();
		ScoreBar(zs,ls);
		if(zmin==NULL) {zmin=new Person(100,200);zmin->body->R=1.0;zmin->body->G=1.0;zmin->body->B=1.0;}
		if(leo==NULL){leo=new Person(80,100);leo->body->R=0.0;leo->body->G=0.0;leo->body->B=0.0;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(bw2==NULL) 
				bw2=new Bow(-90.0);
		
		if(ar==NULL)
				ar=new Arrow(90.0);

		if(ar2==NULL)
				ar2=new Arrow(90.0);

		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
				zmin->take(zmin->right_forearm,ar);
		}
		if(leo->left_forearm->component==NULL) {
				leo->take(leo->left_forearm, bw2);
				leo->take(leo->right_forearm,ar2);
		}
		if(t<1)
		{
			leo->setfront(80,100);
			zmin->plot();
		}
		else if(t<2)
		{
			zmin->headToside(1.0,dt);
			zmin->oLimbsrotate(zmin->left_thigh,dt*5);
			zmin->oLimbsrotate(zmin->right_thigh,-dt*5);

			zmin->plot();
			leo->plot();
			t_pre=t;
		}
		else if(t<5){
			float dangle=90.0/3.0;
			zmin->oLimbsrotate(zmin->left_arm, dt*dangle);
			zmin->oLimbsrotate(zmin->right_arm, dt*dangle);
			zmin->plot();
			leo->plot();
			t_pre=t;
		}
		else if(t<8)
		{
			zmin->Limbshift(zmin->right_forearm,dt*4);
			double *tmpy=&(zmin->left_forearm->component->component->sy1);
			*tmpy=*tmpy-dt*4;
			zmin->plot();
			leo->plot();
			t_pre=t;
		}
		else if(t<9)
		{
			ar->Shift(dt*250);
			zmin->plot();
			leo->plot();
			t_pre=t;
		}
		
			leo->plot();
	}

}

void ztarg(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ztarget==NULL)
				ztarget=new Target();
		if(t<1){
			ztarget->reset();
			ztarget->plot();
		}
		if(t<5)
		{
			ztarget->plot();
			ztarget->shift1(dt*100,150,160);
			ztarget->shift2(dt*80,140,140);
			ztarget->shift3(dt*130,110,150);
		
		}
		else if(t<5.5)
		{
			ztarget->plot();
			ztarget->reset();
			
		}
		else if(t<10)
		{
			ztarget->plot();
			ztarget->shift1(dt*100,170,170);
			ztarget->shift2(dt*80,140,140);
			ztarget->shift3(dt*130,110,150);
		}
	
	}
}

void ztarg1(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ztarget==NULL)
				ztarget=new Target();
		if(t<1){
			ztarget->reset();
			ztarget->plot();
		}
		if(t<4)
		{
			ztarget->plot();
			ztarget->shift1(dt*100,150,159);
		}
		ztarget->plot();
	}
}
void ztarg2(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ztarget==NULL)
				ztarget=new Target();
		if(t<1){
			ztarget->reset2();
			ztarget->plot();
		}
		if(t<4)
		{
			ztarget->plot();
			ztarget->shift2(dt*100,149,139);
		}
		ztarget->plot();
	}
}
void ztarg3(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ztarget==NULL)
				ztarget=new Target();
		if(t<1){
			ztarget->reset3();
			ztarget->plot();
		}
		if(t<4)
		{
			ztarget->plot();
			ztarget->shift3(dt*100,172,150);
		}
		ztarget->plot();
	}
}
void ltarg1(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ltarget==NULL)
				ltarget=new Target();
		if(t<1){
			ltarget->reset();
			ltarget->plot();
		}
		if(t<4)
		{
			ltarget->plot();
			ltarget->shift1(dt*100,152,149);
		}
		ltarget->plot();
	}
}
void ltarg2(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ltarget==NULL)
				ltarget=new Target();
		if(t<1){
			ltarget->reset2();
			ltarget->plot();
		}
		if(t<4)
		{
			ltarget->plot();
			ltarget->shift2(dt*100,164.14,135.86);
		}
		ltarget->plot();
	}
}
void ltarg3(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ltarget==NULL)
				ltarget=new Target();
		if(t<1){
			ltarget->reset3();
			ltarget->plot();
		}
		if(t<4)
		{
			ltarget->plot();
			ltarget->shift3(dt*100,150,130);
		}
		ltarget->plot();
	}
}
void lshot(double t,double t_start, double t_end,double zs,double ls){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		ScoreBar(zs,ls);
		Ground();
		if(zmin==NULL) {zmin=new Person(100,200);zmin->body->R=0.0;zmin->body->G=0.0;zmin->body->B=0.8;}
		if(leo==NULL){leo=new Person(80,100);leo->body->R=0.0;leo->body->G=0.0;leo->body->B=0.0;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(bw2==NULL) 
				bw2=new Bow(-90.0);
		
		if(ar==NULL)
				ar=new Arrow(90.0);

		if(ar2==NULL)
				ar2=new Arrow(90.0);

		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
				zmin->take(zmin->right_forearm,ar);
		}
		if(leo->left_forearm->component==NULL) {
				leo->take(leo->left_forearm, bw2);
				leo->take(leo->right_forearm,ar2);
		}
		zmin->setfront(100,200);
		zmin->plot();
		if(t<1){
			leo->plot();
		}
		else if(t<2)
		{
			leo->headToside(1.0,dt);
			leo->oLimbsrotate(leo->left_thigh,dt*5);
			leo->oLimbsrotate(leo->right_thigh,-dt*5);
			leo->plot();
			t_pre=t;
		}
		else if(t<5){
			float dangle=90.0/3.0;
			leo->oLimbsrotate(leo->left_arm, dt*dangle);
			leo->oLimbsrotate(leo->right_arm, dt*dangle);
			//zmin->plot();
			leo->plot();
			t_pre=t;
		}
		else if(t<8)
		{
			leo->Limbshift(leo->right_forearm,dt*4);
			double *tmpy=&(leo->left_forearm->component->component->sy1);
			*tmpy=*tmpy-dt*4;
			leo->plot();
			//zmin->plot();
			t_pre=t;
		}
		else if(t<10)
		{
			ar2->Shift(dt*200);
			//zmin->plot();
			leo->plot();
			t_pre=t;
		}

	}

}

void plottarget(double x,double y)
{
		float Radius_a=100;
		float Radius_b=100;
		float R,G,B;
		glTranslatef(x,y,0.0);
//		glRotatef(10,0,0,1);
		for(int idx=1;idx<=10;idx++)
		{
			if(idx<=2)
			{	R=1;G=1;B=1;
			}else if(idx<=4)
			{	R=0.1;G=0.1;B=0.1;
			}else if(idx<=6)
			{	R=0;G=0;B=1;
			}else if(idx<=8)
			{	R=1;G=0;B=0;
			}else if(idx<=10)
			{	R=1;G=1;B=0;
			}	
			glColor3f(R,G,B);
			plotCircle(Radius_a,Radius_b);
			glColor3f(0.0,0.0,0.0);
			plotCircleLine(Radius_a,Radius_b);
			Radius_a-=10;
			Radius_b-=10;
		}
//		glRotatef(-10,0,0,1);
		glTranslatef(-x,-y,0);
}
void ScoreBar(int zs,int ls)
{
	double s=1;
	glColor3f(120.0/255.0,0.0/255.0,0.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(5-s,270-s);
	glVertex2f(95+s,270-s);
	glVertex2f(95+s,290+s);
	glVertex2f(5-s,290+s);
	glVertex2f(5-s,270-s);
	glEnd();

	glColor3f(220.0/255.0,20.0/255.0,60.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(5,270);
	glVertex2f(95,270);
	glVertex2f(95,290);
	glVertex2f(5,290);
	glVertex2f(5,270);
	glEnd();

//inning ban
	glColor3f(255.0/255.0,0.0/255.0,255.0/255.0);
	glBegin(GL_POLYGON);

	glVertex2f(65,270);
	glVertex2f(75,270);
	glVertex2f(75,290);
	glVertex2f(65,290);
	glVertex2f(65,270);
	glEnd();
//nation
	glColor3f(0.0/255.0,0.0/255.0,0.0/255.0);
	char* twn="TWN";
	char* mex="MEX";
	output(10,283,twn,font_10);
	output(10,273,mex,font_10);
//inning
	char *tin="0";
	char *xin="4";
	output(70,283,tin,font_10);
	output(70,273,xin,font_10);
//score
	char tsc[3];
	sprintf(tsc,"%d",zs);
	char xsc[3];
	sprintf(xsc,"%d",ls);
	output(85,283,tsc,font_10);
	output(85,273,xsc,font_10);

}
void Kanban(double x,double y)
{
	glColor3f(0.0/255.0,0.0/255.0,139.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(x,y);
	glVertex2f(x+80,y);
	glVertex2f(x+80+6.5,y+30);
	glVertex2f(x+6.5,y+30);
	glVertex2f(x,y);
	glEnd();

	glColor3f(0.0/255.0,0.0/255.0,205.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(x,y);
	glVertex2f(x+79,y);
	glVertex2f(x+79+6.5,y+30);
	glVertex2f(x+6.5,y+30);
	glVertex2f(x,y);
	glEnd();
}
void Ground()
{
	//ScoreBar(0,0);
//Kanban
	for(int idx=0;idx<6;idx++)
	{
		Kanban(80*idx,230);
	}
//Grass
	glColor3f(124.0/255.0,252.0/255.0,0.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(0,0);
	glVertex2f(300,0);
	glVertex2f(300,230);
	glVertex2f(0,230);
	glVertex2f(0,0);
	glEnd();
//White Line

	glColor3f(255.0/255.0,252.0/255.0,255.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(85,0);
	glVertex2f(90,0);
	glVertex2f(140,230);
	glVertex2f(135,230);
	glVertex2f(85,0);
	glEnd();
//White Line

	glColor3f(255.0/255.0,252.0/255.0,255.0/255.0);
	glBegin(GL_POLYGON);
	glVertex2f(0,150);
	glVertex2f(90.0+32.0,150);
	glVertex2f(90.0+32.5,155);
	glVertex2f(0,155);
	glVertex2f(0,150);
	glEnd();
	
//Kanban
	for(int idx=0;idx<6;idx++)
	{
		Kanban(80*idx,0);
	}

}
void display(void)
{
	double t = glutGet(GLUT_ELAPSED_TIME) / 500.0;
	glClear(GL_COLOR_BUFFER_BIT);
	double t_start, t_end;
	t_start=t_end=0.0;
	glPushMatrix();
		


		tctr(&t_start, &t_end, 7);
		firstscene(t, t_start, t_end);
//1st	
		tctr(&t_start, &t_end, 9);
		zshot(t, t_start, t_end, 0, 0);

		tctr(&t_start, &t_end, 5);
		ztarg1(t, t_start, t_end);
		//cheer
		tctr(&t_start, &t_end, 6);
		zcheer(t, t_start, t_end,10,0);
		//
		tctr(&t_start, &t_end, 10);
		lshot(t, t_start, t_end,10,0);
		
		tctr(&t_start, &t_end, 5);
		ltarg1(t, t_start, t_end);
//2nd
		tctr(&t_start, &t_end, 9);
		zshot(t, t_start, t_end, 10, 10);
		
		tctr(&t_start, &t_end, 5);
		ztarg2(t, t_start, t_end);	
		
		tctr(&t_start, &t_end, 10);
		lshot(t, t_start, t_end,19,10);
		
		tctr(&t_start, &t_end, 5);
		ltarg2(t, t_start, t_end);

//3rd

		tctr(&t_start, &t_end, 9);
		zshot(t, t_start, t_end, 19, 18);
		
		tctr(&t_start, &t_end, 5);
		ztarg3(t, t_start, t_end);	
		//upset
		tctr(&t_start, &t_end, 5);
		zupset(t, t_start, t_end,27,18);
		//
		tctr(&t_start, &t_end, 10);
		lshot(t, t_start, t_end, 27, 18);
		
		tctr(&t_start, &t_end, 5);
		ltarg3(t, t_start, t_end);


		tctr(&t_start, &t_end, 7);
		finalscene(t, t_start, t_end);


//		tctr(&t_start, &t_end, 8);
//		ztarg(t, t_start, t_end);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

static void idle(void){
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowPosition(300,300);
	glutInitWindowSize(900,900);
	glutCreateWindow("An Example OpenGL Program");
	init();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutMainLoop();
}
