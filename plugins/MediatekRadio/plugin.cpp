#include <QtQml>
#include <QtQml/QQmlContext>

#include "plugin.h"
#include "MediatekRadio.h"

void MediatekRadioPlugin::registerTypes(const char *uri) {
    //@uri Example
    qmlRegisterSingletonType<MediatekRadio>(uri, 1, 0, "MediatekRadio", [](QQmlEngine*, QJSEngine*) -> QObject* { return new MediatekRadio; });
}
