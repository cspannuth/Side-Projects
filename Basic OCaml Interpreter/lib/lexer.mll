{
open Parser
}


let num = '-'?['0'-'9']+
let var = ['a'-'z' '_'] ['a'-'z' 'A'-'Z' '0'-'9' '_' '\'']*
let whitespace = [' ' '\t' '\n' '\r']+

rule read =
  parse
  | "let" { LET }
  | "=" { EQUALS }
  | "in" { IN }
  | "if" { IF }
  | "then" { THEN }
  | "else" { ELSE }
  | "fun" { FUN }
  | "->" { ARROW }
  | "(" { LPAREN }
  | ")" { RPAREN }
  | "true" { TRUE }
  | "false" { FALSE }
  | "+" { PLUS }
  | "-" { MINUS }
  | "*" { STAR }
  | "/" { SLASH }
  | "mod" { MODULO }
  | "<" { LTHAN }
  | ">" { GTHAN }
  | "<=" { LTHANEQ }
  | ">=" { GTHANEQ }
  | "<>" { NOTEQ }
  | "&&" { AND }
  | "||" { OR }
  | var { IDENT (Lexing.lexeme lexbuf) }
  | num { INT (int_of_string (Lexing.lexeme lexbuf)) }
  | whitespace { read lexbuf }
  | eof { EOF }
