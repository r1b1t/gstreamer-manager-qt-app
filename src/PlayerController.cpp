#include "PlayerController.h"

PlayerController::PlayerController(GstManager *manager, QPushButton *btnPlay, QPushButton *btnPause, QPushButton *btnResume, QPushButton *btnStop, QObject *parent)
    : QObject(parent), m_manager(manager), m_btnPlay(btnPlay), m_btnPause(btnPause), m_btnResume(btnResume), m_btnStop(btnStop)
{
    // Sinyal - Slot bağlantıları
    connect(m_btnPlay, &QPushButton::clicked, this, &PlayerController::onPlay);
    connect(m_btnPause, &QPushButton::clicked, this, &PlayerController::onPause);
    connect(m_btnResume, &QPushButton::clicked, this, &PlayerController::onResume);
    connect(m_btnStop, &QPushButton::clicked, this, &PlayerController::onStop);

    // Başlangıç durumu
    updateButtonStates(false, false, true);
}

void PlayerController::onPlay()
{
    // Play butonu aslında pipeline oluşturulmuşsa çalışır.
    // Ancak Manuel modda pipeline oluşturma işini 'ManualPipelineExecutor' yapacak.
    // Bu yüzden buradaki 'play' sadece var olanı oynatır.
    if(m_manager->getPipeline()) {
        m_manager->play();
        updateButtonStates(true, false, false);
    }
}

void PlayerController::onPause()
{
    m_manager->pause();
    updateButtonStates(false, true, false);
}

void PlayerController::onResume()
{
    m_manager->resume();
    updateButtonStates(true, false, false);
}

void PlayerController::onStop()
{
    m_manager->close(); // Kaynakları tamamen temizle
    updateButtonStates(false, false, true);
}

void PlayerController::forcePlay()
{
    // Dışarıdan tetikleme
    onPlay();
}

void PlayerController::forceStop()
{
    // Dışarıdan (örneğin Log hatasından) durdurma emri gelirse
    // normal stop fonksiyonunu çağırıyoruz.
    onStop();
}

void PlayerController::updateButtonStates(bool isPlaying, bool isPaused, bool isStopped)
{
    m_btnPlay->setEnabled(isStopped);   // Sadece durmuşsa play aktif
    m_btnPause->setEnabled(isPlaying);  // Oynuyorsa pause aktif
    m_btnResume->setEnabled(isPaused);  // Duraklatılmışsa resume aktif
    m_btnStop->setEnabled(!isStopped);  // Durmamışsa stop aktif
}
