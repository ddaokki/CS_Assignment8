#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include <math.h>

extern GLuint vao;
float gTotalTimeElapsed = 0;
int gTotalFrames = 0;
GLuint gTimer;

void init_timer()
{
    glGenQueries(1, &gTimer);
}

void start_timing()
{
    glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
    glEndQuery(GL_TIME_ELAPSED);
    GLint available = GL_FALSE;
    while (available == GL_FALSE)
        glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);
    GLint result;
    glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);
    return result / (1000.0f * 1000.0f * 1000.0f);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);

    glPushMatrix();
    glLoadIdentity();
    float len = sqrt(3.0f);
    GLfloat lightDir[] = { 1.0f / len, 1.0f / len, 1.0f / len, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glPopMatrix();

    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    start_timing();
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, gTriangles.size() * 3, GL_UNSIGNED_INT, 0);
    float timeElapsed = stop_timing();

    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;
    char title[256];
    sprintf(title, "OpenGL Bunny: %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutPostRedisplay();
    glutSwapBuffers();
}
