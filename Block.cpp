#include "Block.hpp"

#include "curses.h"

namespace Bastet{

  boost::array<Block,7> blocks={{
      Block(COLOR_PAIR(4),(OrientationMatrix){{ //I
	    {{//orientation 0 (initial)
		{0,1},{1,1},{2,1},{3,1}
	      }},
	      {{//orientation 1
		  {2,3},{2,1},{2,2},{2,0}
		}},
		{{//orientation 2
		    {0,2},{1,2},{2,2},{3,2}
		  }},
		  {{//orientation 3
		      {1,3},{1,1},{1,2},{1,0}
		    }}
	  }}),
      Block(COLOR_PAIR(6),(OrientationMatrix){{ //J
	    {{//orientation 0 (initial
		{0,1},{1,1},{2,1},{0,0}
	      }},
	      {{//orientation 1
		  {1,2},{1,1},{1,0},{2,0}
		}},
		{{//orientation 2
		    {2,2},{0,1},{1,1},{2,1}
		  }},
		  {{//orientation 3
		      {0,2},{1,2},{1,1},{1,0}
		    }}
	  }}),
      Block(COLOR_PAIR(2),(OrientationMatrix){{ //L
	    {{//orientation 0 (initial
		{0,1},{1,1},{2,1},{2,0}
	      }},
	      {{//orientation 1
		  {1,2},{2,2},{1,1},{1,0}
		}},
		{{//orientation 2
		    {0,2},{0,1},{1,1},{2,1}
		  }},
		  {{//orientation 3
	  {1,2},{1,1},{0,0},{1,0}
		    }}
	  }}),
      Block(COLOR_PAIR(7),(OrientationMatrix){{ //O //should be yellow, but I find no portable way to output a yellow solid block character in ncurses.
	    {{//orientation 0 (initial
		{1,1},{2,1},{1,0},{2,0}
	      }},
	      {{//orientation 1
		  {1,1},{2,1},{1,0},{2,0}
		}},
		{{//orientation 2
		    {1,1},{2,1},{1,0},{2,0}
		  }},
		  {{//orientation 3
		      {1,1},{2,1},{1,0},{2,0}
		    }}
	  }}),
      Block(COLOR_PAIR(3),(OrientationMatrix){{ //S
	    {{//orientation 0 (initial
		{0,1},{1,1},{1,0},{2,0}
	      }},
	      {{//orientation 1
		  {2,2},{1,1},{2,1},{1,0}
		}},
		{{//orientation 2
		    {0,2},{1,2},{1,1},{2,1}
		  }},
		  {{//orientation 3
		      {1,2},{0,1},{1,1},{0,0}
		    }}
	  }}),
      Block(COLOR_PAIR(5),(OrientationMatrix){{ //T
	    {{//orientation 0 (initial
		{0,1},{1,1},{2,1},{1,0}
	      }},
	      {{//orientation 1
		  {1,2},{1,1},{2,1},{1,0}
		}},
		{{//orientation 2
		    {1,2},{0,1},{1,1},{2,1}
		  }},
		  {{//orientation 3
		      {1,2},{0,1},{1,1},{1,0}
		    }}
	  }}),
      Block(COLOR_PAIR(1),(OrientationMatrix){{ //Z
	    {{//orientation 0 (initial
		{1,1},{2,1},{0,0},{1,0}
	      }},
	      {{//orientation 1
		  {1,2},{1,1},{2,1},{2,0}
		}},
		{{//orientation 2
		    {1,2},{2,2},{0,1},{1,1}
		  }},
		  {{//orientation 3
		      {0,2},{0,1},{1,1},{1,0}
		    }}
	  }})
    }};
  
}
