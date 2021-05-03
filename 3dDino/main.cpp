//
//  main.cpp
//  3dDino
//
//  Created by Felix Mbikogbia  on 4/25/21.
//
#include <iostream>
#include <GLUT/GLUT.h>
#include <OPENGL/gl.h>
#include <OPENGL/glu.h>
#include <fstream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace std;

void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT: cam.slide(-0.2,0,0); break;
        case GLUT_KEY_UP: cam.slide(0,0,-0.2); break;
        case GLUT_KEY_RIGHT: cam.slide(0.2,0,0); break;
        case GLUT_KEY_DOWN: cam.slide(0,0,0.2); break;
    }
    
    glutPostRedisplay();
}

void init(void)
{
    glClearColor(0.0, 0.6, 0.8, 1.0); // background blue
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_COLOR_MATERIAL);
    glViewport(0, 0, 640, 430);
    //eye, look, up
    //cam.set(5,5,5,0,0,0,0,1,0)
    //cam.setShape(30.0, 64.0/48.0, .5, 100.0);
}

class Point3
{
public:
    float x, y, z;
    Point3() {};
    Point3(float x, float y, float z): x(x), y(y), z(z) {}
    
    void set(Point3 p) {x = p.x; y = p.y; z = p.z;}
};

double SX = 1, SY =1 , SZ = 1;

class Vector3
{
public:
    float x,y,z;
    Vector3(float x, float y, float z): x(x), y(y), z(z) {}
    Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z) {}
    Vector3(Point3 a, Point3 b): x(b.x - a.x), y(b.y  - a.y), z(b.z  - a.z) {}
    ~Vector3() {}
    
    void set(float xx, float yy, float zz) {x = xx; y = yy; z = zz; }
    void set(Point3 p) {x = p.x; y = p.y; z = p.z;}
    void set(Vector3 v) {x = v.x; y = v.y; z = v.z;}
    Vector3 negative() {return Vector3(-1*x, -1*y, -1*z);}
    
    Vector3& operator=(const Vector3& vector){
        if (&vector != this)
        {
            x = vector.x;
            y = vector.y;
            z = vector.z;
        }
        return *this;
    }
    
    Vector3 operator+(const Vector3 &v){
        return Vector3(x + v.x, y + v.y, z + v.z);
    }
    
    Vector3 operator-(const Vector3 &v){
        return Vector3(x - v.x, y - v.y, z - v.z);
    }
    
    Vector3& operator+=(const Vector3 &v){
        x += v.x; y += v.y; z += v.z;
        return *this;
    }
    
    Vector3& operator-=(const Vector3 &v){
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }
    
    bool operator==(const Vector3 &v) const {
        return x == v.x && y == v.y && z == v.z;
    }
    
    bool operator!=(const Vector3 &v) const {
        return !(*this == v);
    }
    
    Vector3 operator*(float f) const{
        return Vector3(f*x, f*y, f*z);
    }
    
    Vector3 operator/(float f) const{
        float inv = 1.f / f;
        return Vector3(inv*x, inv*y, inv*z);
    }
    
    Vector3 &operator*(float f){
        float inv = 1.f / f;
        x /= inv*x; y /= inv*y; z /= inv*z;
        return *this;
    }
    
    Vector3 &operator*=(float f){
        x *= f; y *= f; z *= f;
        return *this;
    }
    
    
    Vector3 operator-() const{
        return Vector3(-x, -y, -z);
    }
    
    float dot(const Vector3 &v) const {
        return x * v.x + y * v.y + z * v.z;
    }
    
    Vector3 cross(const Vector3 &v) const{
        return  Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    float lengthSquared() const{
        return x*x + y*y + z*z;
    }
    
    float magnitude() const{
        return sqrt(lengthSquared());
    }
    
    Vector3 normalize() {
        float l = magnitude();
        x/=l;
        y/=l;
        z/=l;
        return *this;
    }
    
};


double lambert(Vector3 s, Vector3 m)
{
    float top = s.dot(m);
    float bottom = s.magnitude() * m.normalize();
    float n = top/bottom;
    float lambert = max( (float)0,n );
    return lambert;;
}

double phong(Vector3 v, Vector3 s, Vector3 m, int f)
{
    Vector3 h = s + v;
    Vector3 left = ( h/h.magnitude());
    Vector3 right = ( m/m.magnitude());
    float frac = left.dot(right);
    float n = pow(frac,f);
    float phong = max((float)0,n );
    
    return phong;;
}

Vector3 getR(Vector3 s, Vector3 m)
{
    double top = s.dot(m);
    double bottom = pow(m.magnitude(),2);
    double fraction = top/bottom;
    
    return s.negative() + (m*(2*fraction));
}

Vector3 getSV(Point3 shape, Point3 sun)
{
    return Vector3(shape, sun);
}

double light(Vector3 s, Vector3 m, Vector3 v, double Ia, double Pa, double Id, double Pd,double Is, double Ps, int f)
{
    return (Ia * Pa) + (Id * Pd * lambert(s,m)) + (Is * Ps * phong(v, s, m, f));
}

class Dino
{
public:
    Point3 center;
    Vector3 u, v, n, up;
    double viewAngle, aspect, nearDist, farDist;
    bool turn = false;
    double turnAngle = 0;
    Dino() {};
    Dino(Point3 c){ center.x = c.x; center.y = c.y; center.z = c.z;}
    void drawDino();
    void drawbody();
    void drawHands();
    void drawhead();
    void move(Point3 c){center = c;}
   
    //using the slide funtion tho I want it to hop
    void slide(float delU, float delV, float delN);
    void set(Point3 center,Point3 look, Vector3 up);
    void set(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
};



void Dino::set(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    Point3 Dino = Point3(x1,y1,z1);
    Point3 Look = Point3(x2,y2,z2);
    Vector3 up = Vector3(x3,y3,z3);
    
    center.set(Dino);
    n.set(center.x - Look.x, center.y - Look.y, center.z - Look.z);
    u.set(up.cross(n));
    n.normalize();
    u.normalize();
    v.set(n.cross(u));
}

void Dino::slide(float delU, float delV, float delN)
{
    center.x += delU * u.x + delV * v.x + delN * n.x;
    center.y += delU * u.y + delV * v.y + delN * n.y;
    center.z += delU * u.z + delV * v.z + delN * n.z;
    
    cout << " " << center.x << " " << center.y << " " << center.z << endl;

}

Dino dino;

void Dino::drawbody()
{
   
    glPushMatrix();
        glTranslated(center.x, center.y , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glutSolidCube(1);
    glPopMatrix();
    
    //eye r
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslated(center.x, center.y, center.z);
        //glScaled(1, 1, 1);
        glutSolidSphere(1,1,1);
    glPopMatrix();
    
    //eye l
    glPushMatrix();
        glColor3f(1.0, 1.0, 1.0);
        glTranslated(center.x, center.y, center.z);
        //glScaled(1, 1, 1);
        glutSolidSphere(1,1,1);
    glPopMatrix();
}

void Dino::drawhead()
{
    glPushMatrix();
        glTranslated(center.x-.48, center.y+.75 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glutSolidCube(.5);
    glPopMatrix();
}

void Dino::drawHands()
{
    glPushMatrix();
        glTranslated(center.x-.55, center.y+.2, center.z);
        glColor3f(1.0, 0.0, 0.0);
        glutSolidCube(.1);
    glPopMatrix();
    
    //fingure 1 - 6
}

void Dino::drawDino()
{
    glTranslated(dino.center.x, dino.center.y, dino.center.z);
    glRotated(dino.turnAngle, 0, 1, 0);
    glTranslated(-dino.center.x, -dino.center.y,-dino.center.z);
    
    this -> drawbody();
    this -> drawhead();
    this -> drawHands();
}

void drawcube(int x_offset, int z_offset, int color)
{
     /* this function draws a cube centerd at (x_offset, z_offset)
    x and z _big are the back and rightmost points, x and z _small are
    the front and leftmost points*/
    float x_big = (float)x_offset + 5;
    float z_big = (float)z_offset + 5;
    float x_small = (float)x_offset - 5;
    float z_small = (float)z_offset - 5;
    switch(color)
    {
            
    case 1:
        glColor3f(1.0,0.0,0.0); // red
        break;
    case 2:
        glColor3f(0.2,1.0,1.2); // light blue
        break;
    case 3:
        glColor3f(0.0,0.0,1.0); // blue
        break;
    }
    glBegin(GL_QUADS);
        glVertex3f(x_small,10.0,z_big);  //front
        glVertex3f(x_small,0.0,z_big);
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_big,10.0,z_big);

        glVertex3f(x_big,10.0,z_small);  //back
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_small,0.0,z_small);
        glVertex3f(x_small,10.0,z_small);

        glVertex3f(x_big,10.0,z_big);  //right
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_big,10.0,z_small);

        glVertex3f(x_small,10.0,z_small);  //left
        glVertex3f(x_small,0.0,z_small);
        glVertex3f(x_small,0.0,z_big);
        glVertex3f(x_small,10.0,z_big);

        glVertex3f(x_small,10.0,z_big);  //top
        glVertex3f(x_big,10.0,z_big);
        glVertex3f(x_big,10.0,z_small);
        glVertex3f(x_small,10.0,z_small);

        glVertex3f(x_small,0.0,z_small);  //bottom
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_small,0.0,z_big);
    glEnd();
}

Point3 sun(5,5,5);
double sunNum = 0;

void display(void)
{
    
    GLfloat pos[]  = {sun.x,sun.y,sun.z,0.0};
    GLfloat amb[]  = {.8,.9,.5,0};
    GLfloat dif[]  = {1.0,.8,1.0,0};
    GLfloat spec[] = {1.0,.8,1.0,0};
    glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, pos);


     glPopMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Ground Plane
    glPushMatrix();
        glColor4d(0.0, 0.0, 1.0,0);
        glTranslated(-3,-3,-3);
        glScaled(100,1,100);
        glutSolidCube(2);
    glPopMatrix();
    
    //draw axis lines, x = red, y = green, z = blue
    //glPushMatrix();
        //axis(1);
    //glPopMatrix();

    //draw dino function
    glPushMatrix();
        glScaled(.4,.4,.4);
        dino.drawDino();
    glPopMatrix();


    double x[] = { 1, 2,-3, 4, -4, 3, 3, -5, -2, -1};
    double z[] = { 3,-4, 3,-3, -1,-5, 2,  1,  4, -2};
    for(int i = 0; i < 10; i++)
    {
        //base log
        glPushMatrix();
            glColor3f(.2,.1,.1);
            glTranslated(x[i],0,z[i]);
            glScaled(.3,3,.3);
            glutSolidCube(.5);
        glPopMatrix();
        double n = 0;
        for(int j = 0; j < 3; j++)
        {
            //leaves
            glPushMatrix();
                glColor3f((double)i/20.0,n+.1,(double)i/20.0);
                glTranslated(x[i],n,z[i]);
                glRotated(-90, 1, 0, 0);
                glutSolidCone(.5,.5,5,5);
            glPopMatrix();
            n+=.4;
        }
    }
    
    draw 12 cubes with different colors
    drawcube(75, 57, 2);
    drawcube(-65, -12, 3);
    drawcube(50, -50, 1);
    drawcube(-56, 17, 2);
    drawcube(67, 12, 3);
    drawcube(-87, 32, 1);
    drawcube(-26, 75, 2);
    drawcube(57, 82, 3);
    drawcube(-3, 12, 1);
    drawcube(46, 35, 2);
    drawcube(37, -2, 3);
    glutSwapBuffers();

    //pg 408 hidden surface
    glFlush();
    glutSwapBuffers();

}




/*
void drawcube(int x_offset, int z_offset, int color)
{
     this function draws a cube centerd at (x_offset, z_offset)
    x and z _big are the back and rightmost points, x and z _small are
    the front and leftmost points
    float x_big = (float)x_offset + 5;
    float z_big = (float)z_offset + 5;
    float x_small = (float)x_offset - 5;
    float z_small = (float)z_offset - 5;
    switch(color)
    {
            
    case 1:
        glColor3f(1.0,0.0,0.0); // red
        break;
    case 2:
        glColor3f(0.2,1.0,1.2); // light blue
        break;
    case 3:
        glColor3f(0.0,0.0,1.0); // blue
        break;
    }
    glBegin(GL_QUADS);
        glVertex3f(x_small,10.0,z_big);  front
        glVertex3f(x_small,0.0,z_big);
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_big,10.0,z_big);

        glVertex3f(x_big,10.0,z_small);  back
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_small,0.0,z_small);
        glVertex3f(x_small,10.0,z_small);

        glVertex3f(x_big,10.0,z_big);  right
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_big,10.0,z_small);

        glVertex3f(x_small,10.0,z_small);  left
        glVertex3f(x_small,0.0,z_small);
        glVertex3f(x_small,0.0,z_big);
        glVertex3f(x_small,10.0,z_big);

        glVertex3f(x_small,10.0,z_big);  top
        glVertex3f(x_big,10.0,z_big);
        glVertex3f(x_big,10.0,z_small);
        glVertex3f(x_small,10.0,z_small);

        glVertex3f(x_small,0.0,z_small);  bottom
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_small,0.0,z_big);
    glEnd();
}
*/
/*
void display(void)
{

    
    if (is_depth)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
        glClear(GL_COLOR_BUFFER_BIT);
    

     draw the floor
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.9f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(-100.0, 0.0, -100.0);
        glColor3f(0.2f, 0.9f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(-100.0, 0.0, 100.0);
        glColor3f(0.2f, 0.9f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(100.0, 0.0, 100.0);
        glColor3f(0.2f, 0.9f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(100.0, 0.0, -100.0);
    
        //GLfloat cyan[] = {0.1f, 0.1f, 0.0f, 0.0f};
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, cyan);
    glEnd();
    draw 12 cubes with different colors
    drawcube(75, 57, 2);
    drawcube(-65, -12, 3);
    drawcube(50, -50, 1);
    drawcube(-56, 17, 2);
    drawcube(67, 12, 3);
    drawcube(-87, 32, 1);
    drawcube(-26, 75, 2);
    drawcube(57, 82, 3);
    drawcube(-3, 12, 1);
    drawcube(46, 35, 2);
    drawcube(37, -2, 3);
    glutSwapBuffers();
    
    //glutSolidSphere(1.0, 25, 25);
 
    
}
*/
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(640, 0);
    glutCreateWindow("3D dino");
    glutDisplayFunc(display);
    //glutKeyboardFunc(keyboard);
    //glutSpecialFunc(SpecailKeys);
    init();
    //dino.set(3,0,0,0,0,0,0,1,0);
    glutMainLoop();
}

