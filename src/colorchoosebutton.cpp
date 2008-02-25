#include "colorchoosebutton.h"

ColorChooseButton::ColorChooseButton(QWidget *parent) : QPushButton(parent) {
  color = Qt::black;
  QObject::connect(this, SIGNAL(clicked()), this, SLOT(dummy()));
  
  updateColor();
}

void ColorChooseButton::dummy() {
  QColor newColor = QColorDialog::getColor(color, this->parentWidget());
  if (!newColor.isValid()) {
    return;
  }
  
  setColor(newColor);
  updateColor();
}

QColor ColorChooseButton::getColor() {
  return color;
}

void ColorChooseButton::setColor(QColor newColor) {
  if (!newColor.isValid()) {
    return;
  }
  
  color = newColor;
  updateColor();
}

void ColorChooseButton::updateColor() {
  QPixmap p(QSize(128, 128));
  p.fill(color);
  this->setIcon(QIcon(p));
}
