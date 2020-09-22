#ifndef MEDIATEKRADIO_PLUGIN_H
#define MEDIATEKRADIO_PLUGIN_H

#include <QQmlExtensionPlugin>

class MediatekRadioPlugin : public QQmlExtensionPlugin {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif
