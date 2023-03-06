/*
  Rocky and Bullwinkle Nell Motor Driver v1.95
  Drives a NEMA17 Stepper based upon a 32v DC signal
  from the CPU board that shows that the motor should run.
  If the signal is positive run the motor and count the number of
  steps and then when the signal drops back to 0, step back
  to return Nell to the starting point without any sensing switches.
  There is an 18.5 second run time
*/

const byte INPUT_PIN = 2;   // Input from RaB to tell when to drive Nell
const byte DIR_PIN   = 3;   // a4988 direction of drive
const byte STEP_PIN  = 4;   // a4988 step
const byte SLEEP_PIN = 5;   // a4988 sleep pin to shut down stepper when not driving

// Variables to monitor
int stepsMade        = 0;   // Number of steps made on Nell’s run
int turnOnNell       = 0;   // Check if motor should run or return to start

void setup() {
  pinMode(STEP_PIN, OUTPUT); 
  pinMode(DIR_PIN, OUTPUT);
  pinMode(SLEEP_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT);

  Serial.begin(9600);       // Initiate Serial communication for testing
}

void loop() {
  turnOnNell = digitalRead(INPUT_PIN);                   // Read the input pin

  if (turnOnNell == HIGH && stepsMade < 910) {           // If Input Pin is High run Nell towards the blade
    digitalWrite(SLEEP_PIN, HIGH);                       // Wake a4988
    digitalWrite(DIR_PIN, HIGH);                         // turn the motor Counter-CLockwise, towards the saw
    delay(1);                                            // Data sheet says a4988 needs 1ms before stepping
    runForward();
    }

  else if (turnOnNell == HIGH && stepsMade > 908) {      // At end of run and holding Nell in Place
    digitalWrite(SLEEP_PIN, LOW);
  }

  else if(turnOnNell == LOW && stepsMade > 0){           // If Input Pin Low and steps still remain return Nell to start
  // NOTE for some reason the driver will not completely return to start if at the very end of the run, all other locations 
  // will return exactly to start.  Increasing steps made at the end of a run is there to ensure return to start
    if(stepsMade == 910){  
      stepsMade = 920;
    }
    digitalWrite(SLEEP_PIN, HIGH);                       // Wake a4988
    digitalWrite(DIR_PIN, LOW);                          // turn the motor CLockwise, away from the saw
    delay(1);                                            // Data sheet says a4988 needs 1ms before stepping
    returnToStart();
  }
  
  else{                                                  // Nothing going on, Sleep Motor and a4988
    digitalWrite(SLEEP_PIN, LOW);
  }
}

void runForward(){                                       // run motor and count steps
  while (stepsMade < 910 && turnOnNell == HIGH){ 
    turnOnNell = digitalRead(INPUT_PIN);
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(10500);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(10500);
    ++stepsMade;
    Serial.println(stepsMade); 
  }
}

void returnToStart(){                                   // return motor to starting position
  while (stepsMade > 0) {
    digitalWrite(STEP_PIN, HIGH);
    delay(1);
    digitalWrite(STEP_PIN, LOW);
    delay(1);  
    --stepsMade;
    Serial.println(stepsMade); 
  } 
}

  
