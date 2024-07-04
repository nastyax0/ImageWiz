# ImageWiz
This project was made out of curosity of images work.

This was never meant to be professional work honestly, a quite rough learning experience with how images work and to manipulate them.
Like in CTF-challenges (in digital forensics category), I was fascinated about how the tools are able to recognize the files correctly(most of time) or how they have guessed the metadata and led to birth of this project.

An open invitation for readers to help me understanding more complex manipulation can be done without using python modules and by mathemtical way!

Curiosity and fascination regarding of so much of math involved in traditional image processing made me implement this from ground up.

#Resources used:
Here are some mathematical formulaes i used to do some of the functions:
Conversion of a color image to grayscale is not unique. Different weighting of the color channels can effectively represent the effect of shooting black-and-white film with different-colored photographic filters on the cameras. A common strategy is to match the luminance of the grayscale image to the luminance of the color image.
To convert any color to a grayscale representation of its luminance, first one must obtain the values of its red, green, and blue (RGB) primaries in linear intensity encoding, by gamma expansion. Then, a certain percentage of the RGB values are added together. The weights depend on the exact choice of the RGB primaries, but are never fixed. Regardless of the scale employed (0.0 to 1.0, 0 to 255, 0% to 100%, etc.), the resultant number is the desired linear luminance value.
##1) GRAYSCALE CONVERSION (.https://www.rroij.com/open-access/novel-gray-scale-conversion-techniques-based-on-pixel-depth.php
aid=37541&view=mobile#:~:text=There%20are%20three%20basic%20strategies,average%20method%20and%20luminosity%20method.)
=>A. Average method 
GY = (R + G + B)/3
=>B. Lightness method
GY = (max(R, G, B) + min(R, G, B))/2

=>C. Luminosity method
GY = 0.56 G + 0.33 R + 0.11 B
or,
GY = (R * 0.2126) + (G * 0.7152) + (B * 0.0722)

##2) MAGIC NUMBER(HEADERS OF IMAGE):
https://en.wikipedia.org/wiki/List_of_file_signatures
I cross checked and put them accordingly and per suited.

##3) Historgram Equalizer:
This video helped me a lot and I made it on basis of this 


##4) COLOR MASK:
Color  masking is relatively easier and can be done by this formula easily:
 Rout = r channel data * desired r channel value;
 Gout = g channel data * desired g channel value;
 Bout = b channel data * desired b channel value;

But if we need to provide strenght we need to consider a alpha value between 0 to 1.
This is Alpha blending:
when we calculate we may loose precision thats why converting to linear value then calculating may prove beneficial:
to convert to linear:
dst = pow(channel's value / 255.0f, 2.2f);
by doing raise to 2.2 we are correcting, its gamma correction.

float coutr = srcr * alpha + dstr * (1 - alpha);
float coutg = srcg * alpha + dstg * (1 - alpha);
float coutb = srcb * alpha + dstb * (1 - alpha);

as our calculation done we would convert it back for human perception 
toSRGB(cout);
to convert it back for image:
imagedata=pow(value, 1.0f / 2.2f) * 255.0f

this result sometimes go beyond range (0.0 to 255.0) for that 
if value is greater than 255.0 then 255
if value is lesser than 0.0 then 0.0
else return imagedata;

##5) CONVOLUTION:
//In Progress

##6) INVERT:
complement of the given color = 255 - color of channel;


















