#ifndef FALLINGBLOCK_HPP
#define FALLINGBLOCK_HPP

#include "Block.hpp"
#include "Well.hpp"

namespace Bastet{
  
  class FallingBlock{
  public:
    bool RotateCW(const Well &W); //returns false if it could not
    bool RotateCCW(const Well &W);
    bool MoveLeft(const Well &W);
    bool MoveDown(const Well &W); //returns false if it could not
    void Drop(const Well &W); ///move down as much as you can
    FallingBlock(const Block *block, size_t x, size_t y, Orientation orientation);
    ~FallingBlock();
  private:
    Block *_currentBlock;
    size_t _x,_y;
    Orientation _orientation;
  };
}

#endif //FALLINGBLOCK_HPP
