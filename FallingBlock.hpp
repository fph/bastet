#ifndef FALLINGBLOCK_HPP
#define FALLINGBLOCK_HPP

#include "Block.hpp"
#include "Well.hpp"

namespace Bastet{
  class Well; //forward
  
  class FallingBlock{
  public:
    DotMatrix GetMatrix() const;
    bool RotateCW(const Well &w); //returns false if it could not
    bool RotateCCW(const Well &w);
    bool MoveLeft(const Well &w);
    bool MoveRight(const Well &w);
    bool MoveDown(const Well &w); //returns false if it could not
    int HardDrop(const Well &w); ///move down as much as you can, returns number of times it went down
    FallingBlock(BlockType _block, const Well &w, const Dot &pos=(Dot){3,-2}, Orientation orientation=Orientation());
    ~FallingBlock();
    Color GetColor() const;
    bool IsOutOfScreen() const; //to check for game over
  private:
    BlockType _block;
    Dot _pos; //position
    Orientation _orientation;
  };
}

#endif //FALLINGBLOCK_HPP
