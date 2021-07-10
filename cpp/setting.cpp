#include "setting.h"
#include "controller.h"

#include <QVariant>
#include <QString>
#include <QJsonValue>


Setting::Setting(QObject* parent) : QObject(parent)
{
  if(parent) {
    Controller* controller = parent->property("controller").value<Controller*>();
    if(controller) {
      connect(this, &Setting::valueChanged, controller, &Controller::generate);
      connect(this, &Setting::valueChanged, controller, &Controller::modelChanged);
    }
  }
}

Setting* Setting::fromJson(const QJsonObject& object, QObject* parent)
{
  Setting* setting = new Setting(parent);
  for(QJsonObject::const_iterator pair = object.begin(); pair != object.end(); pair++) {
    setting->setProperty(pair.key().toLocal8Bit(),
                         QVariant::fromValue(pair.value().toString()));
  }
  return setting;
}

QString Setting::getValue() const
{
  if(isDefined()) {
    return m_value;
  } else {
    return m_defaultValue;
  }
}

QString Setting::toString() const
{
  if(isDefined()) {
    return QString("%1=%2\n").arg(m_key, m_value);
  } else {
    return "";
  }
}

QDebug operator<<(QDebug debug, const Setting &setting)
{
  debug.nospace().noquote()
    << setting.m_key << "=" << setting.m_value
    << "(" << setting.m_defaultValue.toLocal8Bit().constData() << ")";
  return debug.space();
}

bool Setting::isDefined() const
{
  return !m_value.isEmpty();
}

