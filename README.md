# intelligent_cast

A simple c++11 type converter between string and numeric value.


### Basic usage
```c++

#include "IntelligentCast.h"

// convert to string from integer 
std::string converted_from_char = intelligent_cast<std::string>(100);

// convert to integer from string
int converted_from_wchar = intelligent_cast<int>("100");
```

All of a combination of the two from below is possible.

|               |                         |       |
|:-----------:       |:------------------:|:------------:|
| char               | unsigned char      |   float      |
| int                | unsigned int       |   double     |
| short              | unsigned short     | std::string  |
| long               | unsigned long      | std::wstring |
| long long          | unsigned long long |     -        |


### Advance usage

Including "OperatorOverloadExtension.h", a combination of operator+ of several types is available.

such as:

```c++
#include "IntelligentCast.h"
#include "OperatorOverloadExtension.h"

//  std::string + integer
std::string str="results are ";
std::string converted = str + 10 + " and " + 5.5;
// converted should be "results are 10 and 5.5"
```
