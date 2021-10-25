# Contributing to this project

This document needs to be properly edited.
Meanwhile, there follow some essencial notes.

## General information

TextTron Snake is free software distributed under GNU GPL vr.3. 
For licensing and author information, please refer to files COPYING 
and AUTHORS, respectively.




## Obtaining the source and building the software

This project's  source code official repository may be found at 

https://github.com/courselab/ttsnake

If you have obtain the source from the Version Control Repository, you will
need GNU Build System (aka Autotools) to build the project, as described
in file `README`, in the root of the project tree (really, reade-me). 

Briefly, bootstrap the build system with

```
$ ./autogen.sh
```

to create the build configuration script and then build the software with

```
$ ./configure 
$ make
```

After the software is built, the binary may be found in the subdirectiry `src`.

The file 

```
src/ttsnake
```

is actually a wrapper script wich invokes the real binary 

```
src/ttsnake.bin
```

with appropirate arguments to read data files from the local subdirectory 'scenes'.

In order to install the software,

```
$ make install
```
This command will copy the file `src/ttsnake.bin` to the system binary path, 
usually `usr/bin` and invoke a install hook to rename it `ttsnake`. The
installed binary will read data from the system data path, usually `usr/share`.

To install the software in a different location, use 

```
$ ./configure --prefix=<some-path>
$ make
```

More build and install options may be consulted in

```
$ ./configure --help
```
and in the file `INSTALL`, found in the root of the project tree.

## General guidelines

This project's development workflow is based 

* GitFlow branching strategy [1]
* Semantiv versionng 2.0.0 release naming scheme [2]

Latest stabe release resides in branch `main`, while pre-releases (alpha, 
beta, release candidates etc.) reside in branch _prerelease_. Late fixes in
past stabe releases for maintenance, if any, are found in _legacy_ branhces.

Contribution admission policy requires developers to proposed modifications
as feature branches and submitted them as pull requests, which in turn
need to be approved by other developers before being merged into the propper 
branches.

You are strongly encouraged to write your contribution and communicate
with the developers and contributors community in English, if possible.
Do not mind at all if your current skills are modest or incipient; no one
expects otherwise --- your coding skills and willing to help matter more.
Use a automatic translator if needed. Just don't name variable and 
functions in a lnauguage other than English.

If you are a developer and is assinged an issue, and you believe you are not
able to handle timely, please, try to reasign it to someone else.

If you don't have direct write access to the repository, you may still fork the
project and send pull requests.

_Code wisdom for this project_

- Branch always (create a new feature branch for each modification)
- Commit small (add & commit single changes for better traceability)
- Merge safe (test, discuss and approve merges to permanent branches)


[1] https://nvie.com/posts/a-successful-git-branching-model/

[2] https://semver.org/



