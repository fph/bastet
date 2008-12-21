namespace Bastet{

  enum BlockType{OBlock=0,IBlock,JBlock,LBlock,ZBlock,SBlock,TBlock};

  typedef unsigned char Color;

  class Dot{
  public:
    int x;
    int y;
  };

  class Block{
  private:
    BlockType _type;
    Color _color;
  public:
    Block(const BlockType &type, const Color &color);
    virtual ~Block();
    /**
     * returns an array of 4 (x,y) pair for the occupied dots
     */
    virtual Dot[4] GetDots(Dot position, size_t orientation) const;
    virtual Color GetColor() const;
  };
}
