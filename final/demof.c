#include "stdio.h"
#include <iostream>
#include <GL/freeglut.h>
#include <unistd.h>
#include <stdlib.h>
#include "string.h"
#include "math.h"
#define _USE_MATH_DEFINES
#define worldx 300.0
#define worldy 300.0
using namespace std;
void plotString(float a,float b);
void plotCircle(float a, float b);
void plot_Half_Circle(float a,float b,float w);
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
		height=16.0;
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
		width=18.0;
		height=18.0;
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
		if(limbs->component!=NULL) Limbshift(limbs->component, -dy);
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
		head->width+=-5.0*dt/t;
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
	BowString(){;}
	BowString(double r): Component(r){
		parent_x=0.0;
		parent_y=0.0;
		shift=10.0;
		width=35.0;
		height=50.0;
	}
	void plot(){
		//puts("Plot String");
		glTranslatef(parent_x, parent_y, 0.0);
		glRotatef(r,0,0,1);
			glColor3f(1.0, 97.0/255.0, 0.0);
			//plot_Half_Circle(height,width,width);
			plotString(height,width);
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




float sx1=0.0;
float sy1=0.0;
double t_pre=0.0;
float centx,centy,topx,topy;
Person* zmin=NULL;
Person* leo=NULL;
Bow* bw=NULL;
Bow* bw2=NULL;
BowString *bs=NULL;

void init(void){
	glClearColor(1.0,1.0,1.0,0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, worldx,0.0, worldy);
}
void plotString(float a,float b)
{
	glBegin(GL_LINE_STRIP);
	glVertex2i(a,b*cos(75.0/360.0*2.0*M_PI));
	glVertex2i(sx1,sy1);
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
			leo=new Person(100,100);
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
		/*if(curling_2==NULL) {
			curling_2=new Curling(0.0);
			curling_2->R=130.0/255.0;
			curling_2->G=90.0/255.0;
			curling_2->B=255.0/255.0;
		}*/
		if(leo->left_forearm->component==NULL) leo->take(leo->left_forearm, bw2);
		//if(leo->right_forearm->component==NULL) leo->take(leo->right_forearm, curling_2);
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
//		background();
		leo->plot();
	}
}
void firstpic(double t,double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		double dt=t-t_pre;
		t-=t_start;
		if(zmin==NULL) {zmin=new Person(150,200);zmin->body->R=0.0;zmin->body->G=0.0;zmin->body->B=0.8;}
		if(bw==NULL) 
				bw=new Bow(-90.0);
		if(zmin->left_forearm->component==NULL) {
				zmin->take(zmin->left_forearm, bw);
		//		zmin->take(zmin->right_forearm,bw->component);
		}
		if(t<1)
		{
			zmin->headToside(1.0,dt);
			zmin->plot();
			t_pre=t;
		}
		else if(t<6){
			zmin->oLimbsrotate(zmin->left_arm, dt*18);
			zmin->oLimbsrotate(zmin->right_arm, dt*18);
			zmin->plot();
			t_pre=t;
		}
		else if(t<10)
		{
			zmin->Limbshift(zmin->right_forearm,dt*1);
			zmin->plot();
			t_pre=t;
		}
		else if(t<10)
		{
			zmin->plot();
			t_pre=t;
		}

	}

}

void secondpic(double t, double t_start, double t_end){
	if(t>=t_start&&t<t_end){	
		//t=t-t_start;
		glPushMatrix();
		lwalkpic(t,t_start,t_end);
		glPopMatrix();
	}
}


void display(void)
{
	double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
		firstpic(t,0.0, 10.0);
		secondpic(t,10.0,14.0);
//		squatpic(t,14.0,19.0);

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
