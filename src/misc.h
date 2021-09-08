// miscellaneous functions (should be empty at the end of the project)

#ifndef MISC_H
#define MISC_H

#include <string>
#include <vector>
using std::vector;
using std::string;

#include "bitboard.h"

vector<string> split(const string& s);

std::ostream& display(std::ostream& os, const Bitboard bb);

#endif
