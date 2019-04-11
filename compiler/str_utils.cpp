#include "str_utils.h"

bool str_utils::ends_with(string str, string ending)
{
  if (str.length() >= ending.length()) {
    return (0 == str.compare(str.length() - ending.length(), ending.length(), ending));
  }
  else {
    return false;
  }
}
