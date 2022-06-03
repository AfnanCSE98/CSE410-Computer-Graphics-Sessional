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

int rotate_deg = 5;

struct point
{
	double x,y,z;
	point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
	point() {}
};

point pos , L , U , R;
point corner[4];

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
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

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

int x = 100 ;
int y = 80 ;


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


void drawSphere_part(double radius,int slices,int stacks)
{
	struct point points[stacks+1][slices+1];
	int i,j;
	double h,r;
	//generate points
	//stacks : # of polygons
	//slices : # of sides of each polygon
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*0.5*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*0.5*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f(1, 0, 0);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                
			}glEnd();
		}
	}
	glPopMatrix();
}

void drawCylinder_part(double radius, double height, int segments){
	glPushMatrix();
	point p[segments+1];
	double angle;

	for(int i=0 ; i<=segments ; i++){
		angle = ((double)(i) / (double)(segments)) * pi * 0.5;
		 p[i].x = radius * cos(angle);
        p[i].y = radius * sin(angle);
        p[i].z = height;
	}

	for(int i=0 ; i<segments ; i++){
		
		glBegin(GL_QUADS);
        glVertex3f(p[i].x, p[i].y, p[i].z);
        glVertex3f(p[i + 1].x, p[i + 1].y, p[i + 1].z);
        glVertex3f(p[i + 1].x, p[i + 1].y, -p[i + 1].z);
        glVertex3f(p[i].x, p[i].y, -p[i].z);

        glEnd();
	}

	 glPopMatrix();
}

void drawCube_part(int s){
	int a = s / 2;
    glBegin(GL_QUADS);
    glVertex3f(a, a, 0);
    glVertex3f(a, -a, 0);
    glVertex3f(-a, -a, 0);
    glVertex3f(-a, a, 0);
    glEnd();
}

void CubeSphere(double side, double radius, int segments)
{
    glPushMatrix();
    double a = (side / 2) - radius ;

    glPushMatrix() ;
    
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(a * corner[i].x, a * corner[i].y, a * corner[i].z);
        glRotatef(90 * i, 0, 0, 1);
        drawSphere_part(radius, segments, segments);
        glPopMatrix();
    }
    
    glPopMatrix();
    
    glPushMatrix() ;
    
    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotated(180,1, 0, 0) ;
        glTranslatef(a * corner[i].x, a * corner[i].y, a * corner[i].z);
        glRotatef(90 * i, 0, 0, 1);
        drawSphere_part(radius, segments, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;

    glPushMatrix() ;
    
    // following 3 loops draws 4*3=12 cylinders
    for (int q = 0; q < 4; q++)
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        glTranslatef(a * corner[q].x, a * corner[q].y, 0);
        glRotatef(90 * q, 0, 0, 1);
        drawCylinder_part(radius, a, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;
   
    glPushMatrix() ;
    
    for (int q = 0; q < 4; q++)
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        glRotatef(90, 1, 0, 0);
        glTranslatef(a * corner[q].x, a * corner[q].y, 0);
        glRotatef(90 * q, 0, 0, 1);
        drawCylinder_part(radius, a, segments);
        glPopMatrix();
    }
    
    glPopMatrix();
   
    glPushMatrix() ;
    
    for (int q = 0; q < 4; q++)
    {
        glPushMatrix();
        glColor3f(0, 1, 0);
        
        glRotatef(90, 0, 1, 0);
        glTranslatef(a * corner[q].x, a * corner[q].y, 0);
        glRotatef(90 * q, 0, 0, 1);
        drawCylinder_part(radius, a, segments);
        glPopMatrix();
    }
    
    glPopMatrix() ;
    
   
    glPushMatrix();
    glColor3f(1, 1, 1);
    double s = side - (2*radius) ;
    
    glPushMatrix() ;
  
    //upper side
    glTranslatef(0, 0, side/2);
    drawCube_part(s);
    glPopMatrix();

	glPushMatrix() ;
    //lower side
    glTranslatef(0, 0, -side/2);
    drawCube_part(s);
    glPopMatrix();



    for (int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glRotatef(90 * i, 0, 0, 1);
        glTranslatef(side/2, 0, 0);
        glRotatef(90, 0, 1, 0);
        drawCube_part(s);
        glPopMatrix();
    }
    

    glPopMatrix();

    glPopMatrix();
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

		case '1':
		// look left
			L = rotateVec(L , U , false , rotate_deg);
			R = rotateVec(R , U , false , rotate_deg);
			break;

		case '2':
		// look right
			R = rotateVec(R , U , true , rotate_deg);
			L = rotateVec(L , U , true , rotate_deg);
			break;

		case '3':
		// look up
			U = rotateVec(U , R , false , rotate_deg);
			L = rotateVec(L , R , false , rotate_deg);
			break;

		case '4':
		// look down
			U = rotateVec(U , R , true , rotate_deg);
			L = rotateVec(L , R , true , rotate_deg);
			break;

		case '5':
		// tilt clockwise
			R = rotateVec(R, L, false , rotate_deg);
    		U = rotateVec(U, L, false , rotate_deg);
			break;

		case '6':
		// tilt anticlockwise
			R = rotateVec(R, L, true , rotate_deg);
    		U = rotateVec(U, L, true , rotate_deg);
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
			pos = sub(pos , L);
			break;
		case GLUT_KEY_UP:		// up arrow key
			pos = add(pos , L);
			break;

		case GLUT_KEY_RIGHT:
			pos = add(pos , R);
			break;
		case GLUT_KEY_LEFT:
			pos = sub(pos , R);
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
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
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

void timer(int sec){
	glutPostRedisplay() ;
    glutTimerFunc(1000/60,timer,0) ;
	x = (x+50) % 60;
	y = (y-20) % 50;
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
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(pos.x, pos.y, pos.z, pos.x + L.x, pos.y + L.y, pos.z + L.z, U.x, U.y, U.z);

	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	
	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    CubeSphere(50 , 10 , 20);

    //drawSS();
	//poly()



	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.1;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
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

	corner[0] = point(1, 1, 1);
    corner[1] = point(-1, 1, 1);
    corner[2] = point(-1, -1, 1);
    corner[3] = point(1, -1, 1);

	// initialize U,L,R
	pos = point(70,100, 0);
	U = point(0 , 0 , 1);
	R = point(sqrt(2) , sqrt(2) , 0);
	L = point(-sqrt(2) , sqrt(2) , 0);

	//give PERSPECTIVE parameters
	gluPerspective(80,	0.8,	1,	1000.0);
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

	 glutTimerFunc(0,timer,0) ;

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
