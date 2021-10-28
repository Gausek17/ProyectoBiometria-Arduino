// -*- mode: c++ -*-

/**
 * Publicador
 * Fichero donde publicamos los valores recogidos
 * Alberto Valls Martinez
 * 26/10/21
 */

#ifndef PUBLICADOR_H_INCLUIDO
#define PUBLICADOR_H_INCLUIDO

/**
 * Clase publicador que transmite beacons via BLE con los valores de CO2
 */
class Publicador {

  
private:
//identificamos el tipo de beacon que queremos medir guardándolo como un texto
  uint8_t beaconUUID[16] = { 
	'A', 'L', 'B', 'E', 'R', 'T', 'O', 'V', 
	'A', 'L', 'L', 'S', '-', 'G', 'T', 'I'
	};

public:
//creamos una emisora con la que mandaremos los datos
  EmisoraBLE laEmisora {
	"GTI-Alberto", //  nombre emisora
	  0x004c, // fabricanteID (Apple)
	  4 // txPower
	  };
  //controlamos su intensidad de la señal
  const int RSSI = -53; // por poner algo, de momento no lo uso

 
public:

  //creamos un enumerador donde guardaremos los datos recogidos
  enum MedicionesID  {
	CO2 = 11,
	TEMPERATURA = 12,
	RUIDO = 13
  };

  /**
   * Publicador()
   * 
   * Constructor de publicador
   */
  Publicador( ) {
	// ATENCION: no hacerlo aquí. (*this).laEmisora.encenderEmisora();
	// Pondremos un método para llamarlo desde el setup() más tarde
  } // ()

  /**
   * encenderEmisora()
   * 
   * Metodo que sirve para encender la Emisora
   */
  void encenderEmisora() {
	(*this).laEmisora.encenderEmisora();
  } // ()

  /**
   * Z,N,Z ->publicarCO2()
   * 
   * Metodo que publica los datos recogidos
   * 
   * @param valorCO2 medida del CO2
   * @param contador la cantidad de medidas que hemos hecho
   * @param tiempoEspera el tiempo que esperamos 
   */
  void publicarCO2( int16_t valorCO2, uint8_t contador,
					long tiempoEspera ) {

	//
	// 1. empezamos anuncio
	//

  //ponemos en los 8 primeros bits el id de la medicion y en los otros 8 el contador
	uint16_t major = (MedicionesID::CO2 << 8) + contador;
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
											major,
											valorCO2, // minor
											(*this).RSSI // rssi
									);


	//
	// 2. esperamos el tiempo que nos digan
	//
	esperar( tiempoEspera );

	//
	// 3. paramos anuncio
	//
	(*this).laEmisora.detenerAnuncio();
  } // ()

/**
 * 
 * Z,N,Z ->publicarTemperatura()
 * Metodo para publicar la temperatura
 * 
 * @param valorTemperatura valor recogido de la temperatura
 * @param contador la cantidad de medidas que hemos realizado
 * @param tiempoEspera tiempo que espera el publicador
 */
  void publicarTemperatura( int16_t valorTemperatura,
							uint8_t contador, long tiempoEspera ) {

	uint16_t major = (MedicionesID::TEMPERATURA << 8) + contador;
	(*this).laEmisora.emitirAnuncioIBeacon( (*this).beaconUUID, 
											major,
											valorTemperatura, // minor
											(*this).RSSI // intensidad 
									);
	esperar( tiempoEspera );

	(*this).laEmisora.detenerAnuncio();//detenemos el anuncio
  } // ()
	
}; // class


#endif
