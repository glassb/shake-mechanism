//  Gimbal Shake Mechanism using Arduino Esplora Board : Version 2
//  Benjamin Glass
//  WINDY FILMS
//  June 5 - July 28,2017


// Include Freefly API
#include <FreeflyAPI.h>

// Include Esplora https://www.arduino.cc/en/Reference/EsploraLibrary
#include <Esplora.h>

//Output ports are configured as MōVI communication port
#include <SoftwareSerial.h>
SoftwareSerial ArduinoToMoviCommunication(3, 11); //RX and TX on digital output pins

//Mapping function definition to make calculations easier.
float map_f(float x, float in_min, float in_max, float out_min, float out_max);

//Variable to remember kill state
bool kill = false;
float pi = 3.14;
int incr = 0;

float panArray[] =
 {-22.07018421,-16.15638394,-10.83609036,-6.46622977,-3.958799147,-5.815387122,-4.363835408,-2.146881026,-1.314005373,0.811940153,4.655558413,9.061392055,12.88326569,13.56206135,16.64974219,17.57118405,16.00482296,11.40762338,5.826007531,6.435762852,-0.139795614,-6.523133218,-9.031179578,-5.172569578,-1.093648303,1.8125695,0.852075837,-4.164169585,-11.8596602,-19.79333922,-24.91508654,-26.30667643,-24.72197003,-21.38066126,-17.61910567,-13.5473088,-8.530208444,-8.582185886,-1.814940901,6.065859964,12.18653694,15.9743306,16.79666216,14.62714596,11.13923271,7.90510518,6.055524524,5.47564072,5.458705031,5.66130713,6.320576899,7.861605731,10.76940616,13.59969841,13.7437432,15.44125904,15.70778721,14.25527877,13.22399423,12.23265712,10.22868608,7.541074802,2.96981814,-0.940194239,-3.220855852,-3.164278112,-2.511791779,-2.844230606,-5.400056117,-6.885362115,-11.66259986,-16.70937832,-22.33830516,-29.57598313,-37.12553593,-43.46616797,-47.12531571,-47.73118824,-45.21838888,-39.65925553,-31.61993216,-21.0708169,-9.063656569,-7.463215507,4.856201296,24.18939719,29.80957121,30.43742755,27.44421598,23.20681797,19.01482262,15.39265551,12.53448657,9.778990135,5.185678445,-2.214633869,-12.21580288,-21.96098781};
float tiltArray[] =
 {-7.295110655,-6.873703592,-5.820038334,-3.227283083,0.203370508,-0.606849597,2.65796878,7.147295042,7.869386581,8.707819248,9.499001977,10.52478039,10.66689144,11.22625194,11.76913987,11.90061522,9.623266436,5.678934873,0.044359436,0.208891908,-4.884739526,-7.845779436,-5.36472428,-2.212048732,0.533814529,1.774230147,-0.170506783,-3.891402512,-8.100445481,-12.51212621,-14.59856704,-14.45360626,-12.28289417,-8.853594225,-5.257497268,-1.991981737,1.219841435,2.08191176,4.331490731,5.875216643,6.743149318,7.779894193,8.289038012,9.106082248,9.994426289,11.69870519,14.05442725,16.71083375,19.785192,21.11574627,21.27610591,19.27118832,16.36205464,14.73371652,14.38267796,13.4529362,12.17070045,10.914942,10.10122074,9.899397237,9.076663653,8.210374665,7.485871035,5.603172183,3.340464033,0.855444646,-1.942848962,-5.299897516,-5.936303112,-5.815035996,-4.126389998,-0.13516476,2.484780189,1.665822528,-2.182156872,-7.107337458,-11.96348278,-15.93533481,-18.18350923,-18.49734326,-17.35861798,-15.37752129,-12.24230557,-12.24403577,-8.865949934,-1.988322866,0.621245924,3.171555149,4.935424412,6.925955832,9.796068281,12.10341068,12.52262956,9.996945275,5.607645294,0.262772579,-3.836280144,-6.938515085};
float rollArray[] =

 {0.771697588,-1.616827156,-3.327054919,-4.372852826,-5.554899443,-5.309184668,-7.195053831,-14.10486276,-17.61179811,-20.87857568,-23.97986409,-26.62503952,-29.43037099,-29.06577576,-31.05440393,-32.23865909,-32.39637251,-31.95509861,-30.56843471,-30.15932737,-27.8832101,-22.67050991,-8.528020248,-5.417704235,-4.224523228,-2.313240369,-1.162235719,-2.30492521,-3.905381555,-4.444585172,-4.378440389,-3.694377773,-1.924646149,0.70902847,4.099480199,6.678220765,8.20960627,8.537790824,8.823473357,7.871834323,6.336194567,5.425805667,5.189500175,5.402056052,5.652315165,6.342387688,7.366929497,8.356475353,9.365430044,10.33874756,10.05607762,9.336454167,9.241898387,9.52433933,9.152075217,9.450851373,8.864438559,8.527160234,8.326166197,9.395561076,11.4169207,13.81406082,16.27846531,19.53535525,21.71254541,22.08905364,20.7062013,19.00814604,17.09661891,17.68939278,15.49293116,13.47416685,11.38453023,9.24603054,5.848948045,1.326539196,-2.855787371,-6.351659567,-8.71909102,-9.72565904,-8.40036662,-5.714816325,-4.144200946,-4.161503015,-4.689531524,-9.673574733,-11.58528507,-12.5909015,-13.56200536,-15.71644839,-18.48832075,-21.98390797,-26.23611962,-29.26251552,-29.3768669,-26.49425623,-21.73299238,-17.3833803};

//Setup code
void setup() {
    //Initialize serial communication
    ArduinoToMoviCommunication.begin(111111); //111111 is the baudrate

    //Initialize Freefly API
    FreeflyAPI.begin();

    //Controlling on pan and tilt
    FreeflyAPI.control.pan.type = RATE;
    FreeflyAPI.control.tilt.type = RATE;
    FreeflyAPI.control.roll.type = RATE;

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


    //Joystick centering *****************
    if (-joystickX <= .04 && -joystickX >= 0) {
      joystickX = 0;
    } else if (-joystickX > .04) {
      joystickX = -.5;
    } else {
      joystickX = .5;
    }

    if (joystickY <= .02 && joystickY >= 0) {
      joystickY = 0;
    } else if (joystickY > .04) {
      joystickY = .5;
    } else {
      joystickY = -.5;
    }
    //**********************************

    Serial.print("Incr: ");
    Serial.print(incr);
    Serial.print("\n\n");

    // Set MōVI Pro pan rate to joystick X
    FreeflyAPI.control.pan.value = -(joystickX) + panArray[incr];

    // Set MōVI Pro tilt rate to joystick Y
    FreeflyAPI.control.tilt.value = (joystickY) + tiltArray[incr];

    FreeflyAPI.control.roll.value = rollArray[incr];


    Serial.print("Pan: ");
    Serial.print(FreeflyAPI.control.pan.value);

    Serial.print(" ");

    Serial.print("Tilt: ");
    Serial.print(FreeflyAPI.control.tilt.value);
    Serial.print("\n\n");


    if ( incr >= (sizeof(panArray)-1) || incr >= (sizeof(tiltArray)-1) || incr >= (sizeof(rollArray)-1) )  {
        incr = 0;

    } else {
        incr+=1;
    }

    //Delay so that we only send about 50 updates per second.
    delay(100*sliderValue);



}

//Re-maps a number from one range to another to make calculations easier.
float map_f(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
