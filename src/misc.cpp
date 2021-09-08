// miscellaneous functions (should be empty at the end of the project)
#include "misc.h"

#include <string>
#include <vector>
using std::vector;
using std::string;

vector<string> split(const string& s)
// split string s into tokens at whitespace
{
  vector<string> s_split;
  string wrd;
  for ( char c : s )
  {
    if (c!=' ' && c!='\t')
      wrd += c;
    else if (wrd!="")
    {
      s_split.push_back(wrd);
      wrd = "";
    }
  }
  if (wrd!="")
    s_split.push_back(wrd);
  return s_split;
}

// presentation of bitboards

std::ostream& display(std::ostream& os, const Bitboard bb) {
  os << "Bitboard(" << bb << "):";

  for (int i=0; i<64; i++)
  {
    if (i%8==0)
      os << "\n\t";
    bool bit = 1 & (bb >> (63-i));  // get the i^th bit from the left
    os << " " << bit;
  }
  return os << "\n";
}
