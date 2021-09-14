# ETC (Electronic throttle control)

## FUNCIONES
 - Autocalibrar el embrague
 - Mandar mensaje CAN a todas las placas ???
 - Controlar embrague (mensaje CAN del volante + mensaje CAN cambio/volante, golpe de gas)
 - Desconecto M+, M-; si en 1s TPS no idle+-5%  => abrir SW
 	Vuelvo a cerrar SW tras 1s afer TPS en idle+-5%
 - Si TPS1 y TPS2 tienen 		diferencia >10% 		durante 100ms  =>  desconecto M+, M-
 - Si APPS1 y APPS2 tienen 		diferencia >10% 		durante 100ms  =>  desconecto M+, M-
 - Si TPS y target TPS tienen		diferencia >10% 		durante 500ms  =>  desconecto M+, M-
 			

## CONECTOR
  M+			OUT	Power motor mariposa
  M-			OUT	Power motor mariposa
  ECUM+		IN	Señal de la ECU a power motor mariposa
  VIN	
  PWMOUT2		OUT	-
  ENTRADA1		IN	Fin de carrera embrague HIGH=GND
  CHASSISGROUND
  CANH		COM	CAN
  TPS2		IN	Sensor TPS
  TPS1		IN	Sensor TPS
  VCC
  ECUAPPS1		OUT	Señal APPS que ETC manda a ECU
  ECUAPPS2		OUT	Señal APPS que ETC manda a ECU
  ECUM-		IN	Señal de la ECU a power motor mariposa
  SDCOUT		OUT	Shutdown circuit
  SDCIN		IN	Shutdown circuit
  STARTER		OUT	- (relé 12V)
  PWMOUT1		OUT	Control del embrague
  ENTRADA2		IN	Fin de carrera embrague HIGH=GND
  CHASSISGROUND
  CANL		COM	CAN
  GND
  GND
  VCC
  APPS1		IN	Sensor APPS
  APPS2		IN	Senosr APPS
 
## Motor Mariposa
 	TPS1	M+	GND
 	TPS2	M-	5V

## HALTECH (Drive By Wire)
  A16		TPS1		TPS1 que también va a ETC
  A17		TPS2		TPS2 que también va a ETC
  A2		ECUAPPS1	ECUAPPS1 que viene de ETC
  B20		ECUAPPS2	ECUAPPS2 que viene de ETC
  B25		ECUM-		Señal de la ECU a power motor mariposa que va a ETC
  B26		ECUM+		Señal de la ECU a power motor mariposa que va a ETC

### ETC modo manual
	APPSMODE = 0 	=>	APPS1 <-> ECUAPPS1
			        APPS2 <-> ECUAPPS2
### Golpe de gas / DV
	
	 CAN			    i2c
NVIDIA ------>  MICROCONTROLADOR ------> DAC ------> DACAPPS1 <-> ECUAPPS1
			        		       DACAPPS2 <-> ECUAPPS2
			          ------> APPSMODE = 1
			         

 
 
 
