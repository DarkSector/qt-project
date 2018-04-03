# qt-project
A simple Qt-Python hybrid experiment

## Requirements
 
 * [Compiler](#compiler)
 * [Qt5](#qt5)
 * [Python2.7 using Anaconda](#anaconda)
 
 
### Compiler

We are working on Mac OS X, so we need to install Xcode command line tools. If the following steps don't work, download them directly from [Apple Developer's website](https://developer.apple.com/download/more/). 
 *  Install Xcode
 *  Install command line tools using the terminal `$ xcode-select --install `
 *  Test it by going to terminal and invoking `$ gcc --version`. Should come back with version and cxx prefix

### Qt5

* Start by installing the Qt5 SDK from (qt.io)[https://www.qt.io/download]. I decided to go with the Open Source version
* Currently, since this is an experiment, I decided to install Qt 5.10.1, 5.8.0, 5.7 since PythonQt bindings are available for the latter two. 
* Qt Creator is a sort of an IDE with internal build tools. So currently, it's a good idea to install this.
* We will not be using CMake since QMake comes bundled with the creator


### Anaconda
* We want to work with OpenCV 2.3.14 on python2.7, best way to do that is install Anaconda for IPython. 
* Grab the latest version [here](https://www.anaconda.com/download/#macos)
* Download Python3 using conda (alternative to pip) and create a virtual environment so that code can be isolated.
* This will create an environemnt called qt using python2.7 `$ conda create -n qt python=2.7 anaconda`
* We can check what environment we're in by using `conda env list`, the active env is marked by a *
* Switch to qt using `source activate qt`
* For more info on the commands: [Here's a cheat sheet](https://conda.io/docs/_downloads/conda-cheatsheet.pdf)
