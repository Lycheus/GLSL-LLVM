#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qdrawutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#include<fstream>
#include <iostream>
#include<iomanip>

#include <string.h>

using namespace std; 

//#define MAX_WIDTH 256
//#define MAX_HEIGHT 256
//cube,morphing_ball.
#define ocean
#ifndef ocean
#define MAX_WIDTH 320
#define MAX_HEIGHT 240
#else
#define MAX_WIDTH 320
#define MAX_HEIGHT 320
#endif

//#define MAX_WIDTH 512
//#define MAX_HEIGHT 512

class DrawView : public QWidget {
public:
	void setPointX(unsigned addr);
	void setPointY(unsigned addr);
	DrawView(int w,int h,char  *ptr) {
	  width=w;
	  height=h;
	  setBackgroundColor(white);  // 背景為白色
	  resize(width, height);
	  framefile=ptr;
	}

protected:
	void drawShapes(QPainter *p);
	void paintEvent(QPaintEvent*);
	bool readshm(QPainter *p);
private:
	int width,height;
	int x,y;
	char  *framefile;
};
void DrawView::setPointX(unsigned addr){
  //fixed by sheng-chih
  //addr/=4;
  //x = addr%width;
  //y = addr/width;
  //cout << x << "," << y << endl;
  x = addr;
  cout << x << ",";
  //end fixed

}
void DrawView::setPointY(unsigned addr){
  //fixed by sheng-chih
  //addr/=4;
  //x = addr%width;
  //y = addr/width;
  //cout << x << "," << y << endl;
  
  //y = addr;
  y=MAX_HEIGHT-addr;
  cout <<  y << endl;
  //end fixed
}
void DrawView::drawShapes(QPainter *p) {

		p->save();  // 儲存繪圖裝置狀態
		printf("draw point\n");
//		p->setPen(QPen(Qt::blue, 4, DotLine));  // 藍色、4pixel大的點狀畫筆
//		p->drawLine(10, 10, 14, 10); // 由(x, y)繪到(x+w, y)
//		while(readshm(p));
		readshm(p);
		p->restore(); // 復原繪圖裝置狀態


}

// 在paintEvent事件函式中呼叫，就不怕圖形被覆蓋了
void DrawView::paintEvent(QPaintEvent*) {
    QPainter p(this);
//		readshm(&p);
		drawShapes(&p);

}

bool DrawView::readshm(QPainter *p)
{
  unsigned long addr=0,color;
  //bool count=1;
  ifstream fin;
  string ansi;
  char *p_tok;
  
  
  fin.open(framefile);
  //將資料轉成位址與顏色
  if(fin.is_open()){
    while(getline(fin,ansi,'\n')){
      //sprintf (buffer, "%u", atol(ansi.c_str()));//for debug
      //if(count){
      cout << ansi << endl;
      p_tok=strtok((char *)ansi.c_str()," \n");
      //addr=atol(ansi.c_str());
      addr=atol(p_tok);
      setPointX(addr);
      p_tok=strtok(NULL," \n");
      addr=atol(p_tok);
      setPointY(addr);
      //}else{
      p_tok=strtok(NULL," \n");
      //color=strtoul(ansi.c_str(),NULL,10);
      color=strtoul((const char*)p_tok,NULL,10);
      cout << color << endl;
      p->setPen(QPen(qRgba((color & 0x000000FF),(color & 0x0000FF00)>>8,(color & 0x00FF0000)>>16,255), 1, DotLine)); 
      //p->setPen(QPen((QColor)(color & 0x00b0b0b0), 2, DotLine));  // 藍色、2pixel大的點狀畫筆
      cout<<"R: "<<(color & 0x000000FF)<<"\tG: "<<((color & 0x0000FF00)>>8)<<"\tB: "<<((color & 0x00FF0000)>>16) <<endl;
      
      //}
      p->drawLine(x, y, x+1, y+1); // 由(x, y)繪到(x+w, y)
      //count=!count;
    }
  }else{
    std::cout << framefile << "File Miss" << endl;
  }
  return 1;
  
}

int main(int argc, char **argv) {
	printf("start QT\n");
	QApplication app(argc, argv);
	DrawView *draw = new DrawView(MAX_WIDTH,MAX_HEIGHT,argv[1]);
	//DrawView *draw = new DrawView(255,255,argv[1]);
	draw->setCaption("QT Display for SystemC");
	app.setMainWidget(draw);
	draw->show();
	draw->setUpdatesEnabled(true);
	return app.exec();

} 


