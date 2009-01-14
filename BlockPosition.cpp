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

  bool BlockPosition::MoveIfPossible(Movement m, BlockType b, Well *w){
    BlockPosition p(*this);
    p.Move(m);
    if (p.IsValid(b,w)){
	*this=p;
	return true;
      }
    else return false;
  }

  
  bool BlockPosition::IsValid(BlockType bt, Well *w) const{
    return w->Accomodates(GetDots(bt)); //XX: must change, unoptimized
  }

  void BlockPosition::Drop(BlockType bt, Well *w){
    while(MoveIfPossible(Down,bt,w));
  }

  bool BlockPosition::IsOutOfScreen(BlockType bt) const{
    BOOST_FOREACH(const Dot &d, GetDots(bt)){
      if(d.y>=0) return false;
    }
    return true;
  }
  
}
