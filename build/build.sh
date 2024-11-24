#! /bin/bash
echo "Build program"
cmake ..
make -j4
echo "Done"
