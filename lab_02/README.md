# Лабораторная работа 2

Использование proxy sql для шардинга данных в несколько инстансов MySQL https://proxysql.com/

## Подготовка базы данных

create table if not exists `users` (
      `id` INT AUTO_INCREMENT
    , `login` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci not null
    , `first_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL
    , `last_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL
    , `age` varchar(255) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL
    , `created_at` TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    , `CONSTRAINT` pk_users PRIMARY KEY (id)
    , `CONSTRAINT` c_login UNIQUE (login)
);

## Запросы

Пример готовых запросов сохранен в коллекцию postman `Arch.postman_collection.json`

### Добавление person

http://localhost:8080/person?add&login=nivanova&first_name=Natalia&last_name=Ivanova&age=25

### Поиск person по логину

http://localhost:8080/person?login=логин

### Поиск person по имени и фамилии

http://localhost:8080/person?first_name=маска&last_name=маска


## Консольные команды

cmake .
/usr/bin/cmake --build /home/natali/git/Architecture/lab_02/build --config Debug --target lab_02 -j 6 --

sudo ./build/lab_02 --host=127.0.0.1  --port=6033 --login=test --password=pzjqUkMnc7vfNHET --database=sql_test --init_db
