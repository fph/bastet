#include "Well.hpp"
#include <ios>

using namespace Bastet;
using namespace std;

int main(){
  Well *w=new Well;
  BlockPosition p;
  p.Drop(Z,w);
  w->LockAndClearLines(Z,p);
  cout<<w->PrettyPrint()<<endl;
  cout<<"Score:"<<Evaluate(w)<<endl;
}
