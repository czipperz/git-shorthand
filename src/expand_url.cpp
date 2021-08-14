#include "expand_url.hpp"

#include <stdlib.h>
#include <cz/dwim/process.hpp>
#include <cz/env.hpp>
#include <cz/file.hpp>
#include <cz/format.hpp>
#include <cz/path.hpp>
#include <cz/string.hpp>
#include <cz/working_directory.hpp>

static bool lookup_acronym(cz::dwim::Dwim* dwim, cz::Str acronym, cz::Str* expanded);
static bool lookup_default(cz::dwim::Dwim* dwim, cz::Str* expanded);
static cz::Str expand_relpath(cz::dwim::Dwim* dwim, cz::Str relpath);

cz::Str expand_url(cz::dwim::Dwim* dwim, cz::Str shorturl) {
    if (shorturl.contains("://"))
        return shorturl;

    cz::Str expanded;
    cz::Str relpath;

    const char* colon = shorturl.find(':');
    if (colon) {
        cz::Str acronym = shorturl.slice_end(colon);
        relpath = shorturl.slice_start(colon + 1);

        if (!lookup_acronym(dwim, acronym, &expanded))
            return shorturl;
    } else {
        relpath = shorturl;

        if (!lookup_default(dwim, &expanded))
            return shorturl;
    }

    relpath = expand_relpath(dwim, relpath);

    return cz::format(dwim->buffer_array.allocator(), expanded, relpath);
}

static bool lookup_acronym(cz::dwim::Dwim* dwim, cz::Str acronym, cz::Str* expanded) {
    cz::String home = {};
    if (!cz::env::get_home(dwim->buffer_array.allocator(), &home)) {
        fprintf(stderr, "Error: Couldn't find user home directory\n");
        return false;
    }

    cz::String path = cz::format(dwim->buffer_array.allocator(), home, "/.git-remotes");

    cz::String contents = {};
    if (!cz::dwim::read_file(dwim, path.buffer(), &contents)) {
        fprintf(stderr, "Error: Couldn't read %s\n", path.buffer());
        return false;
    }

    const char* pos = contents.find(cz::format(dwim->buffer_array.allocator(), '\n', acronym, ':'));
    if (!pos)
        return false;

    const char* start = pos + 1 + acronym.len + 1;
    size_t end = contents.slice_start(start).find_index('\n');
    *expanded = contents.slice(start, start + end);
    return true;
}

static bool lookup_default(cz::dwim::Dwim* dwim, cz::Str* expanded) {
    cz::String home = {};
    if (!cz::env::get_home(dwim->buffer_array.allocator(), &home)) {
        fprintf(stderr, "Error: Couldn't find user home directory\n");
        return false;
    }

    cz::String path = cz::format(dwim->buffer_array.allocator(), home, "/.git-remotes");

    cz::String contents = {};
    if (!cz::dwim::read_file(dwim, path.buffer(), &contents)) {
        fprintf(stderr, "Error: Couldn't read %s\n", path.buffer());
        return false;
    }

    size_t newline = contents.find_index('\n');
    if (newline == 0)
        return false;

    *expanded = contents.slice_end(newline).clone(dwim->buffer_array.allocator());
    return true;
}

/// Expand `.` to the name of the current directory.
static cz::Str expand_relpath(cz::dwim::Dwim* dwim, cz::Str relpath) {
    if (relpath != "." && !relpath.contains("/.") && !relpath.contains("\\.")) {
        return relpath;
    }

    cz::String path = {};
    if (cz::get_working_directory(dwim->buffer_array.allocator(), &path).is_err())
        return relpath;

    while (path.ends_with('/'))
        path.pop();

    cz::Str name;
    if (!cz::path::name_component(path, &name))
        return relpath;

    if (name.len == 0)
        return relpath;

    if (relpath == ".")
        return name;

    return cz::format(dwim->buffer_array.allocator(), relpath.slice_end(relpath.len - 1), name);
}
