#include "UdpWatcherExecutor.h"
#include "ui_mainwindow.h"

UdpWatcherExecutor::UdpWatcherExecutor(GstManager *manager, PlayerController *controller, Ui::MainWindow *ui, QObject *parent)
    : QObject(parent), m_manager(manager), m_controller(controller), m_ui(ui)
{
}

void UdpWatcherExecutor::startReceiver()
{
    // Tab 2'deki (Rtsp sekmesi) UI elemanlarını okuyoruz

    // IP: lineEdit_45
    std::string ip = m_ui->lineEdit_45->text().toStdString();
    // Eğer IP boşsa Unicast dinler, doluysa Multicast grubuna katılır.

    // Port: lineEdit_46
    int port = m_ui->lineEdit_46->text().toInt();
    if (port == 0) port = 5000;

    // Payload: lineEdit_47
    int payload = m_ui->lineEdit_47->text().toInt();
    if (payload == 0) payload = 96;

    // Eski pipeline'ı temizle (Sadece bu manager'a ait olanı)
    m_manager->close();

    // Pipeline oluştur
    std::string pipelineDesc = GstPipelineBuilder::udpReceiver(port, ip, payload);

    // Başlat
    if (m_manager->addPipeline(pipelineDesc)) {
        m_controller->forcePlay();
    } else {
        QMessageBox::critical(nullptr, "Hata", "İzleme başlatılamadı (Tab 2)!");
    }
}
