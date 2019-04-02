SYSTEM_MODE(MANUAL);

int CONNECT_TIMEOUT = 60000;

Timer watchdogTimer(CONNECT_TIMEOUT, doTimeout, true);
bool reconnecting = false;




int one = D3; // relay is connected to D1
int two = D4; // relay is connected to D3
int three = D5; // relay is connected to D4
int four = D6; // relay is connected to D5

int sol [4] = {one, two, three, four};

void setup() {
    pinMode(one, OUTPUT); // Initialize D3 pin as output
    pinMode(two, OUTPUT); // Initialize D4 pin as output
    pinMode(three, OUTPUT); // Initialize D5 pin as output
    pinMode(four, OUTPUT); // Initialize D6 pin as output


    digitalWrite(one, HIGH);
    digitalWrite(two, HIGH);
    digitalWrite(three, HIGH);
    digitalWrite(four, HIGH);
}

void loop() {
     if (!Particle.connected()) {
        if (!WiFi.ready()) {
          if (!WiFi.connecting()) {
            WiFi.connect();
          }
        }
    } else {
        watchdogTimer.start();
        Particle.connect();
        watchdogTimer.stop();
        waitFor(Particle.connected, 10000);
    }

    for (auto && which_relay : sol) {
        digitalWrite(which_relay, LOW);

        delay(600000); // 10 minutes
        digitalWrite(which_relay, HIGH);

    }
}


void handleWifiOff(system_event_t event, int param, void *blah) {
  if (reconnecting) {
    WiFi.on();
  }
}

void handleWifiOn(system_event_t event, int param, void *blah) {
  if (reconnecting) {
    WiFi.connect();
  }
}

void handleWifiConnected(system_event_t event, int param, void *blah) {
  if (reconnecting) {
    reconnecting = false;
    System.off(handleWifiOff);
    System.off(handleWifiOn);
    System.off(handleWifiConnected);
  }
}

void doTimeout() {
  System.on(network_status_off, handleWifiOff);
  System.on(network_status_on, handleWifiOn);
  System.on(network_status_connected, handleWifiConnected);
  reconnecting = true;
  WiFi.off();
}