// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef MODULES_INTENT_H
#define MODULES_INTENT_H

typedef std::pair<std::vector<char>::const_iterator, std::vector<char>::const_iterator> Range;

Range findImport(const std::vector<char>& buffer);

#endif
