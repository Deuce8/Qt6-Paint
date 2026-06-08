# Qt6-Paint

Simple paint program written in C++ using Qt6. I wrote this to be as simple to operate as possible, and try to fulfill
the primary roles of an image editor. Some of this code is poorly optimized or written, which I will slowly refactor in
the future. The majority of this program was translated from a [PyQt6 Project](https://github.com/Deuce8/PyQt6-Paint)
which mirrors this UI.

## Setup

Before building you must install a version of Qt6. Installation instructions for your operating system can be found on
the [Qt6 website](https://doc.qt.io/qt-6/get-and-install-qt.html). You will also need to install either cmake or qmake,
and git if you want to clone the project from a terminal. These instructions assume linux, so will have to be tailored
depending on your operating system.

Afterwards you can clone and build the repository:

---

### CMake

CMake building can be run in parallel by appending the `-j` flag to the build command: `cmake --build build -j`

```bash
git clone https://github.com/Deuce8/Qt6-Paint.git
cd Qt6-Paint
cmake -B build
cmake --build build
```

---

### QMake

Ensure you're using Qt6's version of qmake by either checking with `qmake -v` or running with qmake6 like below:

```bash
git clone https://github.com/Deuce8/Qt6-Paint.git
cd Qt6-Paint
mkdir build
cd build
qmake6 ../Qt6-Paint.pro
make
```

---

To launch the program from inside the project directory execute:

```bash
./build/Qt6-Paint
```

## Notes

I haven't used qml in this project because I already had the UI implemented before learning about it. I will replace the
UI code with it when I figure out how it works.

I heavily control the layout of the UI through code and the style sheet, this is poor design and I plan on making it
more dynamic in the future.

The use of QRectF for roles that only require QRect is bad, but I also haven't figured out how to fix incorrect
coordinates and scalings when using QRect. The same applies to QPoint/QPointF.
