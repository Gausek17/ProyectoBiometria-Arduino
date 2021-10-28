// -*- mode: c++ -*-

#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO
/**
 * Medidor
 * Fichero donde medimos los datos con el sensor
 * Alberto Valls Martinez
 * 26/10/21
 */
/**
 * Clase Medidor
 */
class Medidor {

  // .....................................................
  // .....................................................
private:

public:

  // .....................................................
  // constructor
  // .....................................................
  Medidor(  ) {
  } // ()

  /**
   * iniciarMedidor()
   * 
   * Metodo iniciarMedidor que inicializa nuestro medidor
   */
  void iniciarMedidor() {
	// las cosas que no se puedan hacer en el constructor, if any
  } // ()

  /**
   * medirCO2()->Z
   * 
   * Metodo medirC02 que mide el CO2
   * 
   * @return devuelve el CO2
   */
  int medirCO2() {
	return 235;
  } // ()

  /**
   * medirTemperatura()->Z
   * 
   * Metodo medirTemperatura que mide la temperatura
   * 
   * @return devuelve la temperatura
   */
  int medirTemperatura() {
	return -12; // qué frío !
  } // ()
	
}; // class

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
#endif
