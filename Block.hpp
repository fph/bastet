#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <boost/array.hpp>
#include <curses.h>

namespace Bastet{

  typedef int Color; //to be given to wattrset
  typedef short Orientation;
  struct Dot;

  typedef boost::array<Dot,4> DotMatrix; //the four dots occupied by a tetromino
  typedef boost::array<DotMatrix,4> OrientationMatrix; //the four orientations of a tetromino

  struct Dot{
    int x;
    int y;
    Dot operator +(const Dot &d) const{
      return (Dot){x+d.x,y+d.y};
    }
    Dot &operator +=(const Dot &d){
      x+=d.x;y+=d.y;
      return *this;
    }
    DotMatrix operator +(const DotMatrix &b) const{
      return (DotMatrix){{*this+b[0],*this+b[1],*this+b[2],*this+b[3]}};
    }
  };

  class Block{
  private:
    const OrientationMatrix _matrix;
    const Color _color;
  public:
    Block(Color c, const OrientationMatrix &m):_matrix(m),_color(c){};
    ~Block(){};
    /**
     * returns an array of 4 (x,y) pair for the occupied dots
     */
    DotMatrix GetDots(Dot position, Orientation orientation) const
    {return position+_matrix[orientation];}
    Color GetColor() const {return _color;};
    Orientation NextOrientation(Orientation &o) const
    {return (o+1) % 4;}
    //{return (o+1==HowManyOrientations())?0:(o+1);}
    Orientation PreviousOrientation(Orientation &o) const
    {return (o+3) % 4;}
    //{return (o==0)?HowManyOrientations()-1:(o-1);}
    static Orientation HowManyOrientations()
    {return 4;}
    static Orientation InitialOrientation()
    {return 0;}
  };

  extern boost::array<Block,7> blocks;
}

#endif //BLOCK_HPP
