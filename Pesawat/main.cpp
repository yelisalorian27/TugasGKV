#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#include <math.h>
#include <stdio.h>


// Variabel untuk rotasi dan posisi pesawat
float angleX = 0.0f;
float angleY = 0.0f;
float angleZ = 0.0f;
float posX = 0.0f;
float posY = 0.0f;
float posZ = -10.0f; // Mulai dengan posisi yang lebih jauh agar terlihat seluruh pesawat

// Variabel untuk proyeksi
int projectionMode = 0; // 0: perspective, 1: orthographic
int orthoView = 0;      // 0: front, 1: top, 2: side, 3: isometric

// Variabel untuk kecepatan rotasi dan gerakan
float rotationSpeed = 1.0f;
float moveSpeed = 0.1f;

// Variabel untuk interaksi mouse
int lastX = 0, lastY = 0;
bool mouseDown = false;

// Fungsi untuk menggambar pesawat yang lebih realistis
void drawAirplane() {
    // Menyimpan matriks saat ini
    glPushMatrix();
    
    // Terapkan transformasi
    glTranslatef(posX, posY, posZ);
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
    
    // BADAN PESAWAT
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.95f); // Warna putih sedikit keabu-abuan untuk badan pesawat
    GLUquadricObj *cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    
    // Badan pesawat utama - lebih panjang
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.4f, 0.4f, 3.0f, 32, 32); // Badan utama: radius 0.4, panjang 3.0
    
    // Hidung pesawat yang lebih halus
    glTranslatef(0.0f, 0.0f, 3.0f);
    gluCylinder(cylinder, 0.4f, 0.1f, 0.8f, 32, 32); // Kerucut lebih halus
    glPopMatrix();
    
    // EKOR PESAWAT
    glPushMatrix();
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.9f, 0.9f, 0.95f);
    
    // Bagian ekor (menyempit)
    glTranslatef(0.0f, 0.0f, -1.0f);
    gluCylinder(cylinder, 0.3f, 0.4f, 1.0f, 32, 32); // Ekor menyempit dari depan ke belakang
    glPopMatrix();
    
    // SAYAP UTAMA - desain yang lebih aerodinamis
    glPushMatrix();
    glColor3f(0.85f, 0.85f, 0.9f); // Warna sayap
    
    // Sayap kiri
    glBegin(GL_POLYGON);
    // Bagian depan sayap
    glVertex3f(0.0f, -0.05f, 0.5f);  // Titik penyambungan depan badan
    glVertex3f(-3.0f, 0.0f, 0.2f);   // Ujung depan sayap
    // Bagian belakang sayap
    glVertex3f(-2.8f, 0.0f, 1.2f);   // Ujung belakang sayap
    glVertex3f(-1.0f, -0.05f, 1.3f); // Bagian tengah belakang
    glVertex3f(0.0f, -0.05f, 1.2f);  // Titik penyambungan belakang badan
    glEnd();
    
    // Bagian bawah sayap kiri
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, -0.15f, 0.5f);
    glVertex3f(-3.0f, -0.1f, 0.2f);
    glVertex3f(-2.8f, -0.1f, 1.2f);
    glVertex3f(-1.0f, -0.15f, 1.3f);
    glVertex3f(0.0f, -0.15f, 1.2f);
    glEnd();
    
    // Sisi sayap kiri
    glBegin(GL_POLYGON);
    glVertex3f(-3.0f, 0.0f, 0.2f);
    glVertex3f(-3.0f, -0.1f, 0.2f);
    glVertex3f(-2.8f, -0.1f, 1.2f);
    glVertex3f(-2.8f, 0.0f, 1.2f);
    glEnd();
    
    // Sayap kanan (cerminan dari sayap kiri)
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, -0.05f, 0.5f);
    glVertex3f(3.0f, 0.0f, 0.2f);
    glVertex3f(2.8f, 0.0f, 1.2f);
    glVertex3f(1.0f, -0.05f, 1.3f);
    glVertex3f(0.0f, -0.05f, 1.2f);
    glEnd();
    
    // Bagian bawah sayap kanan
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, -0.15f, 0.5f);
    glVertex3f(3.0f, -0.1f, 0.2f);
    glVertex3f(2.8f, -0.1f, 1.2f);
    glVertex3f(1.0f, -0.15f, 1.3f);
    glVertex3f(0.0f, -0.15f, 1.2f);
    glEnd();
    
    // Sisi sayap kanan
    glBegin(GL_POLYGON);
    glVertex3f(3.0f, 0.0f, 0.2f);
    glVertex3f(3.0f, -0.1f, 0.2f);
    glVertex3f(2.8f, -0.1f, 1.2f);
    glVertex3f(2.8f, 0.0f, 1.2f);
    glEnd();
    glPopMatrix();
    
    // EKOR VERTIKAL
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.95f);
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, 0.3f, -0.8f);  // Base of fin
    glVertex3f(0.0f, 1.2f, -0.6f);  // Top front of fin
    glVertex3f(0.0f, 1.2f, -1.2f);  // Top back of fin
    glVertex3f(0.0f, 0.3f, -1.3f);  // Back of fin base
    glEnd();
    
    // Sisi lain ekor vertikal
    glBegin(GL_POLYGON);
    glVertex3f(0.02f, 0.3f, -0.8f);
    glVertex3f(0.02f, 1.2f, -0.6f);
    glVertex3f(0.02f, 1.2f, -1.2f);
    glVertex3f(0.02f, 0.3f, -1.3f);
    glEnd();
    
    // Tambahkan ketebalan
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, 1.2f, -0.6f);
    glVertex3f(0.02f, 1.2f, -0.6f);
    glVertex3f(0.02f, 1.2f, -1.2f);
    glVertex3f(0.0f, 1.2f, -1.2f);
    glEnd();
    glPopMatrix();
    
    // STABILIZER HORIZONTAL (EKOR HORIZONTAL)
    glPushMatrix();
    glColor3f(0.85f, 0.85f, 0.9f);
    
    // Sayap ekor kiri
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, 0.2f, -0.9f);
    glVertex3f(-1.5f, 0.2f, -1.1f);
    glVertex3f(-1.3f, 0.2f, -1.5f);
    glVertex3f(0.0f, 0.2f, -1.3f);
    glEnd();
    
    // Sayap ekor kanan
    glBegin(GL_POLYGON);
    glVertex3f(0.0f, 0.2f, -0.9f);
    glVertex3f(1.5f, 0.2f, -1.1f);
    glVertex3f(1.3f, 0.2f, -1.5f);
    glVertex3f(0.0f, 0.2f, -1.3f);
    glEnd();
    
    // Tambahkan ketebalan untuk sayap ekor
    glBegin(GL_POLYGON);
    glVertex3f(-1.5f, 0.18f, -1.1f);
    glVertex3f(-1.5f, 0.22f, -1.1f);
    glVertex3f(-1.3f, 0.22f, -1.5f);
    glVertex3f(-1.3f, 0.18f, -1.5f);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(1.5f, 0.18f, -1.1f);
    glVertex3f(1.5f, 0.22f, -1.1f);
    glVertex3f(1.3f, 0.22f, -1.5f);
    glVertex3f(1.3f, 0.18f, -1.5f);
    glEnd();
    glPopMatrix();
    
    // KOKPIT PESAWAT - Lebih halus dan realistis
    glPushMatrix();
    glColor3f(0.1f, 0.3f, 0.6f); // Biru untuk kaca
    
    // Windshield kokpit
    glBegin(GL_POLYGON);
    glVertex3f(-0.25f, 0.35f, 2.1f);
    glVertex3f(0.25f, 0.35f, 2.1f);
    glVertex3f(0.3f, 0.5f, 1.5f);
    glVertex3f(-0.3f, 0.5f, 1.5f);
    glEnd();
    
    // Sisi kokpit
    glBegin(GL_POLYGON);
    glVertex3f(-0.25f, 0.35f, 2.1f);
    glVertex3f(-0.3f, 0.5f, 1.5f);
    glVertex3f(-0.35f, 0.4f, 1.0f);
    glVertex3f(-0.25f, 0.25f, 1.6f);
    glEnd();
    
    glBegin(GL_POLYGON);
    glVertex3f(0.25f, 0.35f, 2.1f);
    glVertex3f(0.3f, 0.5f, 1.5f);
    glVertex3f(0.35f, 0.4f, 1.0f);
    glVertex3f(0.25f, 0.25f, 1.6f);
    glEnd();
    glPopMatrix();
    
    // MESIN PESAWAT - Lebih realistis
    glPushMatrix();
    glColor3f(0.6f, 0.6f, 0.65f);
    
    // Mesin sayap kiri
    glPushMatrix();
    glTranslatef(-1.5f, -0.3f, 0.5f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.25f, 0.25f, 1.0f, 20, 20);
    
    // Nacelle (penutup mesin) bagian depan
    glColor3f(0.4f, 0.4f, 0.4f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.0f, 0.25f, 20, 1);
    
    // Fan mesin di depan
    glColor3f(0.7f, 0.7f, 0.75f);
    glTranslatef(0.0f, 0.0f, -0.02f);
    gluDisk(cylinder, 0.05f, 0.23f, 20, 4);
    
    // Nacelle bagian belakang
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(0.0f, 0.0f, 1.02f);
    gluCylinder(cylinder, 0.25f, 0.2f, 0.2f, 20, 2);
    glPopMatrix();
    
    // Mesin sayap kanan
    glPushMatrix();
    glTranslatef(1.5f, -0.3f, 0.5f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glColor3f(0.6f, 0.6f, 0.65f);
    gluCylinder(cylinder, 0.25f, 0.25f, 1.0f, 20, 20);
    
    // Nacelle bagian depan
    glColor3f(0.4f, 0.4f, 0.4f);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.0f, 0.25f, 20, 1);
    
    // Fan mesin di depan
    glColor3f(0.7f, 0.7f, 0.75f);
    glTranslatef(0.0f, 0.0f, -0.02f);
    gluDisk(cylinder, 0.05f, 0.23f, 20, 4);
    
    // Nacelle bagian belakang
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslatef(0.0f, 0.0f, 1.02f);
    gluCylinder(cylinder, 0.25f, 0.2f, 0.2f, 20, 2);
    glPopMatrix();
    glPopMatrix();
    
    // RODA PESAWAT
    // Roda depan
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
    glTranslatef(0.0f, -0.6f, 1.7f);
    
    // Penyangga roda depan
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.4f, 0.0f);
    glEnd();
    
    // Poros roda
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_POLYGON);
    glVertex3f(-0.1f, -0.05f, 0.05f);
    glVertex3f(0.1f, -0.05f, 0.05f);
    glVertex3f(0.1f, -0.05f, -0.05f);
    glVertex3f(-0.1f, -0.05f, -0.05f);
    glEnd();
    
    // Roda kiri dan kanan
    glColor3f(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(-0.1f, -0.05f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.12f, 0.12f, 0.05f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.04f, 0.12f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.05f);
    gluDisk(cylinder, 0.04f, 0.12f, 16, 2);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0.05f, -0.05f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.12f, 0.12f, 0.05f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.04f, 0.12f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.05f);
    gluDisk(cylinder, 0.04f, 0.12f, 16, 2);
    glPopMatrix();
    glPopMatrix();
    
    // Roda utama (main landing gear)
    glPushMatrix();
    glTranslatef(0.0f, -0.6f, 0.2f);
    
    // Sistem roda utama kiri
    glPushMatrix();
    glTranslatef(-0.7f, 0.0f, 0.0f);
    
    // Penyangga roda utama
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();
    
    // Poros roda
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_POLYGON);
    glVertex3f(-0.15f, -0.05f, 0.1f);
    glVertex3f(0.15f, -0.05f, 0.1f);
    glVertex3f(0.15f, -0.05f, -0.1f);
    glVertex3f(-0.15f, -0.05f, -0.1f);
    glEnd();
    
    // Dua roda di masing-masing sisi
    glColor3f(0.1f, 0.1f, 0.1f);
    // Roda depan
    glPushMatrix();
    glTranslatef(-0.12f, -0.05f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.15f, 0.15f, 0.08f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.08f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glPopMatrix();
    
    // Roda belakang
    glPushMatrix();
    glTranslatef(0.12f, -0.05f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.15f, 0.15f, 0.08f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.08f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glPopMatrix();
    glPopMatrix();
    
    // Sistem roda utama kanan
    glPushMatrix();
    glTranslatef(0.7f, 0.0f, 0.0f);
    
    // Penyangga roda utama
    glColor3f(0.5f, 0.5f, 0.5f);
    glBegin(GL_LINES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 0.5f, 0.0f);
    glEnd();
    
    // Poros roda
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_POLYGON);
    glVertex3f(-0.15f, -0.05f, 0.1f);
    glVertex3f(0.15f, -0.05f, 0.1f);
    glVertex3f(0.15f, -0.05f, -0.1f);
    glVertex3f(-0.15f, -0.05f, -0.1f);
    glEnd();
    
    // Dua roda di masing-masing sisi
    glColor3f(0.1f, 0.1f, 0.1f);
    // Roda depan
    glPushMatrix();
    glTranslatef(-0.12f, -0.05f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.15f, 0.15f, 0.08f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.08f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glPopMatrix();
    
    // Roda belakang
    glPushMatrix();
    glTranslatef(0.12f, -0.05f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    gluCylinder(cylinder, 0.15f, 0.15f, 0.08f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.0f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glTranslatef(0.0f, 0.0f, 0.08f);
    gluDisk(cylinder, 0.05f, 0.15f, 16, 2);
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
    
    // DEKORASI PESAWAT - Garis strip di samping badan pesawat
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.8f); // Warna biru untuk dekorasi
    
    // Garis pada badan pesawat kiri
    glBegin(GL_POLYGON);
    glVertex3f(-0.41f, 0.0f, 2.0f);
    glVertex3f(-0.41f, -0.2f, 2.0f);
    glVertex3f(-0.41f, -0.2f, -1.0f);
    glVertex3f(-0.41f, 0.0f, -1.0f);
    glEnd();
    
    // Garis pada badan pesawat kanan
    glBegin(GL_POLYGON);
    glVertex3f(0.41f, 0.0f, 2.0f);
    glVertex3f(0.41f, -0.2f, 2.0f);
    glVertex3f(0.41f, -0.2f, -1.0f);
    glVertex3f(0.41f, 0.0f, -1.0f);
    glEnd();
    
    // Logo pada ekor
    glColor3f(0.9f, 0.1f, 0.1f); // Warna merah untuk logo
    glBegin(GL_POLYGON);
    glVertex3f(-0.02f, 0.4f, -0.9f);
    glVertex3f(0.02f, 0.4f, -0.9f);
    glVertex3f(0.02f, 1.0f, -0.7f);
    glVertex3f(-0.02f, 1.0f, -0.7f);
    glEnd();
    glPopMatrix();
    
    // Jendela pesawat - deretan jendela kecil di sepanjang badan pesawat
    glPushMatrix();
    glColor3f(0.1f, 0.3f, 0.6f); // Biru untuk jendela
    
    // Jendela sisi kiri
    for (float z = 1.3f; z >= -0.7f; z -= 0.3f) {
        glPushMatrix();
        glTranslatef(-0.402f, 0.1f, z);
        glScalef(0.01f, 0.1f, 0.15f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    
    // Jendela sisi kanan
    for (float z = 1.3f; z >= -0.7f; z -= 0.3f) {
        glPushMatrix();
        glTranslatef(0.402f, 0.1f, z);
        glScalef(0.01f, 0.1f, 0.15f);
        glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPopMatrix();
    
    // Pintu pesawat
    glPushMatrix();
    glColor3f(0.7f, 0.7f, 0.75f);
    // Pintu depan kiri
    glBegin(GL_POLYGON);
    glVertex3f(-0.402f, 0.2f, 1.0f);
    glVertex3f(-0.402f, -0.3f, 1.0f);
    glVertex3f(-0.402f, -0.3f, 0.7f);
    glVertex3f(-0.402f, 0.2f, 0.7f);
    glEnd();
    
    // Pintu depan kanan
    glBegin(GL_POLYGON);
    glVertex3f(0.402f, 0.2f, 1.0f);
    glVertex3f(0.402f, -0.3f, 1.0f);
    glVertex3f(0.402f, -0.3f, 0.7f);
    glVertex3f(0.402f, 0.2f, 0.7f);
    glEnd();
    
    // Pintu belakang kiri
    glBegin(GL_POLYGON);
    glVertex3f(-0.402f, 0.2f, -0.2f);
    glVertex3f(-0.402f, -0.3f, -0.2f);
    glVertex3f(-0.402f, -0.3f, -0.5f);
    glVertex3f(-0.402f, 0.2f, -0.5f);
    glEnd();
    
    // Pintu belakang kanan
    glBegin(GL_POLYGON);
    glVertex3f(0.402f, 0.2f, -0.2f);
    glVertex3f(0.402f, -0.3f, -0.2f);
    glVertex3f(0.402f, -0.3f, -0.5f);
    glVertex3f(0.402f, 0.2f, -0.5f);
    glEnd();
    
    // Gagang pintu
    glColor3f(0.3f, 0.3f, 0.3f);
    // Gagang pintu depan kiri
    glPushMatrix();
    glTranslatef(-0.403f, -0.1f, 0.85f);
    glScalef(0.01f, 0.05f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Gagang pintu depan kanan
    glPushMatrix();
    glTranslatef(0.403f, -0.1f, 0.85f);
    glScalef(0.01f, 0.05f, 0.02f);
    glutSolidCube(1.0f);
    glPopMatrix();
    
    // Gagang pintu belakang kiri
    glPushMatrix();
    glTranslatef(-0.403f, -0.1f, -0.35f);
    glScalef(0.01f, 0.05f, 0.02f);
    glutSolidCube(1.0);
    
    // Complete the remaining code from where it was cut off
        glutSolidCube(1.0f);
        glPopMatrix();
        
        // Gagang pintu belakang kanan
        glPushMatrix();
        glTranslatef(0.403f, -0.1f, -0.35f);
        glScalef(0.01f, 0.05f, 0.02f);
        glutSolidCube(1.0f);
        glPopMatrix();
        glPopMatrix();
        
        // Free quadric object
        gluDeleteQuadric(cylinder);
        
        // Restore the original matrix
        glPopMatrix();
    }
    
    // Function to display the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    if (projectionMode == 0) {
        // Perspective mode
        // Hilangkan gluLookAt tetap di sini untuk memungkinkan posisi dan rotasi bekerja
        
        // Terapkan transformasi berdasarkan input keyboard
        glTranslatef(posX, posY, posZ);
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
    } else {
        // Orthographic mode
        switch (orthoView) {
            case 0: // Front view
                gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
                break;
            case 1: // Top view
                gluLookAt(0, 10, 0, 0, 0, 0, 0, 0, -1);
                break;
            case 2: // Side view
                gluLookAt(10, 0, 0, 0, 0, 0, 0, 1, 0);
                break;
            case 3: // Isometric view
                gluLookAt(8, 8, 8, 0, 0, 0, 0, 1, 0);
                break;
        }
        
        glTranslatef(posX, posY, posZ);
    }
    
    drawAirplane();
    
    glutSwapBuffers();
}

    // Function to reshape the window
void reshape(int w, int h) {
    float aspectRatio = (float)w / (float)h;
    
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if (projectionMode == 0) {
        // Perspective projection
        gluPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
    } else {
        // Orthographic projection - DIPERBAIKI ukurannya
        float size = 5.0f; // Ukuran lebih kecil agar objek terlihat lebih besar
        
        switch (orthoView) {
            case 0: // Front view
                glOrtho(-size * aspectRatio, size * aspectRatio, -size, size, -100.0f, 100.0f);
                break;
            case 1: // Top view
                glOrtho(-size * aspectRatio, size * aspectRatio, -size, size, -100.0f, 100.0f);
                break;
            case 2: // Side view
                glOrtho(-size * aspectRatio, size * aspectRatio, -size, size, -100.0f, 100.0f);
                break;
            case 3: // Isometric view
                glOrtho(-size * aspectRatio, size * aspectRatio, -size, size, -100.0f, 100.0f);
                break;
            default:
                glOrtho(-size * aspectRatio, size * aspectRatio, -size, size, -100.0f, 100.0f);
        }
    }
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

    // Function to handle keyboard input
// Function to handle keyboard input
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Move forward
            posZ += moveSpeed;
            break;
        case 's': // Move backward
            posZ -= moveSpeed;
            break;
        case 'a': // Move left
            posX -= moveSpeed;
            break;
        case 'd': // Move right
            posX += moveSpeed;
            break;
        case 'q': // Move up
            posY += moveSpeed;
            break;
        case 'e': // Move down
            posY -= moveSpeed;
            break;
        case 'x': // Rotate around X-axis
            angleX += rotationSpeed;
            break;
        case 'y': // Rotate around Y-axis
            angleY += rotationSpeed;
            break;
        case 'z': // Rotate around Z-axis
            angleZ += rotationSpeed;
            break;
        case 'p': // Toggle projection mode (perspective/orthographic)
            projectionMode = 1 - projectionMode;
            glutPostRedisplay();
            break;
        case 'v': // Cycle through orthographic views
            if (projectionMode == 1) {
                orthoView = (orthoView + 1) % 4;
                glutPostRedisplay();
            }
            break;
        case '1': // Front orthographic view
            projectionMode = 1;
            orthoView = 0;
            // Reset posisi dan rotasi
            posX = 0.0f; posY = 0.0f; posZ = -10.0f;
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            break;
        case '2': // Top orthographic view - DIPERBAIKI
            projectionMode = 1;
            orthoView = 1;
            posX = 0.0f; posY = 0.0f; posZ = 0.0f; // Ubah posZ ke 0
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            glutPostRedisplay();
            break;
        case '3': // Side orthographic view - DIPERBAIKI
            projectionMode = 1;
            orthoView = 2;
            posX = 0.0f; posY = 0.0f; posZ = 0.0f; // Ubah posZ ke 0
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            glutPostRedisplay();
            break;
        case '4': // Isometric orthographic view
            projectionMode = 1;
            orthoView = 3;
            posX = 0.0f; posY = 0.0f; posZ = 0.0f; // Change to 0.0f instead of -10.0f
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            glutPostRedisplay();
            break;
        case '5': // Dimetric view
            projectionMode = 0;
            posX = 0.0f; posY = 0.0f; posZ = -10.0f;
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            break;
        case '6': // Trimetric view - front close-up
            projectionMode = 0;
            posX = 0.0f; posY = 0.0f; posZ = -5.0f;
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            break;
        case '7': // 1 Titik Hilang
            projectionMode = 0;
            posX = 2.0f; posY = 1.0f; posZ = -8.0f;
            angleX = 15.0f; angleY = 30.0f; angleZ = 0.0f;
            break;
        case '8': // 2 Titik Hilang
            projectionMode = 0;
            posX = 5.0f; posY = 0.0f; posZ = -10.0f;
            angleX = 0.0f; angleY = 90.0f; angleZ = 0.0f;
            break;
        case '9': // Perspektif 3 titik hilang
            projectionMode = 0;
            posX = 0.0f;
            posY = 0.0f;
            posZ = -10.0f;
            angleX = 20.0f;
            angleY = 30.0f;
            angleZ = 10.0f;
            glutPostRedisplay();
            break;
        case '0': // Reset to default view
            projectionMode = 0;
            posX = 0.0f; posY = 0.0f; posZ = -10.0f;
            angleX = 0.0f; angleY = 0.0f; angleZ = 0.0f;
            break;
        case 27: // ESC key - exit
            exit(0);
            break;
    }
    glutPostRedisplay();
}

    // Mouse event handling
    void mouse(int button, int state, int x, int y) {
        if (button == GLUT_LEFT_BUTTON) {
            if (state == GLUT_DOWN) {
                mouseDown = true;
                lastX = x;
                lastY = y;
            } else {
                mouseDown = false;
            }
        }
    }

    // Mouse motion handling
    void motion(int x, int y) {
        if (mouseDown) {
            angleY += (x - lastX) * 0.1f;
            angleX += (y - lastY) * 0.1f;
            lastX = x;
            lastY = y;
            glutPostRedisplay();
        }
    }
    
    // Main function
    int main(int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
        glutInitWindowSize(800, 600);
        glutCreateWindow("3D Airplane Model");
        
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f); // Sky blue background
        
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutMouseFunc(mouse);
        glutMotionFunc(motion);
        
        glutMainLoop();
        return 0;
    }
