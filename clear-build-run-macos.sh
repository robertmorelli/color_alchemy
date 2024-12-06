clear &&\
"$(find ~/qt -type f -name qmake -perm +111 ! -path '*QtDesignStudio*' | sort -V | tail -n 1)" "$PWD" &&\
make &&\
open ColorAlchemy.app
