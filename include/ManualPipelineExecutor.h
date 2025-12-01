#ifndef MANUALPIPELINEEXECUTOR_H
#define MANUALPIPELINEEXECUTOR_H

#include <QObject>
#include <QTextEdit>
#include <GStreamer/Manager.hpp>
#include "PlayerController.h"

class ManualPipelineExecutor : public QObject
{
    Q_OBJECT
public:
    explicit ManualPipelineExecutor(GstManager* manager,
                                    PlayerController* controller,
                                    QTextEdit* inputField,
                                    QObject *parent = nullptr);

    // Pipeline'ı kurar ve başlatır
    void setupAndStart();

private:
    GstManager* m_manager;
    PlayerController* m_controller;
    QTextEdit* m_inputField;
};

#endif // MANUALPIPELINEEXECUTOR_H
