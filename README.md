# cryptdb

```shell script
gdb --args bins/proxy-bin/bin/mysql-proxy  --plugins=proxy --event-threads=4 --max-open-files=1024 --proxy-address=127.0.0.1:3307 --proxy-backend-addresses=127.0.0.1:3306 --proxy-lua-script=$EDBDIR/mysqlproxy/wrapper.lua
```

```shell script
(gdb) b main/rewrite_main.cc:1400
(gdb) r
```

```shell script
mysql -u root -pletmein -h 127.0.0.1 -P 3307
```

```shell script
create database test;
use test;
create table t(sno varchar(20));
insert into t(sno) values("1111");
select * from t1 where sno like "%";
```
