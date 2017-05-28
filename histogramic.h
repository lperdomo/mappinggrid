#ifndef HISTOGRAMIC_H
#define HISTOGRAMIC_H

class Histogramic
{
public:
    Histogramic();
    static int max;
    static int min;
    void addCV(int increment);
    void subCV(int decrement);
private:
    int cv;
};

#endif // HISTOGRAMIC_H
