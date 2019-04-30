volatile float time1 = 0.0;
volatile float time2 = 0.0;
volatile float time3 = 0.0;
volatile int iteration = 0;

float time4 = 0.0;
float time5 = 0.0;
float time6 = 0.0;
int iteration2 = 0;

unsigned long overallTimer1 = 0L; //Runs, pullout a timepoint whenever a threshold is crossed
unsigned long overallTimer2 = 0L;
unsigned long curTime1;
unsigned long curTime2;

float avgTime1;
float avgTime2;

// May need to adjust the pin numbers
int interruptPin1 = 38; //2,3,4,5,6,7,8,9,10,anything greater than 2
int interruptPin2 = 39; //Use 38,39

// The read corresponds int values 0-1023 to 0-5 volts
// This means that 1024/5 represents 1 volt
// Uses the threshold of 3.3 volts
int threshold = (1024/5)*3;

void setup() {
  Serial.begin(9600);
  delay(1000);
  Serial.println("Serial has been set up!");
  delay(500);
  
  pinMode(interruptPin1, INPUT);
  Serial.println("Pin 1 has been connected!");
  delay(500);
  
  pinMode(interruptPin2, INPUT);
  Serial.println("Pin 2 has been conected!");
  delay(500);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin1), RPM_ISR1, RISING);
  Serial.println("Pin 1 attached to interrupt!");
  delay(500);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin2), RPM_ISR2, RISING);
  Serial.println("Pin 2 attached to interrupt!");
  delay(500);
  Serial.println("Setup comlete!!!");
}

void loop()
{
  //Intentionally left blank
  //Serial.print("test");
}

//Controls the first RPM sensor
void RPM_ISR1()
{
  Serial.println("Interrupt1");
  curTime1 = overallTimer1;
  overallTimer1 = micros();
  Serial.println("Time 1 initialized");
 
  //Adjusts the time for each RPM pulse based on a background clock
  //Gives time/rotation in s
  switch (iteration % 3)
  {
    case 0:
      time1 = (float)(overallTimer1 - curTime1)/1000000.0;
      break;
    case 1:
      time2 = (float)(overallTimer1 - curTime1)/1000000.0;
      break;
    case 2:
      time3 = (float)(overallTimer1 - curTime1)/1000000.0;
      break;
    default:
      break;
  } 
    
  iteration++;
//  Serial.print("time1: ");
//  Serial.println(time1);
//  Serial.print("time2: ");
//  Serial.println(time2);
//  Serial.print("time3: ");
//  Serial.println(time3);
  avgTime1 = avgCalc(time1, time2, time3);
  
  Serial.println(60.0*(1.0/avgTime1)); //Prints the average of one RPM sensor
}

//Controls the Second RPM sensor
void RPM_ISR2()
{
  Serial.println("Interrupt2");
  curTime2 = overallTimer2;
  overallTimer2 = micros();
  Serial.println("Time 2 initalized");

//  Serial.print("CurTime(micros) ");
//  Serial.println(curTime2);
//  Serial.print("OverallTime(micros) ");
//  Serial.println(overallTimer2);
  
  //Adjusts the time for each RPM pulse based on a background clock
  //Each RPM reading is the time between two pulses (in s)
  switch (iteration2 % 3)
  {
    case 0:
      time4 = (float)(overallTimer2 - curTime2)/1000000.0;
      break;
    case 1:
      time5 = (float)(overallTimer2 - curTime2)/1000000.0;
      break;
    case 2:
      time6 = (float)(overallTimer2 - curTime2)/1000000.0;
      break;
    default:
      break; 
  }
  
  iteration2++;
  
//  Serial.print("Time4: ");
//  Serial.println(time4);
//  Serial.print("Time5: ");
//  Serial.println(time5);
//  Serial.print("Time6: ");
//  Serial.println(time6); 
   
  avgTime2 = avgCalc(time4, time5, time6); //Avg time/rotation
  
  Serial.println(60.0*(1.0/avgTime2)); //Prints the average of one RPM sensor
}

/**
 * Calculates the average, adjusts for a lack of data
 * 
 * Worth noting that it is a poor adjustment, 
 * the timer starts when the board is booted
 */
float avgCalc(float input1, float input2, float input3)
{
  if (input1 == -0.0) //No data collected
  {
    return 0.0;
  }
  if (input2 == -0.0) //Only 1 input collected
  {
    return input1;
  }
  if (input3 == -0.0) //Averages 2 inputs
  {
    return (input1+input2)/2.0;
  }
  return (input1+input2+input3)/3.0;
}
