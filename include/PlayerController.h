#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <QObject>
#include <QPushButton>
#include <GStreamer/Manager.hpp>

class PlayerController : public QObject
{
    Q_OBJECT
public:
    // Constructor: Manager ve Butonları alır
    explicit PlayerController(GstManager* manager,
                              QPushButton* btnPlay,
                              QPushButton* btnPause,
                              QPushButton* btnResume,
                              QPushButton* btnStop,
                              QObject *parent = nullptr);

public slots:
    void onPlay();
    void onPause();
    void onResume();
    void onStop();
    void forceStop();

    // Dışarıdan (örneğin ManuelExecutor'dan) play sinyali tetiklemek için
    void forcePlay();

private:
    GstManager* m_manager;
    QPushButton* m_btnPlay;
    QPushButton* m_btnPause;
    QPushButton* m_btnResume;
    QPushButton* m_btnStop;

    void updateButtonStates(bool isPlaying, bool isPaused, bool isStopped);
};

#endif // PLAYERCONTROLLER_H

