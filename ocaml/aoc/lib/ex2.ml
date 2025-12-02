type id_range = {
    first: int;
    last: int;
}

let rec range x y =
    if x > y then []
    else x :: range (x + 1) y

let id_range_to_id_list r = range r.first r.last

let divisors n =
    let rec aux i acc =
        if i > n then List.rev acc
        else if n mod i = 0 then aux (i + 1) (i :: acc)
        else aux (i + 1) acc
    in
    aux 1 []

let rec any f = function
    | [] -> false
    | x :: ls -> f x || any f ls

let is_repeated n s =
    let substr = String.sub s 0 n in
    let m = String.length s in
    m != n
    && m mod n = 0
    && String.concat "" (Array.to_list (Array.make (m / n) substr)) = s

let is_invalid s =
    divisors (String.length s)
    |> List.map (fun d -> is_repeated d s)
    |> any (fun x -> x)

let sum_invalid lst = lst
    |> List.map string_of_int
    |> List.filter (is_repeated 2)
    |> List.map int_of_string
    |> List.fold_left (+) 0

let sum_invalid2 lst = lst
    |> List.map string_of_int
    |> List.filter is_invalid
    |> List.map int_of_string
    |> List.fold_left (+) 0

let list_to_id_range = function
    | [x; y] ->
        begin match (int_of_string_opt x, int_of_string_opt y) with
        | (Some xx, Some yy) -> Some { first = xx; last = yy }
        | _ -> None
        end
    | _ -> None

let solve_first data = data
    |> String.split_on_char ','
    |> List.map (fun s -> s
        |> String.split_on_char '-'
        |> list_to_id_range
    )
    |> List.filter_map (fun x -> x)
    |> List.map id_range_to_id_list
    |> List.map sum_invalid
    |> List.fold_left (+) 0

let solve_second data = data
    |> String.split_on_char ','
    |> List.map (fun s -> s
        |> String.split_on_char '-'
        |> list_to_id_range
    )
    |> List.filter_map (fun x -> x)
    |> List.map id_range_to_id_list
    |> List.map sum_invalid2
    |> List.fold_left (+) 0
