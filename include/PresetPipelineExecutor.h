#ifndef PRESETPIPELINEEXECUTOR_H
#define PRESETPIPELINEEXECUTOR_H

#include <QObject>
#include <QMessageBox>
#include <GStreamer/Manager.hpp>
#include <GStreamer/PipelineBuilder.hpp>
#include "PlayerController.h"

namespace Ui {
class MainWindow;
}

class PresetPipelineExecutor : public QObject
{
    Q_OBJECT
public:
    explicit PresetPipelineExecutor(GstManager* manager,
                                    PlayerController* controller,
                                    Ui::MainWindow* ui,
                                    QObject *parent = nullptr);

    void startWindowsCam();
    void startVideoTestSrc();
    void startVideoPlayer();
    void startWindowsScreen();

    // --- YENİ UDP FONKSİYONLARI ---
    void startUdpUnicast();
    void startUdpMulticast();
    void startUdpReceiver();

private:
    GstManager* m_manager;
    PlayerController* m_controller;
    Ui::MainWindow* m_ui;

    // Kaynak oluşturucu yardımcı fonksiyonlar
    // Unicast sekmesindeki UI elemanlarından kaynak stringi üretir
    std::string getUnicastSource();

    // Multicast sekmesindeki UI elemanlarından kaynak stringi üretir
    std::string getMulticastSource();
};

#endif // PRESETPIPELINEEXECUTOR_H
