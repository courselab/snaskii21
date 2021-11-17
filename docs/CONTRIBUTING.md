# Contributing to this project

##### Step 1 - Cloning the project
To contribute to this project, first you should clone the repository

```
git clone https://github.com/courselab/snaskii21
```

##### Step 2 - Pulling any updates in the repository
Before you make your contribution, always make sure your repository is up-to-date with the project, by typing

```
git pull
```

##### Step 3 - Creating your own branch
Before pushing your contribuition to the project, create your own branch

```
git checkout -b <YOUR_BRANCH_NAME>
```

##### Step 4 - Pushing your changes
After this is done, you're set to contribute with our project. Be aware that, if you're not an author, your contribution will be under a period of analysis by the authors and can or cannot be merged. To push your work,

```
git add .
git commit -m <SOME_USEFUL_MESSAGE>
git push <YOUR_BRANCH_NAME>
```

or

```
git add <FILES_TO_BE_PUSHED>
git commit -m <SOME_USEFUL_MESSAGE>
git push <YOUR_BRANCH_NAME>
```

This document is going under properly edition.
There follow some essencial notes.

## General information

TextTron Snake is free software distributed under GNU GPL vr.3. 
For licensing and author information, please refer to files COPYING 
and AUTHORS, respectively.




## Obtaining the source and building the software

This project's source code official repository may be found at 

https://github.com/courselab/ttsnake

If you have obtained the source from the Version Control Repository, you will
need GNU Build System (aka Autotools) to build the project, as described
in file `README`, in the root of the project tree (really, read-me). 

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

This project's development workflow is based on:

* GitFlow branching strategy [1]
* Semantiv versionng 2.0.0 release naming scheme [2]

Latest stabe release resides in branch `main`, while pre-releases (alpha, 
beta, release candidates etc.) reside in branch _prerelease_. Late fixes in
past stabe releases for maintenance, if any, are found in _legacy_ branches.

It's important that every pull request fixes at least one issue. You can pick
an issue that already exists or you can create one yourself, if you can't find one that addresses your problem. After an issue has been
selected, assign yourself so other collaborators can be aware that you are
taking care of it.

First step to make a contribution is creating a branch. The branch created must
follow the naming convention bellow, where `<issue-number>` is the number of the
issue that the pull request will solve (e.g. #42).

```
Branches names should be one of the four following options:

feature/<issue-number>
hotfix/<issue-number>
wip/<issue-number>      
coldfix/<issue-number>  
```

The meaning of each possible branch name is as follows. Choose the name that
best suits the changes you made.

<ul>
  <li><code>feature/&lt;issue-number&gt;</code> -> Implementation of a new feature, or update
  of an existing feature. This also covers updates to documentation.</li>
  <li><code>hotfix/&lt;issue-number&gt;</code> -> Fixes something that is broken in production. 
    Often it is a fix to a problem that users are complaining about.</li>
  <li><code>wip/&lt;issue-number&gt;</code> -> Work in progress, usually tagged with "DO NOT MERGE" 
    as well. This is for helping with early feedback on some development that will 
    end up being a large PR.</li>
  <li><code>coldfix/&lt;issue-number&gt;</code> -> planned fixes, which are announced beforehand.</li>
</ul>

After the changes have been made to the branch you created, feel free to open
a pull request to the proper protected branch (e.g. `develop`). After a set 
amount of reviewers with write access have accepted the changes, it
will be merged into the branch.

You are strongly encouraged to write your contribution and communicate
with the developers and contributors community in English, if possible.
Do not mind at all if your current skills are modest or incipient; no one
expects otherwise --- your coding skills and will to help matter more.
Use an automatic translator if needed. Just don't name variable and 
functions in a language other than English.

If you are a developer and an issue that you believe you are not
able to handle timely is assigned to you, please try to reassign it to someone else.

If you don't have direct write access to the repository, you may still fork the
project and send pull requests.

_Code wisdom for this project_

- Branch always (create a new feature branch for each modification)
- Commit small (add & commit single changes for better traceability)
- Merge safe (test, discuss and approve merges to permanent branches)


[1] https://nvie.com/posts/a-successful-git-branching-model/

[2] https://semver.org/



