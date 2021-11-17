# Contributing to this project

There are always positive contributions to be done in this project. If you wish to contribute, we as a team would appreciate it. Here are the guidelines we would like you to follow:
- [General information](#general-information)
- [Got any questions?](#questions)
- [Found a bug?](#found-bugs)
- [Missing a Feature?](#missing-features)
- [Branch name convention](#branch-name)
- [Submission Guidelines](#submission-guidelines)
- [Commit messages](#commit-messages)
- [Building the source code](#building)
- [Communication Language](#language)
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

As a team, we work to fix all the issues as soon as possible, but before we do that, we need to be able to reproduce and confirm it. In order to reproduce bugs, we require that you provide a minimal reproduction. Having that enables us to go straight to the right problem, and do not require a lot of chatting to understand where the bug is.

## <a name="mising-features"></a> Missing a Feature?
You can request a new feature by [submitting an issue](#submission-guidelines) to our GitHub repository. If you would like to implmement a new feature, please consider the size of the change in order to determine the right steps to proceed:
- For a **Major Feature**, first open an issue and outline your proposal sothat it can be discussed. This process allowss us to better coordinate our efforts, prevent duplication of work, ad help you to craft so that it is successflly accepted into the project.
- **Small Features** can be crafter and directly [submitted as Pull Request](#submission-guidelines).

## <a name="branch-name"></a> Branch name convention

As a convention inside the repository, we use the following as the way to name git branches:
```
feature/#issue-number
```
It make it easier to know the issue number associated with the pull request.

Also, this project's development workflow is based on the [GitFlow branching strategy](https://nvie.com/posts/a-successful-git-branching-model/) that is very well encouraged by the community.

## <a name="submission-guidelines"></a>Submission Guidelines

### Submitting an Issue
Before you submit an issue, please search the issue tracker. An issue for your problem might already exist and the discussion might inform you of workarounds readily available.

You can file new issues directly on the [Github repository issues page](https://github.com/courselab/snaskii21/issues/new).

### Submitting a Pull Request (PR)

Before submitting your Pull Request, consider the following steps:

1. Search on the [pull request tracker](https://github.com/courselab/snaskii21/pulls) for open or closes pull requests related to the same issue.
2. Be sure that an issue describes the problem you are solving. Discussing ideas or implementations inside the issue page might help to ensure that the implementations has fewer flaws.
3. Fork the [courselab/snaskii21](https://github.com/courselab/snaskii21) repository.
4. Inside your forked repository, [create a new git branch](branch-name) to start working on your changes:
    ```
    git checkout -b feature/#<issue-number> develop
    ```
5. Create your patch.
6. Commit your changes using a descriptive commit message that should follow our [commit message stardard](#commit-messages):
    ```
    git commit -am "<your-commit-message>"
    ```
7. Push your changes to the remote Github repository:
    ```
    git push origin <new-branch-name>
    ```
8. Inside the Github page, visit the [pull requests page](https://github.com/courselab/snaskii21/pulls) and select `develop` as destination branch.

## <a name="commit-messages"></a> Commit messages
We have simple rule over how our Git commit messages must be formatted. This format leads to easier to read commit history later on.

Each commit message should be written in the following format:

```
<tag>: <short-summary>
  │       │
  │       └─> Summary in present tense. Not capitalized. No period at the end.
  │
  └─> Commit tag: bug|critical|doc|dup|easy|feature|help|invalid|other|src|wontfix
```

The tag is are the same options used as labels on issues. It makes it intuitive to construct the commit message and descriptive for the ones that are reviewing the pull request later.

## <a name="building"></a>Building the source code

In order to build and run the repository, give a look to the [README.md file](https://github.com/courselab/snaskii21/blob/develop/README.md), that covers all the dependencies and build procedures required.

## <a name="language"></a> Communication Language

You are strongly encouraged to write your contribution and communicate
with the developers and contributors community in English, if possible.
Do not mind at all if your current skills are modest or incipient; no one
expects otherwise --- your coding skills and willing to help matter more.
Use a automatic translator if needed. Just don't name variable and 
functions in a language other than English.

## <a name="versions"></a> Versions

Our versioning naming scheme is based on [Semantic Versioning](http://semver.org/).


_Code wisdom for this project_

- Branch always (create a new feature branch for each modification)
- Commit small (add & commit single changes for better traceability)
- Merge safe (test, discuss and approve merges to permanent branches)
