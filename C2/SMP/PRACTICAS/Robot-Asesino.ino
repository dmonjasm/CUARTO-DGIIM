#define pin1 5
#define pin2 6
#define pin3 3
#define pin4 4

#define pinIR0 A2
#define pinIR1 A3
#define pinIR2 A0
#define pinIR3 A1

// #define pinAnalogAngulo A1

#define pinEcho 10
#define pinTrigger 9

#define TIMEOUT_ECO 23200

uint8_t IR[] = {pinIR0, pinIR1, pinIR2, pinIR3};
uint8_t lecturas_anteriores[] = {0, 0, 0, 0};

bool polaridad = true;

struct Estado // Puede usarse bit set-bit unset
{
	unsigned enemigo : 1;
	unsigned ir_lf : 1;
	unsigned ir_rf : 1;
	unsigned ir_lb : 1;
	unsigned ir_rb : 1;
	// unsigned resto : 2;
	bool operator==(const struct Estado &e2)
	{
		return this->enemigo == e2.enemigo &&
				 this->ir_lb == e2.ir_lb &&
				 this->ir_rb == e2.ir_rb &&
				 this->ir_lf == e2.ir_lf &&
				 this->ir_rf == e2.ir_rf;
	}
	bool operator!=(const struct Estado &e2) { return !(*this == e2); }
};

struct Estado estado_anterior = {0b0, 0b0, 0b0, 0b0, 0b0};
struct Estado estado_actual;

void actualizarEstado(struct Estado &estado_actual);
void movimiento_izquierda(int dir);
void movimiento_derecha(int dir);
bool comprobarAnalogico(uint8_t n);
bool buscarEnemigo();
void mover(int izq, int der);

void setup()
{
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
	pinMode(pin3, OUTPUT);
	pinMode(pin4, OUTPUT);

	pinMode(pinIR0, INPUT);
	pinMode(pinIR1, INPUT);
	pinMode(pinIR2, INPUT);
	pinMode(pinIR3, INPUT);

	// pinMode(pinAnalogAngulo, INPUT);

	pinMode(pinTrigger, OUTPUT);
	pinMode(pinEcho, INPUT_PULLUP);
	digitalWrite(pin1, LOW);
	digitalWrite(pin2, LOW);
	digitalWrite(pin3, LOW);
	digitalWrite(pin4, LOW);
	digitalWrite(pinTrigger, LOW);

	Serial.begin(9600);

	delay(3000);

  mover(1,-1);
}

void loop()
{
	actualizarEstado(estado_actual);

	if (estado_actual != estado_anterior)
	{
		// Se sale por detrás
		if (estado_actual.ir_rb)
		{
			// Giro hacia delante izquierda
			mover(0, 1);
			if (estado_actual.ir_lb)
				mover(1, 1); // Aquí sería sacar el pinchito
		}
		else if (estado_actual.ir_lb)
		{
			// Giro hacia delante derecha
			mover(1, 0);
			if (estado_actual.ir_rb)
				mover(1, 1); // Aquí sería sacar el pinchito
		}

		// Se sale por delante
		else if (estado_actual.ir_rf)
		{
			// Giro hacia atrás izquierda
			mover(0, -1);
			if (estado_actual.ir_lf)
				mover(-1, -1);
		}
		else if (estado_actual.ir_lf)
		{
			// Giro hacia atrás derecha
			mover(-1, 0);
			if (estado_actual.ir_rf)
				mover(-1, -1);
		}
		else if (!estado_actual.enemigo)
		{
			mover(-1, 1);
		}
		else
		{
			mover(1, 1);
		}
	}
	estado_anterior = estado_actual;

	delay(100);
}

void actualizarEstado(struct Estado &estado_actual)
{
	estado_actual.enemigo = buscarEnemigo();
	estado_actual.ir_lf = comprobarAnalogico(0);
	estado_actual.ir_rf = comprobarAnalogico(1);
	estado_actual.ir_lb = comprobarAnalogico(2);
	estado_actual.ir_rb = comprobarAnalogico(3);

  Serial.println("--------------");

}

void movimiento_derecha(int dir)
{
	switch (dir)
	{
	case 1:
		digitalWrite(pin1, LOW);
		digitalWrite(pin2, HIGH);
		break;

	case -1:
		digitalWrite(pin2, LOW);
		digitalWrite(pin1, HIGH);
		break;

	default:
		digitalWrite(pin1, LOW);
		digitalWrite(pin2, LOW);
		break;
	}
}

void movimiento_izquierda(int dir)
{
	switch (dir)
	{
	case -1:
		digitalWrite(pin3, LOW);
		digitalWrite(pin4, HIGH);
		break;

	case 1:
		digitalWrite(pin4, LOW);
		digitalWrite(pin3, HIGH);
		break;

	default:
		digitalWrite(pin3, LOW);
		digitalWrite(pin4, LOW);
		break;
	}
}

void mover(int izq, int der)
{
	movimiento_izquierda(izq);
	movimiento_derecha(der);
}

unsigned long leeDistancia(uint8_t triggerPin, uint8_t ecoPin)
{
	delayMicroseconds(2);			  // Por seguridad
	digitalWrite(triggerPin, HIGH); // Activa el trigger 10 us
	delayMicroseconds(10);
	digitalWrite(triggerPin, LOW);
	return pulseIn(ecoPin, HIGH, TIMEOUT_ECO); // Lee la anchura del
}

/*void gira(int dir)
{
	// Serial.mln("Cambiando");
	if (dir != 1 && dir != -1)
	{
		movimiento_izquierda(0);
		movimiento_derecha(0);
	}

	else
	{
		movimiento_izquierda((-1.0) * dir);
		movimiento_derecha((-1.0) * dir);
	}
}*/

bool buscarEnemigo()
{
	uint16_t distancia = leeDistancia(pinTrigger, pinEcho) / 58;
	return distancia < 50;
}

bool comprobarAnalogico(uint8_t n)
{
	int lectura = analogRead(IR[n]);

	//bool cambio = (lectura > 2 * lecturas_anteriores[n] || lectura < lecturas_anteriores[n] / 2) && lecturas_anteriores[n] > 0;
  bool cambio = lectura < 150;
	lecturas_anteriores[n] = lectura;

 Serial.print(n);
 Serial.print("\t");
 Serial.println(lectura);

	return cambio;
}

/*
void cambia_polaridad()
{
	// Serial.println("Cambiando");
	polaridad = !polaridad;
	if (polaridad)
	{
		movimiento_derecha(1);
		movimiento_izquierda(1);
	}
	else
	{
		movimiento_derecha(-1);
		movimiento_izquierda(-1);
	}
}*/

/* void pruebaInfrarrojos()
{
	uint8_t v0 = analogRead(IR[0]);
	uint8_t v1 = analogRead(IR[1]);
	uint8_t v2 = analogRead(IR[2]);
	uint8_t v3 = analogRead(IR[3]);

	Serial.println("IR0: " + v0);
	Serial.println("IR1: " + v1);
	Serial.println("IR2: " + v2);
	Serial.println("IR3: " + v3);
} */
