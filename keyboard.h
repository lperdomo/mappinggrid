#ifndef KEYBOARD_H 
#define KEYBOARD_H 
 
class Keyboard 
{ 
public: 
    Keyboard(); 
    ~Keyboard(); 
    void setArrowUp(bool pressed); 
    bool isArrowUp(); 
    void setArrowDown(bool pressed); 
    bool isArrowDown(); 
    void setArrowLeft(bool pressed); 
    bool isArrowLeft(); 
    void setArrowRight(bool pressed); 
    bool isArrowRight(); 
    void setQ(bool pressed);
    bool isQ();
    void setC(bool pressed);
    bool isC();
    static Keyboard *getInstance();
    static Keyboard *instance; 
private: 
    bool arrowUp; 
    bool arrowDown; 
    bool arrowLeft; 
    bool arrowRight;
    bool q;
    bool c;
}; 
 
#endif // KEYBOARD_H 
