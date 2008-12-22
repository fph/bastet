#ifndef BLOCK_HPP
#define BLOCK_HPP

namespace Bastet{

  typedef unsigned char Color;
  typedef short Orientation;

  class Dot{
  public:
    int x;
    int y;
    Dot(int x,int y):x(x),y(y){};
  };

  class Block{
  private:
  public:
    Block();
    virtual ~Block();
    /**
     * returns an array of 4 (x,y) pair for the occupied dots
     * To be freed by the recipient TODO: smart pointers
     */
    virtual Dot* GetDots(Dot position, Orientation orientation) const;
    virtual Color GetColor() const;
    virtual Orientation NextOrientation() const;
    virtual Orientation PreviousOrientation() const;
    virtual Orientation InitialOrientation() const;
  };
}

#endif //BLOCK_HPP
