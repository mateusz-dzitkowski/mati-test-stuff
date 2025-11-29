let parse data = data
    |> String.split_on_char '\n'
    |> List.filter (fun s -> s <> "")
    |> List.map (String.split_on_char ' ')
    |> List.map (List.filter (fun s -> s <> ""))
    |> List.map (List.map int_of_string)
    |> Lib.unzip2

let find_or_zero tbl x =
    match Hashtbl.find_opt tbl x with
    | Some c -> c
    | None -> 0

let list_to_counter lst =
    let tbl = Hashtbl.create 1000 in
    List.iter (
        fun x -> Hashtbl.replace tbl x (1 + find_or_zero tbl x)
    ) lst;
    tbl

let solve_first data =
    parse data
    |> (fun (left, right) -> (List.sort compare left, List.sort compare right))
    |> (fun (left, right) -> List.combine left right)
    |> List.map (fun (x, y) -> Int.abs (x - y))
    |> List.fold_left ( + ) 0

let solve_second data =
    parse data
    |> (
        fun (left, right) -> List.map (fun x -> x * (find_or_zero (list_to_counter right ) x )) left
    )
    |> List.fold_left ( + ) 0
