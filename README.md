### PRINTER_ESCPOS_USB

- Print Mode
- Justification
- HRI positioning
Reset
Line feed
QRCode printing

Setup

```bash
$wget https://github.com/libusb/libusb/releases/download/v1.0.23/libusb-1.0.23.tar.bz2
tar -jxvf libusb-1.0.23.tar.bz2

cd /libusb-1.0.23
./configure
make
make install

*** After libusb compiled, move libs for path Qt:
$mv /usr/local/lib/libusb-1.0.* /usr/local/qt5pi/lib/
```

````c++
      EscPosUSB *printer = new EscPosUSB();
      printer->connectPrinter(0x04b8, 0x0e27);
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
      printer->qrCode("http://www.artmachine.com.br", EscPosUSB::Model::MicroQRCode, EscPosUSB::ModelSize::S1, EscPosUSB::ErrorCorrection::H);
      printer->cut();
      printer->printer();
      
````
![alt text](https://github.com/m4rc0nd35/EscPosUSB/blob/main/cupom.png)
