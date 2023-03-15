#include <avr/io.h>


// DDRx : configure la direction de donnee (I/O)
// PORTx : ecrit valeur du pn port en mode output. configure le pin port en input
// PINx : lit les donnes du pin port en mode input

int main(void)
{
    // PB0 est output
    DDRB |= 0b00000001;

    // PB0 est alimente
    PORTB |= 0b00000001;
    return 0;
}