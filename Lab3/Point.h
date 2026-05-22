#ifndef POINT_H
#define POINT_H

class Point {
private:
    int x;
    int y;

public:
    Point(int x = 0, int y = 0);
    int getX() const;
    int getY() const;
};

#endif