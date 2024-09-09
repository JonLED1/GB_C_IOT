int car_red = 2;
int car_yellow = 3;
int car_green = 4;
int hum_red = 5;
int hum_green = 6;
int button = 7;

void setup()
{
  pinMode(button, INPUT);
  pinMode(car_red, OUTPUT);
  pinMode(car_yellow, OUTPUT);
  pinMode(car_green, OUTPUT);
  pinMode(hum_red, OUTPUT);
  pinMode(hum_green, OUTPUT);
}

void loop()
{
  digitalWrite(hum_red, HIGH);
  digitalWrite(hum_green, LOW);
  digitalWrite(car_green, HIGH);
  digitalWrite(car_red, LOW);
  for (int n=0; n<10000; n++){
   	if (digitalRead(button)== HIGH){
    	break;
    }
    delay(1);
  }
  
  for (int n=0; n<3; n++){
  	digitalWrite(car_green, HIGH);
  	delay(1000);
  	digitalWrite(car_green, LOW);
  	delay(1000);
  }
  
  digitalWrite(car_yellow, HIGH);
  delay(2000);
  digitalWrite(car_yellow, LOW);

  digitalWrite(car_red, HIGH);
  digitalWrite(hum_green, HIGH);
  digitalWrite(hum_red, LOW);
  delay(5000);
  
   for (int n=0; n<3; n++){
  	digitalWrite(hum_green, HIGH);
  	delay(1000);
  	digitalWrite(hum_green, LOW);
  	delay(1000);
  }
   
}
