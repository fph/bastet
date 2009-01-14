#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <boost/array.hpp>
#include <curses.h>

namespace Bastet{

  static const int WellHeight=20;
  static const int WellWidth=10;
  static const int RealWellHeight=WellHeight+2;
  
  typedef int Color; //to be given to wattrset
  
  class Orientation{
  public:
    Orientation(unsigned char o=0):_o(o){}
    operator unsigned char() const{
      return _o;
    }
    Orientation operator++(){
      return (++_o & 3);
    }
    Orientation Next() const{
      return ((_o+1) & 3);
    }
    Orientation operator--(){
      return (--_o & 3);
    }
    Orientation Prior() const{
      return((_o-1) & 3);
    }
    const static size_t Number=4;
  private:
    unsigned char _o;
  };

  enum BlockType{
    O=0,
    I=1,
    Z=2,
    T=3,
    J=4,
    S=5,
    L=6
  };

  struct Dot;

  typedef boost::array<Dot,4> DotMatrix; //the four dots occupied by a tetromino
  typedef boost::array<DotMatrix,4> OrientationMatrix; //the four orientations of a tetromino

  struct Dot{
    int x;
    int y;
    bool IsValid() const{
      return (y>=-2) && y<WellHeight && (x>=0) && (x<WellWidth);
    }

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
    bool operator==(const Dot &other) const{
      return (x==other.x) && (y==other.y);
    }
    bool operator<(const Dot &other) const{
      if(x==other.x)
	return y<other.y;
      else return x<other.x;
    }

    friend size_t hash_value(const Dot &d); //for use with boost::hash and unordered_set
  };

  class BlockImpl{
  private:
    const OrientationMatrix _matrix;
    const Color _color;
  public:
    BlockImpl(Color c, const OrientationMatrix &m):_matrix(m),_color(c){};
    ~BlockImpl(){};
    /**
     * returns an array of 4 (x,y) pair for the occupied dots
     */
    const OrientationMatrix &GetOrientationMatrix(){return _matrix;}
    Color GetColor() const {return _color;};
  };

  typedef boost::array<BlockImpl,7> BlockArray;
  extern BlockArray blocks;

  //should be members, but BlockType is an enum...
  //  const DotMatrix GetDots(BlockType b, Dot position, Orientation o);
  const Color GetColor(BlockType b);

}

#endif //BLOCK_HPP
