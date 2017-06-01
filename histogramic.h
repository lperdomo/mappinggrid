#ifndef HISTOGRAMIC_H
#define HISTOGRAMIC_H

class Histogramic
{
public:
    Histogramic();
    static int max;
    static int min;
    void setCV(int cv);
    int getCV();
    void addCV();
    void subCV();
    double proportionalCV();
private:
    int cv;
};

#endif // HISTOGRAMIC_H
