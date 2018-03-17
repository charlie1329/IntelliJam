IntelliJam: An Intelligent Agent for Musical Improvisation
===================================================

The code in this repository is for the main application
being built for my final year project.

Ownership
---------

If I have not explicitly authored myself in some source file (namely .h/.c/.cpp/.py/.m files), 
DO NOT assume I am the author. This repository also contains libraries I may have used, which
I am clearly not the author of.


ICA Folder
----------
During the first semester of the project, I had a separate repository, which was
used for work on blind source separation for training data generation. This repository 
has been copied across here. THE SAME RULES CONCERNING OWNERSHIP APPLY IN THIS FOLDER TOO,
i.e. my name being listed as author on top of the file.

Libraries Used
--------------

This list may be incomplete, but some of the libraries I have used are:

* Eigen (Matrix Operations)
* libsndfile (Dealing with audio files)
* PortAudio (Real-time audio library)
* Catch (Unit testing library)
* Boost (Threading among other things)
* Qt (Graphical User Interface)

PortAudio
---------

In the `include/port_audio` directory, you will see a large number of header files, as well as two .c files.
The header files are just so I can use functions within portAudio. It is unlikely I need all of these, but
adding these to my project means I needn't worry about it.

With regards to the .c files, these are for functionality that I require from portAudio that is not by default
presented in the library (i.e. the .lib/.dll). Therefore I need to compile it from source. The functionality I require
here is some certain utility functions, as well as the use of a ring buffer, which is necessary for processing
in multiple threads. I have a single reader/single writer problem where I can't use mutexes within the callback function
for fear of priority inversion. Therefore, the best solution is to use the portAudio ring buffer for this, which is what
it has been designed for.
