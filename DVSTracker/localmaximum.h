#ifndef LOCALMAXIMUM_H
#define LOCALMAXIMUM_H

class LocalMaximum
{
public:
    LocalMaximum(int posX = 0, int posY = 0, int w = 0);
    void set(int posX, int posY, int w);
    int x;
    int y;
    int weight;
};

#endif // LOCALMAXIMUM_H
