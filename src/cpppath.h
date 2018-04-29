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

// -------------------------------------- version information --------------------------------------

#define CPPPATH_WORLD_VERSION 0
#define CPPPATH_MAJOR_VERSION 0
#define CPPPATH_MINOR_VERSION 2

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

const std::string sep = "/";

inline std::string dirname(const std::string &path);

inline std::string filename(const std::string &path);

inline std::string join(const std::string &a, const std::string &b);

inline bool exists(const std::string& fname);

// ========================================= IMPLEMENATION =========================================

// -------------------------------------------------------------------------------------------------

inline std::string dirname(const std::string &path)
{
  size_t idx = path.find_last_of("/");

  if ( idx == std::string::npos ) return "";

  return path.substr(0, idx);
}

// -------------------------------------------------------------------------------------------------

inline std::string filename(const std::string &path)
{
  size_t idx = path.find_last_of("/");

  if ( idx == std::string::npos ) return path;

  return path.substr(idx+1, path.length());
}

// -------------------------------------------------------------------------------------------------

inline bool exists(const std::string& fname)
{
  std::ifstream file(fname);

  return static_cast<bool>(file);
}

// -------------------------------------------------------------------------------------------------

inline std::string join(const std::string &a, const std::string &b)
{
  if ( a.size() == 0 ) return b;
  if ( b.size() == 0 ) return a;

  if ( strcmp(&a[a.size()-1], &sep[0]) == 0 ) return a+b;

  return a + "/" + b;
}

// =================================================================================================

} // namespace

#endif

