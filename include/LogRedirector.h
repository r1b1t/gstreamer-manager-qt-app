#ifndef LOGREDIRECTOR_H
#define LOGREDIRECTOR_H

#include <QObject>
#include <iostream>
#include <streambuf>
#include <string>

// Standart çıktıları (cout/cerr) yakalayıp Qt Sinyaline dönüştüren sınıf
class LogRedirector : public QObject, public std::streambuf
{
    Q_OBJECT
public:
    // type: 0 = cout (bilgi), 1 = cerr (hata)
    LogRedirector(std::ostream& stream, int type, QObject* parent = nullptr);
    ~LogRedirector();

signals:
    void newLog(QString message, int type);

protected:
    // Karakter geldiğinde çalışan fonksiyon (std::streambuf override)
    virtual int_type overflow(int_type v) override;
    virtual std::streamsize xsputn(const char* p, std::streamsize n) override;

private:
    std::ostream& m_stream;
    std::streambuf* m_oldBuffer; // Orijinal terminal buffer'ı
    std::string m_buffer;        // Satır biriktirmek için
    int m_type;

    void processBuffer();
};

#endif // LOGREDIRECTOR_H
