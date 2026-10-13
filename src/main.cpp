#include <Arduino.h>

// Gamerules
const int board_size_x = 8;
const int board_size_y = 8;
const int tick_interval = 1000;
unsigned long previousMillis = 0;

// Function declarations
void move();
void hori_bounce();
void verti_bounce();
void kill_ball();
void update();
void print_pos();
void print_vel();

struct { // Positions x and y values.
  int x = 20; // Start position x
  int y = 20; // Start position y
} pos;

struct { // Velocity x and y values.
  int x = 1; // Start velocity x
  int y = 3; // Start velocity y
} vel;

struct { // Outer boundaries.
  const int x_outer = 127; // 1 int padding
  const int x_inner = 1;
  const int y_outer = 127;
  const int y_inner = 1;
} limit;

struct { // Playarea definition.
  const int x_outer = 128;
  const int x_inner = 0;
  const int y_outer = 128;
  const int y_inner = 0;
} play_area;

// struct { // Roof
//   int position = limit.y_outer / board_size_y;
// } roof;

// struct { // Floor
//   int position = limit.y_outer - (limit.y_outer / board_size_y);
// } floor;

// struct { // Player 1 racket
//   int min = play_area.y_outer / board_size_x;
//   int max = 3 * (play_area.y_outer / board_size_x);
//   //y verdi 16 til 16 x 3
// } player_1;



// Setup
void setup() {
  Serial.begin(9600);
}

// Loop
void loop() {
  //unsigned long currentMillis = millis();
  move();
  print_pos();
  update();
  delay(100);
}

// Functions
void move(){ // Adds current vector to next tick of ball position.
  pos.x += vel.x;
  pos.y += vel.y;
}

void print_pos(){
  Serial.print("Pos x: ");
  Serial.print(pos.x);
  Serial.print(" ");
  Serial.print("Pos y: ");
  Serial.print(pos.y);
  Serial.println();
}

void print_vel(){
  Serial.print("Vel x: ");
  Serial.print(vel.x);
  Serial.print(" ");
  Serial.print("Vel y: ");
  Serial.print(vel.y);
  Serial.println();
}

void update(){  // Outer bounds
  if(pos.x > limit.x_outer || pos.x <limit.x_inner || pos.y > limit.y_outer || pos.y < limit.y_inner){
    kill_ball();  
  }
  // Walls
  if(pos.y > 112 || pos.y < 16){
    verti_bounce();
  }
  // // Racket
  //   if(pos.x > 112){
  //     check_player();
  //   }
}

void kill_ball(){ // Ends the game and gives error.
  Serial.print("Error Out of Bounds");
  while(1);
}

void hori_bounce(){ // Horizontal bounce.
  vel.x *= -1; // Inverts the vector.
}

void verti_bounce(){ // Vertical bounce.
  vel.y *= -1; // Inverts the vector.
}