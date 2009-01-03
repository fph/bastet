#include "Ui.hpp"
#include "FallingBlock.hpp"
#include "Config.hpp"

#include <cstdio>
#include <cstdlib>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>

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
    vector<string> splits;
    split(splits,message,is_any_of("\n"));
    size_t maxlen=0;
    BOOST_FOREACH(string &s, splits){
      maxlen=max(maxlen,s.size());
    }
    return (Dot){maxlen+1,splits.size()};
  }

  void Ui::MessageDialog(const std::string &message){
    RedrawStatic();

    Dot d=BoundingRect(message);

    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    int ch;
    do{
      ch=getch();
    }
    while(ch!=' ' && ch!=13); //13=return key!=KEY_ENTER, it seems
  }

  std::string Ui::InputDialog(const std::string &message){
    RedrawStatic();
    Dot d=BoundingRect(message);
    d.y+=3;
    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    //nocbreak();
    echo();
    curs_set(1);
    char buf[51];
    mvwgetnstr(w,d.y-2,1,buf,50);
    curs_set(0);
    noecho();
    return string(buf);
  }
  
  int Ui::KeyDialog(const std::string &message){
    RedrawStatic();

    Dot d=BoundingRect(message);

    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    mvwprintw(w,0,0,message.c_str());
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    return getch();
  }
  
  int Ui::MenuDialog(const vector<string> &choices){
    RedrawStatic();
    size_t width=0;
    BOOST_FOREACH(const string &s, choices){
      width=max(width,s.size());
    }

    Dot d={width+5,choices.size()};
    BorderedWindow w(d.y,d.x);
    wattrset((WINDOW *)w,COLOR_PAIR(20));
    for(size_t i=0;i<choices.size();++i){
      mvwprintw(w,i,4,choices[i].c_str());
    }
    w.RedrawBorder();
    wrefresh(w);
    PrepareUiGetch();
    size_t chosen=0;
    int ch;
    bool done=false;
    mvwprintw(w,chosen,1,"-> ");
    wrefresh(w);
    do{
      ch=getch();
      switch(ch){
      case KEY_UP:
	if(chosen==0) break;
	mvwprintw(w,chosen,1,"   ");
	chosen--;
	mvwprintw(w,chosen,1,"-> ");
	wrefresh(w);
	break;
      case KEY_DOWN:
	if(chosen==choices.size()-1) break;
	mvwprintw(w,chosen,1,"   ");
	chosen++;
	mvwprintw(w,chosen,1,"-> ");
	wrefresh(w);
	break;
      case 13: //ENTER
      case ' ':
	done=true;
	break;
      }
    } while(!done);
    return chosen;
  }
  
  void Ui::ChooseLevel(){
    RedrawStatic();
    _level=0;
    int ch='0';
    format fmt("    Get ready!\n"
	       " \n"
	       " Starting level = %1% \n"
	       " 0-9 to change\n"
	       " <SPACE> to start\n");
    string msg;
    while(ch!=' '){
      msg=str(fmt % _level);
      PrepareUiGetch();
      Dot d=BoundingRect(msg );
      BorderedWindow w(d.y,d.x);
      wattrset((WINDOW *)w,COLOR_PAIR(20));
      mvwprintw(w,0,0,msg.c_str());
      w.RedrawBorder();
      ch=getch();
      switch(ch){
      case '0'...'9':
	_level=ch-'0';
      }
    }
    assert(_level>=0 && _level<=9);
  }

  void Ui::RedrawStatic(){
    erase();
    wrefresh(stdscr);
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
    Keys *keys=config.GetKeys();

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
	if(ch==keys->Left)
	  fb.MoveLeft(w);
	else if(ch==keys->Right)
	  fb.MoveRight(w);
	else if(ch==keys->Down){
	  bool val=fb.MoveDown(w);
	  if(val){
	    //_points++;
	    //RedrawScore();
	    time.tv_sec=0;
	    time.tv_usec=delay[_level];
	  }
	  else break;
	}
	else if(ch==keys->RotateCW)
	  fb.RotateCW(w);
	else if(ch==keys->RotateCCW)
	  fb.RotateCCW(w);
	else if(ch==keys->Drop){
	  fb.HardDrop(w);
	  //_points+=2*fb.HardDrop(w);
	  //RedrawScore();
	  break;
	}
	else if(ch==keys->Pause){
	  MessageDialog("Press SPACE or ENTER to resume the game");
	  RedrawStatic();
	  RedrawWell(w,fb);
	  nodelay(stdscr,TRUE);
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

  void Ui::HandleHighScores(){
    HighScores *hs=config.GetHighScores();
    if(hs->Qualifies(_points)){
      string name=InputDialog(" Congratulations! You got a high score \n Please enter your name");
      hs->InsertHighScore(_points,name);
    }else{
      MessageDialog("You did not get into\n"
		    "the high score list!\n"
		    "\n"
		    "     Try again!\n"
		    );
    }
  }

  void Ui::ShowHighScores(){
    HighScores *hs=config.GetHighScores();
    //shows highscores anyway
    string allscores;
    format fmt("%20.20s %8d\n");
    for(HighScores::reverse_iterator it=hs->rbegin();it!=hs->rend();++it){
      allscores+=str(fmt % it->Scorer % it->Score);
    }
    MessageDialog(allscores);
  }

  void Ui::CustomizeKeys(){
    Keys *keys=config.GetKeys();
    format fmt(
      "Press the key you wish to use for:\n\n"
      "%=1.34s\n\n");
    keys->Down=KeyDialog(str(fmt % "move tetromino DOWN (soft-drop)"));
    keys->Left=KeyDialog(str(fmt % "move tetromino LEFT"));
    keys->Right=KeyDialog(str(fmt % "move tetromino RIGHT"));
    keys->RotateCW=KeyDialog(str(fmt % "rotate tetromino CLOCKWISE"));
    keys->RotateCCW=KeyDialog(str(fmt % "rotate tetromino COUNTERCLOCKWISE"));
    keys->Drop=KeyDialog(str(fmt % "DROP tetromino (move down as much as possible immediately)"));
    keys->Pause=KeyDialog(str(fmt % "PAUSE the game"));
  }
}
