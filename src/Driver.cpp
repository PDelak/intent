// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "FrontEnd.h"

extern std::string compilationUnit;

int help()
{
    std::cerr << std::endl;
    std::cerr << "intent.exe" << std::endl;
    std::cerr << "\tfilename [command]" << std::endl;
    std::cerr << "\t-s metamodel model" << std::endl;
    return -1;
}

bool notValidArgNumbers(int argc)
{
    return argc < 2 || argc > 4;
}

int main(int argc, char *argv[]) { 

  if (notValidArgNumbers(argc)) return help();

  std::string option;
  std::string modelFile;

  switch (argc)
  {
  case 2:
      compilationUnit = argv[1];
      break;
  case 3:
      option = argv[2];
      compilationUnit = argv[1];
      break;
  case 4:
      option = argv[1];
      if (option != "-s") return help();
      modelFile = argv[3];
      compilationUnit = argv[2];
      break;

  default:
      break;
  }
    
  try {
      Intent::FrontEnd::Exec(compilationUnit, modelFile, option, option == "gen");
  }  
  catch (const FileNotFoundException& fe) { std::cerr << fe.what() << std::endl; }
  catch (const SymbolNotFound& snf) { std::cerr << snf.what() << std::endl; }
  catch (const SyntaxError& se) { std::cerr << se.what() << std::endl;}
  catch (const SemanticError& semaError) { std::cerr << semaError.what() << std::endl; }
  catch (const std::runtime_error& re) { std::cerr << re.what() << std::endl; }
  catch (...) { std::cerr << "unknown error" << std::endl; }
  
  return 0;
}
