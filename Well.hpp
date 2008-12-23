#ifndef WELL_HPP
#define WELL_HPP

#include "Block.hpp" //for Color
#include <cstddef> //size_t

namespace Bastet{

  class Well{
  private:
    size_t _width,_height;
    Color *_well;
  public:
    Well(size_t width, size_t height);
    ~Well();
    void Clear();
    void operator()(int x, int y);
    void operator()(Dot p);
    size_t GetWidth();
    size_t GetHeight();
  };

}

#endif //WELL_HPP
