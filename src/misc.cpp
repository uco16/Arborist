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
