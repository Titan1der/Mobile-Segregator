//LEAST SIGNIFICANT BIT == LEFT MOTOR
//MOST SIGNIFICANT BIT == RIGHT MOTOR
//PORTB is used for motors
//PORTC is used for inputs
//PORTD is used as a status indicator

//PORTB DEFINITION:
#define stop            0x00    //0000 0000
#define moveForward     0x05    //0000 0101
#define moveBackward    0x0A    //0000 1010
#define turnLeft        0x04    //0000 0100
#define turnRight       0x01    //0000 0001
#define backingLeft     0x08    //0000 1000
#define backingRight    0x02    //0000 0010
#define hardLeft        0x06    //0000 0110
#define hardRight       0x09    //0000 1001

#define openArm         0x10    //0001 0000
#define closeArm        0x20    //0010 0000
#define lowerArmRight   0x40    //0100 0000
#define raiseArmRight   0x80    //1000 0000

#define farLeft         0x1E    //0001 1110
#define wideLeft        0x1C    //0001 1100
#define left            0x1D    //0001 1101
#define midLeft         0x19    //0001 1001
#define center          0x1B    //0001 1011
#define midRight        0x13    //0001 0011
#define right           0x17    //0001 0111
#define wideRight       0x07    //0000 0111
#define farRight        0x0F    //0000 1111

//PORTC DEFINITION
//PINS 0-4                      >> USED FOR LINE FOLLOWER
//PIN 5                         >> USED FOR MICRO SWITCH

//PORTD DEFINITION:
#define lowerBridge     0x80    //1000 0000
#define raiseBridge     0x40    //0100 0000
#define lowerArmLeft    0x10    //0001 0000
#define raiseArmLeft    0x20    //0010 0000
//PINS 0-3                      >> USED AS INDICATORS

unsigned char nextStep = 0x00, checkPoint = 0x00;
unsigned char skipCode1 = 0x00, skipCode2 = 0x00, skipCode3 = 0x00;
unsigned char isHome = 0x00;
int lineStatus = 0, counter = 0, isOpenArm = 0, lines = 0;

int hasReachedThirdCheckpoint() {
    //IF MOBOT REACHED THIRD HORIZONTAL LINE
    if (checkPoint == 0x03) {
       return 1;
    }

    return 0;
}

int hasReachedEnd() {
    if (PORTC.F5 == 1) {
       return 1;
    }

    return 0;
}

void func_DropBalls() {
     if (skipCode3 == 0x00) {
        PORTB = stop;
        Delay_ms(1000);

        PORTB = hardRight;
        Delay_ms(2000);
     }

     skipCode3 = 0x01;
     //IF MOBOT DETECTS BLACK LINE IN THE MIDDLE
     if ((PORTC == center) || (PORTC == midRight) || (PORTC == midLeft)) {
        if (lines == 2) {
        PORTB = hardLeft;
        Delay_ms(3000);
        PORTB = stop;
        Delay_ms(2000);
        PORTD = 0x01;
        PORTB = stop;
        nextStep = 0x06;
        }
        
        else {
             skipCode3 = 0x00;
             lines++;
        }
     }
}

void func_GoHome() {
     if (skipCode2 == 0x00) {
       //STOP ROBOT FOR 2 SECONDS
       PORTB = stop;
       PORTD = 0x0F;
       Delay_ms(2000);
       PORTD = 0x00;

       //MOVE BACKWARDS THEN TURN RIGHT
       PORTB = moveBackward;
       Delay_ms(1000);

       PORTB = stop;
       Delay_ms(1000);

       PORTB = hardRight;
       Delay_ms(3000);
     }

     skipCode2 = 0x01;

     //IF MOBOT DETECTS BLACK LINE IN THE MIDDLE
     //NEXT STEP IS func_LineFollow
     //VARIABLE "skipCode2" IS SET TO 0
     if ((PORTC == center) || (PORTC == midRight) || (PORTC == midLeft)) {
        nextStep = 0x00;
        skipCode2 = 0x00;
     }
}

void func_OpenArm() {
     //OPEN ARM
     PORTB = openArm;
     Delay_ms(5000);
     PORTB = lowerArmRight;
     PORTD = lowerArmLeft;
     Delay_ms(750);
     PORTB = stop;
     PORTD = stop;
}

void func_CloseArm() {
     //CLOSE ARM
     PORTB = closeArm;
     Delay_ms(6000);
     PORTD = hardLeft;
     Delay_ms(500);
     PORTD = hardRight;
     Delay_ms(1000);
     PORTD = hardLeft;
     Delay_ms(500);
     PORTB = raiseArmRight;
     PORTD = raiseArmLeft;
     Delay_ms(750);
     PORTB = stop;
     PORTD = stop;
}

void func_CheckForCheckpoints() {
    //INCREMENT CHECKPOINT AND DISABLE CONDITION BELOW
    if (skipCode1 == 0x00) {
       if ((PORTC == 0x00) || (PORTC == 0x10) || (PORTC == 0x01)) {
          checkPoint++;
          skipCode1 = 0x01;
          Delay_ms(1000);

          //STATUS INDICATOR
          lineStatus++;
          if (lineStatus == 1) {
             PORTD = 0x01;
          }

          else if (lineStatus == 2) {
               PORTD = 0x03;
          }

          else if (lineStatus == 3) {
               PORTD = 0x07;
          }
       }
    }

    //ENABLE THE CONDITION ABOVE WHEN MOBOT HAS FINISHED A CHECKPOINT
    if ((PORTC == center) || (PORTC == midRight) || (PORTC == midLeft)) {
       skipCode1 = 0x00;
    }

    nextStep = 0x00;
}

void func_LineFollow() {
     //LINE FOLLOW CODE
     if (checkpoint == 0x04) {
        counter++;
        Delay_ms(10);

        //INDICATES HOW MANY SECONDS LEFT UNTIL ARM CLOSES
        if (counter == 1) {
           PORTD = 0x01;
        }
        else if (counter == 100) {
           PORTD = 0x03;
        }
        else if (counter == 200) {
             PORTD = 0x06;
        }

        if (counter == 300) {
           //IF COUNTER HAS REACH 3 SECONDS.. STOP FOR 8 SECONDS WITH STYLE
           PORTB = stop;
           func_CloseArm();
           PORTD = 0x08;
           Delay_ms(1000);
           PORTD = 0x04;
           Delay_ms(1000);
           PORTD = 0x02;
           Delay_ms(1000);
           PORTD = 0x01;
           Delay_ms(1000);
           PORTD = 0x00;
           counter = 0;
           isOpenArm = 0;
        }
     }

     if ((isOpenArm == 0) && (checkpoint == 0x04)) {
        func_OpenArm();
        isOpenArm = 1;
     }

     if (PORTC == right) {
          PORTB = hardLeft;
     }

     else if (PORTC == left) {
          PORTB = hardRight;
     }

     else if (PORTC == midRight) {
          PORTB = hardLeft;
     }

     else if (PORTC == midLeft) {
          PORTB = hardRight;
     }

     else if (PORTC == wideRight) {
          PORTB = hardLeft;
     }

     else if (PORTC == wideLeft) {
          PORTB = hardRight;
     }

     else if (PORTC == farRight) {
          PORTB = hardLeft;
     }

     else if (PORTC == farLeft) {
          PORTB = hardRight;
     }

     else if (PORTC == center) {
          PORTB = moveForward;
     }

     else {
          PORTB = moveForward;
     }

     nextStep = 0x01;

     //IF HAS REACHED THIRD CHECKPOINT
     //NEXT STEP IS func_OpenArm
     //CHECKPOINT IS SET TO 0x04
     //VARIABLE "skipCode1" IS SET TO 0
     if (hasReachedThirdCheckpoint() == 1) {
        if (isHome == 0x00) {
          nextStep = 0x00;
          checkPoint = 0x04;
          skipCode1 = 0x00;
          PORTB = moveForward;
          Delay_ms(500);
          PORTB = stop;
        }

        else {
            nextStep = 0x05;
        }

     }

     //IF HAS REACHED END OF ARENA
     //NEXT STEP IS func_CloseArm
     //CHECKPOINT IS SET TO ZERO
     //VARIABLE "skipCode1" IS SET TO 0
     if (hasReachedEnd() == 1) {
        nextStep = 0x03;
        checkPoint = 0x00;
        skipCode1 = 0x00;
        lineStatus = 0;
     }
}

void main() {
     ADCON1 = 0x06;
     CMCON = 0x07;

     PORTB = 0x00;
     PORTC = 0x3F;
     PORTD = 0x00;

     TRISB = 0x00;
     TRISC = 0x3F;
     TRISD = 0x00;

     PORTD = 0x01;
     Delay_ms(1250);
     PORTD = 0x02;
     Delay_ms(1250);
     PORTD = 0x04;
     Delay_ms(1250);
     PORTD = 0x08;
     Delay_ms(1250);

     PORTD = 0x00;

     //INITIALIZE
     checkPoint = 0x00;
     skipCode1 = 0x00;
     skipCode2 = 0x00;
     skipCode3 = 0x00;
     isOpenArm = 0;
     isHome = 0x00;

     nextStep = 0x00;

     while(1) {
        if (nextStep == 0x00) {
           //BASIC LINE FOLLOW
           func_LineFollow();
        }

        else if (nextStep == 0x01) {
             //CHECK FOR CHECKPOINTS
             func_CheckForCheckpoints();
        }

        else if (nextStep == 0x02) {
             //OPEN ARM AND LINE FOLLOW
             func_OpenArm();
             nextStep = 0x00;
        }

        else if (nextStep == 0x03) {
             //CLOSE ARM FUNCTION
             PORTB = stop;
             Delay_ms(2000);
             PORTB = moveBackward;
             Delay_ms(1000);
             func_CloseArm();
             nextStep = 0x04;
        }

        else if (nextStep == 0x04) {
             //GO HOME FUNCTION
             func_GoHome();
             isHome = 0x01;
        }

        else if (nextStep == 0x05) {
             //DROP BALLS FUNCTION
             func_DropBalls();
        }

        else if (nextStep == 0x06) {
             PORTD = lowerBridge;
             Delay_ms(3000);
             PORTD = stop;
             Delay_ms(5000);

             PORTB = moveForward;
             Delay_ms(500);
             PORTB = stop;
             PORTB = moveBackward;
             Delay_ms(500);
             PORTB = stop;
             Delay_ms(3000);

             PORTD = lowerBridge;
             Delay_ms(8000);
             PORTD = 0x03;

             PORTB = moveForward;
             Delay_ms(500);
             PORTB = stop;
             PORTB = moveBackward;
             Delay_ms(500);
             PORTB = stop;
             Delay_ms(3000);

             PORTD = raiseBridge;
             Delay_ms(10000);
             PORTD = 0x0F;
             nextStep = 0x07;
        }

        else if (nextStep == 0x07) {
             //DO NOTHING -- END OF PROGRAM
        }
    }
}