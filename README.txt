IMBS Background Subtraction Library
Copyright 2012 Domenico Daniele Bloisi

This file is part of IMBS and it is distributed under the terms of the
GNU Lesser General Public License (Lesser GPL)


IMBS is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

IMBS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with IMBS.  If not, see <http://www.gnu.org/licenses/>.

IMBS has been written by Domenico Daniele Bloisi
Please, report suggestions/comments/bugs to
domenico.bloisi@gmail.com

Contributors:
Andrea Pennisi
Fabio Previtali



IMBS Background Subtraction Library
-----------------------------------

Introduction
------------ 

This file README.txt contains information about
IMBS algorithm described in
D. D. Bloisi and L. Iocchi
"Independent Multimodal Background Subtraction"
In Proc. of the Third Int. Conf. on Computational Modeling of Objects
Presented in Images: Fundamentals, Methods and Applications, pp. 39-44, 2012.
Please, cite the above paper if you use IMBS.

IMBS software is provided without any warranty about its usability. 
It is for educational purposes and should be regarded as such.

Written by Domenico D. Bloisi

Please, report suggestions/comments/bugs to
domenico.bloisi@gmail.com


Build IMBS
-----------
IMBS is provided with a CMakeLists.txt file and can be compiled
by using cmake (http://www.cmake.org/).

On windows
1) unzip the zip file
2) use the CMake graphical user interface to create the desired
   makefile

On Linux
1) unzip the zip file in <imbs folder>
2) $cd <imbs folder>
3) $mkdir build
4) $cd build
5) $cmake ..
6) $make


Run IMBS
---------
IMBS is provided with an usage example (main.cpp)

On windows

For video files
>imbs -vid video1.avi

For an image sequence (fps = 25 default value)
>imbs -img images/1.png
or you can specify the fps value
>imbs -img images/1.png -fps 7

