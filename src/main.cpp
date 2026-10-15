#include <Arduino.h>

const int number_of_pins = 16; // Kinda means the pixel width and height (1:1)

//Pins
const int cathode_pin[number_of_pins] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // cathode pins x-axis
const int anode_pin[number_of_pins] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Anode pins y-axis

// Joystick pins
const int xaxis1 = A0;
const int yaxis1 = A1;
const int xaxis2 = A3;
const int yaxis2 = A4;

//Joustick variables
int x1 = analogRead(xaxis1);
int x2 = analogRead(xaxis2);
int bevegelse = 0;

// Variables
unsigned long long previousMillis = 0;
unsigned long long previousMillis2 = 0;
unsigned long long previousMillis3 = 0;
unsigned long long previousMillis4 = 0;
const int tick_interval = 128;
int previous_ball_pos[2] = {};

// Function declarations
void hori_bounce();
void verti_bounce();
void update();
void initialize_pins();
void initialize_display();
void draw_ball();
void draw_racket();
void move();

void print_pos();

// Joystick
void Player_joystick();
void Racket();

//Structs
struct { // Positions x and y values.
  int x = 1; // Start position x
  int y = 4; // Start position y
} ball_pos;

struct { // Positions x and y values.
  int x = 15; // Start position x
  int y = 3; // Start position y
} wall;


struct { // Velocity x and y values.
  int x = 1; // Start velocity x
  int y = 2; // Start velocity y
} ball_vel;

struct { // Outer boundaries.
  const int x_outer = 15;
  const int x_inner = 0;
  const int y_outer = 15;
  const int y_inner = 0;
} boundary;

// Joystick structs
struct { // Roof
 int position;
 } roof;

struct { // Floor
  int position;
} floor_wall;

 struct { // Player 1 racket
int x = 15; // Added x coordinates
int y = 4;
} player_1;

//Racket
int racket_midt1;
int racket_midt2;
int racket_bunn;

void setup() {
  Serial.begin(9600);
  initialize_pins();
  initialize_display();

  // Joystick stuff
  roof.position = boundary.y_outer; //limit.y_outer / board_size_y ;
  floor_wall.position = boundary.x_outer; //limit.y_outer - (limit.y_outer / board_size_y);
}

void loop() {
  unsigned long long currentMillis = millis();
  unsigned long long currentMillis2 = millis();
  unsigned long long currentMillis3 = millis();
  unsigned long long currentMillis4 = millis();
  if (currentMillis - previousMillis >= 10){
    previousMillis = currentMillis;
    draw_ball();
  }
  if (currentMillis2 - previousMillis2 >= 10){
    previousMillis2 = currentMillis2;
    draw_racket();
  }
  if (currentMillis3 - previousMillis3 >= 128){
    previousMillis3 = currentMillis3;
    move();
    Player_joystick();
    Racket();
    //print_pos();
  }
  if (currentMillis4 - previousMillis4 >= 1000){
    previousMillis4 = currentMillis4;
    Serial.println(analogRead(yaxis1));
    Serial.println(player_1.y);
  }
}


// Functions

void initialize_pins(){ // Sets all pinmodes
  Serial.println("Pin Inistialization Start");
  // Anode pinmode
  for (int i = 0; i < number_of_pins; i++){
    pinMode(anode_pin[i], OUTPUT);
    Serial.print("pinMode anode: ");
    Serial.print(i + 1);
    Serial.println(" OUTPUT");
  }
  // Cathode pinmode
  for (int i = 0; i < number_of_pins; i++){
    pinMode(cathode_pin[i], OUTPUT);
    Serial.print("pinMode cathode: ");
    Serial.print(i + 1);
    Serial.println(" OUTPUT");
  }
  Serial.println("Pin Initialization Successful");
}

void initialize_display(){ // Resets display / turns all outputs to LOW
  Serial.println("Display Inistialization Start");
  // Anode init
  for (int i = 0; i < number_of_pins; i++){
    digitalWrite(anode_pin[i], LOW);
    Serial.print("digitalWrite anode: ");
    Serial.print(i + 1);
    Serial.println(" LOW");
  }
  
  // Cathode init
  for (int i = 0; i < number_of_pins; i++){
    digitalWrite(cathode_pin[i], LOW);
    Serial.print("digitalWrite cathode: ");
    Serial.print(i + 1);
    Serial.println(" LOW");
  }
  Serial.println("Display Initialization Successful");
}

void hori_bounce(){ // Horizontal bounce.
  ball_vel.x *= -1; // Inverts the vector.
}

void verti_bounce(){ // Vertical bounce.
  ball_vel.y *= -1; // Inverts the vector.
}

void move(){ // Adds current vector to next tick of ball position.
  ball_pos.x += ball_vel.x;
  ball_pos.y += ball_vel.y;
  if(ball_pos.y >= boundary.y_outer){
    verti_bounce();
    ball_pos.y = 15;
  }
  if(ball_pos.y <= boundary.y_inner){
    verti_bounce();
    ball_pos.y = 0;
  }
  if(ball_pos.x >= boundary.x_outer || ball_pos.x <= boundary.x_inner){
    hori_bounce();
  }
  // Out of bounds error handling
  if(ball_pos.x > boundary.x_outer || ball_pos.x < boundary.x_inner || ball_pos.y > boundary.y_outer || ball_pos.y < boundary.y_inner){
    Serial.print("Error Out of Bounds");
    while(1);
  }
}

void draw_ball(){
  for(int i = 0; i < number_of_pins; i++){ // Turn ON all cathodes except cathode x
    if(i == ball_pos.x){
      continue;
    }
    digitalWrite(cathode_pin[i], HIGH);
  }
  for(int i = 0; i < number_of_pins; i++){ // Turn OFF all anodes except anode y
    if(i == ball_pos.y){
      continue;
    }
    digitalWrite(anode_pin[i], LOW);
  }

  digitalWrite(cathode_pin[ball_pos.x], LOW);
  digitalWrite(anode_pin[ball_pos.y], HIGH);
  delay(1);

  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(anode_pin[i], LOW);
  }
  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(cathode_pin[i], LOW);
  }

  
  digitalWrite(anode_pin[ball_pos.y], LOW);
  
  // for(int i = 0; i < number_of_pins; i++){
  //   if(i == ball_pos.x){
  //     continue;
  //   }
  //   digitalWrite(cathode_pin[i], LOW);
  // }
  // digitalWrite(anode_pin[ball_pos.y], LOW);
  // All but x must be high
}

// void draw_racket(){
//   for(int i = 0; i < number_of_pins; i++){
//     if(i == player_1.x){
//       continue;
//     }
//     digitalWrite(cathode_pin[i], HIGH);
//   }
//   digitalWrite(anode_pin[player_1.y], HIGH);
//   delay(1);
//   for(int i = 0; i < number_of_pins; i++){
//     if(i == player_1.x){
//       continue;
//     }
//     digitalWrite(cathode_pin[i], LOW);
//   }
//   digitalWrite(anode_pin[player_1.y], LOW);
// }

void draw_racket(){

  racket_midt1 = player_1.y +1;
  racket_midt2 = player_1.y +2 ;
  racket_bunn = player_1.y +3 ;

  for(int i = 0; i < number_of_pins; i++){ // Turn ON all cathodes except cathode x
    if(i == player_1.x){
      continue;
    }
    digitalWrite(cathode_pin[i], HIGH);
  }
  for(int i = 0; i < number_of_pins; i++){ // Turn OFF all anodes except anode y
    if(i == player_1.y || i == racket_midt1 || i == racket_midt2 || i == racket_bunn){continue;}
    digitalWrite(anode_pin[i], LOW);
  }

  // Turns on the desired LEDs
  digitalWrite(cathode_pin[player_1.x], LOW);
  digitalWrite(anode_pin[player_1.y], HIGH);
  digitalWrite(anode_pin[racket_midt1], HIGH);
  digitalWrite(anode_pin[racket_midt2], HIGH);
  digitalWrite(anode_pin[racket_bunn], HIGH);
  delay(1);

  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(anode_pin[i], LOW);
  }
  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(cathode_pin[i], LOW);
  }
}

void print_pos(){
  Serial.print("Pos x: ");
  Serial.print(ball_pos.x);
  Serial.print(" ");
  Serial.print("Pos y: ");
  Serial.print(ball_pos.y);
  Serial.println();
}

// Joystick
void Player_joystick(){
  if (analogRead(yaxis1) < 350) {
    bevegelse = 1; 
  } else if (analogRead(yaxis1) > 650) {
    bevegelse = -1;
  } else {
    bevegelse = 0;
  }
}


//Racket
void Racket(){

if(player_1.y == 0){
  if(bevegelse == 1){
    player_1.y += bevegelse;
  }
  if(bevegelse == -1){}
}
else if(player_1.y == 12){
  if(bevegelse == -1){
    player_1.y += bevegelse;
  }
  if(bevegelse == 1){}
}
else{
  player_1.y += bevegelse; // (bevegelse * 5);
}

// if(player_1.y == 12){
//   if(bevegelse == -1){
//     player_1.y += bevegelse;
//   }
//   if(bevegelse == 1){}
// }
// else{
//   player_1.y += bevegelse; // (bevegelse * 5);
// }

// if ((player_1.y - 1) >= roof.position) { // changed from < to >
//   player_1.y = roof.position + 12;
//   }
// if ((player_1.y + 1) > floor_wall.position) {
//    player_1.y = floor_wall.position - 1;
//   }
}