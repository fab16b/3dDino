//
//  main.cpp
//  3dDino
//
//  Created by Felix Mbikogbia and Abigail Verhelle  on 4/25/21.
//
#include <iostream>
#include <GLUT/GLUT.h>
#include <OPENGL/gl.h>
#include <OPENGL/glu.h>
#include <fstream>
#include <vector>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
void init(void);
void display(void);
void keyboard(unsigned char, int, int);
void resize(int, int);
void drawcube(int, int, int);

int is_depth; /* depth testing flag */
Camera cam;
int main (int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(40, 40);
    glutCreateWindow("The Cube World");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //Camera(cam.PosX, cam.Yeye,Zeye,XCentre,YCentre,ZCentre,0.0,1.0, 1.0);

    /* this time we're going to keep the aspect ratio
    constant by trapping the window resizes */
    glutReshapeFunc(resize);

    glutMainLoop();
    return 0;
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    is_depth = 1;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //Camera(Xeye,Yeye,Zeye,XCentre,YCentre,ZCentre,0.0,1.0, 1.0);
}

void display(void)
{
    if (is_depth)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
        glClear(GL_COLOR_BUFFER_BIT);
    
    /* draw the floor */
    glBegin(GL_QUADS);
        glColor3f(0.2f, 0.2f, 0.2f);
        glVertex3f(-100.0, 0.0, -100.0);
        glColor3f(0.4f, 0.4f, 0.4f);
        glVertex3f(-100.0, 0.0, 100.0);
        glColor3f(0.6f, 0.6f, 0.6f);
        glVertex3f(100.0, 0.0, 100.0);
        glColor3f(0.8f, 0.8f, 0.8f);
        glVertex3f(100.0, 0.0, -100.0);
    glEnd();
    /* draw 12 cubes with different colors */
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
    
    GLfloat light1_ambient [] = {0.2,0.2,0.2,1.0};
    GLfloat light1_diffuse [] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light1_specular[] = {1.0,1.0,1.0,1.0};
    GLfloat light1_position[] = {1.0, 1.0, 0.0, 0.0};
    GLfloat spot_direction[] = {-1.0, -1.0, 0.0};
    
    glLightfv(GL_LIGHT1,GL_AMBIENT,light1_ambient);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1,GL_SPECULAR, light1_specular);
    glLightfv(GL_LIGHT1,GL_POSITION, light1_position);
    
    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.5);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION,0.5);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2);
    
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF,45.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,spot_direction);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
    glEnable(GL_LIGHT1);
    
    
}

void keyboard(unsigned char key, int x, int y)
{
    /* This time the controls are:
    
      "a": move left
      "d": move right
      "w": move forward
      "s": move back
      "t": toggle depth-testing

    */
    
    switch (key)
    {
    case 'a':
    case 'A':
        glTranslatef(5.0, 0.0, 0.0);
        break;
    case 'd':
    case 'D':
        glTranslatef(-5.0, 0.0, 0.0);
        break;
    case 'w':
    case 'W':
        glTranslatef(0.0, 0.0, 5.0);
        break;
    case 's':
    case 'S':
        glTranslatef(0.0, 0.0, -5.0);
        break;
    case 't':
    case 'T':
        if (is_depth)
        {
            is_depth = 0;
            glDisable(GL_DEPTH_TEST);
        }
        else
        {
            is_depth = 1;
            glEnable(GL_DEPTH_TEST);
        }
    }
    display();
}
    
void resize(int width, int height)
{
    if (height == 0) height = 1;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* note we divide our width by our height to get the aspect ratio */
    gluPerspective(45.0, width / height, 1.0, 400.0);

    /* set initial position */
    glTranslatef(0.0, -5.0, -150.0);

    glMatrixMode(GL_MODELVIEW);
}

void drawcube(int x_offset, int z_offset, int color)
{
    /* this function draws a cube centerd at (x_offset, z_offset)
    x and z _big are the back and rightmost points, x and z _small are
    the front and leftmost points */
    float x_big = (float)x_offset + 5;
    float z_big = (float)z_offset + 5;
    float x_small = (float)x_offset - 5;
    float z_small = (float)z_offset - 5;
    switch(color)
    {
    case 1:
        glColor3f(1.0,0.0,0.0);
        break;
    case 2:
        glColor3f(0.0,1.0,0.0);
        break;
    case 3:
        glColor3f(0.0,0.0,1.0);
        break;
    }
    glBegin(GL_QUADS);
        glVertex3f(x_small,10.0,z_big); /* front */
        glVertex3f(x_small,0.0,z_big);
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_big,10.0,z_big);

        glVertex3f(x_big,10.0,z_small); /* back */
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_small,0.0,z_small);
        glVertex3f(x_small,10.0,z_small);

        glVertex3f(x_big,10.0,z_big); /* right */
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_big,10.0,z_small);

        glVertex3f(x_small,10.0,z_small); /* left */
        glVertex3f(x_small,0.0,z_small);
        glVertex3f(x_small,0.0,z_big);
        glVertex3f(x_small,10.0,z_big);

        glVertex3f(x_small,10.0,z_big); /* top */
        glVertex3f(x_big,10.0,z_big);
        glVertex3f(x_big,10.0,z_small);
        glVertex3f(x_small,10.0,z_small);

        glVertex3f(x_small,0.0,z_small); /* bottom */
        glVertex3f(x_big,0.0,z_small);
        glVertex3f(x_big,0.0,z_big);
        glVertex3f(x_small,0.0,z_big);
    glEnd();
}
