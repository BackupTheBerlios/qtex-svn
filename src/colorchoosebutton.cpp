#include "colorchoosebutton.h"

ColorChooseButton::ColorChooseButton(QWidget *parent) : QPushButton(parent) {
  m_color = Qt::black;
  connect(this, SIGNAL(clicked()), this, SLOT(slotOnClicked()));
  
  updateColor();
}

QColor ColorChooseButton::getColor() {
  return m_color;
}

void ColorChooseButton::setColor(QColor color) {
  if (!color.isValid()) {
    return;
  }
  
  m_color = color;
  updateColor();
}

void ColorChooseButton::slotOnClicked() {
  QColor color = QColorDialog::getColor(m_color, this->parentWidget());
  if (!color.isValid()) {
    return;
  }
  
  setColor(color);
  updateColor();
}

void ColorChooseButton::updateColor() {
  QPixmap p(QSize(128, 128));
  p.fill(m_color);
  this->setIcon(QIcon(p));
}
