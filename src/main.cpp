#include <Arduino.h>

// Function declarations
void move();
void hori_bounce();
void verti_bounce();

struct { // Positions x and y values.
  int x = 0;
  int y = 0;
} pos;

struct { // Velocity x and y values.
  int x = 1;
  int y = 3;
} vel;

struct { // Restriction of playarea definition.
  const int x_outer = 63;
  const int x_inner = 1;
  const int y_outer = 63;
  const int y_inner = 1;
} limit;


// Setup
void setup() {
  Serial.begin(9600);
}

// Loop
void loop() {
  move();
  Serial.print("Pos x: ");
  Serial.print(pos.x);
  Serial.print(" ");
  Serial.print("Pos y: ");
  Serial.print(pos.y);
  Serial.println();
  if(pos.x > limit.x_outer || limit.x_inner < 1){
    hori_bounce();
  }
  if(pos.y > limit.y_outer || limit.y_inner < 1){
    verti_bounce();
  }
  delay(100);
}

// Functions
void move(){
  pos.x += vel.x;
  pos.y += vel.y;
}

void hori_bounce(){ // Horizontal bounce.
  vel.x *= -1;
}

void verti_bounce(){ // Vertical bounce.
  vel.y *= -1;
}