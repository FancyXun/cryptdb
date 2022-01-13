rm -rf $EDBDIR/shadow/*
mysql -u root -proot -e "drop database if exists remote_db"
mysql -u root -proot -e "drop database if exists cryptdbtest"
