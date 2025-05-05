/Users/robertmorelli/Qt/6.9.0/wasm_singleThread/bin/qmake ../ColorAlchemy.pro 
make clean && make
python3 make_standalone.py
rm *.o
rm *.cpp
rm ColorAlchemy.html