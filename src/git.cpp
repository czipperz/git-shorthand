#include "git.hpp"

#include <cz/process.hpp>
#include "main.hpp"

int git(cz::dwim::Dwim* dwim,
        cz::Slice<cz::Str> replaced_arguments,
        int passthrough_argc,
        char** passthrough_argv) {
    cz::Vector<cz::Str> args = {};
    args.reserve(dwim->buffer_array.allocator(), 1 + replaced_arguments.len + passthrough_argc);
    args.push("git");
    args.append(replaced_arguments);
    for (size_t i = 0; i < passthrough_argc; ++i)
        args.push(passthrough_argv[i]);

    if (dry_run) {
        cz::String script = {};
        cz::Process::escape_args(args, &script, dwim->buffer_array.allocator());
        puts(script.buffer());
        return 0;
    }

    cz::Process_Options options;
    options.std_in = cz::std_in_file();
    options.std_out = cz::std_out_file();
    options.std_err = cz::std_err_file();

    cz::Process process;
    if (!process.launch_program(args, options)) {
        fprintf(stderr, "Failed to run git\n");
        return 1;
    }

    return process.join();
}
