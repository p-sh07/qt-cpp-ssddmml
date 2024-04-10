#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SDDMML.h"

class SDDMML : public QMainWindow
{
    Q_OBJECT

public:
    SDDMML(QWidget *parent = nullptr);
    ~SDDMML();

private:
    Ui::SDDMMLClass ui;
};
