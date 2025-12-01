type dial = {
    at: int;
    range: int;
    zeros: int;
}

type rotation =
    | Right of int
    | Left of int

let (mod) x y = ((x mod y) + y) mod y

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

let rotate2 dial rotation =
    let zeros_visited = match rotation with
        | Right amount -> amount / dial.range + if amount mod dial.range > (dial.range - dial.at) then 1 else 0
        | Left amount -> amount / dial.range + if dial.at != 0 && amount mod dial.range > dial.at then 1 else 0
    in match rotation with
        | Right amount -> { at = (dial.at + amount) mod dial.range; range = dial.range; zeros = dial.zeros + zeros_visited}
        | Left amount -> { at = (dial.at - amount) mod dial.range; range = dial.range; zeros = dial.zeros + zeros_visited }

let step2 dial rotation =
    rotate2 dial rotation
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

let solve_second data =
    let init = { at = 50; range = 100; zeros = 0 } in
    parse data
    |> List.fold_left step2 init
    |> (fun dial -> dial.zeros)
