// -*- mode: c++ -*-

/**
 * EmisoraBLE
 * Fichero para enviar beacons desde nuestra emisora
 * Alberto Valls Martinez
 * 26/10/21
 */
#ifndef EMISORA_H_INCLUIDO
#define EMISORA_H_INCLUIDO

// Buena introducción: https://learn.adafruit.com/introduction-to-bluetooth-low-energy/gap
// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/

// fuente: https://www.instructables.com/id/Beaconeddystone-and-Adafruit-NRF52-Advertise-Your-/
// https://github.com/nkolban/ESP32_BLE_Arduino/blob/master/src/BLEBeacon.h

// https://os.mbed.com/blog/entry/BLE-Beacons-URIBeacon-AltBeacons-iBeacon/
// https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/bleadvertising

// ----------------------------------------------------------
// ----------------------------------------------------------
#include "ServicioEnEmisora.h"

/**
 * Clase EmisoraBLE que sirve para identificar la emisora
 */
class EmisoraBLE {
private:

  const char * nombreEmisora;//nombre de la Emisora
  const uint16_t fabricanteID;//id del fabricante
  const int8_t txPower;//nivel de señal esperado cuando se está a un metro de distancia

public:

  //callback vacio de conexion establecida que le pasamos como parámetro 
  using CallbackConexionEstablecida = void ( uint16_t connHandle );
  //callback vacio de conexion terminada que le pasamos como parámetro   
  using CallbackConexionTerminada = void ( uint16_t connHandle, uint8_t reason);//

 /**
  * char,N,Z->EmisoraBLE()
  * 
  * Constructor de Emisora BLE
  * 
  * @param nombreEmisora_ nombre con el que identificamos la emisora
  * @param fabricanteID_ id del fabricante
  * @param txPower_ nivel de señal esperado cuando se está a un metro de distancia
  */
  EmisoraBLE( const char * nombreEmisora_, const uint16_t fabricanteID_,
			  const int8_t txPower_ ) 
	:
	nombreEmisora( nombreEmisora_ ) ,
	fabricanteID( fabricanteID_ ) ,
	txPower( txPower_ )
  {}
  
  /**
   * encenderEmisora()
   * 
   * Metodo encenderEmisora que enciende la emisiora
   */
  void encenderEmisora() {
	
	 Bluefruit.begin();//Encendemos la emisora 

	 // por si acaso detenemos anuncio
	 (*this).detenerAnuncio();
  } // ()

 /**
  * CallbackConexionEstablecida, CallbackConexionTerminada -> encenderEmisora()
  * 
  * Metodo encenderEmisora pasando por parámetro cbce y cbct
  * 
  * @param cbce callback vacío de conexion establecida
  * @param cbct callback vacio de conexion terminada
  */
  void encenderEmisora( CallbackConexionEstablecida cbce,
						CallbackConexionTerminada cbct ) {

	encenderEmisora();//encendemos emisora

  //instalamos callbacks de conexiones
	instalarCallbackConexionEstablecida( cbce );
	instalarCallbackConexionTerminada( cbct );

  } // ()

  /**
   * detenerAnuncio()
   * 
   * Metodo detenerAnuncio que detiene envio anuncio(beacon) si se está emitiendo
   */
  void detenerAnuncio() {

	if ( (*this).estaAnunciando() ) {//si esta anunciando
	  // Serial.println ( "Bluefruit.Advertising.stop() " );
	  Bluefruit.Advertising.stop(); //paramos el anuncio
	}

  }  // ()
  
  /**
   * bool estaAnunciando()->V/F
   * 
   * Metodo estaAnunciando que nos sirve para saber si la emisora esta enviando beacon(true) o no(false)
   * 
   * @return devuelve si está o no en marcha
   */
  bool estaAnunciando() {
	return Bluefruit.Advertising.isRunning();
  } // ()

  /**
   * N,Z,Z,N->emitirAnuncioIBeacon()
   * 
   * Metodo emitirAnuncioIBeacon que envia anuncio con el beacon
   * 
   * @param beaconUUID para identificar la id del beacon
   * @param major para distinguir un grupo
   * @param minor para distinguir un individuo
   * @param rssi intensidad de la señal
   */
  void emitirAnuncioIBeacon( uint8_t * beaconUUID, int16_t major, int16_t minor, uint8_t rssi ) {

	//
	//
	//
	(*this).detenerAnuncio();//detenemos anuncio
	
	//
	// Creamos el beacon
	//
	BLEBeacon elBeacon( beaconUUID, major, minor, rssi );
	elBeacon.setManufacturer( (*this).fabricanteID );

	//
	// 
	//

	Bluefruit.setTxPower( (*this).txPower );
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName(); // para que envíe el nombre de emisora (?!)

	//
	// Ponemos el beacon
	//
	Bluefruit.Advertising.setBeacon( elBeacon );

	//
	// 
	//
	Bluefruit.Advertising.restartOnDisconnect(true); // no hace falta, pero lo pongo
	Bluefruit.Advertising.setInterval(100, 100);    // in unit of 0.625 ms

	//
	// Empieza el anuncio, 0 = tiempo indefinido 
	//
	Bluefruit.Advertising.start( 0 ); 
	
  } // ()
  /**
   * char,N->emitirAnuncioIBeaconLibre()
   * 
   * Metodo emitirAnuncioIBeaconLibre que sirve
   * 
   * @param carga información que necesitamos para el beacon 
   * @param tamanyoCarga el tamaño que tiene la carga para saber si es mayor o menor que 21
   */
 
  void emitirAnuncioIBeaconLibre( const char * carga, const uint8_t tamanyoCarga ) {

	(*this).detenerAnuncio();//detenemos anuncio
  //por seguridad
	Bluefruit.Advertising.clearData();
	Bluefruit.ScanResponse.clearData(); 

	// Bluefruit.setTxPower( (*this).txPower ); creo que no lo pongo porque es uno de los bytes de la parte de carga que utilizo
	Bluefruit.setName( (*this).nombreEmisora );
	Bluefruit.ScanResponse.addName();

	Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);

	
	//
	uint8_t restoPrefijoYCarga[4+21] = {
	  0x4c, 0x00, // companyID 2
	  0x02, // ibeacon type 1byte
	  21, // ibeacon length 1byte (dec=21)  longitud del resto // 0x15 // ibeacon length 1byte (dec=21)  longitud del resto
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-', '-', '-', '-', 
	  '-'
	};

	//
	// addData() hay que usarlo sólo una vez. Por eso copio la carga
	// en el anterior array, donde he dejado 21 sitios libres
	//
  //si el tamaño de la carga es mayor que 21 cogemos 21 y si no, el tamanyoCarga
	memcpy( &restoPrefijoYCarga[4], &carga[0], ( tamanyoCarga > 21 ? 21 : tamanyoCarga ) ); 

	//
	// copio la carga para emitir
	//
	Bluefruit.Advertising.addData( BLE_GAP_AD_TYPE_MANUFACTURER_SPECIFIC_DATA,
								   &restoPrefijoYCarga[0],
								   4+21 );

	//
	// valores que ponemos
	//
	Bluefruit.Advertising.restartOnDisconnect(true);
	Bluefruit.Advertising.setInterval(100, 100);    // in unit of 0.625 ms

	Bluefruit.Advertising.setFastTimeout( 1 );      // number of seconds in fast mode
	//
	// empieza el anuncio, 0 = tiempo indefinido (ya lo pararán)
	//
	Bluefruit.Advertising.start( 0 ); 

	Globales::elPuerto.escribir( "emitiriBeacon libre  Bluefruit.Advertising.start( 0 );  \n");
  } // ()

 /**
  * ServicioEnEmisora->anyadirServicio()->boolean
  * 
  * Metodo anyadirServicio sirve para añadir un servicio y saber si se ha añadido o no
  * 
  * @param servicio que queremos añadir
  * 
  * @return devuelve verdadero si se ha añadido el servicio o false si no se ha añadido 
  */
  bool anyadirServicio( ServicioEnEmisora & servicio ) {

	Globales::elPuerto.escribir( " Bluefruit.Advertising.addService( servicio ); \n");

	bool r = Bluefruit.Advertising.addService( servicio );

	if ( ! r ) {//si no se ha añadido
	  Serial.println( " SERVICION NO AÑADIDO \n");
	}
	

	return r;
	 // nota: uso conversión de tipo de servicio (ServicioEnEmisora) a BLEService
	 // para add Service()
  } // ()

  
  /**
   * ServicioEnEmisora->anyadirServicioConSusCaracteristicas()->boolean
   * 
   * Metodo anyadirServicioConSusCaracteristicas sirve para añadir un metodo con sus caracteristicas
   * 
   * @param servicio que queremos añadir
   * @return devuelve verdadero o falso si se ha añadido el servicio con sus caracteristicas
   */
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio ) { 
	return (*this).anyadirServicio( servicio );
  } // 

 /**
  * ServicioEnEmisora, T-> anyadirServicioConSusCaracteristicas()->boolean
  * 
  * Metodo anyadirServicioConSusCaracteristicas
  * 
  * @param servicio lo que queremos añadir
  * @param caracteristica las caracteristicas del servicio
  * @param restoCaracteristicas el resto de las caracteristicas del servicio
  * 
  * @return devuelve verdadero o falso si se ha añadido el servicio con sus caracteristicas
  */
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicas( ServicioEnEmisora & servicio,
											 ServicioEnEmisora::Caracteristica & caracteristica,
											 T& ... restoCaracteristicas) {

	servicio.anyadirCaracteristica( caracteristica );

	return anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );
	
  } // ()

  /**
   * ServicioEnEmisora, T-> anyadirServicioConSusCaracteristicasYActivar()->boolean
   * 
   * Metodo anyadirServicioConSusCaracteristicasYActivar 
   * 
   * @param servicio que queremos añadir
   * @param restoCaracteristicas del servicio a añadir
   * 
   * @return devuelve verdadero o falso si se ha añadido el servicio con sus caracteristicas y se ha activado
   */
  template <typename ... T>
  bool anyadirServicioConSusCaracteristicasYActivar( ServicioEnEmisora & servicio,
													 // ServicioEnEmisora::Caracteristica & caracteristica,
													 T& ... restoCaracteristicas) {

	bool r = anyadirServicioConSusCaracteristicas( servicio, restoCaracteristicas... );

	servicio.activarServicio();

	return r;
	
  } // ()

  /**
   * CallbackConexionEstablecida->instalarCallbackConexionEstablecida()
   * 
   * Metodo instalarCallbackConexionEstablecida se necesita para instalar el callback
   * 
   * @param cb varaible utilizada para el callback
   */
  void instalarCallbackConexionEstablecida( CallbackConexionEstablecida cb ) {
	Bluefruit.Periph.setConnectCallback( cb );
  } // ()

  /**
   *  * CallbackConexionTerminada->instalarCallbackConexionTerminada()
   * 
   * Metodo instalarCallbackConexionTerminada se necesita para instalar el callback
   * 
   * @param cb varaible utilizada para el callback
   */
  void instalarCallbackConexionTerminada( CallbackConexionTerminada cb ) {
	Bluefruit.Periph.setDisconnectCallback( cb );
  } // ()

  /**
   * Metodo que nos devuelve la conexión deseada
   * 
   * @param que identifica la conexión a consultar
   */
  BLEConnection * getConexion( uint16_t connHandle ) {
	return Bluefruit.Connection( connHandle );
  } // ()

}; // class

#endif
