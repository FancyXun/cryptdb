make clean
make
service mysql stop
make install
service mysql start
rm -rf shadow
mkdir shadow
