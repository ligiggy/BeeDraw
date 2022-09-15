#ifndef TEMPLATEDIALOGSETTTINGUTILS_H
#define TEMPLATEDIALOGSETTTINGUTILS_H


#include "templatedialogsetting.h"


#define GLOBAL_TEMPLATE_SETTING \
    auto templateSetting = TemplateDialogSetttingUtils::getInstance();

class TemplateDialogSetttingUtils
{
protected:
    TemplateDialogSetttingUtils();

public:
    static TemplateDialogSetting* getInstance();

};

#endif // TEMPLATEDIALOGSETTTINGUTILS_H
