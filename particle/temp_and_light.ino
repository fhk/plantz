// OG code taken from sparkfun tutorial:
// https://learn.sparkfun.com/tutorials/sparkfun-inventors-kit-for-photon-experiment-guide/experiment-6-environment-monitor
// Added in event publication of data every 60 seconds

// This #include statement was automatically added by the Particle IDE.
#include <SparkFunRHT03.h>


/////////////////////
// Pin Definitions //
/////////////////////
const int RHT03_DATA_PIN = D3; // RHT03 data pin
const int LIGHT_PIN = A0; // Photocell analog output
const int LED_PIN = D7; // LED to show when the sensor's are being read

///////////////////////////
// RHT03 Object Creation //
///////////////////////////
RHT03 rht; // This creates a RTH03 object, which we'll use to interact with the sensor

unsigned int minimumLight = 65536;
unsigned int maximumLight = 0;
float minimumTempC = 5505;
float maximumTempC = 0;
float minimumTempF = 9941;
float maximumTempF = 0;
float minimumHumidity = 100;
float maximumHumidity = 0;

int light = 0;
double humidity = 0;
double tempF = 0;
double tempC = 0;

unsigned long lastTime = 0UL;

void setup() 
{
    // Serial.begin() is used to open up a serial interface between the Photon
    // and your computer.
    // The '9600' parameter configures the speed of the interface. This value is
    // called the "baud rate", which is equivalent to bits per second (bps).
    Serial.begin(9600); // Start the serial interface at 9600 bps
    
    // Using the 'rht' object created in the global section, we'll begin by calling
    // its member function `begin`.
    // The parameter in this function is the DIGITAL PIN we're using to communicate
    // with the sensor.
    rht.begin(RHT03_DATA_PIN);  // Initialize the RHT03 sensor
    
    // Don't forget to set the pin modes of our analog sensor (INPUT) and the LED (OUTPUT):
    pinMode(LIGHT_PIN, INPUT); // Set the photocell pin as an INPUT.
    pinMode(LED_PIN, OUTPUT); // Set the LED pin as an OUTPUT
    digitalWrite(LED_PIN, LOW); // Initially set the LED pin low -- turn the LED off.
}

void loop() 
{
    digitalWrite(LED_PIN, HIGH); // Turn the LED on -- it'll blink whenever the sensor is being read.
    
    // Use the RHT03 member function `update()` to read new humidity and temperature values from the sensor.
    // There's a chance the reading might fail, so `update()` returns a success indicator. It'll return 1
    // if the update is successful, or a negative number if it fails.
    int update = rht.update();
    
    if (update == 1) // If the update succeeded, print out the new readings:
    {
        // Use analogRead to get the latest light sensor reading:
        light = analogRead(LIGHT_PIN);
        // Do some math to calculate the minimum and maximum light sensor readings we've seen:
        if (light > maximumLight) maximumLight = light;
        if (light < minimumLight) minimumLight = light;
        
        // The `humidity()` RHT03 member function returns the last successfully read relative
        // humidity value from the RHT03.
        // It'll return a float value -- a percentage of RH between 0-100.
        // ONLY CALL THIS FUNCTION AFTER SUCCESSFULLY RUNNING rht.update()!.
        humidity = rht.humidity();
        // Do some math to calculate the max/min humidity
        if (humidity > maximumHumidity) maximumHumidity = humidity;
        if (humidity < minimumHumidity) minimumHumidity = humidity;
        
        // The `tempF()` RHT03 member function returns the last succesfully read 
        // farenheit temperature value from the RHT03.
        // It returns a float variable equal to the temperature in Farenheit.
        // ONLY CALL THIS FUNCTION AFTER SUCCESSFULLY RUNNING rht.update()!.
        tempF = rht.tempF();
        // Do some math to calculate the max/min tempF
        if (tempF > maximumTempF) maximumTempF = tempF;
        if (tempF < minimumTempF) minimumTempF = tempF;
        
        // `tempC()` works just like `tempF()`, but it returns the temperature value in
        // Celsius.
        // ONLY CALL THIS FUNCTION AFTER SUCCESSFULLY RUNNING rht.update()!.
        tempC = rht.tempC();
        // Do some math to calculate the max/min tempC
        if (tempC > maximumTempC) maximumTempC = tempC;
        if (tempC < minimumTempC) minimumTempC = tempC;

    }
    else // If the update failed, give the sensor time to reset:
    {
        Serial.println("Error reading from the RHT03."); // Print an error message
        Serial.println(); // Print a blank line
        
        delay(RHT_READ_INTERVAL_MS); // The RHT03 needs about 1s between read attempts
    }
    digitalWrite(LED_PIN, LOW); // Turn the LED off
    
    char data[64];
    snprintf(data, sizeof(data), "t: %.2f°C, h: %.2f%%, l:%i", tempC, humidity, light); // output: t: 12.34°C, h: 45.67%, l: 200
    unsigned long now = millis();
    //Every 60 seconds publish uptime
    if (now-lastTime>60000UL) {
        lastTime = now;
        // now is in milliseconds
        unsigned nowSec = now/1000UL;
        unsigned sec = nowSec%60;
        unsigned min = (nowSec%3600)/60;
        unsigned hours = (nowSec%86400)/3600;
        Particle.publish("envData", data, PRIVATE);
    }
}
