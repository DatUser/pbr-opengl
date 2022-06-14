#include "cube.hh"


Cube::Cube(Point3& center, float size)
: center(center),
  size(size),
  vertices(std::vector<float>{})
{
}

/*
** x: left (->)
** y: up
** z: away from you
*/
void Cube::generate()
{
  //POS
  //NORMAL
  //COLOR
  vertices = {
    size, size,-size, //----BACK
    0.0, 0.0, 1.0,//
    1.0, 1.0, 1.0,//
    -size,-size,-size,//
    0.0, 0.0, 1.0,//
    1.0, 1.0, 1.0,//
    -size, size,-size,//
    0.0, 0.0, 1.0,//
    1.0, 1.0, 1.0,//
    size, size,-size,
    0.0, 0.0, 1.0,//
    1.0, 1.0, 1.0,//
    size,-size,-size,
    0.0, 0.0, 1.0,//
    1.0, 1.0, 1.0,//
    -size,-size,-size,
    0.0, 0.0, 1.0,//----
    1.0, 1.0, 1.0,//
    -size,-size,-size,//----LEFT
    1.0, 0.0, 0.0,//
    1.0, 0.0, 0.0,//
    -size,-size, size,//
    1.0, 0.0, 0.0,//
    1.0, 0.0, 0.0,//
    -size, size, size,//
    1.0, 0.0, 0.0,//
    1.0, 0.0, 0.0,//
    -size,-size,-size,
    1.0, 0.0, 0.0,//
    1.0, 0.0, 0.0,//
    -size, size, size,
    1.0, 0.0, 0.0,//
    1.0, 0.0, 0.0,//
    -size, size,-size,//
    1.0, 0.0, 0.0,//
    1.0, 0.0, 0.0,//----
    size,-size, size,//----DOWN
    0.0, 1.0, 0.0,//
    1.0, 1.0, 1.0,//
    -size,-size,-size,
    0.0, 1.0, 0.0,//
    1.0, 1.0, 1.0,//
    size,-size,-size,
    0.0, 1.0, 0.0,//
    1.0, 1.0, 1.0,//
    size,-size, size,
    0.0, 1.0, 0.0,//
    1.0, 1.0, 1.0,//
    -size,-size, size,
    0.0, 1.0, 0.0,//
    1.0, 1.0, 1.0,//
    -size,-size,-size,
    0.0, 1.0, 0.0,//----
    1.0, 1.0, 1.0,//
    size, size, size,//---RIGHT
    -1.0, 0.0, 0.0,//
    0.0, 1.0, 0.0,//
    size,-size,-size,
    -1.0, 0.0, 0.0,//
    0.0, 1.0, 0.0,//
    size, size,-size,
    -1.0, 0.0, 0.0,//
    0.0, 1.0, 0.0,//
    size,-size,-size,
    -1.0, 0.0, 0.0,//
    0.0, 1.0, 0.0,//
    size, size, size,
    -1.0, 0.0, 0.0,//
    0.0, 1.0, 0.0,//
    size,-size, size,
    -1.0, 0.0, 0.0,//
    0.0, 1.0, 0.0,//----
    size, size, size,//----UP
    0.0, -1.0, 0.0,//
    1.0, 1.0, 1.0,//
    size, size,-size,
    0.0, -1.0, 0.0,//
    1.0, 1.0, 1.0,//
    -size, size,-size,
    0.0, -1.0, 0.0,//
    1.0, 1.0, 1.0,//
    size, size, size,
    0.0, -1.0, 0.0,//
    1.0, 1.0, 1.0,//
    -size, size,-size,
    0.0, -1.0, 0.0,//
    1.0, 1.0, 1.0,//
    -size, size, size,
    0.0, -1.0, 0.0,//
    1.0, 1.0, 1.0//----
    //size, size, size,//----FRONT(currently not displayed)
    //0.0, 0.0, -1.0,//
    //1.0, 0.0, 0.0,//
    //-size, size, size,
    //0.0, 0.0, -1.0,//
    //1.0, 0.0, 0.0,//
    //size,-size, size,
    //0.0, 0.0, -1.0,//
    //1.0, 0.0, 0.0,//
    //-size, size, size,//
    //0.0, 0.0, -1.0,//
    //1.0, 1.0, 0.0,//
    //-size,-size, size,
    //0.0, 0.0, -1.0,//
    //1.0, 1.0, 0.0,//
    //size,-size, size,
    //0.0, 0.0, -1.0,//
    //1.0, 1.0, 0.0//----
  };
}
