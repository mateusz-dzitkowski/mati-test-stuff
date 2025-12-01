type dial = {
    at: int;
    range: int;
    zeros: int;
}

type rotation =
    | Right of int
    | Left of int

let rotate dial rotation =
    match rotation with
    | Right amount -> { at = (dial.at + amount) mod dial.range; range = dial.range; zeros = dial.zeros }
    | Left amount -> { at = (dial.at - amount) mod dial.range; range = dial.range; zeros = dial.zeros }

let note_zero dial =
    match dial with
    | { at = 0; range = range; zeros = zeros } -> { at = 0; range = range; zeros = zeros + 1 }
    | { at = at; range = range; zeros = zeros } -> { at = at; range = range; zeros = zeros }

let step dial rotation =
    rotate dial rotation
    |> note_zero

let split_string s = (String.get s 0, String.sub s 1 (String.length s - 1))

let tuple_to_rotation (direction, amount) = match direction with
    | 'R' -> Right (int_of_string amount)
    | _ -> Left (int_of_string amount)

let parse data = data
    |> String.split_on_char '\n'
    |> List.filter (fun s -> s <> "")
    |> List.map (fun s -> s
        |> split_string
        |> tuple_to_rotation
    )

let solve_first data =
    let init = { at = 50; range = 100; zeros = 0 } in
    parse data
    |> List.fold_left step init
    |> (fun dial -> dial.zeros)
