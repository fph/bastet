/*
    Bastet - tetris clone with embedded bastard block chooser
    (c) 2005-2009 Federico Poloni <f.polonithirtyseven@sns.it> minus 37

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Block.hpp"

#include "curses.h"

namespace Bastet{

  BlockArray blocks={{
      BlockImpl(COLOR_PAIR(7),(OrientationMatrix){{ //O //should be yellow, but I find no portable way to output a yellow solid block character in ncurses.
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
      BlockImpl(COLOR_PAIR(4),(OrientationMatrix){{ //I
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
      BlockImpl(COLOR_PAIR(1),(OrientationMatrix){{ //Z
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
	  }}),
      BlockImpl(COLOR_PAIR(5),(OrientationMatrix){{ //T
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
      BlockImpl(COLOR_PAIR(6),(OrientationMatrix){{ //J
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
      BlockImpl(COLOR_PAIR(3),(OrientationMatrix){{ //S
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
      BlockImpl(COLOR_PAIR(2),(OrientationMatrix){{ //L
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
	  }})
    }};

//   const DotMatrix GetDots(BlockType b, Dot position, Orientation o){
//     return blocks[b].GetDots(position,o);
//   }
  const Color GetColor(BlockType b){
    return blocks[b].GetColor();
  }
  const char GetChar(BlockType b){
    return "OIZTJSL"[int(b)];
  }

  size_t hash_value(const Dot &d){
    return (d.x+5)*32+d.y;
  }
}
