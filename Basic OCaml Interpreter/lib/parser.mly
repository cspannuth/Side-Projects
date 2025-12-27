%{
open Utils
%}

%token LET
%token EQUALS
%token IN
%token IF
%token THEN
%token ELSE
%token FUN
%token ARROW
%token LPAREN
%token RPAREN
%token TRUE
%token FALSE
%token PLUS
%token MINUS
%token STAR
%token SLASH
%token MODULO
%token LTHAN
%token GTHAN
%token LTHANEQ
%token GTHANEQ
%token NOTEQ
%token AND
%token OR
%token<int> INT
%token<string> IDENT

%token EOF

%right OR
%right AND
%left LTHAN, LTHANEQ, GTHAN, GTHANEQ, EQUALS, NOTEQ
%left PLUS, MINUS
%left STAR, SLASH, MODULO

%start <Utils.prog> prog

%%

prog:
  | e = expr; EOF { e }

expr:
  | IF; e1 = expr; THEN; e2 = expr; ELSE; e3 = expr { If (e1, e2, e3) }
  | LET; x = IDENT; EQUALS; e1 = expr; IN; e2 = expr { Let (x, e1, e2) }
  | FUN; x = IDENT; ARROW; e1 = expr { Fun (x, e1) }
  | e = expr2 { e }

expr2:
  | e1 = expr2; op = bop; e2 = expr2 { Bop (op, e1, e2) }
  | e1 = expr3; e2 = expr3* { List.fold_left (fun e1 e2 -> App (e1, e2)) e1 e2 }

expr3:
  | LPAREN; RPAREN; { Unit }
  | TRUE { True }
  | FALSE { False }
  | n = INT { Num n }
  | x = IDENT { Var x }
  | LPAREN; e = expr; RPAREN { e }


%inline bop:
  | PLUS { Add }
  | MINUS { Sub }
  | STAR { Mul }
  | SLASH { Div }
  | MODULO { Mod }
  | LTHAN { Lt } 
  | LTHANEQ { Lte } 
  | GTHAN { Gt }
  | GTHANEQ { Gte }
  | EQUALS { Eq }
  | NOTEQ { Neq }
  | AND { And } 
  | OR { Or }

