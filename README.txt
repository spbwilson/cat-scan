Sean Wilson
s0831408

=================================
   Compile and Run Commands
=================================
compiling the program:

g++ -o assignment2 assignment2.cc -lglut -lGLU -lGL

running the program:
./assignment2 head

=================================
           Interactions
=================================
Opacity:
'v' -> Make the contours more transparent
'c' -> Make the contours more opaque
Limted from zero upwards in steps of 0.1

Contour:
'n' -> Next contour
'b' -> Previous contour
Limited between 0 and 1 in steps of 0.01


=================================
    Algorithms and Methods
=================================

COMPOSITE RAY CASTING:
The transfer function used in this program is that of composite ray casting.
Using front face culling, the current pixel accumulates colour from each sample
along the ray through the volume until the end of the volume or the opacity value reaches 1 (fully opaque). The scalar in the volume is mapped to a colour
with opacity using the transfer function below:

aout = acurrent + (1 - acurrent) * ain;
finalColour = finalColour + (1 - ain) * currentColour * acurrent * en;

Where acurrent is the current sample value scaled to a value between 0 and 1.
ain is the accumulated alpha value thus far. CurrentColour is the base colour to
be used for the current contour (e.g. the colour of bone or skin). Finally, en is the brightness.

COLOUR LUT:
It was decided that instead of using a linear interpolation for the colour 
transfer, it would look better to use a colour look-up table that differentiates
between skin and bone. As skin is not as dense as the bone, the skull can be
seen through the skin at almost every opacity level. If the opacity value is 
turned down to 0.1, the clear details of the face can be seen. As this increases
the opacity level there is less differentiation between the contours of the bone
and thus the skull looks like a solid white bit of bone at this level. With the
opacity value set to 10 or above, the thickness and definition of the skull can
clearly be seen.

=================================
Composite ray casting Psuedo Code
=================================

for each pixel {
	Initialize variables
	while(collective opacity value < 1 and ray inside bounds) {
		Get sample from volume
		Perform transfer function
		Advance position along the ray direction to the next sample
	}
	Color pixel
}

