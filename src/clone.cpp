#include "clone.hpp"

#include <cz/str.hpp>
#include "expand_url.hpp"
#include "git.hpp"
#include "main.hpp"

int run_clone(cz::dwim::Dwim* dwim, int argc, char** argv) {
    int argi = 1;
    for (; argi < argc; ++argi) {
        cz::Str arg = argv[argi];
        if (arg == "-n" || arg == "--dry-run" || arg == "--nono") {
            dry_run = true;
        } else if (arg == "--help") {
            show_usage(stdout);
            return 0;
        } else {
            break;
        }
    }

    if (argi >= argc) {
        fprintf(stderr, "Error: No url to clone.  See usage.\n\n");
        show_usage(stderr);
        return 1;
    }

    cz::Str url = expand_url(dwim, argv[argi++]);

    cz::Str args[] = {"clone", url};
    return git(dwim, args, argc - 2, argv + 2);
}
