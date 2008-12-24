#ifndef UI_HPP
#define UI_HPP

#include "Well.hpp"
#include "FallingBlock.hpp"

#include <string>
#include <curses.h>

namespace Bastet{
  class BorderedWindow{
  private:
    WINDOW *_window;
    WINDOW *_border;
  public:
    BorderedWindow(int height, int width, int y=-1, int x=-1); ///w and h are "inner" dimensions, excluding the border. y and x are "outer", including the border. y=-1,x=-1 means "center"
    ~BorderedWindow();
    operator WINDOW *(); //returns the inner window
    void RedrawBorder();
    int GetMinX(); ///these are including border
    int GetMinY();
    int GetMaxX();
    int GetMaxY();
    void DrawDot(const Dot &d, Color c);
  };

  class Curses{
  public:
    Curses();
  };

  class Ui{
  public:
    Ui(int argc,char **argv);
    void MessageDialog(const std::string &message);
    void InputDialog(const std::string &message, const std::string &defaultin);
    void RedrawStatic(); //redraws the "static" parts of the screen
    void RedrawWell(const Well &well, const FallingBlock &fb);
    void RedrawNext(const Block &next);
    void RedrawScore(int score, int lines, int level);
    int DropBlock(Well &w, const Block &b, int level);
    
    int ChooseLevel();
    int Play(int level); ///returns points
    bool ShowScores(); ///returns true if wants to play again
  private:
    Curses _curses;
    size_t _width;
    size_t _height;
    BorderedWindow _wellWin;
    BorderedWindow _nextWin;
    BorderedWindow _scoreWin;
  };
}

#endif //UI_HPP
