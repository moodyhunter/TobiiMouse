# TobiiMouse

Make Tobii EyeTracker your mouse.

## Dependencies
### Linux:
 - X11, XRandr development header.
 
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
