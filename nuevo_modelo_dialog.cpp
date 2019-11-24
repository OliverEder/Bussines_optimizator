#include "nuevo_modelo_dialog.h"
#include "ui_nuevo_modelo_dialog.h"
#include <QMessageBox>

Nuevo_Modelo_Dialog::Nuevo_Modelo_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nuevo_Modelo_Dialog)
{
    ui->setupUi(this);
}

Nuevo_Modelo_Dialog::~Nuevo_Modelo_Dialog()
{
    delete ui;
}

void Nuevo_Modelo_Dialog::on_buttonBox_accepted()
{
    if(ui->lineEdit_nombre->text()=="" || ui->plainTextEdit_descripcion->toPlainText().size()==0)
    {
        QMessageBox msgBox;
        msgBox.setText("Los campos no pueden ir vacios");
        msgBox.setIcon(QMessageBox::Information	);
        QDialog::show();
        msgBox.exec();

    }
    else {

       QString nombre_modelo;
       QString descripcion;

       nombre_modelo = ui->lineEdit_nombre->text();
       descripcion = ui->plainTextEdit_descripcion->toPlainText();
       emit(info_nuevo_modelo(nombre_modelo,descripcion));
    }


}
