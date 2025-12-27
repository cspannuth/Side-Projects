include Utils

let parse (s : string) : expr option =
  match Parser.prog Lexer.read (Lexing.from_string s) with
  | e -> Some e
  | exception _ -> None

let expr_of_value v =
        match v with
        | VNum n -> Num n
        | VFun (x, e) -> Fun (x, e)
        | VUnit -> Unit
        | VBool b -> if b then True else False

let rec subst (v : value) (x : string) (e : expr) : expr =
  match e with
  | Unit -> e
  | True -> e
  | False -> e
  | Num n -> Num n
  | Var y -> if x = y then expr_of_value v else Var y
  | App (e1, e2) -> App (subst v x e1, subst v x e2)
  | Bop (op, e1, e2) -> Bop (op, subst v x e1, subst v x e2)
  | If (e1, e2, e3) -> If (subst v x e1, subst v x e2, subst v x e3)
  | Let (y, e1, e2 ) -> if y = x then Let (y, subst v x e1, e2) else Let (y, subst v x e1, subst v x e2)
  | Fun (y, e) -> if x = y then Fun (y, e) else Fun (y, subst v x e)

let rec eval (e : expr) : (value, error) result =
  match e with
  | Unit -> Ok (VUnit)
  | True -> Ok (VBool true)
  | False -> Ok (VBool false)
  | Num n -> Ok (VNum n)
  | Var x -> Error (UnknownVar x)
  | App (e1, e2) -> (
          match eval e1 with
          | Ok (VFun (x, e)) -> (
                  match eval e2 with
                  | Ok v2 -> eval (subst v2 x e)
                  | _ -> Error (InvalidApp)
          )
          | _ -> Error (InvalidApp)
  )
  
  | Bop (op, e1, e2) -> ( 
                  match eval e1 with
                  | Ok e3 -> (
                          match eval e2 with
                          | Ok e4 -> (
                                 match (op, e3, e4) with
                                 | (Add, VNum num1, VNum num2) -> Ok (VNum (num1 + num2))
                                 | (Sub, VNum num1, VNum num2) -> Ok (VNum (num1 - num2))
                                 | (Mul, VNum num1, VNum num2) -> Ok (VNum (num1 * num2))
                                 | (Div, VNum _, VNum 0) -> Error (DivByZero)
                                 | (Div, VNum num1, VNum num2) -> Ok (VNum (num1 / num2))
                                 | (Mod, VNum _, VNum 0) -> Error (DivByZero)
                                 | (Mod, VNum num1, VNum num2) -> Ok (VNum (num1 mod num2))
                                 | (Lt, VNum num1, VNum num2) -> Ok (VBool (num1 < num2))
                                 | (Lte, VNum num1, VNum num2) -> Ok (VBool (num1 <= num2))
                                 | (Gt, VNum num1, VNum num2) -> Ok (VBool (num1 > num2))
                                 | (Gte, VNum num1, VNum num2) -> Ok (VBool (num1 >= num2))
                                 | (Eq, VNum num1, VNum num2) -> Ok (VBool (num1 = num2))
                                 | (Neq, VNum num1, VNum num2) -> Ok (VBool (num1 <> num2))
                                 | (And, VBool true, b) -> (
                                         match b with
                                         | VBool b1 -> Ok (VBool b1)
                                         | _ -> Error (InvalidArgs And)
                                 )
                                 | (And, VBool false, _) -> Ok (VBool false)
                                 | (Or, VBool false, b) -> (
                                         match b with
                                         | VBool b1 -> Ok (VBool b1)
                                         | _ -> Error (InvalidArgs Or)
                                 )
                                 | (Or, VBool true, _) -> Ok (VBool true)
                                 | (op, _, _) -> Error (InvalidArgs op)
                          )
                          | Error err -> Error err
                  )
                  | Error err -> Error err
  )

  | If (e1, e2, e3) -> (
          match eval e1 with
          | Ok (VBool true) -> eval e2
          | Ok (VBool false) -> eval e3
          | _ -> Error (InvalidIfCond)
  )
  | Let (x, e1, e2) -> (
          match eval e1 with
          | Ok v -> let e = subst v x e2 in eval e
          | _ -> Error (InvalidApp)
  )
  | Fun (x, e) -> Ok (VFun (x, e))

let rec fv e = 
        match e with
        | Num _ -> []
        | True -> []
        | False -> []
        | Unit -> []
        | Bop (_, e1, e2) -> fv e1 @ fv e2
        | If (e1, e2, e3) -> fv e1 @ fv e2 @ fv e3
        | Let (x, e1, e2) -> fv e1 @ List.filter ((<>) x) (fv e2)
        | Var x -> [x]
        | Fun (x, e) -> List.filter ((<>) x) (fv e)
        | App (e1, e2) -> fv e1 @ fv e2

let is_well_scoped e = List.is_empty (fv e)

let interp (p : string) : (value, error) result =
        match parse p with
        | Some e -> if is_well_scoped e then eval e else (
                match fv e with
                | [] -> Error (ParseFail)
                | x :: _ -> Error (UnknownVar x)
                )
        | None -> Error (ParseFail)
