#ifndef WELL_HPP
#define WELL_HPP

#include "Block.hpp" //for Color
#include "FallingBlock.hpp"
#include <cstddef> //size_t
#include <vector>
#include <bitset>

namespace Bastet{
  
  class FallingBlock; //forward

  class GameOver{}; //used as an exception

  /*
   * the real height of the well is _height+2, with the top two rows( -1 and -2) hidden (see guidelines)
   */
  class Well{
  private:
    size_t _width,_height;
    Color *_well;
  public:
    Well(size_t width, size_t height);
    ~Well();
    void Clear();
    Color operator()(int x, int y) const;
    Color operator()(const Dot &p) const;
    size_t GetWidth() const;
    size_t GetHeight() const;
    bool Accomodates(const DotMatrix &d) const; //true if the given tetromino fits into the well
    bool IsLineComplete(int y) const;
    std::vector<int> Lock(const FallingBlock &fb); //permanently adds a tetromino to the well; returns a bitset of 4 bits where return[i]==1 iff line (start of fb)+i is complete
    void ClearLines(const std::vector<int> &completed); //removes the given lines from the well (whether they are completed or not)
  };

}

#endif //WELL_HPP
