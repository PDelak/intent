// Copyright (c) 2016 Przemyslaw Delewski 
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file licence_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef TYPESET_LANG_ELEMENTS_H
#define TYPESET_LANG_ELEMENTS_H

 typedef enum {
    DIGIT = 1,
    ONE_MORE,
    TEXTORID,
    PROPERTY,
    TYPE,
    TYPESET,
    ASSIGNMENT,
    SEMICOLON,
    COMMA,
    OR,
    QUESTION_MARK,
    MANY,
    REGEX,
    LITERAL,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    LEFT_PAREN,
    RIGHT_PAREN,
    EOL
  } LangElem;

#endif
