# qt-project
A simple Qt-Python hybrid experiment


## INDEX
 * [Objective](#objective)
 * [Requirements](#Requirements)
 * [Setup, Installation and Running](#installation)
 * [Architecture](#architecture)
 * [Caveats and known issues](#caveats)
 * [Future work](#futurework)


## Requirements
 
 * Mac OS X (Built using 10.11.6)
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


## Setup, Installation and Running

<strong>NOTE: This is a brief description of the development environment. Skip to the Running subsection to see this project in action</strong>

### Development setup

There are two parts to this project. One is the GUI built using Qt5/C++ and the other is the python part that does all the scientific stuff. I will go over them in detail

#### Qt5/C++

The project already includes a built binary for Mac OS X and can be found in release folder in the main branch. However if you would like to build it from source, the source can be found under `qtExamples > QtFaceRecognition`. The build process relies on QMake that comes bundles with QtCreator. Simply import the .pro file and it will setup the project for you.

#### Python

The <strong>python development setup</strong> depends upon the following packages:

* django
* djangorestframework
* opencv==2.4.13
* numpy
* matplotlib

The Python package includes a Django server running django-restframework without authentication. Since this was to be a proof of concept, it basically runs without any auth or session management. The source for it is located under `python > qtAPIServer`. 

The server needs to run on `0.0.0.0:9000` using the default django command `python manage.py runserver 0.0.0.0:9000`

### Running

* Activate your Conda environment: `$ source qt activate`
* Run the provided script `$ sh run.sh`

The script is designed to install the included python package in the conda environment and then open the GUI. It will run the server on `0.0.0.0:9000` and the GUI will interact with it according to design. 

If the above script fails to work, you can manually install the python package by executing `conda install --use-local opencv-server` and once done, you can start the server by `python runkernel.py`

Then you can open the GUI like any other app on OS X.

