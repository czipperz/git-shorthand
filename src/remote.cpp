#include "remote.hpp"

#include <stdlib.h>
#include "expand_url.hpp"
#include "git.hpp"
#include "main.hpp"

static cz::Str get_remote(int argc, char** argv, int* argi);
static cz::Str get_url(cz::dwim::Dwim* dwim, int argc, char** argv, int* argi);

int run_remote(cz::dwim::Dwim* dwim, int argc, char** argv) {
    int argi = 1;
    for (; argi < argc; ++argi) {
        cz::Str arg = argv[argi];
        if (arg == "-n" || arg == "--dry-run" || arg == "--nono") {
            dry_run = true;
        } else {
            break;
        }
    }

    if (argi >= argc) {
        fprintf(stderr, "Error: No remote subcommand.  See usage.\n\n");
        show_usage(stderr);
        return 1;
    }

    cz::Str subcommand = argv[argi++];

    if (subcommand == "add" || subcommand == "a" || subcommand == "ao") {
        cz::Str remote = get_remote(argc, argv, &argi);
        cz::Str url = get_url(dwim, argc, argv, &argi);
        cz::Str args[] = {"remote", "add", remote, url};
        return git(dwim, args, argc - argi, argv + argi);
    } else if (subcommand == "remove" || subcommand == "r" || subcommand == "ro") {
        cz::Str remote = get_remote(argc, argv, &argi);
        cz::Str args[] = {"remote", "remove", remote};
        return git(dwim, args, argc - argi, argv + argi);
    } else if (subcommand == "show" || subcommand == "s") {
        cz::Str args[] = {"remote", "show"};
        return git(dwim, args, argc - argi, argv + argi);
    } else if (subcommand == "so") {
        cz::Str args[] = {"remote", "show", "origin"};
        return git(dwim, args, argc - argi, argv + argi);
    } else {
        return git(dwim, {}, argc, argv);
    }
}

static cz::Str get_remote(int argc, char** argv, int* argi) {
    if (cz::Str(argv[*argi - 1]).ends_with('o')) {
        return "origin";
    } else {
        if (*argi >= argc) {
            fprintf(stderr, "Error: No remote specified.  See usage.\n\n");
            show_usage(stderr);
            exit(1);
        }
        return argv[(*argi)++];
    }
}

static cz::Str get_url(cz::dwim::Dwim* dwim, int argc, char** argv, int* argi) {
    if (*argi >= argc) {
        fprintf(stderr, "Error: No remote url specified.  See usage.\n\n");
        show_usage(stderr);
        exit(1);
    }

    return expand_url(dwim, argv[(*argi)++]);
}
