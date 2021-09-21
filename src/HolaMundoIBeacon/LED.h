// -*- mode: c++ -*-

#ifndef LED_H_INCLUIDO
#define LED_H_INCLUIDO

// ----------------------------------------------------------
// Jordi Bataller i Mascarell
// 2019-07-07
// ----------------------------------------------------------

/**
 * Metodo que provoca un delay 
 * 
 * @param tiempo El parametro tiempo identifica al tiempo que tiene que realizar el delay
 *  
 * 
  */
void esperar (long tiempo) {
  delay (tiempo);
  
}

/**
 * Clase LED con el que identificaremos el pin al que está conectado el LED y su estado
 */
class LED {
private:
  int numeroLED; //identifica el pin del LED
  bool encendido; //nos dice si el led está encendido(true) o apagado(false)
public:

  /**
   * constructor del LED al que asignamos un numero de pin y un estado para el LED 
   * inicializamos la variable encendido en false y el pin empezará apagado al llamar a apagar()
   * 
   * @param numero identifica el pin al que está conectado el LED
   * 
   */
  LED (int numero)
	: numeroLED (numero), encendido(false)
  {
	pinMode(numeroLED, OUTPUT);
	apagar ();
  }

  // .........................................................
  // .........................................................

  /**
   * Metodo que cambia el estado del LED y lo enciende.Cambiando encendido a true.
   * 
   *
   */
  void encender () {
	digitalWrite(numeroLED, HIGH); //hacemos que el LED se encienda
	encendido = true;//cambiamos la variable a true
  }

  /**
   * Metodo que cambia el estado del LED y lo apaga. Cambiando encendido a false.
   */
  void apagar () {
	  digitalWrite(numeroLED, LOW);//hacemos que el LED se apague
	  encendido = false;//cambiamos la variable a false
  }

  /**
   * Metodo que alterna el estado del LED entre encendido y apagado.
   */
  void alternar () {
	if (encendido) {//si está encendido 
	  apagar();//se apaga
	} else {//sino
	  encender ();//se enceinde
	}
  } // ()

  /**
   * Metodo que enciende el LED espera un tiempo y depués lo apaga.
   */
  void brillar (long tiempo) {
	encender (); //enciende el LED llamando a la funcion
	esperar(tiempo);//esperamos un tiempo
	apagar (); //apagar el LED llamando a la funcion
  }
}; // class

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
