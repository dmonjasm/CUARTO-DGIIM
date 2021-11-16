package ddsi_seminario1;

import java.sql.*;
import java.util.Scanner;


public class JDBCExample {

    private static Scanner in = new Scanner(System.in);
    protected static String accion;
    protected static Connection conn;
   
    public static void iniciarConexion(){
        
        try{
            conn = DriverManager.getConnection("jdbc:oracle:thin:@oracle0.ugr.es:1521/practbd.oracle0.ugr.es", "X0274432", "x0274432");
            
            if (conn != null) {
                conn.setAutoCommit(false);
                System.out.println("Connected to the database!\nAutoCommit Disabled");
            } else {
                System.out.println("Failed to make connection!");
            }
         
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        } catch (Exception e) {
            e.printStackTrace();
        }
    };
    
    public static ResultSet tabla_Stock(Statement stmt){
        ResultSet salida=null;
        
        try{
        salida=stmt.executeQuery("select * from STOCK");
        
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
        
        return salida;
    };
    
    public static ResultSet tabla_Pedido(Statement stmt){
        ResultSet salida=null;
        
        try{
        salida=stmt.executeQuery("select * from PEDIDO");
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
        
        return salida;
    };
    
    public static ResultSet tabla_DetallePedido(Statement stmt){
        ResultSet salida=null;
        
        try{
        salida=stmt.executeQuery("select * from DETALLEPEDIDO");
        
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
        
        return salida;
    };
    
    public static void MostrarContenidoBD(Statement stmt){
        ResultSet salida;
        try{
            for(int i=0; i < 3; i++){
                if(i==0){
                    System.out.println("TABLA STOCK:");
                    salida=tabla_Stock(stmt);
                    while(salida.next())
                        System.out.println(salida.getString(1) + " " + salida.getString(2));
                }

                else if(i==1){
                    
                    System.out.println("TABLA PEDIDO");
                    salida=tabla_Pedido(stmt);
                    while(salida.next())
                        System.out.println(salida.getString(1) + " " + salida.getString(2) + " " + salida.getString(3));

                }

                else{
                    
                    System.out.println("TABLA DETALLEPEDIDO");
                    salida=tabla_DetallePedido(stmt);
                    while(salida.next())
                        System.out.println(salida.getString(1) + " " + salida.getString(2) + " " + salida.getString(3));
                }
            
            }
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void CreacionYBorrado(Statement stmt, Connection conn){
        //Se elimina DETALLEPEDIDO y se contempla la posibilidad de que la tabla no existiera previamente
        try{
            try{
                stmt.executeUpdate("DROP TABLE DETALLEPEDIDO");
                }catch(SQLException e){
                    System.out.println("La tabla DETALLEPEDIDO no existía");
                }

                //Se elimina STOCK y se contempla la posibilidad de que la tabla no existiera previamente
                try{
                    stmt.executeUpdate("DROP TABLE STOCK");

                }catch(SQLException e){
                        System.out.println("La tabla STOCK no existía");
                }
                
                //Se crea de 0 la tabla STOCK
                stmt.executeUpdate("CREATE TABLE STOCK (Cproducto int PRIMARY KEY, Cantidad int)");

                //Se elimina PEDIDO y se contempla la posibilidad de que la tabla no existiera previamente
                try{
                    stmt.executeUpdate("DROP TABLE PEDIDO");
                
                }catch(SQLException e){
                        System.out.println("ALa tabla PEDIDO no existía");
                }

                //Se crea de 0 la tabla PEDIDO
                stmt.executeUpdate("CREATE TABLE PEDIDO (Cpedido int PRIMARY KEY, Ccliente int, FechaPedido DATE)");

                //Se crea de 0 la tabla DETALLEPEDIDO una vez se ha resuelto el problema de claves externas
                stmt.executeUpdate("CREATE TABLE DETALLEPEDIDO (Cpedido int, Cproducto int,Cantidad int, FOREIGN KEY(Cpedido) REFERENCES PEDIDO(Cpedido), FOREIGN KEY(Cproducto) REFERENCES STOCK(Cproducto), PRIMARY KEY(Cpedido,Cproducto))");

                for(int i=0; i<10; i++){
                    String num_pedido=String.valueOf(i+1);
                    String cantidad=String.valueOf(10*(i+1));
                    
                    stmt.executeUpdate("INSERT INTO STOCK VALUES("+ num_pedido +","+ cantidad + ")"); 
                    
                    System.out.println("Fila insertada con éxito");
                }
                
                conn.commit();
                System.out.println("TABLAS BORRADAS Y CREADAS CON ÉXITO");
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void AnadirDetalleProducto(Statement stmt, Connection conn, String Cpedido){
        try{
            System.out.println("Datos del detalle");
            System.out.println("Introduzca el código del producto: ");
            String Cproducto = in.nextLine();
            System.out.println("Introduzca la cantidad ");
            String Cantidad = in.nextLine();
            ResultSet Cantidad_Total = stmt.executeQuery("SELECT Cantidad FROM STOCK WHERE Cproducto=" + Cproducto );
            Cantidad_Total.next();
            accion=Cantidad_Total.getString(1);
            Cantidad_Total.close();

            if (Integer.parseInt(accion) >= Integer.parseInt(Cantidad)){
                stmt.executeUpdate("INSERT INTO DETALLEPEDIDO VALUES("+ Cpedido +","+ Cproducto +","+ Cantidad +")"); 
                stmt.executeUpdate("UPDATE STOCK SET Cantidad="+String.valueOf(Integer.parseInt(accion)-Integer.parseInt(Cantidad))+" WHERE Cproducto="+Cproducto);
            }
            else {
                System.out.println("No hay tanta cantidad de dicho producto");
            }

            MostrarContenidoBD(stmt);
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void EliminarDetallesProducto(Statement stmt, Connection conn, Savepoint CheckPoint){
        try{
            conn.rollback(CheckPoint);
            MostrarContenidoBD(stmt);
            System.out.println("Se han eliminado los detalles del pedido actual.");
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void CancelarPedido(Connection conn, Statement stmt){
        try{
            conn.rollback();
            System.out.println("Se han eliminado el pedido actual y todos sus detalles.");
            MostrarContenidoBD(stmt);
            conn.commit();
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void FinalizarPedido(Connection conn){
        try{
            System.out.println("Se han guardado los cambios");
            conn.commit();
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void AltaNuevoPedido(Statement stmt, Connection conn){
        try{
            System.out.println("Datos del pedido");
            System.out.println("Introduzca el código de pedido: ");
            String Cpedido = in.nextLine();
            System.out.println("Introduzca el código del cliente: ");
            String Ccliente = in.nextLine();
            System.out.println("Introduzca la fecha del pedido (formato dd/mm/aaaa): ");
            String FechaPedido = in.nextLine();
            stmt.executeUpdate("INSERT INTO PEDIDO VALUES("+ Cpedido +"," + Ccliente + ", TO_DATE('" + FechaPedido + "', 'dd/mm/yyyy'))"); 
            Savepoint CheckPoint = conn.setSavepoint("Pedido");
            boolean cerrar2 = false;

            while (!cerrar2){
            
                System.out.println("[1]Añadir detalle de producto.");
                System.out.println("[2]Eliminar todos los detalles de producto.");
                System.out.println("[3]Cancelar pedido");
                System.out.println("[4]Finalizar pedido");

                accion = in.nextLine();

                switch(accion){
                    case "1":
                        AnadirDetalleProducto(stmt, conn, Cpedido);
                    break;

                    case "2":
                        EliminarDetallesProducto(stmt, conn, CheckPoint);
                    break;

                    case "3":
                        cerrar2=true;
                        CancelarPedido(conn, stmt);
                    break;

                    case "4":
                        cerrar2=true;
                        FinalizarPedido(conn);
                    break;
                }
            }

            conn.commit();
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void CerrarPrograma(Connection conn){
        try{
            System.out.println("Cerrando Conexión");
            conn.close();
            System.out.println("Conexión Cerrada.\n!Hasta pronto!");
            System.exit(0);
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void main(String[] args) {

        // https://docs.oracle.com/javase/8/docs/api/java/sql/package-summary.html#package.description
        // auto java.sql.Driver discovery -- no longer need to load a java.sql.Driver class via Class.forName

        // register JDBC driver, optional since java 1.6
        /*try {
            Class.forName("oracle.jdbc.driver.OracleDriver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }*/

		// Oracle SID = orcl , find yours in tnsname.ora
        try{
        
        iniciarConexion();
        
        Statement stmt=conn.createStatement();
        boolean cerrar=false;

        System.out.println("!!!Bienvenido a la base de datos!!!. ¿Qué desea hacer? (Introduzca 1, 2, 3 o 4):");
        
        while(!cerrar){
            System.out.println("[1]Borrado y nueva creación de las tablas e inserción de 10 tuplas predifenidas en el código en la tabla Stock.");
            System.out.println("[2]Dar de alta nuevo pedido");
            System.out.println("[3]Mostrar contenido de las tablas de la BD");
            System.out.println("[4]Salir del programa y cerrar conexión a la BD");

            accion = in.nextLine();

            switch(accion){
                case "1":
                    CreacionYBorrado(stmt, conn);
                break;

                case "2": 
                    AltaNuevoPedido(stmt, conn);
                break;

                case "3":
                    MostrarContenidoBD(stmt);
                break;

                case "4":
                    cerrar=true;
                    CerrarPrograma(conn);
                break;

                default:
                    System.out.println("Carácter no reconocido, introduzca un carácter válido (Carácteres Válidos: 1, 2, 3 y 4)");
                break;
            }
        }

        } catch (SQLException e) {
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    }
}
