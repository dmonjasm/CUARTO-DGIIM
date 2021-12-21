DECLARE
	cuota NUMBER;
	base_hasta NUMBER;
	tipo_aplicable NUMBER(4,1);
	resultado REAL;
BEGIN
	SELECT base_hasta, cuota, tipo_aplicable INTO base_hasta, cuota, tipo_aplicable FROM tabla_impositiva WHERE 
		base_hasta <= 34558.2 AND base_hasta + 
		resto_hasta >= 34558.2;
	resultado := cuota + (34558.2 - base_hasta) * tipo_aplicable / 100;
	DBMS_OUTPUT.PUT_LINE('IRPF para ' 
                            || 34558.2 
                            || ' = ' 
                            || resultado);
END;

