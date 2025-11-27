#ifndef UDPWATCHEREXECUTOR_H
#define UDPWATCHEREXECUTOR_H

#include <QObject>
#include <QMessageBox>
#include <GStreamerCppLibrary/GStreamer/Manager.hpp>
#include <GStreamerCppLibrary/GStreamer/PipelineBuilder.hpp>
#include "PlayerController.h"

namespace Ui { class MainWindow; }

class UdpWatcherExecutor : public QObject
{
    Q_OBJECT
public:
    explicit UdpWatcherExecutor(GstManager* manager,
                                PlayerController* controller,
                                Ui::MainWindow* ui,
                                QObject *parent = nullptr);

    // Sadece UDP İzleme fonksiyonu var
    void startReceiver();

private:
    GstManager* m_manager;
    PlayerController* m_controller;
    Ui::MainWindow* m_ui;
};

#endif // UDPWATCHEREXECUTOR_H
