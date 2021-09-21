
// -*- mode: c++ -*-

// ----------------------------------------------------------
// Jordi Bataller i Mascarell
// 2019-07-07
// ----------------------------------------------------------

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO

/**
 * Constructor con el que inicializaremos el puerto serie
 * 
 * @param baudios que controla la velocidad de comunicación 
 */
class PuertoSerie  {

public:
  // .........................................................
  // .........................................................
  PuertoSerie (long baudios) {
	Serial.begin( baudios );
	// mejor no poner esto aquí: while ( !Serial ) delay(10);   
  } // 

  /**
   * Metodo que hace un delay mientras el puerto serie esté no disponible
   * 
   */
  void esperarDisponible() {

	while ( !Serial ) {
	  delay(10);//   
	}

  } // ()

  /**
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
