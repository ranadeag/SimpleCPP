#include <simplecpp>
#include <sstream>

namespace simplecpp{

Text::Text(double cx, double cy, string m, Composite* owner)
  : Sprite(cx, cy, owner){
  
  reset(cx, cy, m);
  addText(this);
}

Text::Text(double cx, double cy, double m, Composite* owner)
  : Sprite(cx, cy, owner){
  reset(cx, cy, m);

}

Text::Text(Composite* owner)
  : Sprite(owner){
  reset(0,0,"");
}


void Text::reset(double cx, double cy, string m){
  pose = Pose(cx,cy);
  message = m;
  show();
}

void Text::reset(double cx, double cy, double m){
  ostringstream message;
  message << m;
  reset(cx, cy, message.str());
}

void Text::paint(Pose *p){
  if(visible){
    xp = (p ? Pose(*p,pose) : pose).prod(Position(0,0));
  }
}

double Text::getAngle(){ return __angle__; }
Color Text::getColor(){ return color; }

double Text::getScale(){ return __scale__; }

bool Text::isVisible(){ return visible; }

string Text::getMessage(){ return message; }

XPoint Text::getPos(){ return xp; }

void Text::setMessage(string s){
  message = s;
  repaint();
}

}
