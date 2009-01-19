#include "Well.hpp"
#include <cassert>
#include <cstring>
#include <boost/foreach.hpp>

namespace Bastet{
  Well::Well(){
    Clear();
  }
  
  Well::~Well(){
  }

  void Well::Clear(){
    BOOST_FOREACH(WellLine &l, _well){
      l.reset();
    }
  }

  bool Well::Accomodates(const DotMatrix &m) const{
    BOOST_FOREACH(const Dot &d, m){
      if(!d.IsValid() || _well[d.y+2][d.x]==true) return false;
    }
    return true;
  }

  bool Well::IsLineComplete(int y) const{
    for(int x=0;x<(int)WellWidth;++x)
      if(_well[y+2][x]==false)
	return false;
    return true;
  }

  LinesCompleted Well::Lock(BlockType t, const BlockPosition &p){
    if(p.IsOutOfScreen(t))
      throw(GameOver());
    BOOST_FOREACH(const Dot &d,p.GetDots(t)){
      _well[d.y+2][d.x]=true;
    }
    //checks for completedness
    LinesCompleted lc;
    lc._baseY=p.GetBaseY();
    for(int k=0;k<4;++k){
      int l=lc._baseY+k;
      if(IsValidLine(l) && IsLineComplete(l))
	lc._completed[k]=true;
    }
    return lc;
  }
  
  void Well::ClearLines(const LinesCompleted &completed){
    WellType::reverse_iterator it=completed.Clear(_well.rbegin(),_well.rend());
    for(;it!=_well.rend();++it){
      it->reset();
    }
  }

  int Well::LockAndClearLines(BlockType t, const BlockPosition &p){
    LinesCompleted lc=Lock(t,p);
    ClearLines(lc);
    return lc._completed.count();
  }
}
