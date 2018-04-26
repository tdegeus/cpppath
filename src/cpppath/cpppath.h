/* =================================================================================================

(c - MIT) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppmat

================================================================================================= */

#ifndef CPPPATH_H
#define CPPPATH_H

// =================================================================================================

#include <algorithm>
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <iso646.h> // to fix a Microsoft Visual Studio error on "and" and "or"

// =================================================================================================

#define CPPPATH_WORLD_VERSION 0
#define CPPPATH_MAJOR_VERSION 0
#define CPPPATH_MINOR_VERSION 1

#define CPPPATH_VERSION_AT_LEAST(x,y,z) \
  (CPPPATH_WORLD_VERSION>x || (CPPPATH_WORLD_VERSION>=x && \
  (CPPPATH_MAJOR_VERSION>y || (CPPPATH_MAJOR_VERSION>=y && \
                               CPPPATH_MINOR_VERSION>=z))))

#define CPPPATH_VERSION(x,y,z) \
  (CPPPATH_WORLD_VERSION==x && \
   CPPPATH_MAJOR_VERSION==y && \
   CPPPATH_MINOR_VERSION==z)

// =================================================================================================

// dummy operation that can be use to suppress the "unused parameter" warnings
#define UNUSED(p) ( (void)(p) )

// =================================================================================================

namespace cpppath {

// -------------------------------------------------------------------------------------------------

inline std::string dirname (const std::string &path);
inline std::string filename(const std::string &path);

inline bool exists(const std::string& fname);

// -------------------------------------------------------------------------------------------------

}

// =================================================================================================

#include "cpppath.cpp"

// =================================================================================================

#endif

