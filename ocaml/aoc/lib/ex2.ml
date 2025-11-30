let parse data = data
    |> String.split_on_char '\n'
    |> List.filter (fun s -> s <> "")
    |> List.map (String.split_on_char ' ')
    |> List.map (List.filter (fun s -> s <> ""))
    |> List.map (List.map int_of_string)

let id x = x

let rec diffs = function
    | x :: y :: tl -> (y - x) :: diffs (y :: tl)
    | _ -> []

let is_safe = function
    | lst -> let diff = diffs lst in Lib.all (fun x -> x >= 1 && x <= 3) diff || Lib.all (fun x -> x <= -1 && x >= -3) diff

let solve_first data =
    parse data
    |> List.map is_safe
    |> List.filter id
    |> List.length

let remove_nth n lst =
    let rec aux i acc = function
        | [] -> List.rev acc
        | _ :: tl when i == n -> List.rev_append acc tl
        | x :: tl -> aux (i + 1) (x :: acc) tl
    in aux 0 [] lst

let rec range n =
    if n <= 0 then []
    else range (n - 1) @ [n-1]

let solve_second data =
    parse data
    |> List.map (fun lst -> lst
        |> List.length
        |> range
        |> List.map (fun n -> remove_nth n lst)
        |> Lib.any is_safe
    )
    |> List.filter id
    |> List.length
