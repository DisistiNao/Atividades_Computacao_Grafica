#include <GL/glut.h>
#include <cmath>
#include <vector>

// Dependencias:
// sudo apt-get install build-essential freeglut3-dev

// Pra rodar:
// g++ Atividade03/atividade.cpp -o Atividade03/atividade -lGL -lGLU -lglut
// ./Atividade03/atividade

float cameraAngle = 0.0f;
float zoom = -5.0f;
float angleX = 0, angleY = 0;
float parentRot = 0, childRot = 0;

// HSV to RGB conversion
void HSVtoRGB(float h, float s, float v, float &r, float &g, float &b) {
    int i = int(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);
    switch(i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }
}

// Desenha um cubo manual com cores HSV convertidas para RGB
void drawColoredCube(float alpha = 1.0f) {
    glBegin(GL_TRIANGLES);
    float hsv[6][3] = {
        {0.0f, 1, 1}, {0.1f, 1, 1}, {0.2f, 1, 1},
        {0.3f, 1, 1}, {0.5f, 1, 1}, {0.7f, 1, 1}
    };

    float r, g, b;
    float vertices[][3] = {
        {-1,-1,-1},{ 1,-1,-1},{ 1, 1,-1},{-1, 1,-1}, // Back
        {-1,-1, 1},{ 1,-1, 1},{ 1, 1, 1},{-1, 1, 1}  // Front
    };
    int faces[][6] = {
        {0,1,2, 2,3,0}, // back
        {4,5,6, 6,7,4}, // front
        {0,4,7, 7,3,0}, // left
        {1,5,6, 6,2,1}, // right
        {3,2,6, 6,7,3}, // top
        {0,1,5, 5,4,0}  // bottom
    };

    for (int i = 0; i < 6; ++i) {
        HSVtoRGB(hsv[i][0], hsv[i][1], hsv[i][2], r, g, b);
        glColor4f(r, g, b, alpha);
        for (int j = 0; j < 6; ++j) {
            int v = faces[i][j];
            glVertex3fv(vertices[v]);
        }
    }
    glEnd();

    // Arestas
    glColor3f(0, 0, 0);
    glLineWidth(2);
    glBegin(GL_LINES);
    int edges[][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };
    for (auto &e : edges) {
        glVertex3fv(vertices[e[0]]);
        glVertex3fv(vertices[e[1]]);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(zoom * sin(cameraAngle), 0, zoom * cos(cameraAngle), 0, 0, 0, 0, 1, 0);

    glRotatef(angleX, 1, 0, 0);
    glRotatef(angleY, 0, 1, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Pai
    glPushMatrix();
    glRotatef(parentRot, 0, 1, 0);
    drawColoredCube(0.6f);

    // Filho
    glPushMatrix();
    glTranslatef(2.5f, 0, 0);
    glScalef(0.5, 0.5, 0.5);
    glRotatef(childRot, 0, 1, 0);
    drawColoredCube(0.8f);
    glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = 1.0f * w / h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, ratio, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'a': cameraAngle -= 0.1f; break;
        case 'd': cameraAngle += 0.1f; break;
        case 'w': zoom += 0.5f; break;
        case 's': zoom -= 0.5f; break;
        case 'j': parentRot += 5; break;
        case 'k': childRot += 5; break;
        case 27: exit(0); // ESC
    }
    glutPostRedisplay();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cubo Hierárquico Colorido - OpenGL");

    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
