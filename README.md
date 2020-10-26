### ESCPOSUSB

>Qt native usb printer EPSON TM 20x Esc/Pos thermal with library libusb in raspberry.

**Supports**
- [x]  Print Mode
- [x] Justification
- [x] HRI
- [x] Reset
- [ ] Line feed
- [x] end line
- [x] QRCode printing
- [ ] Print image jpg/png
- [x] font size
- [x] font style
- [x] image stored flash printer
- [x] QML

**Setup install**

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

![alt text](https://github.com/m4rc0nd35/EscPosUSB/blob/main/cupom.png)
