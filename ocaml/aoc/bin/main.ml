let exercise_number = ref 0
let is_second_stage = ref false
let spec_list = [
    ("-e", Arg.Set_int exercise_number, "exercise number");
    ("-s", Arg.Set is_second_stage, "should solve second stage");
]
let anon_fun _ = ()
let usage_msg = "echo input | aoc -e 1"

let () =
    Arg.parse spec_list anon_fun usage_msg;

    let stage = if !is_second_stage then 2 else 1 in
    let data = Aoc.Lib.read_all () in
    let result_f = match (!exercise_number, !is_second_stage) with
    | (1, false) -> Result.Ok Aoc.Ex1.solve_first
    | (_, _) -> Result.Error "unknown exercise"
    in match result_f with
    | Ok f -> Printf.printf "exercise %d, stage %d - result = %d" !exercise_number stage (f data)
    | Error err -> Printf.printf "an error occurred: %s" err