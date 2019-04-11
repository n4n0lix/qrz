#include "file_utils.h"

namespace file_utils {

  string load_file(string path)
  {
    std::ifstream file(path);

    if (file)
    {
      std::stringstream buffer;
      buffer << file.rdbuf();
      file.close();

      return buffer.str();
    }

    return "";
  }

}