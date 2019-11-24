#ifndef AGREGAR_ELEMENTO_H
#define AGREGAR_ELEMENTO_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qjsonvalue.h>
#include <iostream>

using namespace std;
namespace Ui {
class Agregar_Elemento;
}

class Agregar_Elemento : public QDialog
{
    Q_OBJECT

public:
    explicit Agregar_Elemento(QWidget *parent = nullptr);
    ~Agregar_Elemento();

private slots:
    void on_pushButton_clicked();
    void recibir_json(QJsonObject obj);

    void on_Bloque_comboBox_currentIndexChanged(const QString &arg1);

    void on_Ingreso_doubleSpinBox_valueChanged(double arg1);

    void on_Egreso_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_2_clicked();
    void on_Elemento_comboBox_currentIndexChanged(const QString &arg1);

    void on_Elemento_comboBox_currentTextChanged(const QString &arg1);

signals:
    void enviar_nuevoElemento(QString bloque, QString elemento, QString variante, int idVariante, bool requerido, double ingreso, double egreso, double ganancia);

private:
    Ui::Agregar_Elemento *ui;
    QJsonDocument doc;
    QJsonObject obj_agregarElemento;
    QJsonArray bloc_aray;
};

#endif // AGREGAR_ELEMENTO_H
