// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef INTENT_FRONTEND_H
#define INTENT_FRONTEND_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <limits>
#include <iostream>
#include <filesystem>
#include <stack>
#include <sstream>

#include "AST.h"
#include "dparse.h"

#include "d.h"
#include "util.h"
#include "lua.hpp"
#include "lauxlib.h"
#include "dparse_tree.h"

#include "Type.h"
#include "ASTRtti.h"
#include "ASTPrinter.h"
#include "NodeLinker.h"
#include "mkdparse.h"
#include "PtablesReader.h"
#include "TypeParser.h"
#include "BuiltinReductions.h"
#include "TypesetMerger.h"
#include "DynamicReductions.h"
#include "MatchVisitor.h"
#include "AST2Code.h"
#include "MatchReferences.h"

namespace Intent {
  namespace FrontEnd {

    void DParser_pass(char* path);
    std::vector<char> readFile(const std::string& file);
    void executeLUA(const std::string& file);
    std::pair<std::string, std::string> compileDL(const std::string& filename, 
                                                  const std::string& modelFile, 
                                                  size_t& grammarSize);
    bool compileHelper(const std::string& fileName,
        const std::string& metamodel,
        const std::string& model,
        std::unordered_map<std::string, reductionf>& builtinRMappings,
        size_t grammarSize,
        const std::string& option);

    void Execute(const std::string& fileName,
        const std::string& metamodel,
        const std::string& model,
        std::unordered_map<std::string, reductionf>& builtinRMappings,
        size_t grammarSize,
        const std::string& option);

    std::unordered_map<std::string, reductionf> make_builtin_reductions();

    void addDynamicReductions(const std::string& fileName,
        const std::string& metamodel,
        const std::string& model,
        std::unordered_map<std::string, reductionf>& builtinRMappings,
        size_t grammarSize,
        const std::string& option);

    std::string visitMatchers(const std::string& fileName,
        const std::string& metamodel,
        const std::string& model,
        std::unordered_map<std::string, reductionf>& builtinRMappings,
        size_t grammarSize,
        const std::string& option);
    
    void Exec(const std::string& compilationUnit, 
        const std::string& modelFile, const std::string& option, bool compileOnly);
  }
}

#endif
