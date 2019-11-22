# cpppath

[![Travis](https://travis-ci.org/tdegeus/cpppath.svg?branch=master)](https://travis-ci.org/tdegeus/cpppath)
[![Build status](https://ci.appveyor.com/api/projects/status/aw19lhd9x6oma9ob?svg=true)](https://ci.appveyor.com/project/tdegeus/cpppath)

Simple, header only, file-path module for C++ similar to `os` in Python. This module is nothing fancy, but it might be helpful to accomplish some simple tasks. 

>   **Disclaimer**
>   
>   This library is free to use under the [MIT license](https://github.com/tdegeus/cpppath/blob/master/LICENSE). Any additions are very much appreciated, in terms of suggested functionality, code, documentation, testimonials, word-of-mouth advertisement, etc. Bug reports or feature requests can be filed on [GitHub](https://github.com/tdegeus/cpppath). As always, the code comes with no guarantee. None of the developers can be held responsible for possible mistakes.
>   
>   Download: [.zip file](https://github.com/tdegeus/cpppath/zipball/master) | [.tar.gz file](https://github.com/tdegeus/cpppath/tarball/master).
>   
>   (c - [MIT](https://github.com/tdegeus/cpppath/blob/master/LICENSE)) T.W.J. de Geus (Tom) | tom@geus.me | www.geus.me | [github.com/tdegeus/cpppath](https://github.com/tdegeus/cpppath)

## Contents

<!-- MarkdownTOC -->

- [Getting cpppath](#getting-cpppath)
    - [Using conda](#using-conda)
    - [From source](#from-source)
- [Usage](#usage)
- [Compiling](#compiling)
    - [By hand](#by-hand)
    - [Using pkg-config](#using-pkg-config)
    - [Using CMake](#using-cmake)
- [Overview](#overview)
    - [cpppath::sep](#cpppathsep)
    - [cpppath::dirname](#cpppathdirname)
    - [cpppath::filename](#cpppathfilename)
    - [cpppath::filebase](#cpppathfilebase)
    - [cpppath::splitext](#cpppathsplitext)
    - [cpppath::ext](#cpppathext)
    - [cpppath::split](#cpppathsplit)
    - [cpppath::join](#cpppathjoin)
    - [cpppath::select](#cpppathselect)
    - [cpppath::normpath](#cpppathnormpath)
    - [cpppath::commonprefix](#cpppathcommonprefix)
    - [cpppath::commondirname](#cpppathcommondirname)
    - [cpppath::exists](#cpppathexists)
    - [cpppath::curdir](#cpppathcurdir)
- [Create a new release](#create-a-new-release)

<!-- /MarkdownTOC -->

# Getting cpppath

## Using conda

```bash
conda install -c conda-forge cpppath
```

## From source

```bash
# Download cpppath
git checkout https://github.com/tdegeus/cpppath.git
cd cpppath

# For CMake or pkg-config use
cmake .
make install
```

# Usage

This library is header only, so one just has to

```cpp
#include <cpppath.h>
```

and make sure that the header is in the include path of the compiler. See below.

Consider this micro-example:

```cpp
#include <cpppath.h>

int main()
{
    std::cout << cpppath::join({"path", "to", "foo", "bar.txt"}) << std::endl;
    return 0;
}
```

which will print 

* Unix: `"path/to/foo/bar.txt"`
* Windows: `"path\to\foo\bar.txt"`

# Compiling

## By hand

Presuming that the compiler is `c++`, compile using:

```
c++ -I/path/to/cpppath/include ...
```

## Using pkg-config

Presuming that the compiler is `c++`, compile using:

```
c++ `pkg-config --cflags cpppath` ...
```

## Using CMake

The `CMakeLists.txt` can be as follows

```cmake
cmake_minimum_required(VERSION 3.1)

project(example)

find_package(cpppath REQUIRED)

add_library(example main.cpp)

target_link_libraries(example
    cpppath)
```

Compilation (on Unix) can then proceed using 

```bash
cmake .
make
```

# Overview

## cpppath::sep

Get OS's separator.

* Unix: "/"
* Windows: "\\"

## cpppath::dirname

Get dirname part of a path. 
Depending on the path, an empty string may be returned.

Example: "/path/to/foo/bar.txt" returns "/path/to/foo"

## cpppath::filename

Get filename part of a path. 
Depending on the path, an empty string may be returned.

Example: "/path/to/foo/bar.txt" returns "bar.txt"

## cpppath::filebase

Get filename part of a path, *without extension*.
Depending on the path, an empty string may be returned.

Example: "/path/to/foo/bar.txt" returns "bar"

## cpppath::splitext

Split the pathname path into a pair (root, ext) such that root + ext == path,
and ext is empty or begins with a period and contains at most one period.
Leading periods on the basename are ignored; splitext(".cshrc") returns {".cshrc", ""}.

## cpppath::ext

Get the extension of a path.
Depending on the path, an empty string may be returned.

Example: "/path/to/foo/bar.txt" returns "txt"

## cpppath::split

Split sub-paths using the separator. The output is a list of path components. 

Optionally the list can be sliced as split(path)\[begin: end\]. Negative indices may be used to that count from the right (instead of from the left).

## cpppath::join

Join path components using separator.
Provides option to prepend the output string with the separator.

## cpppath::select

Selection of sub-paths (see `split`). Negative indices may be used to that count from the right (instead of from the left).

Example: select("/path/to/foo/bar.txt", 2) returns "foo/bar.txt"

Example: select("/path/to/foo/bar.txt", 2, 3) returns "foo"

## cpppath::normpath

Normalize a path by collapsing redundant separators and up-level references so that `A//B`, `A/B/`, `A/./B` and `A/foo/../B` all become `A/B`. This string manipulation may change the meaning of a path that contains symbolic links.

## cpppath::commonprefix

Select the common part of a list of strings. For example:

```cpp
std::vector<std::string> paths = {"/path/to/id=000/file.txt", "/path/to/id=001/file.txt"};

  std::cout << cpppath::commonprefix(paths) << std::endl;
```

returns `"/path/to/id=00"`.

## cpppath::commondirname

Select the common path of a list of paths. For example:

```cpp
std::vector<std::string> paths = {"/path/to/id=000/file.txt", "/path/to/id=001/file.txt"};

std::cout << cpppath::commondirname(paths) << std::endl;
```

returns `"/path/to"`.

This can also be used to select the part of the paths that in unique to each string. For example:

```cpp
std::vector<std::string> paths = {"/path/to/id=000/file.txt", "/path/to/id=001/file.txt"};

std::cout << cpppath::split(paths[0], cpppath::commondirname(paths)+"/")[0] << std::endl;
```

returns `"id=000/file.txt"`.

## cpppath::exists

Returns `true` is the `path` exists.

## cpppath::curdir

The current working directory.

# Create a new release

1.  Update the version number in `include/cpppath.h`. 

2.  Upload the changes to GitHub and create a new release there (with the correct version number).

3.  Update the package at [conda-forge](https://github.com/conda-forge/cpppath-feedstock).
