/*
    Bastet - tetris clone with embedded bastard block chooser
    (c) 2005-2009 Federico Poloni <f.polonithirtyseven@sns.it> minus 37

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    bool operator==(const BlockPosition &p) const{
      return _pos==p._pos && _orientation==p._orientation;
    }
    ///returns an y such that the block lies completely in [y,y+3]
    int GetBaseY() const{return _pos.y;}
    void Move(Movement m);
    bool MoveIfPossible(Movement m, BlockType b, const Well *w);

    void Drop(BlockType bt, const Well *w);

    const DotMatrix GetDots(BlockType b) const;
    bool IsValid(BlockType bt, const Well *w) const;
    bool IsOutOfScreen(BlockType bt) const;

  friend size_t hash_value(const BlockPosition &p){
    return hash_value(p._pos)*4+int(p._orientation);
  }
    
  };
   
}

#endif //BLOCK_POSITION_HPP
