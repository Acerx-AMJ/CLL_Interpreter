#include "parser.hpp"
#include "fmt.hpp"

Parser::Parser(std::vector<Token>& tokens)
   : tokens(tokens) {}

Program& Parser::parse() {
   while (!is(Type::eof)) {
      program.statements.push_back(std::move(parse_stmt()));
   }
   return program;
}

// Parse statement functions

Stmt Parser::parse_stmt() {
   auto& token = current();

   if (token.type == Type::keyword) {
      if (token.lexeme == "let"s || token.lexeme == "con"s) {
         return std::move(parse_var_decl());
      }
   }
   return std::move(parse_expr());
}

Stmt Parser::parse_var_decl() {
   bool constant = current().lexeme == "con"s;
   advance();

   std::vector<Stmt> identifiers;
   auto identifier = std::move(parse_primary_expr());
   fmt::raise_if(identifier->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead at line {}.", stmt_type_str[int(identifier->type)], line());

   while (is(Type::comma)) {
      advance();
      identifiers.push_back(std::move(identifier));
      identifier = std::move(parse_primary_expr());
      fmt::raise_if(identifier->type != StmtType::identifier, "Expected 'IdentifierLiteral', got '{}' instead at line {}.", stmt_type_str[int(identifier->type)], line());
   }

   identifiers.push_back(std::move(identifier));
   std::vector<Stmt> body;

   if (is(Type::equals)) {
      advance();
      auto value = std::move(parse_expr());

      while (is(Type::comma)) {
         advance();
         body.push_back(std::move(value));
         value = std::move(parse_expr());
      }
      body.push_back(std::move(value));

      fmt::raise_if(body.size() > identifiers.size(), "Expected identifier count to be bigger than value count at line {}.", line());
      return std::make_unique<VarDeclaration>(constant, std::move(identifiers), std::move(body), line());
   }

   fmt::raise_if(constant, "Expected constant variable at line {} to have initialized value.", line());
   return std::make_unique<VarDeclaration>(constant, std::move(identifiers), std::move(body), line());
}

// Parse expression functions

Stmt Parser::parse_expr() {
   return parse_assignment_expr();
}

Stmt Parser::parse_assignment_expr() {
   auto left = parse_additive_expr();

   while (is(Type::equals)) {
      Type op = current().type;
      advance();

      auto right = parse_assignment_expr();
      left = std::make_unique<AssignmentExpr>(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_additive_expr() {
   auto left = parse_multiplicative_expr();

   while (is(Type::plus) || is(Type::minus)) {
      Type op = current().type;
      advance();

      auto right = parse_multiplicative_expr();
      left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_multiplicative_expr() {
   auto left = parse_unary_expr();

   while (is(Type::multiply) || is(Type::divide) || is(Type::remainder)) {
      Type op = current().type;
      advance();

      auto right = parse_unary_expr();
      left = std::make_unique<BinaryExpr>(op, std::move(left), std::move(right), line());
   }
   return std::move(left);
}

Stmt Parser::parse_unary_expr() {
   if (is(Type::minus) || is(Type::plus)) {
      Type op = current().type;
      advance();

      auto expr = parse_call_expr();
      return std::make_unique<UnaryExpr>(op, std::move(expr), line());
   }
   return parse_call_expr();
}

Stmt Parser::parse_call_expr() {
   auto identifier = parse_primary_expr();
   if (identifier->type != StmtType::identifier || !is(Type::l_paren)) {
      return std::move(identifier);
   }
   auto args = parse_args_list();
   fmt::raise_if(args->type != StmtType::args, "Expected arguments list after identifier, got '{}' instead at line {}.", stmt_type_str[int(args->type)], line());
   return std::make_unique<CallExpr>(std::move(args), std::move(identifier), line());
}

Stmt Parser::parse_args_list() {
   if (!is(Type::l_paren)) {
      return parse_primary_expr();
   }
   advance();
   std::vector<Stmt> args;

   if (is(Type::r_paren)) {
      advance();
      return std::make_unique<ArgsListExpr>(std::move(args), line());
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
   fmt::raise_if(!is(Type::r_paren), "Expected a matching ')' after argument list, got '{}' instead at line {}.", type_str[int(current().type)], line());
   advance();
   return std::make_unique<ArgsListExpr>(std::move(args), line());
}

Stmt Parser::parse_primary_expr() {
   if (is(Type::identifier)) {
      std::string identifier = current().lexeme;
      advance();
      return std::make_unique<IdentLiteral>(identifier, line());
   } else if (is(Type::number)) {
      long double number = 0.0;

      try {
         number = std::stold(current().lexeme);
      } catch (...) {
         fmt::raise("Failed to convert string '{}' to number at line {}.", current().lexeme, line());
      }
      advance();
      return std::make_unique<NumberLiteral>(number, line());
   } else if (is(Type::l_paren)) {
      advance();
      auto value = parse_expr();

      fmt::raise_if(!is(Type::r_paren), "Expected to find a matching parenthesis after '(', got '{}' instead at line {}.", type_str[int(current().type)], line());
      advance();
      return std::move(value);
   } else {
      fmt::raise("Expected primary expression, got '{}' instead at line {}.", type_str[int(current().type)], line());
      return std::make_unique<NullLiteral>();
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
