#ifndef NUEVO_MODELO_DIALOG_H
#define NUEVO_MODELO_DIALOG_H

#include <QDialog>
#include <iostream>
using namespace std;
namespace Ui {
class Nuevo_Modelo_Dialog;
}

class Nuevo_Modelo_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Nuevo_Modelo_Dialog(QWidget *parent = nullptr);
    ~Nuevo_Modelo_Dialog();
signals:
    void info_nuevo_modelo(QString,QString);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Nuevo_Modelo_Dialog *ui;
};

#endif // NUEVO_MODELO_DIALOG_H
