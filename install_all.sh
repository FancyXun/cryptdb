test haha
rm .vimrc
ln -s conf/dot.vimrc .vimrc

apt-get install gawk liblua5.1-0-dev libntl-dev         \
                libmysqlclient-dev libssl-dev libbsd-dev        \
                libevent-dev libglib2.0-dev libgmp-dev          \
                mysql-server libaio-dev automake                \
                gtk-doc-tools flex cmake libncurses5-dev        \
                bison g++ make wget -y



rm -rf proxy-src
tar zxf bins/proxy-src.tar.gz
cd proxy-src
./autogen.sh
./configure --enable-maintainer-mode --with-lua=lua5.1 --prefix="/cryptdb/bins/proxy-bin"
make
make install
cd ..

apt remove bison libbison-dev
dpkg -i deb/libbison-dev_2.7.1.dfsg-1_amd64.deb
dpkg -i deb/bison_2.7.1.dfsg-1_amd64.deb
rm -rf mysql-src 
tar zxf bins/mysql-src.tar.gz
cd mysql-src
mkdir build
cd build
cmake -DWITH_EMBEDDED_SERVER=on -DENABLE_DTRACE=off ..
make
cd ../..


make clean
make
service mysql stop
make install
service mysql start
rm -rf shadow
mkdir shadow
echo "You must do: export EDBDIR=/full/path/to/cryptdb/ before running cryptdb; we recommend putting it into your .bashrc"


