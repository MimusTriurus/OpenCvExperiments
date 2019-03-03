#include "ConfigGetter.h"
#include <QDomDocument>
#include <QVariant>

//ConfigGetter::ConfigGetter( QObject *parent ) : QObject( parent ) {

//}

void ConfigGetter::getSettings( ) {
    _init = true;
    QVariant v( "111" );
    _config.insert( "1", v );
}

bool ConfigGetter::_init{ false };

QMap<QString, QVariant> ConfigGetter::_config;
