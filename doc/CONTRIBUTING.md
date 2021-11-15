# Contributing to this project

This document needs to be properly edited.
Meanwhile, there follow some essencial notes.

## General information

TextTron Snake is free software distributed under GNU GPL vr.3. 
For licensing and author information, please refer to files COPYING 
and AUTHORS, respectively.




## Obtaining the source and building the software - TODO- Put this somewhere else, see issue 50

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



