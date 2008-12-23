#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <boost/array.hpp>
#include <curses.h>

namespace Bastet{

  typedef int Color; //to be given to wattrset
  typedef short Orientation;

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
  };

  namespace BlockType{
    enum BlockType{O,I,S,Z,T,L,J};
  }

  boost::array<Dot,4> operator +(const Dot &a,const boost::array<Dot,4> &b){
    return (boost::array<Dot,4>){{a+b[0],a+b[1],a+b[2],a+b[3]}};
  }

  template <BlockType::BlockType T> class Block{
  private:
    static const boost::array<boost::array<Dot,4>,4> _dotMatrix;
  public:
    Block(){};
    virtual ~Block(){};
    /**
     * returns an array of 4 (x,y) pair for the occupied dots
     */
    const boost::array<Dot,4>& GetDots(Dot position, Orientation orientation) const
    {return _dotMatrix[orientation%4];}
    Color GetColor() const;
    Orientation NextOrientation(Orientation &o) const
    {return o+1 % 4;}
    //{return (o+1==HowManyOrientations())?0:(o+1);}
    Orientation PreviousOrientation(Orientation &o) const
    {return o-1 % 4;}
    //{return (o==0)?HowManyOrientations()-1:(o-1);}
    Orientation HowManyOrientations() const
    {return 4;}
    Orientation InitialOrientation() const
    {return 0;}
  };

  template<> Color Block<BlockType::I>::GetColor() const {return COLOR_PAIR(4);}
  template<> Color Block<BlockType::J>::GetColor() const {return COLOR_PAIR(6);}
  template<> Color Block<BlockType::L>::GetColor() const {return COLOR_PAIR(2);}
  template<> Color Block<BlockType::O>::GetColor() const {return COLOR_PAIR(2)|A_BOLD;}
  template<> Color Block<BlockType::S>::GetColor() const {return COLOR_PAIR(3);}
  template<> Color Block<BlockType::T>::GetColor() const {return COLOR_PAIR(5);}
  template<> Color Block<BlockType::Z>::GetColor() const {return COLOR_PAIR(1);}
 
}

#endif //BLOCK_HPP
