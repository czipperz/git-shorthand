#include "main.hpp"
#include <cz/str.hpp>
#include "clone.hpp"
#include "git.hpp"
#include "remote.hpp"

bool dry_run = false;
static const char* program_name;

int actual_main(int argc, char** argv) {
    program_name = argv[0];

    if (argc <= 1) {
        fprintf(stderr, "Error: No command.  See usage.\n\n");
        show_usage(stderr);
        return 1;
    }

    cz::dwim::Dwim dwim = {};
    dwim.init();
    CZ_DEFER(dwim.drop());

    int argi = 1;
    for (; argi < argc; ++argi) {
        cz::Str arg = argv[argi];
        if (arg == "-n" || arg == "--dry-run" || arg == "--nono") {
            dry_run = true;
        } else {
            break;
        }
    }

    cz::Str command = argv[argi];
    if (command == "help" || command == "--help") {
        show_usage(stdout);
        return 0;
    } else if (command == "remote") {
        return run_remote(&dwim, argc - argi, argv + argi);
    } else if (command == "clone") {
        return run_clone(&dwim, argc - argi, argv + argi);
    } else {
        return git(&dwim, {}, argc - argi, argv + argi);
    }

    return 0;
}

void show_usage(FILE* file) {
    fprintf(file,
            "git-shorthand: Shorthand Git commands.\n\
\n\
All commands forward extra arguments to Git.\n\
\n\
# Usage\n\
git-shorthand [OPTIONS] COMMAND ARGUMENTS...\n\
\n\
Options:\n\
--help | help              Show usage.\n\
-n | --dry-run | --nono    Print what would happen and don't actually perform the action.\n\
\n\
## Clone command\n\
git-shorthand clone SHORTURL GITARGUMENTS...\n\
Clone a repository.\n\
\n\
## Remote command\n\
remote -- usage: git-shorthand remote SUBCOMMAND ARGUMENTS...\n\
\n\
remote add REMOTE SHORTURL GITARGUMENTS...\n\
remote a REMOTE SHORTURL GITARGUMENTS...\n\
remote ao SHORTURL GITARGUMENTS...\n\
Add a remote to the repository.  `ao` adds `origin`.\n\
\n\
remote remove REMOTE GITARGUMENTS...\n\
remote r REMOTE GITARGUMENTS...\n\
remote ro GITARGUMENTS...\n\
Remove a remote from the repository.  `ro` removes `origin`.\n\
\n\
remote show GITARGUMENTS...\n\
remote s GITARGUMENTS...\n\
remote so GITARGUMENTS...\n\
Show the info of the repository's remotes.  `so` adds an argument `origin` to the start of GITARGUMENTS.\n\
\n\
All other commands are forwarded as written to Git.\n\
\n\
SHORTURL: either a URL or ACRONYM:RELPATH or RELPATH\n\
\n\
# Url acronym examples:\n\
~/.config/git-remotes:\n\
git@github.com/\n\
c:git@github.com:czipperz/\n\
\n\
$ git-shorthand clone czipperz/cz\n\
git clone git@github.com/czipperz/cz\n\
\n\
$ git-shorthand clone c:cz\n\
git clone git@github.com/czipperz/cz\n\
\n\
You can also use . to represent the name of the current directory:\n\
$ pwd\n\
/home/czipperz/git-shorthand\n\
$ git-shorthand clone czipperz/.\n\
git clone git@github.com/czipperz/git-shorthand\n\
$ git-shorthand clone c:.\n\
git clone git@github.com/czipperz/git-shorthand\n\
\n\
");
}
