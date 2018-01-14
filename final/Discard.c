#include <GL/freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>


void rotate2D(double *x_o, double *y_o, double rx,double ry,double theta);
void output(double x, double y,char *string);
static void resize(int width, int height);
void draw_solid_circle(float x, float y, float radius);
void *font=GLUT_BITMAP_TIMES_ROMAN_24;
void human(double fx,double fy,double tx,double ty);
void rectangle(double x,double y,double length,double width,double r,double g,double b,double angle);

static void 
display(void)
{
	
    const double t = glutGet(GLUT_ELAPSED_TIME) / 10000.0;
    const double b = t*360.0;

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(1,0.7,0.7);
//kanban
/*
	glBegin(GL_POLYGON);
		glVertex2f(0.6,0.4);
		glVertex2f(0.6,0.9);
		glVertex2f(-0.6,0.9);
		glVertex2f(-0.6,0.4);	
	glEnd();
*/
	rectangle(0,0.65,1.2,0.5,1,0.7,0.7,0);
//grass
	glColor3d(0.3,1,0.3);
	glBegin(GL_POLYGON);
		glVertex2f(0.4,0.4);
		glVertex2f(0.6,-0.6);

		glVertex2f(-0.6,-0.6);
		glVertex2f(-0.4,0.4);	
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2f(1,-0.6);
		glVertex2f(1,-1);
		glVertex2f(-1,-1);
		glVertex2f(-1,-0.6);
	glEnd();

	printf("t=%d\n",t);
	if(t>5)
	{	
/*		glBegin(GL_POLYGON);
			glVertex2f(1,-0.6);
			glVertex2f(1,-1);
			glVertex2f(-1,-1);
			glVertex2f(-1,-0.6);
		glEnd();*/
	}
/*	double x1,x2,x3,x4,y1,y2,y3,y4;

	
	
//	glTranslatef(0,0.5,0);
    glPushMatrix();
		glTranslatef(0,-0.25,0);
        glRotated(b,0,0,-1);
		x1=0.12;
		y1=0;
		
		x2=0.12;
		y2=0.75;
		
		x3=-0.12;
		y3=0.75;

		x4=-0.12;
		y4=0.0;
		for(int i=0;i<6;i++)
		{
			double theta=60;
			rotate2D(&x1, &y1, 0, 0,theta);
			rotate2D(&x2, &y2, 0, 0,theta);
			rotate2D(&x3, &y3, 0, 0,theta);
			rotate2D(&x4, &y4, 0, 0,theta);
		//	double time=glutGet(GLUT_ELAPSED_TIME);
			double time=glutGet(GLUT_ELAPSED_TIME)+(i+5)*10000.0*3.0;
			//printf("b=%d\n",b);
			glColor3d(fabs(cos(0.5*time/10000.0*360.0/60.0)),fabs(cos(0.5*time/10000.0*360.0/60.0)),1);
			//glColor3d(fabs(cos(time/10000.0*360.0/50.0)),fabs(cos(time/10000.0*360.0/50.0)),1);
			glBegin(GL_POLYGON);
			glVertex2f(x1,y1);
			glVertex2f(x2,y2);
			glVertex2f(x3,y3);
			glVertex2f(x4,y4);
			
			glEnd();

		}
		

		glColor3d(0,1.5,0);
		draw_solid_circle(0,0,0.6);
		glColor3d(0,0,0);
		draw_solid_circle(0,0,0.45);

	glPopMatrix();



	glPushMatrix();
		glTranslatef(0.6*sin(glutGet(GLUT_ELAPSED_TIME)/1000000.0*360.0),0,0);	
		//glColor3d(1,1,1);
		glColor3d(1,fabs(cos(0.5*glutGet(GLUT_ELAPSED_TIME)/10000.0*360.0/60.0)),fabs(cos(0.5*glutGet(GLUT_ELAPSED_TIME)/10000.0*360.0/60.0)));
		output(-0.3,0.75,"ROTATING BOX!");
	glPopMatrix();
	//glColor3d(1,1,1);
//	output(1,0.5,"ROTATING BOX!");
//	glTranslatef(0,0.5,0);
*/
	glutSwapBuffers();
}
void rectangle(double x,double y,double length,double width,double r,double g,double b,double angle)
{

	double hl=length/2.0;
	double hw=width/2.0;
	double x1=hl;
	double y1=-hw;
	double x2=hl;
	double y2=hw;
	double x3=-hl;
	double y3=hw;
	double x4=-hl;
	double y4=-hw;
/*	
	rotate2D(&x1, &y1, 0, 0,angle);
	rotate2D(&x2, &y2, 0, 0,angle);
	rotate2D(&x3, &y3, 0, 0,angle);
	rotate2D(&x4, &y4, 0, 0,angle);
*/
	glPushMatrix();
	glColor3d(r,g,b);
	glTranslatef(x,y,0);
	glBegin(GL_POLYGON);
		glVertex2f(x1,y1);
		glVertex2f(x2,y2);
		glVertex2f(x3,y3);
		glVertex2f(x4,y4);
	glEnd();
	//glTranslatef(x,y,0);
	glPopMatrix();
}
static void 
idle(void)
{
    glutPostRedisplay();
}

int 
main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(600,600);
    glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);

    glutCreateWindow("Moving Square");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    glutMainLoop();
}

static void 
resize(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}
void output(double x, double y,char *string)
{
	int len,i;
	glRasterPos2f(x,y);
	len=strlen(string);
	for(i=0;i<len;i++)
	{
		glutBitmapCharacter(font,string[i]);
	}
}

void draw_solid_circle(float x, float y, float radius)
{
    int count;
    int sections=200;
                      
    GLfloat TWOPI=2.0f * 3.14159f;

    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);

        for(count=0; count<=sections; count++)
        {
            glVertex2f(x+radius*cos(count*TWOPI/sections), y+radius*sin(count*TWOPI/sections));
        }
    glEnd();
}
void rotate2D(double *x_o, double *y_o, double rx,double ry,double theta)
{
	double x=*x_o;
	double y=*y_o;
	theta=theta/180*3.14;
	*x_o=rx+(x-rx)*cos(theta)-(y-ry)*sin(theta);
	*y_o=ry+(x-rx)*sin(theta)+(y-ry)*cos(theta);
}
