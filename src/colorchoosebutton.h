#ifndef __COLORCHOOSEBUTTON_H__
#define __COLORCHOOSEBUTTON_H__

#include <QColor>
#include <QColorDialog>
#include <QPushButton>

class ColorChooseButton : public QPushButton
{
  Q_OBJECT
  
  public:
    ColorChooseButton(QWidget *);
    QColor getColor();
    void setColor(QColor);
  
  
  private slots:
    void dummy();
    
  private:
    void updateColor();
    QColor color;
  
};

#endif
