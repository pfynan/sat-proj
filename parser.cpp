/*********************************************************************
			   ================
			   CNF Parsing code
				18-760
			      Fall 2006
			   ================
**********************************************************************/

#include "parser.h"
#include <iostream>
using std::ifstream;
using std::vector;
#include <zlib.h>
#include <cstdlib>
#include "cnf.h"

//=====================================================================
// DIMACS Parser:

#define CHUNK_LIMIT 1048576

class StreamBuffer {
  gzFile  in;
  char    buf[CHUNK_LIMIT];
  int     pos;
  int     size;

  void assureLookahead() {
    if (pos >= size) {
      pos  = 0;
      size = gzread(in, buf, sizeof(buf)); } }

public:
  StreamBuffer(gzFile i) : in(i), pos(0), size(0) {
    assureLookahead(); }

  int  operator *  () { return (pos >= size) ? EOF : buf[pos]; }
  void operator ++ () { pos++; assureLookahead(); }
};

//-  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void skipWhitespace(StreamBuffer &in) {
  while ((*in >= 9 && *in <= 13) || *in == 32)
    ++in;
}


void skipLine(StreamBuffer &in) {
  while (true) {
    if (*in == EOF) return;
    if (*in == '\n') { ++in; return; }
    ++in;
  }
}


int parseInt(StreamBuffer &in) {
  int     val = 0;
  bool    neg = false;
  skipWhitespace(in);
  if      (*in == '-') neg = true, ++in;
  else if (*in == '+') ++in;
  if (*in < '0' || *in > '9')
    fprintf(stderr, "PARSE ERROR! Unexpected char: %c\n", *in), exit(3);
  while (*in >= '0' && *in <= '9') {
    val = val*10 + (*in - '0');
    ++in;
  }
  return neg ? -val : val;
}


void readClause(StreamBuffer &in, vector<vector<int> > &clauses) {
  int parsed_lit;
  vector<int> newClause;
  while (true) {
    parsed_lit = parseInt(in);
    if (parsed_lit == 0) break;
    newClause.push_back(parsed_lit);
  }
  clauses.push_back(newClause);
}


void parse_DIMACS_main(StreamBuffer &in, vector<vector<int> > &clauses) {
  while (true) {
    skipWhitespace(in);
    if (*in == EOF) break;
    else if (*in == 'c' || *in == 'p') skipLine(in);
    else readClause(in, clauses);
  }
}


void parse_DIMACS(gzFile input_stream, vector<vector<int> > &clauses)
{
  StreamBuffer in(input_stream);
  parse_DIMACS_main(in, clauses);
}


Cnf parse_DIMACS_CNF(const char *DIMACS_cnf_file) {
  unsigned int i, j;
  Cnf ret;
  int candidate,maxVarIndex;
  gzFile in = gzopen(DIMACS_cnf_file, "rb");
  if (in == NULL) {
    fprintf(stderr, "ERROR! Could not open file: %s\n",
	    DIMACS_cnf_file);
    exit(1);
  }
  parse_DIMACS(in, ret.clauses);
  gzclose(in);

  maxVarIndex = 0;
  for (i = 0; i < ret.clauses.size(); ++i)
    for (j = 0; j < ret.clauses[i].size(); ++j) {
      candidate = abs(ret.clauses[i][j]);
      if (candidate > maxVarIndex) maxVarIndex = candidate;
    }

  ret.variables = maxVarIndex + 1;
  return ret;
}

