#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <vector>
#include <string>
#include <iostream>

#include "load_mesh.cpp"
#include "frame_timer.cpp"

void initialize_opengl()
{
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE); // Back-face culling 비활성화

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); // 스케일 변환 있으므로 필요

    // 고정 파이프라인 머티리얼 설정
    GLfloat ka[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat kd[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ks[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat p = 0.0f;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ks);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, p);

    // 조명 색상 설정
    GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // 투영 행렬 설정
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1000.0);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1280, 1280);
    glutCreateWindow("OpenGL Bunny");

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "GLEW Init Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    // 메쉬 및 OpenGL 초기화
    load_mesh("bunny.obj");
    initialize_opengl();
    init_timer();
    init_vertex_array(); // Q2 전용 VAO/VBO/EBO 초기화

    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
