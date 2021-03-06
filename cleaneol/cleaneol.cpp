// cleaneol.cpp : Main
//
// Detects mixed CRLF vs LF line endings.
//
// Command line arguments:
// If just a file name is given,
// just check the file given on the command line.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>


int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cerr << "Missing source file name" << std::endl;
    return 1;
  }
  char const* const infile = argv[--argc];
  // Open the file in binary mode.
  std::ifstream inf(infile, std::ios::binary | std::ios::in);
  if (!inf.is_open())
  {
    std::cerr << "failed to open: " << infile << std::endl;
    return 2;
  }

  char buf[4096] = { 0 };
  std::streamsize n = 0;
  enum EOL_State { cr, lf };
  EOL_State eol = lf;
  std::streamsize endings[2] = { 0, 0 };

  do
  {
    inf.read(buf, sizeof(buf));
    n = inf.gcount();
    if (n > 0)
    {
      for (std::streamsize i = 0; i < n; ++i)
      {
        if (buf[i] == '\r')
        {
          if (lf == eol)
          {
            eol = cr;
          }
          ++endings[eol];
        }
        else if (buf[i] == '\n')
        {
          if (cr == eol)
          {
            eol = lf;
            ++endings[eol];
          }
          else
          {
            ++endings[eol];
          }
        }
      }
    }
  } while (inf);

  std::cout << "CR: " << endings[cr] << std::endl;
  std::cout << "LF: " << endings[lf] << std::endl;

  return 0;
}
