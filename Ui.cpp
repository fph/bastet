#include "Ui.hpp"
#include "FallingBlock.hpp"

#include <cstdio>
#include <cstdlib>
#include <boost/format.hpp>
#include <boost/foreach.hpp>

using namespace std;
using namespace boost;

namespace Bastet{

  Score &operator +=(Score &a, const Score &b){
    a.first+=b.first;
    a.second+=b.second;
    return a;
  }

  void voidendwin(){
    endwin();
  }

  void PrepareUiGetch(){ ///gets ready for a getch() in the UI, i.e. empties the char buffer, sets blocking IO
    nodelay(stdscr,TRUE);
    while(getch()!=ERR);
    nodelay(stdscr,FALSE);
  }
  
  BorderedWindow::BorderedWindow(int height, int width, int y, int x){
    if(y==-1 || x==-1){
      int screen_h, screen_w;
      getmaxyx(stdscr,screen_h, screen_w);
      if(y==-1)
	y=(screen_h-height-2)/2-1;
      if(x==-1)
	x=(screen_w-width-2)/2-1;
    }
    _border=newwin(height+2,width+2,y,x);
    _window=derwin(_border,height,width,1,1); 
    //    wattrset(_border,COLOR_PAIR(21));
    RedrawBorder();
  }

  BorderedWindow::~BorderedWindow(){
    delwin(_window);
    delwin(_border);
  }
  
  BorderedWindow::operator WINDOW*(){
    return _window;
  }

  void BorderedWindow::RedrawBorder(){
    box(_border,0,0);
    wrefresh(_border);
  }
 
  int BorderedWindow::GetMinX(){
    int y,x;
    getbegyx(_border,y,x);
    return x;
  }

  int BorderedWindow::GetMinY(){
    int y,x;
    getbegyx(_border,y,x);
    return y;
  }

  int BorderedWindow::GetMaxX(){
    int y,x;
    getmaxyx(_border,y,x);
    return GetMinX()+x;
  }

  int BorderedWindow::GetMaxY(){
    int y,x;
    getmaxyx(_border,y,x);
    return GetMinY()+y;
  }

  void BorderedWindow::DrawDot(const Dot &d, Color c){
    wattrset((WINDOW *)(*this),c);
    mvwaddch(*this,d.y,2*d.x,' ');
    mvwaddch(*this,d.y,2*d.x+1,' ');
  }

  Curses::Curses(){
    if(initscr()==NULL){
      fprintf(stderr,"bastet: error while initializing graphics (ncurses library).\n");
      exit(1);
    }
    if(!has_colors()){
      endwin();
      fprintf(stderr,"bastet: no color support, sorry. Ask the author for a black and white version.");
      exit(1);
    }
    
    /* Turn off cursor. */
    curs_set(0);
    atexit(voidendwin); /*make sure curses are properly stopped*/
    
    /* Setup keyboard. We'd like to get each and every character, but
       not to display them on the terminal. */
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    nonl();
    noecho();
    cbreak();
    
    start_color();
    /* 1 - 16 is for blocks */
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_BLACK, COLOR_GREEN);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
    init_pair(6, COLOR_BLACK, COLOR_BLUE);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    
    
    /* 17 - ? is for other things */
    init_pair(17, COLOR_RED,    COLOR_BLACK); //points
    init_pair(18, COLOR_YELLOW, COLOR_BLACK); //number of lines
    init_pair(19, COLOR_GREEN,  COLOR_BLACK); //level
    init_pair(20, COLOR_YELLOW, COLOR_BLACK); //messages
    init_pair(21, COLOR_WHITE,  COLOR_BLACK); //window borders
    init_pair(22, COLOR_WHITE,  COLOR_BLACK); //end of line animation

    /* Set random seed. */
    srandom(time(NULL)+37);
  }

  Ui::Ui():_width(10),_height(20),
				_wellWin(_height,2*_width),
				_nextWin(5,14,_wellWin.GetMinY(),_wellWin.GetMaxX()+1),
				_scoreWin(7,14,_nextWin.GetMaxY(),_nextWin.GetMinX())
  {
  }

  Dot BoundingRect(const std::string &message){ //returns x and y of the minimal rectangle containing the given string
    size_t height=0;
    size_t width=0;
    size_t start=0;
    bool terminate=false;
    while(!terminate){
      size_t next=message.find('\n',start);
      if(next==string::npos){
	next=message.size();
	terminate=true;
      }
      width=max(width,next-start);
      start=next+1;
      height++;
    }
    return (Dot){width,height};
  }

  void Ui::MessageDialog(const std::string &message){
    RedrawStatic();

    Dot d=BoundingRect(message);

    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
  }

  void Ui::ChooseLevel(){
    _level=0;
    int ch='0';
    format fmt("    Bastet\n"
	       "\n"
	       "Starting level = %1% \n"
	       "0-9 to change\n"
	       "<SPACE> to start\n");
    string msg;
    while(ch!=' '){
      msg=str(fmt % _level);
      PrepareUiGetch();
      MessageDialog(msg);
      ch=getch();
      switch(ch){
      case '0'...'9':
	_level=ch-'0';
      }
    }
    assert(_level>=0 && _level<=9);
  }

  void Ui::RedrawStatic(){
    _wellWin.RedrawBorder();
    _nextWin.RedrawBorder();
    _scoreWin.RedrawBorder();

    wattrset((WINDOW*)_nextWin,COLOR_PAIR(17));
    mvwprintw(_nextWin,0,0,"Next block:");
    wrefresh(_nextWin);

    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(17));
    mvwprintw(_scoreWin,1,0,"Score:");
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(18));
    mvwprintw(_scoreWin,3,0,"Lines:");
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(19));
    mvwprintw(_scoreWin,5,0,"Level:");
    wrefresh(_scoreWin);

  }

  //must be <1E+06, because it should fit into a timeval usec field(see man select)
  static const boost::array<int,10> delay = {{999999, 770000, 593000, 457000, 352000, 271000, 208000, 160000, 124000, 95000}};

  void Ui::DropBlock(Well &w, const Block &b){
    fd_set in, tmp_in;
    struct timeval time;

    FD_ZERO(&in);
    FD_SET(0,&in); //adds stdin
    
    time.tv_sec=0;
    time.tv_usec=delay[_level];
    
    //assumes nodelay(stdscr,TRUE) has already been called
    FallingBlock fb(b,w);    

    RedrawWell(w,fb);

    while(1){ //break = tetromino locked
      tmp_in=in;
      int sel_ret=select(FD_SETSIZE,&tmp_in, NULL, NULL, &time);
      if(sel_ret==0){ //timeout
	if(!fb.MoveDown(w))
	  break;
	time.tv_sec=0;
	time.tv_usec=delay[_level];
      }
      else{ //keypress
	int ch=getch();
	if(ch==KEY_LEFT)
	  fb.MoveLeft(w);
	else if(ch==KEY_RIGHT)
	  fb.MoveRight(w);
	else if(ch==KEY_DOWN){
	  bool val=fb.MoveDown(w);
	  if(val){
	    //_points++;
	    //RedrawScore();
	    time.tv_sec=0;
	    time.tv_usec=delay[_level];
	  }
	  else break;
	}
	else if(ch==KEY_UP)
	  fb.RotateCW(w);
	else if(ch==KEY_BACKSPACE)
	  fb.RotateCCW(w);
	else if(ch==' '){
	  fb.HardDrop(w);
	  //_points+=2*fb.HardDrop(w);
	  //RedrawScore();
	  break;
	}
	else if(ch=='p'){
	  //TODO: stub, pause
	}
	else {} //default...

      } //keypress switch
      RedrawWell(w,fb);
    } //while(1) vvv piece locked
    std::vector<int> v=w.Lock(fb);
    RedrawWell(w,fb);
    if(!v.empty()){
      CompletedLinesAnimation(v);
      w.ClearLines(v);
      _lines+=v.size();
      switch(v.size()){
      case 1:
	_points+=100;
	break;
      case 2:
	_points+=300;
	break;
      case 3:
	_points+=500;
	break;
      case 4:
	_points+=800;
	break;
      }
      RedrawScore();
    }
  }

  void Ui::RedrawWell(const Well &w, const FallingBlock &fb){
    for(size_t i=0;i<_width;++i)
      for(size_t j=0;j<_height;++j)
	_wellWin.DrawDot((Dot){i,j},w(i,j));
    
    BOOST_FOREACH(const Dot &d, fb.GetMatrix())
      _wellWin.DrawDot(d,fb.GetColor());

    wrefresh(_wellWin);
  }

  void Ui::RedrawNext(const Block &next){
    wmove((WINDOW*)_nextWin,1,0);
    wclrtobot((WINDOW*)_nextWin);
    
    BOOST_FOREACH(const Dot &d, next.GetDots((Dot){2,2},Block::InitialOrientation()))
      _nextWin.DrawDot(d,next.GetColor());
    wrefresh(_nextWin);
    return;
  }

  void Ui::RedrawScore(){
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(17));
    mvwprintw(_scoreWin,1,7,"%6d",_points);
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(18));
    mvwprintw(_scoreWin,3,7,"%6d",_lines);
    wattrset((WINDOW*)_scoreWin,COLOR_PAIR(19));
    mvwprintw(_scoreWin,5,7,"%6d",_level);
    wrefresh(_scoreWin);
    return;
  }

  void Ui::CompletedLinesAnimation(const std::vector<int> &completed){
    wattrset((WINDOW*)_wellWin,COLOR_PAIR(22));
    for(int i=0;i<6;++i){
      BOOST_FOREACH(int k, completed){
	wmove(_wellWin,k,0);
	whline(_wellWin, i%2?' ':':',_width*2);
      }
      wrefresh(_wellWin);
      usleep(500000/6);
    }
  }

  void Ui::Play(){
    _points=0;
    _lines=0;
    RedrawStatic();
    RedrawScore();
    Well w(_width,_height);
    Block *current=0;
    Block *next=&blocks[random()%7];
    RedrawNext(*next);
    nodelay(stdscr,TRUE);
    try{
      while(true){
	current=next;
	next=&blocks[random()%7];
	RedrawNext(*next);
	DropBlock(w,*current);
      }
    } catch(GameOver &go){

    }
    return;
  }

}
