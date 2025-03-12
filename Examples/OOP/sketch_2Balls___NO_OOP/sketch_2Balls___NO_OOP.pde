int x = 0;
int y = 250;

int x2 = 0;
int y2 = 125;

void setup(){
  size(500, 500);
}

void draw(){
  background(0);

  ellipse(x, y, 50, 50);
  ellipse(x2, y2, 20, 20);
  
  x++;
  x2++;
}
