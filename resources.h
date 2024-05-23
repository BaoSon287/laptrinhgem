#ifndef RESPROPERTY_H
#define RESPROPERTY_H

#include "common.h"

struct resources {
    int id;
    float weight;
    int score;
    SDL_Point position; // Vị trí của tài nguyên
    int width; // Chiều rộng của tài nguyên
    int height; // Chiều cao của tài nguyên
    bool isCollected;
    resources(int resId = -1);
    void setId(int resId);
    void setDimensions(int w, int h); // Phương thức để thiết lập kích thước
};
void resources::setId(int resId) {
    id = resId;
    switch(id) {
        case ID_GOLD100:
            weight = 2;
            score = 100;
            setDimensions(50, 50); // Thiết lập kích thước cho mỗi loại tài nguyên
            break;
        case ID_GOLD250:
            weight = 3;
            score = 250;
            setDimensions(40, 40);
            break;
        case ID_GOLD500:
            weight = 5;
            score = 500;
            setDimensions(50, 50);
            break;
        case ID_DIAMOND:
            weight = 1;
            score = 5000;
            setDimensions(20, 20);
            break;
        case ID_BOMB:
            weight = 0;
            score = -500;
            setDimensions(80, 80);
            break;
        case ID_ROCK10:
            weight = 19;
            score = 10;
            setDimensions(20, 20);
            break;
        case ID_ROCK20:
            weight = 15;
            score = 20;
            setDimensions(30, 30);
            break;
        case -1:
            weight = 0;
            score = 0;
            setDimensions(0, 0); // Kích thước mặc định cho trường hợp không có tài nguyên
            break;
    }
}

resources::resources(int resId) {
    this->setId(resId);
}
void resources::setDimensions(int w, int h) {
    width = w;
    height = h;
}
#endif
