#include "PresetPipelineExecutor.h"
#include "ui_mainwindow.h" // <--- BU ÇOK ÖNEMLİ, MUTLAKA EKLE

PresetPipelineExecutor::PresetPipelineExecutor(GstManager *manager,
                                               PlayerController *controller,
                                               Ui::MainWindow *ui,
                                               QObject *parent)
    : QObject(parent),
    m_manager(manager),
    m_controller(controller),
    m_ui(ui) // UI Pointer'ını sakla
{
}

void PresetPipelineExecutor::startWindowsCam()
{
    // UI Dosyandaki isimleri kullanıyoruz
    int width = m_ui->lineEdit->text().toInt(); if (width == 0) width = 640;
    int height = m_ui->lineEdit_2->text().toInt(); if (height == 0) height = 480;
    int fps = m_ui->lineEdit_3->text().toInt(); if (fps == 0) fps = 30;
    int deviceIndex = m_ui->lineEdit_11->text().toInt();
    bool mirror = (m_ui->comboBox_2->currentIndex() == 0);

    std::string pipelineDesc = GstPipelineBuilder::windowscam(width, height, fps, deviceIndex, mirror, true);

    if (m_manager->addPipeline(pipelineDesc)) m_controller->forcePlay();
    else QMessageBox::critical(nullptr, "Hata", "Kamera başlatılamadı!");
}

void PresetPipelineExecutor::startVideoTestSrc()
{
    int width = m_ui->lineEdit_5->text().toInt(); if (width == 0) width = 640;
    int height = m_ui->lineEdit_6->text().toInt(); if (height == 0) height = 480;
    int fps = m_ui->lineEdit_7->text().toInt(); if (fps == 0) fps = 30;
    std::string pattern = m_ui->comboBox->currentText().toStdString();

    std::string pipelineDesc = GstPipelineBuilder::videotestsrc(width, height, fps, pattern, true);

    if (m_manager->addPipeline(pipelineDesc)) m_controller->forcePlay();
    else QMessageBox::critical(nullptr, "Hata", "Test videosu başlatılamadı!");
}

void PresetPipelineExecutor::startVideoPlayer()
{
    QString path = m_ui->lineEdit_4->text().trimmed(); // Video Path
    if(path.isEmpty()) {
        QMessageBox::warning(nullptr, "Uyarı", "Lütfen dosya seçin.");
        return;
    }

    std::string pipelineDesc = GstPipelineBuilder::videoplayer(path.toStdString(), true);

    if (m_manager->addPipeline(pipelineDesc)) m_controller->forcePlay();
    else QMessageBox::critical(nullptr, "Hata", "Video oynatılamadı!");
}

void PresetPipelineExecutor::startWindowsScreen()
{
    int width = m_ui->lineEdit_8->text().toInt(); if (width == 0) width = 1920;
    int height = m_ui->lineEdit_9->text().toInt(); if (height == 0) height = 1080;
    int fps = m_ui->lineEdit_10->text().toInt(); if (fps == 0) fps = 30;

    std::string pipelineDesc = GstPipelineBuilder::windowsscreen(width, height, fps, true);

    if (m_manager->addPipeline(pipelineDesc)) m_controller->forcePlay();
    else QMessageBox::critical(nullptr, "Hata", "Ekran yakalama başlatılamadı!");
}

// ==================================================
//               YARDIMCI FONKSİYONLAR
// ==================================================

// Unicast sekmesindeki (Page 7) "Yayın Kaynağı" (Combobox_3) seçimine göre pipeline başını hazırlar.
std::string PresetPipelineExecutor::getUnicastSource()
{
    int sourceIndex = m_ui->comboBox_3->currentIndex();
    // 0: windowscam, 1: videotestsrc, 2: videoplayer, 3: windowsscreen

    // ÖNEMLİ: Tüm builder fonksiyonlarına 'addSink = false' gönderiyoruz.
    // Çünkü görüntüyü ekrana değil, UDP Encoder'a vereceğiz.

    if (sourceIndex == 0) { // Kamera
        int w = m_ui->lineEdit_16->text().toInt(); if(!w) w=640;
        int h = m_ui->lineEdit_17->text().toInt(); if(!h) h=480;
        int fps = m_ui->lineEdit_18->text().toInt(); if(!fps) fps=30;
        int dev = m_ui->lineEdit_19->text().toInt();
        bool mirror = (m_ui->comboBox_4->currentIndex() == 0);
        return GstPipelineBuilder::windowscam(w, h, fps, dev, mirror, false);
    }
    else if (sourceIndex == 1) { // Test
        int w = m_ui->lineEdit_20->text().toInt(); if(!w) w=640;
        int h = m_ui->lineEdit_21->text().toInt(); if(!h) h=480;
        int fps = m_ui->lineEdit_22->text().toInt(); if(!fps) fps=30;
        std::string pattern = m_ui->comboBox_5->currentText().toStdString();
        return GstPipelineBuilder::videotestsrc(w, h, fps, pattern, false);
    }
    else if (sourceIndex == 2) { // Dosya
        QString path = m_ui->lineEdit_23->text().trimmed();
        if(path.isEmpty()) return "";
        return GstPipelineBuilder::videoplayer(path.toStdString(), false);
    }
    else if (sourceIndex == 3) { // Ekran
        int w = m_ui->lineEdit_24->text().toInt(); if(!w) w=1920;
        int h = m_ui->lineEdit_25->text().toInt(); if(!h) h=1080;
        int fps = m_ui->lineEdit_26->text().toInt(); if(!fps) fps=30;
        return GstPipelineBuilder::windowsscreen(w, h, fps, false);
    }
    return "";
}

// Multicast sekmesindeki (Page 8) "Yayın Kaynağı" (Combobox_6) seçimine göre pipeline başını hazırlar.
std::string PresetPipelineExecutor::getMulticastSource()
{
    int sourceIndex = m_ui->comboBox_6->currentIndex();

    if (sourceIndex == 0) { // Kamera
        int w = m_ui->lineEdit_31->text().toInt(); if(!w) w=640;
        int h = m_ui->lineEdit_32->text().toInt(); if(!h) h=480;
        int fps = m_ui->lineEdit_33->text().toInt(); if(!fps) fps=30;
        int dev = m_ui->lineEdit_34->text().toInt();
        bool mirror = (m_ui->comboBox_7->currentIndex() == 0);
        return GstPipelineBuilder::windowscam(w, h, fps, dev, mirror, false);
    }
    else if (sourceIndex == 1) { // Test
        int w = m_ui->lineEdit_35->text().toInt(); if(!w) w=640;
        int h = m_ui->lineEdit_36->text().toInt(); if(!h) h=480;
        int fps = m_ui->lineEdit_37->text().toInt(); if(!fps) fps=30;
        std::string pattern = m_ui->comboBox_8->currentText().toStdString();
        return GstPipelineBuilder::videotestsrc(w, h, fps, pattern, false);
    }
    else if (sourceIndex == 2) { // Dosya
        QString path = m_ui->lineEdit_38->text().trimmed();
        if(path.isEmpty()) return "";
        return GstPipelineBuilder::videoplayer(path.toStdString(), false);
    }
    else if (sourceIndex == 3) { // Ekran
        int w = m_ui->lineEdit_39->text().toInt(); if(!w) w=1920;
        int h = m_ui->lineEdit_40->text().toInt(); if(!h) h=1080;
        int fps = m_ui->lineEdit_41->text().toInt(); if(!fps) fps=30;
        return GstPipelineBuilder::windowsscreen(w, h, fps, false);
    }
    return "";
}

// ==================================================
//               ANA FONKSİYONLAR
// ==================================================

void PresetPipelineExecutor::startUdpUnicast()
{
    // 1. Ağ Ayarlarını Al
    std::string ip = m_ui->lineEdit_12->text().toStdString();
    if(ip.empty()) ip = "127.0.0.1";

    int port = m_ui->lineEdit_13->text().toInt();
    if(port == 0) port = 5000;

    int bitrate = m_ui->lineEdit_14->text().toInt();
    if(bitrate == 0) bitrate = 2048;

    int payload = m_ui->lineEdit_15->text().toInt();
    if(payload == 0) payload = 96;

    // 2. Kaynağı Oluştur
    std::string sourcePipe = getUnicastSource();
    if (sourcePipe.empty()) {
        QMessageBox::warning(nullptr, "Hata", "Kaynak oluşturulamadı (Dosya yolu boş olabilir).");
        return;
    }

    // 3. UDP Sender Pipeline Oluştur
    std::string finalPipeline = GstPipelineBuilder::udpUnicastSender(sourcePipe, ip, port, bitrate, payload);

    if (m_manager->addPipeline(finalPipeline)) {
        m_controller->forcePlay();
    } else {
        QMessageBox::critical(nullptr, "Hata", "UDP Unicast başlatılamadı!");
    }
}

void PresetPipelineExecutor::startUdpMulticast()
{
    // 1. Ağ Ayarlarını Al
    std::string ip = m_ui->lineEdit_27->text().toStdString();
    if(ip.empty()) ip = "239.255.0.1"; // Multicast IP örneği

    int port = m_ui->lineEdit_28->text().toInt();
    if(port == 0) port = 5000;

    int bitrate = m_ui->lineEdit_29->text().toInt();
    if(bitrate == 0) bitrate = 4096;

    int payload = m_ui->lineEdit_30->text().toInt();
    if(payload == 0) payload = 96;

    // 2. Kaynağı Oluştur
    std::string sourcePipe = getMulticastSource();
    if (sourcePipe.empty()) {
        QMessageBox::warning(nullptr, "Hata", "Kaynak oluşturulamadı.");
        return;
    }


    // 3. UDP Sender Pipeline Oluştur
    std::string finalPipeline = GstPipelineBuilder::udpMulticastSender(sourcePipe, ip, port, bitrate, payload);

    if (m_manager->addPipeline(finalPipeline)) {
        m_controller->forcePlay();
    } else {
        QMessageBox::critical(nullptr, "Hata", "UDP Multicast başlatılamadı!");
    }
}

void PresetPipelineExecutor::startUdpReceiver()
{
    // --- DİKKAT: XML'e göre Tab 2 (GStreamer UDP Yayın İzle) elemanlarını kullanıyoruz ---
    // IP (Multicast): lineEdit_45
    std::string ip = m_ui->lineEdit_45->text().toStdString();

    // Port: lineEdit_46
    int port = m_ui->lineEdit_46->text().toInt();
    if(port == 0) port = 5000;

    // Payload: lineEdit_47
    int payload = m_ui->lineEdit_47->text().toInt();
    if(payload == 0) payload = 96;


    // Receiver Pipeline Oluştur
    std::string finalPipeline = GstPipelineBuilder::udpReceiver(port, ip, payload);

    if (m_manager->addPipeline(finalPipeline)) {
        m_controller->forcePlay();
    } else {
        QMessageBox::critical(nullptr, "Hata", "UDP Alıcı başlatılamadı!");
    }
}
