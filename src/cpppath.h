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
#define CPPPATH_MINOR_VERSION 6

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

// join sub-paths together using the separator
inline std::string join(const std::vector<std::string> &paths);
inline std::string join(const std::string &a, const std::string &b);

// split sub-paths using the separator (N.B. if "end==0" the upper bound in the last index)
inline std::vector<std::string> split(const std::string& path);
inline std::vector<std::string> split(const std::string& path, int start, int end=0);

// select path of a path
inline std::string select(const std::string& path, int start, int end=0);

// normalize a path by collapsing redundant separators and up-level references
inline std::string normpath(const std::string &path);

// the current working directory
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

    if      ( path[0]           == sep[0] ) out += path;
    else if ( out[out.size()-1] == sep[0] ) out += path;
    else                                    out += sep + path;
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
  int N = paths.size();

  // implicit assumption
  if ( end  == 0 ) end   = N;

  // allow negative indices, counting from the end
  if ( begin < 0 ) begin = (N+(begin%N)) % N;
  if ( end   < 0 ) end   = (N+(end  %N)) % N;

  // allocate output
  std::vector<std::string> out;

  // select path components
  for ( int i = begin ; i < end ; ++i ) out.push_back(paths[i]);

  return out;
}

// -------------------------------------------------------------------------------------------------

inline std::string select(const std::string& path, int begin, int end)
{
  // set prefix (retains "/" if needed)
  // - allocate
  std::string prefix = "";
  // - set
  if ( path[0] == sep[0] ) prefix = "/";

  return prefix + join(split(path,begin,end));
}

// -------------------------------------------------------------------------------------------------

inline std::string normpath(const std::string &path)
{
  // retain "/" prefix
  bool root = path[0] == sep[0];

  // list of path components (this removes already all "//")
  std::vector<std::string> paths = split(path);

  // filter "."
  {
    // - temporary list of path components
    std::vector<std::string> tmp;
    // - copy, if not "."
    for ( auto &i : paths )
      if ( i != "." )
        tmp.push_back(i);
    // - rename temporary variable
    paths = tmp;
  }

  // filter "foo/../"
  {
    // - loop to find all occurrences
    while ( true )
    {
      // -- logical, break if ".." not found (below)
      bool found = false;
      // -- loop to find "..", if found collapse if with the preceding path component
      for ( size_t i = 1 ; i < paths.size() ; ++i )
      {
        if ( paths[i] == ".." )
        {
          // --- temporary list of path components
          std::vector<std::string> tmp;
          // --- copy selection
          for ( size_t j = 0 ; j < paths.size() ; ++j )
            if ( j != i and j != i-1 )
              tmp.push_back(paths[j]);
          // --- rename temporary variable
          paths = tmp;
          // --- signal to continue searching
          found = true;
          // --- loop from the beginning of the path
          break;
        }
      }
      // -- break when no ".." are found
      if ( !found ) break;
    }
  }

  // return path
  if ( root ) return "/"+join(paths);
  else        return     join(paths);
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

