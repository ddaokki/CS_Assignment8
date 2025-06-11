// frame_timer.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

extern std::vector<Vector3> gPositions;
extern std::vector<Vector3> gNormals;
extern std::vector<Triangle> gTriangles;

float   gTotalTimeElapsed = 0;
int     gTotalFrames = 0;
GLuint  gTimer;

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
	float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
	return timeElapsed;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0.0, 0.0, 0.0,   // eye
		0.0, 0.0, -1.0,   // center (w 방향)
		0.0, 1.0, 0.0    // up
	);
	
	// 변환
	glPushMatrix(); // 현재 MODELVIEW 행렬 저장
	glLoadIdentity(); // 조명 방향 설정 시 단위행렬 상태
	float len = sqrt(3.0f);
	GLfloat lightDir[] = {
		1.0f / len,
		1.0f / len,
		1.0f / len,
		0.0f  // 방향벡터이므로 w = 0
	};
	glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
	glPopMatrix();
	glTranslatef(0.1f, -1.0f, -1.5f);
	glScalef(10.0f, 10.0f, 10.0f);
	


	start_timing();

	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < gTriangles.size(); ++i)
	{
		unsigned int k0 = gTriangles[i].indices[0];
		unsigned int k1 = gTriangles[i].indices[1];
		unsigned int k2 = gTriangles[i].indices[2];

		glNormal3f(gNormals[k0].x, gNormals[k0].y, gNormals[k0].z);
		glVertex3f(gPositions[k0].x, gPositions[k0].y, gPositions[k0].z);

		glNormal3f(gNormals[k1].x, gNormals[k1].y, gNormals[k1].z);
		glVertex3f(gPositions[k1].x, gPositions[k1].y, gPositions[k1].z);

		glNormal3f(gNormals[k2].x, gNormals[k2].y, gNormals[k2].z);
		glVertex3f(gPositions[k2].x, gPositions[k2].y, gPositions[k2].z);
	}
	glEnd();

	float timeElapsed = stop_timing();
	gTotalFrames++;
	gTotalTimeElapsed += timeElapsed;
	float fps = gTotalFrames / gTotalTimeElapsed;
	char string[1024] = { 0 };
	sprintf(string, "OpenGL Bunny: %0.2f FPS", fps);
	glutSetWindowTitle(string);

	glutPostRedisplay();
	glutSwapBuffers();
}
