# TobiiMouse [![HitCount](http://hits.dwyl.io/lhy0403/TobiiMouse.svg)](http://hits.dwyl.io/lhy0403/TobiiMouse)

Make tobii EyeTracker act like a mouse.


Travis CI: [![Build Status](https://travis-ci.com/lhy0403/TobiiMouse.svg?branch=master)](https://travis-ci.com/lhy0403/TobiiMouse)

AppVeyor: [![Build status](https://ci.appveyor.com/api/projects/status/06yq9v21s6b619nh/branch/master?svg=true)](https://ci.appveyor.com/project/lhy0403/tobiimouse/branch/master)

## Dependencies
### Linux:
 - X11 and XRandr development header.
 
### Windows: 
 - Coming soon.
 
## Build
### For Linux:
```bash
# Install some packages:

# For Debian and Ubuntu: 
#sudo apt-get install qt5-default qt5-qmake libx11-dev libxrandr-dev

# For Arch Linux:
sudo pacman -S   # TODO

# Clone the repository.
git clone https://github.com/lhy0403/TobiiMouse
cd ./TobiiMouse/src/
mkdir build && cd build

# QMake and make
qmake ../
make

# Done, if everything goes well, you should have successfully built it.
```
### For Windows:
```cmd
REM Coming soon!
```
