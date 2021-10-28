
// -*- mode: c++ -*-

/**
 * PuertoSerie
 * Fichero del puerto serie conectado
 * Alberto Valls Martinez
 * 26/10/21
 */

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO

/**
 * Constructor con el que inicializaremos el puerto serie
 * 
 * @param baudios que controla la velocidad de comunicación 
 */
class PuertoSerie  {

public:
/**
 * PuertoSerie()
 * 
 * Constructor que inicializa el PuertoSerie
 */
  PuertoSerie (long baudios) {
	Serial.begin( baudios );
	// mejor no poner esto aquí: while ( !Serial ) delay(10);   
  } // 

  /**
   * 
   * esperarDisponible()
   * Metodo que hace un delay mientras el puerto serie esté no disponible
   * 
   */
  void esperarDisponible() {

	while ( !Serial ) {
	  delay(10);//   
	}

  } // ()

  /**
   * 
   * T->escribir()
   * Metodo que utilizamos para escribir un mensaje de tipo T(cualquier tipo de dato)
   * 
   */
  template<typename T>
  void escribir (T mensaje) {
	Serial.print( mensaje );
  } // ()
  
}; // class PuertoSerie

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
