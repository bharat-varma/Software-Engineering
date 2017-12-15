#include "toks.h"
#include <cstdio>

#pragma warning(disable: 4996)

vector<vector<vector<string> > > Toks::toSentences(string s, bool fromFile)
  {
    if(fromFile)
    {
      FILE * f = fopen(s.c_str(), "rt");
      s = "";
      while(!feof(f))
      {
        char buffer[1024];
        fgets(buffer, 1024, f);
        buffer[strlen(buffer)-1] = '\0';
        if(buffer[0]=='\0')
        {
          buffer[0] = ' ';
          buffer[1] = '\0';
        }
        s += buffer;
      }
      fclose(f);
    }

    const char * line = s.c_str();
    const char * st = line;
    const char * c = st;
    vector<string> tokens;

    while(*c)
    {
      // skip white spaces
      while(*c && isspace(*c)) c++;
      st = c;
      while(*c && !isspace(*c)) c++;
      if(*st==*c) continue;
      char tk[64];
      strncpy(tk, st, c-st);
      tk[c-st] = '\0';
      tokens.push_back(tk);
    }

    vector<vector<vector<string> > > out;// = new std::vector<vector<vector<string> > >;
    int i = 0;
    vector<vector<string >> clause;
    vector<string> row;
    while(i < tokens.size())
    {
      const char * tk = tokens[i].c_str();
      if(!strcmp(tk, "if")) // an if
      {
        clause.push_back(row);
        row.clear();
      }
      else if(!strcmp(tk, "and"))
      {
        clause.push_back(row);
        row.clear();
      }
      else if(!strcmp(tk, "holds"))
      {
        
      }
      else if(islower(tk[0]))  // constant
      {
        row.push_back("c:"+tokens[i]);
      }
      else if(isupper(tk[0])) // variable
      {
        row.push_back("v:"+tokens[i]);
      }
      else if(isdigit(tk[0])) // a number
      {
        row.push_back("n:"+tokens[i]);        
      }
      else if(tk[0]=='_') // temp variable... check if there is a "holds" following
      {
        if(!strcmp(tokens[i+1].c_str(), "holds"))
        {
          row.push_back("h:"+tokens[i]);
          i++;
        }
        else
        {
          row.push_back("v:"+tokens[i]);
        }
      }
      else if(tk[0]=='.') // .
      {
        clause.push_back(row);
        out.push_back(clause);
        row.clear();
        clause.clear();
      }
      else  // anything else
      {
        row.push_back(tokens[i]);
      }
      i++;

	  /*
      printf("Line:\n");
      for(string s : row)
      {
        printf("[%s], ", s.c_str());        
      }
      printf("\n");*/
    }

    return out;
  }

int main_test()
{
  vector<vector<vector<string> > >  s = Toks::toSentences("family.pl.nl", true);
  for(int i=0;i<s.size();i++)
  {
    for(int j=0;j<((s)[i]).size();j++)
    {
      for(int k=0;k<((s)[i])[j].size();k++)
      {
        const char * tk = (s)[i][j][k].c_str();
        printf("[%s]\n", tk);
      }
      printf("-------\n");
    }
    printf("----------------\n");
  }
  return 0;
}
