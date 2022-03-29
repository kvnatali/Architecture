# 1) Создадим БД: #

Положить файл createTable.sql в home

cd
sudo mysql -u root

mysql> CREATE USER 'user1'@'localhost' IDENTIFIED BY '******';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'user1'@'localhost';
mysql> \q

mysql -u user1 -p

mysql> create database study;
mysql> use study;
mysql> source createTable.sql;
mysql> select * from users;

# 2) #


