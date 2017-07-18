//  Gimbal Shake Mechanism using Arduino Esplora Board
//  Benjamin Glass
//  WINDY FILMS
//  June 5 - July 27,2017


// Include Freefly API
#include <FreeflyAPI.h>

// Include Esplora Library
#include <Esplora.h>

//Output ports are configured as MōVI communication port
#include <SoftwareSerial.h>
SoftwareSerial ArduinoToMoviCommunication(3, 11); //RX and TX on digital output pins

//Mapping function definition to make calculations easier.
float map_f(float x, float in_min, float in_max, float out_min, float out_max);

//Variable to remember kill state
bool kill = false;

//Sin function increments
float incr1 = 1;
float incr2 = 30.5;


//Setup code
void setup() {
    //Initialize serial communication
    ArduinoToMoviCommunication.begin(111111); //111111 is the baudrate

    //Initialize Freefly API
    FreeflyAPI.begin();


    //set pan and tilt to RATE, defer everything else
    FreeflyAPI.control.pan.type = RATE;
    FreeflyAPI.control.tilt.type = RATE;

    FreeflyAPI.control.focus.type = DEFER;
    FreeflyAPI.control.iris.type = DEFER;
    FreeflyAPI.control.zoom.type = DEFER;

    //Serial monitor for testing
    Serial.begin(9600);
}


//Loop code
void loop() {


    // Read Joystick's X and Y axis
    float joystickX =  (float)Esplora.readJoystickX() / 512; //Divide by the range to get -1 to 1.
    float joystickY =  (float)Esplora.readJoystickY() / 512;

    //Read the slider with range 0 <-> 1
    float sliderValue = map_f(Esplora.readSlider(), 0, 1023, 0, 1); //Use the mapping function to map slider range to user range.

    float pi = 3.14;

    float shake_num_pan = (sin((incr1/60)*2*pi)/2);     //const. changing increment value added to create shake
    float shake_num_tilt = (sin(((incr2/60)*2*pi))/2);  // divide by 10 can be changed to control shake amplitude

    //Joystick centering
    if (-joystickX <= .4 && -joystickX >= 0) {
      joystickX = 0;
    } else if (-joystickX > .4) {
      joystickX = -.5;
    } else {
      joystickX = .5;
    }

    if (joystickY <= .4 && joystickY >= 0) {
      joystickY = 0;
    } else if (joystickY > .4) {
      joystickY = .5;
    } else {
      joystickY = -.5;
    }

    // Set MōVI Pro pan rate to joystick X
    FreeflyAPI.control.pan.value = -(joystickX) + shake_num_pan;

    // Set MōVI Pro tilt rate to joystick Y
    FreeflyAPI.control.tilt.value = (joystickY) + shake_num_tilt;


    //reset and increment incr1 and incr2 after the increments get too high
    if (incr1 == 100000) {
      incr1 = 1;
      incr2 = 30.5;
    } else {
      incr1 += 14.75;
      incr2 += 14.75;
    }




    delay(100*sliderValue); //affects how fast the shake is




    /****************************  BUTTONS  **********************************/

    //LEFT BUTTON
    if (Esplora.readButton(SWITCH_LEFT) == LOW) { //If Left Button Pressed

    FreeflyAPI.control.pan.value = -.5;

    }

    //RIGHT BUTTON
    if (Esplora.readButton(SWITCH_RIGHT) == LOW) {  //If Up Button Pressed

    FreeflyAPI.control.pan.value = .5;
        }


    //KILL - DOWN BUTTON
    if (Esplora.readButton(SWITCH_DOWN) == LOW) { //If Down Button Pressed
        //Toggle
        kill = !kill; //We are setting kill value to be the inverse of its current value
    }

    FreeflyAPI.control.gimbal_kill = kill; //Now set it to actual command



    //START-STOP RECORD - UP BUTTON
    if (Esplora.readButton(SWITCH_UP) == LOW) { //If Right Button Pressed
        FreeflyAPI.control.fiz_record_button_flag = true; //Set FIZ record button also on
    } else {
        FreeflyAPI.control.fiz_record_button_flag = false; //Set FIZ record button off
    }




    // Create the message to be sent to MōVI Pro
    FreeflyAPI.send();

    /* In a loop send each byte of the message
     that the Freefly API created through
     the Serial port
     */
    while (true) {

        //Create temporary value to hold the message
        uint8_t message;

        /* Remove single byte from the message API created
         and exit this loop if there is no more messages
         left to be sent this round.
         */
        if (BufRemove(1, &message) == 0) break;

        //Now physically send one byte of message on Serial port
        ArduinoToMoviCommunication.write(message);
    }

}

//Re-maps a number from one range to another to make calculations easier.
float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
