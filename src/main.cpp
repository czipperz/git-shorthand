#include <stdio.h>
#include <cz/str.hpp>

static void show_usage(FILE* file, const char* arg0);
static int run_remote(int argc, char** argv) {return 0;}
static int run_clone(int argc, char** argv) {return 0;}

int actual_main(int argc, char** argv) {
    if (argc <= 1) {
        show_usage(stdout, argv[0]);
        return 0;
    }

    cz::Str command = argv[1];
    if (command == "help" || command == "--help") {
        show_usage(stdout, argv[0]);
        return 0;
    } else if (command == "remote") {
        return run_remote(argc, argv);
    } else if (command == "clone") {
        return run_clone(argc, argv);
    } else {
        fprintf(stderr, "Error: Invalid command.  See usage.\n\n");
        show_usage(stderr, argv[0]);
        return 1;
    }

    return 0;
}

static void show_usage(FILE* file, const char* arg0) {
    fprintf(file, "git-shorthand: Shorthand Git commands.\n\
\n\
All commands forward extra arguments to Git.\n\
\n\
# Usage\n\
git-shorthand COMMAND ARGUMENTS...\n\
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
");
}
