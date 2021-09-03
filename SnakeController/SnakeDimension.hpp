#pragma once

namespace Snake
{

struct Dimension
{
    Dimension(int x, int y){this->x = x; this->y = y;};
    int x;
    int y;

    bool operator==(Dimension const& rhs) const { return x == rhs.x and y == rhs.y; }
};

} // namespace Snake
