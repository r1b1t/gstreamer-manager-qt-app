#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox> // Mesaj kutuları için gerekli
#include <QDateTime>
#include <QFileDialog>
#include <GStreamerCppLibrary/GStreamer/Manager.hpp>
#include "PlayerController.h"
#include "PresetPipelineExecutor.h"
#include "ManualPipelineExecutor.h"
#include "LogRedirector.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // "Başlat" butonuna basılınca yapılacak özel işlem
    void onPlayClicked();
    void onLogCaptured(QString message, int type); //
    void onSelectFileClicked();

private:
    Ui::MainWindow *ui;

    // Pointer olarak tutuyoruz
    GstManager* m_gstManager;
    PlayerController* m_playerController;
    PresetPipelineExecutor* m_presetExecutor;
    ManualPipelineExecutor* m_manualExecutor;
    LogRedirector* m_coutRedirector; // Normal loglar için
    LogRedirector* m_cerrRedirector;
};

#endif // MAINWINDOW_H
