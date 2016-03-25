// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>
#include <algorithm>
#include <vector>
#include "Modules.h"

Range findImport(const std::vector<char>& buffer)
{
  const std::string import_keyword = "import";

  auto typeset_it = std::search(buffer.begin(),
    buffer.end(),
    import_keyword.c_str(),
    import_keyword.c_str() + import_keyword.size());

  return std::make_pair(buffer.begin(), buffer.end());
}
