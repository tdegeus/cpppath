/* =================================================================================================

(c - MIT) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | github.com/tdegeus/cppmat

================================================================================================= */

#ifndef CPPPATH_H
#define CPPPATH_H

#include <assert.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <tuple>
#include <stdio.h>
#include <unistd.h>

#define CPPPATH_VERSION_MAJOR 0
#define CPPPATH_VERSION_MINOR 1
#define CPPPATH_VERSION_PATCH 0

#define CPPPATH_VERSION_AT_LEAST(x,y,z) \
  (CPPPATH_VERSION_MAJOR > x || (CPPPATH_VERSION_MAJOR >= x && \
  (CPPPATH_VERSION_MINOR > y || (CPPPATH_VERSION_MINOR >= y && \
                                 CPPPATH_VERSION_PATCH >= z))))

#define CPPPATH_VERSION(x,y,z) \
  (CPPPATH_VERSION_MAJOR == x && \
   CPPPATH_VERSION_MINOR == y && \
   CPPPATH_VERSION_PATCH == z)

// =================================================================================================

namespace cpppath {

// =========================================== OVERVIEW ============================================

// get dirname part of a path

inline std::string dirname(
    const std::string& path,
    const std::string& sep="/");

// get filename part of a path

inline std::string filename(
    const std::string& path,
    const std::string& sep="/");

inline std::string filebase(
    const std::string& path,
    const std::string& sep="/",
    const std::string& ext=".");

// join sub-paths together using the separator
// provides option to prepend the output string with the separator

inline std::string join(
    const std::vector<std::string>& paths,
    const std::string& sep="/");

inline std::string join(
    const std::vector<std::string>& paths,
    bool preprend,
    const std::string& sep="/");

inline std::string join(
    const std::vector<std::string>& paths,
    const char* sep);

inline std::string join(
    const std::vector<std::string>& paths,
    bool preprend,
    const char* sep);

// split sub-paths using the separator (N.B. if "end == 0" the upper bound in the last index)

inline std::vector<std::string> split(
    const std::string& path,
    const std::string& sep="/");

inline std::vector<std::string> split(
    const std::string& path,
    int start,
    int end=0,
    const std::string& sep="/");

// select path of a path

inline std::string select(
    const std::string& path,
    int start,
    int end=0,
    const std::string& sep="/");

// normalize a path by collapsing redundant separators and up-level references

inline std::string normpath(
    const std::string& path,
    const std::string& sep="/");

// common prefix in a list of strings

inline std::string common_prefix(
    const std::vector<std::string>& paths);

// common dirname in a list of paths

inline std::string common_dirname(
    const std::vector<std::string>& paths);

// return the current working directory

inline std::string curdir();

// check if a path exists

inline bool exists(
    const std::string& path);

// ========================================= IMPLEMENATION =========================================

inline std::string dirname(
    const std::string& path,
    const std::string& sep)
{
  size_t idx = path.find_last_of(sep);

  if (idx == std::string::npos) {
      return "";
  }

  return path.substr(0, idx);
}

// -------------------------------------------------------------------------------------------------

inline std::string filename(
    const std::string& path,
    const std::string& sep)
{
    size_t idx = path.find_last_of(sep);

    if (idx == std::string::npos) {
        return path;
    }

    return path.substr(idx+1, path.length());
}

// -------------------------------------------------------------------------------------------------

inline std::string filebase(
    const std::string& path,
    const std::string& sep,
    const std::string& ext)
{
    std::string out = filename(path, sep);

    size_t idx = out.find_first_of(ext);

    if (idx == std::string::npos) {
        return out;
    }

    return out.substr(0, idx);
}

// -------------------------------------------------------------------------------------------------

inline std::string join(
    const std::vector<std::string>& paths,
    const std::string& sep)
{
    if (paths.size() == 1) {
        return paths[0];
    }

    std::string out = "";

    for (auto path: paths) {

        if (out.size() == 0) {
            out += path;
            continue;
        }

        if (path[0] == sep[0]) {
            out += path;
        }
        else if (out[out.size() - 1] == sep[0]) {
            out += path;
        }
        else {
            out += sep + path;
        }
    }

    return out;
}

// -------------------------------------------------------------------------------------------------

inline std::string join(
    const std::vector<std::string>& paths,
    bool preprend,
    const std::string& sep)
{
    if (preprend) {
        return sep + join(paths, sep);
    }

    return join(paths, sep);
}

// -------------------------------------------------------------------------------------------------

inline std::string join(
    const std::vector<std::string>& paths,
    const char *sep)
{
    return join(paths, std::string(sep));
}

// -------------------------------------------------------------------------------------------------

inline std::string join(
    const std::vector<std::string>& paths,
    bool preprend,
    const char *sep)
{
    return join(paths, preprend, std::string(sep));
}

// -------------------------------------------------------------------------------------------------

inline std::vector<std::string> split(
    const std::string& path,
    const std::string& sep)
{
    std::vector<std::string> out;

    size_t prev = 0;
    size_t pos = 0;

    do {
        // find next match (starting from "prev")
        pos = path.find(sep, prev);

        // no match found -> use length of string as 'match'
        if (pos == std::string::npos) {
            pos = path.length();
        }

        // get sub-string
        std::string token = path.substr(prev, pos - prev);

        // store sub-string in list
        if (!token.empty()) {
            out.push_back(token);
        }

        // move further
        prev = pos + sep.length();
    }
    while (pos < path.length() && prev < path.length());

    return out;
}

// -------------------------------------------------------------------------------------------------

inline std::vector<std::string> split(
    const std::string& path,
    int begin,
    int end,
    const std::string& sep)
{
    std::vector<std::string> paths = split(path, sep);

    int N = paths.size();

    // automatically set the length

    if (end == 0) {
        end = N;
    }

    // convert negative indices that count from the end

    if (begin < 0) {
        begin = (N + (begin % N)) % N;
    }

    if (end < 0) {
        end = (N + (end % N)) % N;
    }

    // select path components

    std::vector<std::string> out;

    for (int i = begin; i < end; ++i) {
        out.push_back(paths[i]);
    }

    return out;
}

// -------------------------------------------------------------------------------------------------

inline std::string select(
    const std::string& path,
    int begin,
    int end,
    const std::string& sep)
{
    std::string prefix = "";

    if (path[0] == sep[0]) {
        prefix = "/";
    }

    return prefix + join(split(path, begin, end), sep);
}

// -------------------------------------------------------------------------------------------------

inline std::string normpath(
    const std::string& path,
    const std::string& sep)
{
    bool root = path[0] == sep[0];

    // list of path components (this removes already all "//")
    std::vector<std::string> paths = split(path, sep);

    // filter "."
    {
        std::vector<std::string> tmp;

        for (auto& i: paths) {
            if (i != ".") {
                tmp.push_back(i);
            }
        }

        paths = tmp;
    }

    // filter "foo/../"
    {
        while (true) {

            bool found = false;

            for (size_t i = 1; i < paths.size(); ++i) {

                if (paths[i] == "..") {

                    std::vector<std::string> tmp;

                    for (size_t j = 0; j < paths.size(); ++j) {
                        if (j != i && j != i - 1) {
                            tmp.push_back(paths[j]);
                        }
                    }

                    paths = tmp;
                    found = true;
                    break;
                }
            }

            if (!found) {
                break;
            }
        }
    }

    if (root) {
        return sep + join(paths, sep);
    }

    return join(paths, sep);
}

// -------------------------------------------------------------------------------------------------

namespace detail
{
    bool all_equal(const std::vector<std::string>& paths, size_t i)
    {
        for (size_t j = 1; j < paths.size(); ++j) {
            if (paths[0][i] != paths[j][i]) {
                return false;
            }
        }
        return true;
    }
}

// -------------------------------------------------------------------------------------------------

inline std::string common_prefix(
    const std::vector<std::string>& paths)
{
    if (paths.size() == 0) {
        return "";
    }

    size_t i;
    size_t n = paths[0].size();

    for (auto& path: paths) {
        if (path.size() < n) {
            n = path.size();
        }
    }

    for (i = 0; i < n; ++i) {
        if (detail::all_equal(paths, i)) {
            ++i;
        }
        else {
            break;
        }
    }

    return paths[0].substr(0, i);
}

// -------------------------------------------------------------------------------------------------

inline std::string common_dirname(
    const std::vector<std::string>& paths)
{
    return dirname(common_prefix(paths));
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

inline bool exists(
    const std::string& path)
{
    std::ifstream file(path);
    return static_cast<bool>(file);
}

// =================================================================================================

} // namespace

#endif

