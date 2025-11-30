let read_all () =
    let buf = Buffer.create 4096 in
    (
        try
        	while true do
        		Buffer.add_channel buf stdin 4096
        	done
        with End_of_file -> ()
    );
    Buffer.contents buf

let unzip2 = function
    | [] -> ([], [])
    | lst ->
        List.fold_right (function
            | [a; b] -> fun (xs, ys) -> (a :: xs, b :: ys)
            | _ -> failwith "bruh"
        )
        lst
        ([], [])

let rec all f = function
    | x :: tl -> (f x) && (all f tl)
    | _ -> true

let rec any f = function
    | x :: tl -> (f x) || (any f tl)
    | _ -> false