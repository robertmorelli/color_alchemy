# Color Alchemy

**Contributors:** Bobby, Catherine, Jessie, Jonas, Mitch, and Tucker

**Description:**
Color Alchemy is an educational app designed to teach the fundamentals of color mixing through interactive gameplay. Players combine the split primary pigments (from flasks) into a cauldron to create target colors. Different levels involve walkthroughs to teach the user basic principles of color mixing. An interactive pie chart and hints help players to get a better intuition for how to mix target colors.
## setup packages
   you will need to run this command for the color comparisons:
   
      git submodule update --init --recursive

## how to set up development on macos

1. install clang

         xcode-select --install
2. (optional for `4`) install the qt cli

         brew install qt
3. clone the repo (obviously)
4. install [Qt creator](https://www.qt.io/download-dev)
5. set up environment

   open the `.pro` file (in the root directory of the repo) with qt creator and you should be prompted to set up the build dependencies for macos
6. (optional) How to run the project from the command line (from the root directory of the repo)

         qmake && make && open ColorAlchemy.app
