# How to contribute ?

Thank you for taking the time to contribute! This document will guide you
through the workflow and best practices you need to know to send your
contribution.

There are many ways to contribute :

* [Reporting a bug](#reporting-bugs)
* [Making a feature request](#feature-requests-and-discussions)
* [Contributing code (C++, Python, CMake, etc.)](#code-contribution)

## Reporting bugs

If you have found a bug, you can first search the existing issues
to see if it has already been reported.

If it's a new bug, please open a new issue on GitLab.
The 'Bug' issue template will help you provide all important information and
help fixing the bug quicker. Remember to add as much information as possible!

## Feature requests and discussions

Feature requests are welcome! Generally you are welcome to simply open an issue
and discuss your idea there. For more complex requests there is an issue
template for in depth description called 'Request for Comments'.

## Code contribution

Clone the repository, create a feature branch, commit your changes, push the
feature branch to a fork (or the main repository if you are a core developer),
then send a merge request.

Note that we also accept PRs which we will manually merge.

### Commit message

On your feature branch, write a good [commit message](https://xkcd.com/1296/):
short and descriptive. If fixing an issue or bug, put the issue number in the
commit message so that GitLab can [cross-link it](https://docs.gitlab.com/ce/user/project/issues/crosslinking_issues.html).
You can prefix your commit message with an indicating flag (DOC, BUG, PKG,
TEST, SuperBuild, etc.).

Standard prefixes for commit messages:

    BUG: Fix for runtime crash or incorrect result
    COMP: Compiler error or warning fix
    DOC: Documentation change
    ENH: New functionality
    PERF: Performance improvement
    STYLE: No logic impact (indentation, comments)
    WIP: Work In Progress not ready for merge

For example, here are some good commit messages:

    BUG: #1701 Warn users if parameter string is unset
    DOC: Fix typo in Monteverdi French translation
    COMP: Allow GeoTIFF and TIFF to be disabled when no 3rd party drags them

### Merge request

Your contribution is ready to be added to the main repository? Send a Merge
Request against the `develop` branch on GitLab using the merge request
template. The merge request will then be discussed by the community and the core team.

* Merge requests can not be merged until all discussions have been resolved (this is enforced by GitLab)
* Merge requests **must receive at least 2 positives votes from core developers** (members of Main Repositories group in Gitlab with at least "Developer" level; this includes PSC members) before being merged
* The merger is responsible for checking that the branch is up-to-date with develop
* Merge requests can be merged by anyone (not just PSC or RM) with push access to develop
* Merge requests can be merged once the CI pipeline passes successfully. See
  next section for details on the CI pipelines.

### Contribution license agreement

This project requires that contributors sign out a [Contributor License
Agreement](https://en.wikipedia.org/wiki/Contributor_License_Agreement). The
purpose of this CLA is to ensure that the project has the necessary ownership or
grants of rights over all contributions to allow them to distribute under the
chosen license (Apache License Version 2.0)

The copyright owner (or owner's agent) must be mentioned in headers of all
modified source files and also added to the NOTICE
file.

## GitLab guidelines

In order to organize the issues in our GitLab instance, we use both labels and
milestones.

The milestones should be used to track in which release a feature is merged.
GitLab can then provide a summary of all features and bugs added to a given release
version.

Regarding labels, we use the following set:
* ~story: significant feature to be implemented with a detailed work plan, it can
  correspond to a Request for Comments that has turned into a development action
* ~bug: Bug, crash or unexpected behavior, reported by a user or a developer
* ~feature: Feature request expressed by a user/developer
* ~patch: A small patch fixing build warnings, compilation errors, typos in logs or documentation
* ~"To Do": action is planned
* ~Doing: work in progress
* ~api ~app ~documentation ~monteverdi ~packaging ~qgis: optional context information

## Versioning

We use [semantic versioning](https://semver.org/). See the website for the full spec, in summary:

> Given a version number MAJOR.MINOR.PATCH, increment the:
>
>  1. MAJOR version when you make incompatible API changes,
>  2. MINOR version when you add functionality in a backwards-compatible manner, and
>  3. PATCH version when you make backwards-compatible bug fixes.
>
> Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.

The develop branch is always the place where the upcoming major or minor release is worked on. Patch releases are done on release branches, for example 1.0.1 and 1.0.2 could be found on the release-1.0 branch.

For the purpose of defining backwards compatibility, the API covers (not an exhaustive list): the C++ API, the Python bindings, application names, application parameters, output format and interpretation of input data.

When we deprecate part of our public API, we should do two things: (1) update our documentation to let users know about the change, (2) issue a new minor or major release with the deprecation in place.