/*	SparkFun Inventor's Kit for Photon
    Experiment 3 - Part 2: Internet Houseplant Monitor 
    This sketch was written by SparkFun Electronics
    Joel Bartlett <joel@sparkfun.com>
    August 31, 2015
    https://github.com/sparkfun/Inventors_Kit_For_Photon_Experiments

    This application monitors the moisture level of your houseplant 
    and exposes that data to be monitored via the Internet. 
    Development environment specifics:
    Particle Build environment (https://www.particle.io/build)
    Particle Photon RedBoard
    Released under the MIT License(http://opensource.org/licenses/MIT)
*/
int val = 0;//variable to store soil value
int soil = A2;//Declare a variable for the soil moisture sensor 
int soilPower = D6;//Variable for Soil moisture Power
//Rather than powering the sensor through the V-USB or 3.3V pins, 
//we'll use a digital pin to power the sensor. This will 
//prevent oxidation of the sensor as it sits in the corrosive soil. 

// UDP setup

UDP Udp;
unsigned int localPort = 5212;  //reserved for incoming traffic
char UDPinData[64];
char UDPoutData[1024]; 

void setup() 
{
Serial.begin(9600);   // open serial over USB

pinMode(soilPower, OUTPUT);//Set D7 as an OUTPUT
digitalWrite(soilPower, LOW);//Set to LOW so no power is flowing through the sensor

//This line creates a variable that is exposed through the cloud.
//You can request its value using a variety of methods
Spark.variable("soil", &val, INT);

// Print your device IP Address via serial
Serial.println(WiFi.localIP());

// Start UDP
Udp.begin(localPort);
memset(&UDPoutData[0], 0, sizeof (UDPoutData));
}

void loop() 
{
//Serial.print("Soil Moisture = ");    
//get soil moisture value from the function below and print it
//Serial.println(readSoil());
    // check whether there has been a request to the server and process it
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        Udp.read(UDPinData, 64);
//generate the data to be sent by the particle server here 
// ....
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
        sprintf(UDPoutData, "%d \n", val);
        Udp.write((unsigned char*)UDPoutData,768); //768 is actual size of data as against buffer size in my case
        Udp.endPacket();
        memset(&UDPoutData[0], 0, sizeof (UDPoutData));
    }//finished writing packet

delay(1000);//take a reading every second
//This time is used so you can test the sensor and see it change in real-time.
//For in-plant applications, you will want to take readings much less frequently.

    //If your soil is too dry, turn on Red LED to notify you
    //This value will vary depending on your soil and plant
    if(readSoil() < 200)
    {
      // take control of the RGB LED
      RGB.control(true);
      RGB.color(255, 0, 0);//set RGB LED to Red
    }
    else
    {
      // resume normal operation
      RGB.control(false);
    }
}
//This is a function used to get the soil moisture content
int readSoil()
{
    digitalWrite(soilPower, HIGH);//turn D6 "On"
    delay(10);//wait 10 milliseconds 
    val = analogRead(soil);
    digitalWrite(soilPower, LOW);//turn D6 "Off"
    return val;
}