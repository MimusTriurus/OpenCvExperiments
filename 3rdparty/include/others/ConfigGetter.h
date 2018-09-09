#ifndef CONFIGGETTER_H
#define CONFIGGETTER_H

#include <QObject>
#include <QMap>

class ConfigGetter : public QObject {
    Q_OBJECT
public:
    explicit ConfigGetter( QObject *parent = nullptr ) = delete;

    template<class T>
    static T getParam( const QString &key, const T &defaultValue = T( ) ) {
        if ( !_init )
            getSettings( );
        if ( _config.contains( key ) ) {
            QVariant var{ _config[ key ] };
            if ( var.isValid( ) && var.canConvert< T >( ) )
                return var.value< T >( );
        }

        return defaultValue;
    }
private:
    static QMap<QString, QVariant> _config;
    static bool _init;
    static void getSettings( );
};

#endif // CONFIGGETTER_H
