# Лабораторная работа 3

Данные должны кешироваться в Apache Ignite/Redis;

## Консольные команды

### Запуск докер контейнера

{code}
cd ~/git/Architecture/lab_03/docker/

docker-compose up --remove-orphans
или
docker-compose up
{code}

Результат
{code}
ignite-node-1    | [13:02:26] Ignite node started OK (id=eff81624)
ignite-node-1    | [13:02:26] Topology snapshot [ver=2, locNode=eff81624, servers=2, clients=0, state=ACTIVE, CPUs=8, offheap=3.8GB, heap=2.0GB]
ignite-node-1    | [13:02:26]   ^-- Baseline [id=0, size=2, online=2, offline=0]
{code}

### Сборка сервера

{code}
cd ~/git/Architecture/lab_03/
cmake .
/usr/bin/cmake --build ~/git/Architecture/lab_03 --config Debug --target lab_03 -j 6 --
{code}

### запуск сервера

{code}
cd ~/git/Architecture/lab_03/
./start.sh
{code}

### тестирование в несколько потоков с кешем и без

wrk -c 10 -d 60 -t 10 "http://127.0.0.1:8080/person?login=EGordeeva"

wrk -c 10 -d 60 -t 10 "http://127.0.0.1:8080/person?login=PBarinov&no_cache"

