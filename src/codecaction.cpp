#include "codecaction.h"

CodecAction::CodecAction(QWidget *parent, QString description, QString codec) : QAction(parent) {
  m_Codec = codec;
  m_Description = description;
  
  setText(m_Description + " (" + m_Codec + ")");
  
  connect(this, SIGNAL(triggered()), this, SLOT(slotWasTriggered()));
}

QStringList CodecAction::getAvailableCodecs() {
  QStringList codecs;
  codecs << QString("ISO 8859-1") << QString("UTF-8");
  
  return codecs;
}

QStringList CodecAction::getAvailableDescriptions() {
  QStringList descriptions;
  descriptions << tr("Western europe") << tr("Unicode");
  
  return descriptions;
}

QString CodecAction::getCodec() {
  return m_Codec;
}

QString CodecAction::getDescription() {
  return m_Description;
}

QString CodecAction::getDefaultCodec() {
  QSettings settings("QteX", "QteX");
  settings.beginGroup(QString("editor"));
  QString codec = settings.value(QString("encoding"), QString("ISO 8859-1")).toString();
  settings.endGroup();
  
  return codec;
}

void CodecAction::slotWasTriggered() {
  emit signalWasTriggered(this);
}
