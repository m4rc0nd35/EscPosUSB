### PRINTER_ESCPOS_USB


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
