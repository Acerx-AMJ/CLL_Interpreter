#include "parser.hpp"
#include "fmt.hpp"

Parser::Parser(std::vector<Token>& tokens)
   : tokens(tokens) {}

Program& Parser::parse() {
   while (!is(Type::eof)) {
      program.statements.push_back(std::move(parse_expr()));
   }
   return program;
}

// Parse statement functions

Stmt Parser::parse_stmt() {
   auto& token = current();

   if (token.type == Type::keyword) {
      if (token.lexeme == "let"s || token.lexeme == "con"s) {
         return parse_var_decl();
      } else if (token.lexeme == "fn"s) {
         return parse_fn_decl();
      } else if (token.lexeme == "delete"s) {
         return parse_del_stmt();
      } else if (token.lexeme == "exists"s) {
         return parse_exists_stmt();
      } else if (token.lexeme == "if"s) {
         return parse_if_else_stmt();
      } else if (token.lexeme == "while"s) {
         return parse_while_loop();
      } else if (token.lexeme == "for"s) {
         return parse_for_loop();
      } else if (token.lexeme == "break"s) {
         advance();
         return parse_unless_stmt(BreakStmt::make(line()));
      } else if (token.lexeme == "continue"s) {
         advance();
         return parse_unless_stmt(ContinueStmt::make(line()));
      } else if (token.lexeme == "return"s) {
         return parse_return_stmt();
      } else if (token.lexeme == "do"s) {
         return parse_unless_stmt(parse_block());
      } else {
         fmt::raise(token.line, "Unknown keyword '{}'.", token.lexeme);
      }
   }
   return parse_expr();
}

Stmt Parser::parse_var_decl() {
   bool constant = current().lexeme == "con"s;
   advance();

   std::vector<Stmt> identifiers;
   auto identifier = std::move(parse_primary_expr());
   fmt::raise_if(line(), identifier->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead.", stmt_type_str[int(identifier->type)]);

   while (is(Type::comma)) {
      advance();
      identifiers.push_back(std::move(identifier));
      identifier = std::move(parse_primary_expr());
      fmt::raise_if(line(), identifier->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead.", stmt_type_str[int(identifier->type)]);
   }

   identifiers.push_back(std::move(identifier));
   std::vector<Stmt> body;

   if (is(Type::assign)) {
      advance();
      auto value = std::move(parse_expr());

      while (is(Type::comma)) {
         advance();
         body.push_back(std::move(value));
         value = std::move(parse_expr());
      }
      body.push_back(std::move(value));

      fmt::raise_if(line(), body.size() > identifiers.size(), "Expected identifier count to be bigger than value count.");
      fmt::raise_if(line(), constant && body.size() != 1 && body.size() != identifiers.size(), "Expected constant variable's value count to be equal to 1 or identifier count.");
      return VarDeclaration::make(constant, std::move(identifiers), std::move(body), line());
   }

   fmt::raise_if(line(), constant, "Expected constant variableto have initialized value.");
   return VarDeclaration::make(constant, std::move(identifiers), std::move(body), line());
}

Stmt Parser::parse_fn_decl() {
   advance();
   auto original_line = line();
   auto identifier = parse_primary_expr();
   fmt::raise_if(line(), identifier->type != StmtType::identifier, "Expected identifier after 'fn' keyword, got '{}' instead.", stmt_type_str[int(identifier->type)]);

   fmt::raise_if(line(), !is(Type::l_paren), "Expected '(' after 'fn {}', got '{}' instead.", get_stmt<IdentLiteral>(identifier).identifier, type_str[int(current().type)]);
   advance();

   // For now skip arguments
   std::vector<Stmt> arguments;

   fmt::raise_if(line(), !is(Type::r_paren), "Expected ')' after '('/parameter list, got '{}' instead.", type_str[int(current().type)]);
   advance();

   // For now skip returns
   auto returns = NullLiteral::make(line());

   auto body = parse_block();
   return parse_unless_stmt(FnDeclaration::make(std::move(identifier), std::move(arguments), std::move(returns), std::move(body), original_line));
}

Stmt Parser::parse_exists_stmt() {
   advance();
   auto identifier = parse_primary_expr();
   fmt::raise_if(line(), identifier->type != StmtType::identifier, "Expected identifier after 'exists' statement, got '{}' instead.", stmt_type_str[int(identifier->type)]);
   return ExistsStmt::make(std::move(identifier), line());
}

Stmt Parser::parse_del_stmt() {
   advance();

   std::vector<Stmt> identifiers;
   auto identifier = std::move(parse_primary_expr());
   fmt::raise_if(line(), identifier->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead.", stmt_type_str[int(identifier->type)]);

   while (is(Type::comma)) {
      advance();
      identifiers.push_back(std::move(identifier));
      identifier = std::move(parse_primary_expr());
      fmt::raise_if(line(), identifier->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead.", stmt_type_str[int(identifier->type)]);
   }

   identifiers.push_back(std::move(identifier));
   std::vector<Stmt> body;
   return parse_unless_stmt(DeleteStmt::make(std::move(identifiers), line()));
}

Stmt Parser::parse_if_else_stmt() {
   auto ifclause = parse_if_clause();
   std::vector<Stmt> elifclauses;

   while (is(Type::keyword) && current().lexeme == "elif"s) {
      auto elif = parse_if_clause();
      elifclauses.push_back(std::move(elif));
   }

   if (is(Type::keyword) && current().lexeme == "else"s) {
      auto elseclause = parse_if_clause();
      return parse_unless_stmt(IfElseStmt::make(std::move(ifclause), std::move(elifclauses), std::move(elseclause), line()));
   }
   return parse_unless_stmt(IfElseStmt::make(std::move(ifclause), std::move(elifclauses), line()));
}

Stmt Parser::parse_if_clause() {
   std::string keyword = current().lexeme;
   advance();

   auto expr = (keyword == "else"s ? NullLiteral::make(line()) : parse_expr());
   auto stmt = parse_block();
   return IfClauseStmt::make(keyword, std::move(expr), std::move(stmt), line());
}

Stmt Parser::parse_while_loop() {
   advance();

   if (is(Type::keyword) && current().lexeme == "do"s || is(Type::l_brace)) {
      auto stmt = parse_block();
      return WhileStmt::make(true, NullLiteral::make(line()), std::move(stmt), line());
   }

   auto expr = parse_expr();
   auto stmt = parse_block();
   return parse_unless_stmt(WhileStmt::make(false, std::move(expr), std::move(stmt), line()));
}

Stmt Parser::parse_for_loop() {
   advance();

   if (is(Type::keyword) && current().lexeme == "do"s || is(Type::l_brace)) {
      auto stmt = parse_block();
      return ForStmt::make(std::nullopt, std::nullopt, std::nullopt, std::move(stmt), line());
   }
   std::optional<Stmt> initexpr = std::nullopt, condition = std::nullopt, loopexpr = std::nullopt;
   
   if (!is(Type::semicolon)) {
      initexpr = std::optional(parse_expr());
   }
   fmt::raise_if(line(), !is(Type::semicolon), "Expected semicolon after initial for loop expression, got '{}' instead.", type_str[int(current().type)]);
   advance();

   if (!is(Type::semicolon)) {
      condition = std::optional(parse_expr());
   }
   fmt::raise_if(line(), !is(Type::semicolon), "Expected semicolon after conditional for loop expression, got '{}' instead.", type_str[int(current().type)]);
   advance();

   if ((!is(Type::keyword) || current().lexeme != "do"s) && !is(Type::l_brace)) {
      loopexpr = std::optional(parse_expr());
   }

   auto stmt = parse_block();
   return parse_unless_stmt(ForStmt::make(std::move(initexpr), std::move(condition), std::move(loopexpr), std::move(stmt), line()));
}

Stmt Parser::parse_block() {
   if (is(Type::keyword) && current().lexeme == "do"s) {
      advance();
      auto program = std::make_unique<Program>(line());
      auto stmt = parse_expr();
      program->statements.push_back(std::move(stmt));
      return std::move(program);
   }
   fmt::raise_if(line(), !is(Type::l_brace), "Expected a 'do' keyword or a new scope.");
   return parse_primary_expr();
}

Stmt Parser::parse_return_stmt() {
   advance();
   auto expr = ReturnStmt::make(parse_expr(), line());
   return parse_unless_stmt(std::move(expr));
}

Stmt Parser::parse_unless_stmt(Stmt stmt) {
   if (is(Type::keyword) && current().lexeme == "unless"s) {
      advance();
      auto expr = parse_expr();
      return UnlessStmt::make(std::move(expr), std::move(stmt), line());
   }
   return std::move(stmt);
}

// Parse expression functions

Stmt Parser::parse_expr() {
   return parse_ternary_expr();
}

Stmt Parser::parse_ternary_expr() {
   auto left = parse_value_or_expr();

   while (is(Type::quesion)) {
      advance();

      auto middle = parse_ternary_expr();
      fmt::raise_if(line(), !is(Type::colon), "Expected ':' after '{} ? {}'.", stmt_type_str[int(left->type)], stmt_type_str[int(middle->type)]);
      advance();

      auto right = parse_ternary_expr();
      left = TernaryExpr::make(std::move(left), std::move(middle), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_value_or_expr() {
   auto left = parse_assignment_expr();

   while (is(Type::binary_cond)) {
      Type op = current().type;
      advance();

      auto right = parse_value_or_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_assignment_expr() {
   auto left = parse_logical_or_expr();

   while (is(Type::assign) || is(Type::plus_eq) || is(Type::minus_eq) || is(Type::multiply_eq) || is(Type::divide_eq) || is(Type::remainder_eq) || is(Type::exponentiate_eq)) {
      Type op = current().type;
      advance();

      auto right = parse_assignment_expr();
      left = AssignmentExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_logical_or_expr() {
   auto left = parse_logical_and_expr();

   while (is(Type::log_or)) {
      Type op = current().type;
      advance();

      auto right = parse_logical_and_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_logical_and_expr() {
   auto left = parse_equality_expr();

   while (is(Type::log_and)) {
      Type op = current().type;
      advance();

      auto right = parse_equality_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_equality_expr() {
   auto left = parse_relational_expr();

   while (is(Type::equals) || is(Type::really_equals) || is(Type::not_equals) || is(Type::really_not_equals) || is(Type::divisible)) {
      Type op = current().type;
      advance();

      auto right = parse_relational_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_relational_expr() {
   auto left = parse_additive_expr();

   while (is(Type::greater) || is(Type::greater_equal) || is(Type::smaller) || is(Type::smaller_equal)) {
      Type op = current().type;
      advance();

      auto right = parse_additive_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_additive_expr() {
   auto left = parse_multiplicative_expr();

   while (is(Type::plus) || is(Type::minus)) {
      Type op = current().type;
      advance();

      auto right = parse_multiplicative_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_multiplicative_expr() {
   auto left = parse_exponentiative_expr();

   while (is(Type::multiply) || is(Type::divide) || is(Type::remainder)) {
      Type op = current().type;
      advance();

      auto right = parse_exponentiative_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_exponentiative_expr() {
   auto left = parse_unary_expr();

   while (is(Type::exponentiate)) {
      Type op = current().type;
      advance();

      auto right = parse_exponentiative_expr();
      left = BinaryExpr::make(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_unary_expr() {
   std::vector<Type> ops;

   while (is(Type::minus) || is(Type::plus) || is(Type::log_not)) {
      ops.push_back(current().type);
      advance();
   }
   auto expr = parse_reverse_unary_expr();
   for (int i = ops.size() - 1; i >= 0; --i) {
      expr = UnaryExpr::make(ops.at(i), std::move(expr), line());
   }
   return expr;
}

Stmt Parser::parse_reverse_unary_expr() {
   auto expr = parse_call_expr();
   if (is(Type::increment) || is(Type::decrement)) {
      Type op = current().type;
      advance();
      expr = UnaryExpr::make(op, std::move(expr), line());
   }
   return expr;
}

Stmt Parser::parse_call_expr() {
   auto identifier = parse_primary_expr();
   if (identifier->type != StmtType::identifier) {
      return std::move(identifier);
   }

   while (is(Type::l_paren)) {
      auto args = parse_args_list();
      fmt::raise_if(line(), args->type != StmtType::args, "Expected arguments list after identifier, got '{}' instead.", stmt_type_str[int(args->type)]);
      identifier = CallExpr::make(std::move(args), std::move(identifier), line());
   }
   return std::move(identifier);
}

Stmt Parser::parse_args_list() {
   if (!is(Type::l_paren)) {
      return parse_primary_expr();
   }
   advance();
   std::vector<Stmt> args;

   if (is(Type::r_paren)) {
      advance();
      return ArgsListExpr::make(std::move(args), line());
   }

   auto arg = parse_expr();
   while (is(Type::comma)) {
      advance();
      if (is(Type::r_paren)) {
         break;
      }

      args.push_back(std::move(arg));
      arg = parse_expr();
   }

   args.push_back(std::move(arg));
   fmt::raise_if(line(), !is(Type::r_paren), "Expected a matching ')' after argument list, got '{}' instead.", type_str[int(current().type)]);
   advance();
   return ArgsListExpr::make(std::move(args), line());
}

Stmt Parser::parse_primary_expr() {
   if (is(Type::identifier)) {
      std::string identifier = current().lexeme;
      advance();
      return IdentLiteral::make(identifier, line());
   } else if (is(Type::number)) {
      long double number = 0.0;

      try {
         number = std::stold(current().lexeme);
      } catch (...) {
         fmt::raise(line(), "Failed to convert string '{}' to number. Number might be too large, too small, or invalid.", current().lexeme);
      }
      advance();
      return NumberLiteral::make(number, line());
   } else if (is(Type::character)) {
      char ch = current().lexeme.at(0);
      advance();
      return CharLiteral::make(ch, line());
   } else if (is(Type::string)) {
      std::string string = current().lexeme;
      advance();
      return StringLiteral::make(string, line());
   } else if (is(Type::l_paren)) {
      advance();
      auto value = parse_expr();

      fmt::raise_if(line(), !is(Type::r_paren), "Expected to find a matching parenthesis after '(', got '{}' instead.", type_str[int(current().type)]);
      advance();
      return std::move(value);
   } else if (is(Type::l_brace)) {
      advance();
      auto program = std::make_unique<Program>(line());

      while (!is(Type::eof) && !is(Type::r_brace)) {
         auto stmt = parse_expr();
         program->statements.push_back(std::move(stmt));
      }
      fmt::raise_if(program->line, !is(Type::r_brace), "Unterminated scope.");
      advance();
      return std::move(program);
   } else if (is(Type::keyword)) {
      return std::move(parse_stmt());
   } else {
      fmt::raise(line(), "Expected primary expression, got '{}' instead.", type_str[int(current().type)]);
   }
}

// Utility functions

void Parser::advance() {
   if (index + 1 < tokens.size())
      ++index;
}

bool Parser::is(Type type) const {
   return index < tokens.size() && tokens.at(index).type == type;
}

Token& Parser::current() {
   return tokens.at(index);
}

int Parser::line() const {
   return (index == 0 ? tokens.at(index).line : tokens.at(index - 1).line);
}
