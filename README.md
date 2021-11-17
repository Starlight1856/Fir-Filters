General Purpose FIR filters
---------------------------

This upload contains general purpose FIR filters,
implemented in both C and C++. The filters rely
both on circular buffer arrays and STL vectors.

Also included are two filter design applications
in Python 3.8 + Qt5 user interface. One for
High Pass/Low Pass, the other for BandPass/BandStop.
They include a basic selection of windowing options.

While not cpu optimised they operate at a useful speed.

Tested with a 201 tap filter on 10 year old desktop;

C code filter 4.5 Million samps/sec.
C++ code filter using arrays 4.4 Million samps/sec.
C++ code filter using STL vectors 2.0 Million samps/sec.


Folder Contents
---------------

filtercode : source code for filters.
firdesign : Python scripts and Qt5 ui for FIR design.
testing : Code used for testing filters. This also includes
          make files, and filter coefficient files.
doc : Further information in firfilter2.pdf.


Paul Byrne 9/11/21
