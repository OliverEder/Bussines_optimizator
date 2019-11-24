#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "agregar_elemento.h"
#include "nuevo_modelo_dialog.h"
#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qjsonvalue.h>
using namespace std;
namespace Ui {
class MainWindow;
}
class Agregar_Elemento;
class Nuevo_Modelo_Dialog;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void json_agregarElemento(QJsonObject obj);

private slots:
    void on_actionNuevo_triggered();

    void on_actionAgregar_elemento_triggered();

    void on_actionCerrar_triggered();

    void recibir_nuevo_modelo(QString, QString);

    void recibir_nuevoElemento(QString , QString , QString , int ,bool , double , double , double );

    void on_treeWidget_itemSelectionChanged();

    void update_tree();

    void on_actionGuardar_triggered();

    void on_actionAbrir_triggered();

    void on_actionOptimizar_triggered();

    void generar_tensor(QJsonObject);

    void generar_raiz(QList<int> *nodos_vivos);

private:
    Ui::MainWindow *ui;
    Agregar_Elemento *ventana_Agregar_Elemento;
    Nuevo_Modelo_Dialog *ventana_nuevo_modelo;
    QJsonDocument doc;
    QJsonObject obj;
    QJsonArray bloc_aray;
};

#endif // MAINWINDOW_H
