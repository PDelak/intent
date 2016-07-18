// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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

extern D_ParserTables parser_tables_dparser_gram;
extern ProgramPtr programPtr;
std::unordered_map<size_t, std::pair<std::string, std::string>> IdToMatchCase;
static std::string compilationUnit;

const int MAX_LINE_LENGTH = 44;  /* must be at least 4 */
const int INDENT_SPACES = 4;

void DParser_pass(char* path)
{
  int set_op_priority_from_rule = 0;
  int right_recursive_BNF = 0;
  int states_for_whitespace = 1;
  int states_for_all_nterms = 0;
  int tokenizer = 0;
  int longest_match = 1;
  char grammar_ident[256] = "gram";
  int ident_from_filename = 0;
  int scanner_blocks = 4;
  int scanner_block_size = 0;
  int write_line_directives = 1;
  int write_header = -1;
  int token_type = 0;
  char write_extension[256] = "c";

  char output_file[1024] = "";
  char *grammar_pathname;
  Grammar *g;

  grammar_pathname = path;
  g = new_D_Grammar(grammar_pathname);
  /* grammar construction options */
  g->set_op_priority_from_rule = set_op_priority_from_rule;
  g->right_recursive_BNF = right_recursive_BNF;
  g->states_for_whitespace = states_for_whitespace;
  g->states_for_all_nterms = states_for_all_nterms;
  g->tokenizer = tokenizer;
  g->longest_match = longest_match;
  /* grammar writing options */
  strcpy(g->grammar_ident, grammar_ident);
  if (ident_from_filename) {
  	char *n = strrchr(grammar_pathname, '/'), *e;
  	n = n ? n : grammar_pathname;
  	e = strchr(n, '.');
  	e = e ? e : n + strlen(n);
  	memcpy(g->grammar_ident, n, e - n);
  	g->grammar_ident[e - n] = 0;
  }
  g->scanner_blocks = scanner_blocks;
  g->scanner_block_size = scanner_block_size;
  g->write_line_directives = write_line_directives;
  g->write_header = write_header;
  g->token_type = token_type;
  strcpy(g->write_extension, write_extension);

  if (!output_file[0]) {
  	strncpy(output_file, grammar_pathname, sizeof(output_file) - 1);
  	strncat(output_file, ".d_parser.", sizeof(output_file) - strlen(output_file) - 1);
  	strncat(output_file, g->write_extension, sizeof(output_file) - strlen(output_file) - 1);
  }
  g->write_pathname = output_file;

  /* don't print anything to stdout, when the grammar is printed there */
  if (d_rdebug_grammar_level > 0)
  	d_verbose_level = 0;

  mkdparse(g, grammar_pathname);

  if (d_rdebug_grammar_level == 0) {
  	if (write_c_tables(g) < 0)
  		d_fail("unable to write C tables '%s'", grammar_pathname);
  }
  else
  	print_rdebug_grammar(g, grammar_pathname);

  free_D_Grammar(g);
  g = 0;

}


void
visit_node_default(int depth, char *name, char *value, void *client_data);
static void
visit_parsetree(D_ParserTables pt, D_ParseNode *pn, int depth, print_node_fn_t fn, void *client_data);

static void
visit_parsetree(D_ParserTables pt, D_ParseNode *pn, int depth, print_node_fn_t fn, void *client_data) {
  int nch = d_get_number_of_children(pn), i;
  char *name = (char*)pt.symbols[pn->symbol].name;
  int len = pn->end_skip - pn->start_loc.s;
  char *value = (char*)malloc(len + 2);
  memcpy(value, pn->start_loc.s, len);
  value[len] = 0;
  fn(depth, name, value, client_data);
  free(value);

  depth++;
  if (nch != 0) {
    for (i = 0; i < nch; i++) {
      D_ParseNode *xpn = d_get_child(pn, i);
      visit_parsetree(pt, xpn, depth, fn, client_data);
    }
  }
}

void
visit_parsetree(D_ParserTables pt, D_ParseNode *pn, print_node_fn_t fn, void *client_data) {
  visit_parsetree(pt, pn, 0, (NULL == fn) ? visit_node_default : fn, client_data);
}

void
visit_node_parenthesised(int depth, char *name, char *value, void *client_data) {
  printf("( %s )", name);
}

static char *
change_newline2space(char *s) {
  char *ss = s;
  while (*ss++)
  if (*ss == '\n')
    *ss = ' ';
  if (strlen(s)>MAX_LINE_LENGTH) {
    *(s + MAX_LINE_LENGTH - 3) = '.';
    *(s + MAX_LINE_LENGTH - 2) = '.';
    *(s + MAX_LINE_LENGTH - 1) = '.';
    *(s + MAX_LINE_LENGTH) = '\0';
  }
  return s;
}

void
visit_node_default(int depth, char *name, char *value, void *client_data) {
  printf("%*s", depth*INDENT_SPACES, "");
  printf("%s  %s.\n", name, change_newline2space(value));
}


std::vector<char> readFile(const std::string& file)
{
  std::ifstream in(file);
  if (!in.is_open()) throw FileNotFoundException("FileNotFound");
  in.unsetf(std::ios::skipws);
  std::istream_iterator<char> begin(in);
  std::istream_iterator<char> end;

  std::vector<char> buffer;
  std::copy(begin, end, std::back_inserter(buffer));
  return buffer;
}


void executeLUA(const std::string& file)
{
  lua_State *lState;
  lState = luaL_newstate();
  luaL_openlibs(lState);
  luaL_dofile(lState, file.c_str());
  lua_close(lState);
}

D_ParseNode * amb(struct D_Parser *,
	int n, struct D_ParseNode **v)
{
  d_fail("unresolved ambiguity line %d file %s", v[0]->start_loc.line,
	  v[0]->start_loc.pathname);
  return v[0];
}

void tests()
{  
  assert(get_type_instance<type>() == get_type_instance<type>());
  assert(get_type_instance<type>() != get_type_instance<string_type>());

  std::tr2::sys::path p = std::tr2::sys::current_path().append("..\\tests\\");
  {
    std::vector<char> test = readFile(p.string() + "test1.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("test1.dl", false, output);
  }  
  {
    std::vector<char> test = readFile(p.string() + "test2.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("test2.dl", false, output);
  }  
  {
    std::vector<char> test = readFile(p.string() + "block1.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("block1.dl", false, output);
  }
  {
    std::vector<char> test = readFile(p.string() + "block2.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("block2.dl", false, output);
  }
  
  {
    std::vector<char> test = readFile(p.string() + "loops.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("loops.dl", false, output);
  } 
  
  {
    std::vector<char> test = readFile(p.string() + "function.lua");
    std::string output;
    std::copy(test.begin(), test.end(), std::back_inserter(output));
//    compilationPass("function.dl", false, output);
  } 
  
}

void testTypes()
{
  string_type s;
  string_type s2;
  assert(s == s2);
  type* t1 = new string_type;
  type* t2 = new string_type;
  TypePtr pt1(new string_type);
  TypePtr pt2(new string_type);
  assert(equal(t1,t2));
  assert(equal(pt1, pt2));
}

void testAstRtti()
{
  NodePtr node(new ReturnStatement(0));
  assert(is<ReturnStatement>(node));
}

void DParseTablesTests(const std::string& filename, const std::unordered_map<std::string, reductionf>& builtinRMappings)
{
  D_ParserTablesReader parser;
  auto path = std::tr2::sys::current_path().append("\\");

  auto holder = parse(path.string().append("intent.g.d_parser.c"), 
                  parser, 
                  builtinRMappings);

  std::cout << "\ncompilation success\n";
  
  auto p = new_D_Parser(holder->parserTables, sizeof(D_ParseNode_User));
  auto buffer = readFile(filename);

  const char* b = &buffer[0];
  const char* e = &buffer[0] + buffer.size();
 
  if (dparse(p, const_cast<char*>(b), 
             std::distance(b, e)) && !p->syntax_errors) 
  {
    printf("success\n");
    MatchVisitor matchVisitor;
    programPtr->accept(&matchVisitor);
  }
  else
    printf("failure\n");
  
}

void testTypeParser()
{

  type_parser tp;
  
  std::vector<char> buffer = readFile("dsl3.dl");

  const char* b = &buffer[0];
  const char* e = &buffer[0] + buffer.size();
  std::string typeset;
  try {
    size_t pos = tp.parse(b, e, typeset);
    std::cout << "pos : " << pos << std::endl;
    std::cout << typeset << std::endl;
    std::string remainingBuf;
    std::copy(b + pos, e, std::back_inserter(remainingBuf));
    std::cout << remainingBuf << std::endl;
  }

  catch (const SyntaxError& se) {
    std::cerr << "compilation error : " << se.what() << std::endl;
  }
  catch (const std::runtime_error& re) {
    std::cerr << re.what() << std::endl;
  }

}


void testFindingReferences()
{
}


// return pair of metamodel (typeset) and model
std::pair<std::string, std::string> compileDL(const std::string& filename, size_t& grammarSize)
{
  auto path = std::tr2::sys::current_path().append("\\");
  auto intent_typeset = readFile(path.string().append("intenttypeset.dl"));

  type_parser tp;
  typeset_merger tpm;
  std::string model;
  size_t pos = 0;
  auto buffer = readFile(path.string().append(filename));
  std::vector<char> tmpBuffer;
  std::string typeset;
  const std::string typeset_keyword = "grammar";

  auto typeset_it = std::search(buffer.begin(), 
                                buffer.end(), 
                                typeset_keyword.c_str(), 
                                typeset_keyword.c_str() + typeset_keyword.size());

  if (typeset_it != buffer.end()) {
    auto it = std::find(buffer.begin(), buffer.end(), '[');
    auto end_grammar = std::find(buffer.begin(), buffer.end(), ']');

    grammarSize = std::count(typeset_it, end_grammar, '\n');


    if (it != buffer.end()) {
      const char* intent_typeset_begin = &intent_typeset[0];
      const char* intent_typeset_end = &intent_typeset[0] + intent_typeset.size();
      std::string tmpTypeset;
      
      tpm.parse(intent_typeset_begin, 
                intent_typeset_end, 
                tmpTypeset);


      std::copy(buffer.begin(), 
                it + 1, 
                std::back_inserter(tmpBuffer));

      tmpBuffer.push_back('\n');

      std::copy(tmpTypeset.begin(), 
                tmpTypeset.end(), 
                std::back_inserter(tmpBuffer));

      std::copy(it + 1, 
                buffer.end(), 
                std::back_inserter(tmpBuffer));
    }
  }
  else {
    std::copy(intent_typeset.begin(), 
              intent_typeset.end(), 
              std::back_inserter(tmpBuffer));
                             
    //
    // add main production
    //
    std::string mainProd = "  syntax main = ;\n]";    
    auto it = std::find(tmpBuffer.rbegin(), tmpBuffer.rend(), ']');
    size_t size = std::distance(tmpBuffer.begin(), it.base());
    tmpBuffer.resize(size + mainProd.size() - 1);
    std::copy(mainProd.begin(), mainProd.end(), it.base() - 1 );
    
  

    std::copy(buffer.begin(), 
              buffer.end(), 
              std::back_inserter(tmpBuffer));

  }


  const char* b = &tmpBuffer[0];
  const char* e = &tmpBuffer[0] + tmpBuffer.size();
  
  pos = tp.parse(b, e, typeset);
  
  std::copy(b + pos, e, std::back_inserter(model));    
    
  std::string typeset_copy = "{\n#include <iostream>\n}\n";
  typeset_copy += typeset;

  return std::make_pair(typeset_copy, model);

}

void serializeMetamodel(const std::string& metamodel)
{
  std::ofstream out;
  out.open("tmp.g");
  out << metamodel;
}

std::unordered_map<std::string, std::string> matchCases(ProgramPtr programPtr)
{
  std::unordered_map<std::string, std::string> casesMap;
  MatchVisitor matchVisitor;
  programPtr->accept(&matchVisitor);
  for (auto match : matchVisitor.matchTable) {
    std::string suffix;
    std::string code;
    for (auto mcase : match.second.cases) {
      if (mcase.id.empty()) suffix = "_0";
      else {
        std::stringstream ss;
        ss << "_";
        ss << mcase.id;
        suffix = ss.str();
      }
      code = mcase.code;
      std::string id = "*" + match.first + suffix + "*";
      casesMap[id] = code;
      
    }
  }
  return casesMap;
}

static void
syntax_error_fn(struct D_Parser *ap) {    
    Parser *p = (Parser *)ap;
    char *after = 0;
    ZNode *z = p->snode_hash.last_all ? p->snode_hash.last_all->zns.v[0] : 0;
    while (z && z->pn->parse_node.start_loc.s == z->pn->parse_node.end)
        z = (z->sns.v && z->sns.v[0]->zns.v) ? z->sns.v[0]->zns.v[0] : 0;
    if (z && z->pn->parse_node.start_loc.s != z->pn->parse_node.end)
        after = dup_str(z->pn->parse_node.start_loc.s, z->pn->parse_node.end);
    if (after)
        fprintf(stderr, "%s : syntax error after '%s' in ", compilationUnit.c_str(), after);
    else
        fprintf(stderr, "%s : syntax error ", compilationUnit.c_str());
    if (after)
        FREE(after);
}


bool compileHelper(const std::string& fileName,
                   const std::string& metamodel,
                   const std::string& model,
                   std::unordered_map<std::string, reductionf>& builtinRMappings,
                   size_t grammarSize)
{
  D_ParserTablesReader parser;

  auto holder = parse(metamodel,
    parser,
    builtinRMappings);

  programPtr.reset(new Program);
  D_Parser *p = new_D_Parser(holder->parserTables, sizeof(NodeAdapter));  
  p->save_parse_tree = 1;
  p->ambiguity_fn = amb;
  p->syntax_error_fn = syntax_error_fn;
  
  std::string intermediate_file = "tmp.intent.lua";
  programPtr->setIntermediateName(intermediate_file);

  const char* b = model.c_str();
  const char* e = model.c_str() + model.size();
  
  bool result = (dparse(p, const_cast<char*>(b), std::distance(b, e)) && !p->syntax_errors);
  
  std::stringstream ss;
  ss << p->loc.line + grammarSize;
  std::string msg = ss.str();
  msg += " line ";

  if (!result) throw SyntaxError(msg.c_str());
  return result;

}


void addDynamicReductions(const std::string& fileName,
                          const std::string& metamodel, 
                          const std::string& model, 
                          std::unordered_map<std::string, reductionf>& builtinRMappings,
                          size_t grammarSize)
{
  if (compileHelper(fileName, metamodel, model, builtinRMappings, grammarSize)) {
    auto dynamicReductions = make_dynamic_reductions();

    auto matches = matchCases(programPtr);

    size_t index = 0;
    for (auto matchcase : matches) {
      IdToMatchCase[index] = std::make_pair(matchcase.first, matchcase.second);
      builtinRMappings.insert(std::make_pair(matchcase.first, dynamicReductions[index]));
      index++;
    }
  }
}

std::string visitMatchers(const std::string& fileName,
                          const std::string& metamodel, 
                          const std::string& model, 
                          std::unordered_map<std::string, reductionf>& builtinRMappings,
                          size_t grammarSize)
{
  std::string code;

  if (compileHelper(fileName, metamodel, model, builtinRMappings, grammarSize)) {
    std::ofstream out("matchers.out");
    AST2Code ast2CodeVisitor;
    programPtr->accept(&ast2CodeVisitor);
    out << ast2CodeVisitor.serialize() << std::endl;
    code =  ast2CodeVisitor.serialize();
  }
  return code;
}

void Execute(const std::string& fileName,
             const std::string& metamodel, 
             const std::string& model, 
             std::unordered_map<std::string, reductionf>& builtinRMappings,
             size_t grammarSize) 
{
  if (compileHelper(fileName, metamodel, model, builtinRMappings, grammarSize)) 
  {
    ASTNodeLinker nlinker;
    programPtr->accept(&nlinker);

    programPtr->test();
    if (!programPtr->isTest()) {
      std::ofstream out(programPtr->intermediateName());
      programPtr->generate(out);
    }
    std::string intermediate_file = "tmp.intent.lua";
    executeLUA(intermediate_file);
  }
}

auto make_builtin_reductions()
{
  std::unordered_map<std::string, reductionf> reductions = {
    { "*program_0*", program_reduction },
    { "*compound_statement_0*", compound_statement_reduction },
    { "*statement_0*", statement_reduction },
    { "*expression_statement_0*", expression_statement_reduction },
    { "*expression_0*", expression_reduction },
    { "*if_statement_0*", if_reduction },
    { "*if_statement_1*", if_else_reduction },
    { "*while_loop_0*", while_loop_reduction },
    { "*for_loop_0*", for_loop_reduction },
    { "*bin_expr_0*", binary_expr_reduction },
    { "*bin_expr_1*", binary_expr_paren_reduction },
    { "*unary_expression_0*", unary_expr_not_reduction },
    { "*unary_expression_1*", unary_expr_table_reduction },
    { "*let_statement_0*", let_statement_reduction },
    { "*type_ex_0*", type_reduction },
    { "*parameter_0*", parameter_reduction },
    { "*argument_0*", argument_reduction },
    { "*function_declaration_0*", function_declaration_reduction },
    { "*function_expression_0*", function_expression_reduction },
    { "*function_body_0*", function_body_reduction },
    { "*function_call_0*", function_call_reduction },
    { "*return_statement_0*", return_statement_reduction },
    { "*bool_0*", bool_reduction },
    { "*table_element_0*", table_element_reduction },
    { "*literal_0*", literal_reduction },
    { "*table_0*", table_reduction },
    { "*identifier_0*", identifier_reduction },
    { "*number_0*", number_reduction },
    { "*string_0*", string_reduction },
    { "*whitespace_0*", NULL },
    { "*singleLineComment_0*", NULL },
    { "*multiLineComment_0*", NULL },
    { "*nestedComment_0*", NULL },
    { "*match_0*", match_reduction },
    { "*match_case_0*", matchcase_reduction },
    { "*match_id_0*", matchid_reduction },
    { "*code_0*", code_reduction },
    { "*extension_0*", extension_reduction },
  };
  return reductions;
}



int main(int argc, char *argv[]) { 

  testFindingReferences();

  auto builtinRMappings = make_builtin_reductions();

  if(argc < 2) {
    std::cerr << "expected input file name" << std::endl;
    return -1;
  }
  std::string option;
  if (argc == 3) {
    option = argv[2]; 
  }
  std::string fileName = argv[1];
  compilationUnit = fileName;
  bool showTree = false;
  bool compileOnly = false;
  if (option.compare("tree") == 0) showTree = true;
  if (option.compare("gen") == 0) compileOnly = true;
  try {
    tests();
    std::string metamodel;
    std::string model;
    size_t grammarSize = 0;
    std::tie(metamodel, model) = compileDL(fileName, grammarSize);
    
    serializeMetamodel(metamodel);
    DParser_pass("tmp.g");
    addDynamicReductions(fileName, "tmp.g.d_parser.c", model, builtinRMappings, grammarSize);
    std::string codeGen = visitMatchers(fileName, "tmp.g.d_parser.c", model, builtinRMappings, 0);
    if(!compileOnly) Execute(fileName, "tmp.g.d_parser.c", codeGen, builtinRMappings, 0);
  }  
  catch (const FileNotFoundException& fe) { std::cerr << fe.what() << std::endl; }
  catch (const SymbolNotFound& snf) { std::cerr << snf.what() << std::endl; }
  catch (const SyntaxError& se) { std::cerr << se.what() << std::endl;}
  catch (const SemanticError& semaError) { std::cerr << semaError.what() << std::endl; }
  catch (const std::runtime_error& re) { std::cerr << re.what() << std::endl; }
  catch (...) { std::cerr << "unknown error" << std::endl; }
  
  return 0;
}
