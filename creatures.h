#include <string>
using namespace std;
#ifndef CREATURES_H
#define CREATURES_H

class Creature {
 public:
  Creature(string type);

 private:
  string type;
};

#endif