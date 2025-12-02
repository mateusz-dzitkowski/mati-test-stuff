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
