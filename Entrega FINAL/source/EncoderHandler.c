#include "EncoderHandler.h"
#include "encoderEvent.h"

//Variables
static char Number;


void UpdateNumber (void){
	counter_type Event=encoderRot();
	if ( (Event == COUNT_UP) & (Number < MAX_NUM) ){
		Number++;
	}
	else if( (Event == COUNT_DOWN) & (Number > MIN_NUM)){
		Number--;
	}
}

char GetNumber(void) {
	return Number;
}

void ResetNumber (void){
	Number = MIN_NUM;
}
