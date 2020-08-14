# Development
**[WINDOWS INSTRUCTIONS]**
1. Open the command line (or PowerShell) and clone the repository into your desired folder with this command `git clone --recurse-submodules -j8 https://github.com/L4ZZA/city-uni-engine.git`
2. Install [GitKraken](https://www.gitkraken.com) (recommended)
3. Open the cloned repository from the file explorer.
4. Run the batch script `generate_solution.bat`
5. Open and build the whole solution (make sure it successfully does so).
6. Read the section below and start developing.

## Workflow
Before starting with the fun, some precautions:

- Always keep your local repository up to date with the remote one, by pulling the new changes as soon as they're available (on GitKraken it will show as an arrow down next to the branch name on the left).
- Whatever you are going to do, never work directly on develop or on master branch. These to branches should be populated through merges (pull-requests).
- Create your branch from wherever you need to (most likely develop) but before that just make sure your local develop is up-to-date with the remote one (a.k.a origin)
- Before merging the branch create a pull request. Once the request has been discussed and approved then the creator of the branch can merge the branch to its parent (usually develop) and delete the branch.

## Comments
Anything that is not clear from the above instructions or from the code, create a descriptive issue on bitbucket and I'll try to make it clearer by resolving it.
