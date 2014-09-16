#include "Well.hpp"
#include "BastetBlockChooser.hpp"
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

  w->Clear();
  BlockPosition p2;
  p2.Drop(I,w);
  w->LockAndClearLines(I,p2);
  cout<<w->PrettyPrint()<<endl;
  cout<<"Score:"<<Evaluate(w)<<endl;  
}
