#include "FallingBlock.hpp"
#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>

namespace Bastet{
  FallingBlock::FallingBlock(BlockType block, const Well &w, const Dot &pos, Orientation orientation):
    _block(block),_pos(pos),_orientation(orientation){
    
    if(!w.Accomodates(GetMatrix()))
      throw(GameOver());
  };
  
  FallingBlock::~FallingBlock(){};
  
  DotMatrix FallingBlock::GetMatrix() const{
    return blocks[_block].GetDots(_pos,_orientation);
  }

  bool FallingBlock::RotateCW(const Well &w){
    Orientation o=_orientation.Next();
    if(!w.Accomodates(blocks[_block].GetDots(_pos,o)))
      return false;
    _orientation=o;
    return true;
  }
  
  bool FallingBlock::RotateCCW(const Well &w){
    Orientation o=_orientation.Prior();
    if(!w.Accomodates(blocks[_block].GetDots(_pos,o)))
      return false;
    _orientation=o;
    return true;
  }
  
  bool FallingBlock::MoveLeft(const Well &w){
    Dot newpos(_pos);
    newpos.x-=1;
    if(!w.Accomodates(blocks[_block].GetDots(newpos,_orientation)))
      return false;
    _pos=newpos;
    return true;
  }

  bool FallingBlock::MoveRight(const Well &w){
    Dot newpos(_pos);
    newpos.x+=1;
    if(!w.Accomodates(blocks[_block].GetDots(newpos,_orientation)))
      return false;
    _pos=newpos;
    return true;
  }

  bool FallingBlock::MoveDown(const Well &w){
    Dot newpos(_pos);
    newpos.y+=1;
    if(!w.Accomodates(blocks[_block].GetDots(newpos,_orientation)))
      return false;
    _pos=newpos;
    return true;
  }

  int FallingBlock::HardDrop(const Well &w){
    int drops=0;
    while(MoveDown(w)) drops++;
    return drops;
  }

  Color FallingBlock::GetColor() const{
    return blocks[_block].GetColor();
  }

  bool FallingBlock::IsOutOfScreen() const{
    BOOST_FOREACH(const Dot &d, GetMatrix()){
      if(d.y>=0) return false;
    }
    return true;
  }

  size_t hash_value(const FallingBlock &fb){
    size_t seed=37;
    boost::hash_combine(seed,fb._block);
    boost::hash_combine(seed,fb._pos);
    boost::hash_combine(seed,fb._orientation);
    return seed;
  }
  
  std::ostream &operator<<(std::ostream &s, const FallingBlock &fb){
    s<<"pos:("<<fb._pos.x<<","<<fb._pos.y<<")or:"<<int(fb._orientation);
    return s;
  }
  
}
