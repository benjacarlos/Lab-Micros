#include "EncoderHandler.h"
#include "encoder.h"
#include "AdminID.h"


//Limites
#define MAX_NUM '9'
#define MIN_NUM '0'

//Variables
static char Number;
static char Numero_entero[TAMANO_ID];
static char Pointer;

//Funciones internas
void UpdateNumber(encoderUd_t Event);
char GetNumber(void);
void ResetNumber(void);


//Funciones interfaz
void InitNumber(void){
	for(char i=0; i< TAMANO_ID; i++ ){
		Numero_entero[i]=0;
	}
	Pointer=0;
}
//Vamos a recibir true cuando el número completo esté formado
bool GettingTotalNumber(void){
	bool TaskEnd=false;
	if( (IsPin != false) & (Pointer < PIN_MAXIMO) ){
		encoderUd_t Event=pullEncoderEvent();
		if (Event.isValid != false){
			switch(Event.input){
				case UP : case DOWN:
					UpdateNumber(Event);
					break;
				case ENTER:
					Numero_entero[Pointer]=GetNumber();
					Pointer++;
					if (Pointer == PIN_MAXIMO){
						TaskEnd=true;
						Pointer=0;
					}
					break;
				case BACK:
					if (Pointer > 0){
						Pointer--;
					}
					break;
				default :
					//Asumo que este se detona si se quiere tener el pin de minimo tamaño
					if (Pointer == PIN_MINIMO){
						TaskEnd=true;
					}
					break;
			}
		}
	}
	else if ((IsID != false) & (Pointer < TAMANO_ID)){
		encoderUd_t Event=pullEncoderEvent();
		if (Event.isValid != false){
			switch(Event.input){
				case UP : case DOWN:
					UpdateNumber(Event);
					break;
				case ENTER:
					Numero_entero[Pointer]=GetNumber();
					Pointer++;
					if (Pointer == PIN_MAXIMO){
						TaskEnd=true;
						Pointer=0;
					}
					break;
				case BACK:
					if (Pointer > 0){
						Pointer--;
					}
					break;
			}
		}
	}
	//Como default voy a asumir que estamos algo que solo requiere un mnumero
	else {
		Pointer=0;
		encoderUd_t Event=pullEncoderEvent();
		if (Event.isValid != false){
			switch(Event.input){
			case UP : case DOWN:
				UpdateNumber(Event);
				break;
			case ENTER:
				Numero_entero[Pointer]=GetNumber();
				TaskEnd=true;
				break;
			}
		}
	}
	return TaskEnd;
}
//Para obtener el numero completo
char * GetTotalNum(void){
	return Numero_entero;
}


//Funciones internas
void UpdateNumber (encoderUd_t Event){
	if ( (Event.isValid) != false){
		if ( (Event.input == UP) & (Number < MAX_NUM) ){
			Number++;
		}
		//Si ya estoy en el maximo doy una vuelta completa
		else if ( (Event.input == UP) & (Number == MAX_NUM) ){
			Number=MIN_NUM;
		}
		else if( (Event.input == DOWN) & (Number > MIN_NUM) ){
			Number--;
		}
		//Si ya estoy en el minimo doy una vuelta completa
		else if( (Event.input == DOWN) & (Number == MIN_NUM) ){
			Number=MAX_NUM;
		}
	}
}

char GetNumber(void) {
	return Number;
}

void ResetNumber(void){
	Number = MIN_NUM;
}
