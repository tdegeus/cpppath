# cpppath

Simple, header only, file-path module for C++. 

## Contents

<!-- MarkdownTOC -->

- [Usage](#usage)
- [Overview](#overview)
    - [dirname](#dirname)
    - [filename](#filename)
    - [exists](#exists)
    - [join](#join)
- [Installation](#installation)

<!-- /MarkdownTOC -->

## Usage

This library is header only, so one just has to

```cpp
#include <cpppath.h>
```

Then compile using 

```bash
c++ -I/path/to/cpppath
```

The `-I/path/to/cpppath` can be simplified or often even omitted by ['installing' cpppath](#installation)

## Overview

The following functions are available:

### dirname

```cpp
std::string dirname(const std::string &path);
```

Return the directory from the `path`. Depending on the path, an empty string may be returned.

### filename

```cpp
std::string filename(const std::string &path);
```

Return the filename from the `path`. Depending on the path, an empty string may be returned.

### exists

```cpp
bool exists(const std::string& path);
```

Return `true` is the `path` exists.

### join

```cpp
std::string join(const std::string &a, const std::string &b);
```

Join path components.

## Installation

To install:

```bash
cd /path/to/temp_dir
cmake /path/to/cpppath
make install
```

Thereafter one usually does not have to specify any include path. 

> To install in a custom location (e.g. `~/opt`) use
> 
> ```bash
> cmake .. -DCMAKE_INSTALL_PREFIX:PATH=~/opt
> ```
> 
> To avoid having to specify the include path one can set the following environment variables:
> 
> ```bash
> export CPLUS_INCLUDE_PATH=$HOME/opt/include:$CPLUS_INCLUDE_PATH
> ```
> Usually one adds this line to the `~/.bashrc` (or its equivalent), see for example [this page](https://www.cyberciti.biz/faq/set-environment-variable-linux/).

Should this not work, one can use

```bash
c++ `pkg-config --cflags cpppath`
```

Or using CMake, add the following to your `CMakeLists.txt`:

```cmake
find_package(PkgConfig)

pkg_check_modules(CPPPATH REQUIRED cpppath)
include_directories(${CPPPATH_INCLUDE_DIRS})
```


