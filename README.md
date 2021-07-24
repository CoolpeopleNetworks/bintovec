# bintovec - a file to C++ code generator.

bintovec is a tool that is used to turn any file (picture, audio, text, etc) into C++20 source code (specifically into a std::vector<unsigned char>).

## Prerequisites:
1) A compiler compatible with C++20 (tested on Apple Clang 12 and Microsoft Visual Studio 2019)

NOTE: The source files created by this tool also require C++20.

## Usage
```shell
Usage: bintovec [options] input_filename output_filename 

Positional arguments:
input_filename  	The input file to turn into a C++ vector<>
output_filename 	The resulting files containing the C++ representation of the input file.

Optional arguments:
-h --help       	show this help message and exit
-n --name       	Specifies the C++ variable name that is used in the output file
-ns --namespace 	Specifies the C++ namespace the resulting vector will be placed into
```

## Examples
As an example, here's how to create C++ source code from a JPG photo:
```shell
$ bintovec photo.jpg photo_source_code
```

This results in two files:
    photo_source_code.hpp
    photo_source_code.cpp

These two files contain a declaration and definition of a std::vector<unsigned char> that is initialized with the contents of the input file (in this case, photo.jpg):

```shell
$ cat photo_source_code.hpp
// Generated file - Do Not Modify
#include <vector>

extern const std::vector<unsigned char> photo_source_code;

$ cat photo_source_code.cpp
// Generated file - Do Not Modify
#include <vector>

const std::vector<unsigned char> photo_source_code =
{
    0x23, 0x20, 0x62, 0x69, 0x6e, 0x74, 0x6f, 0x76,
    0x65, 0x63, 0x0a, 0x0a, 0x62, 0x69, 0x6e, 0x74,
    0x6f, 0x76, 0x65, 0x63, 0x20, 0x69, 0x73, 0x20,
    0x61, 0x20, 0x74, 0x6f, 0x6f, 0x6c, 0x20, 0x74,
    0x68, 0x61, 0x74, 0x20, 0x69, 0x73, 0x20, 0x75,
    0x73, 0x65, 0x64, 0x20, 0x74, 0x6f, 0x20, 0x74,
    0x75, 0x72, 0x6e, 0x20, 0x61, 0x6e, 0x79, 0x20,
    0x66, 0x69, 0x6c, 0x65, 0x20, 0x28, 0x70, 0x69,
    0x63, 0x74, 0x75, 0x72, 0x65, 0x2c, 0x20, 0x61,
    ...<snip>...
    0x73, 0x75, 0x6c, 0x74, 0x69, 0x6e, 0x67, 0x20,
    0x76, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x20, 0x77,
    0x69, 0x6c, 0x6c, 0x20, 0x62, 0x65, 0x20, 0x70,
    0x6c, 0x61, 0x63, 0x65, 0x64, 0x20, 0x69, 0x6e,
    0x74, 0x6f, 0x0a, 0x60, 0x60, 0x60, 0x0a, 0x0a,
    0x23, 0x23, 0x20, 0x45, 0x78, 0x61, 0x6d, 0x70,
    0x6c, 0x65, 0x73, 0x0a
};
```

## Specifying the name of the C++ variable created
The name of the variable created can be specified using the '-n' argument.
Note: The variable name given will be modified to conform to C++ variable specifications (spaces removed, must start with an alphabetic character or underline, etc)

```shell
$ bintovec photo.jpg photo_source_code -n NeatPhoto

$ cat photo_source_code.hpp
// Generated file - Do Not Modify
#include <vector>

extern const std::vector<unsigned char> NeatPhoto;

```

## Specifying the namespace of the C++ variable created
The namespace of the variable created can be specified using the '-ns' argument.
```shell
$ bintovec photo.jpg photo_source_code -ns Photos

$ cat photo_source_code.hpp
// Generated file - Do Not Modify
#include <vector>

namespace Photos
{
    extern const std::vector<unsigned char> photo_source_code;
}
```

## Notes:
