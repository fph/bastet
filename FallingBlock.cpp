#include "FallingBlock.hpp"
#include <boost/foreach.hpp>

namespace Bastet{
  FallingBlock::FallingBlock(const Block &block, const Well &w, const Dot &pos, Orientation orientation):
    _block(block),_pos(pos),_orientation(orientation){
    
    const DotMatrix &m=_block.GetDots(_pos,_orientation);
    if(!w.Accomodates(m))
      throw(GameOver());
  };
  
  FallingBlock::~FallingBlock(){};
  
  DotMatrix FallingBlock::GetMatrix() const{
    return _block.GetDots(_pos,_orientation);
  }

  bool FallingBlock::RotateCW(const Well &w){
    Orientation o=_block.NextOrientation(_orientation);
    if(!w.Accomodates(_block.GetDots(_pos,o)))
      return false;
    _orientation=o;
    return true;
  }
  
  bool FallingBlock::RotateCCW(const Well &w){
    Orientation o=_block.PreviousOrientation(_orientation);
    if(!w.Accomodates(_block.GetDots(_pos,o)))
      return false;
    _orientation=o;
    return true;
  }
  
  bool FallingBlock::MoveLeft(const Well &w){
    Dot newpos(_pos);
    newpos.x-=1;
    if(!w.Accomodates(_block.GetDots(newpos,_orientation)))
      return false;
    _pos=newpos;
    return true;
  }

  bool FallingBlock::MoveRight(const Well &w){
    Dot newpos(_pos);
    newpos.x+=1;
    if(!w.Accomodates(_block.GetDots(newpos,_orientation)))
      return false;
    _pos=newpos;
    return true;
  }

  bool FallingBlock::MoveDown(const Well &w){
    Dot newpos(_pos);
    newpos.y+=1;
    if(!w.Accomodates(_block.GetDots(newpos,_orientation)))
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
    return _block.GetColor();
  }

  bool FallingBlock::IsOutOfScreen() const{
    BOOST_FOREACH(const Dot &d, GetMatrix()){
      if(d.y>=0) return false;
    }
    return true;
  }
}
