//
// Created by 杨充 on 2025/7/15.
//

#ifndef PARSER_H
#define PARSER_H

#include "error_handler.h"
#include "constants.h"

// 词法分析器标记
typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_POWER,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_IDENTIFIER,
    TOKEN_FUNCTION,
    TOKEN_CONSTANT,
    TOKEN_END
} TokenType;

// 标记结构
typedef struct {
    TokenType type;
    double value;
    const char* start;
    int length;
} Token;

// 解析器状态
typedef struct {
    const char* input;
    const char* current;
    Token current_token;
    ErrorHandler* error_handler;
} Parser;

// 初始化解析器
void parser_init(Parser* parser, const char* input, ErrorHandler* error_handler);

// 获取下一个标记
Token parser_next_token(Parser* parser);

// 解析表达式
double parser_parse_expression(Parser* parser, CalculatorError* error);

#endif // PARSER_H
