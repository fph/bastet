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

  class WellLine: public boost::array<bool,WellWidth>{

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
