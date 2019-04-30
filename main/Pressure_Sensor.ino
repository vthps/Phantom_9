void setup() {
  //Pin mappings on drive
  #define static_sensor 23 //Use 23 and 24
  #define dynamic_sensor 24
}

void velocity_function(){
  //Constants
  int pressureConversion = 5500; //1V = 5500 Pa
  
  //Get Velocity
  //Read analog signals, (sensors return a voltage)
  int static_sensor_input = analogRead(static_sensor);
  int dynamic_sensor_input = analogRead(dynamic_sensor);

  //Map to the pressure sensor
  //0-2048 reading (represents voltage 0-3.3 input), pressure sensor outputs 0-10
  double static_sensor_volt = (static_sensor_input/2048)*10;
  //int static_sensor_volt = (static_sensor_input/10)*3.3;
  double dynamic_sensor_volt = (dynamic_sensor_input/2048)*10;

  
  //Calibrate pressure to voltage every time
  //Make it easy to change
  double static_pressure = static_sensor_volt * pressureConversion;
  double dynamic_pressure = dynamic_sensor_volt * pressureConversion;

  //Benerilis equation converts pressure to velocity (m/s)
  //calculated velocity in m/s using Bernoulli's Equation (see documentation)
  //Calibrated to be in pascals (Pa)
  double velocity = sqrt((2 * abs(static_pressure - dynamic_pressure)) / 1000);

  //Print
  Serial.println("Static Pressure is: ");
  Serial.print(static_pressure);
  Serial.println("Dynamic Pressure is: ");
  Serial.print(dynamic_pressure);
  Serial.println("Velocity: ");
  Serial.print(velocity);
}

//Depth function (only needs one sensor)
//Water is 2988 pascals per foot
//Pressure in Pascals
//Depth in ft
void depth_function() {
  // this function can be called for in the race code and calculates depth
  
  //Constants
  int pressureConversion = 5500; //1V = 5500 Pa
  double depthPressure = 2988.98; //2988.98Pa/ft

  //retrieve raw data
  double raw_static_pressure = analogRead(static_sensor);

  //convert from raw data to volts (5V = 1024 raw data increments)
  double static_voltage = (raw_static_pressure/1024) * 5;

  //convert volts to Pascals (1V = 5500Pa)
  double static_pressure = static_voltage * pressureConversion;
  
  //calculated depth assuming 2988.98Pa/ft
  double depth = static_pressure / depthPressure;

  //Print
  Serial.println("Depth: ");
  Serial.print(depth);
}
