#include "keyboard.h" 
 
Keyboard *Keyboard::instance = 0; 
 
Keyboard::Keyboard() 
{ 
    arrowUp = false; 
    arrowDown = false; 
    arrowLeft = false; 
    arrowRight = false; 
} 
 
Keyboard::~Keyboard() 
{ 
} 
 
void Keyboard::setArrowUp(bool pressed) 
{ 
    arrowUp = pressed; 
} 
 
bool Keyboard::isArrowUp() 
{ 
    return arrowUp; 
} 
 
void Keyboard::setArrowDown(bool pressed) 
{ 
    arrowDown = pressed; 
} 
 
bool Keyboard::isArrowDown() 
{ 
    return arrowDown; 
} 
 
void Keyboard::setArrowLeft(bool pressed) 
{ 
    arrowLeft = pressed; 
} 
 
bool Keyboard::isArrowLeft() 
{ 
    return arrowLeft; 
} 
 
void Keyboard::setArrowRight(bool pressed) 
{ 
    arrowRight = pressed; 
} 
 
bool Keyboard::isArrowRight() 
{ 
    return arrowRight; 
} 
 
Keyboard *Keyboard::getInstance() 
{ 
    if (!instance) { 
        instance = new Keyboard; 
    } 
    return instance; 
} 
