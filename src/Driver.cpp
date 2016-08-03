// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "FrontEnd.h"

extern std::string compilationUnit;

using namespace Intent::FrontEnd;

int help()
{
    std::cerr << std::endl;
    std::cerr << "intent.exe" << std::endl;
    std::cerr << "\tfilename [command]" << std::endl;
    std::cerr << "\t-s metamodel model" << std::endl;
    return -1;
}

int main(int argc, char *argv[]) { 

  auto builtinRMappings = make_builtin_reductions();
  std::string option;
  std::string modelFile;

  if (argc < 2 || argc > 4) return help();
  else if(argc == 4) {
      option = argv[1];
      if (option != "-s") return help();

      modelFile = argv[3];
      compilationUnit = argv[2];
  } else if(argc == 3) {
      std::string fileName = argv[1];
      option = argv[2];
      compilationUnit = fileName;
  }
  else if (argc == 2) {
      std::string fileName = argv[1];
      compilationUnit = fileName;
  }


  bool showTree = false;
  bool compileOnly = false;
  
  if (option.compare("tree") == 0) showTree = true;
  if (option.compare("gen") == 0) compileOnly = true;

  try {
    size_t grammarSize = 0;
    std::string metamodel;
    std::string model;
    std::tie(metamodel, model) = compileDL(compilationUnit, modelFile, grammarSize);
    
    DParser_pass("tmp.g");
    addDynamicReductions(compilationUnit, "tmp.g.d_parser.c", model, builtinRMappings, grammarSize, option);
    std::string codeGen = visitMatchers(compilationUnit, "tmp.g.d_parser.c", model, builtinRMappings, 0, option);
    if(!compileOnly) Execute(compilationUnit, "tmp.g.d_parser.c", codeGen, builtinRMappings, 0, option);
  }  
  catch (const FileNotFoundException& fe) { std::cerr << fe.what() << std::endl; }
  catch (const SymbolNotFound& snf) { std::cerr << snf.what() << std::endl; }
  catch (const SyntaxError& se) { std::cerr << se.what() << std::endl;}
  catch (const SemanticError& semaError) { std::cerr << semaError.what() << std::endl; }
  catch (const std::runtime_error& re) { std::cerr << re.what() << std::endl; }
  catch (...) { std::cerr << "unknown error" << std::endl; }
  
  return 0;
}
