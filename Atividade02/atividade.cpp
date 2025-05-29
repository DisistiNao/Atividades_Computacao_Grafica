#include <GL/glut.h>
#include <iostream>

// Dependencias:
// sudo apt-get install build-essential freeglut3-dev

// Pra rodar:
// g++ Atividade02/atividade.cpp -o Atividade02/atividade -lGL -lGLU -lglut
// ./Atividade02/atividade

const float squareSize = 0.2f;

float squareX = 0.0f;
float squareY = 0.0f;

const float xMin = -1.0f + squareSize / 2.0f;
const float xMax = 1.0f - squareSize / 2.0f;
const float yMin = -1.0f + squareSize / 2.0f;
const float yMax = 1.0f - squareSize / 2.0f;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
        glVertex2f(squareX - squareSize / 2.0f, squareY - squareSize / 2.0f);
        glVertex2f(squareX + squareSize / 2.0f, squareY - squareSize / 2.0f);
        glVertex2f(squareX + squareSize / 2.0f, squareY + squareSize / 2.0f);
        glVertex2f(squareX - squareSize / 2.0f, squareY + squareSize / 2.0f);
    glEnd();

    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}


void specialKeys(int key, int x, int y) {
    const float moveStep = 0.05f;

    switch (key) {
        case GLUT_KEY_UP:    squareY += moveStep; break;
        case GLUT_KEY_DOWN:  squareY -= moveStep; break;
        case GLUT_KEY_LEFT:  squareX -= moveStep; break;
        case GLUT_KEY_RIGHT: squareX += moveStep; break;
    }

    if (squareX + squareSize / 2.0f > 1.0f) squareX = 1.0f - squareSize / 2.0f;
    if (squareX - squareSize / 2.0f < -1.0f) squareX = -1.0f + squareSize / 2.0f;
    if (squareY + squareSize / 2.0f > 1.0f) squareY = 1.0f - squareSize / 2.0f;
    if (squareY - squareSize / 2.0f < -1.0f) squareY = -1.0f + squareSize / 2.0f;
    
    glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        GLdouble objX, objY, objZ;
        GLdouble modelview[16], projection[16];
        GLint viewport[4];

        glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        glGetIntegerv(GL_VIEWPORT, viewport);

        gluUnProject(x, viewport[3] - y, 0.0, modelview, projection, viewport, &objX, &objY, &objZ);
        
        std::cout << "Clique em: X = " << objX << ", Y = " << objY << std::endl;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Atividade 02");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKeys);
    glutMouseFunc(mouseClick);

    glutMainLoop();

    return 0;
}