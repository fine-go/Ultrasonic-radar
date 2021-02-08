#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QPaintDevice>
#include <QPainter>
#include <QPen>
#include <QtMath>
#include <QTimer>
#include <QSerialPort>
#include <QSerialPortInfo>

#define lineNum 90
#define foutwight 3

namespace Ui {
class myWidget;
}

class myWidget : public QWidget
{
    Q_OBJECT

public:
    explicit myWidget(QWidget *parent = nullptr);
    ~myWidget();

private slots:
    void on_pushButtonSearch_clicked();

    void on_pushButtonOpen_clicked();

    void on_pushButtonSend_clicked();

    void recive();

    void on_pushButtonClose_clicked();
    void on_pushButtonClear_clicked();

private:
    Ui::myWidget *ui;

    QPainter *myPainter = nullptr;

    bool flag = false;
    bool initflag = false;
    bool first = true;
    double V = 12.0;

    int x;
    int y;
    double angle;

    QPainter myPainerLine[lineNum];
    QPen myPenLine[lineNum];
    QLineF myLine[lineNum];
    QPainter myPainerLine2[lineNum];
    QPen myPenLine2[lineNum];
    QLineF myLine2[lineNum];
    int flag_status = 1;
    double timer = 0.2;
    QColor startColor = Qt::green;
    QColor startColorLine2 = Qt::red;
    int currentAlpha = 255;
    double countf;
    QPoint center;
    double numAngle[lineNum];

    void refreshPort();
    void send();
    void send(bool temp);
    void count(QByteArray buffer);
    void scan();
    QPointF getPoint(QPoint o, double angle, int discense);

    QByteArray numA;
    QByteArray numD;

    QByteArray mybuffer;
    QSerialPort *myserial;

protected:
    void paintEvent(QPaintEvent *);




};

#endif // MYWIDGET_H
