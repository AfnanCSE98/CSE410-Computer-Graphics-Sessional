#include<stdio.h>
#include<stdlib.h>
#include<math.h>

//#include <windows.h>
#include "glut.h"

#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

double radius ;
double rotateAngle;
double distance  ;
int segments;

struct point
{
	double x,y,z;
	point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
	point(const point &p){
        this->x = p.x ;
        this->y = p.y ;
        this->z = p.z ;
    }

	point() {}
};

point pos , L , U , R;
point corner[4];

point *p;
point *d;

point rotateVec(point p , point ref , bool clockwise , int deg){
	// cross product
	point temp;
	temp.x = p.y * ref.z - p.z * ref.y;
    temp.y = p.z * ref.x - p.x * ref.z;
    temp.z = p.x * ref.y - p.y * ref.x;

	double ang;
	if(clockwise){
		ang = (pi * deg) / 180;
	}
	else{
		ang = - (pi * deg) / 180;
	}

	point new_;
	new_.x = p.x * cos(ang) + temp.x * sin(ang);
    new_.y = p.y * cos(ang) + temp.y * sin(ang);
    new_.z = p.z * cos(ang) + temp.z * sin(ang);

	return new_;
}

void drawAxes()
{
	glPushMatrix();

	if(drawaxes==1)
	{
		glColor3f(0.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			 glColor3f(1.0, 0, 0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);
			
			glColor3f(0, 1.0, 0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);
			
			glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
		glPopMatrix();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw polys using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'w':
			pos.x -= 2*cos(rotateAngle*acos(-1.0)/180.0) ;
            pos.y -= 2*sin(rotateAngle*acos(-1.0)/180.0) ;
            distance -= 2 ;
			break;

		case 's':
			pos.x += 2*cos(rotateAngle*acos(-1.0)/180.0) ;
            pos.y += 2*sin(rotateAngle*acos(-1.0)/180.0) ;
            distance += 2 ;
			break;

		case 'a':
			rotateAngle += 4 ;
			break;

		case 'd':
			rotateAngle -= 4 ;
			break;

		default:
			break;
	}
}

point add(point p1 , point p2){
	
	point tmp;
	
	tmp.x = p1.x + p2.x;
	tmp.y = p1.y + p2.y;
	tmp.z = p1.z + p2.z;

	return tmp;
}


point sub(point p1 , point p2){
	
	point tmp;
	
	tmp.x = p1.x - p2.x;
	tmp.y = p1.y - p2.y;
	tmp.z = p1.z - p2.z;

	return tmp;
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			cameraHeight -= 3.0;
			break;
		case GLUT_KEY_UP:		// up arrow key
			cameraHeight += 3.0;
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			 cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			pos = add(pos , U);
			break;
		case GLUT_KEY_PAGE_DOWN:
			pos = sub(pos , U);
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
		
			break;
		case GLUT_KEY_END:
			
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
		
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}


void generate_points(){
	double h = radius / 2 ;
    for(int i=0;i<=segments;i++){
        angle = ((1.0*i)/(1.0*segments))*acos(-1.0)*2 ;
        if(i%8==0){
            d[i/8].x = radius*cos(angle) ;
            d[i/8].y = radius*sin(angle) ;
            d[i/8].z = h/4 ;
        }
		
        p[i].x = radius*cos(angle) ;
        p[i].y = radius*sin(angle) ;
        p[i].z = h/2 ;
    }

}

void draw_wheel(){
	glPushMatrix();

	for(int i=0; i <=segments ; i++ ){
		glColor3f(0,128,255);

		glBegin(GL_QUADS) ;
        glVertex3f(p[i].x,p[i].y,p[i].z) ;
        glVertex3f(p[i+1].x,p[i+1].y,p[i+1].z) ;
        glVertex3f(p[i+1].x,p[i+1].y,-p[i+1].z) ;
        glVertex3f(p[i].x,p[i].y,-p[i].z) ;
        
        glEnd() ;

	}

	glPopMatrix();
}

void draw_rect(){
	glPushMatrix();

	double shade;
	for(int i=0;i<2;i++){
        if(i==0){
			glColor3f(204,204,0);
		}
		else{
			glColor3f(255,204,255);
		}
        
        
        glBegin(GL_QUADS) ;
        glVertex3f(d[i].x,d[i].y,d[i].z) ;
        glVertex3f(d[i+2].x,d[i+2].y,d[i+2].z) ;
        glVertex3f(d[i+2].x,d[i+2].y,-d[i+2].z) ;
        glVertex3f(d[i].x,d[i].y,-d[i].z) ;
        glEnd();
    }
    glPopMatrix() ;
}

void Wheel(){
	glPushMatrix();

	glTranslatef(0,0,radius);//put above xy plane.without this translation,wheel will be halfside along xy plane
    glTranslatef(pos.x,pos.y,pos.z) ;//enable movement with key w/s pressed
    glRotatef(rotateAngle,0,0,1) ;//rotate around z axis when key  a/d pressed
    glRotatef(distance*(360.0/(2*3.1416*radius)),0,1,0) ;//enable rotation while moving upon key w/s presed
    glRotatef(90, 1,0,0) ;//put wheel vertical to xy plane,without it wheel will be parallel to xy plane

	generate_points();

	draw_wheel();

	draw_rect();

	glPopMatrix();
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	gluLookAt(100*cos(cameraAngle), 100*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(pos.x, pos.y, pos.z, pos.x + L.x, pos.y + L.y, pos.z + L.z, U.x, U.y, U.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    Wheel();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	//angle+=0.1;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=100.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//related to wheel
	radius = 20;
	segments = 31;
	rotateAngle = 0;
	distance = 0;
	
	pos = point(0,0,0) ;

	p = new point[segments+1];
	d = new point[4];

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
