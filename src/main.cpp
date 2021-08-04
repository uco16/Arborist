#include <iostream>
#include <string>
#include <vector>
#include "position.h"
#include "misc.h"
#include "evaluate.h"

using std::string;
using std::vector;

void help() { 
  std::cout << "Available commands:\n\n"
    << "\thelp" << "\n\t\tDisplay this help message.\n\n"
    << "\tuci"  << "\n\t\tStart engine in accordance with the universal chess interface (UCI) specification.\n\n"
//    << "\tucinewgame" << "\nNot yet supported.\n"
    << "\tisready" << "\n\t\tPing the engine to see if it is ready (does not stop search).\n\n"
    << "\tposition [fen <fenstring> | startpos ]" << "\n\t\tSet up position.\n\n"
    << "\tmoves <move1> ... <movei>" << "\n\t\tPlay moves.\n\n"
    << "\tgo" << "\n\t\tStart calculating on the position set up with the 'position' command.\n\n"
    << "\tquit, exit" << "\n\t\tQuit the program.\n";
}

void unknown() { std::cout << "I do not know that command. Use command 'help' to show a help message.\n"; }

void uci() 
{
  std::cout << "id name Arborist\n" << "id author Umut Özer\n";
  std::cout << "uciok\n";
}

//void ucinewgame()
//{
//  std::cout << "Not yet supported\n";
//}

int position(Position& pos, const vector<string>& tokens)
// initialize pos in the position specified by user
{
  if (tokens.size()==0 || !(tokens[0]=="startpos" || tokens[0]=="fen") ) {
    std::cout << "'position' expected 'fen <fenstring>' or 'startpos' argument.\n";
    return 1;
  }

  string option {tokens[0]};
  string fen;
  if (option=="startpos") {
    fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
  }
  else if (option=="fen")
  {
    if (tokens.size()==1) {
      std::cout << "'position fen' expected fen string.\n";
      return 1;
    }
    fen = tokens[1] + ' ' + tokens[2] + ' ' + tokens[3] + ' ' + tokens[4] + ' ' + tokens[5] + ' ' + tokens[6];
  }
  std::cout << "'" << fen << "'\n";
  pos.set(fen);
  return 0;
}

void moves(Position& pos, const vector<string>& tokens)
{
  for (string mv: tokens)
    pos.move(mv);
}

void handle(const string& line, Position& pos)
{
  vector<string> inputs {split(line)};
  bool understood {false};
  for (int i=0; i<inputs.size(); ++i)
  {
    string cmd {inputs[i]};
    if (cmd=="help") 
      help();
    else if (cmd=="quit" || cmd=="exit") 
      std::cout<< "Bye!\n";
    else if (cmd=="uci") uci();
//    else if (cmd=="ucinewgame") ucinewgame();
    else if (cmd=="isready") 
      std::cout << "readyok\n";
    else if (cmd=="position") 
    {
      vector<string> tokens = {inputs.begin()+i+1, inputs.end()};
      position(pos, tokens);
    }
    else if (cmd=="moves") 
    {
      vector<string> tokens = {inputs.begin()+i+1, inputs.end()};
      moves(pos, tokens);
    }
    else if (cmd=="evaluate")
      std::cout << evaluate(pos) << "\n";
    else 
      continue;
    // an if statement matched: main command was found
    understood = true;
    break;  // only execute one main command per line
  }
  if (!understood) // no main command was matched
    unknown();
}

void listen()
{
  char indicator {'+'};
  string line;
  Position pos;
  while (line!="quit" && line!="exit")
  {
    std::cout << indicator << ' ';
    std::getline(std::cin, line);
    handle(line, pos);
    std::cout << "current position: " << pos.fen() << "\n";
  }
}

void welcome() { std::cout << "Welcome to the Arborist UCI chess engine!\n"; }

int main()
{
  welcome();
  listen();
}
