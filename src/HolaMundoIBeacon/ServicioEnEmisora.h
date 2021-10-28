// -*- mode: c++ -*-

/**
 * ServicioEnEmisora
 * Fichero donde llamamos a los demás metodos
 * Alberto Valls Martinez
 * 26/10/21
 */
#ifndef SERVICIO_EMISORA_H_INCLUIDO
#define SERVICIO_EMISORA_H_INCLUIDO

// ----------------------------------------------------
// ----------------------------------------------------
#include <vector>



/**
 * T,Z->alReves->T
 * Metodo que pone al revés el contenido de un array en el mismo array
 * 
 * @param p puntero al array que queremos dar la vuelta y cambiar su contenido
 * @param n tamaño del array
 * @return devuelve el parametro con el contenido dado la vuelta
 */
template< typename T >
T *  alReves( T * p, int n ) {
  T aux;

  for( int i=0; i < n/2; i++ ) {
	aux = p[i];
	p[i] = p[n-i-1];
	p[n-i-1] = aux;
  }
  return p;
} // ()

/**
 * char,N,Z->stringAint8AlReves()->N
 * 
 * Metodo que convierte una string const char en uint8_t y le da la vuelta
 * 
 * @param pString puntero que hacemos al char string que será el nombre del Servicio
 * @param pUint puntero que hacemos al uuidServicio
 * @param tamMax tamaño maximo del string
 * @return devolvemos el string al revés en formato ASCII 
 */
uint8_t * stringAUint8AlReves( const char * pString, uint8_t * pUint, int tamMax ) {

	int longitudString =  strlen( pString );
	int longitudCopiar = ( longitudString > tamMax ? tamMax : longitudString );
	// copio nombreServicio -> uuidServicio pero al revés
	for( int i=0; i<=longitudCopiar-1; i++ ) {
	  pUint[ tamMax-i-1 ] = pString[ i ];
	} // for

	return pUint;
} // ()

/**
 * Clase ServicioEnEmisora
 */
class ServicioEnEmisora {

public:
/**
 * N,BLECharacteristic,N,N->CallbackCaracteristicaEscrita()
 */

  
  using CallbackCaracteristicaEscrita = void ( uint16_t conn_handle,
											   BLECharacteristic * chr,
											   uint8_t * data, uint16_t len); 
  /**
   * Clase Caracteristica
   */
  class Caracteristica {
  private:
	uint8_t uuidCaracteristica[16] = { // el uuid se copia aquí (al revés) a partir de un string-c
	  // least signficant byte, el primero
	  '0', '1', '2', '3', 
	  '4', '5', '6', '7', 
	  '8', '9', 'A', 'B', 
	  'C', 'D', 'E', 'F'
	};


	BLECharacteristic laCaracteristica;

  public:
  /**
   * Constructor
   */
	
	Caracteristica( const char * nombreCaracteristica_ )
	  :
	  laCaracteristica( stringAUint8AlReves( nombreCaracteristica_, &uuidCaracteristica[0], 16 ) )
	{
	  
	} // ()

	/**
  * Constructor sobrecargado
	 */
	Caracteristica( const char * nombreCaracteristica_ ,
					uint8_t props,
					SecureMode_t permisoRead,
					SecureMode_t permisoWrite, 
					uint8_t tam ) 
	  :
	  Caracteristica( nombreCaracteristica_ ) // llamada al otro constructor
	{
	  (*this).asignarPropiedadesPermisosYTamanyoDatos( props, permisoRead, permisoWrite, tam );//aginamos propiedades,permisos y el tamaño de datos
   
	} // ()

  private:
	
	// CHR_PROPS_WRITE , CHR_PROPS_READ ,  CHR_PROPS_NOTIFY 
	/**
  * 
  * N->asignarPropiedades()
  * Metodo asignarPropiedades para asiganar las propiedades deseadas 
  * 
  * @param props que son las propiedades que asignamos
	 */
	void asignarPropiedades ( uint8_t props ) {
	  // no puedo escribir AUN si el constructor llama a esto: Serial.println( " laCaracteristica.setProperties( props ); ");
	  (*this).laCaracteristica.setProperties( props );
	} // ()

	// .........................................................
	// BleSecurityMode::SECMODE_OPEN  , BleSecurityMode::SECMODE_NO_ACCESS
  /**
   * SecureMode_t,SecureMode_t->asignarPermisos()
   * Metodo asiganrPermisos que se usa para asinar el permiso que queramos
   * 
   * @param permisoRead para leer el permiso
   * @param permisoWrite para escribir los permisos
   */
	void asignarPermisos( SecureMode_t permisoRead, SecureMode_t permisoWrite ) {
	  // no puedo escribir AUN si el constructor llama a esto: Serial.println( "laCaracteristica.setPermission( permisoRead, permisoWrite ); " );
	  (*this).laCaracteristica.setPermission( permisoRead, permisoWrite );
	} // ()

	/**
  * N->asignarTamanyoDatos()
   * Metodo asignarTamanyoDatos para establecer el tamaño de los datos enviados
   * 
   * @param tam determina el tamaño
	 */
	void asignarTamanyoDatos( uint8_t tam ) {
	  // no puedo escribir AUN si el constructor llama a esto: Serial.print( " (*this).laCaracteristica.setFixedLen( tam = " );
	  // no puedo escribir AUN si el constructor llama a esto: Serial.println( tam );
	  // (*this).laCaracteristica.setFixedLen( tam );
	  (*this).laCaracteristica.setMaxLen( tam );
	} // ()

  public:
/**
 * Metodo asignarPropiedadesPermisosYTamanyoDatos
 * 
 * @param props que son las propiedades que asignamos
 * @param permisoRead para leer el permiso
 * @param permisoWrite para escribir los permisos
 * @param tam determina el tamaño
 */
	void asignarPropiedadesPermisosYTamanyoDatos( uint8_t props,
												 SecureMode_t permisoRead,
												 SecureMode_t permisoWrite, 
												 uint8_t tam ) {
	  asignarPropiedades( props );
	  asignarPermisos( permisoRead, permisoWrite );
	  asignarTamanyoDatos( tam );
	} // ()
												 

	/**
  * char->escribirDatos()->N
   *Metodo escribirDatos 
   *
   *@param str para escribir los datos que deseemos
   *
   *@return devuelve los datos escritos
	 */
	uint16_t escribirDatos( const char * str ) {
	  // Serial.print( " return (*this).laCaracteristica.write( str  = " );
	  // Serial.println( str );

	  uint16_t r = (*this).laCaracteristica.write( str );

	  // Serial.print( ">>>Escritos " ); Serial.print( r ); Serial.println( " bytes con write() " );

	  return r;
	} // ()

 /**
  * char->notificarDatos()->R
  */
	uint16_t notificarDatos( const char * str ) {
	  
	  uint16_t r = laCaracteristica.notify( &str[0] );

	  return r;
	} //  ()

  /**
   * 
   * CallbackCaracteristicaEscrita->instalarCallbackCaracteristicaEscrita()
   * Metodo instalarCallbackCaracteristicaEscrita
   * 
   * @param cb variable utilizada para el callback
   */
	void instalarCallbackCaracteristicaEscrita( CallbackCaracteristicaEscrita cb ) {
	  (*this).laCaracteristica.setWriteCallback( cb );
	} // ()

	/**
  * activar()
  * Metodo activar que activa la caracteristica
	 */
	void activar() {
	  err_t error = (*this).laCaracteristica.begin();
	  Globales::elPuerto.escribir(  " (*this).laCaracteristica.begin(); error = " );
	  Globales::elPuerto.escribir(  error );
	} // ()

  }; // class Caracteristica
  
  // --------------------------------------------------------
  // --------------------------------------------------------
private:
  
  uint8_t uuidServicio[16] = { // el uuid se copia aquí (al revés) a partir de un string-c
	// least signficant byte, el primero
	'0', '1', '2', '3', 
	'4', '5', '6', '7', 
	'8', '9', 'A', 'B', 
	'C', 'D', 'E', 'F'
  };

  //
  //
  //
  BLEService elServicio;

  //
  //
  //
  std::vector< Caracteristica * > lasCaracteristicas;

public:
  /**
   * char->ServicioEnEmisora()
   * Constructor de Servicio en emisora 
   * 
   * @param nombreServicio_ que nos indica el nombre del servicio
   */
  // .........................................................
  // .........................................................
  ServicioEnEmisora( const char * nombreServicio_ )
	:
	elServicio( stringAUint8AlReves( nombreServicio_, &uuidServicio[0], 16 ) )
  {
	
  } // ()
  
  
  /**
   * 
   * escibeUUID()
   * Metodo escribeUUID que escribe la UUID del servicio por pantalla
   */
  void escribeUUID() {
	Serial.println ( "**********" );
	for (int i=0; i<= 15; i++) {
	  Serial.print( (char) uuidServicio[i] );
	}
	Serial.println ( "\n**********" );
  } // ()

  /**
   * Caracteristica->anyadirCaracteristica()
   * Metodo anyadirCaracteristica que añade la caracteristica
   * 
   * @param car para añadir la caracteristica
   */
  void anyadirCaracteristica( Caracteristica & car ) {
	(*this).lasCaracteristicas.push_back( & car );
  } // ()

 /**
  * activarServicio()
  * Metodo activarServicio que activa finalmente nuestro servicio  
  * 
  */
  void activarServicio( ) {
	// entiendo que al llegar aquí ya ha sido configurado
	// todo: características y servicio

	err_t error = (*this).elServicio.begin();
	Serial.print( " (*this).elServicio.begin(); error = " );
	Serial.println( error );

	for( auto pCar : (*this).lasCaracteristicas ) {
	  (*pCar).activar();
	} // for

  } // ()

  /**
   * Metodo para la conexión del servicio BLE
   * 
   * @return nos devuelve el servicio
   */
  operator BLEService&() {
	// "conversión de tipo": si pongo esta clase en un sitio donde necesitan un BLEService
	return elServicio;
  } // ()
	
}; // class

#endif
