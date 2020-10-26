#ifndef ESCPOSUSB_H
#define ESCPOSUSB_H
#include <QObject>
#include <QDebug>
#include <QTextCodec>
#include <libusb.h>
#include <exception>

#define NUL 0
#define SOH 0x01
#define EOT 0x04
#define ENQ 0x05
#define ACK 0x06
#define HT 0x09
#define LF 0x0A
#define FF 0x0C
#define CR 0x0D
#define DLE 0x10
#define DC4 0x14
#define CAN 0x18
#define ESC 0x1B
#define FS 0x1C
#define GS 0x1D

class EscPosUSB : public QObject
{
    Q_OBJECT

public:
    enum ModelSize {
        S1 = 49, S2, S3, S4, S5, S6, S7, S8,
        S9, S10, S11, S12, S13, S14, S15, S16
    };
    Q_ENUMS(ModelSize)
    enum Model {
        Model1 = 49,
        Model2,
        MicroQRCode,
    };
    Q_ENUMS(Model)
    enum ErrorCorrection {
        L = 48, // 7%
        M, // 15%
        Q, // 25%
        H, // 30%
    };
    Q_ENUMS(ErrorCorrection)
    enum Size {
        defaultSize = 0x0, heigthx2 = 0x01, heigthx3 = 0x02, heigthx4 = 0x03,
        heigthx5 = 0x04, heigthx6 = 0x05, heigthx7 = 0x06, heigthx8 = 0x07,
        widthx2 = 0x10, widthx3 = 0x20, widthx4 = 0x30, widthx5 = 0x40,
        widthx6 = 0x50, widthx7 = 0x60, widthx8 = 0x70
    };
    Q_ENUMS(Size)
    enum Position{
        noPrintCod = 0, top, bottom, topBottom
    };
    Q_ENUMS(Position)
    enum FontFamily{
        A, B, C, D, E
    };
    Q_ENUMS(FontFamily)
    enum BarCodeType{
        UPC_A = 66, UPC_B, JAN_EAN8, CODE39, ITF, CODABAR, CODE93, CODE128, GS1_128,
        GS1DataBarOmni, GS1DataBarTrunc, GS1DataBarLimited, GS1DataBarExpanded
    };
    Q_ENUMS(BarCodeType)
    enum Align{
        LEFT, CENTER, RIGHT
    };
    Q_ENUMS(Align)
    enum Style{
        OFF = 0x0, Font2 = 0x01, Emphasized = 0x08,
        DoubleHeight = 0x10, DoubleWidth = 0x20, Underline = 0x80
    };
    Q_ENUM(Style)
    enum Encoding {
        EncodingPC437 = 0,
        EncodingKatakana = 1,
        EncodingPC850 = 2,// Qt supported
        EncodingPC860 = 3,
        EncodingPC866 = 17,// Qt supported
        EncodingPC852 = 18,
        EncodingISO8859_2 = 39,// Qt supported
        EncodingISO8859_15 = 40,// Qt supported
    };
    Q_ENUM(Encoding)
    explicit EscPosUSB(QObject *parent = nullptr);
    Q_INVOKABLE void printer();
    Q_INVOKABLE void setFontSize(Size=defaultSize);
    Q_INVOKABLE void text(QByteArray);
    Q_INVOKABLE void textList(QList<QString>, bool=false);
    Q_INVOKABLE void cut();
    Q_INVOKABLE void setStyles(Style=OFF);
    Q_INVOKABLE void setJustification(Align=LEFT);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void image(QByteArray img);
    Q_INVOKABLE void endline();
    Q_INVOKABLE void barCode(QByteArray, Position, FontFamily, BarCodeType, int, int);
    Q_INVOKABLE void qrCode(QByteArray, Model, ModelSize, ErrorCorrection);
    Q_INVOKABLE void colsText(QList<int> cols, QStringList list, int=64);
    Q_INVOKABLE void column(int, int, int, int);
    Q_INVOKABLE bool connectPrinter(int, int);

private:
    QByteArray _buffer;
    int m_timeoutRead = 10000;
    QString m_idPdv;
    QTextCodec *m_codec = nullptr;

    //More variables for libusb
    libusb_context *_context;
    libusb_device_handle *_printer;
};

#endif // ESCPOSUSB_H
