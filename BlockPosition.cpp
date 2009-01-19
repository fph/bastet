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

#include "BlockPosition.hpp"

#include "Block.hpp"
#include "Well.hpp"
#include "boost/foreach.hpp"

namespace Bastet{
  const DotMatrix BlockPosition::GetDots(BlockType b) const{
    return _pos+((blocks[b].GetOrientationMatrix())[_orientation]);
  }

  void BlockPosition::Move(Movement m){
    switch(m){
    case RotateCW:
      _orientation=_orientation.Next();
      break;
    case RotateCCW:
      _orientation=_orientation.Prior();
      break;
    case Left:
      _pos.x-=1;
      break;
    case Right:
      _pos.x+=1;
      break;
    case Down:
      _pos.y+=1;
      break;
    }
  }

  bool BlockPosition::MoveIfPossible(Movement m, BlockType b, const Well *w){
    BlockPosition p(*this);
    p.Move(m);
    if (p.IsValid(b,w)){
	*this=p;
	return true;
      }
    else return false;
  }

  
  bool BlockPosition::IsValid(BlockType bt, const Well *w) const{
    return w->Accomodates(GetDots(bt)); //XX: must change, unoptimized
  }

  void BlockPosition::Drop(BlockType bt, const Well *w){
    while(MoveIfPossible(Down,bt,w));
  }

  bool BlockPosition::IsOutOfScreen(BlockType bt) const{
    BOOST_FOREACH(const Dot &d, GetDots(bt)){
      if(d.y>=0) return false;
    }
    return true;
  }
  
}
