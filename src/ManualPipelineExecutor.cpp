#include "ManualPipelineExecutor.h"
#include <QMessageBox>

ManualPipelineExecutor::ManualPipelineExecutor(GstManager *manager, PlayerController *controller, QTextEdit *inputField, QObject *parent)
    : QObject(parent), m_manager(manager), m_controller(controller), m_inputField(inputField)
{
    // Burada Play butonuna basıldığında, eğer pipeline yoksa bu sınıf devreye girmeli.
    // Bunu MainWindow üzerinden bağlayacağız.
}

void ManualPipelineExecutor::setupAndStart()
{
    QString pipelineText = m_inputField->toPlainText().trimmed();

    if (pipelineText.isEmpty()) {
        QMessageBox::warning(nullptr, "Hata", "Lütfen geçerli bir GStreamer pipeline girin.");
        return;
    }

    // Eğer önceki pipeline çalışıyorsa durdur
    m_manager->close();

    // Yeni pipeline ekle (std::string'e çevirerek)
    bool success = m_manager->addPipeline(pipelineText.toStdString());

    if (success) {
        // Başarılıysa Controller üzerinden oynatmayı başlat
        m_controller->forcePlay();
    } else {
        QMessageBox::critical(nullptr, "GStreamer Hatası", "Pipeline oluşturulamadı. Söz dizimini kontrol edin.");
    }
}
