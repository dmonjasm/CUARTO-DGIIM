/*Borrado y creación de 0 de todas las tablas*/
DROP TABLE PARTICIPAN;
DROP TABLE INGRESAN;
DROP TABLE GASTAN;
DROP TABLE ORGANIZAN;
DROP TABLE ATIENDEN;
DROP TABLE ASIENTOS_CONTABLES;
DROP TABLE CENTROS;
DROP TABLE CLIENTES;
DROP TABLE EMPLEADOS;
DROP TABLE ACTIVIDADES;

/*Creación empleados*/
CREATE TABLE EMPLEADOS(
                        Codigo_Empleados INT PRIMARY KEY, 
                        DNI_Empleados VARCHAR(9), 
                        Nombre_Empleados VARCHAR(100), 
                        Apellidos_Empleados VARCHAR(100), 
                        Fecha_de_Nacimiento_Empleados DATE,
                        Direccion_Empleados VARCHAR(100),
                        Telefono_Empleados INT,
                        Email_Empleados VARCHAR(100),
                        IBAN_Empleados VARCHAR(20),
                        Tipo_Empleados VARCHAR(100)
);

/*Creación de clientes para dependencias*/
CREATE TABLE CLIENTES(
                        Codigo_Clientes INT PRIMARY KEY, 
                        DNI_Clientes VARCHAR(9), 
                        Nombre_Clientes VARCHAR(100), 
                        Apellidos_Clientes VARCHAR(100), 
                        Fecha_de_Nacimiento_Clientes DATE,
                        Direccion_Clientes VARCHAR(100),
                        Telefono_Clientes INT,
                        Email_Clientes VARCHAR(100),
                        IBAN_Clientes VARCHAR(20),
                        Reserva VARCHAR(100)); 


/*Creación centros*/
CREATE TABLE CENTROS(
                         Codigo_Centros INT PRIMARY KEY, 
                         Nombre_Centros VARCHAR(100), 
                         Localizacion_Centros VARCHAR(100), 
                         Horario_Centros TIMESTAMP
);

/*Creación asientos_contables*/
CREATE TABLE ASIENTOS_CONTABLES
(
                         Codigo_Asientos_Contables INT PRIMARY KEY,
                         Cuenta INT,
                         Fecha_Asientos_Contables DATE, 
                         Centro_Asientos_Contables VARCHAR(40),
                         Area VARCHAR(50),
                         Codigo_Deudor_Acreedor INT,
                         Concepto VARCHAR(100), 
                         Debe FLOAT,
                         Haber FLOAT,
                         Saldo FLOAT,
                         Divisa VARCHAR (3)
);

/*Creación tabla ingresan*/
CREATE TABLE INGRESAN
(
                         Codigo_Asientos_Contables INT,
                         Codigo_Clientes INT,
                         FOREIGN KEY (Codigo_Asientos_Contables) REFERENCES ASIENTOS_CONTABLES(Codigo_Asientos_Contables),
                         FOREIGN KEY (Codigo_Clientes) REFERENCES CLIENTES(Codigo_Clientes)
);

/*Creación tabla gastan*/
CREATE TABLE GASTAN
(
                         Codigo_Asientos_Contables INT,
                         Codigo_Empleados INT,
                         FOREIGN KEY (Codigo_Asientos_Contables) REFERENCES ASIENTOS_CONTABLES(Codigo_Asientos_Contables),
                         FOREIGN KEY (Codigo_Empleados) REFERENCES EMPLEADOS(Codigo_Empleados)
);

/*Creacion de talba actividades*/
CREATE TABLE ACTIVIDADES(Fecha_Actividades DATE,
                         Hora_Actividades TIMESTAMP, 
                         Sala_Actividades INT, 
                         Centro_Actividades VARCHAR(40), 
                         Tipo_Actividades VARCHAR(30), 
                         Aforo INT, 
                         PRIMARY KEY(Fecha_Actividades, Hora_Actividades, Sala_Actividades, Centro_Actividades));
              
/*Creacion de tabla participan*/
CREATE TABLE PARTICIPAN(Fecha_Actividades DATE, 
                        Hora_Actividades TIMESTAMP, 
                        Sala_Actividades INT, 
                        Centro_Actividades VARCHAR(40), 
                        Codigo_Clientes INT, 
                        PRIMARY KEY(Fecha_Actividades, Hora_Actividades, Centro_Actividades, Codigo_Clientes), 
                        FOREIGN KEY(Fecha_Actividades, Hora_Actividades, Sala_Actividades, Centro_Actividades) REFERENCES actividades(Fecha_Actividades, Hora_Actividades, Sala_Actividades, Centro_Actividades),
                        FOREIGN KEY(Codigo_clientes) REFERENCES CLIENTES(Codigo_clientes));

/*Creacion de tabla organizan*/
CREATE TABLE ORGANIZAN(Codigo_Empleados INT, 
                       Fecha_Actividades DATE, 
                       Hora_Actividades TIMESTAMP, 
                       Sala_Actividades INT, 
                       Centro_Actividades VARCHAR(40),
                       PRIMARY KEY(Codigo_Empleados, Fecha_Actividades, Hora_Actividades, Sala_Actividades, Centro_Actividades),
                       FOREIGN KEY(Codigo_Empleados) REFERENCES EMPLEADOS(Codigo_Empleados),
                       FOREIGN KEY(Fecha_Actividades, Hora_Actividades, Sala_Actividades, Centro_Actividades) REFERENCES ACTIVIDADES(Fecha_Actividades, Hora_Actividades, Sala_Actividades, Centro_Actividades));

/*Creacion de tabla atienden*/
CREATE TABLE ATIENDEN(Codigo_Empleados INT,
                      Codigo_Clientes INT,
                      PRIMARY KEY(Codigo_Empleados, Codigo_Clientes),
                      FOREIGN KEY(Codigo_Empleados) REFERENCES EMPLEADOS(Codigo_Empleados),
                      FOREIGN KEY(Codigo_Clientes) REFERENCES CLIENTES(Codigo_Clientes));
