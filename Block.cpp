#include "Block.hpp"

namespace Bastet{

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::O>::_dotMatrix = {{
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
    }};

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::I>::_dotMatrix = {{
      {{//orientation 0 (initial
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
    }};

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::J>::_dotMatrix = {{
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
    }};

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::L>::_dotMatrix = {{
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
    }};

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::S>::_dotMatrix = {{
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
    }};

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::T>::_dotMatrix = {{
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
    }};

  template<>
  const boost::array<boost::array<Dot,4>,4> Block<BlockType::Z>::_dotMatrix = {{
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
    }};
  
}
