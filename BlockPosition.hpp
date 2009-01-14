#ifndef BLOCK_POSITION_HPP
#define BLOCK_POSITION_HPP

#include "Block.hpp"

namespace Bastet{

  /** Block position = position (x,y) + orientation -- this is the object that gets evaluated by the block chooser
   */
  class Well;
  
  enum Movement {RotateCW,RotateCCW,Left,Right,Down};

  class BlockPosition{
  private:
    Dot _pos;
    Orientation _orientation;
  public:
    BlockPosition(Dot d=(Dot){3,-2}, Orientation o=Orientation()):_pos(d),_orientation(o){};
    void Move(Movement m);
    bool MoveIfPossible(Movement m, BlockType b, Well *w);

    void Drop(BlockType bt, Well *w);

    const DotMatrix GetDots(BlockType b) const;
    bool IsValid(BlockType bt, Well *w) const;
    bool IsOutOfScreen(BlockType bt) const;

  friend size_t hash_value(const BlockPosition &p){
    return hash_value(p._pos)*4+int(p._orientation);
  }
    
  };
   
}

#endif //BLOCK_POSITION_HPP
