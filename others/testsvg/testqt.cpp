#include <qapplication.h>
#include <qwidget.h>
#include <qpainter.h>
#include <qpicture.h>
#include <QtSvg>

class MyMainWindow:public QWidget
{
	public:
		MyMainWindow(const char *fname);
	
	protected:
		void paintEvent(QPaintEvent *);
	
	private:
		QPainter *paint;
		QImage *pic;
};

MyMainWindow::MyMainWindow(const char *fname)
{
	resize(200,200);
	paint = new QPainter();
	pic = new QImage();
	pic->load(fname,"svg");
}

void MyMainWindow::paintEvent(QPaintEvent *)
{
	paint->begin(this);
	paint->setWindow(pic->rect());
	paint->drawImage(QPoint(0,0), *pic);
	paint->end();
}

int main(int argc, char **argv)
{
	if (argc <= 1)
	{
		return 0;
	}
	
	QApplication a(argc,argv);
	
	MyMainWindow w(argv[1]);

	a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
	w.show();
	a.exec();

	return 1;
}

