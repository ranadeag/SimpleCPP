#include <simplecpp>

namespace simplecpp{
Circle::Circle(Composite* owner)
  : Sprite(0,0, owner){
  radius = 0;
  num_seg= 1000;
  show();
}
Circle::Circle(const double x, const double y, const double r, Composite *owner)
  : Sprite(x,y,owner){
  radius = r;
  num_seg = 1000;
  show();
}

void Circle::init(const double x, const double y, const double r, bool repaintP){
  reset(x,y,r,repaintP);
}

void Circle::reset(const double x, const double y, const double r, bool repaintP){
  pose = Pose(x,y);
  radius = r;
  num_seg = 1000;
  if (repaintP) 
    show();
}

double Circle::getRadius(){
  return radius;
}

void Circle::paint(Pose *p){
  if(visible){
    XPoint res_center;
    double s;
    if(p){
      Pose r = Pose(*p,pose);
      res_center = r.prod(Position(0,0));
      s = r.getScale();
    }
    else{
      res_center = pose.prod(Position(0,0));
      s = pose.getScale();
    }
    drawCircle(res_center, radius,num_seg, color, fill);  // need x and y diameter 
  }
}

void Circle::scale(double factor, bool repaintP){
cout<<"in scale circle"<<endl;
radius *= factor;
if(repaintP) repaint();
}
Sprite* Circle::clone(){
	Circle *p = new Circle();
	p->z_index = this->z_index; // should z_index change?  also on copy const.
	p->pose = this->pose;
	p->visible = this->visible;
	//p->color = this->color;
	p->fill = this->fill;
	p->penIsDown = this->penIsDown;
	p->radius = this->radius;
	p->num_seg = this->num_seg;
	return p;
}
}
