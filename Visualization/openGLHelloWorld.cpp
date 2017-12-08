// A simple OpenGL program that draws a triangle and circle.

#include "GL/freeglut.h"
#include "GL/gl.h"
#include <iostream>
#include <cmath>

using namespace std;

void drawTriangle()
{
    glClearColor(0.4, 0.4, 0.4, 0.4);
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glBegin(GL_TRIANGLES);
        glVertex3f(-0.7, 0.7, 0);
        glVertex3f(0.7, 0.7, 0);
        glVertex3f(0, -1, 0);
    glEnd();

    glFlush();
}

void drawCircle()
{
    const double w = glutGet( GLUT_WINDOW_WIDTH );
    const double h = glutGet( GLUT_WINDOW_HEIGHT );
    const double ar = w / h;

    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( -150 * ar, 150 * ar, -150, 150, -1, 1 );
    //glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    const int circle_points=100;
    const float cx=0, cy=0, r=100;
    const float pi = 3.14159f;
    int i = 0;

    glBegin(GL_LINE_LOOP);
    glColor3f(1.0,1.0,1.0); //preto
    for(i=0;i<circle_points;i++)
    {
        const float theta=(2*pi*i)/circle_points;
        glVertex2f(cx+r*cos(theta),cy+r*sin(theta));
    }
    glEnd();
    glFlush();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL - Creating a triangle");
    glutDisplayFunc(drawTriangle);
    glutCreateWindow("OpenGL - Creating a circle");
    glutDisplayFunc(drawCircle);
    glutMainLoop();
    return 0;
}