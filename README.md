# git-shorthand

Shorthand Git commands.

There are two things going on here:
1. Heavy usage of acronyms in subcommands.
2. Shorthand urls.
   Urls are sourced from the `~/.git-remotes` configuration file.

Note: I recommend aliasing `gr` to `git-shorthand remote` and `gcl` to `git-shorthand clone`.

This repository is licensed under GPL3.  If you wish to
purchase a different license, email czipperz AT gmail DOT com.

## Building

1. Clone the repository and the submodules.

```
git clone https://github.com/czipperz/git-shorthand
cd git-shorthand
git submodule init
git submodule update
```

2. Build git-shorthand by running (on all platforms):

```
./build-release
```

3. After building, git-shorthand can be ran via `./build/release/git-shorthand`.
