#include <QCoreApplication>
#include "escposusb.h"

int main(int argc, char *argv[])
{
  QCoreApplication a(argc, argv);

  try{
      EscPosUSB *printer = new EscPosUSB();
      printer->connectPrinter(0x04b8, 0x0e27);
      qDebug() << "PRINTER INIT" << endl;
      printer->setJustification(EscPosUSB::Align::RIGHT);
      printer->text("justification");
      printer->endline();
      printer->setStyles(EscPosUSB::Style::Font2);
      printer->setJustification(EscPosUSB::Align::LEFT);
      printer->text("Maçã Ação");
      printer->endline();
      printer->setStyles(EscPosUSB::Style::Emphasized);
      printer->setJustification(EscPosUSB::Align::CENTER);
      printer->text("TESTE PrinterSerial");
      printer->endline();
      printer->setStyles(EscPosUSB::Style::Underline);
      printer->setJustification(EscPosUSB::Align::RIGHT);
      printer->text("TESTE PrinterSerial");
      printer->endline();
      printer->image("00");
      printer->endline();
      printer->barCode("123456789", EscPosUSB::Position::topBottom, EscPosUSB::FontFamily::A, EscPosUSB::BarCodeType::CODE39, 20, 3);
      printer->qrCode("http://www.mtsis.com.br", EscPosUSB::Model::MicroQRCode, EscPosUSB::ModelSize::S1, EscPosUSB::ErrorCorrection::H);
      printer->cut();
      printer->printer();
      qDebug() << "PRINTER END" << endl;
  }catch(const char* e){
      qDebug() << "EXCEPINFO" << e << endl;
  }

  return a.exec();
}
