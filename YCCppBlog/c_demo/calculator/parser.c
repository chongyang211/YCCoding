//
// Created by 杨充 on 2025/7/15.
//
#include "parser.h"
#include "constants.h"
#include "error_handler.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// 函数原型
static void parser_skip_whitespace(Parser* parser);
static double parser_parse_primary(Parser* parser, CalculatorError* error);
static double parser_parse_power(Parser* parser, CalculatorError* error);
static double parser_parse_term(Parser* parser, CalculatorError* error);
static double parser_parse_expression(Parser* parser, CalculatorError* error);
static double parser_parse_function_call(Parser* parser, CalculatorError* error);

// 初始化解析器
void parser_init(Parser* parser, const char* input, ErrorHandler* error_handler) {
    parser->input = input;
    parser->current = input;
    parser->error_handler = error_handler;
    parser_next_token(parser); // 获取第一个token
}

// 跳过空白
static void parser_skip_whitespace(Parser* parser) {
    while (*parser->current != '\0' && isspace((unsigned char)*parser->current)) {
        parser->current++;
    }
}

// 获取下一个标记
Token parser_next_token(Parser* parser) {
    parser_skip_whitespace(parser);

    Token token;
    token.start = parser->current;
    token.length = 1;

    if (*parser->current == '\0') {
        token.type = TOKEN_END;
        return parser->current_token = token;
    }

    char c = *parser->current;
    parser->current++;

    // 处理数字
    if (isdigit(c) || c == '.') {
        token.type = TOKEN_NUMBER;
        token.start = parser->current - 1;

        // 解析整个数字
        char* end;
        token.value = strtod(token.start, &end);
        parser->current = end;
        token.length = end - token.start;

        return parser->current_token = token;
    }

    // 处理操作符
    switch (c) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        case '^': token.type = TOKEN_POWER; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        default:
            // 处理标识符（函数名或变量）
            if (isalpha(c) || c == '_') {
                token.type = TOKEN_IDENTIFIER;
                token.start = parser->current - 1;

                // 解析整个标识符
                while (isalnum(*parser->current) || *parser->current == '_') {
                    parser->current++;
                }
                token.length = parser->current - token.start;

                // 检查是否是函数调用
                if (*parser->current == '(') {
                    token.type = TOKEN_FUNCTION;
                } else {
                    // 检查是否是常量
                    const MathConstant* constant = constants_get(token.start);
                    if (constant) {
                        token.type = TOKEN_CONSTANT;
                        token.value = constant->value;
                    } else {
                        token.type = TOKEN_IDENTIFIER;
                    }
                }

                return parser->current_token = token;
            }

            // 未知字符
            token.type = TOKEN_END;
            char error_msg[64];
            snprintf(error_msg, sizeof(error_msg), "未知字符 '%c'", c);
            error_handler_set(parser->error_handler, ERROR_SYNTAX, error_msg);
            break;
    }

    return parser->current_token = token;
}

// 解析主表达式
static double parser_parse_primary(Parser* parser, CalculatorError* error) {
    Token token = parser->current_token;

    switch (token.type) {
        case TOKEN_NUMBER: {
            parser_next_token(parser);
            return token.value;
        }
        case TOKEN_CONSTANT: {
            parser_next_token(parser);
            return token.value;
        }
        case TOKEN_MINUS: { // 处理负号
            parser_next_token(parser);
            double value = parser_parse_primary(parser, error);
            return -value;
        }
        case TOKEN_LPAREN: {
            parser_next_token(parser);
            double value = parser_parse_expression(parser, error);

            if (*error != ERROR_NONE) {
                return NAN;
            }

            if (parser->current_token.type != TOKEN_RPAREN) {
                *error = ERROR_PARENTHESIS;
                error_handler_set(parser->error_handler, ERROR_PARENTHESIS, "缺少右括号");
                return NAN;
            }

            parser_next_token(parser);
            return value;
        }
        case TOKEN_FUNCTION: {
            return parser_parse_function_call(parser, error);
        }
        case TOKEN_IDENTIFIER: {
            *error = ERROR_UNDEFINED_VAR;
            char error_msg[128];
            snprintf(error_msg, sizeof(error_msg), "未定义的变量 '%.*s'",
                    token.length, token.start);
            error_handler_set(parser->error_handler, ERROR_UNDEFINED_VAR, error_msg);
            parser_next_token(parser);
            return NAN;
        }
        default: {
            *error = ERROR_SYNTAX;
            error_handler_set(parser->error_handler, ERROR_SYNTAX, "非法的表达式");
            return NAN;
        }
    }
}

// 解析函数调用
static double parser_parse_function_call(Parser* parser, CalculatorError* error) {
    Token token = parser->current_token;
    char func_name[64];
    snprintf(func_name, sizeof(func_name), "%.*s", token.length, token.start);
    parser_next_token(parser); // 吃掉函数名

    if (parser->current_token.type != TOKEN_LPAREN) {
        *error = ERROR_SYNTAX;
        error_handler_set(parser->error_handler, ERROR_SYNTAX, "缺少括号");
        return NAN;
    }
    parser_next_token(parser); // 吃掉'('

    double arg = parser_parse_expression(parser, error);
    if (*error != ERROR_NONE) {
        return NAN;
    }

    if (parser->current_token.type != TOKEN_RPAREN) {
        *error = ERROR_PARENTHESIS;
        error_handler_set(parser->error_handler, ERROR_PARENTHESIS, "缺少右括号");
        return NAN;
    }
    parser_next_token(parser); // 吃掉')'

    // 应用函数
    if (strcmp(func_name, "sin") == 0) {
        return sin(arg);
    } else if (strcmp(func_name, "cos") == 0) {
        return cos(arg);
    } else if (strcmp(func_name, "tan") == 0) {
        return tan(arg);
    } else if (strcmp(func_name, "log") == 0) {
        if (arg <= 0) {
            *error = ERROR_DOMAIN;
            error_handler_set(parser->error_handler, ERROR_DOMAIN, "log的参数必须大于0");
            return NAN;
        }
        return log10(arg);
    } else if (strcmp(func_name, "ln") == 0) {
        if (arg <= 0) {
            *error = ERROR_DOMAIN;
            error_handler_set(parser->error_handler, ERROR_DOMAIN, "ln的参数必须大于0");
            return NAN;
        }
        return log(arg);
    } else if (strcmp(func_name, "sqrt") == 0) {
        if (arg < 0) {
            *error = ERROR_DOMAIN;
            error_handler_set(parser->error_handler, ERROR_DOMAIN, "sqrt的参数必须非负");
            return NAN;
        }
        return sqrt(arg);
    } else {
        *error = ERROR_SYNTAX;
        char error_msg[128];
        snprintf(error_msg, sizeof(error_msg), "未知函数 '%s'", func_name);
        error_handler_set(parser->error_handler, ERROR_SYNTAX, error_msg);
        return NAN;
    }
}

// 解析幂运算 (^)
static double parser_parse_power(Parser* parser, CalculatorError* error) {
    double left = parser_parse_primary(parser, error);
    if (*error != ERROR_NONE) return NAN;

    if (parser->current_token.type == TOKEN_POWER) {
        parser_next_token(parser);
        double right = parser_parse_power(parser, error); // 右结合
        if (*error != ERROR_NONE) return NAN;
        return pow(left, right);
    }

    return left;
}

// 解析乘除运算 (* /)
static double parser_parse_term(Parser* parser, CalculatorError* error) {
    double result = parser_parse_power(parser, error);
    if (*error != ERROR_NONE) return NAN;

    while (1) {
        Token token = parser->current_token;

        if (token.type == TOKEN_MULTIPLY) {
            parser_next_token(parser);
            double right = parser_parse_power(parser, error);
            if (*error != ERROR_NONE) return NAN;
            result *= right;
        } else if (token.type == TOKEN_DIVIDE) {
            parser_next_token(parser);
            double right = parser_parse_power(parser, error);
            if (*error != ERROR_NONE) return NAN;

            if (right == 0.0) {
                *error = ERROR_DIV_ZERO;
                error_handler_set(parser->error_handler, ERROR_DIV_ZERO, "除以零错误");
                return NAN;
            }
            result /= right;
        } else {
            break;
        }
    }

    return result;
}

// 解析加减运算 (+ -)
static double parser_parse_expression(Parser* parser, CalculatorError* error) {
    // 检查是否为空表达式
    if (parser->current_token.type == TOKEN_END) {
        *error = ERROR_SYNTAX;
        error_handler_set(parser->error_handler, ERROR_SYNTAX, "表达式为空");
        return NAN;
    }

    double result = parser_parse_term(parser, error);
    if (*error != ERROR_NONE) return NAN;

    while (1) {
        Token token = parser->current_token;

        if (token.type == TOKEN_PLUS) {
            parser_next_token(parser);
            double term = parser_parse_term(parser, error);
            if (*error != ERROR_NONE) return NAN;
            result += term;
        } else if (token.type == TOKEN_MINUS) {
            parser_next_token(parser);
            double term = parser_parse_term(parser, error);
            if (*error != ERROR_NONE) return NAN;
            result -= term;
        } else {
            break;
        }
    }

    return result;
}