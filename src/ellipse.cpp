#include <simplecpp>

namespace simplecpp{
Ellipse::Ellipse(Composite* owner)
  : Sprite(0,0, owner){
  radius_x = 0;
  radius_y = 0;
  num_seg= 1000;
  show();
}
Ellipse::Ellipse(const double x, const double y, const double rx, const double ry, Composite *owner)
  : Sprite(x,y,owner){
  radius_x = rx;
  radius_y = ry;
  num_seg = 1000;
  show();
}

void Ellipse::init(const double x, const double y, const double rx, const double ry,  bool repaintP){
  reset(x, y, rx, ry, repaintP);
}

void Ellipse::reset(const double x, const double y, const double rx, const double ry, bool repaintP){
  pose = Pose(x,y);
  radius_x = rx;
  radius_y = ry;
  num_seg = 1000;
  if (repaintP) 
    show();
}

double Ellipse::getRadius(){
  return radius_x;
}

void Ellipse::paint(Pose *p){
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
    drawEllipse(res_center, radius_x, radius_y, num_seg, color, fill);  // need x and y diameter 
  }
}

void Ellipse::scale(double factor, bool repaintP){
cout<<"in scale Ellipse"<<endl;
radius_x *= factor;
radius_y *= factor;
if(repaintP) repaint();
}
Sprite* Ellipse::clone(){
 	Ellipse *p = new Ellipse();
// 	p->z_index = this->z_index; // should z_index change?  also on copy const.
// 	p->pose = this->pose;
// 	p->visible = this->visible;
// 	//p->color = this->color;
// 	p->fill = this->fill;
// 	p->penIsDown = this->penIsDown;
// 	p->radius = this->radius;
// 	p->num_seg = this->num_seg;
	return p;
}
}
