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
    BOOST_FOREACH(WellLine &l, _well)
      l.assign(0);
  }

  const Color &Well::operator()(const Dot &d) const{
    assert(d.IsValid());
    return _well[d.y+2][d.x];
  }
  Color &Well::operator()(const Dot &d){
    assert(d.IsValid());
    return _well[d.y+2][d.x];
  }
  
  bool Well::Accomodates(const DotMatrix &m) const{
    BOOST_FOREACH(const Dot &d, m){
      if(!d.IsValid() || operator()(d)!=0) return false;
    }
    return true;
  }

  bool Well::IsLineComplete(int y) const{
    for(int x=0;x<(int)WellWidth;++x)
      if(operator()((Dot){x,y})==0)
	return false;
    return true;
  }

  std::vector<int> Well::Lock(const FallingBlock &fb){
    std::vector<int> ret;
    if(fb.IsOutOfScreen())
      throw(GameOver());
    BOOST_FOREACH(const Dot &d,fb.GetMatrix()){
      operator()(d)=fb.GetColor();
      //check for completeness
      if(IsLineComplete(d.y)) ret.push_back(d.y);
    }
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

  int Well::LockAndClearLines(const FallingBlock &fb){
    std::vector<int> v=Lock(fb);
    ClearLines(v);
    return v.size();
  }
}
