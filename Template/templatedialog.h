#ifndef TEMPLATEDIALOG_H
#define TEMPLATEDIALOG_H

#include <QDialog>

namespace Ui
{
class TemplateDialog;
}

class TemplateDialogPrivate;
class TemplateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TemplateDialog(QWidget *parent = nullptr);
    ~TemplateDialog();

public:
    void init();

private:
    void read();

private:
    void onLoadCloudDataClicked();
    void onConvertClicked();

    void onDrawROIClicked();
    void onMoveClicked();
    void onChooseClicked();

    void onOKClicked();

signals:
    void refreshWindow();

private:
    Ui::TemplateDialog *ui;

    QScopedPointer<TemplateDialogPrivate> d_ptr;
};

#endif // TEMPLATEDIALOG_H
