
class Ball{  
  int x;
  int y;
  int d;
  
  Ball( int _x, int _y, int _d){
    x = _x;
    y = _y;
    d = _d;
  }
  
  void display(){
    ellipse(x, y, d, d);
  }
  
  void move(){
    x++;
  }
}

Ball ball1;
Ball ball2;

void setup(){
  size(500, 500);
  
  ball1 = new Ball(0, 250, 50);
  ball2 = new Ball(0, 125, 25);  
}

void draw(){
  background(0);
    
  ball1.display();
  ball2.display();
  
  ball1.move();
  ball2.move();
}
