/*

 programa para mostrar una animacion que simula una tela caendo a un cubo

 creaditos para : https://www.youtube.com/channel/UC6A2B9G_y-fzAXEu2hHPlM
 por sus tutoriales

 $ g++ -lGL -lGLU -lSDL2 animacion.cpp

*/
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL2/SDL.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

std::vector<unsigned int> frames;
int curframe = 0;

struct Coordinate {
    float x, y, z;
    Coordinate(float a, float b, float c) : x(a), y(b), z(c) {};
};

struct Face {
    int facenum;
    bool four;
    int faces[4];
    Face(int facen, int f1, int f2, int f3) : facenum(facen) {
        faces[0] = f1;
        faces[1] = f2;
        faces[2] = f3;
        four = false;
    }
    Face(int facen, int f1, int f2, int f3, int f4) : facenum(facen) {
        faces[0] = f1;
        faces[1] = f2;
        faces[2] = f3;
        faces[3] = f4;
        four = true;
    }
};

unsigned int loadobjet(const char *filename) {
    std::vector<std::string*> coord;
    std::vector<Coordinate*> vertex;
    std::vector<Face*> faces;
    std::vector<Coordinate*> normals;
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cout << "no puede abrirse" << std::endl;
        return -1;
    }
    char buf[256];
    while (!in.eof()) {
        in.getline(buf, 256);
        coord.push_back(new std::string(buf));
    }
    for (int i = 0; i < coord.size(); i++) {
        if ((*coord[i])[0] == '#')
            continue;
        else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {
            char tmp;
            float tmpx, tmpy, tmpz;
            sscanf(coord[i]->c_str(), "%c %f %f %f", &tmp, &tmpx, &tmpy, &tmpz);
            vertex.push_back(new Coordinate(tmpx, tmpy, tmpz));
        }
        else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
            float tmpx, tmpy, tmpz;
            sscanf(coord[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
            normals.push_back(new Coordinate(tmpx, tmpy, tmpz));
        }
        else if ((*coord[i])[0] == 'f') {
            int a, b, c, d, e;
            if (count(coord[i]->begin(), coord[i]->end(), ' ') == 4) {
                sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d",
                       &a, &b, &c, &b, &d, &b, &e, &b);
                faces.push_back(new Face(b, a, c, d, e));
            }
            else {
                sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d",
                       &a, &b, &c, &b, &d, &b);
                faces.push_back(new Face(b, a, c, d));
            }
        }
    }

    // draw
    int num;
    num = glGenLists(1);
    glNewList(num, GL_COMPILE);

    for (int i = 0; i < faces.size(); i++) {
        if (faces[i]->four) {
            glBegin(GL_QUADS);
                glNormal3f(normals[faces[i]->facenum-1]->x,
                           normals[faces[i]->facenum-1]->y,
                           normals[faces[i]->facenum-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[0]-1]->x,
                           vertex[faces[i]->faces[0]-1]->y,
                           vertex[faces[i]->faces[0]-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[1]-1]->x,
                           vertex[faces[i]->faces[1]-1]->y,
                           vertex[faces[i]->faces[1]-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[2]-1]->x,
                           vertex[faces[i]->faces[2]-1]->y,
                           vertex[faces[i]->faces[2]-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[3]-1]->x,
                           vertex[faces[i]->faces[3]-1]->y,
                           vertex[faces[i]->faces[3]-1]->z
                           );
            glEnd();
        }
        else {
            glBegin(GL_TRIANGLES);
                glNormal3f(normals[faces[i]->facenum-1]->x,
                           normals[faces[i]->facenum-1]->y,
                           normals[faces[i]->facenum-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[0]-1]->x,
                           vertex[faces[i]->faces[0]-1]->y,
                           vertex[faces[i]->faces[0]-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[1]-1]->x,
                           vertex[faces[i]->faces[1]-1]->y,
                           vertex[faces[i]->faces[1]-1]->z
                           );
                glVertex3f(vertex[faces[i]->faces[2]-1]->x,
                           vertex[faces[i]->faces[2]-1]->y,
                           vertex[faces[i]->faces[2]-1]->z
                           );
            glEnd();
        }
    }
    glEndList();

    for (int i = 0; i < coord.size(); i++)
        delete coord[i];
    for (int i = 0; i < faces.size(); i++)
        delete faces[i];
    for (int i = 0; i < normals.size(); i++)
        delete normals[i];
    for (int i = 0; i < vertex.size(); i++)
        delete vertex[i];

    return num;
}

void loadAnimation(std::vector<unsigned int>& frames, std::string filename, unsigned int num) {
    char tmp[200];
    for (int i = 1; i <= num; i++) {
        if (i < 10)
            sprintf(tmp, "00000%d", i);
        else if (i < 100)
            sprintf(tmp, "0000%d", i);
        else if (i < 1000)
            sprintf(tmp, "000%d", i);
        else if (i < 10000)
            sprintf(tmp, "00%d", i);
        else if (i < 100000)
            sprintf(tmp, "0%d", i);
        else if (i < 1000000)
            sprintf(tmp, "%d", i);

        std::string tmp2(filename+tmp);
        tmp2 += ".obj";
        std::cout << tmp2 << std::endl;
        unsigned int id = loadobjet(tmp2.c_str());
        frames.push_back(id);
    }
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 640.0/480.0, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_COLOR_MATERIAL);
    loadAnimation(frames, "animacion_", 120);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(25, 0.0, 1.0, 0.0);
    glColor3f(0.4, 0.4, 0.0);
    glCallList(frames[curframe]);
    curframe++;

    if (curframe >= 119)
        curframe = 0;
}

int main(int agrc, char** argv) {
    SDL_Window* window;
    window = SDL_CreateWindow("ANIMACION", 30, 30, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    bool running = true;
    SDL_GL_CreateContext(window);
    init();

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: running = false; break;
            }
        }

        SDL_GL_SwapWindow(window);
        display();
        SDL_Delay(100);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
