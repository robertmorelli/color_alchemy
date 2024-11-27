# Color Alchemy

**Contributors:** Bobby, Catherine, Jessie, Jonas, Mitch, and Tucker

**Description:**
Color Alchemy is an educational app designed to teach the fundamentals of color mixing through interactive gameplay. Players combine the split primary pigments (from flasks) into a cauldron to create target colors. An interactive pie chart and hints help players to get a better intuition for how to mix target colors.

## how to set up development on macos

1. install dependencies (you need [Homebrew](https://brew.sh/))

   `xcode-select --install`

   `brew install qt`
2. clone the repo (obviously)
3. install [Qt creator](https://www.qt.io/download-dev)
4. set up environment

   open the `.pro` file (in the root directory of the repo) with qt creator and you should be prompted to set up the build dependencies for macos
5. (optional) How to run the project from the command line (from the root directory of the repo)

   `qmake && make && open ColorAlchemy.app`
