#ifndef SIZE_H
#define SIZE_H

class Size {
private:
    int width;
    int height;

public:
    Size(int width = 0, int height = 0);
    int getWidth() const;
    int getHeight() const;
};

#endif