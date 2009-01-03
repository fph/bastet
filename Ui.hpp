#ifndef UI_HPP
#define UI_HPP

#include "Well.hpp"
#include "FallingBlock.hpp"

#include <string>
#include <curses.h>

namespace Bastet{
  typedef std::pair<int,int> Score; //(points, lines)
  Score &operator +=(Score &a, const Score &b);

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
    Ui();
    void MessageDialog(const std::string &message); //shows msg, ask for "space"
    std::string InputDialog(const std::string &message); //asks for a string
    int KeyDialog(const std::string &message); //asks for a single key
    int MenuDialog(const std::vector<std::string> &choices); //asks to choose one, returns index
    void RedrawStatic(); //redraws the "static" parts of the screen
    void RedrawWell(const Well &well, const FallingBlock &fb);
    void RedrawNext(const Block &next);
    void RedrawScore();
    void CompletedLinesAnimation(const std::vector<int> &completed);
    void DropBlock(Well &w, const Block &b); //returns <score,lines>
    
    void ChooseLevel();
    void Play();
    void HandleHighScores(); ///if needed, asks name for highscores
    void ShowHighScores();
    void CustomizeKeys();
  private:
    int _level;
    int _points;
    int _lines;
    Curses _curses;
    size_t _width;
    size_t _height;
    BorderedWindow _wellWin;
    BorderedWindow _nextWin;
    BorderedWindow _scoreWin;
  };
}

#endif //UI_HPP
