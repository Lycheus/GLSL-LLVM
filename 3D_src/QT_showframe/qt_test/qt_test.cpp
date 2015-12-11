#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qpen.h>
#include <qpixmap.h>
#include <qdrawutil.h>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include<fstream>
#include <iostream>
#include<iomanip>
using namespace std; 

#define SEGSIZE 30720

class DrawView : public QWidget {
public:
	void setPoint(unsigned addr);
	DrawView(int w,int h,char  *ptr) {
			width=w;
			height=h;
			setBackgroundColor(white);  // �I�����զ�
			resize(width, height);
			segptr=ptr;
	}

protected:
	void drawShapes(QPainter *p);
	void paintEvent(QPaintEvent*);
	bool readshm(QPainter *p);
private:
	int width,height;
	int x,y;
	char  *segptr;
};
void DrawView::setPoint(unsigned addr){
	addr/=4;
	x = addr%width;
	y = addr/width;
//	cout << x << "," << y << endl;
}
void DrawView::drawShapes(QPainter *p) {

		p->save();  // �x�sø�ϸ˸m���A
		printf("draw point\n");
//		p->setPen(QPen(Qt::blue, 4, DotLine));  // �Ŧ�B4pixel�j���I���e��
//		p->drawLine(10, 10, 14, 10); // ��(x, y)ø��(x+w, y)
		while(readshm(p))
			;
		p->restore(); // �_��ø�ϸ˸m���A


}

// �bpaintEvent�ƥ�禡���I�s�A�N���ȹϧγQ�л\�F
void DrawView::paintEvent(QPaintEvent*) {
    QPainter p(this);
//		readshm(&p);
		drawShapes(&p);

}

bool DrawView::readshm(QPainter *p)
{
	string str=string(segptr);
	unsigned long addr=0,color;
	unsigned int i = 0;
	int pre,len;
	bool count=1;
//�P�_�O�_���s��Ƽg�J
	len = str.length();
	//cout << "the size of share memory is " << len << endl;
	if(!len){
	  usleep(50);
	  *(segptr)='f';
	  return 1;
	}
/*	while(1){
	  if(*(segptr)=='f')
 	    usleep(50);
	  else
	    break;
	  cout << "share memory is " << str << endl;
	} 
*/
	if(*(segptr)=='q')	return 0;
//�N����ন��}�P�C��
	while ( len ) {
		pre=i;
		i = str.find(",",i) ;
		if ( i == string::npos ) break;
		if(count){
			addr=atol(str.substr(pre,i-pre).c_str());
			setPoint(addr);
//			cout << addr << endl;
		}else{
			color=strtoul(str.substr(pre,i-pre).c_str(),NULL,10);
//			cout << str.substr(pre,i-pre).c_str() << endl;
//			p->setPen(QPen((QColor)(color & 0x00b0b0b0), 2, DotLine));  // �Ŧ�B2pixel�j���I���e��
//			p->setPen(QPen(qRgba(0,0,255,255), 2, DotLine));  // RGBA
			p->setPen(QPen(qRgba((color & 0x000000FF),(color & 0x0000FF00)>>8,(color & 0x00FF0000)>>16,255), 1, DotLine));  
//			p->setPen(QPen((QColor)(color & 0xFFFFFF00), 2, DotLine));  // �Ŧ�B2pixel�j���I���e��
//			p->setPen(QPen((QColor)(color & 0x00FFFFFF), 2, DotLine));  // �Ŧ�B2pixel�j���I���e��
//			setColor(color);
//			cout <<std::hex <<color << endl;
		}
		
//		p->setPen(QPen(Qt::blue, 2, DotLine));  // �Ŧ�B2pixel�j���I���e��
		p->drawLine(x, y, x+1, y+1); // ��(x, y)ø��(x+w, y)

		count=!count;
		i ++ ;
	}
//�]�wŪ������
//	strcpy(segptr, "f");
	*segptr='f';
	return 1;

}

int main(int argc, char **argv) {
	key_t key;
	int   shmid;
	char  *segptr;

	/* Create unique key via call to ftok() */
	key = ftok(".", 'S');

	/* Open the shared memory segment - create if necessary */
	if((shmid = shmget(key, SEGSIZE, IPC_CREAT|IPC_EXCL|0666)) == -1) 
	{
		printf("Shared memory segment exists - opening as client\n");

		/* Segment probably already exists - try as a client */
		if((shmid = shmget(key, SEGSIZE, 0)) == -1) 
		{
			perror("shmget");
			exit(1);
		}
	}
	else
	{
		printf("Creating new shared memory segment\n");
	}

	/* Attach (map) the shared memory segment into the current process */
	if((segptr = (char *)shmat(shmid, 0, 0))==(char *)-1)
	{
		perror("shmat");
		exit(1);
	}
	printf("start QT\n");
	QApplication app(argc, argv);
	DrawView *draw = new DrawView(320,320,segptr);
	draw->setCaption("QT Display for SystemC");
	app.setMainWidget(draw);
	draw->show();
	draw->setUpdatesEnabled(true);
	return app.exec();

} 


