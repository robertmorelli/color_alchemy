/Users/robertmorelli/Qt/6.9.0/wasm_multithread/bin/qmake ../ColorAlchemy.pro
make
python3 make_standalone.py
rm *.o
rm *.cpp
rm ColorAlchemy.html