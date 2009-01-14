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
      BOOST_FOREACH(bool &b, l){
	b=false;
      }
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

  std::vector<int> Well::Lock(BlockType t, const BlockPosition &p){
    std::vector<int> ret;
    if(p.IsOutOfScreen(t))
      throw(GameOver());
    BOOST_FOREACH(const Dot &d,p.GetDots(t)){
      _well[d.y+2][d.x]=true;
      //check for completeness
      if(IsLineComplete(d.y)) ret.push_back(d.y);
    }
    //TODO: XXX: remove duplicates
    sort(ret.begin(),ret.end());
    return ret;
  }
  
  void Well::ClearLines(const std::vector<int> &completed){
    //a bit messy but should work
    if(completed.empty()) return;
    std::vector<int>::const_reverse_iterator it=completed.rbegin();
    int orig=*it,dest=*it;
    while(orig>=-2){
      if(it!=completed.rend() && orig==*it){
	orig--;
	it++;
	continue;
      }
      
      _well[dest+2]=_well[orig+2];
      orig--;
      dest--;
    }
    for(;dest>=-2;dest--){
      _well[dest+2].assign(0);
    }
  }

  int Well::LockAndClearLines(BlockType t, const BlockPosition &p){
    std::vector<int> v=Lock(t,p);
    ClearLines(v);
    return v.size();
  }
}
