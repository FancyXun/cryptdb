apt install ruby -y

rm .vimrc
ln -s conf/dot.vimrc .vimrc

apt-get install gawk liblua5.1-0-dev libntl-dev         \
                libmysqlclient-dev libssl-dev libbsd-dev        \
                libevent-dev libglib2.0-dev libgmp-dev          \
                mysql-server libaio-dev automake                \
                gtk-doc-tools flex cmake libncurses5-dev        \
                bison g++ make wget -y


apt remove bison libbison-dev -y
dpkg -i deb/libbison-dev_2.7.1.dfsg-1_amd64.deb
dpkg -i deb/bison_2.7.1.dfsg-1_amd64.deb

