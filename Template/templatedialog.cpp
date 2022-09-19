#include "templatedialog.h"
#include "ui_templatedialog.h"

#include "qdebug.h"
#include "qfiledialog.h"
#include "qmessagebox.h"

#include "drawroiscene.h"
#include "roirectitem.h"
#include "templatedialogsetttingutils.h"


class TemplateDialogPrivate
{
public:

    DrawRoiScene* m_graphicsScene;

    QGraphicsPixmapItem* m_imageItem;

    QImage m_img;

    RoiRectItem *m_rectItem = nullptr;


};

////////////////////////////////////////////////////
TemplateDialog::TemplateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TemplateDialog),
    d_ptr(new TemplateDialogPrivate)
{
    ui->setupUi(this);

    init();
}

TemplateDialog::~TemplateDialog()
{
    delete ui;
}

void TemplateDialog::init()
{
    setWindowFlags(Qt::Dialog |
                   Qt::WindowMinMaxButtonsHint |
                   Qt::WindowCloseButtonHint);

    d_ptr->m_graphicsScene = new DrawRoiScene();
    d_ptr->m_imageItem = new QGraphicsPixmapItem();

    ui->graphicsView->setScene(d_ptr->m_graphicsScene);

    connect(ui->Tb_Load, &QToolButton::clicked, this, &TemplateDialog::onLoadCloudDataClicked);
    connect(ui->Pb_Convert, &QPushButton::clicked, this, &TemplateDialog::onConvertClicked);

    connect(ui->Pb_DrawROI, &QPushButton::clicked, this, &TemplateDialog::onDrawROIClicked);
    connect(ui->Pb_Move, &QPushButton::clicked, this, &TemplateDialog::onMoveClicked);
    connect(ui->Pb_Choose, &QPushButton::clicked, this, &TemplateDialog::onChooseClicked);

    connect(ui->Pb_GetROIImage, &QPushButton::clicked, this, &TemplateDialog::onGetROIImageClicked);

    connect(ui->Pb_OK, &QPushButton::clicked, this, &TemplateDialog::onOKClicked);

    read();
}

void TemplateDialog::read()
{
    GLOBAL_TEMPLATE_SETTING;
    ui->Le_FilePath->setText(templateSetting->getTemplateFilePath());
}

void TemplateDialog::onLoadCloudDataClicked()
{
    QString strFileName = ui->Le_FilePath->text();

    QFile file(strFileName);
    if(!file.exists())
    {
        qDebug() << "dir path";
        strFileName = QApplication::applicationDirPath();
    }

    QStringList filters;
    filters << QString("%0(*.%0);;%1(*.%1)").arg("jpg").arg("png");

    strFileName = QFileDialog::getOpenFileName(this, "open file dialog",
                                               strFileName, filters.join(";;"));
    if(strFileName.isEmpty())
    {
        return;
    }

    ui->Le_FilePath->setText(strFileName);
}

void TemplateDialog::onConvertClicked()
{
    d_ptr->m_img = QImage(ui->Le_FilePath->text());
    d_ptr->m_imageItem->setPixmap(QPixmap::fromImage(d_ptr->m_img));
    d_ptr->m_graphicsScene->addItem(d_ptr->m_imageItem);

    ui->graphicsView->shrinkToViewPort();
}

void TemplateDialog::onDrawROIClicked()
{
    if(d_ptr->m_rectItem)
    {
        d_ptr->m_graphicsScene->removeItem(d_ptr->m_rectItem);
        delete d_ptr->m_rectItem;
    }

    d_ptr->m_rectItem = new RoiRectItem();
    d_ptr->m_graphicsScene->addItem(d_ptr->m_rectItem);

    d_ptr->m_graphicsScene->setCurrentItem(d_ptr->m_rectItem);
    d_ptr->m_graphicsScene->setMode(DrawRoiScene::DrawMode);

    QPen* rectPen = new QPen(QColor(Qt::green));
    rectPen->setWidth(0);
    d_ptr->m_rectItem->setPen(*rectPen);
}

void TemplateDialog::onMoveClicked()
{
    d_ptr->m_graphicsScene->setMode(DrawRoiScene::MoveMode);
}

void TemplateDialog::onChooseClicked()
{
    d_ptr->m_graphicsScene->setMode(DrawRoiScene::SelectMode);
}

void TemplateDialog::onGetROIImageClicked()
{
    QPainterPath painterPath = d_ptr->m_rectItem->mapToScene(d_ptr->m_rectItem->clipPath());

    QImage fixedImage(d_ptr->m_img.size(), QImage::Format_ARGB32_Premultiplied);
    fixedImage.fill(Qt::transparent);

    QPainter imgPainter(&fixedImage);
    imgPainter.setClipPath(painterPath);
    imgPainter.drawImage(QPoint(), d_ptr->m_img);
    imgPainter.end();

    fixedImage = fixedImage.copy(painterPath.boundingRect().toRect());
    fixedImage.save("E:Template\\Test.png");
}

void TemplateDialog::onOKClicked()
{
    GLOBAL_TEMPLATE_SETTING;
    templateSetting->setTemplateFilePath(ui->Le_FilePath->text());
    templateSetting->write();
}

