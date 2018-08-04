#include <simplecpp>
GLuint vbo, vao,FBO, fbo=0;

GLuint renderTexture, renderedTexture, depthrenderbuffer;
GLuint depthTexture;

GLuint FramebufferName = 0;	

GLFWimage image;
vector<GLuint> Gtexture;
vector<pair<pair<int,int>, pair<int,int> > > texCoord;
map<GLuint, vector<int> > Gtex;

int screen_width;
int screen_height;
char *title;
GLFWwindow* window;

XEvent eve;
bool callback=false;
bool lbutton=false;

struct LtSprite
{
  bool operator()(const Sprite* s1, const Sprite* s2) const {
    return s1->getZIndex() < s2->getZIndex();
  }
};

std::multiset<Sprite *, LtSprite> spriteSet;
std::multiset<Sprite *, LtSprite>:: iterator iter;

std::multiset<Text *, LtSprite> textSet;

namespace simplecpp{
  bool display = false;



void cursor_pos_callback(GLFWwindow* window,  double x, double y){
	double xpos, ypos;
	
	if(lbutton){
		
		eve.type=1;
		glfwGetCursorPos(window, &xpos, &ypos);
		    eve.xmotion.x = xpos;
		    eve.xmotion.y = ypos;
	}

}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	
	if(action == GLFW_PRESS){
			eve.type = 4;
			if(button == GLFW_MOUSE_BUTTON_LEFT){
				lbutton = true;
				eve.xbutton.button = 0;
			}
			else if(button == GLFW_MOUSE_BUTTON_MIDDLE){
				eve.xbutton.button = 1;
			}
			else if(button == GLFW_MOUSE_BUTTON_RIGHT){
				eve.xbutton.button = 2;
			}
	
	
			glfwGetCursorPos(window, &xpos, &ypos);
		    eve.xbutton.x = xpos;
		    eve.xbutton.y = ypos;
		}

		if(action == GLFW_RELEASE){
			lbutton=false;
			eve.type = 2;
		}
	
    callback=true;
}

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	eve.type = 3;
	callback=true;
	if(action==GLFW_PRESS){
		int _disp_ = key -  GLFW_KEY_A;
		eve.key = 'a' + _disp_ ;
	}
	
	double xpos,ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
    eve.xkey.x = xpos;
    eve.xkey.y = ypos;
}

void window_close_callback(GLFWwindow *window){
	glfwTerminate();
	exit(0);
}

int getClick(){
	
	double xpos, ypos;
	while(1)
	 {
	 	glfwWaitEvents();
    
	 	if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && lbutton){
			glfwGetCursorPos(window, &xpos, &ypos);
			break;
		}
	}
	return xpos * 65536 + ypos;
}

void loadTexture(GLuint *texture, char* path){
    *texture = SOIL_load_OGL_texture(path,
                                     SOIL_LOAD_AUTO,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA
                                    );


    if(! *texture){
        printf("Failed to load %s", path);
    }
    
}

void createObjTexture(int w, int h){
  glGenTextures(1, &renderedTexture);
  glBindTexture(GL_TEXTURE_2D, renderedTexture);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  int i;
  i=glGetError();
  if(i!=0)
  {
    std::cout << "Error happened while loading the texture: " << i << std::endl;
  }
  glBindTexture(GL_TEXTURE_2D,0);
}

void createObjFBO(){
  
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  createObjTexture(screen_width, screen_height);
  glBindTexture(GL_TEXTURE_2D, renderedTexture);
  
  glGenRenderbuffers(1, &depthrenderbuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderedTexture, 0);
  GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers);
  
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    cout<<"Error in FBO"<<endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void bindFBO(){
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void unbindFBO(){
  glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void config(GLFWwindow* window){
  int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float ratio = width / (float) height;
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawRect(int x, int y, int w, int h, GLuint texture){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glPushMatrix(); 
    glOrtho(0,screen_width,0,screen_height,-1,1);
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
        glTexCoord2i(0,0);
        glVertex2i(x,screen_height-h);
        glTexCoord2i(1,0);
        glVertex2i(x+w,screen_height-h);
        glTexCoord2i(1,1);
        glVertex2i(x+w,screen_height-y);
        glTexCoord2i(0,1);
        glVertex2i(x,screen_height-y);
    glEnd();
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

int BGImage(char * path, int _x_, int _y_, int _xx_, int _yy_){
	GLuint texture;
	loadTexture(&texture, path);
	int arr[] = { _x_,_y_,_xx_,_yy_ };
  	int n = sizeof(arr) / sizeof(arr[0]); 
  	vector<int> vect(arr, arr + n);
	Gtex[texture] = vect;
	drawRect(_x_,_y_,_xx_,_yy_,texture);
  	Gtexture.push_back(texture);
	glfwSwapBuffers(window);
	
	repaint();
	return texture;
}


unsigned int createTexture(int w,int h)
{
	unsigned int textureId;
	glGenTextures(1,&textureId);
	glBindTexture(GL_TEXTURE_2D,textureId);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
	int i;
	i=glGetError();
	if(i!=0)
	{
		std::cout << "Error happened while loading the texture: " << i << std::endl;
	}
	return textureId;
}


GLuint createFBO(){

	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
	renderTexture = createTexture(screen_width, screen_height);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
//create DepthBuffer
	GLuint depthrenderbuffer;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
//Bind color to fbo
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);
//check for errors
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout<<"Error in FBO"<<endl;

	return FramebufferName;
}

void window_size_callback(GLFWwindow *window, int w, int h){
	glfwSetWindowSize(window, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	repaint();
}
void initGL(void)
{
  glClearDepth(1.0);
  glClearColor(1,1,1, 1.0f);
  glColor3f(1.0, 1.0, 1.0);
  glDepthFunc(GL_LESS);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  createObjFBO();	
  bindFBO(); config(window); unbindFBO();
  glColor3f(0,0,0);
  repaint();
  
}

void initCanvas(const char *window_title, int width, int height){    
	
	display = true;
	screen_width = width;
	screen_height = height;

	if (!glfwInit())
	    exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	window = glfwCreateWindow(screen_width, screen_height, window_title, NULL, NULL);
	if (!window)
	{
	    glfwTerminate();
	    exit(EXIT_FAILURE);
	}
	glfwGetFramebufferSize( window, &screen_width, &screen_height );
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	//glfwSwapInterval(1);
	glfwSetMouseButtonCallback(window, mouse_button_callback);	
  glfwSetCursorPosCallback(window, cursor_pos_callback);
	
	glfwSetKeyCallback(window, keyboard_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwWindowHint(GLFW_RESIZABLE, 1);	
	if ( GLEW_OK != glewInit( ) )
	    {
		std::cout << "Failed to initialize GLEW" << std::endl;
		exit(0);
	    }
        glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS,1);

	initGL();
}


void resizeCanvas(int w, int h){
	glfwSetWindowSize(window, w, h);
	screen_width=w;
	screen_height=h;
	repaint();
}
double randuv(double u, double v){
	return u + (v-u)*rand()/(1.0+RAND_MAX);
}
  

void abort(){ std::abort(); }

  int canvas_width(){
    return screen_width;
  }
  
  int canvas_height(){
    return screen_height;
  }

  void wait(float duration){
	glfwPollEvents();
    usleep((int) (duration*1000000));
  }

  void closeCanvas(){
    spriteSet.clear();    // Delete all elements from set
    glfwTerminate();
    exit(0);
  }

  void drawLine(XPoint start, XPoint end, Color line_color, unsigned int line_width){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, 0, screen_height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    glLineWidth(line_width);
    float r,g,b;
    int rr,gg,bb;
    rr = line_color & 0xFF0000; rr = rr >> 16;
    
    gg = line_color & 0x00FF00; gg = gg >> 8;
    bb = line_color & 0x0000FF;
    r = rr / 255.0;
    g = gg / 255.0;
    b = bb / 255.0;
 
    
    glColor3f(r,g,b); 
    glBegin(GL_LINE_LOOP);
			glVertex3f(start.x, screen_height - start.y, 0.0);
			glVertex3f(end.x, screen_height - end.y, 0.0);
    glEnd();
   
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }



   void imprintLine(short x1, short y1, short x2, short y2, Color line_color, unsigned int line_width){
			bindFBO();
			cout<<"in imprint line: "<< x2 << " " << y2 << endl;
			Line *l = new Line(x1,y1,x2,y2);
			l->setThickness(line_width);
			l->setColor(line_color);
			l->imprint();
			unbindFBO();
   }


  Color COLOR(const char *color_string){
      if(!display){
       cout << "You must first call initCanvas before using any graphics features.\n";
       exit(1);
      }

    	COLR *c = new COLR();
      glm::vec3 v = c->getColor(color_string);
    	
    	int r = v[0];
    	int g = v[1];
    	int b = v[2];
       
    	unsigned long long int cc = r << 16 | g << 8 | b;
    	return cc;
	
   }

  Color COLOR(unsigned int red, unsigned int green, unsigned int blue){
    Color clr = red << 16 | green << 8 | blue;
    return clr;
  }

 void drawCircle(XPoint center, float r_orig, int num_seg, Color fill_color, bool fill, unsigned int line_width, int line_style, int cap_style, int join_style, int function){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, 0, screen_height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);    
    
    float r,g,b;
    int rr,gg,bb;
    rr = fill_color & 0xFF0000; rr = rr >> 16;
    
    gg = fill_color & 0x00FF00; gg = gg >> 8;
    bb = fill_color & 0x0000FF;
    r = rr / 255.0;
    g = gg / 255.0;
    b = bb / 255.0;
 
    glColor3f(r,g,b); 
    if(fill) glBegin(GL_POLYGON);
    else     glBegin(GL_LINE_LOOP);
    
    for(int i = 0; i < num_seg; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_seg);

        float X = r_orig * cosf(theta);
        float Y = r_orig * sinf(theta);

        glVertex3f(X + center.x ,screen_height -  (Y + center.y),0.0f);
    }

    glEnd();
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
}


void drawEllipse(XPoint center, float r_orig, float y_orig, int num_seg, Color fill_color, bool fill, unsigned int line_width, int line_style, int cap_style, int join_style, int function){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, 0, screen_height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);    
    
    float r,g,b;
    int rr,gg,bb;
    rr = fill_color & 0xFF0000; rr = rr >> 16;
    
    gg = fill_color & 0x00FF00; gg = gg >> 8;
    bb = fill_color & 0x0000FF;
    r = rr / 255.0;
    g = gg / 255.0;
    b = bb / 255.0;
 
    
    
    float theta = 2 * 3.1415926 / float(num_seg); 
    float c = cosf(theta);//precalculate the sine and cosine
    float s = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0 
    float y = 0; 

    glColor3f(r,g,b); 
    if(fill) glBegin(GL_POLYGON);
    else     glBegin(GL_LINE_LOOP);

    for(int ii = 0; ii < num_seg; ii++) 
    { 
        //apply radius and offset
        glVertex2f(x * r_orig + center.x, screen_height - ( y * y_orig + center.y));//output vertex 

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    } 
    glEnd(); 
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    
}


void drawText(){
	GLint ctx, myFont;
	ctx = glcGenContext();
	glcContext(ctx);	
	myFont = glcGenFontID();
	glcNewFontFromFamily(myFont, "Courier");
	glcFont(myFont);
	glEnable(GL_LINE_SMOOTH);
	glViewport(0, 0, screen_width, screen_height);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();  
	glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);        
	glOrtho(0.0, screen_width, screen_height, 0.0, -1.0, 1.0); 
	glMatrixMode(GL_MODELVIEW); 
	
	glPushMatrix();	
	
	glColor3f(0,0,0);
	glcScale(15,15);
	
	std::multiset<Text *, LtSprite>:: iterator textIter;

	for(textIter = textSet.begin(); textIter != textSet.end(); textIter++){
		if((*textIter)->isVisible()){
			glcScale((*textIter)->getScale(), (*textIter)->getScale());
			glcRotate((*textIter)->getAngle());
			float r,g,b;
		    int rr,gg,bb;
		    Color fill_color = (*textIter)->getColor();
		    rr = fill_color & 0xFF0000; rr = rr >> 16;
		    
		    gg = fill_color & 0x00FF00; gg = gg >> 8;
		    bb = fill_color & 0x0000FF;
		    r = rr / 255.0;
		    g = gg / 255.0;
		    b = bb / 255.0;
		    glColor3f(r,g,b);
			int n = (*textIter)->getMessage().length();
			XPoint xp = (*textIter)->getPos();
			char ss[n+1]; 
			strcpy(ss, (*textIter)->getMessage().c_str()); 
			glRasterPos3f(xp.x,xp.y,0);
	    	glcRenderString(ss);
	    	xp.x += 50;
	    	xp.y += 50;
    	}
    	glcRotate(-(*textIter)->getAngle());
    	glcScale(1/(*textIter)->getScale(),1/(*textIter)->getScale());
    }
	glPopMatrix();
	
}

  void drawPolygon(XPoint *points, int npoints, Color fill_color, bool fill, unsigned int line_width, int line_style, int cap_style, int join_style, int fill_rule, int function){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screen_width, 0, screen_height, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glPushAttrib(GL_DEPTH_TEST);
    glDisable(GL_DEPTH_TEST);
    
    float r,g,b;
    int rr,gg,bb;
    rr = fill_color & 0xFF0000; rr = rr >> 16;
    
    gg = fill_color & 0x00FF00; gg = gg >> 8;
    bb = fill_color & 0x0000FF;
    r = rr / 255.0;
    g = gg / 255.0;
    b = bb / 255.0;
 
    
    glColor3f(r,g,b); 
    if(fill) glBegin(GL_POLYGON);
    else     glBegin(GL_LINE_LOOP);
      for(int iter=0;iter<npoints;iter++){
      glVertex3f(points[iter].x, screen_height -  points[iter].y, 0.0);   
    }
    glEnd();
    
    glPopAttrib();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

  }

  void addSprite(Sprite *t){
    if(t){
       spriteSet.insert(t);
    }
  }
   void addText(Text *t){
    if(t){
       textSet.insert(t);
       repaint();
    }
  }

  void removeSprite(Sprite *t){
    if(t){
      for( iter = spriteSet.begin(); iter != spriteSet.end(); iter ++){
	if((*iter) == t){spriteSet.erase(iter); break;}
      } 
    }
    repaint();
  }

  // void c_imprint(Sprite* s){
		// Sprite *copy = s->clone();
  //   copySprite.insert(copy);
  // }
  
 void c_imprint(Sprite* s){
    bindFBO();
    s->paint();
    unbindFBO();
  }
bool globalRepaintFlag = true;
  void beginFrame(){  globalRepaintFlag = false;  }
  void endFrame(){  
  	globalRepaintFlag = true;
    repaint();
  }

  void repaint(){
    if(!display){
      cout << "Repaint: You must first call initCanvas before using any graphics features.\n";
      exit(1);
    }
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glColor3f(0,0,0);
    map<GLuint, vector<int> > :: iterator gtext_iter;
    drawRect(0,0,screen_width,screen_height,renderedTexture);
    if(!Gtexture.empty()){
	
    for(gtext_iter=Gtex.begin(); gtext_iter != Gtex.end(); gtext_iter++){
		  drawRect(gtext_iter->second[0], screen_height - gtext_iter->second[1], gtext_iter->second[2], screen_height - gtext_iter->second[3], gtext_iter->first);
	 }
	}
    for(iter = spriteSet.begin(); iter != spriteSet.end(); iter++){
	       
  	 (*iter)->paint();
    }
    drawText();
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  void nextEvent(XEvent &event){
    while(1){
    	glfwWaitEvents();

  	    if(callback){
  	    	event = eve;
  	    	break;
  	    }  		
    }
  }
  

  char charFromEvent(XEvent &event){
  	event = eve;
    return event.key;
  }


  void echoKey(XEvent &event, Color clr){
    char _c_ = charFromEvent(event);
    Text *a = new Text(event.xkey.x, event.xkey.y, string(1,_c_));
  }    


  int textWidth(char op){
    string s;
    s = op;
    return  textWidth(s); 
  }

  int textWidth(string text){ return text.size()*9; }
  int textHeight(){ return 15; }

  void spriteStatus(){
    cout<<"Count: "<<spriteSet.size()<<endl;
    for( iter = spriteSet.begin(); iter != spriteSet.end(); iter ++){
      cout<<"["<<"]-->"<<(*iter)<<endl;
    }
  }

  bool checkEvent(XEvent &event){
  	glfwPollEvents();
  	event = eve;
    if(event.type != 0)
      return true;
    return false;
  }

   bool mouseDragEvent(XEvent &event){
   	 event = eve;
     return event.type == MotionNotify;
   }

  bool mouseButtonPressEvent(XEvent &event){
	 
  	return event.type == ButtonPress;
  }

  bool mouseButtonReleaseEvent(XEvent &event){
		
    return event.type == ButtonRelease;
  }

  bool keyPressEvent(XEvent &event){
		
    return event.type == KeyPress;
  }

 void screenshot(char* filename){
 	repaint();
	SOIL_save_screenshot(filename,1,0,0,screen_width, screen_height);
  }


}