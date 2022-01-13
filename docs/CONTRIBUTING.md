# Contributing to this project

There are always positive contributions to be done in this project. If you wish to contribute, we as a team would appreciate it. Here are the guidelines we would like you to follow:
- [General information](#general-information)
- [Got any questions?](#questions)
- [Found a bug?](#found-bugs)
- [Missing a Feature?](#missing-features)
- [Branch name](#branch-name)
- [Submission Guidelines](#submission-guidelines)
- [Commit messages](#commit-messages)
- [Building the source code](#building)
- [Communication Language](#language)
- [Code Styles](#code-styles)
- [Versions](#versions)

## <a name="general-information"></a> General information

TextTron Snake is free software distributed under GNU GPL vr.3.
In order to check the license specification give a look to the [license file](https://github.com/courselab/snaskii21/blob/develop/COPYING).
Moreover, for checking the authors of the project, you can visit the [authors file](https://github.com/courselab/snaskii21/blob/develop/AUTHORS.md).


## <a name="questions"></a> Got any questions?

Please do not open new issues on the GitHub repository in order to make questions about the project.
Instead, please feel free to send messages directly to the contributors of the project.

How should you make questions? Be direct and clear. Every body has limited time, making questions that goes straight makes it easier to answer.

## <a name="found-bug"></a> Found a bug?
If you find a bug in the source code, you can help us by [submitting an issue](#submission-guidelines) to our [Github Repository](https://github.com/courselab/snaskii21). Even better, you can submit a Pull Request with a fix.

As a team, we work to fix all the issues as soon as possible, but before we do that, we need to be able to reproduce and confirm it. In order to reproduce bugs, we require that you provide a description on how to reproduce it. That enables us to go straight to the problem, and does not require wasting time exchanging information.

## <a name="mising-features"></a> Missing a Feature?
You can request a new feature by [submitting an issue](#submission-guidelines) to our GitHub repository. If you would like to implmement a new feature, please consider the size of the change in order to determine the right steps to proceed:
- For a **Major Feature**, first open an issue and outline your proposal sothat it can be discussed. This process allowss us to better coordinate our efforts, prevent duplication of work, ad help you to craft so that it is successflly accepted into the project.
- **Small Features** can be crafter and directly [submitted as Pull Request](#submission-guidelines).

## <a name="branch-name"></a> Branch name convention

First step to make a contribution is creating a branch. The branch created must
follow the naming convention bellow, where `<issue-number>` is the number of the
issue that the pull request will solve (e.g. #42).

Branches names should be one of the four following options:

```
feature/<issue-number>

hotfix/<issue-number>

wip/<issue-number>

coldfix/<issue-number>
```

The meaning of each possible branch name is as follows. Choose the name that
best suits the changes you made.

- `feature/<issue-number>` : Implementation of a new feature, or update
of an existing feature. This also covers updates to documentation.
- `hotfix/<issue-number>` : Fixes something that is broken in production. 
Often it is a fix to a problem that users are complaining about.
- `wip/<issue-number>` : Work in progress, usually tagged with "DO NOT MERGE" 
as well. This is for helping with early feedback on some development that will 
end up being a large PR.
- `coldfix/<issue-number>` : Planned fixes, which are announced beforehand.

Also, this project's development workflow is based on the [GitFlow branching strategy](https://nvie.com/posts/a-successful-git-branching-model/) that is very well encouraged by the community.

## <a name="submission-guidelines"></a>Submission Guidelines

### Submitting an Issue

Before you submit an issue, please search the issue tracker. An issue for your problem might already exist and the discussion might inform you of workarounds readily available.

You can file new issues directly on the [Github repository issues page](https://github.com/courselab/snaskii21/issues/new).

### Submitting a Pull Request (PR)

It's important that every pull request fixes at least one issue. You can pick
an issue that already exists or you can create one yourself, if you can't find one that addresses your problem. After an issue has been
selected, assign yourself so other collaborators can be aware that you are
taking care of it.

Before submitting your Pull Request, consider the following steps:

1. Search on the [pull request tracker](https://github.com/courselab/snaskii21/pulls) for open or closes pull requests related to the same issue.
2. Be sure that an issue describes the problem you are solving. Discussing ideas or implementations inside the issue page might help to ensure that the implementations has fewer flaws.
3. Fork the [courselab/snaskii21](https://github.com/courselab/snaskii21) repository.
4. Inside your forked repository, [create a new git branch](branch-name) to start working on your changes:
    ```
    $ git checkout -b <new-branch-name> develop
    ```
5. Create your patch.
6. Commit your changes using a descriptive commit message that should follow our [commit message stardard](#commit-messages):
    ```
    $ git add
    $ git commit -am "<your-commit-message>"
    ```
7. Push your changes to the remote Github repository:
    ```
    $  git push origin <new-branch-name>
    ```

8. Inside the Github page, visit the [pull requests page](https://github.com/courselab/snaskii21/pulls) and select the proper protected branch (e.g. `develop`) as destination branch. 
After a set amount of reviewers with write access have accepted the changes, it will be merged into the respective branch.

9. Always keep your branch up to date with the destination branch, so that you do not run into code conflicts.

    If you have forked the repository, you should read the github docs page on [Configuring a remote for a fork](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/configuring-a-remote-for-a-fork). That is a way to configure a remote that points to the upstream repository in Git and always be aware of the changes inside courselab.

    In summary, you must specify a new remote *upstream* repository to be synced with the fork:

    ```
    $ git remote add upstream https://github.com/courselab/snaskii21
    ```

    And then you can use the `git pull` command to update your fork with the changes from the upstream repository:

    ```
    $ git pull upstream develop
    ```
10. Use our [pull request template](./PULL_REQUEST_TEMPLATE.md) to make your pull request more descriptive. 

## <a name="commit-messages"></a> Commit messages

We have simple rule over how our Git commit messages must be formatted. This format leads to easier to read commit history later on.

Each commit message should be written in the following format:

```
<tag>: <short-summary>
  │          │
  │          └─> Summary in present tense. Not capitalized. No period at the end.
  │
  └─> Commit tag: bug|hotfix|doc|dup|easy|feature|help|invalid|other|src|wontfix
```

The tag is the same option used as labels on issues. It makes it intuitive to construct the commit message and descriptive for anyone that is reviewing your pull request later.

## <a name="building"></a>Building the source code

This project's source code official repository may be found at 

https://github.com/courselab/ttsnake

If you have obtained the source from the Version Control Repository, you will
need GNU Build System (aka Autotools) to build the project, as described
in [file README](https://github.com/courselab/snaskii21/blob/develop/README.md), in the root of the project tree (really, read-me). 

## <a name="language"></a> Communication Language

You are strongly encouraged to write your contribution and communicate
with the developers and contributors community in English, if possible.
Do not mind at all if your current skills are modest or incipient; no one
expects otherwise --- your coding skills and will to help matter more.
Use an automatic translator if needed. Just don't name variable and 
functions in a language other than English.

## <a name="code-styles"></a> Code Styles

- Variables/functions formatting: snake_case
- Structs formatting: snake_case_t
- Indentation: Tabs
- Blocks: Same-line curly brackets separated by spaces
- If-like statements: Spaces before parenthesis
- Pointer asterisks: 'int *a'
- Declare multiple variable in same line: Yes
- One-liners: Break to multiple lines nevertheless
- Line's max-width: 80


## <a name="versions"></a> Versions

Our versioning naming scheme is based on [Semantic Versioning](http://semver.org/).


_Code wisdom for this project_

- Branch always (create a new feature branch for each modification)
- Commit small (add & commit single changes for better traceability)
- Merge safe (test, discuss and approve merges to permanent branches)
