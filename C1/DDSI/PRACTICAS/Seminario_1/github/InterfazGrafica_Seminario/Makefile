JC = javac -cp
JAR = jar cfm
JV = java -cp


default: Seminario1.jar ejecucion

ejecucion:
	$(JV) ./dist/lib/ojdbc11.jar:Seminario1.jar ddsi_seminario1.VentanaPrincipal
	
Seminario1.jar: compilado
	$(JAR) Seminario1.jar manifest_ventana.txt ./ddsi_seminario1/*.class

compilado: ddsi_seminario1/*.java
	$(JC) ./dist/lib/ojdbc11.jar ./ddsi_seminario1/*.java
	
clean:
	@rm -f *.jar ddsi_seminario1/*.class
