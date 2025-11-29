let exp1 = Sexplib.Sexp.of_string "(This (is an) (s expression))"

let () = Printf.printf "%s\n" (Sexplib.Sexp.to_string exp1)
