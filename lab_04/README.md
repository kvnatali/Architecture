# Лабораторная работа 4

## Цель

Получение практических навыков в реализации асинхронной записи в СУБД.

## Задание

Разработать приложение на языке C++ осуществляющее хранение сущности Person и
предоставляющее REST API по добавлению и получению сущности Person.

Должны выполняться следующие условия:
Данные должны храниться в СУБД MySQL;
Сущность Person должна содержать следующие поля:
* login – уникальное поле
* first_name
* last_name
* age

Реализовать API
    HTTP GET http://localhost:8080/person?login=логин возвращать JSON объект с полями login,
first_name,last_name,age
    HTTP POST http://localhost:8080/person с параметрами login, first_name,last_name,age –
    должен добавлять данные в очередь (на базе RabbitMQ или Kafka)

Реализовать приложение, которое осуществляет чтение данных из очереди и сохранение их
в СУБД MySQL.


### Запуск докер контейнера

````
cd ~/git/Architecture/lab_04/docker/

docker-compose up --remove-orphans
или
docker-compose up
````

### Сборка сервера

````
cd ~/git/Architecture/lab_04/
cmake .
/usr/bin/cmake --build ~/git/Architecture/lab_04/build --config Debug --target lab_04 -j 6 --
````

### запуск сервера

````
cd ~/git/Architecture/lab_04/
./start.sh
./start_writer.sh
````