# qt-project
A simple Qt-Python hybrid experiment


## INDEX
 * [Objective](#objective)
 * [Requirements](#requirements)
 * [Setup, Installation and Running](#setup-installation-and-running)
 * [Architecture](#architecture)
 * [Caveats and known issues](#caveats-and-known-issues)
 * [Future work](#futurework)

## Objective

To create a Qt GUI application that interacts with a python kernel to detect faces in a given image. The image may be loaded to the GUI or be taken via the camera of the user. 


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


## Architecture

So the objective is to build a hybrid application that relies on a C++ GUI and a python kernel. The kernel being an app that does all the heavy lifting on the data. In this case, it uses pyOpenCV to detect faces on an image loaded via the GUI. The GUI follows the old server-frontend architecture, relying on HTTP Requests. The reason for this decision is described further into this section.


There are multiple ways you can have your Qt application built in C++ interact with a python kernel. They are [documented here](http://doc.qt.io/qt-5/ipc.html). But to summarize: 

* Shared Memory
* DBus
* QProcess
* Sockets
* Session management

However there are issues with most of them. [Shared Memory only works with within Qt Applications](https://stackoverflow.com/questions/5134412/parent-process-cant-access-shared-memory-in-pyqt). You can share memory between Qt and non Qt applications. (It can work within PyQt applications but not between the two langugages)

[Linux DBus isn't available on Mac OS natively](https://github.com/zbentley/dbus-osx-examples/tree/master/installation). Setup requires installation via Homebrew and setup with Launchd.

QProcess works but it isn't a way to communicate large amounts of data, it's a way to spawn and control processes from within your Qt Application

[Session management are for sharing events across multiple processes on X11/Linux, may not work on Windows and Mac OS](http://doc.qt.io/qt-5/session.html).

Sockets: We're left with LocalSockets, TCP/IP Sockets, SSL Sockets. 

LocalSockets imply a standard unix filesystem socket can be used to communicate. [But there is a limitation of data you can read from it](https://stackoverflow.com/questions/14831819/reading-more-than-2048-bytes-from-qlocalsocket). And since sockets seem to be the only well/officially documented IPC system, it seems prudent to use it. So, to make it platform independent, TCP/IP sockets seem to be the best bet.

Other options like accessing QObject via PythonQt are available. However these are open source projects and don't always support Qt's latest releases. The last release [PythonQt supported was `Qt5.8`](https://sourceforge.net/p/pythonqt/news/2017/06/pythonqt-32-released/). This application is built using `Qt 5.10`

Other options like boost::python are available but that is a steep learning curve.

The biggest issue in using HTTP TCP/IP was data conversion. Since all of the scientific data structures are written in `numpy`, we have to make sure that we're sharing only primitives over the medium. So to restrict it, I've enforced `content-type` to `application/json` which seems to be well supported by Qt and of course Python.

`qtExamples` directory has some of my attempts to get test other IPC strategies.

<strong>To summarize:</strong> The GUI sends HTTP POST/GET requests (with the image location on local file system) to the python server which then picks the image directly from the file system, operates on it and responds with coordinates where the features are located.

It also generates an RGB histogram (to showcase charting capabilities) and responds with plot points which are then presented on a line chart in the GUI.

The Python kernel itself uses Haar-0cascades to detect facial features. A lot more cascade classifier training files are available but that requires additional GUI changes. [Here is a good resource on how these classifiers are trained](https://memememememememe.me/post/training-haar-cascades/). 

<strong>Note: This application doesn't actually train the classifier but uses training data from 6k+ images already available to us via opencv</strong>

The histogram is generated by simple histogram calculator available under the opencv package. You can also use numpy for generating histograms but I chose to go with OpenCV.

## Caveats and known issues

This application was built in a very short time, the following issues may persist: 

* Requests may take too long for multiple feature detection. It is after all, running a weak server.
* Classifier isn't too trained. Using bright images may not entirely work. It relies on frontal face features.
* Camera detection. Ideally the proper way to implement a camera is to check for cameras, allow the user to select it from a list and enforce resolution
* No tests (too little time)
* C++ source is cramped. Proper encapsulation & abstraction of features is much desired. 

Qt is a very robust but a very tough enterprise framework. It has a steep learning curve. Python bindings for PyQt and PySide are great to create a prototype but in the end C++ is definitely the way to go when using it. Other Frameworks like TKinter, Kivy are poorly documented (comparatively) and are therefore not reliable. Qt is the way to go for enterprise software if you can get the hang of it.
