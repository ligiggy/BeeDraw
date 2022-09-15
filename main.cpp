#include "templatedialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TemplateDialog templateDialog;
    templateDialog.show();

    return a.exec();
}
