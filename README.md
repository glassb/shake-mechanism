# Shake Mechanism - For Freefly MōVI Pro by Arduino Microcontroller

An Arduino Esplora microcontroller sends commands to a MōVI Pro that change the rotational velocity of the MōVI gimbal motors. The result is jerky camera motion. This is an inexpensive and easy setup for cinematographers that desire camera frame shake. 

***Notice*: Windy Films treats this project as a proof of concept.** Although the mechanism is functional and expandable, the hardware limitations and lack of time for production lead us to look to others for further development. By making the software available and presenting the hardware configuration, our hope is that this mechanism appears as a widely useful feature that can be integrated easily into an existing environment, including the Freefly MōVI Pro. As a motion feature incorporated into the MōVI environment, the shake mechanism would be (a) developed more extensively, and (b) the mechanism would be more easily operated.

The Arduino code and full documentation are available here.

# Links
- **Highlight Video:** https://www.youtube.com/watch?v=TiPTbAjHiWw
- **Windy Films Blogpost:** https://windyfilms.com/blog/2017/7/25/movi
- **Download the Freefly API here:** http://freeflysystems.com/support/movi-pro-support and install the API files as a library in the Arduino Web Editor. Include the API library in the Arduino code file. 

<br/>
<br/>

**July 27, 2017 Update:** We have begun to process a second version of the shake mechanism, that relies on real motion data rather than arithmetic functions. Our hope is that data collectection will make the shake mechanism more realistic.

**Note on the file *rotationrates_data.csv*:** This file contains motion data collected from three different types of movement: static (standing upright), walking (about 2.5 meters/second), and running (about 7 meters/second). In all three movements, data was recorded using SensorLog mobile application for 10 seconds at 100Hz. The phone was attached directly to a Canon 1D DSLR for realistic camera movement. The data recorded the rotation speed of the phone in the X, Y, and Z directions. The units of measurement are radians/second.

The file ***rotationrates_exceldata.xslx*** houses all three different movement's data, as well as a hybrid data, that averages all three movements together to create and somewhat intermediate "shake". Spreadsheets include graphs.





