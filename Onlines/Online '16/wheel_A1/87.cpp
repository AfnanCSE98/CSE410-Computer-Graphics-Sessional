#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <iostream>

#include <GL/glut.h>
#include<vector>

using namespace std ;


#define pi (2*acos(0.0))

#define GRID_COUNT 12
#define GRID_DIFF 15

#define WHEEL_RADIUS 30
#define WHEEL_WEIGHT 15

#define WHEEL_SLICES 15
#define WHEEL_STACKS 10

#define INCLINE_ANGLE 45

double cameraHeight;
double cameraAngle;

int drawgrid;
int drawaxes;

double angle;

class point {

	public:

    double x,y,z;

	point() {
        x = y = z = 0.0;
    }

    point(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double getX() {
        return x;
    }

    double getY() {
        return y;
    }

    double getZ() {
        return z;
    }

	void setX(double x) {
        this->x = x;
    }

    void setY(double y) {
        this->y = y;
    }

    void setZ(double z) {
        this->z = z;
    }

    point operator+(point vector_point);
    point operator-(point vector_point);
    point operator*(double scalar);
    point operator&(point vector_point);

};

point point::operator+(point vector_point) {
    return point(x+vector_point.x, y+vector_point.y, z+vector_point.z);
}

point point::operator-(point vector_point) {
    return point(x-vector_point.x, y-vector_point.y, z-vector_point.z);
}

point point::operator*(double scalar) {
    return point(x*scalar, y*scalar, z*scalar);
}

point point::operator&(point vector_point) {
    return point(y*vector_point.z-z*vector_point.y, z*vector_point.x-x*vector_point.z, x*vector_point.y-y*vector_point.x);
}
class wheel
{
	public:
	point pos;
	point dir;

	double wheel_radius ;
	double wheel_weight ;

	double left_right_theta;
	double up_down_theta;

	double per_change_distance ;
	double per_change_angle_left_right ;
	double per_change_angle_up_down ;

	wheel() {
    }

	wheel(point pos,point dir) {
        this->pos = pos;
        this->dir = dir;

		this->wheel_radius = WHEEL_RADIUS;
		this->wheel_weight = WHEEL_WEIGHT;

		this->per_change_angle_left_right = 2;
		this->per_change_angle_up_down = 2;
		this->per_change_distance = (per_change_angle_up_down*2*pi*wheel_radius)/360;

		this->left_right_theta = 0;
		this->up_down_theta= 0;

    }	

	void moveForward();
	void moveBackward();

	void rotateLeft();
	void rotateRight();

};

void wheel::moveForward() {
  	up_down_theta += per_change_angle_up_down;
	pos.x -= per_change_distance*cos((left_right_theta * pi) / 180);
	pos.y -= per_change_distance*sin((left_right_theta * pi) / 180);
}

void wheel::moveBackward() {
  	up_down_theta -= per_change_angle_up_down;
	pos.x += per_change_distance*cos(left_right_theta * pi / 180);
	pos.y += per_change_distance*sin(left_right_theta * pi / 180);
}

void wheel::rotateLeft() {
  	left_right_theta += per_change_angle_left_right;
}

void wheel::rotateRight() {
  	left_right_theta -= per_change_angle_left_right;
}

point pos ;
point dir ;
wheel w;

void drawAxes()
{
	if(drawaxes==1)
	{
		//glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glColor3f(1.0, 1.0, 0.0); //Yellow - x
			glVertex3f( GRID_DIFF*GRID_COUNT,0,0);
			glVertex3f(-GRID_DIFF*GRID_COUNT,0,0);

			glColor3f(1.0, 0.0, 1.0); //Violet - y
			glVertex3f(0,-GRID_DIFF*GRID_COUNT,0);
			glVertex3f(0, GRID_DIFF*GRID_COUNT,0);

			glColor3f(0.0, 1.0, 1.0); // Sky - z
			glVertex3f(0,0, GRID_DIFF*GRID_COUNT);
			glVertex3f(0,0,-GRID_DIFF*GRID_COUNT);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-GRID_COUNT;i<=GRID_COUNT;i++){

				//lines parallel to Y-axis
				glVertex3f(i*GRID_DIFF, -GRID_DIFF*GRID_COUNT, 0);
				glVertex3f(i*GRID_DIFF,  GRID_DIFF*GRID_COUNT, 0);

				//lines parallel to X-axis
				glVertex3f(-GRID_DIFF*GRID_COUNT, i*GRID_DIFF, 0);
				glVertex3f( GRID_DIFF*GRID_COUNT, i*GRID_DIFF, 0);
			}
		}glEnd();
	}
}

void drawRectangle(double height,double width) 
{
	glBegin(GL_QUADS);
	{
		glVertex3f(width / 2, height / 2, 0);
		glVertex3f(width / 2, -height / 2, 0);
		glVertex3f(-width / 2, -height / 2, 0);
		glVertex3f(-width / 2, height / 2, 0);
	}
	glEnd();
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,0);
		glVertex3f( a,-a,0);
		glVertex3f(-a,-a,0);
		glVertex3f(-a, a,0);
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
    //draw triangles using generated points
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


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		//glColor3f(i/stacks,i/stacks,i/stacks);
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glColor3f((double)j/(double)stacks,(double)j/(double)stacks,(double)j/(double)stacks);
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			drawgrid=1-drawgrid;
			break;

		case 'w':
			w.moveForward();
			break;

		case 's':
			w.moveBackward();
			break;

		case 'a':
			w.rotateLeft();
			break;
		case 'd':
			w.rotateRight();

		default:
			break;
	}
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
			break;
		case GLUT_KEY_PAGE_DOWN:
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

void drawCylinder(double hight , double width ,int slices,int stacks)
{
	//glColor3f(0,1,0); // Green
	//glTranslatef(width,width,0);

	point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h= width*sin(((double)i/(double)stacks)*(pi/2));
		r= hight;

		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*(2*pi));
			points[i][j].z=h;
			points[i][j].y=r*sin(((double)j/(double)slices)*(2*pi));
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)stacks-i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			//glColor3f((double)j/(double)slices,(double)j/(double)slices,(double)j/(double)slices);
			glColor3f((double)j/(double)slices,1,1);
			
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                
			}glEnd();
		}
	}
}

void drawWheel(double radius , double width ,int slices,int stacks)
{
	//move 
	//cout<<"X = "<<w.pos.x<<endl;
	//cout<<"Y = "<<w.pos.y<<endl;
	glTranslatef(w.pos.x,w.pos.y,0);

    if((GRID_DIFF*GRID_COUNT)<w.pos.y)
    {
        glTranslatef(0,0,(w.pos.y-GRID_DIFF*GRID_COUNT)*sin(INCLINE_ANGLE));
    }
	//glTranslatef(50,50,0);

	//Rotate Left Right 
	glRotatef(w.left_right_theta,0,0,1);

	//Wheel Rectangle
	glPushMatrix();
    {
		glColor3f(1,1,1); //White
        glTranslatef(0,0,radius);
		glRotatef(90,1,0,0);
		glRotatef(w.up_down_theta,0,0,1);
		glRotatef(90,0,1,0);
        drawRectangle(2*radius,width);
    }
    glPopMatrix();

	//Wheel Rectangle
	glPushMatrix();
    {
       	glColor3f(1,1,1); //White
        glTranslatef(0,0,radius);
		glRotatef(90,0,1,0);
		glRotatef(90,1,0,0);
		glRotatef(w.up_down_theta,0,0,1);
		glRotatef(90,0,1,0);
        drawRectangle(2*radius,width);
    }
    glPopMatrix();
	
	//Wheel Circle on Origin
	glTranslatef(0,width/2,radius);

	//Sleep To Walk
	glRotatef(90,1,0,0);

    glPushMatrix();
    {

    if((GRID_DIFF*GRID_COUNT)<w.pos.y)
    {
        //glRotatef(INCLINE_ANGLE,0,0,1);
        //glTranslatef(0,0,(w.pos.y-GRID_DIFF*GRID_COUNT)*sin(INCLINE_ANGLE));
    }

    //dir line
    glBegin(GL_LINES);{


			glColor3f(1.0, 1.0, 0.0); //Yellow - x
			glVertex3f( GRID_DIFF*GRID_COUNT,0,0);
			glVertex3f(-GRID_DIFF*GRID_COUNT,0,0);

			glColor3f(1.0, 0.0, 1.0); //Violet - y
			glVertex3f(0,-GRID_DIFF*GRID_COUNT,0);
			glVertex3f(0, GRID_DIFF*GRID_COUNT,0);

			glColor3f(0.0, 1.0, 1.0); // Sky - z
			glVertex3f(0,0, GRID_DIFF*GRID_COUNT);
			glVertex3f(0,0,-GRID_DIFF*GRID_COUNT);
	}glEnd();
    }
    glPopMatrix();

	//Rotate Up Down
	glRotatef(w.up_down_theta,0,0,1);
	drawCylinder(radius,width,slices,stacks);

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
	gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	//gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();

    glPushMatrix();
    {
        glTranslatef(0,GRID_DIFF*GRID_COUNT,0);
        glRotatef(INCLINE_ANGLE,1,0,0);
		glTranslatef(0,GRID_DIFF*GRID_COUNT,0);
        drawGrid();
    }
    glPopMatrix();

	drawGrid();

	//glRotatef(0,w.dir.x,w.dir.y,w.dir.z);
	drawWheel(w.wheel_radius,w.wheel_weight,WHEEL_SLICES,WHEEL_STACKS);


	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=1;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	pos = point(0.0, 0.0, 0.0);
	dir = point(1.0, 0.0, 0.0);

	w = wheel(pos,dir);

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

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
