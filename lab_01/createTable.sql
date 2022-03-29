create table if not exists users (
id INT AUTO_INCREMENT,
login varchar(255) not null,
first_name varchar(255),
last_name varchar(255),
age int,
created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
CONSTRAINT pk_users PRIMARY KEY (id),
CONSTRAINT c_login UNIQUE (login)
);

INSERT INTO users (login, first_name, last_name, age) 
VALUES ('KAgeev', 'Константин', 'Агеев', 30);

INSERT INTO users (login, first_name, last_name, age) 
VALUES ('PBarinov', 'Павел', 'Баринов', 30);

INSERT INTO users (login, first_name, last_name, age) 
VALUES ('EGordeeva', 'Елизавета', 'Гордеева', 30);