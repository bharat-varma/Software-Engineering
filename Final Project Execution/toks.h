#ifndef __TOKS_H__
#define __TOKS_H__

#include <string>
#include <cstring>
#include <cctype>
#include <vector>

using namespace std;

class Toks
{

public:

  static vector<vector<vector<string> > > toSentences(string s, bool fromFile);
};

int main_test();

#endif
