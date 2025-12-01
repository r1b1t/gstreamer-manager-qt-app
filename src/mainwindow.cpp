#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // --- YENİ: TERMİNALİ ELE GEÇİR ---
    // cout (std::cout) yakalayıcı
    m_coutRedirector = new LogRedirector(std::cout, 0, this);
    connect(m_coutRedirector, &LogRedirector::newLog, this, &MainWindow::onLogCaptured);

    // cerr (std::cerr) yakalayıcı - Kütüphanen hataları buraya yazıyor
    m_cerrRedirector = new LogRedirector(std::cerr, 1, this);
    connect(m_cerrRedirector, &LogRedirector::newLog, this, &MainWindow::onLogCaptured);
    // --------------------------------

    // Pencere boyutu ve yerleşimi için görsel ayarlar
    this->resize(950, 600);
    if (ui->centralwidget->layout()) {
        ui->centralwidget->layout()->setAlignment(Qt::AlignTop);
    }
    ui->consoleNormal->setMaximumHeight(100);
    ui->consoleNormal->setMinimumHeight(80);


    // 1. GstManager'ı oluştur (Backend)
    m_gstManager = new GstManager();

    // 2. PlayerController'ı oluştur (Butonların yönetimini teslim et)
    m_playerController = new PlayerController(m_gstManager,
                                              ui->btnPlayNormal,
                                              ui->btnPauseNormal,
                                              ui->btnResumeNormal,
                                              ui->btnStopNormal,
                                              this);

    // 3. ManualPipelineExecutor'ı oluştur (Manuel metin kutusu yönetimi)
    m_manualExecutor = new ManualPipelineExecutor(m_gstManager,
                                                  m_playerController,
                                                  ui->txtManualPipeline,
                                                  this);

    // 4. PresetPipelineExecutor'ı oluştur (Hazır ayarların yönetimi)
    // UI dosyasındaki (XML) ilgili input alanlarını buraya bağlıyoruz.
    // Bu isimler senin paylaştığın mainwindow.ui yapısına göredir.
    // 4. PresetPipelineExecutor'ı oluştur
    m_presetExecutor = new PresetPipelineExecutor(m_gstManager,
                                                  m_playerController,
                                                  ui, // <--- Sadece 'ui' veriyoruz
                                                  this);


    // --- Dosya Seç Butonu Bağlantısı ---
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSelectFileClickedMain);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onSelectFileClickedUDP);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::onSelectFileClickedMulticast);


    // 5. BUTON BAĞLANTISI MANTIĞI:
    // PlayerController "Play" butonunu zaten kendine bağlamıştı.
    // Ancak biz "Play" tuşuna basılınca önce pipeline'ın kurulmasını, sonra oynamasını istiyoruz.
    // Bu yüzden controller'ın bağını kesip, kendi fonksiyonumuza (onPlayClicked) bağlıyoruz.

    // PlayerController'ın otomatik başlatmasını iptal et, bizim "Karar Verici" fonksiyona bağla
    disconnect(ui->btnPlayNormal, &QPushButton::clicked, m_playerController, &PlayerController::onPlay);
    connect(ui->btnPlayNormal, &QPushButton::clicked, this, &MainWindow::onPlayClicked);
}

// --- YENİ FONKSİYON: Logları İşle ve Karar Ver ---
void MainWindow::onLogCaptured(QString message, int type)
{
    // 1. Konsola Yazdır (Renkli)
    QString timeStamp = QDateTime::currentDateTime().toString("HH:mm:ss");
    QString coloredMsg;

    // Type 1 = Cerr (Hata), Type 0 = Cout (Bilgi)
    // Ancak senin kütüphanen bazen hatayı cout'a da yazıyor olabilir, metni kontrol edeceğiz.

    bool isCritical = false;

    // Kütüphaneden gelen mesajları analiz et (Senin log çıktılarına göre)
    if (message.contains("RUNTIME ERROR") ||
        message.contains("not found") ||
        message.contains("EOS"))
    {
        isCritical = true;
        type = 1; // Kırmızı yap
    }

    if (type == 1) {
        coloredMsg = QString("<span style='color: red;'>[%1] %2</span>").arg(timeStamp, message);
    } else {
        coloredMsg = QString("<span style='color: blue;'>[%1]</span> %2").arg(timeStamp, message);
    }

    ui->consoleNormal->append(coloredMsg);

    // 2. Eğer KRİTİK bir durumsa (Pencere kapandı, kamera yok vs.) Durdur!
    if (isCritical) {
        // Controller'ı durmaya zorla. Bu sayede butonlar "Başlat" haline döner.
        m_playerController->forceStop();
    }
}

MainWindow::~MainWindow()
{
    // Bellek temizliği (Oluşturma sırasının tersine silmek iyi bir pratiktir)
    delete m_presetExecutor;
    delete m_manualExecutor;
    delete m_playerController;
    delete m_gstManager; // Pipeline'ı da kapatır
    delete ui;
}

// --- Yeni Slot: Dosya Seç ---
void MainWindow::onSelectFileClickedMain()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Video Dosyası Seç",
                                                    "",
                                                    "Video Dosyaları (*.mp4 *.mkv *.avi *.mov *.webp);;Tüm Dosyalar (*.*)");

    if (!fileName.isEmpty()) {
        ui->lineEdit_4->setText(fileName);
    }
}

// --- Yeni Slot: Dosya Seç ---
void MainWindow::onSelectFileClickedUDP()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Video Dosyası Seç",
                                                    "",
                                                    "Video Dosyaları (*.mp4 *.mkv *.avi *.mov *.webp);;Tüm Dosyalar (*.*)");

    if (!fileName.isEmpty()) {
        ui->lineEdit_23->setText(fileName);
    }
}

// --- Yeni Slot: Dosya Seç ---
void MainWindow::onSelectFileClickedMulticast()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Video Dosyası Seç",
                                                    "",
                                                    "Video Dosyaları (*.mp4 *.mkv *.avi *.mov *.webp);;Tüm Dosyalar (*.*)");

    if (!fileName.isEmpty()) {
        ui->lineEdit_38->setText(fileName);
    }
}

// --- onPlayClicked Güncellemesi ---
void MainWindow::onPlayClicked()
{
    // 1. Hangi sekme açık?
    int currentTabInfo = ui->tabWidget->currentIndex();
    // XML'e göre: 0 = GStreamer (Yayıncı), 1 = UDP İzle (Alıcı)

    // 2. Motor dolu mu?
    if (m_gstManager->getPipeline() != nullptr) {
        // Zaten bir şey çalışıyor (ister yayın ister izleme)
        // PlayerController zaten butonu pasif yapmış olmalı ama güvenlik için:
        return;
    }

    if (currentTabInfo == 0)
    {
        // --- TAB 1: YAYINCI MODU ---
        QString manualText = ui->txtManualPipeline->toPlainText().trimmed();

        if (!manualText.isEmpty()) {
            m_manualExecutor->setupAndStart();
        }
        else {
            int presetIndex = ui->comboPresets->currentIndex();
            switch (presetIndex) {
            case 0: m_presetExecutor->startWindowsCam(); break;
            case 1: m_presetExecutor->startVideoTestSrc(); break;
            case 2: m_presetExecutor->startVideoPlayer(); break;
            case 3: m_presetExecutor->startWindowsScreen(); break;
            case 4: m_presetExecutor->startUdpUnicast(); break;
            case 5: m_presetExecutor->startUdpMulticast(); break;
            default: break;
            }
        }
    }
    else if (currentTabInfo == 1)
    {
        // --- TAB 2: İZLEYİCİ MODU ---
        // PresetPipelineExecutor içindeki startUdpReceiver fonksiyonu
        // Tab 2'deki lineEdit_45, 46, 47'yi okuyacak şekilde ayarlanmıştı.
        m_presetExecutor->startUdpReceiver();
    }
}
