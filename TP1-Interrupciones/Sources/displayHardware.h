//
// Funciones que se comunica con el hardware del display
// En este caso display de segmentos
//


#ifdef DISPLAYHARDWARE_H_
#define DISPLAYHARDWARE_H_

// Seteo IO del display con el K64
void InitHardwareDisplay(void);
//Imprime el caracter c en la posicion que recibe.
void PrintChar(const char c,unsigned int pos);

#endif /* SEGMENTDISPLAY_H_ */
