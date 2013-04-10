This project simply adds a 3-d histogram class with animated drawing
functionality to ROOT (root.cern.ch), a physics analysis software
library.

The add-on 3-d histogram class is called TH3Danim, and it's just a
TH3D histogram with additional functions and data for drawing animated
2-d histograms by sweeping through one of the 3 possible axes in time;
all of the usual 3-d histogram functions still work just fine.

AnimDraw is the primary drawing function, but before calling that, you
want to call SetDrawFrequency with the drawing frequency in Hz, and
SetShownAxes to set the 2-d axes ("yx" means y vs. x, "zy" means z
vs. y, etc.).

****** Order of function calls: ******

Create histogram -> Fill with data -> SetDrawFrequency -> SetShownAxes
-> AnimDraw.

At the moment this ordering is more or less rigid; SetShownAxes
creates the frame histograms, so if you change the histogram data
you'll need to recall this function (I know it's wrong, I know).

Again: Be aware that at the moment calling SetShownAxes will
initialize the frame histograms for drawing, so make sure that the
histogram already contains data before calling SetShownAxes.

The project is extremely simple and needs lots of work to make it more
usable, but it's already working well enough to use all of the
functionality I described above.

The file test.cc contains example code for drawing an animated
histogram for random data and can be used as a model for using the
class.
