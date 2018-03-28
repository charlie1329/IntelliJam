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

The libraries I have used within this project (and therefore give credit to) are:

* Eigen (Matrix Operations): Found at <https://eigen.tuxfamily.org>
* libsndfile (Dealing with audio files): Found at <http://www.mega-nerd.com/libsndfile/>
* PortAudio (Real-time audio library): Found at <http://www.portaudio.com/>
* Catch (Unit testing library): Found at <https://github.com/catchorg/Catch2>
* Boost (Threading among other things): Found at <http://www.boost.org/>
* Qt (Graphical User Interface): Found at <https://www.qt.io/>
* Keras (LSTM training): Found at <https://keras.io/>
* Scipy (Fourier Transform in Python): Found at <https://www.scipy.org/>
* Numpy: Found at <http://www.numpy.org/>
* Matplotlib: Found at <https://matplotlib.org/>
* RobustICA Package: Found at <http://www.i3s.unice.fr/~zarzoso/robustica.html>, written by: Vicente Zarzoso
* Matlab STFT/ISTFT implementation: Found at <https://uk.mathworks.com/matlabcentral/fileexchange/45577-inverse-short-time-fourier-transformation--istft--with-matlab-implementation>, written by: Hristo Zhivomirov  
* Windows MIDI API: Found at: <https://msdn.microsoft.com/en-us/library/windows/desktop/dd798495(v=vs.85).aspx>


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
