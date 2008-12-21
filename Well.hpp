#include "Block.hpp" //for Color

namespace Bastet{

  class Well{
  private:
    size_t _width,_height;
    Color *_well;
  public:
    Well(size_t width, size_t height);
    ~Well();
    void clear();
    void operator()(size_t int x, size_t y);
    void operator()(Dot p);
    size_t get_width();
    size_t get_height();
  };

}
