# intelligent_cast

A simple c++11 type converter between string and numeric value.


###Basic usage
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

Especially following conversions are also possible.

```c++

//  std::string to std::wstring
std::string str("abcd");
std::wstring converted_from_char = intelligent_cast<std::wstring>(str);
// internally static_cast<double> is applied
int val = 100;
double converted_from_wchar = intelligent_cast<double>(val);
```
###Advance usage

