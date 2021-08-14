#include "clone.hpp"

#include <cz/str.hpp>
#include "expand_url.hpp"
#include "git.hpp"
#include "main.hpp"

int run_clone(cz::dwim::Dwim* dwim, int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: No url to clone.  See usage.\n\n");
        show_usage(stderr);
        return 1;
    }

    cz::Str url = expand_url(dwim, argv[1]);

    cz::Str args[] = {"clone", url};
    return git(dwim, args, argc - 2, argv + 2);
}
