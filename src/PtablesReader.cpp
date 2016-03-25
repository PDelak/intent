// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <fstream>
#include <memory>
#include <sstream>

#include "PtablesReader.h"


D_ParserTablesReader::D_ParserTablesReader()
  : currentLine(1)
  , parserTablesHolder(new ParserTablesHolder)
{
  rules.push("D_Reduction", D_REDUCTION_ID);
  rules.push("D_Shift", D_SHIFT_ID);
  rules.push("D_State", D_STATE_ID);
  rules.push("D_Symbol", D_SYMBOL);
  rules.push("D_ParserTables", D_PARSE_TABLES);
  rules.push("D_RightEpsilonHint", D_RIGHT_EPSILON_HINT);
  rules.push("D_ErrorRecoveryHint", D_ERROR_RECOVERY_HINT);
  rules.push("SB_uint8", SBUINT8);
  rules.push("SB_uint16", SBUINT16);
  rules.push("SB_uint32", SBUINT32);
  rules.push("void", VOID_KEYWORD);
  rules.push("sizeof", SIZEOF_KEYWORD);
  rules.push("SB_trans_uint8", SB_TRANS_UINT8);
  rules.push("SB_trans_uint16", SB_TRANS_UINT16);
  rules.push("SB_trans_uint32", SB_TRANS_UINT32);
  rules.push("d_final_reduction_code_[0-9]+_[0-9]+_gram", REDUCTION_CODE);
  rules.push("\\*([a-zA-Z0-9_])+\\*", REDUCTION_CODE_KEY);
  rules.push("\\\"([ $a-zA-Z0-9_!<>{}&:;\\|\\(\\)\\[\\]\\+\\-=\\/\\.\\\\,\\?*\\^\\\\\\\"#@]+)\\\"", SYMBOL_DEF);
  rules.push("\\,", COMMA);
  rules.push("=", ASSIGNMENT);
  
  rules.push("\\{", OPEN_BRACE);
  rules.push("\\}", CLOSE_BRACE);

  rules.push("\\[", OPEN_BRACKET);
  rules.push("\\]", CLOSE_BRACKET);
  
  rules.push("\\(", OPEN_PARENTHESIS);
  rules.push("\\)", CLOSE_PARENTHESIS);

  rules.push("&", ADDRESS);

  rules.push(";", SEMICOLON);
  rules.push("\\*\\*", POINTERTOPOINTER);
  rules.push("\\*", POINTER);
  rules.push("unsigned", UNSIGNED_TYPE);
  rules.push("char", CHAR_TYPE);

  rules.push("-?(0x)?([a-fA-F]+)?([0-9a-fA-F]+)",         DIGIT);
  rules.push("([a-zA-Z0-9_]+)",  TEXTORID);

  rules.push("\r|\n|\r\n",       EOL);

  lexertl::generator::build (rules, sm);
  parserTablesHolder->addressSymbolTable[0] = "NULL";
}

std::string D_ParserTablesReader::langElemToString(LangElem elem)
{
  std::string value;
  switch (elem)
  {
  case D_ParserTablesReader::DIGIT:
    value = "digit";
    break;
  case D_ParserTablesReader::TEXTORID:
    value = "text";
    break;
  case D_ParserTablesReader::COLON:
    value = "";
    break;
  case D_ParserTablesReader::D_REDUCTION_ID:
    value = "D_Reduction_id";
    break;
  case D_ParserTablesReader::D_SHIFT_ID:
    value = "D_Shift_id";
    break;
  case D_ParserTablesReader::ASSIGNMENT:
    value = "=";
    break;
  case D_ParserTablesReader::OPEN_COMMENT:
    value = "/*";
    break;
  case D_ParserTablesReader::CLOSE_COMMENT:
    value = "*/";
    break;
  case D_ParserTablesReader::OPEN_BRACE:
    value = "{";
    break;
  case D_ParserTablesReader::CLOSE_BRACE:
    value = "}";
    break;
  case D_ParserTablesReader::OPEN_BRACKET:
    value = "[";
    break;
  case D_ParserTablesReader::CLOSE_BRACKET:
    value = "]";
    break;
  case D_ParserTablesReader::COMMA:
    value = ",";
    break;
  case D_ParserTablesReader::SEMICOLON:
    value = ";";
    break;
  case D_ParserTablesReader::EOL:
    break;
  default:
    break;
  }
  return value;
}

bool D_ParserTablesReader::isEOL(const lexertl::recursive_match_results<const char* >& results, bool eof)
{
  return !eof && results.id == EOL;
}

void D_ParserTablesReader::getNextToken(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  getNextTokenHelper(results,eof);
  while(isEOL(results,eof)) 
  {
    ++currentLine;
    getNextTokenHelper(results,eof); 
  }
}

void D_ParserTablesReader::parseComment(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  do
  {
      getNextTokenHelper(results, eof);
      if (isEOL(results, eof)) ++currentLine;
  } while (results.id != CLOSE_COMMENT);
}

D_ReductionPtr D_ParserTablesReader::parseReduction(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  /*
  unsigned short	nelements;
  unsigned short	symbol;
  D_ReductionCode	speculative_code;
  D_ReductionCode	final_code;
  unsigned short	op_assoc;
  unsigned short	rule_assoc;
  int 			op_priority;
  int 			rule_priority;
  int			action_index;
  int			npass_code;
  D_ReductionCode	*pass_code;
  */

  D_ReductionPtr reduction(new D_Reduction);

  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  reduction->nelements = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected nelements"));
  getToken<COMMA>(results, eof, "expected ','");
  reduction->symbol = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected symbol"));
  getToken<COMMA>(results, eof, "expected ','");
  getToken<TEXTORID>(results, eof, "expected speculative_code");
  reduction->speculative_code = nullptr;
  getToken<COMMA>(results, eof, "expected ','");
  //getToken<TEXTORID>(results, eof, "expected final_code");
  getNextToken(results, eof);
  std::string funName = results.str();
  if (funName == "NULL") reduction->final_code = nullptr;  
  else {    
    auto it = parserTablesHolder->builtinReductionMap.find(funName);
    if (it != parserTablesHolder->builtinReductionMap.end())
      reduction->final_code = (D_ReductionCode)it->second;
    else 
      reduction->final_code = nullptr;
  }
  getToken<COMMA>(results, eof, "expected ','");
  reduction->op_assoc = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected op_assoc"));
  getToken<COMMA>(results, eof, "expected ','");
  reduction->rule_assoc = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected rule_assoc"));
  getToken<COMMA>(results, eof, "expected ','");
  reduction->op_priority = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected op_priority"));
  getToken<COMMA>(results, eof, "expected ','");
  reduction->rule_priority = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected rule_priority"));
  getToken<COMMA>(results, eof, "expected ','");
  reduction->action_index = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected action_index"));
  getToken<COMMA>(results, eof, "expected ','");
  reduction->npass_code = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected npass_code"));
  getToken<COMMA>(results, eof, "expected ','");
  getToken<TEXTORID>(results, eof, "expected pass_code");
  reduction->pass_code = nullptr;
  getToken<CLOSE_BRACE>(results, eof, "expected '}'");

  return reduction;
}



void D_ParserTablesReader::parseShiftPointersToPointersTable(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  std::vector<std::string> values;
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  do {
    getNextToken(results, eof);
    switch (results.id)
    {
    case COMMA:
    case ADDRESS:
      continue;
    case DIGIT:
    case TEXTORID:
      values.push_back(results.str());
      break;
    case CLOSE_BRACE:
      break;

    default:
      std::string err = "Expected id or 0 but got " + results.str();
      throw SyntaxError(err.c_str());
    }
  } while (results.id != CLOSE_BRACE);

  D_Shift*** shift = new D_Shift**[values.size()];
  for (size_t i = 0; i < values.size(); i++)
  {
    if (values[i] == "0" || values[i] == "NULL") shift[i] = static_cast<D_Shift**>(0);
    else {
      // get value from shiftMap
      shift[i] = parserTablesHolder->shiftMapPointer.find(values[i])->second.first;
    }

  }
  // create address to name mapping
  
  parserTablesHolder->addressSymbolTable[shift] = name;
  parserTablesHolder->shiftMapPointerPointer[name] = std::make_pair(shift, values.size());
}

D_ShiftPtr D_ParserTablesReader::parseShift(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  /*
  unsigned short	symbol;
  unsigned char		shift_kind;
  unsigned char		op_assoc;
  int			op_priority;
  int			term_priority;
  int			action_index;
  D_ReductionCode	speculative_code;
  */

  D_ShiftPtr shift(new D_Shift);
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  shift->symbol = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected symbol"));
  getToken<COMMA>(results, eof, "expected ','");
  shift->shift_kind = lexical_cast<unsigned char>(getToken<DIGIT>(results, eof, "expected shift_kind"));
  getToken<COMMA>(results, eof, "expected ','");
  shift->op_assoc = lexical_cast<unsigned char>(getToken<DIGIT>(results, eof, "expected op_assoc"));
  getToken<COMMA>(results, eof, "expected ','");
  shift->op_priority = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected op_priority"));
  getToken<COMMA>(results, eof, "expected ','");
  shift->term_priority = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected term_priority"));
  getToken<COMMA>(results, eof, "expected ','");
  shift->action_index = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected action_index"));
  getToken<COMMA>(results, eof, "expected ','");
  getToken<TEXTORID>(results, eof, "expected speculative_code");
  shift->speculative_code = nullptr;
  getToken<CLOSE_BRACE>(results, eof, "expected '}'");

  return shift;
}

void D_ParserTablesReader::parseVoidPtr(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  getToken<VOID_KEYWORD>(results, eof, "expected void");
  getToken<POINTER>(results, eof, "expected '*'");
}

D_ParserTablesReader::SCAN_KIND D_ParserTablesReader::scanKindFromString(const std::string& kind)
{
  if (kind == "D_SCAN_ALL")       return SCAN_ALL;
  if (kind == "D_SCAN_LONGEST")   return SCAN_LONGEST;
  if (kind == "D_SCAN_MIXED")     return SCAN_MIXED;
  if (kind == "D_SCAN_TRAILING")  return SCAN_TRAILING;
  if (kind == "D_SCAN_RESERVED")  return SCAN_RESERVED;
  if (kind == "D_SCAN_DEFAULT")   return SCAN_DEFAULT;
  return SCAN_DEFAULT;
}

std::string D_ParserTablesReader::scanKindToString(SCAN_KIND kind) const
{
  std::string value;
  switch (kind)
  {
  case D_ParserTablesReader::SCAN_ALL:
    value = "D_SCAN_ALL";
    break;
  case D_ParserTablesReader::SCAN_LONGEST:
    value = "D_SCAN_LONGEST";
    break;
  case D_ParserTablesReader::SCAN_MIXED:
    value = "D_SCAN_MIXED";
    break;
  case D_ParserTablesReader::SCAN_TRAILING:
    value = "D_SCAN_TRAILING";
    break;
  case D_ParserTablesReader::SCAN_RESERVED:
    value = "D_SCAN_RESERVED";
    break;
  default:
    value = "D_SCAN_DEFAULT";
    break;
  }
  return value;
}

D_State D_ParserTablesReader::parseStateElement(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  D_State state;

  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  do {
    getAndBind(parserTablesHolder->unsignedCharMap, getToken<TEXTORID>(results, eof, "expected name"), state.goto_valid);
    
    getToken<COMMA>(results, eof, "expected ','");

    // goto_table_offset
    state.goto_table_offset = lexical_cast<int>(getToken<DIGIT>(results, eof, "goto_table_offset"));
    getToken<COMMA>(results, eof, "expected ','");

    // reductions
    getToken<OPEN_BRACE>(results, eof, "expected '{'");    
    state.reductions.n = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "n"));
    getToken<COMMA>(results, eof, "expected ','");
    getAndBind(parserTablesHolder->reductionMapPointer, getToken<TEXTORID>(results, eof, "expected reduction name"), state.reductions.v);
    getToken<CLOSE_BRACE>(results, eof, "expected '}'");

    getToken<COMMA>(results, eof, "expected ','");

    // right_epsilon_hints
    getToken<OPEN_BRACE>(results, eof, "expected '{'");    
    state.right_epsilon_hints.n = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "n"));
    getToken<COMMA>(results, eof, "expected ','");
    getAndBind(parserTablesHolder->rightEpsilonHintMap, getToken<TEXTORID>(results, eof, "expected rightEpsilonHint"), state.right_epsilon_hints.v);
    getToken<CLOSE_BRACE>(results, eof, "expected '}'");

    getToken<COMMA>(results, eof, "expected ','");

    // errorDiscoveryHint
    getToken<OPEN_BRACE>(results, eof, "expected '{'");    
    state.error_recovery_hints.n = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "n"));
    getToken<COMMA>(results, eof, "expected ','");
    getAndBind(parserTablesHolder->errorRecoveryHintMap, getToken<TEXTORID>(results, eof, "expected errorRecoveryHint"), state.error_recovery_hints.v);
    getToken<CLOSE_BRACE>(results, eof, "expected '}'");

    getToken<COMMA>(results, eof, "expected ','");
    
    int shifts = lexical_cast<int>(getToken<DIGIT>(results, eof, "shifts"));
    state.shifts = shifts;
    getToken<COMMA>(results, eof, "expected ','");

    std::string scanner_code_ptr = getToken<TEXTORID>(results, eof, "expected scanner code");
    // TODO scanner code ptr initialized as NULL for now
    state.scanner_code = NULL;
    getToken<COMMA>(results, eof, "expected ','");

    std::string scanner_table;
    state.scanner_table = NULL;

    getNextToken(results, eof);
    if (results.str() == "NULL") ;  // set null
    else if (results.id == OPEN_PARENTHESIS) {
      parseVoidPtr(results, eof);
      getToken<CLOSE_PARENTHESIS>(results, eof, "expected ')'");
      getAndBind(parserTablesHolder->sbUint8PointersMap, getToken<TEXTORID>(results, eof, "expected name"), state.scanner_table);
    }
    //state.scanner_table = NULL;

    getToken<COMMA>(results, eof, "expected ','");
    
    // scan size
    getToken<SIZEOF_KEYWORD>(results, eof, "expected 'sizeof'");
    getToken<OPEN_PARENTHESIS>(results, eof, "expected '('");
    std::string unsigned_type = getToken<UNSIGNED_TYPE>(results, eof, "expected 'unsigned'");
    std::string char_type = getToken<CHAR_TYPE>(results, eof, "expected 'char'");
    getToken<CLOSE_PARENTHESIS>(results, eof, "expected ')'");
    getToken<COMMA>(results, eof, "expected ','");

    if (char_type == "char") { state.scanner_size = sizeof(unsigned char); }
    
    state.accept = lexical_cast<char>(getToken<DIGIT>(results, eof, "accept"));

    getToken<COMMA>(results, eof, "expected ','");

    std::string scan_kind = getToken<TEXTORID>(results, eof, "expected scan kind");
    state.scan_kind = scanKindFromString(scan_kind);

    getToken<COMMA>(results, eof, "expected ','");

    std::string transition_table;
    state.transition_table = NULL;

    // transition table
    getNextToken(results, eof);
    if (results.str() == "NULL");  // set null
    else if (results.id == OPEN_PARENTHESIS) {
      parseVoidPtr(results, eof);
      getToken<CLOSE_PARENTHESIS>(results, eof, "expected ')'");
      getAndBind(parserTablesHolder->sbTransUint8Pointers, getToken<TEXTORID>(results, eof, "expected transition name"), state.transition_table);

    }
    
    getToken<COMMA>(results, eof, "expected ','");

    getAndBind(parserTablesHolder->shiftMapPointerPointer, getToken<TEXTORID>(results, eof, "expected accepts_diffs"), state.accepts_diff);

    getToken<COMMA>(results, eof, "expected ','");
    
    state.reduces_to = lexical_cast<int>(getToken<DIGIT>(results, eof, "reduces_to"));

    getToken<CLOSE_BRACE>(results, eof, "expected '}'");
    
  } while (results.id != CLOSE_BRACE);
  return state;
}

void D_ParserTablesReader::parseState(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  std::vector<D_State> states;

  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  do {    
    auto state = parseStateElement(results, eof);
    states.push_back(state);
    getNextToken(results, eof);
    if (results.id == COMMA) continue;
  } while (results.id != CLOSE_BRACE);
  D_State* state_table = new D_State[states.size()];
  for (size_t index = 0; index < states.size(); index++) {
    state_table[index].accept = states[index].accept;
    state_table[index].accepts_diff = states[index].accepts_diff;
    state_table[index].error_recovery_hints.n = states[index].error_recovery_hints.n;
    state_table[index].error_recovery_hints.v = states[index].error_recovery_hints.v;
    state_table[index].goto_table_offset = states[index].goto_table_offset;
    state_table[index].goto_valid = states[index].goto_valid;
    state_table[index].reduces_to = states[index].reduces_to;
    state_table[index].reductions.n = states[index].reductions.n;
    state_table[index].reductions.v = states[index].reductions.v;
    state_table[index].right_epsilon_hints.n = states[index].right_epsilon_hints.n;
    state_table[index].right_epsilon_hints.v = states[index].right_epsilon_hints.v;
    state_table[index].scanner_code = states[index].scanner_code;
    state_table[index].scanner_size = states[index].scanner_size;
    state_table[index].scanner_table = states[index].scanner_table;
    state_table[index].scan_kind = states[index].scan_kind;
    state_table[index].shifts = states[index].shifts;
    state_table[index].transition_table = states[index].transition_table;
  }
  parserTablesHolder->statePointersMap[name] = std::make_pair(state_table, states.size());
  parserTablesHolder->addressSymbolTable[state_table] = name;
}

std::string D_ParserTablesReader::symbolConstToString(unsigned int symbol) const
{
  std::string value;
  switch (symbol)
  {
  case D_SYMBOL_NTERM:
    value = "D_SYMBOL_NTERM";
    break;
  case D_SYMBOL_INTERNAL:
    value = "D_SYMBOL_INTERNAL";
    break;
  case D_SYMBOL_EBNF:
    value = "D_SYMBOL_EBNF";
    break;
  case D_SYMBOL_STRING:
    value = "D_SYMBOL_STRING";
    break;
  case D_SYMBOL_REGEX:
    value = "D_SYMBOL_REGEX";
    break;
  case D_SYMBOL_CODE:
    value = "D_SYMBOL_CODE";
    break;
  case D_SYMBOL_TOKEN:
    value = "D_SYMBOL_TOKEN";
    break;
  }
  return value;
}


unsigned int D_ParserTablesReader::symbolFromString(const std::string& symbol)
{
  if (symbol == "D_SYMBOL_NTERM")       return D_SYMBOL_NTERM;
  if (symbol == "D_SYMBOL_INTERNAL")    return D_SYMBOL_INTERNAL;
  if (symbol == "D_SYMBOL_EBNF")        return D_SYMBOL_EBNF;
  if (symbol == "D_SYMBOL_STRING")      return D_SYMBOL_STRING;
  if (symbol == "D_SYMBOL_REGEX")       return D_SYMBOL_REGEX;
  if (symbol == "D_SYMBOL_CODE")        return D_SYMBOL_CODE;
  if (symbol == "D_SYMBOL_TOKEN")       return D_SYMBOL_TOKEN;
  return 0xFF;
}

D_Symbol D_ParserTablesReader::parseSymbolElement(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  D_Symbol sym;
  
  do {
    std::string symbol_const = getToken<TEXTORID>(results, eof, "expected D_SYMBOL Const");
    symbolFromString(symbol_const);
    getToken<COMMA>(results, eof, "expected ','");
    getToken<SYMBOL_DEF>(results, eof, "expected symbol definition");
    std::string symbol_def = std::string(results.start + 1, results.end - 1);
    getToken<COMMA>(results, eof, "expected ','");
    int len = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected digit"));
    getNextToken(results, eof);
    int start_symbol = 0;
    if (results.id == COMMA) {
      start_symbol = lexical_cast<int>(getToken<DIGIT>(results, eof, "expected digit"));
    } 
    while (results.id != CLOSE_BRACE) { getNextToken(results, eof); }
    const char *sptr = new char[len + 1];
    
    strcpy(const_cast<char*>(sptr), symbol_def.c_str());
    sym.kind = symbolFromString(symbol_const);
    sym.name = sptr;
    sym.name_len = len;
    sym.start_symbol = start_symbol;

} while (results.id != CLOSE_BRACE);
  getToken<COMMA>(results, eof, "expected ','");
return sym;
}
D_ParserTables* D_ParserTablesReader::parseTable(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  D_ParserTables* pTables = new D_ParserTables;
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  unsigned int nstates = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "expected nstates"));
  pTables->nstates = nstates;
  getToken<COMMA>(results, eof, "expected ','");

  getAndBind(parserTablesHolder->statePointersMap, getToken<TEXTORID>(results, eof, "expected state"), pTables->state);

  getToken<COMMA>(results, eof, "expected ','");
  getAndBind(parserTablesHolder->unsignedShortMap, getToken<TEXTORID>(results, eof, "expected goto_table"), pTables->goto_table);

  getToken<COMMA>(results, eof, "expected ','");
  unsigned int whitespace_state = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "expected whitespace_state"));
  pTables->whitespace_state = whitespace_state;
  getToken<COMMA>(results, eof, "expected ','");
  unsigned int nsymbols = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "expected nsymbols"));
  pTables->nsymbols = nsymbols;
  getToken<COMMA>(results, eof, "expected ','");
  getAndBind(parserTablesHolder->symbolMap, getToken<TEXTORID>(results, eof, "expected symbol"), pTables->symbols);

  getToken<COMMA>(results, eof, "expected ','");
  getToken<TEXTORID>(results, eof, "expected default_white_space");
  pTables->default_white_space = NULL;
  getToken<COMMA>(results, eof, "expected ','");
  unsigned int npasses = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "expected npasses"));
  pTables->npasses = npasses;
  getToken<COMMA>(results, eof, "expected ','");
  getToken<TEXTORID>(results, eof, "expected passes");
  pTables->passes = NULL;
  getToken<COMMA>(results, eof, "expected ','");
  unsigned int save_parse_tree = lexical_cast<unsigned int>(getToken<DIGIT>(results, eof, "expected save_parse_tree"));
  pTables->save_parse_tree = save_parse_tree;
  getToken<CLOSE_BRACE>(results, eof, "expected '}'");
  this->parserTablesHolder->parserTablesName = name;
  return pTables;
}

void D_ParserTablesReader::parseSymbol(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  getToken<OPEN_BRACE>(results, eof, "expected '{'");

  std::vector<D_Symbol> symbols;

  do {
    D_Symbol sym;
    getNextToken(results, eof);
    if (results.id == CLOSE_BRACE) break;
    if (results.id == OPEN_BRACE) {
      sym = parseSymbolElement(results, eof);
      symbols.push_back(sym);
    }
    
  } while (results.id != CLOSE_BRACE);

  D_Symbol* symbol_table = new D_Symbol[symbols.size()];
  for (size_t index = 0; index < symbols.size(); index++) {
    symbol_table[index].kind = symbols[index].kind;
    symbol_table[index].name = symbols[index].name;
    symbol_table[index].name_len = symbols[index].name_len;
    symbol_table[index].start_symbol = symbols[index].start_symbol;
  }
  parserTablesHolder->symbolMap[name] = std::make_pair(symbol_table, symbols.size());
  parserTablesHolder->addressSymbolTable[symbol_table] = name;
}

D_RightEpsilonHint D_ParserTablesReader::parseRightEpsilonHintElement(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  D_RightEpsilonHint hint;
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  hint.depth = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected depth"));
  getToken<COMMA>(results, eof, "expected ','");
  hint.preceeding_state = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected proceeding state"));
  getToken<COMMA>(results, eof, "expected ','");
  getToken<ADDRESS>(results, eof, "expected '&'");
  const auto& map = parserTablesHolder->reductionMap;
  std::string name = getToken<TEXTORID>(results, eof, "expected name");
  auto i = map.find(name);
  if (i != map.end()) { hint.reduction = i->second; }
  else hint.reduction = 0;
  getToken<CLOSE_BRACE>(results, eof, "expected '}'");
  return hint;
}


D_ErrorRecoveryHint D_ParserTablesReader::parseErrorRecoveryHintElement(lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  D_ErrorRecoveryHint hint;
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  hint.depth = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected depth"));
  getToken<COMMA>(results, eof, "expected ','");
  hint.symbol = lexical_cast<unsigned short>(getToken<DIGIT>(results, eof, "expected symbol"));
  getToken<COMMA>(results, eof, "expected ','");
  std::string str = getToken<SYMBOL_DEF>(results, eof, "expected symbol_def string");
  const char *sptr = new char[str.length() + 1];
  strcpy(const_cast<char*>(sptr), str.c_str());
  hint.string = sptr;
  getToken<CLOSE_BRACE>(results, eof, "expected '}'");
  return hint;
}

void D_ParserTablesReader::parseRightEpsilonHint(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  std::vector<D_RightEpsilonHint> values;
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  do {
    auto rightEpsilon = parseRightEpsilonHintElement(results, eof);
    values.push_back(rightEpsilon);
    getNextToken(results, eof);
    if (results.id == COMMA) continue;
  } while (results.id != CLOSE_BRACE);
  D_RightEpsilonHint* epsilon_table = new D_RightEpsilonHint[values.size()];
  for (size_t i = 0; i < values.size(); i++) {
    epsilon_table[i].depth = values[i].depth;
    epsilon_table[i].preceeding_state = values[i].preceeding_state;
    epsilon_table[i].reduction = values[i].reduction;
  }
  parserTablesHolder->rightEpsilonHintMap[name] = std::make_pair(epsilon_table, values.size());
  parserTablesHolder->addressSymbolTable[epsilon_table] = name;
}

void D_ParserTablesReader::parseErrorRecoveryHint(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
{
  std::vector<D_ErrorRecoveryHint> values;
  getToken<OPEN_BRACE>(results, eof, "expected '{'");
  do {
    auto errRec = parseErrorRecoveryHintElement(results, eof);
    values.push_back(errRec);
    getNextToken(results, eof);
    if (results.id == COMMA) continue;
  } while (results.id != CLOSE_BRACE);
  D_ErrorRecoveryHint* err_rec = new D_ErrorRecoveryHint[values.size()];
  for (size_t i = 0; i < values.size(); i++) {
    err_rec[i].depth = values[i].depth;
    err_rec[i].string = values[i].string;
    err_rec[i].symbol = values[i].symbol;
  }
  parserTablesHolder->errorRecoveryHintMap[name] = std::make_pair(err_rec, values.size());
  parserTablesHolder->addressSymbolTable[err_rec] = name;
}

D_ParserTables* D_ParserTablesReader::parse(const char* begin, const char* end, const std::unordered_map<std::string, reductionf>& builtinRMappings)
{
  bool eof = false;
  lexertl::recursive_match_results<const char *> results (begin, end);
  do
  {
    getNextToken(results, eof);
    if (results.id == REDUCTION_CODE) {
      std::string funName = results.str();
      while (results.id != OPEN_BRACE) getNextToken(results, eof);
      getNextToken(results, eof);
      std::string reduction_key = results.str();
      auto it = builtinRMappings.find(reduction_key);
      if (it != builtinRMappings.end()) parserTablesHolder->builtinReductionMap.insert(std::make_pair(funName, it->second));
    }
    if (results.id == D_REDUCTION_ID) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {        
        std::string name = results.str();
        getToken<ASSIGNMENT>(results, eof, "expected '='");
        D_ReductionPtr r = parseReduction(results, eof);
        parserTablesHolder->reductionMap[name] = r;
        // create address to name mapping
        parserTablesHolder->addressSymbolTable[r] = name;

      }
      if (results.id == POINTER) {
        getToken<TEXTORID>(results, eof, "expected name");
        std::string name = results.str();

        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");
        getToken<ASSIGNMENT>(results, eof, "expected '='");
        parsePointersTable<D_Reduction>(name, results, eof, parserTablesHolder->reductionMapPointer, parserTablesHolder->reductionMap);
      }
      getToken<SEMICOLON>(results, eof, "expected ';'");
    }
    if (results.id == D_SHIFT_ID) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {
        std::string name = results.str();
        getToken<ASSIGNMENT>(results, eof, "expected '='");
        D_ShiftPtr s = parseShift(results, eof);
        parserTablesHolder->shiftMap[name] = s;
        // create address to name mapping
        parserTablesHolder->addressSymbolTable[s] = name;

      } 
      if (results.id == POINTER) {
        getToken<TEXTORID>(results, eof, "expected name");
        std::string name = results.str();

        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");
        getToken<ASSIGNMENT>(results, eof, "expected '='");

        parsePointersTable<D_Shift>(name, results, eof, parserTablesHolder->shiftMapPointer, parserTablesHolder->shiftMap);


      }
      if (results.id == POINTERTOPOINTER) {
        getToken<TEXTORID>(results, eof, "expected name");
        std::string name = results.str();
        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");
        getToken<ASSIGNMENT>(results, eof, "expected '='");
        parseShiftPointersToPointersTable(name, results, eof);

      }
      getToken<SEMICOLON>(results, eof, "expected ';'");
    }
    if (results.id == UNSIGNED_TYPE) {
      getNextToken(results, eof);
      bool charTable = true;
      if (results.id != CHAR_TYPE) charTable = false;
      getToken<TEXTORID>(results, eof, "expected name");
      std::string name = results.str();
      getToken<OPEN_BRACKET>(results, eof, "expected '['");
      getNextToken(results, eof);
      if (results.id == TEXTORID || results.id == DIGIT) { getToken<CLOSE_BRACKET>(results, eof, "expected ']'"); }
      else if (results.id != CLOSE_BRACKET) { SyntaxError("expected ']'"); }
      getToken<ASSIGNMENT>(results, eof, "expected '='");
          
      if(charTable) parseUnsignedTable<unsigned char>(name, results, eof, parserTablesHolder->unsignedCharMap);
      else parseUnsignedTable<unsigned short>(name, results, eof, parserTablesHolder->unsignedShortMap);
      getToken<SEMICOLON>(results, eof, "expected ';'");
    }
    if (results.id == SBUINT8 || results.id == SBUINT16 || results.id == SBUINT32 
        || results.id == SB_TRANS_UINT8 || results.id == SB_TRANS_UINT16 || results.id == SB_TRANS_UINT32)
    {
      LangElem le = static_cast<LangElem>(results.id);
      std::string type = results.str();
      getToken<TEXTORID>(results, eof, "expected name");
      std::string name = results.str();
      getToken<OPEN_BRACKET>(results, eof, "expected '['");
      getNextToken(results, eof);
      if (results.id == TEXTORID || results.id == DIGIT) { getToken<CLOSE_BRACKET>(results, eof, "expected ']'"); }
      else if (results.id != CLOSE_BRACKET) { SyntaxError("expected ']'"); }
      getToken<ASSIGNMENT>(results, eof, "expected '='");
      if (le == SBUINT8) parseSBUint<SB_uint8>(type, name, results, eof);
      if (le == SBUINT16) parseSBUint<SB_uint16>(type, name, results, eof);
      if (le == SBUINT32) parseSBUint<SB_uint32>(type, name, results, eof);
      if (le == SB_TRANS_UINT8) parseSBUint<SB_trans_uint8>(type, name, results, eof);
      if (le == SB_TRANS_UINT16) parseSBUint<SB_trans_uint16>(type, name, results, eof);
      if (le == SB_TRANS_UINT32) parseSBUint<SB_trans_uint32>(type, name, results, eof);

      getToken<SEMICOLON>(results, eof, "expected ';'");
    }
    if (results.id == D_STATE_ID) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {
        std::string name = results.str();
        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");

        getToken<ASSIGNMENT>(results, eof, "expected '='");
        parseState(name, results, eof);
        getToken<SEMICOLON>(results, eof, "expected ';'");
      }
    }
    if (results.id == D_SYMBOL) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {
        std::string name = results.str();
        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");
        getToken<ASSIGNMENT>(results, eof, "expected '='");
        parseSymbol(name, results, eof);
        getToken<SEMICOLON>(results, eof, "expected ';'");
      }
    }
    if (results.id == D_PARSE_TABLES) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {
        std::string name = results.str();
        getToken<ASSIGNMENT>(results, eof, "expected '='");
        this->parserTablesHolder->parserTables = parseTable(name, results, eof);
        getToken<SEMICOLON>(results, eof, "expected ';'");
      }
    }
    if (results.id == D_RIGHT_EPSILON_HINT) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {
        std::string name = results.str();
        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");

        getToken<ASSIGNMENT>(results, eof, "expected '='");
        parseRightEpsilonHint(name, results, eof);
        getToken<SEMICOLON>(results, eof, "expected ';'");
      }
    }
    if (results.id == D_ERROR_RECOVERY_HINT) {
      getNextToken(results, eof);
      if (results.id == TEXTORID) {
        std::string name = results.str();
        getToken<OPEN_BRACKET>(results, eof, "expected '['");
        getToken<CLOSE_BRACKET>(results, eof, "expected ']'");

        getToken<ASSIGNMENT>(results, eof, "expected '='");
        parseErrorRecoveryHint(name, results, eof);
        getToken<SEMICOLON>(results, eof, "expected ';'");
      }
    }
  } while (!eof);
  return this->parserTablesHolder->parserTables;
}



std::string D_ParserTablesReader::reductionsToString() const
{
  /*
  unsigned short	nelements;
  unsigned short	symbol;
  D_ReductionCode	speculative_code;
  D_ReductionCode	final_code;
  unsigned short	op_assoc;
  unsigned short	rule_assoc;
  int 			op_priority;
  int 			rule_priority;
  int			action_index;
  int			npass_code;
  D_ReductionCode	*pass_code;
  */
  std::string value;
  for (const auto & reduction : parserTablesHolder->reductionMap) {
    value += "D_Reduction ";
    value += reduction.first;
    value += " = ";   
    value += " { ";
    value += to_string(reduction.second->nelements);
    value += ",";
    value += to_string(reduction.second->symbol);
    value += ",";
    value += parserTablesHolder->addressSymbolTable[reduction.second->speculative_code];
    value += ",";
    value += parserTablesHolder->addressSymbolTable[reduction.second->final_code];
    value += ",";
    value += to_string(reduction.second->op_assoc);
    value += ",";
    value += to_string(reduction.second->rule_assoc);
    value += ",";
    value += to_string(reduction.second->op_priority);
    value += ",";
    value += to_string(reduction.second->rule_priority);
    value += ",";
    value += to_string(reduction.second->action_index);
    value += ",";
    value += to_string(reduction.second->npass_code);
    value += ",";
    value += parserTablesHolder->addressSymbolTable[reduction.second->pass_code];
    value += " } ";
    value += "\n";
  }
  
  return value;
}

std::string D_ParserTablesReader::shiftsToString() const
{
  /*
  unsigned short	symbol;
  unsigned char		shift_kind;
  unsigned char		op_assoc;
  int			op_priority;
  int			term_priority;
  int			action_index;
  D_ReductionCode	speculative_code;
  */
  std::string value;
  for (const auto & shift : parserTablesHolder->shiftMap) {
    value += "D_Shift ";
    value += shift.first;
    value += " = ";
    value += " { ";
    value += to_string(shift.second->symbol);
    value += ",";
    value += to_string(shift.second->shift_kind);
    value += ",";
    value += to_string(shift.second->op_assoc);
    value += ",";
    value += to_string(shift.second->op_priority);
    value += ",";
    value += to_string(shift.second->term_priority);
    value += ",";
    value += to_string(shift.second->action_index);
    value += ",";
    value += parserTablesHolder->addressSymbolTable[shift.second->speculative_code];
    value += " } ";
    value += "\n";
  }
  return value;
}

std::string D_ParserTablesReader::shiftPointerTablesToString() const
{
  std::string value;

  for (const auto & shiftPTable : parserTablesHolder->shiftMapPointer) {

    size_t size = shiftPTable.second.second;

    value += "D_Shift * ";
    value += shiftPTable.first;
    value += "[]";
    value += " = ";
    value += " { ";
    for (size_t i = 0; i < size; i++) {
      std::string address = parserTablesHolder->addressSymbolTable[shiftPTable.second.first[i]];
      if (address != "NULL") value += "&";
      if (i > 0) value += ",";
      value += address;
    }
    value += " } ";
    value += "\n";

  }
  return value;
}

std::string D_ParserTablesReader::reductionPointerTablesToString() const
{
  std::string value;

  for (const auto & shiftPTable : parserTablesHolder->reductionMapPointer) {

    size_t size = shiftPTable.second.second;

    value += "D_Reduction * ";
    value += shiftPTable.first;
    value += "[]";
    value += " = ";
    value += " { ";
    for (size_t i = 0; i < size; i++) {
      std::string address = parserTablesHolder->addressSymbolTable[shiftPTable.second.first[i]];
      if (address != "NULL") value += "&";
      if (i > 0) value += ",";
      value += address;
    }
    value += " } ";
    value += "\n";

  }
  return value;
}

std::string D_ParserTablesReader::shiftPointerPointerTablesToString() const
{
  std::string value;
  for (const auto & shiftPPTable : parserTablesHolder->shiftMapPointerPointer) {
    std::string address = parserTablesHolder->addressSymbolTable[shiftPPTable.second.first];
    size_t size = shiftPPTable.second.second;

    value += "D_Shift ** ";
    value += shiftPPTable.first;
    value += "[]";
    value += " = ";
    value += " { ";
    for (size_t i = 0; i < size; i++) {
      std::string address = parserTablesHolder->addressSymbolTable[shiftPPTable.second.first[i]];
      if (i > 0) value += ",";
      value += address;
    }
    value += " } ";
    value += "\n";
  }
  return value;
}

std::string D_ParserTablesReader::stateToString() const
{
  std::string value;
  for (const auto & state : parserTablesHolder->statePointersMap) {
    size_t size = state.second.second;
    D_State* s = state.second.first;
    value += "D_State ";
    value += state.first;
    value += "[]";
    value += " = ";
    value += " { ";
    for (size_t index = 0; index < size; index++) {
      if (index > 0) value += ",";
      value += "\n";
      value += " { ";
      value += parserTablesHolder->addressSymbolTable[s[index].goto_valid];
      value += " , ";
      value += to_string(s[index].goto_table_offset);
      value += " , ";
      value += " { ";
      value += to_string(s[index].reductions.n);
      value += " , ";
      value += parserTablesHolder->addressSymbolTable[s[index].reductions.v];
      value += " } ";
      value += " , ";
      value += " { ";
      value += to_string(s[index].right_epsilon_hints.n);
      value += " , ";
      value += parserTablesHolder->addressSymbolTable[s[index].right_epsilon_hints.v];
      value += " } ";
      value += " , ";
      value += " { ";
      value += to_string(s[index].error_recovery_hints.n);
      value += " , ";
      value += (parserTablesHolder->addressSymbolTable[s[index].error_recovery_hints.v]);
      value += " } ";
      value += ",";
      value += to_string(s[index].shifts);
      value += " , ";
      value += (parserTablesHolder->addressSymbolTable[s[index].scanner_code]);
      value += " , ";
      if (parserTablesHolder->addressSymbolTable[s[index].scanner_table] != "NULL") value += "(void*)";
      value += (parserTablesHolder->addressSymbolTable[s[index].scanner_table]);
      value += " , ";
      if (s[index].scanner_size == sizeof(unsigned char)) value += "sizeof(unsigned char)";
      else value += to_string(s[index].scanner_size);
      value += " , ";
      value += to_string(s[index].accept);
      value += " , ";
      value += scanKindToString(static_cast<SCAN_KIND>(s[index].scan_kind));
      value += " , ";
      if (parserTablesHolder->addressSymbolTable[s[index].transition_table] != "NULL") value += "(void*)";
      value += (parserTablesHolder->addressSymbolTable[s[index].transition_table]);
      value += " , ";
      value += (parserTablesHolder->addressSymbolTable[s[index].accepts_diff]);
      value += " , ";
      value += to_string(s[index].reduces_to);
      value += " } ";
    }
    value += " } ";
  }
  value += '\n';
  return value;
}

std::string D_ParserTablesReader::symbolToString() const
{
  std::string value;
  for (const auto & symbol : parserTablesHolder->symbolMap) {
    size_t size = symbol.second.second;
    D_Symbol* s = symbol.second.first;
    value += "D_Symbol ";
    value += symbol.first;
    value += "[]";
    value += " = ";
    value += " { ";
    for (size_t index = 0; index < size; index++) {
      if (index > 0) value += ",";
      value += "\n";
      value += " { ";
      value += (symbolConstToString(s[index].kind));
      value += " , ";
      value += s[index].name;
      value += " , ";
      value += to_string(s[index].name_len);
      value += " , ";
      value += to_string(s[index].start_symbol);
      value += " } ";
    }
      value += " } ";
  }
  value += '\n';
  return value;
}

std::string D_ParserTablesReader::parserTablesToString(const D_ParserTables* parserTables) const
{
  if (!parserTables) return "";
  std::string value;
  value += "D_ParserTables ";
  value += parserTablesHolder->parserTablesName;
  value += " { ";
  value += to_string(parserTables->nstates);
  value += " , ";
  value += (parserTablesHolder->addressSymbolTable[parserTables->state]);
  value += " , ";
  value += (parserTablesHolder->addressSymbolTable[parserTables->goto_table]);
  value += " , ";
  value += to_string(parserTables->whitespace_state);
  value += " , ";
  value += to_string(parserTables->nsymbols);
  value += " , ";
  value += (parserTablesHolder->addressSymbolTable[parserTables->symbols]);
  value += " , ";
  value += (parserTablesHolder->addressSymbolTable[parserTables->default_white_space]);
  value += " , ";
  value += to_string(parserTables->npasses);
  value += " , ";
  value += (parserTablesHolder->addressSymbolTable[parserTables->passes]);
  value += " , ";
  value += to_string(parserTables->save_parse_tree);
  value += " } ";
  return value;
}

std::string D_ParserTablesReader::rightEpsilonHintToString() const
{
  std::string value;
  for (const auto & repsilon : parserTablesHolder->rightEpsilonHintMap) {
    size_t size = repsilon.second.second;
    D_RightEpsilonHint* re = repsilon.second.first;
    value += "D_RightEpsilonHint ";
    value += repsilon.first;
    value += "[]";
    value += " = ";
    value += "{";
    for (size_t index = 0; index < size; index++) {
      if (index > 0) value += ",";
      value += "{";
      value += to_string(re[index].depth);
      value += ",";
      value += to_string(re[index].preceeding_state);
      value += ",";
      value += "&";
      value += parserTablesHolder->addressSymbolTable[re[index].reduction];
      value += "}";
    }
    value += "}";
    value += "\n";
  }
  return value;
}

std::string D_ParserTablesReader::errorRecoveryHintToString() const
{
  std::string value;
  for (const auto & repsilon : parserTablesHolder->errorRecoveryHintMap) {
    size_t size = repsilon.second.second;
    D_ErrorRecoveryHint* re = repsilon.second.first;
    value += "D_ErrorRecoveryHint ";
    value += repsilon.first;
    value += "[]";
    value += " = ";
    value += "{";
    for (size_t index = 0; index < size; index++) {
      if (index > 0) value += ",";
      value += "{";
      value += to_string(re[index].depth);
      value += ",";
      value += to_string(re[index].symbol);
      value += ",";
      value += re[index].string;
      value += "}";
    }
    value += "}";
    value += "\n";
  }
  return value;
}

std::string D_ParserTablesReader::toString(const D_ParserTables* parserTables)
{
  std::string value = reductionsToString();
  
  value
    .append(shiftsToString())
    .append(shiftPointerTablesToString())
    .append(shiftPointerPointerTablesToString())
    .append(reductionPointerTablesToString())
    .append(unsignedTableToString(parserTablesHolder->unsignedCharMap))
    .append(unsignedTableToString(parserTablesHolder->unsignedShortMap))
    .append(sbUintTableToString<SB_uint8>())
    .append(sbUintTableToString<SB_uint16>())
    .append(sbUintTableToString<SB_uint32>())
    .append(sbUintTableToString<SB_trans_uint8>())
    .append(rightEpsilonHintToString())
    .append(errorRecoveryHintToString())
    .append(stateToString())
    .append(symbolToString())
    .append(parserTablesToString(parserTables))
    ;
    
  return value;
}



ParserTablesHolderPtr parse(const std::string& file, D_ParserTablesReader& parser, const std::unordered_map<std::string, reductionf>& builtinRMappings)
{
    std::ifstream in(file);
    if (!in.is_open()) throw FileNotFoundException("FileNotFound");
    in.unsetf(std::ios::skipws);
    std::istream_iterator<char> begin(in);
    std::istream_iterator<char> end;

    std::vector<char> buffer;
    std::copy(begin, end, std::back_inserter(buffer));
    
    return parse(buffer, parser, builtinRMappings);
}

ParserTablesHolderPtr parse(const std::vector<char>& buffer, D_ParserTablesReader& parser, const std::unordered_map<std::string, reductionf>& builtinRMappings)
{
  try {
    const char* b = &buffer[0];
    const char* e = &buffer[0] + buffer.size();

    parser.parserTablesHolder->parserTables = (parser.parse(b, e, builtinRMappings));
  }
  catch (const SyntaxError& se) {
    std::cerr << se.what() << std::endl;
  }
  catch (const std::runtime_error& re) {
    std::cerr << re.what() << std::endl;
  }
  return parser.parserTablesHolder;
}


ParserTablesHolder::~ParserTablesHolder()
{}