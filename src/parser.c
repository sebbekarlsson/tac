#include "include/parser.h"
#include "include/types.h"
#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


parser_T* init_parser(lexer_T* lexer)
{
  parser_T* parser = calloc(1, sizeof(struct PARSER_STRUCT));
  parser->lexer = lexer;
  parser->token = lexer_next_token(lexer);

  return parser;
}

token_T* parser_eat(parser_T* parser, int type)
{
  if (parser->token->type != type)
  {
    printf("[Parser]: Unexpected token: `%s`, was expecting: `%s`\n", token_to_str(parser->token), token_type_to_str(type));
    exit(1);
  }

  parser->token = lexer_next_token(parser->lexer);
  return parser->token;
}

AST_T* parser_parse(parser_T* parser)
{
  return parser_parse_compound(parser);
}

AST_T* parser_parse_id(parser_T* parser)
{
  char* value = calloc(strlen(parser->token->value) + 1, sizeof(char));
  strcpy(value, parser->token->value);
  parser_eat(parser, TOKEN_ID);

  if (parser->token->type == TOKEN_EQUALS)
  {
    parser_eat(parser, TOKEN_EQUALS);
    AST_T* ast = init_ast(AST_ASSIGNMENT);
    ast->name = value;
    ast->value = parser_parse_expr(parser);
    return ast;
  }

  AST_T* ast = init_ast(AST_VARIABLE);
  ast->name = value;


  if (parser->token->type == TOKEN_COLON)
  {
    parser_eat(parser, TOKEN_COLON);

    while (parser->token->type == TOKEN_ID)
    {
      ast->data_type = typename_to_int(parser->token->value);
      parser_eat(parser, TOKEN_ID);

      if (parser->token->type == TOKEN_LT)
      {
        parser_eat(parser, TOKEN_LT);
        ast->data_type += typename_to_int(parser->token->value);
        parser_eat(parser, TOKEN_ID);
        parser_eat(parser, TOKEN_GT);
      }
    }
  }
  else
  {
    if (parser->token->type == TOKEN_LPAREN)
    {
      ast->type = AST_CALL;
      ast->value = parser_parse_list(parser);
    }
    else
    if (parser->token->type == TOKEN_LBRACKET)
    {
      parser_eat(parser, TOKEN_LBRACKET);
      ast->type = AST_ACCESS;
      ast->int_value = atoi(parser->token->value);
      parser_eat(parser, TOKEN_INT);
      parser_eat(parser, TOKEN_RBRACKET);
    }
  }

  return ast;
}

AST_T* parser_parse_block(parser_T* parser)
{
  parser_eat(parser, TOKEN_LBRACE);
  
  AST_T* ast = init_ast(AST_COMPOUND);

  while (parser->token->type != TOKEN_RBRACE)
  {
    list_push(ast->children, parser_parse_expr(parser));
  }

  parser_eat(parser, TOKEN_RBRACE);

  return ast;
}

AST_T* parser_parse_list(parser_T* parser)
{
  unsigned int is_bracket = parser->token->type == TOKEN_LBRACKET;

  parser_eat(parser, is_bracket ? TOKEN_LBRACKET : TOKEN_LPAREN);

  AST_T* ast = init_ast(AST_COMPOUND);
  
  list_push(ast->children, parser_parse_expr(parser));

  while (parser->token->type == TOKEN_COMMA)
  {
    parser_eat(parser, TOKEN_COMMA);
    list_push(ast->children, parser_parse_expr(parser));
  }
 
  parser_eat(parser, is_bracket ? TOKEN_RBRACKET : TOKEN_RPAREN);

  if (parser->token->type == TOKEN_COLON)
  {
    parser_eat(parser, TOKEN_COLON);

    while (parser->token->type == TOKEN_ID)
    {
      ast->data_type = typename_to_int(parser->token->value);
      parser_eat(parser, TOKEN_ID);

      if (parser->token->type == TOKEN_LT)
      {
        parser_eat(parser, TOKEN_LT);
        ast->data_type += typename_to_int(parser->token->value);
        parser_eat(parser, TOKEN_ID);
        parser_eat(parser, TOKEN_GT);
      }
    }
  }

  if (parser->token->type == TOKEN_ARROW_RIGHT)
  {
    parser_eat(parser, TOKEN_ARROW_RIGHT);
    ast->type = AST_FUNCTION;
    ast->value = parser_parse_compound(parser);
  }

  return ast;
}

AST_T* parser_parse_int(parser_T* parser)
{
  int int_value = atoi(parser->token->value);
  parser_eat(parser, TOKEN_INT);

  AST_T* ast = init_ast(AST_INT);
  ast->int_value = int_value;

  return ast;
}

AST_T* parser_parse_string(parser_T* parser)
{
  char* value = mkstr(parser->token->value);
  parser_eat(parser, TOKEN_STRING);

  AST_T* ast = init_ast(AST_STRING);
  ast->string_value = value;

  return ast;
}

AST_T* parser_parse_expr(parser_T* parser)
{
  switch (parser->token->type)
  {
    case TOKEN_ID: return parser_parse_id(parser);
    case TOKEN_LPAREN: return parser_parse_list(parser);
    case TOKEN_INT: return parser_parse_int(parser);
    case TOKEN_STRING: return parser_parse_string(parser);
    default: { printf("[Parser]: Unexpected token `%s`\n", token_to_str(parser->token)); exit(1); };
  }
}

AST_T* parser_parse_compound(parser_T* parser)
{
  unsigned int should_close = 0;

  if (parser->token->type == TOKEN_LBRACE)
  {
    parser_eat(parser, TOKEN_LBRACE);
    should_close = 1;
  }

  AST_T* compound = init_ast(AST_COMPOUND);

  while (parser->token->type != TOKEN_EOF && parser->token->type != TOKEN_RBRACE)
  {
    list_push(compound->children, parser_parse_expr(parser));

    if (parser->token->type == TOKEN_SEMI)
      parser_eat(parser, TOKEN_SEMI);
  }

  if (should_close)
    parser_eat(parser, TOKEN_RBRACE);

  return compound;
}
