#pragma once

#include <cz/dwim/dwim.hpp>
#include <cz/slice.hpp>
#include <cz/str.hpp>

int git(cz::dwim::Dwim* dwim,
        cz::Slice<cz::Str> replaced_arguments,
        int passthrough_argc,
        char** passthrough_argv);
