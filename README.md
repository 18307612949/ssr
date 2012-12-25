SimpleScreenRecorder
====================

A screen recorder for Linux. Despite the name, this program is actually quite complex. It's 'simple' in the sense that it's easier to use than ffmpeg/avconv or VLC :).

License
-------

GNU GPL v3 - read 'COPYING' for more info.

Build dependencies
------------------

This list may be incomplete. All instructions and package names are for Ubuntu 12.10, it could be different for other versions/distros.
- Qt 4 (package qt4-qmake, libqt4-dev)
- libavformat (package libavformat-dev)
- libavcodec (package libavcodec-dev)
- libavdevice (package libavdevice-dev)
- libavutil (package libavutil-dev)
- libswscale (package libswscale-dev)
- libGL (32/64) (package libgl1-mesa-dev)
- libX11 (32/64) (package libx11-dev)
- libXext (package libxext-dev)
- libXfixes (package libxfixes-dev)
- 32-bit libraries (package g++-multilib, ia32-libs)

Everything combined:

    sudo apt-get install qt4-qmake libqt4-dev libavformat-dev libavcodec-dev libavdevice-dev libavutil-dev libswscale-dev libgl1-mesa-dev libx11-dev libxext-dev libxfixes-dev g++-multilib ia32-libs

If the 32-bit version of libGL and libX11 isn't found during linking, but 64-bit works fine, try this:

    cd /usr/lib/i386-linux-gnu/
    sudo ln -s libGL.so.1.2.0 mesa/libGL.so    (replace 1.2.0 with the highest version you have)
    sudo ln -s mesa/libGL.so libGL.so
    sudo ln -s libX11.so.6 libX11.so

I don't know whether this is the right way to do it, but it works for me.

Compiling
---------

First compile GLInject. Go to the glinject directory and run both 'compile32' and 'compile64'. If compile32 fails but compile64 works, read the instructions above again :).

You can compile the main program by running 'qmake' followed by 'make'. Alternatively you can use Qt Creator (just open the project file).

Other files you may want to read
--------------------------------

notes.txt contains notes about the design of the program. It's a good idea read this first if you want to make changes to the code.

todo.txt contains a list of things that I still need to do. This file changes a lot of course.
