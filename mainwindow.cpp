/*
Clase de la ventana principal
Autor: Oliver Espinosa Meneses
Version:1.0
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"
#include <QFile>
#include <QFileDialog>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <qjsonvalue.h>
#include <QStringList>
#include <QListIterator>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QList>
#include <QVector>
#include <string>
int ***tensor_bloques_id;
int ***tensor_bloques_egreso;
int **matriz_bloques;
int *vector_bloques;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    ventana_Agregar_Elemento = new Agregar_Elemento();
    ventana_nuevo_modelo = new Nuevo_Modelo_Dialog;
    ui->statusBar->showMessage("Sin Modelo de negocio");
    ui->statusBar->setStatusTip("Sin Modelo de negocio");
    ui->treeWidget->hide();
    ui->actionAgregar_elemento->setDisabled(true);

    //Conexiones para enviar parametros entre clases
    QObject::connect(ventana_nuevo_modelo,SIGNAL(info_nuevo_modelo(QString,QString)),this, SLOT(recibir_nuevo_modelo(QString,QString)));
    QObject::connect(this,SIGNAL(json_agregarElemento(QJsonObject)),ventana_Agregar_Elemento,SLOT(recibir_json(QJsonObject)));
    QObject::connect(ventana_Agregar_Elemento,SIGNAL(enviar_nuevoElemento(QString , QString, QString , int ,bool, double, double, double)),this, SLOT(recibir_nuevoElemento(QString , QString , QString , int, bool , double , double , double )));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNuevo_triggered()
{

    //Si el treewidget esta oculto entonces mostrar la ventana_nuevo_modelo
    if(ui->treeWidget->isHidden()==true)
    {
        ventana_nuevo_modelo->show();

    }
}

void MainWindow::recibir_nuevo_modelo(QString nombre_modelo, QString descripcion)
{
    QJsonObject bloque;
    QJsonArray bloques;
    QJsonArray elementos;
    QJsonObject elemento;
    //Se agregan campos de nombre, descripcion al JsonObject obj
    obj["nombre"]=nombre_modelo;
    obj["descripcion"]=descripcion;
    obj["bloques"]="";

    //se agregan campos al objeto
    bloque.insert("Bloque","Propuesta de valor");
    bloque.insert("Elementos","");
    bloques.append(bloque);

    bloque["Bloque"]="Asociaciones clave";
    bloques.append(bloque);

    bloque["Bloque"]="Acciones clave";
    bloques.append(bloque);

    bloque["Bloque"]="Recursos clave";
    bloques.append(bloque);

    bloque["Bloque"]="Canales";
    bloques.append(bloque);

    bloque["Bloque"]="Relaciones con el cliente";
    bloques.append(bloque);

    bloque["Bloque"]="Segmentos de mercado";
    bloques.append(bloque);

    bloque["Bloque"]="Fuentes de ingreso";
    bloques.append(bloque);

    //bloque["Acciones clave"]=elemento;
    //se agrega el objeto al campo bloques
    obj["bloques"]=bloques;

    ui->actionAgregar_elemento->setDisabled(false);
    ui->treeWidget->show();
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("Categorias");
    ui->actionNuevo->setEnabled(false);
    QTreeWidgetItem *Modelo = new QTreeWidgetItem(ui->treeWidget);
    Modelo->setText(0, "negocio: " + nombre_modelo);
    QList<QTreeWidgetItem *> items;
    cout << "Numbero de bloques " << bloques.size() << endl;
    for(int i=0; i<bloques.size() ; i++)
    {
        bloque=bloques.at(i).toObject();
        items.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(bloque.value("Bloque").toString()))));
    }

    //items.append(bloque1);
    Modelo->addChildren(items);

    ui->statusBar->showMessage("Nuevo Modelo de negocio");
    ui->statusBar->setStatusTip("Nuevo Modelo de negocio");
}

void MainWindow::on_actionAgregar_elemento_triggered()
{
    if(ui->treeWidget->isHidden()==false)
    {
        ventana_Agregar_Elemento->show();
        emit(json_agregarElemento(obj));

    }
}

void MainWindow::recibir_nuevoElemento(QString bloque, QString elemento, QString variante, int idVariante, bool requerido, double ingreso, double egreso, double ganancia )
{
    QJsonArray arreglo_bloques;
    QJsonObject objeto_bloque;

    QJsonArray arreglo_elementos;
    QJsonObject objeto_elemento;

    QJsonArray arreglo_variantes;
    QJsonObject objeto_variante;

    QJsonArray arreglo_elementos_vacio;
    QJsonArray arreglo_variantes_vacio;

    bool elemento_encontrado=false;

    cout << "============================" << endl;
    cout << "datos recibidos" << endl;
    cout << "bloque:" << bloque.toStdString() << endl;
    cout << "elemento: " << elemento.toStdString() << endl;
    cout << "variante:" << variante.toStdString() << endl;
    cout << "ID Variante:" << idVariante << endl;
    cout << "requerido:" << requerido << endl;
    cout << "ingreso:" << ingreso << endl;
    cout << "egreso:" << egreso << endl;
    cout << "ganancia:" << ganancia << endl;
    cout << "============================" << endl;
    //se asigna a un arreglo el contenido de bloques
    arreglo_bloques=obj["bloques"].toArray();
    //se recorre el arreglo

    //se recorre el arreglo que objetos bloques

    for(int i=0 ; i < arreglo_bloques.size() ; i++)
    {
       //Se pasa el objeto en curso a objeto_bloque
       objeto_bloque = arreglo_bloques.at(i).toObject();
       //se busca el bloque
       if(objeto_bloque.value("Bloque").toString() == bloque)
       {
           cout << "1 Bloque encontrado " << i <<endl;
           if(objeto_bloque.value("Elementos").isArray())
           {
               cout << "2 Elemento es un arreglo" << endl;
               arreglo_elementos = objeto_bloque.value("Elementos").toArray();
               //se recorre el arreglo de los elementos
               for(int j=0 ; j < arreglo_elementos.size() ; j++)
               {

                    objeto_elemento=arreglo_elementos.at(j).toObject();
                    //si el elemento en curso es igual que elemento
                    if(objeto_elemento.value("Elemento").toString() == elemento)
                    {
                        cout << "3 Elemento encontrado" << endl;
                        if(objeto_elemento.value("Variantes").isArray())
                        {
                            cout << "4 Variantes es un arreglo" << endl;
                            arreglo_variantes=objeto_elemento.value("Variantes").toArray();
                            objeto_variante.insert("Variante",variante);
                            objeto_variante.insert("ID", idVariante);
                            objeto_variante.insert("Requerido", requerido);
                            objeto_variante.insert("Ingreso", ingreso);
                            objeto_variante.insert("Egreso", egreso);
                            objeto_variante.insert("Ganancia", ganancia);
                            arreglo_variantes.append(objeto_variante);
                            objeto_elemento["Variantes"]=arreglo_variantes;
                            arreglo_elementos.replace(j,objeto_elemento);
                            objeto_bloque["Elementos"]=arreglo_elementos;
                            arreglo_bloques.replace(i,objeto_bloque);
                            obj["bloques"]=arreglo_bloques;
                            cout << "1" << endl;
                            arreglo_variantes = arreglo_variantes_vacio;

                        }
                        else
                        {

                            arreglo_variantes = arreglo_variantes_vacio;
                            cout << "5 Variantes no es un arreglo" << endl;
                            objeto_variante.insert("Variante",variante);
                            objeto_variante.insert("ID", idVariante);
                            objeto_variante.insert("Requerido", requerido);
                            objeto_variante.insert("Ingreso", ingreso);
                            objeto_variante.insert("Egreso", egreso);
                            objeto_variante.insert("Ganancia", ganancia);
                            arreglo_variantes.append(objeto_variante);
                            objeto_elemento["Variantes"]=arreglo_variantes;
                            arreglo_elementos.replace(j,objeto_elemento);
                            objeto_bloque["Elementos"]=arreglo_elementos;
                            arreglo_bloques.replace(i,objeto_bloque);
                            obj["bloques"]=arreglo_bloques;
                            cout << "2" << endl;
                            arreglo_variantes = arreglo_variantes_vacio;
                        }
                        elemento_encontrado = true;
                        break;
                    }
                    else
                    {
                        cout << "6 Elemento no encontrado" << endl;
                        elemento_encontrado = false;

                    }
               }
               if(elemento_encontrado==false)
               {
                   cout << "6."<< endl;
                   cout << "se genera la estructura" << endl;
                   arreglo_variantes = arreglo_variantes_vacio;
                   cout << "tamaño del arreglo variantes:"<<arreglo_variantes.size() << endl;
                   objeto_variante.insert("Variante",variante);
                   objeto_variante.insert("ID", idVariante);
                   objeto_variante.insert("Requerido", requerido);
                   objeto_variante.insert("Ingreso", ingreso);
                   objeto_variante.insert("Egreso", egreso);
                   objeto_variante.insert("Ganancia", ganancia);
                   arreglo_variantes.append(objeto_variante);

                   objeto_elemento.insert("Elemento", elemento);
                   objeto_elemento.insert("Variantes", arreglo_variantes);
                   arreglo_elementos.append(objeto_elemento);
                   objeto_bloque["Elementos"]=arreglo_elementos;
                   arreglo_bloques.replace(i,objeto_bloque);
                   obj["bloques"]=arreglo_bloques;
               }

           }
           else
           {

               cout << "7 No es un arreglo" << endl;
               cout << "se genera la estructura" << endl;
               //se vacian los arreglos
               arreglo_elementos = arreglo_elementos_vacio;
               arreglo_variantes = arreglo_variantes_vacio;

               objeto_variante.insert("Variante",variante);
               objeto_variante.insert("ID", idVariante);
               objeto_variante.insert("Requerido", requerido);
               objeto_variante.insert("Ingreso", ingreso);
               objeto_variante.insert("Egreso", egreso);
               objeto_variante.insert("Ganancia", ganancia);
               arreglo_variantes.append(objeto_variante);

               objeto_elemento.insert("Elemento", elemento);
               objeto_elemento["Variantes"]=arreglo_variantes;
               arreglo_elementos.append(objeto_elemento);
               objeto_bloque["Elementos"]=arreglo_elementos;
               arreglo_bloques.replace(i,objeto_bloque);
               obj["bloques"]=arreglo_bloques;

               //se vacian los arreglos
               arreglo_elementos = arreglo_elementos_vacio;
               arreglo_variantes = arreglo_variantes_vacio;

           }
           break;
       }
    }
    update_tree();

}
/*
    funcion que actualiza el arbol.
*/
void MainWindow::update_tree()
{

    QJsonArray arreglo_bloques;
    QJsonObject objeto_bloque;

    QJsonArray arreglo_elementos;
    QJsonObject objeto_elemento;

    QJsonArray arreglo_variantes;
    QJsonObject objeto_variante;
    ui->treeWidget->clear();
    ui->treeWidget->show();
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setHeaderLabel("Categorias");
    ui->actionNuevo->setEnabled(false);
    ui->actionAgregar_elemento->setDisabled(false);
    QTreeWidgetItem *Modelo = new QTreeWidgetItem(ui->treeWidget);
    Modelo->setText(0, "negocio: " + obj["nombre"].toString());
    QList<QTreeWidgetItem *> items_bloques;
    QList<QTreeWidgetItem *> items_elementos;
    QList<QTreeWidgetItem *> items_variantes;
    arreglo_bloques=obj.value("bloques").toArray();
    for(int i=0; i< arreglo_bloques.size() ; i++)
    {
        objeto_bloque = arreglo_bloques.at(i).toObject();
        items_bloques.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(objeto_bloque.value("Bloque").toString()))));

        if(objeto_bloque.value("Elementos").isArray())
        {

            arreglo_elementos= objeto_bloque.value("Elementos").toArray();

            for(int j ; j<arreglo_elementos.size() ; j++)
            {
                objeto_elemento= arreglo_elementos.at(j).toObject();
                items_elementos.append(new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString(objeto_elemento.value("Elemento").toString()))));
            }


            items_bloques.at(i)->addChildren(items_elementos);
        }
    }
    Modelo->addChildren(items_bloques);
}

void MainWindow::on_actionCerrar_triggered()
{
    this->close();
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    //Accion cuando un elemento es eleccionado
}

void MainWindow::on_actionGuardar_triggered()
{
    //guardar archivo
    QString json_filter = "JSON (*.json)";
    QString nombre_archivo = QFileDialog::getSaveFileName(this, tr("Guardar archivo"), "/");
    if(nombre_archivo.isEmpty())
    {

    }
    else
    {
        QJsonDocument doc;
        doc.setObject(obj);
        QByteArray data_json = doc.toJson();
        QFile output(nombre_archivo);
        if(output.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            output.write(data_json);
            output.close();
            QMessageBox::information(this, tr("Mensaje"), tr("Documento guardado correctamente"));
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), output.errorString());
        }

    }
}
/*
    Abre un archivo json
*/
void MainWindow::on_actionAbrir_triggered()
{
    QString json_filter = "JSON (*.json)";
    QString nombre_archivo = QFileDialog::getOpenFileName(this, tr("Abrir archivo"), "/", json_filter, &json_filter, QFileDialog::DontUseNativeDialog);

    if(nombre_archivo.isEmpty())
    {

    }
    else
    {
        QJsonDocument doc;
        QByteArray data_json;
        QFile input(nombre_archivo);
        if(input.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            data_json = input.readAll();
            doc = doc.fromJson(data_json);
            obj= doc.object();
            QMessageBox::information(this, tr("Mensaje"), tr("Documento JSON leido correctamente"));
            update_tree();

        }
        else
        {
            QMessageBox::critical(this, tr("Error"), input.errorString());
        }
    }
}
/*
    Funcion Optimizar
    En esta funcion se generar el preprocesamiento de los elementos y las variantes
    ingresadas por el usuario.

    Se llena un tensor, donde cada matriz representa un bloque y cada fila del
    bloque representa un conjunto o elemento, y cada columna una variante de cada
    elemento.

    Se generan los nodos a partir de la combinacion de las variantes de distintos
    elementos. Los elementos de un bloque seran conjuntos cuyos elementos son las
    variantes. Los nodos son una combinacion de un elemento de cada conjunto de cada
    bloque. Una vez generados los nodos, se generara un grafo a partir de los nodos
    generados.

*/
void MainWindow::on_actionOptimizar_triggered()
{


    QList<nodo> nodos_raiz;
    QList<nodo> nodos_vivos;
    QList<nodo> nodos_utiles;
    QList<nodo> nodos_grafo;

    //Generar tensor con elementos y variantes de cada bloque.
    generar_tensor(obj);
    //Generar los nodos raiz de cada bloque
    generar_raiz(&nodos_raiz);
    //Generar todos los nodos
    generar_combinatoria(&nodos_raiz, &nodos_grafo);

}
/*
    Se generan dos tensores donde cada matriz contiene las variantes de cada elemento del
    bloque. Un tensor contiene los ID  de cada variante y otro tensor contiene los egresos
    de cada variante.
*/
void MainWindow::generar_tensor(QJsonObject obj)
{
    cout << "Generando tensores" << endl;
    QJsonObject objeto_bloque;
    QJsonArray arreglo_bloques;
    QJsonObject objeto_elemento;
    QJsonArray arreglo_elementos;
    QJsonObject objeto_variante;
    QJsonArray arreglo_variantes;


    arreglo_bloques=obj["bloques"].toArray();
    tensor_bloques_id = (int ***) malloc(sizeof(int**) * arreglo_bloques.size());
    tensor_bloques_egreso = (int ***) malloc(sizeof(int**) * arreglo_bloques.size());
    matriz_bloques = (int **) malloc(sizeof(int*) * arreglo_bloques.size());
    vector_bloques = (int *) malloc(sizeof(int) * arreglo_bloques.size());
    for(int i=0 ; i<arreglo_bloques.size() ; i++)
    {
        *(vector_bloques+i) = 0;
    }
    for(int i=0 ; i < arreglo_bloques.size() ; i++)
    {
       //Se pasa el objeto en curso a objeto_bloque
       objeto_bloque = arreglo_bloques.at(i).toObject();
       if(objeto_bloque.value("Elementos").isArray())
       {
            arreglo_elementos = objeto_bloque.value("Elementos").toArray();
            *(vector_bloques+i) = arreglo_elementos.size();
            matriz_bloques[i] = (int *) malloc(sizeof(int) * arreglo_elementos.size());
            tensor_bloques_id[i] = (int **) malloc(sizeof(int*) * arreglo_elementos.size());
            tensor_bloques_egreso[i] = (int **) malloc(sizeof(int*) * arreglo_elementos.size());
            for(int j=0 ; j < arreglo_elementos.size() ; j++)
            {
                objeto_elemento = arreglo_elementos.at(j).toObject();
                if(objeto_elemento.value("Variantes").isArray())
                {
                    arreglo_variantes = objeto_elemento.value("Variantes").toArray();
                    *(*(matriz_bloques+i)+j)= arreglo_variantes.size();
                    tensor_bloques_id[i][j] = (int *) malloc(sizeof(int) * arreglo_variantes.size());
                    tensor_bloques_egreso[i][j] = (int *) malloc(sizeof(int) * arreglo_variantes.size());
                    for(int k=0; k<arreglo_variantes.size() ; k++)
                    {
                        objeto_variante = arreglo_variantes.at(k).toObject();
                        *(*(*(tensor_bloques_id+i)+j)+k) = objeto_variante.value("ID").toInt();
                        *(*(*(tensor_bloques_egreso+i)+j)+k) = objeto_variante.value("Egreso").toInt();
                    }
                }
                else
                {
                    cout << "El elemento " << objeto_elemento.value("Elemento").toString().toStdString() << " no contiene variantes" <<endl;
                    cout << "No es posible generar la optimizacion" << endl;
                    break;
                }
            }
       }
       else
       {
           cout << "El bloque "<< objeto_bloque.value("Bloque").toString().toStdString() << " no contiene elementos"<<endl;
           cout << "No es posible generar la optimizacion" << endl;
           break;
       }
    }


}
/*
    Se generan los nodos raiz de cada bloque.
*/
void MainWindow::generar_raiz(QList<nodo> *nodos_raiz)
{
    nodo nodo_raiz;

    cout << "Generando nodos raiz" << endl;

    for (int i=0 ; i<8 ; i++)
    {
        for (int j=0 ; j<*(vector_bloques+i) ; j++)
        {
            nodo_raiz.append(*(*(*(tensor_bloques_id+i)+j)+0));
        }
        nodos_raiz->append(nodo_raiz);
        nodo_raiz.clear();
    }

}
/*
    Se genera la combinatoria de todos los nodos del arbol
    se recibe una lista con los nodos raiz de cada bloque

*/
void MainWindow::generar_combinatoria(QList<nodo> *nodos_raiz, QList<nodo> *nodos_grafo)
{
    nodo nodo_nuevo;
    QList<nodo> nodos_vivos;
    QList<nodo> nodos_utiles;
    cout << "Generando combinatoria" << endl;
    for (int b=0 ; b<nodos_raiz->size() ; b++)
    {
        nodos_vivos.append(nodos_raiz->at(b));

        do{
            nodo_nuevo = nodos_vivos.at(0);
            for (int e=0 ; e<nodo_nuevo.size() ; e++)
            {
                nodo_nuevo = nodos_vivos.at(0);
                for (int v=0 ; v<*(*(matriz_bloques+e)+v) ; v++)
                {
                    nodo_nuevo.replace(e, *(*(*(tensor_bloques_id+b)+e)+v));
                    //cout << *(*(*(tensor_bloques_id+b)+e)+v) << " ";
                    for (int i=0 ; i<nodo_nuevo.size() ; i++)
                    {
                        cout << nodo_nuevo.at(i) << " ";
                    }
                    cout << endl;
                }
                cout << endl;
            }

            nodo_nuevo.clear();
            nodos_vivos.pop_front();
        }while(nodos_vivos.size() != 0);

    }


}
