//
//  main.cpp
//  3dDino
//
//  Created by Felix Mbikogbia  on 4/25/21.
//
#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <iostream>
#include <OPENGL/gl.h>
#include <OPENGL/glu.h>
#include <cmath>
using namespace std;

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
    Vector3(float x = 0, float y = 0, float z = 0): x(x), y(y), z(z) {}
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
    
    Vector3 &operator/=(float f){
        float inv = 1.f / f;
        x *= inv; y *= inv; z *= inv;
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

class Dino
{
public:
    Point3 center;

    Dino() {};
    Dino(Point3 c){ center.x = c.x; center.y = c.y; center.z = c.z;}
    void drawDino();
    void drawbody();
    void drawHands();
    void drawhead();
    void move(Point3 c){center = c;}
   
    
    Vector3 u, v, up;
    Vector3 n;
    double viewAngle, aspect, nearDist, farDist;
    bool turn = false;
    double turnAngle = 0;
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
        glScaled(.8,1.2,.8);
        glutSolidCube(6);
    glPopMatrix();
    
    //tail
    
    glPushMatrix();
        glTranslated(center.x+2, center.y-2 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(2);
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(center.x+3.5, center.y-2 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(1.5);
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(center.x+4.5, center.y-2 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(center.x+5.1, center.y-2 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(.5);
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(center.x+5.5, center.y-2 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(.3);
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(center.x+5.7, center.y-2 , center.z);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(.2);
    glPopMatrix();
    

}

void Dino::drawhead()
{
    glPushMatrix();
        glTranslated(center.x-2, center.y+5 , center.z);
        glColor3f(0.9, 0.0, 0.0);
        glScaled(1,1,.8);
        glutSolidCube(4);
    glPopMatrix();
    
    //eye r
    glPushMatrix();
        glTranslated(center.x-2, center.y+5 , center.z-1.7);
        glColor3f(0, 0, 0);
        glScaled(.5, .5, .5);
        glutSolidSphere(.45,10,10);
    glPopMatrix();
    
    //eye l
    glPushMatrix();
        glTranslated(center.x-2, center.y+5 , center.z+1.7);
        glColor3f(0, 0, 0);
        glScaled(.5, .5, .5);
        glutSolidSphere(.45,10,10);
    glPopMatrix();}

void Dino::drawHands()
{
    glPushMatrix();
        glTranslated(center.x-2.9, center.y+.2, center.z-1.7);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1, .5, .5);
        glutSolidCube(1);
    glPopMatrix();
    
    glPushMatrix();
        glTranslated(center.x-2.9, center.y+.2, center.z+1.7);
        glColor3f(1.0, 0.0, 0.0);
        glScaled(1, .5, .5);
        glutSolidCube(1);
    glPopMatrix();
    
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

class Camera{
public:
    Point3 eye, look, up;
    Vector3 u,v,n;
    double viewAngle, aspect, nearDist, farDist;
    void setModelviewMatrix();
    Camera() {};
    void set(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
    void roll(float angle);
    void pitch(float angle);
    void yaw(float angle);
    void slide(float delU, float delV, float delN);
    void setShape(float vAng, float asp, float nearD, float farD);
    void getShape(float &vang, float &asp, float &nearD, float &farD);
};

Camera cam;

void Camera::setModelviewMatrix(void)
{
    float m[16];
    Vector3 eVec(eye.x, eye.y, eye.z);
    m[0] = u.x;  m[4] = u.y; m[8] = u.z; m[12] = - eVec.dot(u);
    m[1] =  v.x; m[5] = v.y;  m[9] = v.z; m[13] = - eVec.dot(v);
    m[2] =  n.x; m[6] = n.y; m[10] = n.z; m[14] = - eVec.dot(n);
    m[3] = 0;  m[7] = 0;   m[11] = 0;   m[15] = 1.0;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(m);
}

void Camera::set(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
        Point3 Eye = Point3(x1,y1,z1);
        Point3 Look = Point3(x2,y2,y3);
        Vector3 Up = Vector3(x3,y3,z3);

        eye.set(Eye);
        n.set(eye.x - Look.x, eye.y - Look.y, eye.y - Look.y);
        u.set(Up.cross(n));
        n.normalize();
        u.normalize();
        v.set(n.cross(u)); //might just be “v.set” in the vector class
        setModelviewMatrix();
}

void Camera::setShape(float vAng, float asp, float nearD, float farD)
{
    viewAngle = vAng;
    aspect = asp;
    nearDist = nearD;
    farDist = farD;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(vAng, asp, nearD, farD);
}

void Camera::slide(float delU, float delV, float delN)
{
    eye.x += delU * u.x + delV * v.x + delN * n.x;
    eye.y += delU * u.y + delV * v.y + delN * n.y;
    eye.z += delU * u.z + delV * v.z + delN * n.z;
    look.x += delU * u.x + delV * v.x + delN * n.x;
    look.y += delU * u.y + delV * v.y + delN * n.y;
    look.z += delU * u.z + delV * v.z + delN * n.z;
    setModelviewMatrix();
}

void Camera::roll(float angle)
{
    float cs = cos(3.14149265/180* angle);
    float sn = sin(3.14149265/180* angle);
    Vector3 t(u);
    u.set(cs * t.x - sn * v.x, cs * t.y - sn * v.y, cs * t.z - sn * v.z);
    v.set(sn * t.x + cs * v.x, sn * t.y + cs * v.y, sn * t.z + cs * v.z);
    setModelviewMatrix();
}

// similar to roll switch the v for n when setting the vector
void Camera::pitch(float angle)
{
    float cs = cos(3.14149265/180* angle);
    float sn = sin(3.14149265/180* angle);
    Vector3 t(v);
    v.set(cs * t.x - sn * n.x, cs * t.y - sn * n.y, cs * t.z - sn * n.z);
    n.set(sn * t.x + cs * n.x, sn * t.y + cs * n.y, sn * t.z + cs * n.z);
    setModelviewMatrix();
}

// similar to roll switch the v for u when setting the vector
void Camera::yaw(float angle)
{
    float j = 3.14149265/180* angle;
    float cs = cos(j);
    float sn = sin(j);
    Vector3 t(n);
    n.set(cs * t.x - sn * u.x, cs * t.y - sn * u.y, cs * t.z - sn * u.z);
    u.set(sn * t.x + cs * u.x, sn * t.y + cs * u.y, sn * t.z + cs * u.z);
    setModelviewMatrix();
}

double lambert(Vector3 s, Vector3 m)
{
    float top = s.dot(m);
    float bottom = s.magnitude() * m.magnitude();
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
    
    return phong;
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



Point3 sun(5,5,5);
double sunNum = 0;
void display(void)
{
    
    GLfloat pos[]  = {sun.x,sun.y,sun.z,1.0};
    GLfloat amb[]  = {.8,.9,.5,0};
    GLfloat dif[]  = {1.0,.8,1.0,0};
    GLfloat spec[] = {1.0,.8,1.0,0};
    glLightfv(GL_LIGHT0, GL_AMBIENT,  amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  dif);
    glLightfv(GL_LIGHT0, GL_SPECULAR, spec);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glPopMatrix();

    
    //Ground Plane
    //glPushMatrix();
        //glColor4d(0.2, 0.5, 0.2,0);
        //glTranslated(-3,-3,-3);
        //glScaled(25,1,25);
        //glutSolidCube(1);
    //glPopMatrix();
    
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.5f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(-50.0, 0.0, -50.0);
        glColor3f(0.2f, 0.5f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(-50.0, 0.0, 50.0);
        glColor3f(0.2f, 0.5f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(50.0, 0.0, 50.0);
        glColor3f(0.2f, 0.5f, 0.2f);
        //glColor3f(0.0,0.0,0.0);
        glVertex3f(50.0, 0.0, -50.0);
    
    
    //double x[] = { 1, 2,-3, 4, -4, 3, 3, -5, -2, -1};
    //double z[] = { 3,-4, 3,-3, -1,-5, 2,  1,  4, -2};
    
    double x[] = {2, 2,  4, -5, 2, -10, 8,  8, -10 };
    double z[] = {2, 6, -5, 3, -6, -10, 8, -10, 8};
    for(int i = 0; i < 9; i++)
    {
        //base log
        glPushMatrix();
            glColor3f(.2,.1,.1);
            glTranslated(x[i],0,z[i]);
            glScaled(.3,3,.3);
            glutSolidCube(1);
        glPopMatrix();
        
        glPushMatrix();
            glColor3f(.2, .9, .2);
            glTranslated(x[i],.6,z[i]);
            glutSolidSphere(1, 8, 4);
        glPopMatrix();
        

    }
    
    double a[] = {-2, 6, -5, -7, 5};
    double b[] = {-6, -4, -2,  7,  4};
    for(int i = 0; i < 5; i++)
    {
        //base log
        glPushMatrix();
            glColor3f(.2,.1,.1);
            glTranslated(a[i],0,b[i]);
            //glScaled(.3,3,.3);
            glutSolidCube(1);
        glPopMatrix();
        
    }
    
    //draw dino function
    glPushMatrix();
        glScaled(.4,.4,.4);
        dino.drawDino();
    glPopMatrix();

    
    //pg 408 hidden surface
    glFlush();
    //glutSwapBuffers();

}

int follow = 0;
void keyboard(unsigned char key, int xmouse, int ymouse)
{
    switch(key)
    {
            //all camera movement
            //yaw, pitch, roll, slide
            // yaw pitch and roll all change by 2 or -2
            case 'a': cam.yaw(2.0);    break;
            case 'd': cam.yaw(-2.0);     break;
            case 'w': cam.pitch(2.0);  break;
            case 's': cam.pitch(-2.0);   break;
            case 'q': cam.roll(2.0);   break;
            case 'e': cam.roll(-2.0);    break;
            case 'D': cam.slide( .2, 0,  0);    break;
            case 'S': cam.slide( 0,  0, .2);    break;
            case 'A': cam.slide(-.2, 0,  0);   break;
            case 'W': cam.slide( 0,  0,-.2);   break;
            case 'E': cam.slide( 0, .2,  0);     break;
            case 'Q': cam.slide( 0,-.2,  0);    break;
            case 'f' : follow ? follow = false : follow = true;     break;

            // all dino movement

            case 'j': dino.turnAngle += 10; break;
            case 'l': dino.turnAngle -= 10; break;
            case 'i': dino.slide(0,  .2, 0); break;
            case 'k': dino.slide(0, -.2, 0); break;
            

            case 'u': sun.x = 13*cos(sunNum); sun.y = 13*sin(sunNum); sunNum += .2; break;
            
            case 27: exit(1);
    }

    if(follow)
        cam.set(cam.eye.x,cam.eye.y,cam.eye.z,dino.center.x*.2,dino.center.y*.2,dino.center.z*2,0,1,0);

    glutPostRedisplay();

}


void SpecialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_RIGHT: dino.slide(0.2,0,0); break;
        case GLUT_KEY_DOWN: dino.slide(0,0,0.2); break;
        case GLUT_KEY_LEFT: dino.slide(-0.2,0,0); break;
        case GLUT_KEY_UP: dino.slide(0,0,-0.2); break;
    }
    
    if(follow)
        cam.set(cam.eye.x,cam.eye.y,cam.eye.z,dino.center.x*.2,dino.center.y*.2,dino.center.z*2,0,1,0);
    
    glutPostRedisplay();
}
void init(void)
{
    glClearColor(0.0, 0.6, 0.8, 1.0); // background blue
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glViewport(0, 0, 640, 430);
    //eye, look, up
    cam.setShape(30.0, 64.0/48.0, .5, 100.0);
    cam.set(5,5,5,0,0,0,0,1,0);
    dino.set(3,0,0,0,0,0,0,1,0);
   
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 500);
    glutInitWindowPosition(680, 480);
    glutCreateWindow("3D dino");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(SpecialKeys);
    init();
    glutMainLoop();
}

