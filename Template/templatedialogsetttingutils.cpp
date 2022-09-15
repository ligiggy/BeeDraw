#include "templatedialogsetttingutils.h"
#include "qapplication.h"
#include "qdebug.h"


TemplateDialogSetttingUtils::TemplateDialogSetttingUtils()
{

}

TemplateDialogSetting *TemplateDialogSetttingUtils::getInstance()
{
    static TemplateDialogSetting* self = nullptr;

    if(self == nullptr)
    {
        self = new TemplateDialogSetting();

        self->setFilePath(QApplication::applicationDirPath() + "/Param/TemplateDialog.cfg");
    }

    return self;
}
