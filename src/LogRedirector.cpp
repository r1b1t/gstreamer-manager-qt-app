#include "LogRedirector.h"
#include <QString>

LogRedirector::LogRedirector(std::ostream& stream, int type, QObject* parent)
    : QObject(parent), m_stream(stream), m_type(type)
{
    // 1. Orijinal terminal bufferını sakla
    m_oldBuffer = m_stream.rdbuf();
    // 2. Akışı kendine yönlendir
    m_stream.rdbuf(this);
}

LogRedirector::~LogRedirector()
{
    // Yıkılırken her şeyi eski haline getir (Yoksa crash olur)
    if (m_stream.rdbuf() == this) {
        m_stream.rdbuf(m_oldBuffer);
    }
}

std::streambuf::int_type LogRedirector::overflow(std::streambuf::int_type v)
{
    if (v == '\n') {
        processBuffer();
    } else {
        m_buffer += (char)v;
    }
    return v;
}

std::streamsize LogRedirector::xsputn(const char* p, std::streamsize n)
{
    m_buffer.append(p, p + n);

    // Satır sonu var mı diye bak
    size_t pos = 0;
    while ((pos = m_buffer.find('\n')) != std::string::npos) {
        std::string line = m_buffer.substr(0, pos);
        // Sinyal gönder (Burada direkt emit ediyoruz)
        emit newLog(QString::fromStdString(line), m_type);

        // Orijinal terminale de yazsın ki IDE'den de gör (Opsiyonel)
        // m_oldBuffer->sputn(line.c_str(), line.size());
        // m_oldBuffer->sputc('\n');

        m_buffer.erase(0, pos + 1);
    }
    return n;
}

void LogRedirector::processBuffer()
{
    if (!m_buffer.empty()) {
        emit newLog(QString::fromStdString(m_buffer), m_type);
        m_buffer.clear();
    }
}
