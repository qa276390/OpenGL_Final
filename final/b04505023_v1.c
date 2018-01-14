#include "stdio.h"
#include <iostream>
#include <GL/freeglut.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include "math.h"
#define _USE_MATH_DEFINES
#define worldx 350.0
#define worldy 350.0
using namespace std;
void plotCircle(float a, float b);

class Component{
public:
	double parent_x, parent_y;
	double r;
	double shift;
	double width;
	double height;
	Component* component=NULL;
	Component(){;}
	Component(double r){this->r=r;}
	virtual void plot()=0;
};

class Limbs:public Component{
public:
	Limbs(double parent_x, double parent_y, double r){
		this->parent_x=parent_x;
		this->parent_y=parent_y;
		this->r=r;
	}
	Limbs(){;}
	Limbs(double r){this->r=r;}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(0, 0, 0);
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
		height=13.0;
	}
	void reset(double r){
		this->r=r;
		parent_x=0.0;
		parent_y=-12.0;
		shift=1.0;
		width=5.0;
		height=13.0;
		component=NULL;
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
	void reset(double parent_x, double parent_y, double r){
		this->parent_x=parent_x;
		this->parent_y=parent_y;
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
		height=15.0;
	}
	void reset(double r){
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
		width=20.0;
		height=20.0;
	}	
	void reset(double r){
		this->r=r;
		parent_x=0.0;
		parent_y=45.0;
		shift=0.0;
		width=20.0;
		height=20.0;
	}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(0.0, 0.0, 0.0);
			plotCircle(width+0.8, height+0.8);
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
		glRectf(-width/2-0.8, 30.0+0.8, width/2+0.8, 0.0-0.8);
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
		right_arm=new Arm(-10,20.0,0.0);
		left_arm=new Arm(10,20.0,0.0);
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
			right_thigh->plot();
			right_arm->plot();
			if(!front){
				body->plot();
				head->plot();
				right_thigh->plot();
			}
			left_thigh->plot();
			left_arm->plot();
		glRotatef(-r,0,0,1);
		glTranslatef(-x,-y,0.0);
	}
};

class Wand:public Component{
public:
	double R, G, B;
	Wand(){;}
	Wand(double r): Component(r){
		R=1.0;
		G=97.0/255.0;
		B=0.0;
		parent_x=0.0;
		parent_y=-12.0;
		shift=30.0;
		width=4.0;
		height=50.0;
	}
	void reset(){
		r=0.0;
		parent_x=0.0;
		parent_y=-12.0;
		shift=30.0;
	}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(R, G, B);
			glRectf(-width/2, shift, width/2, shift-height);
			glRectf(-width*3/2, shift-height, width*3/2, shift-height-5.0);
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
};

class Curling:public Component{
public:
	double R, G, B;
	Curling(){;}
	Curling(double r): Component(r){
		R=153.0/255.0;
		G=230.0/255.0;
		B=77.0/255.0;
		parent_x=0.0;
		parent_y=-12.0;
		shift=0.0;
		width=10.0;
		height=5.0;
	}
	void reset(){
		r=0.0;
		parent_x=0.0;
		parent_y=-12.0;
		shift=0.0;
	}
	void plot(){
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(R, G, B);
			glRectf(-width/2, shift, width/2, shift-height);
		glRotatef(-r,0,0,1);
		glTranslatef(-parent_x, -parent_y, 0.0);
	}
	
};
class Curling2{
public:
	double x;
	double y;
	double r;

	Curling2(){
		Curling2(310.0, 150.0);
	}
	Curling2(double x, double y){
		this->x=x;
		this->y=y;
		r=0.0;
	}
	void move(double dx, double dy, double dr){
		this->x+=dx;
		this->y+=dy;
		this->r+=dr;
	}
	void plot(){
		glTranslatef(x,y,0.0);
		glRotatef(r,0,0,1);
		plotCircle(7.0,7.0);
		glColor3f(0.0,0.0,0.0);
		glRectf(0.0,1.0,5.0,-1.0);
		glRotatef(-r,0,0,1);
		glTranslatef(-x,-y,0.0);
	}
};

//global varible
double t_pre=0.0;
float centx,centy,topx,topy;
Person* ticba=NULL;
Person* leo=NULL;
Wand* wd=NULL;
Wand* wd2=NULL;
Curling* curling=NULL;
Curling* curling_2=NULL;
Curling2** cl=new Curling2*[6];
void init(void){
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, worldx,0.0, worldy);
}

void plotU(){
	float opt[6][2],ipt[6][2];
	opt[0][0] = cos(1.0/7*M_PI)*(-30)-sin(1.0/7*M_PI)*0;
	opt[0][1] = sin(1.0/7*M_PI)*(-30)+cos(1.0/7*M_PI)*0;
	for(int i=1;i<6;i++){
		opt[i][0] = cos(1.0/7*M_PI)*opt[i-1][0]-sin(1.0/7*M_PI)*opt[i-1][1];
		opt[i][1] = sin(1.0/7*M_PI)*opt[i-1][0]+cos(1.0/7*M_PI)*opt[i-1][1];
	}
	ipt[0][0] = cos(1.0/7*M_PI)*(-20)-sin(1.0/7*M_PI)*0;
	ipt[0][1] = sin(1.0/7*M_PI)*(-20)+cos(1.0/7*M_PI)*0;
	for(int i=1;i<6;i++){
		ipt[i][0] = cos(1.0/7*M_PI)*ipt[i-1][0]-sin(1.0/7*M_PI)*ipt[i-1][1];
		ipt[i][1] = sin(1.0/7*M_PI)*ipt[i-1][0]+cos(1.0/7*M_PI)*ipt[i-1][1];
	}
	for(int i=0;i<6;i++){
		ipt[i][0] += 150;
		ipt[i][1] += 250;
		opt[i][0] += 150;
		opt[i][1] += 250;
	}
	glColor3f(0.0,0.0,0.0);
	glLineWidth(6.0);
	glLineStipple(1,0xAAAA);
	glBegin(GL_LINE_LOOP);
		glVertex2i( 150-20, 250);
		glVertex2i( 150-20, 290);
		glVertex2i( 150-30, 290);
		glVertex2i( 150-30, 250);
		for(int i=0;i<6;i++)
				glVertex2i(opt[i][0],opt[i][1]);
		glVertex2i( 150+30, 250);
		glVertex2i( 150+30, 290);
		glVertex2i( 150+20, 290);
		glVertex2i( 150+20, 250);
		for(int i=0;i<6;i++)
				glVertex2i(ipt[5-i][0],ipt[5-i][1]);
	glEnd();
}
void plotStar(float cx, float cy, float tx,float ty,float R, float G, float B)
{
	centx = cx;
	centy = cy;
	topx = tx;
	topy = ty;
	glColor3f(R,G,B);
	float opt[5][2];
	float ipt[5][2];
	opt[0][0] = topx; opt[0][1] = topy;
	ipt[0][0] = -0.4*topx; ipt[0][1] = -0.4*topy;
	for(int i=1;i<5;i++){
		opt[i][0] = cos(0.4*M_PI)*opt[i-1][0]-sin(0.4*M_PI)*opt[i-1][1];
		ipt[i][0] = -0.4*opt[i][0];
		opt[i][1] = sin(0.4*M_PI)*opt[i-1][0]+cos(0.4*M_PI)*opt[i-1][1];
		ipt[i][1] = -0.4*opt[i][1];
	}
	for(int i=0;i<5;i++){
		opt[i][0] += centx;
		opt[i][1] += centy;
		ipt[i][0] += centx;
		ipt[i][1] += centy;
	}
	glBegin(GL_POLYGON);
	for(int i=0;i<5;i++){	
		glVertex2i( ipt[i][0], ipt[i][1]);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	for(int i=0;i<5;i++){
		glVertex2i( opt[i][0], opt[i][1]);
		glVertex2i( ipt[(i+2)%5][0], ipt[(i+2)%5][1]);
		glVertex2i( ipt[(i+3)%5][0], ipt[(i+3)%5][1]);
	}
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
void taudiance(double x, double y, double scale){
	glScalef(scale, scale, 1.0);
		Person person(1/scale*x,1/scale*y);
		person.plot();
	glScalef(1/scale, 1/scale,1.0);
}
void laudiance(double x, double y, double scale){
	glScalef(scale, scale, 1.0);
		Person person(1/scale*x,1/scale*y);
		person.body->R=0.0;
		person.body->G=0.0;
		person.body->B=0.0;
		person.plot();
	glScalef(1/scale, 1/scale,1.0);
}
void background(){
	for(int i=0;i<15;i++){
		if(i<8)laudiance(-200+50*i, 300, 0.7);
		else taudiance(-200+50*i, 300, 0.7);
	}
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 280, 1000, 234);
	glColor3f(0.73, 0.73, 0.73);
	glRectf(-500, 234, 1000, 230);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 230, 1000, 220);
	glColor3f(0.7, 0.7, 0.7);
	glRectf(-500, 220, 1000, 214);
	glColor3f(0.7, 0.7, 0.7);
	glRectf(-500, 120, 1000, 110);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 110, 1000, 90);
	glColor3f(0.67, 0.67, 0.67);
	glRectf(-500, 90, 1000, 78);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 78, 1000, 20);

	glTranslatef(150, 175, 0);
		glColor3f(0.9, 0.0, 0.0);
		plotCircle(20, 10);
		glColor3f(1.0, 1.0, 1.0);
		plotCircle(13, 5.5);
	glTranslatef(-150, -175, 0);

	glTranslatef(110, 175, 0);
	glRotatef(0, 0, 0, 1);
		char str1[14]={'C',' ','U',' ','R',' ','L',' ','I',' ','N',' ','G'};
			int len = (int) strlen(str1);
			for (int i = 0 ; i < len; i++)
			{
				glColor3f(0.0,0.0,0.0);
				glRasterPos2f(6*i,0.0);
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,str1[i]);
			}
	glRotatef(0, 0, 0, 1);
	glTranslatef(-110, -175, 0);
}

void background2(){
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 280, 1000, 230);
	glColor3f(0.7, 0.7, 0.7);
	glRectf(-500, 235, 1000, 230);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 230, 1000, 220);
	glColor3f(0.7, 0.7, 0.7);
	glRectf(-500, 220, 1000, 215);
	glColor3f(0.7, 0.7, 0.7);
	glRectf(-500, 85, 1000, 80);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 80, 1000, 70);
	glColor3f(0.7, 0.7, 0.7);
	glRectf(-500, 70, 1000, 65);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-500, 65, 1000, 15);

}

void plotzone(double x, double y){
	background2();
	glColor3f(0.0,0.0,1.0);
	glTranslatef(x, y, 0.0);
		plotCircle(50.0, 50.0);
		glColor3f(1.0,1.0,1.0);
		plotCircle(35.0, 35.0);
		glColor3f(1.0,0.0,0.0);
		plotCircle(20.0, 20.0);
		glColor3f(1.0,1.0,1.0);
		plotCircle(10.0, 10.0);
	glTranslatef(-x, -y, 0.0);
}
void lwalkpic(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		//gluLookAt(280.0, 150.0, 10.0, 280.0, 150.0, 0.0, 0.0, 1.0, 0.0);
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(leo==NULL) {
			leo=new Person(290,200);
			leo->body->R=0.0;
			leo->body->G=0.0;
			leo->body->B=0.0;
		}
		if(wd2==NULL) {
			wd2=new Wand(0.0);
			wd2->R=200.0/255.0;
			wd2->G=200.0/255.0;
			wd2->B=255.0/255.0;
		}
		if(curling_2==NULL) {
			curling_2=new Curling(0.0);
			curling_2->R=130.0/255.0;
			curling_2->G=90.0/255.0;
			curling_2->B=255.0/255.0;
		}
		if(leo->left_forearm->component==NULL) leo->take(leo->left_forearm, wd2);
		if(leo->right_forearm->component==NULL) leo->take(leo->right_forearm, curling_2);
		if(t<1) ;
		else if(t<2)leo->bodyToside(1.0, dt);
		leo->istep(t, 2.0, dt, 2);
		leo->lstep(t, 2.5, dt, 2);
		leo->rstep(t, 3.25, dt, 2);
		leo->lstep(t, 4.0, dt, 2);
		leo->rstep(t, 4.75, dt, 2);
		leo->lstep(t, 5.5, dt, 2);
		leo->rstep(t, 6.25, dt, 2);
		leo->lstep(t, 7.0, dt, 2);
		if(t>=7.75&&t<8.25)leo->oLimbsrotate(leo->right_arm, 10*4*dt);
		background();
		leo->plot();
	}
}
void twalkpic(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		//gluLookAt(280.0, 150.0, 10.0, 280.0, 150.0, 0.0, 0.0, 1.0, 0.0);
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ticba==NULL) ticba=new Person(290,200);
		if(wd==NULL) wd=new Wand(0.0);
		if(curling==NULL) curling=new Curling(0.0);
		if(ticba->canreset){
			ticba->setfront(290.0, 200.0);
			wd->reset();
			curling->reset();
		}
		if(ticba->left_forearm->component==NULL) ticba->take(ticba->left_forearm, wd);
		if(ticba->right_forearm->component==NULL) ticba->take(ticba->right_forearm, curling);
		if(t<1) ;
		else if(t<2)ticba->bodyToside(1.0, dt);
		ticba->istep(t, 2.0, dt, 2);
		ticba->lstep(t, 2.5, dt, 2);
		ticba->rstep(t, 3.25, dt, 2);
		ticba->lstep(t, 4.0, dt, 2);
		ticba->rstep(t, 4.75, dt, 2);
		ticba->lstep(t, 5.5, dt, 2);
		ticba->rstep(t, 6.25, dt, 2);
		ticba->lstep(t, 7.0, dt, 2);
		if(t>=7.75&&t<8.25)ticba->oLimbsrotate(ticba->right_arm, 10*4*dt);

		background();
		ticba->plot();
	}
}
void firstzonepic(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
			//target circle
			plotzone(60, 150);
			//curling movng
			if(cl[0]==NULL){
				cl[0]=new Curling2(310.0, 150.0);
			}
			
			double _t=t;
			double t1=5;

			if(t<t1)cl[0]->move(-250.0*(1.568*dt/t1)*cos(_t/t1*M_PI/2.0), 0.0, 360.0*(1.568*dt/t1)*cos(_t/t1*M_PI/2.0));
			glColor3f(153.0/255.0, 230.0/255.0, 77.0/255.0);
			cl[0]->plot();

	}
}
void secondzonepic(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		glPushMatrix();
			//target circle
			plotzone(60, 150);
			//curling moving
			if(cl[0]==NULL) cl[0]=new Curling2(310.0, 150.0);
			if(cl[1]==NULL) cl[1]=new Curling2(310.0, 150.0);
			
			double _t=t;
			double t2=4.0;
			double t3=3.0;
			if(t<t2);
			else if(t<t2+t3){
				_t=t-t2;
			}

			if(t<t2);
			else if(t<t2+t3){
				cl[0]->move(-30.0*(1.568*dt/t3)*cos(_t/t3*M_PI/2.0), 0.0, 50.0*(1.568*dt/t3)*cos(_t/t3*M_PI/2.0));
			}
			glColor3f(153.0/255.0, 230.0/255.0, 77.0/255.0);
			cl[0]->plot();

			if(t<t2){
				cl[1]->move(-250.0*(1.385*dt/t2)*cos(_t/(t2+0.3)*M_PI/2.0), 0.0, 360.0*(1.4*dt/(t2+0.3))*cos(_t/t2*M_PI/2.0));
			}
			glColor3f(130.0/255.0, 90.0/255.0, 255.0/255.0);
			cl[1]->plot();
			
		glPopMatrix();
	}
}
void thirdzonepic(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		glPushMatrix();
			//target circle
			plotzone(60, 150);
			//curling moving
			if(cl[0]==NULL) cl[0]=new Curling2(310.0, 150.0);
			if(cl[1]==NULL) cl[1]=new Curling2(310.0, 150.0);
			if(cl[2]==NULL) cl[2]=new Curling2(310.0, 150.0);
			
			double _t=t;
			double t4=4.0;

			glColor3f(153.0/255.0, 230.0/255.0, 77.0/255.0);
			cl[0]->plot();
			
			glColor3f(130.0/255.0, 90.0/255.0, 255.0/255.0);
			cl[1]->plot();
			
			if(t<t4){
				cl[2]->move(-236.0*(1.385*dt/t4)*cos(_t/(t4+0.3)*M_PI/2.0), 7.0*(1.385*dt/t4)*cos(_t/(t4+0.3)*M_PI/2.0), 360.0*(1.2*dt/(t4+0.3))*cos(_t/t4*M_PI/2.0));
			}
			glColor3f(153.0/255.0, 230.0/255.0, 77.0/255.0);
			cl[2]->plot();
		glPopMatrix();
	}
}

void thirdpic(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ticba==NULL) ticba=new Person(150,200);
		if(wd==NULL) wd=new Wand(-90.0);
		if(ticba->left_forearm->component==NULL) ticba->take(ticba->left_forearm, wd);
		if(t<1) ticba->bend(45*dt);
		ticba->plot();
	}
}
void lsquatpic(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(leo==NULL) leo=new Person(150,200);
		if(wd2==NULL) wd2=new Wand(-90.0);
		if(curling_2==NULL) curling_2=new Curling(0.0);
		if(leo->left_forearm->component==NULL) leo->take(leo->left_forearm, wd2);
		if(leo->right_forearm->component==NULL) leo->take(leo->right_forearm, curling_2);
		leo->squat(t, 1.0, dt);
		if(t<2.0&&t>=1.0) leo->left_forearm->component->shift+=15.0*dt;
		background();
		leo->plot();
	}
}
void tsquatpic(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ticba==NULL) ticba=new Person(150,200);
		if(wd==NULL) wd=new Wand(-90.0);
		if(curling==NULL) curling=new Curling(0.0);
		if(ticba->left_forearm->component==NULL) ticba->take(ticba->left_forearm, wd);
		if(ticba->right_forearm->component==NULL) ticba->take(ticba->right_forearm, curling);
		ticba->squat(t, 1.0, dt);
		if(t<2.0&&t>=1.0){
			ticba->left_forearm->component->shift+=15.0*dt;
		}
		background();
		ticba->plot();
	}
}

void tnod(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ticba==NULL) ticba=new Person(150,200);
		if(wd==NULL) wd=new Wand(-90.0);
		if(curling==NULL) curling=new Curling(0.0);
		if(ticba->canreset){
			ticba->setside(150, 200.0);
			wd->reset();
		}
		//if(ticba->left_forearm->component==NULL) ticba->take(ticba->left_forearm, wd);
		if(t<1.25);
		else if(t<1.75){
			ticba->head->r+=20*dt;
		}
		else if(t<2.25){
			ticba->head->r+=-20*dt;
		}
		else if(t<2.75){
			ticba->head->r+=20*dt;
		}
		else if(t<3.25){
			ticba->head->r+=-20*dt;
		}
		glTranslatef(ticba->x, ticba->y, 0);
		glScalef(2.0, 2.0, 1.0);
		glTranslatef(-ticba->x, -ticba->y, 0);
			background();
			ticba->plot();
		glTranslatef(ticba->x, ticba->y, 0);
		glScalef(0.5, 0.5, 1.0);
		glTranslatef(-ticba->x, -ticba->y, 0);
	}
}
void tsad(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ticba==NULL) ticba=new Person(150,200);
		if(wd==NULL) wd=new Wand(-90.0);
		if(curling==NULL) curling=new Curling(0.0);
		if(ticba->canreset){
			ticba->setside(150, 200.0);
			wd->reset();
		}
		//if(ticba->left_forearm->component==NULL) ticba->take(ticba->left_forearm, wd);
		if(t<1.25);
		else if(t<1.75){
			ticba->head->r+=20*dt;
			ticba->oLimbsrotate(ticba->right_arm, -90*2*dt);
			ticba->Limbsrotate(ticba->right_forearm, -100*2*dt);
			ticba->oLimbsrotate(ticba->left_arm, -90*2*dt);
			ticba->Limbsrotate(ticba->left_forearm, -100*2*dt);
		}
		else if(t<2.25){
			ticba->Limbsrotate(ticba->left_thigh, -60*2*dt);
			ticba->Limbsrotate(ticba->right_thigh, -60*2*dt);
			ticba->Limbsrotate(ticba->left_calf, 90*2*dt);
			ticba->Limbsrotate(ticba->right_calf, 90*2*dt);
			ticba->x+=-30*dt;
			ticba->y+=-30*dt;
		}
		else if(t<3.25){
			ticba->bend(50*dt);
		}
		glTranslatef(ticba->x-60, ticba->y, 0);
		glScalef(2.0, 2.0, 1.0);
		glTranslatef(-ticba->x+60, -ticba->y, 0);
			background();
			ticba->plot();
		glTranslatef(ticba->x-60, ticba->y, 0);
		glScalef(0.5, 0.5, 1.0);
		glTranslatef(-ticba->x+60, -ticba->y, 0);
	}
}

void tcheer(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(ticba==NULL) ticba=new Person(150,200);
		if(wd==NULL) wd=new Wand(-90.0);
		if(curling==NULL) curling=new Curling(0.0);
		if(ticba->canreset){
			ticba->setfront(ticba->x, 200.0);
			wd->reset();
		}
		if(ticba->left_forearm->component==NULL) ticba->take(ticba->left_forearm, wd);
		if(t<1.25);
		else if(t<1.75){
			ticba->oLimbsrotate(ticba->right_arm, 50*2*dt);
			ticba->Limbsrotate(ticba->right_forearm, 130*2*dt);
		}
		else if(t<2){
			ticba->Limbsrotate(ticba->right_arm, -50*dt);
		}
		glTranslatef(ticba->x, ticba->y, 0);
		glScalef(2.0, 2.0, 1.0);
		glTranslatef(-ticba->x, -ticba->y, 0);
			background();
			ticba->plot();
		glTranslatef(ticba->x, ticba->y, 0);
		glScalef(0.5, 0.5, 1.0);
		glTranslatef(-ticba->x, -ticba->y, 0);
	}
}

void lcheer(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){
		t-=t_start;
		double dt=t-t_pre;
		t_pre=t;
		if(leo==NULL) leo=new Person(150,200);
		if(wd2==NULL) wd2=new Wand(-90.0);
		if(curling_2==NULL) curling_2=new Curling(0.0);
		if(leo->canreset){
			leo->setfront(150, 200.0);
			wd2->reset();
		}
		if(leo->left_forearm->component==NULL) leo->take(leo->left_forearm, wd2);
		if(t<1.25);
		else if(t<1.75){
			leo->oLimbsrotate(leo->right_arm, 50*2*dt);
			leo->Limbsrotate(leo->right_forearm, 130*2*dt);
		}
		else if(t<2){
			leo->Limbsrotate(leo->right_arm, -50*dt);
		}
		glTranslatef(leo->x, leo->y, 0);
		glScalef(2.0, 2.0, 1.0);
		glTranslatef(-leo->x, -leo->y, 0);
			background();
			leo->plot();
		glTranslatef(leo->x, leo->y, 0);
		glScalef(0.5, 0.5, 1.0);
		glTranslatef(-leo->x, -leo->y, 0);
	}
}



void tctr(double *t_start, double *t_end, double t_add){
	*t_start=*t_end;
	*t_end+=t_add;
}

void start(){
	glColor3f(0.0,0.0,0.0);
	glRectf(0.0, 0.0,worldx,worldy/2);
	glEnd();
	plotStar(150.0,200.0,0.0,15.0,0.0,0.0,0.0);
	plotStar(180.0,210.0,0.0,-15.0,0.0,1.0,0.0);
	plotStar(120.0,210.0,0.0,-15.0,1.0,1.0,0.0);
	plotStar(200.0,250.0,0.0,-15.0,1.0,0.0,0.0);
	plotStar(100.0,250.0,0.0,-15.0,0.0,0.0,1.0);
	plotU();
	glColor3f(0.0,1.0,1.0);
	
	glPushMatrix();
	glTranslatef(150.0,100.0,0.0);
	glRotatef(45,0,0,1);
		
	glTranslatef(7.5,-30.0,0.0);
	glRotatef(-30,0,0,1);
	glColor3f(0.0,1.0,1.0);
	glRectf(-20.0,0.0,-15.0,-20.0);
	glRectf(0.0,0.0,-20.0,-10.0);
	glRotatef(30,0,0,1);
	glTranslatef(-7.5,30.0,0.0);
	
	glColor3f(1.0,1.0,0.0);
	glRectf(0.0,0.0,15.0,-40.0);
	glColor3f(0.0,1.0,1.0);
	glRectf(5.0,-15.0,-20.0,-20.0);
	glTranslatef(7.5, 7.5, 0.0);
	plotCircle(20.0, 20.0);
	glTranslatef(-7.5, -7.5, 0.0);
		
	glTranslatef(-20.0,-20.0,0.0);
	glRotatef(-45,0,0,1);
	glColor3f(1.0,0.0,0.0);
	glRectf(-5.0,0.0,5.0,-5.0);
	glRotatef(45,0,0,1);
	glTranslatef(20.0,20.0,0.0);
	glColor3f(0.0,1.0,1.0);
		
	glTranslatef(7.5,-30.0,0.0);
	glRectf(-5.0,0.0,5.0,-20.0);
		
	glTranslatef(0.0,-20.0,0.0);
	glRotatef(35,0,0,1);
	glRectf(-2.5,0.0,2.5,-20.0);
	glPopMatrix();
	char str1[13]={'C',' ','U',' ','R',' ','L',' ','I',' ','N',' ','G'};
   	int len = (int) strlen(str1);
    for (int i = 0; i < len; i++)
    {
        glColor3f(0.1*i,1-0.1*i,0.2*i);
        glRasterPos2f(95+10*i,172);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str1[i]);
	}
	glEnd();
}

void end(){
	glColor3f(0.0, 0.0, 0.0);
	glRectf(0.0, worldy, worldx, 0.0);
	char str1[16]={'B','L','A','C','K',' ','T','E','A','M',' ','W','I', 'N'};
   	int len = (int) strlen(str1);
    for (int i = 0; i < len; i++)
    {
        glColor3f(0.1*i,1-0.1*i,0.2*i);
        glRasterPos2f(95+10*i,172);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,str1[i]);
	}
}

void display(void)
{
	double scale;
	scale=1.0;
	double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0*scale;
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();

		double t_start, t_end;
		t_start=t_end=5.0;
		if(t<t_start)
		start();
			tctr(&t_start, &t_end, 8.25);
		twalkpic(t, t_start, t_end);
			tctr(&t_start, &t_end, 11.0);
		tsquatpic(t, t_start, t_end);
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;
			tctr(&t_start, &t_end, 6.0);
		firstzonepic(t, t_start, t_end);
			if(fabs(t-t_end)<0.05)ticba->canreset=true;
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;
			tctr(&t_start, &t_end, 4.0);
		tnod(t, t_start, t_end);
			if(fabs(t-t_end)<0.05)ticba->canreset=true;
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;

			tctr(&t_start, &t_end, 8.25);
		lwalkpic(t, t_start, t_end);
			tctr(&t_start, &t_end, 11.0);
		lsquatpic(t, t_start, t_end);
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;
			tctr(&t_start, &t_end, 8.0);
		secondzonepic(t, t_start, t_end);
			if(fabs(t-t_end)<0.05)leo->canreset=true;
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;
			tctr(&t_start, &t_end, 4.0);
		lcheer(t, t_start, t_end);

			tctr(&t_start, &t_end, 8.25);
		twalkpic(t, t_start, t_end);
			tctr(&t_start, &t_end, 11.0);
		tsquatpic(t, t_start, t_end);
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;
			tctr(&t_start, &t_end, 5.0);
		thirdzonepic(t, t_start, t_end);
			if(fabs(t-t_end)<0.05)ticba->canreset=true;
			if((t-t_end)>=0.0&&(t-t_end)<0.035*scale) t_pre=0.0;
			tctr(&t_start, &t_end, 4.0);
		tsad(t, t_start, t_end);
		if(t>t_end){
			end();
		}
//		secondpic(t,11.5,12.0);
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
