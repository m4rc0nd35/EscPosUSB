#include "escposusb.h"
using namespace std;
EscPosUSB::EscPosUSB(QObject *parent) : QObject(parent)
{
//    m_codec = QTextCodec::codecForName("UTF-8");
}
bool EscPosUSB::connectPrinter(int vendor, int product)
{
    bool connected = false;
    try{
        int r = libusb_init(&_context); //initialize a library session
        if(r < 0)
            throw "[EXCEPTION] Init Error ";

//        libusb_set_debug(_context, 3); //set verbosity level to 3, as suggested in the documentation
//        libusb_set_option(_context, LIBUSB_OPTION_LOG_LEVEL, 3);
        libusb_device **list = NULL;
        // get count devices
        unsigned int countDevices = libusb_get_device_list(_context, &list);
        // break device true
        for (size_t idx = 0; idx < countDevices; ++idx) {
            libusb_device *device = list[idx];
            libusb_device_descriptor desc;
            libusb_get_device_descriptor(device, &desc);
            // get device
            if(desc.idVendor == vendor && desc.idProduct == product){
                libusb_open(device, &_printer);
                connected = true;
                break;
            }
        }
        libusb_free_device_list(list, countDevices);
        //these are vendorID and productID I found for my usb device
        if(_printer == NULL)
            throw "[EXCEPTION] Cannot open device";

        //find out if kernel driver is attached
        if(libusb_kernel_driver_active(_printer, 0) == 1) {
            qDebug() << "Kernel Driver Active" << endl;
            if(libusb_detach_kernel_driver(_printer, 0) == 0) //detach it
                throw "[EXCEPTION] Kernel Driver Detached!";
        }
        //claim interface 0 (the first) of device (mine had jsut 1)
        r = libusb_claim_interface(_printer, 0);
        if(r < 0)
            throw "[EXCEPTION] claim interface -1";
    } catch (const char* e){
        connected = false;
        qDebug() << e << endl;
        throw e;
    }
    return connected;
}
void EscPosUSB::qrCode(QByteArray data, Model model, ModelSize moduleSize, ErrorCorrection erroCorrection)
{
    int iTam = data.length() + 4;
    int pL = (iTam % 256) ;
    int pH = iTam >> 8;
    int k = (pL + pH % 256) - 3;
    // function 165 Model
    char mT[9] = {GS, '(', 'k', 0x04, 0x00, 0x31, 0x41, char(model), 0x0};
    _buffer.append(mT,9);
    // function 167 size of module
    const char mS[8] = {GS, '(', 'k', 0x03, 0x00, 0x31, 0x43, char(moduleSize)};
    _buffer.append(mS,8);
    // function 169
    // L = 0, M = 1, Q = 2, H = 3
    const char eL[8] = {GS, '(', 'k', 0x03, 0x00, 0x31, 0x45, char(erroCorrection)};
    _buffer.append(eL,8);
    // function 180 QR Store
    const char dH[8] = {GS, '(', 'k', char(pL), char(pH), 0x31, 0x50, 0x30};
    _buffer.append(dH,8);
    _buffer.append(data.mid(0, 7092));
    _buffer.append(k);
    // function 181 Print QR code
    const char pT[8] = {GS, '(', 'k', 0x03, 0x00, 0x31, 0x51, 0x30};
    _buffer.append(pT,8);
}
void EscPosUSB::barCode(QByteArray data, Position position, FontFamily font, BarCodeType barcode, int height, int width)
{
    const char pP[3] = {GS, 'H', char(position)};
    _buffer.append(pP,3);
    const char pF[3] = {GS, 'f', char(font)};
    _buffer.append(pF,3);
    const char pH[3] = {GS, 'h', char(height)};
    _buffer.append(pH,3);
    const char pW[3] = {GS, 'w', char(width)};
    _buffer.append(pW,3);
    const char pK[3] = {GS, 'k', char(barcode)};
    _buffer.append(pK,3);
    _buffer.append(data.count());
    _buffer.append(data);
}
void EscPosUSB::endline()
{
    _buffer.append(LF);
}
void EscPosUSB::image(QByteArray img)
{
    /* image("IM") image stored in printer flash */
    const char pI[7] = {GS, '(','L', ACK, 0x0, '0','E'};
    _buffer.append(pI,7);
    _buffer.append(img);
    const char pE[2] = {SOH, SOH};
    _buffer.append(pE,2);
}
void EscPosUSB::cut()
{
    _buffer.append(GS);
    _buffer.append("V");
    _buffer.append(66);
    _buffer.append(1);
}
void EscPosUSB::textList(QList<QString> l, bool c)
{
    for (int i=0; i < l.count(); i++)
    {
        _buffer.append(l[i]);
        if(c)
            cut();
        else
            if(l.count() > 1 && l.count() != (i+1))
            {
                text("----------------------------------------------------------------");
                endline();
            }
    }
}
void EscPosUSB::text(QByteArray s)
{
//    qDebug() << "string" << s << m_codec->fromUnicode(s);
    _buffer.append(s);
}
void EscPosUSB::setFontSize(Size s)
{
    char sT[3] = {GS, '!', char(s)};
    _buffer.append(sT,3);
}
void EscPosUSB::setStyles(Style mode)
{
    /* mode 0~5 */
    char sT[3] = {ESC, '!', char(mode)};
    _buffer.append(sT,3);
}
void EscPosUSB::setJustification(Align mode)
{
    /* mode 0~2 */
    char mA[3] = {ESC, 'a', char(mode)};
    _buffer.append(mA,3);
}
void EscPosUSB::reset()
{
    /* return status */
    char sT[3] = {DLE, EOT, SOH};
    _buffer.append(sT,3);
    /* reset */
    char rT[2] = {ESC, '@'};
    _buffer.append(rT,2);
}
void EscPosUSB::printer()
{
    try {
      std::string text = _buffer.toStdString();
        int totalChars = _buffer.length();
        int actual = 0;
        int r = libusb_bulk_transfer(_printer, (1 | LIBUSB_ENDPOINT_OUT), (unsigned char *) &text[0], totalChars, &actual, 0);
        if(r != 0 || actual != totalChars)
            throw "[EXCEPTION TRANSFER] Write Error";

        _buffer.clear();
    } catch (const char* e) {
        qDebug() << e << endl;
        throw e;
    }
}
void EscPosUSB::colsText(QList<int> cols, QStringList list, int limit)
{ // cols = [start, ..., stop], list = values, max char per colun(default 64)
    QString ret;
    for(int i = 0; cols.count() > i; i++)
        if((cols.count() - 1) == i)
            ret.insert((cols[i] - list[i].count()), list[i]);
        else
            ret.insert(cols[i], list[i].mid(0,limit));

    _buffer.append(ret);
}
void EscPosUSB::column(int x, int y, int c1, int c2)
{
    int iTam = (y + x + c1 + c2);
    int pL = (iTam % 256) ;
    int pH = iTam >> 8;
    _buffer.append(GS);
    _buffer.append("P");
    _buffer.append(pL); // pL
    _buffer.append(pH); // pH
    _buffer.append(9); // fn
    _buffer.append(y); // y
    _buffer.append(c1); // c1
    _buffer.append(c2); // c2
    _buffer.append(x); // d
}
