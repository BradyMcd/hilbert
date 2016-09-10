##Hilbert Curves

[Hilbert curves](www.wikipedia.org/wiki/Hilbert_curve) are space filling and self
avoiding paths through discrete 2d planes. A single curve will touch all points in a
square exactly once so long as that square has a side length of some power of 2. The
property that you probably want to use a Hilbert curve for though is that it is roughly
locality preserving; if *d* is the distance of some point *p* along the curve then the
difference *d* - *d'* will be small if the distance between *p* and *p'* is also small.
This property is relatively coarse and so is best used in cases where error can be
tolerated or at least in cases where the comparisons you want aren't extremely exact.
Questions like "are these points clustered together" will be answered more or less
accurately while questions like "Which of these clustered points are nearest neighbors"
will probably not yield accurate answers. Further experimentation can be done using the
utilities built with `make play`. One obvious edge case exists, even points which
literally touch will have massive *d* value differences if they are split between the
first and final quadrant. This can be mostly remedied by also comparing *d* values along
a Hilbert curve which has been somehow offset, rotated or reflected along the vertical
axis and detecting which quadrant a d-value corresponds to is quite easy.

###What's here

* Calculate distance along a Hilbert curve at any given level of recursion up to 32
either iteratively or recursively
* Convert points along a rectangular plane to a stretched but square plane
* Simple possible edge case detection
* Yet another geometric struct holding literally only x and y to keep track of

###What's missing

* Fast rotation, I've taken a few cracks at the magic number for fast rotation but I
don't quite have the general case yet
* Rectangular plane support, there is a way to do it
* An interface to cleanly work with play utilities

##Play utilities

These utilities aren't particularly well documentented, in general the command name
is followed by an argument related to the level of recursion you want to look at.
Both `draw` and `draw_quads` require cairo be installed, they create png images.
`draw` accepts a second argument, the side length of the image created and
`draw_quads` accepts up to 4 arguments, the level of recursion, the number of sections
up to 16, the side length of the image and optionally an offset.