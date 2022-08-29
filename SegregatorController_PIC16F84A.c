#define stop 0x00
#define push 0x01
#define pull 0x02
#define panLeft 0x04
#define panRight 0x08

void func_DropBall() {
     PORTB = push;
     Delay_ms(1000);
     PORTB = pull;
     Delay_ms(500);
     PORTB = stop;
}

void main() {
     PORTA = 0x00;
     PORTB = 0x00;
     
     TRISA = 0x07;
     TRISB = 0x00;
     
     PORTB = 0x10;
     Delay_ms(200);
     PORTB = 0x20;
     Delay_ms(200);
     PORTB = 0x40;
     Delay_ms(200);
     PORTB = 0x80;
     Delay_ms(200);
     
     while(1) {
       if (PORTB == 0x80) {
          PORTB = 0x00;
          Delay_ms(50);
       }
       
       else {
            PORTB = 0x80;
            Delay_ms(50);
       }
     
		// Check the color of the ball
		// 0x01 = Red
		// 0x03 = Yellow
		// 0x07 = Green
       if ((PORTA == 0x01) || (PORTA == 0x03) || (PORTA == 0x07)) {
          PORTB = 0x30;
          Delay_ms(2000);
          
           if (PORTA == 0x01) {
              func_DropBall();
              PORTB = panLeft;
              Delay_ms(50);
              PORTB = stop;
           }
           
           else if (PORTA == 0x03) {
                PORTB = panRight;
                Delay_ms(175);
                func_DropBall();
                PORTB = panLeft;
                Delay_ms(275);
                PORTB = stop;
           }

           else {
                PORTB = panRight;
                Delay_ms(300);
                func_DropBall();
                PORTB = panLeft;
                Delay_ms(500);
                PORTB = stop;
           }
       }
    }
}