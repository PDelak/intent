// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef D_PARSER_TABLES_READER_H
#define D_PARSER_TABLES_READER_H

#include <sstream>
#include <unordered_map>
#include <tuple>
#include "../lexertl/rules.hpp"
#include "../lexertl/state_machine.hpp"
#include "../lexertl/generator.hpp"
#include "../lexertl/lookup.hpp"
#include "../dparser/dparse_tables.h"
#include "Exceptions.h"
#include "DynamicReductions.h"

#ifdef _WIN32
#pragma warning( disable : 4996) 
#endif

template <typename T>
T lexical_cast(const std::string& str)
{  
  if (str.find("0x") != std::string::npos) return std::stoi(str, 0, 16);
  return std::stoi(str);
}

template <typename SOURCE>
std::string to_string(const SOURCE& var)
{
  return std::to_string(var);
}

typedef D_Reduction*                                                              D_ReductionPtr;
typedef D_Shift*                                                                  D_ShiftPtr;

typedef std::unordered_map<std::string, D_ReductionPtr>                           reduction_map;
typedef std::unordered_map<std::string, D_ShiftPtr>                               shift_map;

typedef std::unordered_map<std::string, std::pair<D_Shift**, size_t>>             shift_pointers_map;
typedef std::unordered_map<std::string, std::pair<D_Reduction**, size_t>>         reduction_pointers_map;
typedef std::unordered_map<std::string, std::pair<unsigned char*, size_t>>        unsigned_char_pointers_map;
typedef std::unordered_map<std::string, std::pair<unsigned short*, size_t>>       unsigned_short_pointers_map;
typedef std::unordered_map<std::string, std::pair<unsigned int*, size_t>>         unsigned_int_pointers_map;
typedef std::unordered_map<std::string, std::pair<D_Shift***, size_t>>            shift_map_pointer_pointer;
typedef std::unordered_map<std::string, std::pair<SB_uint8*, size_t>>             sb_uint8_pointers_map;
typedef std::unordered_map<std::string, std::pair<SB_uint16*, size_t>>            sb_uint16_pointers_map;
typedef std::unordered_map<std::string, std::pair<SB_uint32*, size_t>>            sb_uint32_pointers_map;
typedef std::unordered_map<std::string, std::pair<SB_trans_uint8*, size_t>>       sb_trans_uint8_pointers_map;
typedef std::unordered_map<std::string, std::pair<SB_trans_uint16*, size_t>>      sb_trans_uint16_pointers_map;
typedef std::unordered_map<std::string, std::pair<SB_trans_uint32*, size_t>>      sb_trans_uint32_pointers_map;
typedef std::unordered_map<std::string, std::pair<D_State*, size_t>>              state_pointers_map;
typedef std::unordered_map<std::string, std::pair<D_RightEpsilonHint*, size_t>>   right_epsilon_hint_map;
typedef std::unordered_map<std::string, std::pair<D_ErrorRecoveryHint*, size_t>>  error_recovery_hint_map;
typedef std::unordered_map<std::string, std::pair<D_Symbol*, size_t>>             symbol_map;
typedef std::unordered_map<std::string, void*>                                    builtin_reduction_map;


typedef std::unordered_map<void*, std::string>                                    address_symbol_table;

class ParserTablesHolder;
typedef std::shared_ptr<ParserTablesHolder>                                       parser_tables_holder_ptr;

template<typename T>
struct PrintUnsignedTableType;

template<>
struct PrintUnsignedTableType<unsigned_char_pointers_map>
{
  static std::string toString() { return "char"; }
};

template<>
struct PrintUnsignedTableType<unsigned_short_pointers_map>
{
  static std::string toString() { return "short"; }
};



template<typename SB_Type, typename Element>
void fill_shift(SB_Type* sb, const std::vector<Element>& values)
{
  for (size_t i = 0; i < values.size(); i++){
    sb[i].shift = values[i].D_ShiftPointerPointer;
  }
}
template<typename SB_Type, typename Element>
static void fill_table(SB_Type* sb, const std::vector<Element>& values)
{
  for (size_t i = 0; i < values.size(); i++){
    size_t size = values[i].scannerBlockElements.size();
    // k is index of std::vector<T>
    // T can be on of
    // 1-> unsigned char*
    // 2-> unsigned short*
    // 3-> unsigned int*
    for (size_t k = 0; k < size; k++) {
      sb[i].scanner_block[k] = values[i].scannerBlockElements[k];
    }
  }
}

class ParserTablesHolder
{
public:
  ParserTablesHolder()
    :parserTables(nullptr)
  {}
  ~ParserTablesHolder();
  D_ParserTables*                    parserTables;
private:
  reduction_map                      reductionMap;
  shift_map                          shiftMap;
  shift_pointers_map                 shiftMapPointer;
  shift_map_pointer_pointer          shiftMapPointerPointer;
  unsigned_char_pointers_map         unsignedCharMap;
  unsigned_short_pointers_map        unsignedShortMap;
  unsigned_int_pointers_map          unsignedIntMap;
  reduction_pointers_map             reductionMapPointer;
  sb_uint8_pointers_map              sbUint8PointersMap;
  sb_uint16_pointers_map             sbUint16PointersMap;
  sb_uint32_pointers_map             sbUint32PointersMap;
  sb_trans_uint8_pointers_map        sbTransUint8Pointers;
  sb_trans_uint16_pointers_map       sbTransUint16Pointers;
  sb_trans_uint32_pointers_map       sbTransUint32Pointers;
  state_pointers_map                 statePointersMap;
  right_epsilon_hint_map             rightEpsilonHintMap;
  error_recovery_hint_map            errorRecoveryHintMap;
  symbol_map                         symbolMap;
  builtin_reduction_map              builtinReductionMap;
  std::string                        parserTablesName;
  mutable address_symbol_table       addressSymbolTable;
  friend class D_ParserTablesReader;
};

typedef std::shared_ptr<ParserTablesHolder>                                       ParserTablesHolderPtr;

class D_ParserTablesReader
{

  template<typename T>
  struct UnsignedTableSelector;

  template<>
  struct UnsignedTableSelector<unsigned char>
  {
    unsigned_char_pointers_map& get(D_ParserTablesReader* p) { return p->parserTablesHolder->unsignedCharMap; }
  };

  template<>
  struct UnsignedTableSelector<unsigned short>
  {
    unsigned_short_pointers_map& get(D_ParserTablesReader* p) { return p->parserTablesHolder->unsignedShortMap; }
  };

  template<>
  struct UnsignedTableSelector<unsigned int>
  {
    unsigned_int_pointers_map& get(D_ParserTablesReader* p) { return p->parserTablesHolder->unsignedIntMap; }
  };

  template<typename T>
  struct SBTypeTrait;

  template<typename UnsignedTable>
  struct SBUintElement
  {
    SBUintElement() :D_ShiftPointerPointer(NULL) {};
    typedef typename SBTypeTrait<UnsignedTable>::scanner_block_type scanner_block_type;
    D_Shift** D_ShiftPointerPointer;
    std::vector<scanner_block_type*> scannerBlockElements;
   
  };


  template<>
  struct SBTypeTrait<SB_uint8>
  {
    typedef SB_uint8                            sb_uint_type;
    typedef sb_uint8_pointers_map               pointers_map;
    typedef unsigned char                       scanner_block_type;
    typedef unsigned_char_pointers_map          unsigned_map_type;

    static std::string type_name() { return "SB_uint8"; }

    pointers_map& get(D_ParserTablesReader* p) const { return p->parserTablesHolder->sbUint8PointersMap; }

    static void fill_shift(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values) 
    {
      ::fill_shift(sb, values);
    }
    static void fill_table(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_table(sb, values);
    }

    static SBUintElement<sb_uint_type> parse_shift(D_ParserTablesReader* p, lexertl::recursive_match_results<const char* >& results, bool& eof)
    {
      p->getNextToken(results, eof);
      auto element = p->fillShift<sb_uint_type>(results.str(), results, eof);
      p->getToken<COMMA>(results, eof, "expected ','");
      return element;
    }

  };

  template<>
  struct SBTypeTrait<SB_uint16>
  {
    typedef SB_uint16                           sb_uint_type;
    typedef sb_uint16_pointers_map              pointers_map;
    typedef unsigned short                      scanner_block_type;
    typedef unsigned_short_pointers_map         unsigned_map_type;

    static std::string type_name() { return "SB_uint16"; }

    pointers_map& get(D_ParserTablesReader* p) const { return p->parserTablesHolder->sbUint16PointersMap; }

    static void fill_shift(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_shift(sb, values);
    }
    static void fill_table(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_table(sb, values);
    }
    static SBUintElement<sb_uint_type> parse_shift(D_ParserTablesReader* p, lexertl::recursive_match_results<const char* >& results, bool& eof)
    {
      p->getNextToken(results, eof);
      auto element = p->fillShift<sb_uint_type>(results.str(), results, eof);
      p->getToken<COMMA>(results, eof, "expected ','");
      return element;
    }

  };

  template<>
  struct SBTypeTrait<SB_uint32>
  {
    typedef SB_uint32                           sb_uint_type;
    typedef sb_uint32_pointers_map              pointers_map;
    typedef unsigned int                        scanner_block_type;
    typedef unsigned_int_pointers_map           unsigned_map_type;

    static std::string type_name() { return "SB_uint32"; }

    pointers_map& get(D_ParserTablesReader* p) const { return p->parserTablesHolder->sbUint32PointersMap; }

    static void fill_shift(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_shift(sb, values);
    }
    static void fill_table(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_table(sb, values);
    }
    static SBUintElement<sb_uint_type> parse_shift(D_ParserTablesReader* p, lexertl::recursive_match_results<const char* >& results, bool& eof)
    {
      p->getNextToken(results, eof);
      auto element = p->fillShift<sb_uint_type>(results.str(), results, eof);
      p->getToken<COMMA>(results, eof, "expected ','");
      return element;
    }
  };

  template<>
  struct SBTypeTrait<SB_trans_uint8>
  {
    typedef SB_trans_uint8                      sb_uint_type;

    typedef sb_trans_uint8_pointers_map         pointers_map;
    typedef unsigned char                       scanner_block_type;
    typedef unsigned_char_pointers_map          unsigned_map_type;

    static std::string type_name() { return "SB_trans_uint8"; }

    pointers_map& get(D_ParserTablesReader* p) const { return p->parserTablesHolder->sbTransUint8Pointers; }

    static void fill_shift(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {   }

    static void fill_table(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_table(sb, values);
    }
    static SBUintElement<sb_uint_type> parse_shift(D_ParserTablesReader* p, lexertl::recursive_match_results<const char* >& results, bool& eof)
    {
      return SBUintElement<sb_uint_type>();      
    }

  };

  template<>
  struct SBTypeTrait<SB_trans_uint16>
  {
    typedef SB_trans_uint16                     sb_uint_type;
    typedef sb_trans_uint16_pointers_map        pointers_map;
    typedef unsigned short                      scanner_block_type;
    typedef unsigned_short_pointers_map         unsigned_map_type;

    static std::string type_name() { return "SB_trans_uint16"; }

    pointers_map& get(D_ParserTablesReader* p) const { return p->parserTablesHolder->sbTransUint16Pointers; }

    static void fill_shift(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {    }
    static void fill_table(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_table(sb, values);
    }
    static SBUintElement<sb_uint_type> parse_shift(D_ParserTablesReader* p, lexertl::recursive_match_results<const char* >& results, bool& eof)
    {
      return SBUintElement<sb_uint_type>();
    }

  };

  template<>
  struct SBTypeTrait<SB_trans_uint32>
  {
    typedef SB_trans_uint32                     sb_uint_type;
    typedef sb_trans_uint32_pointers_map        pointers_map;
    typedef unsigned int                        scanner_block_type;
    typedef unsigned_int_pointers_map           unsigned_map_type;

    static std::string type_name() { return "SB_trans_uint32"; }

    pointers_map& get(D_ParserTablesReader* p) const { return p->parserTablesHolder->sbTransUint32Pointers; }

    static void fill_shift(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {    }
    static void fill_table(sb_uint_type* sb, const std::vector<SBUintElement<sb_uint_type>>& values)
    {
      ::fill_table(sb, values);
    }
    static SBUintElement<sb_uint_type> parse_shift(D_ParserTablesReader* p, lexertl::recursive_match_results<const char* >& results, bool& eof)
    {
      return SBUintElement<sb_uint_type>();
    }

  };

  enum LangElem {
    DIGIT = 1,
    TEXTORID,
    COLON,
    D_REDUCTION_ID,
    D_SHIFT_ID,
    D_STATE_ID,
    D_SYMBOL,
    D_RIGHT_EPSILON_HINT,
    D_ERROR_RECOVERY_HINT,
    SYMBOL_DEF,
    D_PARSE_TABLES,
    VOID_KEYWORD,
    SIZEOF_KEYWORD,
    NULL_KEYWORD,
    ASSIGNMENT,
    OPEN_COMMENT,
    CLOSE_COMMENT,
    OPEN_BRACE,
    CLOSE_BRACE,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_PARENTHESIS,
    CLOSE_PARENTHESIS,
    ADDRESS,
    POINTER,
    POINTERTOPOINTER,
    UNSIGNED_TYPE,
    CHAR_TYPE,
    SBUINT8,
    SBUINT16,
    SBUINT32,
    SB_TRANS_UINT8,
    SB_TRANS_UINT16,
    SB_TRANS_UINT32,
    REDUCTION_CODE,
    REDUCTION_CODE_KEY,
    COMMA,
    SEMICOLON,
    QUOTE,
    EOL
  };

  enum SCAN_KIND
  {
    SCAN_ALL = 0,
    SCAN_LONGEST = 1,
    SCAN_MIXED = 2,
    SCAN_TRAILING	= 3,
    SCAN_RESERVED	= 4,
    SCAN_DEFAULT = 0
  };

public:
  D_ParserTablesReader();  
  
  void getNextToken(lexertl::recursive_match_results<const char* >& results, bool& eof);
  bool isEOL(const lexertl::recursive_match_results<const char* >& results, bool eof);
  void parseComment(lexertl::recursive_match_results<const char* >& results, bool& eof);
  D_ReductionPtr parseReduction(lexertl::recursive_match_results<const char* >& results, bool& eof);
  
  D_ShiftPtr parseShift(lexertl::recursive_match_results<const char* >& results, bool& eof);

  template<typename T, typename POINTERMAP, typename MAP>
  void parsePointersTable(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof, POINTERMAP& spm, const MAP& m)
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

    T** obj = new T*[values.size()];
    for (size_t i = 0; i < values.size(); i++)
    {
      if (values[i] == "0" || values[i] == "NULL") obj[i] = static_cast<T*>(0);
      else {
        // get value from shiftMap
        obj[i] = m.find(values[i])->second;
      }

    }
    spm[name] = std::make_pair(obj, values.size());
    // create address to name mapping
    parserTablesHolder->addressSymbolTable[obj] = name;
  }

  void parseShiftPointersToPointersTable(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof);
  
  template<typename T, typename MAP>
  void parseUnsignedTable(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof, MAP& map)
  {
    getToken<OPEN_BRACE>(results, eof, "expected '{'");
    std::vector<std::string> values;
    values.reserve(1000);
    
    do {
      getNextToken(results, eof);
      switch (results.id)
      {
      case COMMA:
        continue;
      case DIGIT:
        values.push_back(results.str());
        break;
      case CLOSE_BRACE:
        break;
      default:
        std::string err = "Expected id or 0 but got " + results.str();
        throw SyntaxError(err.c_str());
      }
    } while (results.id != CLOSE_BRACE);
    
    T* buffer = new T[values.size()];
    for (size_t i = 0; i < values.size(); i++) {
      buffer[i] = lexical_cast<T>(values[i]);
    }
    map[name] = std::make_pair(buffer, values.size());
    parserTablesHolder->addressSymbolTable[buffer] = name;
  }

  template<typename SBUintType>
  void parseSBUint(const std::string& type, const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof)
  {
    auto values = parseElements<SBUintType>(results, eof);

    SBTypeTrait<SBUintType> sb_pointer_selector;
    typename SBTypeTrait<SBUintType>::pointers_map& pmap = sb_pointer_selector.get(this);

    SBUintType * sb = new SBUintType[values.size()];

    SBTypeTrait<SBUintType>::fill_shift(sb, values);
    SBTypeTrait<SBUintType>::fill_table(sb, values);

    pmap[name] = std::make_pair(sb, values.size());
    parserTablesHolder->addressSymbolTable[sb] = name;
  }

  template<typename SBUintType>
  std::vector<SBUintElement<SBUintType>> parseElements(lexertl::recursive_match_results<const char* >& results, bool& eof)
  {
    std::vector<SBUintElement<SBUintType>> values;
    getToken<OPEN_BRACE>(results, eof, "expected '{'");
    do {
      SBUintElement<SBUintType> element = parseSBUintElement<SBUintType>(results, eof);
      values.push_back(element);
      getNextToken(results, eof);
      switch (results.id)
      {
      case COMMA:
        continue;
      }
    } while (results.id != CLOSE_BRACE);
    return values;
  }

  template<typename SBUintType>
  SBUintElement<SBUintType> fillShift(const std::string& shiftKey, lexertl::recursive_match_results<const char* >& results, bool& eof)
  {
    SBUintElement<SBUintType> element;
    element.D_ShiftPointerPointer = NULL;
    auto it = parserTablesHolder->shiftMapPointer.find(shiftKey);

    if (it != parserTablesHolder->shiftMapPointer.end()) element.D_ShiftPointerPointer = it->second.first;
    
    return element;
  }

  template<typename SBUintType>
  SBUintElement<SBUintType> parseSBUintElement(lexertl::recursive_match_results<const char* >& results, bool& eof)
  {
    getToken<OPEN_BRACE>(results, eof, "expected '{'");
    auto element = SBTypeTrait<SBUintType>::parse_shift(this, results, eof);

    // select specific scanner block type based on 
    // passed SBUintType 
    // which can be
    // 1-> SB_uint8
    // 2-> SB_uint16
    // 3-> SB_uint32

    typedef typename SBTypeTrait<SBUintType>::scanner_block_type scanner_block_type;
    typedef typename SBTypeTrait<SBUintType>::unsigned_map_type unsigned_map_type;
    auto scanner_block = parseScannerBlockTable<scanner_block_type, unsigned_map_type>(results, eof);
    element.scannerBlockElements = scanner_block;
    getToken<CLOSE_BRACE>(results, eof, "expected '}'");
    return element;
  }

  template<typename SCANNER_BLOCK_TYPE, typename MAP_TYPE>
  std::vector<SCANNER_BLOCK_TYPE*> parseScannerBlockTable(lexertl::recursive_match_results<const char* >& results, bool& eof)
  {
    std::vector<std::string> values;
    values.reserve(1000);
    getToken<OPEN_BRACE>(results, eof, "expected '{'");
    do {
      getNextToken(results, eof);
      switch (results.id)
      {
      case COMMA:
        continue;
      case TEXTORID:
        values.push_back(results.str());
        break;
      }

    } while (results.id != CLOSE_BRACE);

    // create scanner_block_type* table 
    // scanner_block_type can be
    // 1) unsigned char
    // 2) unsigned short
    // 3) unsigned int
    
    std::vector<SCANNER_BLOCK_TYPE*>  scanner_table;
    scanner_table.reserve(1000);
    int index = 0;
    for each (auto value in values)
    {      
      UnsignedTableSelector<SCANNER_BLOCK_TYPE> ut;
      MAP_TYPE& m = ut.get(this);
      // find specific pointers in tables
      auto it = m.find(value);
      // fill scanner_table with found pointers
      if (it != m.end()) scanner_table.push_back(it->second.first); 
      index++;
    }
    
    return scanner_table;
  }
  
  void parseState(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof);
  D_State parseStateElement(lexertl::recursive_match_results<const char* >& results, bool& eof);
  void parseVoidPtr(lexertl::recursive_match_results<const char* >& results, bool& eof);
  void parseSymbol(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof);
  D_Symbol parseSymbolElement(lexertl::recursive_match_results<const char* >& results, bool& eof);
  D_ParserTables* parseTable(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof);
  void parseRightEpsilonHint(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof);
  D_RightEpsilonHint parseRightEpsilonHintElement(lexertl::recursive_match_results<const char* >& results, bool& eof);
  void parseErrorRecoveryHint(const std::string& name, lexertl::recursive_match_results<const char* >& results, bool& eof);
  D_ErrorRecoveryHint parseErrorRecoveryHintElement(lexertl::recursive_match_results<const char* >& results, bool& eof);

  SCAN_KIND scanKindFromString(const std::string& kind);
  std::string scanKindToString(SCAN_KIND kind) const;
  unsigned int symbolFromString(const std::string& symbol);
  std::string symbolConstToString(unsigned int symbol) const;

  template<typename T,typename MAP>
  void getAndBind(const MAP& map, const std::string& name, T& value)
  {
    auto i = map.find(name);
    if (i != map.end()) { value = i->second.first; }
    else value = 0;
  }

  D_ParserTables* parse(const char* begin, const char* end, const std::unordered_map<std::string, reductionf>& builtRMappings);

  std::string langElemToString(LangElem elem);

  template<LangElem elem>
  std::string getToken(lexertl::recursive_match_results<const char* >& results, bool& eof, const std::string& err)
  {
    getNextToken(results, eof);
    if (results.id != elem) {
      std::string e = err.c_str();
      e += " ";
      e += "but got ";
      e += results.str();
      e += " line: ";
      std::stringstream ss;
      ss << currentLine;
      e += (ss.str());
      throw SyntaxError(e.c_str());
    }
    return results.str();
  }

  template<typename Iterator>
  void getNextTokenHelper(lexertl::recursive_match_results<Iterator>& results,bool& eof)
  {
    do 
    {
      lexertl::lookup (sm, results);
     
      if (results.id == 0) 
      { 
        eof = true;
        break;
      }
      
    }
    while(results.id == results.npos()) ;
  }
  
  std::string reductionsToString() const;
  std::string shiftsToString() const;
  std::string shiftPointerTablesToString() const;
  std::string reductionPointerTablesToString() const;
  std::string shiftPointerPointerTablesToString() const;
  std::string stateToString() const;
  std::string symbolToString() const;
  std::string parserTablesToString(const D_ParserTables* parserTables) const;
  std::string rightEpsilonHintToString() const;
  std::string errorRecoveryHintToString() const;
  std::string toString(const D_ParserTables* parserTables);

  template<typename Table>
  std::string unsignedTableToString(const Table& table) const
  {
    std::string value;
    for (const auto & table : table) {
      size_t size = table.second.second;
      value += "unsigned ";
      value += PrintUnsignedTableType<Table>::toString();
      value += " ";
      value += table.first;
      value += "[";
      value += to_string(size);
      value += "]";
      value += "=";
      value += "{";
      for (size_t i = 0; i < size; i++) {
        if (i > 0) value += ",";
        value += to_string(table.second.first[i]);
      }
      value += "}";
      value += "\n";
    }
    return value;
  }

  template<typename T>
  std::string SB_Shift_print(T ptr)
  {
    std::string value;
    value += this->parserTablesHolder->addressSymbolTable[ptr.shift];
    value += ",";
    return value;
  }

  template<typename T>
  struct shift_printer;
  
  template<> struct shift_printer<SB_uint8> 
  {
    static std::string print(D_ParserTablesReader* p, SB_uint8 ptr) { return p->SB_Shift_print(ptr); }
  };

  template<> struct shift_printer<SB_uint16>
  {
    static std::string print(D_ParserTablesReader* p, SB_uint16 ptr) { return p->SB_Shift_print(ptr); }
  };

  template<> struct shift_printer<SB_uint32>
  {
    static std::string print(D_ParserTablesReader* p, SB_uint32 ptr) { return p->SB_Shift_print(ptr); }
  };

  template<> struct shift_printer<SB_trans_uint8>
  {
    static std::string print(D_ParserTablesReader* p, SB_trans_uint8 ptr) { return ""; }
  };

  template<> struct shift_printer<SB_trans_uint16>
  {
    static std::string print(D_ParserTablesReader* p, SB_trans_uint16 ptr) { return ""; }
  };

  template<> struct shift_printer<SB_trans_uint32>
  {
    static std::string print(D_ParserTablesReader* p, SB_trans_uint32 ptr) { return ""; }
  };

  template<typename T>
  std::string sbUintTableToString() 
  {    
    SBTypeTrait<T> sb_pointer_selector;
    typename SBTypeTrait<T>::pointers_map& pmap = sb_pointer_selector.get(this);

    if (pmap.empty()) return "";
    std::string value;
    value += SBTypeTrait<T>::type_name();
    value += " ";

    for (auto & sbUint : pmap) {
      size_t size = sbUint.second.second;
      value += sbUint.first;
      value += "[";
      value += to_string(size);
      value += "]";
      value += "=";
      value += "{";
      for (size_t i = 0; i < size; i++){
        if (i > 0) value += ",";
        value += '\n';
        value += "{";        
        value += shift_printer<T>::print(this, sbUint.second.first[i]);
        value += "{";

        for (size_t k = 0; k < SCANNER_BLOCKS; k++) {
          if (k > 0) value += ",";
          value += parserTablesHolder->addressSymbolTable[sbUint.second.first[i].scanner_block[k]];
        }
        value += "}";
        value += "}";
        
      }
      value += "}";
      value += "\n";
    }
    return value;
  }

  ParserTablesHolderPtr              parserTablesHolder;

private:  
  lexertl::rules                     rules;
  lexertl::state_machine             sm;
  size_t                             currentLine;
    
};

ParserTablesHolderPtr parse(const std::string& file, D_ParserTablesReader& p, const std::unordered_map<std::string, reductionf>& builtinRMappings);
ParserTablesHolderPtr parse(const std::vector<char>& buffer, D_ParserTablesReader& p, const std::unordered_map<std::string, reductionf>& builtinRMappings);

#endif
