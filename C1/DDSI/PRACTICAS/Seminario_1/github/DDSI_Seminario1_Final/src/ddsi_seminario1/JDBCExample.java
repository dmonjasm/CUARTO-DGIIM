package ddsi_seminario1;

import java.sql.*;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;


public class JDBCExample {

    private static Scanner in = new Scanner(System.in);
    protected static String accion;
    protected static Connection conn;
    protected static String Ccliente, Cpedido,FechaPedido, Cantidad, Cproducto;
    public static Savepoint CheckPoint;
   
    public static void iniciarConexion(){
        
        try{
            conn = DriverManager.getConnection("jdbc:oracle:thin:@oracle0.ugr.es:1521/practbd.oracle0.ugr.es", "X7449010", "x7449010");
            
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

    public static void CreacionYBorrado(Statement stmt){
        //Se elimina DETALLEPEDIDO y se contempla la posibilidad de que la tabla no existiera previamente
        try{
            try{
                stmt.executeUpdate("DROP TABLE DETALLEPEDIDO");
                }catch(SQLException e){
                    System.out.println("La tabla DETALLEPEDIDO no exist??a");
                }

                //Se elimina STOCK y se contempla la posibilidad de que la tabla no existiera previamente
                try{
                    stmt.executeUpdate("DROP TABLE STOCK");

                }catch(SQLException e){
                        System.out.println("La tabla STOCK no exist??a");
                }
                
                //Se crea de 0 la tabla STOCK
                stmt.executeUpdate("CREATE TABLE STOCK (Cproducto int PRIMARY KEY, Cantidad int)");

                //Se elimina PEDIDO y se contempla la posibilidad de que la tabla no existiera previamente
                try{
                    stmt.executeUpdate("DROP TABLE PEDIDO");
                
                }catch(SQLException e){
                        System.out.println("ALa tabla PEDIDO no exist??a");
                }

                //Se crea de 0 la tabla PEDIDO
                stmt.executeUpdate("CREATE TABLE PEDIDO (Cpedido int PRIMARY KEY, Ccliente int, FechaPedido DATE)");

                //Se crea de 0 la tabla DETALLEPEDIDO una vez se ha resuelto el problema de claves externas
                stmt.executeUpdate("CREATE TABLE DETALLEPEDIDO (Cpedido int, Cproducto int,Cantidad int, FOREIGN KEY(Cpedido) REFERENCES PEDIDO(Cpedido), FOREIGN KEY(Cproducto) REFERENCES STOCK(Cproducto), PRIMARY KEY(Cpedido,Cproducto))");

                for(int i=0; i<10; i++){
                    String num_pedido=String.valueOf(i+1);
                    String cantidad=String.valueOf(10*(i+1));
                    
                    stmt.executeUpdate("INSERT INTO STOCK VALUES("+ num_pedido +","+ cantidad + ")"); 
                    
                    System.out.println("Fila insertada con ??xito");
                }
                
                conn.commit();
                System.out.println("TABLAS BORRADAS Y CREADAS CON ??XITO");
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };
    
    public static void ObtenerCantidadProducto(Statement stmt){
        ResultSet Cantidad_Total;
        try {
            Cantidad_Total = stmt.executeQuery("SELECT Cantidad FROM STOCK WHERE Cproducto=" + Cproducto );
            Cantidad_Total.next();
            accion=Cantidad_Total.getString(1);
            Cantidad_Total.close();
        } catch (SQLException ex) {
            Logger.getLogger(JDBCExample.class.getName()).log(Level.SEVERE, null, ex);
        }
           
    }

    public static void AnadirDetalleProducto(Statement stmt){
        try{
//            System.out.println("Datos del detalle");
//            System.out.println("Introduzca el c??digo del producto: ");
//            Cproducto = in.nextLine();
//            System.out.println("Introduzca la cantidad ");
//            Cantidad = in.nextLine();
            
//            ResultSet Cantidad_Total = stmt.executeQuery("SELECT Cantidad FROM STOCK WHERE Cproducto=" + Cproducto );
//            Cantidad_Total.next();
//            accion=Cantidad_Total.getString(1);
//            Cantidad_Total.close();
                
//            if (Integer.parseInt(accion) >= Integer.parseInt(Cantidad)){
                stmt.executeUpdate("INSERT INTO DETALLEPEDIDO VALUES("+ Cpedido +","+ Cproducto +","+ Cantidad +")"); 
                stmt.executeUpdate("UPDATE STOCK SET Cantidad="+String.valueOf(Integer.parseInt(accion)-Integer.parseInt(Cantidad))+" WHERE Cproducto="+Cproducto);
            //}
            //else {
              //  System.out.println("No hay tanta cantidad de dicho producto");
            //}

            //MostrarContenidoBD(stmt);
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void EliminarDetallesProducto(Statement stmt){
        try{
            conn.rollback(CheckPoint);
            MostrarContenidoBD(stmt);
            System.out.println("Se han eliminado los detalles del pedido actual.");
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void CancelarPedido(Statement stmt){
        try{
            conn.rollback();
            System.out.println("Se han eliminado el pedido actual y todos sus detalles.");
            MostrarContenidoBD(stmt);
            conn.commit();
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void FinalizarPedido(){
        try{
            System.out.println("Se han guardado los cambios");
            conn.commit();
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void AltaNuevoPedido(Statement stmt){
        try{
//            System.out.println("Datos del pedido");
//            System.out.println("Introduzca el c??digo de pedido: ");
//            Cpedido = in.nextLine();
//            System.out.println("Introduzca el c??digo del cliente: ");
//            Ccliente = in.nextLine();
//            System.out.println("Introduzca la fecha del pedido (formato dd/mm/aaaa): ");
//            FechaPedido = in.nextLine();
            stmt.executeUpdate("INSERT INTO PEDIDO VALUES("+ Cpedido +"," + Ccliente + ", TO_DATE('" + FechaPedido + "', 'dd/mm/yyyy'))"); 
            CheckPoint = conn.setSavepoint("Pedido");
            boolean cerrar2 = false;

//            while (!cerrar2){
//            
//                System.out.println("[1]A??adir detalle de producto.");
//                System.out.println("[2]Eliminar todos los detalles de producto.");
//                System.out.println("[3]Cancelar pedido");
//                System.out.println("[4]Finalizar pedido");
//
//                accion = in.nextLine();
//
//                switch(accion){
//                    case "1":
//                        AnadirDetalleProducto(stmt, conn, Cpedido);
//                    break;
//
//                    case "2":
//                        EliminarDetallesProducto(stmt, conn, CheckPoint);
//                    break;
//
//                    case "3":
//                        cerrar2=true;
//                        CancelarPedido(conn, stmt);
//                    break;
//
//                    case "4":
//                        cerrar2=true;
//                        FinalizarPedido(conn);
//                    break;
//                }
//            }

            //conn.commit(); //ESTO PUEDE FALLAR
        }catch(SQLException e){
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    };

    public static void CerrarPrograma(Connection conn){
        try{
            conn.commit();
            System.out.println("Cerrando Conexi??n");
            conn.close();
            System.out.println("Conexi??n Cerrada.\n!Hasta pronto!");
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

        System.out.println("!!!Bienvenido a la base de datos!!!. ??Qu?? desea hacer? (Introduzca 1, 2, 3 o 4):");
        
        while(!cerrar){
            System.out.println("[1]Borrado y nueva creaci??n de las tablas e inserci??n de 10 tuplas predifenidas en el c??digo en la tabla Stock.");
            System.out.println("[2]Dar de alta nuevo pedido");
            System.out.println("[3]Mostrar contenido de las tablas de la BD");
            System.out.println("[4]Salir del programa y cerrar conexi??n a la BD");

            accion = in.nextLine();

            switch(accion){
                case "1":
                    CreacionYBorrado(stmt);
                break;

                case "2": 
                    AltaNuevoPedido(stmt);
                break;

                case "3":
                    MostrarContenidoBD(stmt);
                break;

                case "4":
                    cerrar=true;
                    CerrarPrograma(conn);
                break;

                default:
                    System.out.println("Car??cter no reconocido, introduzca un car??cter v??lido (Car??cteres V??lidos: 1, 2, 3 y 4)");
                break;
            }
        }

        } catch (SQLException e) {
            System.err.format("SQL State: %s\n%s\n", e.getSQLState(), e.getMessage());
        }
    }
}
