/* =================================================================================================

(c - MIT) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppmat

================================================================================================= */

#ifndef CPPPATH_H
#define CPPPATH_H

// ==================================== PREPROCESSOR DIRECTIVES ====================================

// ---------------------------------------- load libraries -----------------------------------------

#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <tuple>
#include <stdio.h>
#include <unistd.h>

// -------------------------------------- version information --------------------------------------

#define CPPPATH_WORLD_VERSION 0
#define CPPPATH_MAJOR_VERSION 0
#define CPPPATH_MINOR_VERSION 3

#define CPPPATH_VERSION_AT_LEAST(x,y,z) \
  (CPPPATH_WORLD_VERSION>x || (CPPPATH_WORLD_VERSION>=x && \
  (CPPPATH_MAJOR_VERSION>y || (CPPPATH_MAJOR_VERSION>=y && \
                               CPPPATH_MINOR_VERSION>=z))))

#define CPPPATH_VERSION(x,y,z) \
  (CPPPATH_WORLD_VERSION==x && \
   CPPPATH_MAJOR_VERSION==y && \
   CPPPATH_MINOR_VERSION==z)

// -------------------------- contain everything in the namespace cpppath --------------------------

namespace cpppath {

// =========================================== OVERVIEW ============================================

// path-separator
const std::string sep = "/";

// get dirname/filename part of a path
inline std::string dirname (const std::string &path);
inline std::string filename(const std::string &path);

// join sub-paths together, using the separator
inline std::string join(const std::vector<std::string> &paths);
inline std::string join(const std::string &a, const std::string &b);

// split sub-paths, using the separator
inline std::vector<std::string> split(const std::string& path);
inline std::vector<std::string> split(const std::string& path, int start, int end);

// return the current working directory
inline std::string curdir();

// check if a path exists
inline bool exists(const std::string& path);

// ========================================= IMPLEMENATION =========================================

// -------------------------------------------------------------------------------------------------

inline std::string dirname(const std::string &path)
{
  size_t idx = path.find_last_of(sep);

  if ( idx == std::string::npos ) return "";

  return path.substr(0, idx);
}

// -------------------------------------------------------------------------------------------------

inline std::string filename(const std::string &path)
{
  size_t idx = path.find_last_of(sep);

  if ( idx == std::string::npos ) return path;

  return path.substr(idx+1, path.length());
}

// -------------------------------------------------------------------------------------------------

inline std::string join(const std::vector<std::string> &paths)
{
  if ( paths.size() == 1 ) return paths[0];

  std::string out = "";

  for ( auto path : paths )
  {
    if ( out.size() == 0 )
    {
      out += path;
      continue;
    }

    if      ( strcmp(&path[0]          , &sep[0]) == 0 ) out += path;
    else if ( strcmp(&out[out.size()-1], &sep[0]) == 0 ) out += path;
    else                                                 out += sep + path;
  }

  return out;
}

// -------------------------------------------------------------------------------------------------

inline std::string join(const std::string &a, const std::string &b)
{
  return join({a,b});
}

// -------------------------------------------------------------------------------------------------

inline std::vector<std::string> split(const std::string& path)
{
  // allocate output
  std::vector<std::string> out;

  // initialize indices
  size_t prev = 0, pos = 0;

  // loop over the string
  do
  {
    // - find next match (starting from "prev")
    pos = path.find(sep, prev);

    // - no match found -> use length of string as 'match'
    if ( pos == std::string::npos ) pos = path.length();

    // - get sub-string
    std::string token = path.substr(prev, pos-prev);

    // - store sub-string in list
    if ( !token.empty() ) out.push_back(token);

    // - move further
    prev = pos + sep.length();
  }
  while ( pos < path.length() and prev < path.length() );

  // return output
  return out;
}

// -------------------------------------------------------------------------------------------------

inline std::vector<std::string> split(const std::string& path, int begin, int end)
{
  // list of path components
  std::vector<std::string> paths = split(path);

  // number of path components
  size_t N = paths.size();

  // allow negative indices, counting from the end
  // N.B. -1 -> N (not zero)
  if ( begin == -1 ) begin = N; else begin = (N+(begin%N)) % N;
  if ( end   == -1 ) end   = N; else end   = (N+(end  %N)) % N;

  // allocate output
  std::vector<std::string> out;

  // select path components
  for ( int i = begin ; i < end ; ++i ) out.push_back(paths[i]);

  return out;
}

// -------------------------------------------------------------------------------------------------

inline std::string curdir()
{
  char buff[FILENAME_MAX];

  getcwd(buff, FILENAME_MAX);

  std::string cwd(buff);

  return cwd;
}

// -------------------------------------------------------------------------------------------------

inline bool exists(const std::string& path)
{
  std::ifstream file(path);

  return static_cast<bool>(file);
}

// =================================================================================================

} // namespace

#endif

