#include "iniconfig.h"
#include <QSettings>

#include "iniconfig_p.h"




IniConfig::IniConfig()
    : d_ptr (new IniConfigPrivate)
{

}


IniConfig::IniConfig(IniConfigPrivate& d)
{
    d_ptr = &d;
}


IniConfig::~IniConfig()
{
    if(!d_ptr)
    {
        delete d_ptr;
    }
}
