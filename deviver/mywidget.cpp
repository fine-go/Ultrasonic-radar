#include "mywidget.h"
#include "ui_mywidget.h"
#include <QDebug>
#include "QtCore/qmath.h"



# define pi 360
myWidget::myWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWidget)
{
    ui->setupUi(this);
    myPainter = new QPainter(this);
    this->setWindowTitle("雷达站");
    resize(400, 400);
}

myWidget::~myWidget()
{

    delete ui;
}

void myWidget::count(QByteArray buffer)
{
    mybuffer.append(buffer);
    bool isfoundA = false;
    bool isfoundD = false;
    QByteArray temp;

    int len = 0;
    if(mybuffer.length()>2)
    {
        char *date = mybuffer.data();
        // *data 表示的是第一个字符
       // qDebug() << "147 *deta"<< *date;
        /*================A=================*/
        if(*date == 'A')
        {
            len = 1;
            if (len<(mybuffer.length()))
                ++date;
            //qDebug() << "152 *deta"<< *date;
            while(*date != 'A' && len <= (mybuffer.length()-1))
            {
                len++;
                temp.append(*date);
                ++date;
                //qDebug() << "158*deta"<< *date;
                if (*date == 'A')
                {
                    isfoundA = true;
                }
            }
            //qDebug() << "160 len" <<len;
            //qDebug() << "161 temp "<<temp;
            //qDebug() << "164 *deta"<< *date;
            if(len != 0 && isfoundA)
            {
                mybuffer.remove(0,len+1);
                //qDebug() << "ok";
                numA = temp;
                //qDebug() << "numA" << numA;
                QString t = "numA :"+numA;
                ui->textEditsend->append(t);
            }
           // qDebug() << "170 *deta"<< *date;

            temp.clear();
            len = 0;
        }
       // qDebug() << "167 "<<mybuffer;
       // qDebug() << "177"<<mybuffer.length();
        /*================D=================*/
        if(mybuffer.length()>0)
        {
            char *date = mybuffer.data();
            if(*date == 'D')
            {
                len = 1;
                if (len<(mybuffer.length()))
                    ++date;
                //qDebug() << "152 *deta"<< *date;
                while(*date != 'D' && len <= (mybuffer.length()-1))
                {
                    len++;
                    temp.append(*date);
                    ++date;
                    //qDebug() << "158*deta"<< *date;
                    if (*date == 'D')
                    {
                        isfoundD = true;
                    }
                }
                //qDebug() << "160 len" <<len;
                //qDebug() << "161 temp "<<temp;
                    //qDebug() << "164 *deta"<< *date;
                if(len != 0 && isfoundD)
                {
                    mybuffer.remove(0,len+1);
                    //qDebug() << "ok";
                    numD = temp;
                   // qDebug() << "numD" << numD;
                    QString t = "numD :"+numD;
                    ui->textEditsend->append(t);
                }
                //qDebug() << "170 *deta"<< *date;

                temp.clear();
                len = 0;
            }
            //qDebug() << "167 "<<mybuffer;
            //qDebug() << "177"<<mybuffer.length();
          }
    }
   // qDebug() << "mybuffer "<<mybuffer;
   // qDebug() << "mybuffer len"<<mybuffer.length();
    scan();
}

void myWidget::paintEvent(QPaintEvent *)
{
    //第一次绘图，画出背景表格和显示出角度等信息
    // 将背景设为黑色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::black);
    this->setPalette(palette);
    myPainter->begin(this);
    //定义画笔
    QPen myPen;
    myPen.setWidth(5);
    myPen.setColor(Qt::green);
    myPainter->setPen(myPen);
    //设置圆心坐标
    int w = this->width();
    int h = this->height();
    x = w/2;
    y = int(h*0.9);
    center = QPoint(x,y);
    //qDebug() <<QString("x = %1").arg(x);
    //qDebug() <<QString("y = %1").arg(y);
    //画圆
    int startAngle = 0 * 16;
    int spanAngle = 180 * 16;
    int R_max = 0;
    for(int i = 1;i<5;i++)
    {
        int R = y/4*i;
        //qDebug() <<QString("R = %1").arg(R);
        QRectF rectangle(x-R, y-R, 2*R,2*R );
        myPainter->drawArc(rectangle, startAngle, spanAngle);
        myPainter->setFont(QFont("myFont",18 ));
        QString s = QString::number(20*i, 10);
       // qDebug() << s;
        myPainter->drawText(x+R-8, y+25,QString(s));
        myPainter->drawText(x-R+8, y+25,QString(s));
        if(i == 3)
          {
            R_max = R;
            countf = R_max/600.0;
        }
    }
    //画射线，每30度分
    for(int i = 0;i<7;i++)
    {
        QLineF myLine;
        myLine.setP1(QPoint(x, y));
        myLine.setAngle(30*i);
        myPainter->drawLine(myLine);
    }
    for(int i = 0;i<7;i++)
    {
        QString s = QString::number(30*i, 10);
       // qDebug() << s;
        QPointF o(x+((R_max*1.05)*qCos(qDegreesToRadians(30.0*i-2))) , y-((R_max*1.05)*qSin(qDegreesToRadians(30.0*i-2))));
       // qDebug() << o;
        myPainter->drawText(o,s);
    }
    myPainter->end();

    if(initflag)
    {
        myPainter->begin(this);
        myPainter->setPen(myPen);
        myPainter->setFont(QFont("myFont",18 ));
        QString a = QString::number(numA.toInt(), 10);
        QString d = QString::number(numD.toInt(), 10);
        QString s = "Angle: "+a+"     "+"Disence: "+d+" mm";
        myPainter->drawText(int(0.6*w), int(0.95*h), s);
        myPainter->end();

        if(first)
        {
            first = false;
            currentAlpha = 255;
            for(int i=0;i<lineNum;i++)
            {
                //颜色渐变
                QColor currentColor = startColor;
                currentColor.setAlpha(currentAlpha);
                QColor currentColor2 = startColorLine2;
                currentColor2.setAlpha(currentAlpha);
                currentAlpha -= int(255/lineNum) ;
                //画绿线
                //i = 0时表示的是最前面的线
                myPainerLine[i].begin(this);
                myPenLine[i].setWidth(foutwight);
                myPenLine[i].setColor(currentColor);
                myPainerLine[i].setPen(myPenLine[i]);
                myLine[i].setP1(center);
                QPointF temp = getPoint(center, numA.toInt(), numD.toInt());
                myLine[i].setP2(temp);
                myPainerLine[i].drawLine(myLine[i]);
                myPainerLine[i].end();
                //画红线
                myPainerLine2[i].begin(this);
                myPenLine2[i].setWidth(foutwight);
                myPenLine2[i].setColor(currentColor2);
                myPainerLine2[i].setPen(myPenLine2[i]);
                myLine2[i].setP1(temp);
                myLine2[i].setAngle(myLine[i].angle());
                myPainerLine2[i].drawLine(myLine2[i]);
                myPainerLine2[i].end();
                numAngle[i] = myLine[i].angle();
            }
        }
        else {
            currentAlpha = 0;
            for(int i=lineNum-1;i>=0;i--)
            {
                //颜色渐变
                QColor currentColor = startColor;
                currentColor.setAlpha(currentAlpha);
                QColor currentColor2 = startColorLine2;
                currentColor2.setAlpha(currentAlpha);
                currentAlpha += int(255/lineNum) ;
                //画绿线
                //i = 0时表示的是最前面的线
                myPainerLine[i].begin(this);
                myPenLine[i].setWidth(foutwight);
                myPenLine[i].setColor(currentColor);
                myPainerLine[i].setPen(myPenLine[i]);
                myLine[i].setP1(center);
                //得到当前的角度---上一个射线的角度
                if(i == 0)
                {
                    numAngle[i] = numA.toInt();
                }
                else {
                  numAngle[i] = numAngle[i-1];
                }
                qDebug() <<"i = "<< i<<"angle"<<numAngle[i];

                QPointF temp = getPoint(center,  numAngle[i], numD.toInt());
                                qDebug() <<"i = "<< i<<temp;
                myLine[i].setP2(temp);
                myPainerLine[i].drawLine(myLine[i]);
                myPainerLine[i].end();
                //画红线
                myPainerLine2[i].begin(this);
                myPenLine2[i].setWidth(foutwight);
                myPenLine2[i].setColor(currentColor2);
                myPainerLine2[i].setPen(myPenLine2[i]);
                myLine2[i].setP1(temp);
                myLine2[i].setAngle( numAngle[i]);
                myPainerLine2[i].drawLine(myLine2[i]);
                myPainerLine2[i].end();
            }
        }
    }//end if(init_flag)

}

void myWidget::scan()
{
   if(!numA.isEmpty() and !numD.isEmpty())
   {
       initflag = true;
       update();
   }
}

void myWidget::on_pushButtonSearch_clicked()
{
    //查找串口，显示在box里面
    //foreach 函数在C++标准中没有此关键字，此为QT的关键字
    //用法为foreach(变量，容器)，info里面存放了可用端口的信息
    ui->portName->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
       {
          ui->portName->addItem(info.portName());
       }
    //同时给串口对象分配空间
    myserial = new QSerialPort();
    ui->textEditRvcf->setTextColor(Qt::white);
    ui->textEditsend->setTextColor(Qt::white);


}

void myWidget::on_pushButtonOpen_clicked()
{
    //打开串口
    //设置串口名
    myserial->setPortName(ui->portName->currentText());

    //设置波特率
    myserial->setBaudRate(ui->botelv->currentText().toInt());
    //设置校验位
    switch(ui->jiaoyanwei->currentIndex())
    {
        case 0:
        {
            myserial->setParity(QSerialPort::NoParity);
            break;
        }
    default:
        break;
    }
    //设置数据位
    switch (ui->databit->currentIndex())
    {
        case 0:
        {
            myserial->setDataBits(QSerialPort::Data8);
            break;
        }
    default:
        break;
    }
    //设置停止位
    switch(ui->stopbit->currentIndex())
    {
        case 0:
        {
            myserial->setStopBits(QSerialPort::OneStop);
            break;
        }
    default:
        break;
    }

    //设置流控制
    myserial->setFlowControl(QSerialPort::NoFlowControl);

    //设置串口名之后打开串口
    bool open_flag = myserial->open(QIODevice::ReadWrite);
    if(!open_flag)
    {
        ui->textEditRvcf->append("open fail");
    }
    else
    {
        myserial->setDataTerminalReady(true);
        //打开按钮和区域失效
        ui->portName->setEnabled(false);
        ui->botelv->setEnabled(false);
        ui->databit->setEnabled(false);
        ui->stopbit->setEnabled(false);
        ui->jiaoyanwei->setEnabled(false);

        ui->pushButtonOpen->setEnabled(false);
        ui->pushButtonSearch->setEnabled(false);

        connect(myserial, SIGNAL(readyRead()),this, SLOT(recive()));

        send(true);
      }

}

void myWidget::on_pushButtonSend_clicked()
{
    //发送数据
    if(!myserial->isOpen())
        return;
    else
    {
        QByteArray temp = ui->textEditsend->toPlainText().toLatin1();
        //qDebug() << temp;
        qint64 flag = myserial->write(temp);
        //qDebug() << flag;

    }
}

void myWidget::recive()
{
    //如何处理黏包问题
    //qDebug() << "recive";
    //从接收缓冲区中读取数据
    QByteArray buffer = myserial->readAll();
    //从界面中读取以前收到的数据
   // QString recv = ui->textEditRvcf->toPlainText();
   // recv += QString(buffer);
   // //清空以前的显示
   // ui->textEditRvcf->clear();
    //重新显示
    ui->textEditRvcf->append(buffer);
    count(buffer);
}

void myWidget::send()
{
    //发送数据
    if(!myserial->isOpen())
        return;
    else
    {
        QByteArray temp = ui->textEditsend->toPlainText().toLatin1();
        qDebug() << temp;
        qint64 flag = myserial->write(temp);
        qDebug() << flag;

    }
}

void myWidget::send(bool temp)
{
    //发送数据
    if(!myserial->isOpen())
        return;
    else
    {
        if(temp)
        {
            QByteArray temp = "o";
            //qDebug() << "temp" <<temp;
            qint64 flag = myserial->write(temp);
            qDebug() << "glsh"<< flag;
        }

    }
}

void myWidget::on_pushButtonClose_clicked()
{
    if(!myserial->isOpen())
        return;
    else
    {
        myserial->close();

        ui->portName->setEnabled(true);
        ui->botelv->setEnabled(true);
        ui->databit->setEnabled(true);
        ui->stopbit->setEnabled(true);
        ui->jiaoyanwei->setEnabled(true);

        ui->pushButtonOpen->setEnabled(true);
        ui->pushButtonSearch->setEnabled(true);

        ui->textEditRvcf->clear();
        ui->textEditsend->clear();
    }
}

void myWidget::on_pushButtonClear_clicked()
{
    ui->textEditRvcf->clear();
    ui->textEditsend->clear();
}

QPointF myWidget::getPoint(QPoint o, double angle, int Distence)
{
    double x;
    double y;

    x = o.x() + Distence*countf*qCos(qDegreesToRadians(angle));
    y = o.y() - Distence*countf*qSin(qDegreesToRadians(angle));
    //qDebug() << "countf" << countf;
    //qDebug() << QPoint(int(x), int(y));
    return QPointF(x, y);
}
