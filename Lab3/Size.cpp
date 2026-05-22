#include "Size.h"

Size::Size(int width, int height) : width(width), height(height) {}

int Size::getWidth() const { return width; }
int Size::getHeight() const { return height; }