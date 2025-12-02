type id_range = {
    first: int;
    last: int;
}

let rec range x y =
    if x > y then []
    else x :: range (x + 1) y

let id_range_to_id_list r = range r.first r.last

let is_invalid s =
    String.length s mod 2 == 0
    && String.sub s 0 (String.length s / 2) = String.sub s (String.length s / 2) (String.length s / 2)

let sum_invalid lst = lst
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