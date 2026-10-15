#include <Arduino.h>

const int number_of_pins = 16; // Kinda means the pixel width and height (1:1)

//Pins
const int cathode_pin[number_of_pins] = {23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53}; // cathode pins x-axis
const int anode_pin[number_of_pins] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52}; // Anode pins y-axis

// Player 1 joystick pins
//const int player_1_stick_x = A4;
const int player_1_stick_y = A5;
// Player 2 joystick pins
//const int player_2_stick_x = A2;
const int player_2_stick_y = A3;

// 7 Segment display pins 
// player 1 Må bytte pins
const int g = 2;
const int f = 3;
const int e = 4;
const int d = 5;
const int c = 6;
const int b = 7;
const int a = 8;
// player 2 
const int g2 = A8;
const int f2 = A9;
const int e2 = 13;
const int d2 = 12;
const int c2 = 11;
const int b2 = 10;
const int a2 = 9;

//Joystick variables
// int player_1_stick_val_x = analogRead(player_1_stick_x);
// int player_2_stick_val_x = analogRead(player_2_stick_x);
int player_1_bevegelse_y = 0;
int player_2_bevegelse_y = 0;

// 7 Segment display variables Må bytte noen pins
int segdisplay1[] = {8, 7, 6, 5, 4, 3, 2};
int points1 = 0;
int segdisplay2[] = {9, 10, 11, 12, 13, A1, A0};
int points2 = 0;

// Variables
unsigned long long previousMillis = 0;
unsigned long long previousMillis2 = 0;
unsigned long long previousMillis3 = 0;
unsigned long long previousMillis4 = 0;
//unsigned long long previousMillis5 = 0;
unsigned long long previousMillis6 = 0;
unsigned long MillisTimer1 = 0;

const int tick_interval = 128;
const int restart_game_pause_duration = 200;
int previous_ball_pos[2] = {};

// Function declarations
void hori_bounce();
void verti_bounce();
void update();
void initialize_pins();
void initialize_display();
void draw_ball();
void draw_racket_1();
void draw_racket_2();
void move();


//Print functions declarations
void print_pos();
void print_vel();
void print_score();
void print_player_pos();

// Joystick
void Player_joystick();
void Racket();

// 7 segment display
void showpoints(int tall, int display[]);

void Racket_ballhit();

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
int x = 0; // Added x coordinates
int y = 4;
} player_1;

 struct { // Player 2 racket
int x = 15; // Added x coordinates
int y = 4;
} player_2;

//Racket
int racket1_midt1;
int racket1_midt2;
int racket1_bunn;

int racket2_midt1;
int racket2_midt2;
int racket2_bunn;

void setup() {
  Serial.begin(9600);
  initialize_pins();
  initialize_display();

  // 7 segment stuff
  showpoints(points1, segdisplay1);
  showpoints(points2, segdisplay2);

  // Joystick stuff
  roof.position = boundary.y_outer; //limit.y_outer / board_size_y ;
  floor_wall.position = boundary.x_outer; //limit.y_outer - (limit.y_outer / board_size_y);
}

void loop() {
  unsigned long long currentMillis = millis();
  unsigned long long currentMillis2 = millis();
  unsigned long long currentMillis3 = millis();
  unsigned long long currentMillis4 = millis();
  unsigned long long currentMillis5 = millis();
  unsigned long long currentMillis6 = millis();
  if (currentMillis - previousMillis >= 10){
    previousMillis = currentMillis;
    draw_ball();
  }
  if (currentMillis2 - previousMillis2 >= 10){
    previousMillis2 = currentMillis2;
    draw_racket_1();
  }
  if (currentMillis4 - previousMillis4 >= 10){
    previousMillis4 = currentMillis4;
    draw_racket_2();
  }
  if (currentMillis6 - previousMillis6 >= 64){
    previousMillis6 = currentMillis6;
    Player_joystick();
    Racket();
  }
  if (currentMillis3 - previousMillis3 >= 128){
    previousMillis3 = currentMillis3;
    move();
    // print_score();
    // print_pos();
    // print_vel();
    // print_player_pos();
    // Serial.println();
    showpoints(points1, segdisplay1);
    showpoints(points2, segdisplay2);
  }
  if (currentMillis5 == MillisTimer1){
    if(ball_vel.x == 0){
      ball_vel.x = 1;
    }
    if(ball_vel.y == 0){
      ball_vel.y = 2;
    }

  }
  //Serial.print(MillisTimer1);
  //Maybe move to a millis?
}


// Functions

void initialize_pins(){ // Sets all pinmodes
  Serial.println("Pin Inistialization Start");
  // Anode pinmode
  for (int i = 0; i < number_of_pins; i++){
    pinMode(anode_pin[i], OUTPUT);
    // Serial.print("pinMode anode: ");
    // Serial.print(i + 1);
    // Serial.println(" OUTPUT");
  }
  // Cathode pinmode
  for (int i = 0; i < number_of_pins; i++){
    pinMode(cathode_pin[i], OUTPUT);
    // Serial.print("pinMode cathode: ");
    // Serial.print(i + 1);
    // Serial.println(" OUTPUT");
  }

  // 7 segment display pins
  pinMode(g,OUTPUT);
  pinMode(f,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(a,OUTPUT);
  for (int i = 0; i < 7; i++) {
    pinMode(segdisplay1[i], OUTPUT);
    pinMode(segdisplay2[i], OUTPUT);
  }
  Serial.println("Pin Initialization Successful");
}

void initialize_display(){ // Resets display / turns all outputs to LOW
  Serial.println("Display Inistialization Start");
  // Anode init
  for (int i = 0; i < number_of_pins; i++){
    digitalWrite(anode_pin[i], LOW);
    // Serial.print("digitalWrite anode: ");
    // Serial.print(i + 1);
    // Serial.println(" LOW");
  }
  
  // Cathode init
  for (int i = 0; i < number_of_pins; i++){
    digitalWrite(cathode_pin[i], LOW);
    // Serial.print("digitalWrite cathode: ");
    // Serial.print(i + 1);
    // Serial.println(" LOW");
  }
  Serial.println("Display Initialization Successful");
}

void hori_bounce(){ // Horizontal bounce.
  ball_vel.x *= -1; // Inverts the vector.
}

void verti_bounce(){ // Vertical bounce.
  ball_vel.y *= -1; // Inverts the vector.
}

void restart_game(){
  unsigned long long timer = millis();
  MillisTimer1 = timer + restart_game_pause_duration;
  ball_pos.x = 8;
  ball_pos.y = 8;
  ball_vel.x = 0;
  ball_vel.y = 0;
}


// Print fucntions
void print_score(){
  Serial.print("Scoreboard: ");
  Serial.print("Player 1 points: ");
  Serial.print(points1);
  Serial.print(" ");
  Serial.print("Player 2 points: ");
  Serial.print(points2);
  Serial.print(" ");
}

void print_pos(){
  Serial.print("Ball Position: ");
  Serial.print("Pos x: ");
  Serial.print(ball_pos.x);
  Serial.print(" ");
  Serial.print("Pos y: ");
  Serial.print(ball_pos.y);
  Serial.print(" ");
}

void print_vel(){
  Serial.print("Ball Velocity: ");
  Serial.print("Vel x: ");
  Serial.print(ball_vel.x);
  Serial.print(" ");
  Serial.print("Vel y: ");
  Serial.print(ball_vel.y);
  Serial.print(" ");
}

void print_player_pos(){
  Serial.print("Player 1 Position: ");
  Serial.print("Pos y: ");
  Serial.print(player_1.y);
  Serial.print(" ");
  Serial.print("Player 2 Position: ");
  Serial.print("Pos y: ");
  Serial.print(player_2.y);
  Serial.print(" ");
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
  if(ball_pos.x >= boundary.x_outer + 1){
    points1++;
    hori_bounce();
    restart_game();
  }
  if(ball_pos.x <= boundary.x_inner -1 ){
    points2++;
    hori_bounce();
    restart_game();
  }
  // Out of bounds error handling
  if(ball_pos.x > boundary.x_outer || ball_pos.x < boundary.x_inner || ball_pos.y > boundary.y_outer || ball_pos.y < boundary.y_inner){
    Serial.print("Error Out of Bounds");
    while(1);
  }
  Racket_ballhit();
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

void draw_racket_1(){

  racket1_midt1 = player_1.y +1;
  racket1_midt2 = player_1.y +2 ;
  racket1_bunn = player_1.y +3 ;

  for(int i = 0; i < number_of_pins; i++){ // Turn ON all cathodes except cathode x
    if(i == player_1.x){
      continue;
    }
    digitalWrite(cathode_pin[i], HIGH);
  }
  for(int i = 0; i < number_of_pins; i++){ // Turn OFF all anodes except anode y
    if(i == player_1.y || i == racket1_midt1 || i == racket1_midt2 || i == racket1_bunn){continue;}
    digitalWrite(anode_pin[i], LOW);
  }

  // Turns on the desired LEDs
  digitalWrite(cathode_pin[player_1.x], LOW);
  digitalWrite(anode_pin[player_1.y], HIGH);
  digitalWrite(anode_pin[racket1_midt1], HIGH);
  digitalWrite(anode_pin[racket1_midt2], HIGH);
  digitalWrite(anode_pin[racket1_bunn], HIGH);
  delay(1);

  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(anode_pin[i], LOW);
  }
  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(cathode_pin[i], LOW);
  }
}

void draw_racket_2(){

  racket2_midt1 = player_2.y +1;
  racket2_midt2 = player_2.y +2 ;
  racket2_bunn = player_2.y +3 ;

  for(int i = 0; i < number_of_pins; i++){ // Turn ON all cathodes except cathode x
    if(i == player_2.x){
      continue;
    }
    digitalWrite(cathode_pin[i], HIGH);
  }
  for(int i = 0; i < number_of_pins; i++){ // Turn OFF all anodes except anode y
    if(i == player_2.y || i == racket2_midt1 || i == racket2_midt2 || i == racket2_bunn){continue;}
    digitalWrite(anode_pin[i], LOW);
  }

  // Turns on the desired LEDs
  digitalWrite(cathode_pin[player_2.x], LOW);
  digitalWrite(anode_pin[player_2.y], HIGH);
  digitalWrite(anode_pin[racket2_midt1], HIGH);
  digitalWrite(anode_pin[racket2_midt2], HIGH);
  digitalWrite(anode_pin[racket2_bunn], HIGH);
  delay(1);

  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(anode_pin[i], LOW);
  }
  for(int i = 0; i < number_of_pins; i++){
    digitalWrite(cathode_pin[i], LOW);
  }
}

// Joystick
void Player_joystick(){
  // Player 1
  if (analogRead(player_1_stick_y) < 350) {
    player_1_bevegelse_y = 1; 
  } else if (analogRead(player_1_stick_y) > 650) {
    player_1_bevegelse_y = -1;
  } else {
    player_1_bevegelse_y = 0;
  }

  // Player 2
  if (analogRead(player_2_stick_y) < 350) {
    player_2_bevegelse_y = 1; 
  } else if (analogRead(player_2_stick_y) > 650) {
    player_2_bevegelse_y = -1;
  } else {
    player_2_bevegelse_y = 0;
  }
}


//Racket
void Racket(){

//Player 1
if(player_1.y == 0){
  if(player_1_bevegelse_y == 1){
    player_1.y += player_1_bevegelse_y;
  }
  if(player_1_bevegelse_y == -1){}
}
else if(player_1.y == 12){
  if(player_1_bevegelse_y == -1){
    player_1.y += player_1_bevegelse_y;
  }
  if(player_1_bevegelse_y == 1){}
}
else{
  player_1.y += player_1_bevegelse_y; // (bevegelse * 5);
}

//Player 2
if(player_2.y == 0){
  if(player_2_bevegelse_y == 1){
    player_2.y += player_2_bevegelse_y;
  }
  if(player_2_bevegelse_y == -1){}
}
else if(player_2.y == 12){
  if(player_2_bevegelse_y == -1){
    player_2.y += player_2_bevegelse_y;
  }
  if(player_2_bevegelse_y == 1){}
}
else{
  player_2.y += player_2_bevegelse_y; // (bevegelse * 5);
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

void Racket_ballhit(){ //eddit
  if(ball_pos.x == 15){
    if (ball_pos.y == player_2.y|| ball_pos.y == racket2_midt1 || ball_pos.y == racket2_midt2 || ball_pos.y == racket2_bunn){
      hori_bounce();
      ball_pos.x = 14;
    }
  }
  if(ball_pos.x == 0){
    if (ball_pos.y == player_1.y|| ball_pos.y == racket1_midt1 || ball_pos.y == racket1_midt2 || ball_pos.y == racket1_bunn){
      hori_bounce();
      ball_pos.x = 1;
    }
  }
}

void showpoints(int tall, int display[]) {
  // skru ev segmenter  
  for (int i = 0; i < 7; i++) {
    digitalWrite(display[i], LOW);
  } // skrur på når den får riktig input 
  if (tall == 0) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[4], HIGH);
    digitalWrite(display[5], HIGH);
  } else if (tall == 1) {
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
  } else if (tall == 2) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[1], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[4], HIGH);
    digitalWrite(display[6], HIGH);
  } else if (tall == 3) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[6], HIGH);
  } else if (tall == 4) {
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[5], HIGH);
    digitalWrite(display[6], HIGH);
  } else if (tall == 5) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[5], HIGH);
    digitalWrite(display[6], HIGH);
  } else if (tall == 6) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[4], HIGH);
    digitalWrite(display[5], HIGH);
    digitalWrite(display[6], HIGH);
  } else if (tall == 7) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
  } else if (tall == 8) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[4], HIGH);
    digitalWrite(display[5], HIGH);
    digitalWrite(display[6], HIGH);
  } else if (tall == 9) {
    digitalWrite(display[0], HIGH);
    digitalWrite(display[1], HIGH);
    digitalWrite(display[2], HIGH);
    digitalWrite(display[3], HIGH);
    digitalWrite(display[5], HIGH);
    digitalWrite(display[6], HIGH);
  }
}