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
