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

#ifndef WELL_HPP
#define WELL_HPP

#include "Block.hpp" //for Color
#include "BlockPosition.hpp"
#include <cstddef> //size_t
#include <vector>
#include <bitset>
#include <boost/array.hpp>

//DBG
#include <iostream>

namespace Bastet{
  
  class GameOver{}; //used as an exception

  class WellLine: public std::bitset<WellWidth>{

  };

  ///complex type that holds which lines are completed
  ///if _completed[k]==true, then line _baseY+k exists and is completed
  class LinesCompleted{
  public:
    int _baseY;
    std::bitset<4> _completed;
    ///clear, returns iterator such that the segment [it, rend) is "new" (to be zeroed out by hand)
    template<typename Iterator> Iterator Clear(Iterator rbegin, Iterator rend) const;
  };
  

  /*
   * the real height of the well is _height+2, with the top two rows( -1 and -2) hidden (see guidelines)
   */
  class Well{
  private:
    typedef boost::array<WellLine,RealWellHeight> WellType;
    WellType _well;
  public:
    Well();
    ~Well();
    void Clear();
    bool Accomodates(const DotMatrix &d) const; //true if the given tetromino fits into the well
    bool IsValidLine(int y) const{return (y>=-2) && (y<WellHeight);};
    bool IsLineComplete(int y) const;
    LinesCompleted Lock(BlockType t, const BlockPosition &p); //permanently adds a tetromino to the well; returns a bitset of 4 bits where return[i]==1 iff line (start of fb)+i is complete
    void ClearLines(const LinesCompleted &lc); //removes the given lines from the well (whether they are completed or not)
    int LockAndClearLines(BlockType t, const BlockPosition &p); //locks, clear lines, returns number of lines cleared
    friend long Evaluate(const Well *w, int extralines); //for BastetBlockChooser
  };
  
  template <typename Iterator> Iterator LinesCompleted::Clear(Iterator rbegin, Iterator rend) const{
    if(_completed.none()) return rend;
    Iterator orig=rbegin;
    Iterator dest=rbegin;
    int j=WellHeight-1;
    while(orig<rend){
      if(j-_baseY>=0 && j-_baseY<4 && _completed[j-_baseY]){
	//skip
      }
      else{
	*dest=*orig;
	dest++;
      }
      j--;
      orig++;
    }
    return dest++;
  }

}

#endif //WELL_HPP
