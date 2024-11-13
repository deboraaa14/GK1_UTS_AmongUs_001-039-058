#include <glut.h>
#include <cmath>
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <ctime>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float n = 400.0, a, x, y;
float charX = 0.0;
float charY = -0.4;
float charScale = 0.003;
float minX = -0.9f;  // Batas dinding kiri
float maxX = 0.9f;   // Batas dinding kanan
float minY = -0.8f;  // Batas lantai bawah
float maxY = 0.3f;   // Batas dinding atas

void drawDetailedBox(float x, float y, float width, float height, float r, float g, float b);
void drawDetailedChair(float x, float y);
void drawDetailedFloor();
void drawDetailedWall();
void drawDetailedWindow(float x, float y);
void drawCharacter(float posX, float posY, float scale);
void display();
void init();

void drawGradientQuad(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4,
    float r1, float g1, float b1, float r2, float g2, float b2) {
    glBegin(GL_QUADS);
    glColor3f(r1, g1, b1);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glColor3f(r2, g2, b2);
    glVertex2f(x3, y3);
    glVertex2f(x4, y4);
    glEnd();
}

void drawDetailedBox(float x, float y, float width, float height, float r, float g, float b) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Bayangan
    glColor4f(0.0, 0.0, 0.0, 0.3);
    glBegin(GL_QUADS);
    glVertex2f(-width / 2 + 0.03, -height / 2 - 0.03);
    glVertex2f(width / 2 + 0.03, -height / 2 - 0.03);
    glVertex2f(width / 2 + 0.03, height / 2 - 0.03);
    glVertex2f(-width / 2 + 0.03, height / 2 - 0.03);
    glEnd();

    // Gradasi
    drawGradientQuad(-width / 2, -height / 2, width / 2, -height / 2, width / 2, height / 2, -width / 2, height / 2,
        r, g, b, r * 0.7, g * 0.7, b * 0.7);

    // Highlight 
    glColor3f(r * 1.2, g * 1.2, b * 1.2);
    glBegin(GL_QUADS);
    glVertex2f(-width / 2, height / 2);
    glVertex2f(width / 2, height / 2);
    glVertex2f(width / 2, height / 2 - height * 0.1);
    glVertex2f(-width / 2, height / 2 - height * 0.1);
    glEnd();

    // Refeklesi Kaca
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    float alpha = 0.2;
    glBegin(GL_QUADS);
    glColor4f(1.0, 1.0, 1.0, alpha);
    glVertex2f(-width / 4, height / 4);
    glVertex2f(width / 4, height / 4);
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glVertex2f(width / 4, -height / 4);
    glVertex2f(-width / 4, -height / 4);
    glEnd();
    glDisable(GL_BLEND);

    // Stroke
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-width / 2, -height / 2);
    glVertex2f(width / 2, -height / 2);
    glVertex2f(width / 2, height / 2);
    glVertex2f(-width / 2, height / 2);
    glEnd();

    glPopMatrix();
}

void drawDetailedChair(float x, float y) {
    glPushMatrix();
    glTranslatef(x, y, 0);

    // Warna Dasar
    float baseR = 0.2, baseG = 0.6, baseB = 0.8;

    // Gradasi
    drawGradientQuad(-0.03, 0.0, 0.03, 0.0, 0.03, 0.04, -0.03, 0.04,
        baseR, baseG, baseB, baseR * 0.7, baseG * 0.7, baseB * 0.7);

    // Backrest with curved top and gradient
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(baseR * 1.2, baseG * 1.2, baseB * 1.2);
    glVertex2f(0, 0.07); // Center point

    int segments = 12;
    float radius = 0.03;
    for (int i = 0; i <= segments; i++) {
        float theta = M_PI * (1.0f - (float)i / segments);
        float px = radius * cos(theta);
        float py = radius * sin(theta) + 0.07;
        glColor3f(baseR * 0.8, baseG * 0.8, baseB * 0.8);
        glVertex2f(px, py);
    }
    glEnd();

    // Glossy highlight on backrest
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_TRIANGLE_STRIP);
    float alpha = 0.3;
    glColor4f(1.0, 1.0, 1.0, alpha);
    glVertex2f(-0.015, 0.06);
    glVertex2f(0.015, 0.06);
    glColor4f(1.0, 1.0, 1.0, 0.0);
    glVertex2f(-0.01, 0.08);
    glVertex2f(0.01, 0.08);
    glEnd();
    glDisable(GL_BLEND);

    // Stroke
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1.5);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i <= segments; i++) {
        float theta = M_PI * (1.0f - (float)i / segments);
        float px = radius * cos(theta);
        float py = radius * sin(theta) + 0.07;
        glVertex2f(px, py);
    }
    glVertex2f(0.03, 0.04);
    glVertex2f(0.03, 0);
    glVertex2f(-0.03, 0);
    glVertex2f(-0.03, 0.04);
    glEnd();

    glPopMatrix();
}

void drawDetailedFloor() {
    // Warna dasar Gradasi
    drawGradientQuad(-1.2, -1.2, 1.2, -1.2, 1.2, 0.3, -1.2, 0.3,
        0.25, 0.25, 0.25, 0.2, 0.2, 0.2);

    // Grid Efek
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Garis Vertikal
    for (float x = -1.2; x <= 1.2; x += 0.1) {
        glBegin(GL_LINES);
        glColor4f(0.3, 0.3, 0.3, 0.7);
        glVertex2f(x, -1.2);
        glColor4f(0.3, 0.3, 0.3, 0.3);
        glVertex2f(x, 0.3);
        glEnd();
    }

    // Garis Horizontal
    for (float y = -1.2; y <= 0.3; y += 0.1) {
        glBegin(GL_LINES);
        glColor4f(0.3, 0.3, 0.3, 0.7);
        glVertex2f(-1.2, y);
        glColor4f(0.3, 0.3, 0.3, 0.3);
        glVertex2f(1.2, y);
        glEnd();
    }

    glDisable(GL_BLEND);
}

void drawDetailedWall() {
    // Warna dasar n Gradasi
    drawGradientQuad(-1.2, 0.0, 1.2, 0.0, 1.2, 0.99, -1.2, 0.99,
        0.35, 0.35, 0.35, 0.25, 0.25, 0.25);

    // Texture
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (float y = 0.0; y <= 0.99; y += 0.05) {
        glBegin(GL_LINES);
        glColor4f(0.3, 0.3, 0.3, 0.1);
        glVertex2f(-1.2, y);
        glColor4f(0.3, 0.3, 0.3, 0.2);
        glVertex2f(1.2, y);
        glEnd();
    }
    glDisable(GL_BLEND);

    drawDetailedWindow(0.0, 0.55);
}

void drawDetailedWindow(float x, float y)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    float windowWidth = 1.25;
    float windowHeight = 0.15;
    float startX = -windowWidth / 2;
    float endX = windowWidth / 2;

    // Warna Dasar Biru
    glColor3f(0.1, 0.2, 0.4);
    glBegin(GL_POLYGON);
    glVertex2f(startX, 0.05);
    glVertex2f(endX, 0.05);
    glVertex2f(endX, 0.05 + windowHeight);
    glVertex2f(startX, 0.05 + windowHeight);
    glEnd();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Diagonal
    glBegin(GL_POLYGON);

    // Warna Dibawah Kiri
    glColor4f(1.0, 1.0, 1.0, 0.1);
    glVertex2f(startX, 0.05);

    // Warna Diatas Kiri
    glColor4f(1.0, 1.0, 1.0, 0.2);
    glVertex2f(startX, 0.05 + windowHeight);

    // Warna Diatas Kanan
    glColor4f(1.0, 1.0, 1.0, 0.4);
    glVertex2f(endX, 0.05 + windowHeight);

    // Warna Dibawah Kanan
    glColor4f(1.0, 1.0, 1.0, 0.2);
    glVertex2f(endX, 0.05);

    glEnd();
    glDisable(GL_BLEND);

    // Stroke
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(startX, 0.05);
    glVertex2f(endX, 0.05);
    glVertex2f(endX, 0.05 + windowHeight);
    glVertex2f(startX, 0.05 + windowHeight);
    glEnd();

    // Detail
    glColor3f(0.05, 0.05, 0.05);
    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(startX + 0.01, 0.06);
    glVertex2f(endX - 0.01, 0.06);
    glVertex2f(endX - 0.01, 0.05 + windowHeight - 0.01);
    glVertex2f(startX + 0.01, 0.05 + windowHeight - 0.01);
    glEnd();

    glPopMatrix();
}

void drawCharacter(float posX, float posY, float scale) {
    glPushMatrix();
    glTranslatef(posX, posY, 0.0);
    glScalef(scale, scale, 1.0);

    // Meningkatkan jumlah titik untuk kurva yang lebih mulus
    float n = 200.0;

    // Menyesuaikan proporsi
    float x_head = 0.0f, y_head = 40.0f, r_head = 20.0f,  // Menaikkan posisi kepala
        x_left = -13.0f, x_right = 13.0f, y_leg = -38.0f, r_leg = 7.0f;  // Menurunkan posisi kaki

    //omagus oxygen (lebih tinggi dan ramping)
    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(-18, 25);    // Dinaikkan
    glVertex2f(-24, 24);    // Dipersempit
    glVertex2f(-26, 23);
    glVertex2f(-27, 22);
    glVertex2f(-28, 13);
    glVertex2f(-29, 0);
    glVertex2f(-28, -15);
    glVertex2f(-27, -25);   // Diperpanjang
    glVertex2f(-26, -26);
    glVertex2f(-25, -27);
    glVertex2f(-24, -28);
    glVertex2f(-18, -28);   // Diperpanjang
    glEnd();

    //omagus_rightleg (lebih panjang)
    glBegin(GL_QUADS);
    glColor3ub(180, 0, 0);
    glVertex2f(6, -30);     // Disesuaikan
    glVertex2f(20, -30);
    glVertex2f(20, -40);    // Diperpanjang
    glVertex2f(6, -40);
    glEnd();

    // Kaki kanan yang lebih mulus
    glColor3ub(180, 0, 0);
    glBegin(GL_POLYGON);
    float a = 2 * M_PI / n;
    float x, y;
    for (int i = 0; i <= n; i++) {
        x = x_right + r_leg * cos(i * a);
        y = y_leg + r_leg * sin(i * a);
        glVertex2f(x, y);
    }
    glEnd();

    //omagus_leftleg (lebih panjang)
    glBegin(GL_QUADS);
    glColor3ub(200, 0, 0);
    glVertex2f(-6, -30);    // Disesuaikan
    glVertex2f(-20, -30);
    glVertex2f(-20, -40);   // Diperpanjang
    glVertex2f(-6, -40);
    glEnd();

    // Kaki kiri yang lebih mulus
    glColor3ub(200, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= n; i++) {
        x = x_left + r_leg * cos(i * a);
        y = y_leg + r_leg * sin(i * a);
        glVertex2f(x, y);
    }
    glEnd();

    //omagus_body (lebih tinggi dan ramping)
    glBegin(GL_POLYGON);
    glColor3ub(255, 0, 0);
    glVertex2f(19, 0);      // Dipersempit
    glVertex2f(19, 40);     // Dinaikkan
    glVertex2f(-19, 40);
    glVertex2f(-21, 0);
    glColor3ub(200, 0, 0);
    glVertex2f(-20, -30);   // Diperpanjang
    glVertex2f(20, -30);   // Diperpanjang
    for (int i = 0; i <= 20; i++) {
        float t = i / 20.0f;
        float x = -19 + 38 * t;
        float y = -30 - sin(t * M_PI) * 5;
        glVertex2f(x, y);
    }
    glEnd();

    // Kepala yang lebih mulus
    glColor3ub(255, 0, 0);
    glBegin(GL_POLYGON);
    for (int i = 0; i <= n; i++) {
        x = x_head + r_head * cos(i * a);
        y = y_head + r_head * sin(i * a);
        glVertex2f(x, y);
    }
    glEnd();

    //omagus_eyes (lebih mulus)
    glColor3ub(0, 134, 179);
    glBegin(GL_POLYGON);
    float eye_x = 5.0f, eye_y = 33.0f, eye_r = 8.0f;  // Dinaikkan posisi mata
    for (int i = 0; i <= n; i++) {
        x = eye_x + eye_r * cos(i * a);
        y = eye_y + eye_r * sin(i * a);
        glVertex2f(x, y);
    }
    glEnd();

    // Visor yang lebih mulus
    glBegin(GL_QUADS);
    glColor3ub(0, 134, 179);
    glVertex2f(3, 41);      // Dinaikkan
    glVertex2f(3, 25);
    glColor3ub(51, 204, 255);
    glVertex2f(19, 25);
    glVertex2f(19, 41);
    glEnd();

    glColor3ub(51, 204, 255);
    glBegin(GL_POLYGON);
    float visor_x = 16.0f, visor_y = 33.0f;  // Dinaikkan
    for (int i = 0; i <= n; i++) {
        x = visor_x + eye_r * cos(i * a);
        y = visor_y + eye_r * sin(i * a);
        glVertex2f(x, y);
    }
    glEnd();

    // Highlight visor yang lebih mulus
    glBegin(GL_QUADS);
    glColor3ub(204, 242, 255);
    glVertex2f(13, 37);     // Dinaikkan
    glVertex2f(21, 37);
    glVertex2f(21, 33);
    glVertex2f(15, 33);
    glEnd();

    glPopMatrix();
}

struct Box {
    float x, y;      // Posisi tengah
    float width;     // Lebar penuh
    float height;    // Tinggi penuh
};

// Mendefinisikan kotak-kotak
Box boxes[] = {
    {-0.4f, -0.35f, 0.2f, 0.2f},  // Kotak kiri
    {0.35f, -0.35f, 0.2f, 0.2f}   // Kotak kanan
};

struct Character {
    float width = 0.1f;   // Lebar karakter (disesuaikan dengan skala)
    float height = 0.16f;  // Tinggi karakter (disesuaikan dengan skala)
};

Character character;

bool checkCollision(float x, float y) {
    // Hitung batas-batas karakter
    float charLeft = x - character.width / 2;
    float charRight = x + character.width / 2;
    float charTop = y + character.height / 2;
    float charBottom = y - character.height / 2;

    // Cek batas dinding
    if (charLeft < minX || charRight > maxX || charBottom < minY || charTop > maxY) {
        return true;
    }

    // Cek tabrakan dengan kotak-kotak
    for (const Box& box : boxes) {
        float boxLeft = box.x - box.width / 2;
        float boxRight = box.x + box.width / 2;
        float boxTop = box.y + box.height / 2;
        float boxBottom = box.y - box.height / 2;

        // Deteksi tabrakan menggunakan AABB (Axis-Aligned Bounding Box)
        if (charRight > boxLeft &&
            charLeft < boxRight &&
            charTop > boxBottom &&
            charBottom < boxTop) {
            return true;
        }
    }

    return false;
}

void handleKeypress(unsigned char key, int x, int y) {
    float moveSpeed = 0.02f;
    float newX = charX;
    float newY = charY;

    switch (key) {
    case 'a':
        newX = charX - moveSpeed;
        break;
    case 'd':
        newX = charX + moveSpeed;
        break;
    case 'w':
        newY = charY + moveSpeed;
        break;
    case 's':
        newY = charY - moveSpeed;
        break;
    case 27:  // ESC
        exit(0);
        break;
    }

    // Cek collision sebelum mengupdate posisi
    if (!checkCollision(newX, newY)) {
        charX = newX;
        charY = newY;
    }

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawDetailedWall();
    drawDetailedFloor();

    // Draw detailed boxes
    drawDetailedBox(-0.4, -0.35, 0.2, 0.2, 0.2, 0.5, 0.2);
    drawDetailedBox(0.35, -0.35, 0.2, 0.2, 0.2, 0.5, 0.2);

    // Draw detailed chairs
    for (float x = -0.7; x <= -0.3; x += 0.1) {
        drawDetailedChair(x, 0.30);
    }
    for (float x = 0.3; x <= 0.7; x += 0.1) {
        drawDetailedChair(x, 0.30);
    }

    // Draw character
    drawCharacter(charX, charY, charScale);

    glutSwapBuffers();
}

void init() {
    glClearColor(0.15, 0.15, 0.15, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glutInitWindowSize(1200, 900);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Enhanced Among Us Room");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeypress);
    glutMainLoop();
    return 0;
}