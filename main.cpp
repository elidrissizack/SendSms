/********
 *  Author : ELIDRISSIZAK
 *  Version : 1.0
 *
 *  Source Code C++ to send sms with a gsm modem C++
 *  FrameWork : QT
 *
 * */


#include <QCoreApplication>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <qdebug.h>
#include <qthread.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString namemodem;
    QSerialPortInfo infoport;
    foreach(    QSerialPortInfo info,infoport.availablePorts())
    {
        if(info.description() == "HUAWEI Mobile Connect - 3G PC UI Interface") // tape name of your modem gsm
            namemodem =  info.portName();
    }
    // initialize the modem
    QSerialPort serial;
    serial.setPortName("COM16");
    serial.open(QIODevice::ReadWrite);
    serial.setBaudRate(QSerialPort::Baud115200);
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::NoParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);

    if (serial.isOpen() && serial.isWritable())
    {
        // Text message mode
        QString output = "AT+CMGF=1\r\n";
        serial.write(output.toLatin1());
        serial.flush();
        serial.waitForBytesWritten(1000);

        // read the response of the modem
        serial.waitForReadyRead(200);
        qDebug() << serial.readAll() << "Response of the modem";

        // set phone number
        serial.write(QByteArray("AT+CMGS=\"+212620477XXX\"\r\n")); // tape phone number
        serial.flush();
        serial.waitForBytesWritten(1000);

        //set message
        serial.write(QByteArray("Tape text \x1A /r/n")); //tape message
        serial.flush();
        serial.waitForBytesWritten(1000);

        // get reponse
        serial.waitForReadyRead(200);
        qDebug() << serial.readAll() << "Final response";


        // close the communication
        serial.close();
        return 0;
    }
    else
    {
        qDebug() << "You have a problem";
        return 0;
    }

    return a.exec();
}
