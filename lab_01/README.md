# 1) Создадим БД: #

Положить файл `createTable.sql` в `home`
```bash
cd
sudo mysql -u root
```
```sql
mysql> CREATE USER 'user1'@'localhost' IDENTIFIED BY '******';
mysql> GRANT ALL PRIVILEGES ON *.* TO 'user1'@'localhost';
mysql> \q
```
```bash
mysql -u user1 -p
```
```sql
mysql> create database study;
mysql> use study;
mysql> source createTable.sql;
mysql> select * from users;
```

# 2) #


