#include "Well.hpp"
#include <cassert>
#include <cstring>
#include <boost/foreach.hpp>

namespace Bastet{
  Well::Well(size_t width, size_t height):_width(width),_height(height),_well(new Color[width*(height+2)]){
    Clear();
    }
  
  Well::~Well(){
    delete [] _well;
  }

  void Well::Clear(){
    memset(_well,0,_width*(_height+2)*sizeof(Color));
  }

  Color Well::operator()(int x, int y) const{
    assert(y>=-2 && y<int(_height) && x>=0 && x<int(_width));
    return _well[(y+2)*_width+x];
  }
  Color Well::operator()(const Dot &p) const{
    return operator()(p.x,p.y);
  }
  size_t Well::GetWidth() const{
    return _width;
  }
  size_t Well::GetHeight() const{
    return _height;
  }
  
  bool Well::Accomodates(const DotMatrix &m) const{
    BOOST_FOREACH(const Dot &d, m){
      if( d.x < 0 || d.x>=(int)_width || d.y<-2 || d.y>=(int)_height || operator()(d)!=0)
        return false;
    }
    return true;
  }

  bool Well::IsLineComplete(int y) const{
    for(int x=0;x<(int)_width;++x)
      if(operator()(x,y)==0)
	return false;
    return true;
  }

  std::vector<int> Well::Lock(const FallingBlock &fb){
    std::vector<int> ret;
    if(fb.IsOutOfScreen())
      throw(GameOver());
    BOOST_FOREACH(const Dot &d,fb.GetMatrix()){
      _well[(d.y+2)*_width+d.x]=fb.GetColor(); //TODO: add a proper private lvalue operator ()
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
      memcpy(_well+(dest+2)*_width,_well+(orig+2)*_width,_width*sizeof(Color));
      orig--;
      dest--;
    }
    for(;dest>=-2;dest--){
      memset(_well+(dest+2)*_width,0,_width*sizeof(Color));
    }
  }
}
