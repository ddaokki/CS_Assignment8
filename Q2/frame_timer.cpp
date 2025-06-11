// frame_timer.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

extern std::vector<Vector3> gPositions;
extern std::vector<Vector3> gNormals;
extern std::vector<Triangle> gTriangles;

float   gTotalTimeElapsed = 0.0f;
int     gTotalFrames = 0;
GLuint  gTimer;

GLuint vao, vbo, nbo, ebo;

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
    float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f); // ns → s
    return timeElapsed;
}

void init_vertex_array()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Position VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * gPositions.size(), gPositions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0); // location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Normal VBO
    glGenBuffers(1, &nbo);
    glBindBuffer(GL_ARRAY_BUFFER, nbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * gNormals.size(), gNormals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // location 1
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Index EBO
    std::vector<unsigned int> indices;
    indices.reserve(gTriangles.size() * 3);
    for (const auto& tri : gTriangles)
    {
        indices.push_back(tri.indices[0]);
        indices.push_back(tri.indices[1]);
        indices.push_back(tri.indices[2]);
    }

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 0.0,
        0.0, 0.0, -1.0,
        0.0, 1.0, 0.0);

    // 조명 방향 설정
    glPushMatrix();
    glLoadIdentity();
    float len = sqrtf(3.0f);
    GLfloat lightDir[] = { 1.0f / len, 1.0f / len, 1.0f / len, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glPopMatrix();

    // 모델 변환
    glTranslatef(0.1f, -1.0f, -1.5f);
    glScalef(10.0f, 10.0f, 10.0f);

    start_timing();

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(gTriangles.size() * 3), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    float timeElapsed = stop_timing();
    gTotalFrames++;
    gTotalTimeElapsed += timeElapsed;
    float fps = gTotalFrames / gTotalTimeElapsed;

    char title[128];
    sprintf(title, "OpenGL Bunny (VAO): %.2f FPS", fps);
    glutSetWindowTitle(title);

    glutPostRedisplay();
    glutSwapBuffers();
}
