#include<bits/stdc++.h>
#include "1705098_headers.h"

using namespace std;

#define pi (2*acos(0.0))
using namespace std;

const double window_height = 500, window_width = 500;
const double view_angle = 80;
int numCaptures = 0;
int video_demo = 0;

Camera camera;

void sendRaysThroughAllPixels(Point &top_left , double du , double dv , bitmap_image &image){
    Color color;
    Object *nearest = nullptr;

    for(int i = 0 ; i < pixels ; i++) {
        for(int j = 0 ; j < pixels; j++) {

            Point current_pixel = top_left.add(camera.r.mul(du * j)).sub(camera.u.mul(dv * i)) ;

            Ray ray(camera.pos, current_pixel.sub(camera.pos));

            double t_min = INF;
            for(Object *object: objects) {
                double t = object->intersect(ray, color, 0);
                if(t > 0 && t < t_min) {
                    t_min = t;
                    nearest = object;
                }
            }
            if(t_min != INF) {
                color.clear();
                nearest->intersect(ray, color, 1);
                color.clamp();
                image.set_pixel(j, i, color[0] * RGB, color[1] * RGB, color[2] * RGB);
            }
        }
    }
}

void capture() {

    cout<<"Generating Image...."<<endl;

    bitmap_image image(pixels, pixels);
    image.clear();

    double plane_distance = (window_height/2.0) / tan( (view_angle * PI)/(180.0 *2.0) );
    Point top_left = camera.pos.add(camera.l.mul(plane_distance)).sub(camera.r.mul(window_width*0.5)).add(camera.u.mul(window_height*0.5));
    double du = window_width/pixels;
    double dv = window_height/pixels;
    top_left = top_left.add(camera.r.mul(0.5 * du)).sub(camera.u.mul(0.5 * dv));
    
    sendRaysThroughAllPixels(top_left, du, dv, image);
    
    image.save_image("1705098_out_" + to_string(++numCaptures) + ".bmp");
    cout<<"Image generation complete"<<'\n';
}

void onkchobi(){
    for(int i = 0 ; i < 10 ; i++) {
      capture();
    }
}
void keyboardListener(unsigned char key, int x,int y){

    switch(key){
        case '1':
            camera.look_left();if(video_demo)onkchobi();
            break;
        case '2':
            camera.look_right();if(video_demo)onkchobi();
            break;
        case '3':
            camera.look_up();if(video_demo)onkchobi();
            break;
        case '4':
            camera.look_down();if(video_demo)onkchobi();
            break;
        case '5':
            camera.tilt_clockwise();if(video_demo)onkchobi();
            break;
        case '6':
            camera.tilt_counter_clockwise();if(video_demo)onkchobi();
            break;
        case 'r':
            REFRACTION_ON ^= 1;
            break;
        case 'v':
            video_demo ^= 1;
            break;
        case '0':
            capture();
            break;
        default:
            break;
    }
}


void specialKeyListener(int key, int x, int y){
    switch(key){
        case GLUT_KEY_DOWN:		//down arrow key
            camera.down_arrow();if(video_demo)onkchobi();
            break;
        case GLUT_KEY_UP:		// up arrow key
            camera.up_arrow();if(video_demo)onkchobi();
            break;
        case GLUT_KEY_RIGHT:
            camera.right_arrow();if(video_demo)onkchobi();
            break;
        case GLUT_KEY_LEFT:
            camera.left_arrow();if(video_demo)onkchobi();
            break;
        case GLUT_KEY_PAGE_UP:
            camera.page_up();if(video_demo)onkchobi();
            break;
        case GLUT_KEY_PAGE_DOWN:
            camera.page_down();if(video_demo)onkchobi();
            break;
        default:
            break;
    }
}


void display_objects() {
    
    //draw axes
    glBegin(GL_LINES);{

        glColor3f(1.0, 0, 0);
        glVertex3f( INF,0,0);
        glVertex3f(-INF,0,0);

        glColor3f(0, 1.0, 0);
        glVertex3f(0,-INF,0);
        glVertex3f(0, INF,0);

        glColor3f(0, 0, 1.0);
        glVertex3f(0,0, INF);
        glVertex3f(0,0,-INF);
    }glEnd();

    for(Object *object: objects) {
        object->draw();
    }

    for(Light light: lights) {
        light.draw();
    }

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

   gluLookAt(camera.pos.x, camera.pos.y, camera.pos.z, camera.pos.x + camera.l.x, camera.pos.y + camera.l.y, camera.pos.z + camera.l.z, camera.u.x, camera.u.y, camera.u.z);
   //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);

    display_objects();
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate(){
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void load_data() {
    ifstream in("scene.txt");
    in>>reflection_level>>pixels;

    in>>total_objects;

    for(int i = 0 ; i < total_objects ; i++) {
        string shape_name;
        in>>shape_name;

        Object *object = nullptr;
        if(shape_name == "sphere") {
            Point center;
            double radius;
            in>>center>>radius;
            object = new Sphere(center, radius);
        }
        else if(shape_name == "triangle") {
            Point A, B, C;
            in>>A>>B>>C;
            object = new Triangle(A,B,C);
        }
        else if(shape_name == "general") {
            double a, b, c, d, e, f, g, h, i, j;
            Point refPoint;
            double length, width, height;

            in >> a >> b >> c >> d >> e >> f >> g >> h >> i >> j;
            in >> refPoint >> length >> width >> height;
            object = new General_Surface(a,b,c,d,e,f,g,h,i,j , refPoint, length, width, height);

        }
        else {
            cout<<"Unknown shape in input file"<<'\n';
            return;
        }

        in>>object->object_color;

        //reflection_coefficients
        double amb, diff ,spec, refl,shine;
        in>>amb>>diff>>spec>>refl;
        in>>shine;
        object->set_ambient_coEfficient(amb);
        object->set_diffuse_coEfficient(diff);
        object->set_specular_coEfficient(spec);
        object->set_reflection_coEfficient(refl);
        object->set_shine(shine);

        //push to objects list
        objects.push_back(object);
    }

    //push_back floor object
    Object *floor = new Floor(floor_width , tile_width);
    objects.push_back(floor);

    //inupt light src
    in>>total_lights;
    for(int i = 0 ; i < total_lights ; i++) {
        Light light;
        in>>light.light_pos;
        in>>light.color;
        lights.push_back(light);
    }
    //cout<<total_lights<<" lights loaded"<<'\n';
    int total_spotlights;
    in>>total_spotlights;
    for(int i=0 ; i<total_spotlights ; i++){
        Light light;
        in>>light.light_pos;
        in>>light.color;

        Point dir;double cutoff_angle;
        in>>dir;
        in>>cutoff_angle;
        light.set_spotlight(true , dir, cutoff_angle);
        lights.push_back(light);
    }

    in.close();

}

void init(){
    //codes for initialization
    camera = Camera();
    REFRACTION_ON = 0;

    load_data();

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
    gluPerspective(view_angle,	1,	1,	2000.0);

}

void clear_memory() {
    for(int i = 0 ; i < objects.size() ; i++) {
        delete objects[i];
    }
    objects.clear();
    lights.clear();
}



int main(int argc, char **argv){

    glutInit(&argc,argv);
    glutInitWindowSize(window_width, window_height);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Ray Tracing");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    glutMainLoop();		//The main loop of OpenGL

    clear_memory();
    return 0;
}
