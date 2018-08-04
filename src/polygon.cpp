#include <simplecpp>

namespace simplecpp{
Polygon::Polygon(Composite* owner): Sprite(0,0,owner) {
  init(canvas_width()/2, canvas_height()/2, NULL, 0, owner);
}

Polygon::Polygon(const double x, const double y, const double points[][2], 
		 int no, Composite *owner) : Sprite(x,y,owner){
  init(x, y, points,no, owner);
}

void Polygon::reset(const double x, const double y, 
		    const double points[][2], int no, Composite *owner){
  vertex.clear();
  for(int iter = 0; iter < no; iter ++){
    vertex.push_back(Position(points[iter][0], points[iter][1]));
  }
  Sprite::reset(x,y,owner);
}


void Polygon::init(const double x, const double y, 
		   const double points[][2], int no, Composite *owner){
  reset(x, y, points, no, owner);
  show();
}


void Polygon::paint(Pose *p){
  if(visible && vertex.size()>0){
    XPoint xpt_vertex[vertex.size()];
    if(p){
      Pose r = Pose(*p,pose);
      r.compute_res_vertex(xpt_vertex, vertex, vertex.size());
    }
    else pose.compute_res_vertex(xpt_vertex, vertex, vertex.size());
    drawPolygon(xpt_vertex, vertex.size(), color, fill);
  }
}

Sprite* Polygon::clone(){
	Polygon *p = new Polygon();
	p->z_index = this->z_index; // should z_index change?  also on copy const.
	p->pose = this->pose;
	p->visible = this->visible;
	//p->color = this->color;
	p->fill = this->fill;
	p->penIsDown = this->penIsDown;
	p->vertex = this->vertex;
	return p;
}
}
