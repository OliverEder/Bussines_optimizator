/*
Clase de la ventana principal
Autor: Oliver Espinosa Meneses
Version:1.0
*/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treemodel.h"
#include <QInputDialog>
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
#include "math.h"

int ***tensor_bloques_id;
double ***tensor_bloques_dist;
int **matriz_bloques;
int *vector_bloques;
double **matriz_ayacencias;

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

QJsonObject MainWindow::generar_modelo_json(QString nombre_modelo, QString descripcion)
{
    QJsonObject modelo_json;
    QJsonObject bloque;
    QJsonArray bloques;
    QJsonArray elementos;
    QJsonObject elemento;
    QJsonObject modelo_json1;
    //Se agregan campos de nombre, descripcion al JsonObject obj
    modelo_json["nombre"]=nombre_modelo;
    modelo_json["descripcion"]=descripcion;
    modelo_json["bloques"]="";

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
    modelo_json["bloques"]=bloques;
    return modelo_json;
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

    obj = generar_modelo_json( nombre_modelo, descripcion);

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
    Funcion que se ejecuta al oprimir el boton de optimizar.
    Se genera un InputDialog que muestra una lista de opciones.
    Al seleccionar una opcion se envia un parametro a la funcion
    optimizar.
*/
void MainWindow::on_actionOptimizar_triggered()
{
    QInputDialog qDialog ;
    QStringList items;
    items << QString("Egreso");
    items << QString("Ingreso");
    items << QString("Ganancia");
    qDialog.setLabelText("¿Que dese optimizar?");
    qDialog.setOptions(QInputDialog::UseListViewForComboBoxItems);
    qDialog.setComboBoxItems(items);
    qDialog.setWindowTitle("Optimizar");

    if (qDialog.exec())
    {
       optimizar(qDialog.textValue());
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
void MainWindow::optimizar(QString cadena)
{
    QList<nodo> nodos_raiz;
    QList<nodo_dist> nodos_raiz_dist;
    QList<nodo> nodos_vivos;
    QList<nodo> nodos_utiles;
    QList<nodo> nodos_grafo;
    QList<nodo_dist> nodos_grafo_dist;
    list <int> ruta;
    QMap<QString, int> resultados_ruta;

    ruta.clear();
    double max = 0;

    if(cadena == "Egreso")
    {
        //Egreso
        //Generar tensor con elementos y variantes de cada bloque.
        generar_tensor(obj);
    }
    else
    {
        calcular_maximo(cadena, &max, obj);
        cout << "Maximo:" << max << endl;
        //Generar tensor con elementos y variantes de cada bloque.
        generar_tensor_max(cadena, &max, obj);
    }
    //Generar los nodos raiz de cada bloque
    generar_raiz(&nodos_raiz, &nodos_raiz_dist);
    //Generar todos los nodos
    generar_combinatoria(&nodos_raiz, &nodos_grafo, &nodos_raiz_dist, &nodos_grafo_dist);
    //Generar Matriz de adyacencias
    generar_matriz_adyacencias(&nodos_grafo, &nodos_grafo_dist);
    //Self Adaptative Modified Pulse Couple Neural Network
    SAMPCNN(&nodos_grafo, &nodos_grafo_dist, &ruta, &resultados_ruta);
    //Generar reporte.
    generar_reporte(&nodos_grafo, &ruta, &resultados_ruta, obj);

}

/*
    Se generan dos tensores donde cada matriz contiene las variantes de cada elemento del
    bloque. Un tensor contiene los ID  de cada variante y otro tensor contiene los egresos
    de cada variante.
*/

void MainWindow::calcular_maximo(QString cadena,double *max, QJsonObject obj)
{
    QJsonObject objeto_bloque;
    QJsonArray arreglo_bloques;
    QJsonObject objeto_elemento;
    QJsonArray arreglo_elementos;
    QJsonObject objeto_variante;
    QJsonArray arreglo_variantes;

    double current=0;

    arreglo_bloques=obj["bloques"].toArray();

    for(int i=0 ; i < arreglo_bloques.size() ; i++)
    {
       //Se pasa el objeto en curso a objeto_bloque
       objeto_bloque = arreglo_bloques.at(i).toObject();
       if(objeto_bloque.value("Elementos").isArray())
       {
            arreglo_elementos = objeto_bloque.value("Elementos").toArray();
            for(int j=0 ; j < arreglo_elementos.size() ; j++)
            {
                objeto_elemento = arreglo_elementos.at(j).toObject();
                if(objeto_elemento.value("Variantes").isArray())
                {
                    arreglo_variantes = objeto_elemento.value("Variantes").toArray();
                    for(int k=0; k<arreglo_variantes.size() ; k++)
                    {
                        objeto_variante = arreglo_variantes.at(k).toObject();
                        current = objeto_variante.value(cadena).toDouble();
                        if(*max-current < 0.00)
                        {
                            *max = current;
                        }

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
    Se generan areglos con la informacion de los bloques, elemenots y variantes de la
    estrucutra JSON.
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
    tensor_bloques_dist = (double ***) malloc(sizeof(double**) * arreglo_bloques.size());
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
            tensor_bloques_dist[i] = (double **) malloc(sizeof(double*) * arreglo_elementos.size());
            for(int j=0 ; j < arreglo_elementos.size() ; j++)
            {
                objeto_elemento = arreglo_elementos.at(j).toObject();
                if(objeto_elemento.value("Variantes").isArray())
                {
                    arreglo_variantes = objeto_elemento.value("Variantes").toArray();
                    *(*(matriz_bloques+i)+j)= arreglo_variantes.size();
                    tensor_bloques_id[i][j] = (int *) malloc(sizeof(int) * arreglo_variantes.size());
                    tensor_bloques_dist[i][j] = (double *) malloc(sizeof(double) * arreglo_variantes.size());
                    for(int k=0; k<arreglo_variantes.size() ; k++)
                    {
                        objeto_variante = arreglo_variantes.at(k).toObject();
                        *(*(*(tensor_bloques_id+i)+j)+k) = objeto_variante.value("ID").toInt();
                        *(*(*(tensor_bloques_dist+i)+j)+k) = objeto_variante.value("Egreso").toDouble();
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
    Se generan areglos con la informacion de los bloques, elemenots y variantes de la
    estrucutra JSON. Esta funcion se creo para tomar en cuenta una optimizacion de ingresos
    o gastos.
*/
void MainWindow::generar_tensor_max(QString cadena,double *max, QJsonObject obj)
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
    tensor_bloques_dist = (double ***) malloc(sizeof(double**) * arreglo_bloques.size());
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
            tensor_bloques_dist[i] = (double **) malloc(sizeof(double*) * arreglo_elementos.size());
            for(int j=0 ; j < arreglo_elementos.size() ; j++)
            {
                objeto_elemento = arreglo_elementos.at(j).toObject();
                if(objeto_elemento.value("Variantes").isArray())
                {
                    arreglo_variantes = objeto_elemento.value("Variantes").toArray();
                    *(*(matriz_bloques+i)+j)= arreglo_variantes.size();
                    tensor_bloques_id[i][j] = (int *) malloc(sizeof(int) * arreglo_variantes.size());
                    tensor_bloques_dist[i][j] = (double *) malloc(sizeof(double) * arreglo_variantes.size());
                    for(int k=0; k<arreglo_variantes.size() ; k++)
                    {
                        objeto_variante = arreglo_variantes.at(k).toObject();
                        *(*(*(tensor_bloques_id+i)+j)+k) = objeto_variante.value("ID").toInt();
                        *(*(*(tensor_bloques_dist+i)+j)+k) = (*max + objeto_variante.value(cadena).toDouble()) + 1;
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
void MainWindow::generar_raiz(QList<nodo> *nodos_raiz, QList<nodo_dist> *nodos_raiz_dist)
{
    nodo nodo_raiz;
    nodo_dist raiz_dist;

    cout << "Generando nodos raiz" << endl;

    for (int i=0 ; i<8 ; i++)
    {
        for (int j=0 ; j<*(vector_bloques+i) ; j++)
        {
            nodo_raiz.append(*(*(*(tensor_bloques_id+i)+j)+0));
            raiz_dist.append(*(*(*(tensor_bloques_dist+i)+j)+0));
        }
        nodos_raiz_dist->append(raiz_dist);
        raiz_dist.clear();

        nodos_raiz->append(nodo_raiz);
        nodo_raiz.clear();
    }

}
/*
    Se genera la combinatoria de todos los nodos del arbol
    se recibe una lista con los nodos raiz de cada bloque

*/
void MainWindow::generar_combinatoria(QList<nodo> *nodos_raiz, QList<nodo> *nodos_grafo, QList<nodo_dist> *nodos_raiz_dist, QList<nodo_dist> *nodos_grafo_dist)
{
    nodo nodo_nuevo;    
    nodo nodo_final;    
    QList<nodo> nodos_vivos;    
    QList<nodo> nodos_utiles;

    nodo_dist nodo_fdist;
    nodo_dist nodo_ndist;
    QList<nodo_dist> nodos_vdist;

    cout << "Generando combinatoria" << endl;
    for (int b=0 ; b<nodos_raiz->size() ; b++)
    {
        nodos_vivos.append(nodos_raiz->at(b));
        nodos_vdist.append(nodos_raiz_dist->at(b));

        nodos_utiles.append(nodos_raiz->at(b));

        nodo_final.clear();
        nodo_final = nodos_vivos.at(0);
        nodo_final.push_front(b);
        nodos_grafo->append(nodo_final);

        nodo_fdist.clear();
        nodo_fdist = nodos_vdist.at(0);
        nodo_fdist.push_front(b);
        nodos_grafo_dist->append(nodo_fdist);
        do{
            nodo_nuevo = nodos_vivos.at(0);
            nodo_ndist = nodos_vdist.at(0);
            for (int e=0 ; e<nodo_nuevo.size() ; e++)
            {
                nodo_nuevo = nodos_vivos.at(0);
                nodo_ndist = nodos_vdist.at(0);
                for (int v=0 ; v<*(*(matriz_bloques+e)+v) ; v++)
                {
                    nodo_nuevo.replace(e, *(*(*(tensor_bloques_id+b)+e)+v));
                    nodo_ndist.replace(e, *(*(*(tensor_bloques_dist+b)+e)+v));

                    if(nodos_utiles.contains(nodo_nuevo))
                    {
                        //cout << "El nodo ya existe " << nodos_utiles.contains(nodo_nuevo)<< endl;
                    }
                    else
                    {
                        //cout << "nodo nuevo " << nodos_utiles.contains(nodo_nuevo)<< endl;

                        nodos_vivos.append(nodo_nuevo);
                        nodos_utiles.append(nodo_nuevo);
                        nodo_final.clear();
                        nodo_final = nodo_nuevo;
                        nodo_final.push_front(b);
                        nodos_grafo->append(nodo_final);

                        nodos_vdist.append(nodo_ndist);
                        nodo_fdist.clear();
                        nodo_fdist = nodo_ndist;
                        nodo_fdist.push_front(b);
                        nodos_grafo_dist->append(nodo_fdist);
                    }
                }
            }
            nodo_final.clear();
            nodo_nuevo.clear();
            nodos_vivos.pop_front();

            nodo_fdist.clear();
            nodo_ndist.clear();
            nodos_vdist.pop_front();
        }while(nodos_vivos.size() != 0);

    }
    //Se agrega el primer nodo el cual sera el punto de partida de la busqueda
    nodo_final.clear();
    nodo_final.push_front(-1);
    nodo_final.push_front(-1);
    nodos_grafo->push_front(nodo_final);
    nodo_final.clear();

    nodo_fdist.clear();
    nodo_fdist.push_front(1);
    nodo_fdist.push_front(-1);
    nodos_grafo_dist->push_front(nodo_fdist);
    nodo_fdist.clear();

    //se arega el ultimo nodo, el cul sera el punto objetivo de la busqueda
    nodo_final.clear();
    nodo_final.push_front(-1);
    nodo_final.push_front(8);
    nodos_grafo->push_back(nodo_final);
    nodo_final.clear();

    nodo_fdist.clear();
    nodo_fdist.push_front(1);
    nodo_fdist.push_front(8);
    nodos_grafo_dist->push_back(nodo_fdist);
    nodo_fdist.clear();



    cout << "nodos finales:" << nodos_grafo->size() <<endl;
    cout << "distancias finales:" << nodos_grafo_dist->size() <<endl;
    /*
    int contador= 0;
    for (int i=0 ; i<nodos_grafo->size() ; i++)
    {
        contador++;
        cout << contador << " -> " ;
        for (int j=0 ; j<nodos_grafo->at(i).size(); j++)
        {

            cout << nodos_grafo->at(i).at(j) << " ";
        }
        cout << endl;
    }
    */

}
/*
    Se genera la matriz de adyacencias
*/
void MainWindow::generar_matriz_adyacencias(QList<nodo> *nodos_grafo, QList<nodo_dist> *nodos_grafo_dist)
{
    cout << "Generando matriz de adyacencias" << endl;
    int m_lenth = nodos_grafo->size();
    matriz_ayacencias = (double **) malloc(sizeof(double*) * m_lenth);
    for (int i=0 ; i<m_lenth ; i++)
    {
        matriz_ayacencias[i] = (double *) malloc(sizeof(double) * m_lenth);
        for (int j=0 ; j<m_lenth ; j++)
        {
            *(*(matriz_ayacencias+i)+j) = 0;
        }

    }

    for (int b=-1 ; b<9; b++)
    {
        for (int i=0 ; i<m_lenth ; i++)
        {
            if(nodos_grafo->at(i).at(0) == b)
            {
                for (int j=0 ; j<m_lenth ; j++)
                {
                    if(nodos_grafo->at(j).at(0) == b+1)
                    {
                        //*(*(matriz_ayacencias+i)+j) = 1;
                        //*(*(matriz_ayacencias+j)+i) = 1;
                        double dist_total=0;
                        for (int k=0 ; k<nodos_grafo_dist->at(j).size()-1 ; k++)
                        {
                              dist_total = dist_total + nodos_grafo_dist->at(j).at(k+1);
                        }
                        *(*(matriz_ayacencias+i)+j) = dist_total;
                        *(*(matriz_ayacencias+j)+i) = dist_total;
                    }
                }
            }
        }
    }

    /*
    int count =0;
    for (int i=0 ; i<m_lenth ; i++)
    {
        cout << count << endl;
        count++;
        for (int j=0 ; j<m_lenth ; j++)
        {
            cout << *(*(matriz_ayacencias+i)+j) << "," ;
        }
    cout << endl;
    }
    */

}
/*
    Sefl Adaptative Modified Pulse Coupled Neural Network
*/
void MainWindow::SAMPCNN(QList<nodo> *nodos_grafo, QList<nodo_dist> *nodos_grafo_dist, list<int> *ruta, QMap<QString, int> *resultados_ruta)
{
    int **puntero_matrizU;
    int **puntero_matrizY;
    int menor=1000;
    int E_umbral=0;
    int suma_pulsos=0;
    int delta_E=1;
    int pulsofinal=0;
    int iteracion=0;
    int current=0;
    int nodo_s = 1;
    int nodo_g = nodos_grafo->size();
    unsigned t0, t1;

    list <int> historico;
    list <int> agenda;
    //list <int> ruta;

    //Matrices dinamicas U e Y
    puntero_matrizU = new int*[nodos_grafo->size()];
    puntero_matrizY = new int*[nodos_grafo->size()];
    for(int i=0 ; i<nodos_grafo->size() ; i++)
    {
        puntero_matrizU[i]= new int[nodos_grafo->size()];
        puntero_matrizY[i]= new int[nodos_grafo->size()];
    }
    //Se llenan de ceros las matrices U e Y
    for(int i=0;i<nodos_grafo->size();i++)
    {
        for(int j=0 ; j<nodos_grafo->size() ; j++)
        {
           *(*(puntero_matrizU+i)+j)=0;
           *(*(puntero_matrizY+i)+j)=0;
        }
    }

    //cout<<"indice de nodo incial "<<nodo_s<<endl;
    //cout<<"indice de nodo final "<<nodo_g<<endl;
    historico.push_front(nodo_s-1);

    //se copian el costo de los nodos vecinos al nodo inicial.
    for(int i=0 ; i<nodos_grafo->size() ; i++)
    {
        *(*(puntero_matrizU+nodo_s-1)+i) = *(*(matriz_ayacencias+nodo_s-1)+i);
    }


    //se obtiene el costo menor de entre los costos a los nodos vecinos del nodo incial.
    for(int i=0; i< nodos_grafo->size() ; i++)
    {
        if(*(*(puntero_matrizU+nodo_s-1)+i)!=0)
        {
            if(*(*(puntero_matrizU+nodo_s-1)+i)<=menor)
            {
                menor=*(*(puntero_matrizU+nodo_s-1)+i);
            }
        }
    }
    E_umbral=menor;
    //Se inicia la toma del tiempo
    t0=clock();
    do{

        //inicio de ciclo

        //Los valores que complen la condicion del umbral en la matriz U dispararan en la matriz y
        list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int j=0 ; j<nodos_grafo->size() ; j++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+j)!=0)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+j)==E_umbral)
                    {
                        for(int k=0 ; k<nodos_grafo->size() ; k++)
                        {
                            if(*(*(puntero_matrizY+k)+j)==1)
                            {
                                suma_pulsos=suma_pulsos+1;
                            }
                        }
                        if(suma_pulsos==0)
                        {
                            *(*(puntero_matrizY+*iterador_historico)+j)=1;
                            agenda.push_back(j);
                            historico.push_back(j);
                        }
                        suma_pulsos=0;
                    }
                }
            }
        }
        //Se agregan los nodos vecinos en la matriz U, de los nodos que dispararon en el nodo Y
        list<int>::iterator iterador_agenda;
        for(iterador_agenda=agenda.begin();iterador_agenda!=agenda.end();iterador_agenda++)
        {
            for(int i=0 ; i<nodos_grafo->size() ; i++)
            {
                if(*(*(matriz_ayacencias+*iterador_agenda)+i)!=0)
                {
                    *(*(puntero_matrizU+*iterador_agenda)+i)=*(*(matriz_ayacencias+*iterador_agenda)+i)+E_umbral;
                }

            }
        }


        //se pone el valor alto en los nodos vecinos ya disparados.
        //list<int>::iterator iterador_historico;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {

            for(int i=0 ; i<nodos_grafo->size() ; i++)
            {
                list<int>::iterator iterador_historico1;
                for(iterador_historico1 = historico.begin();iterador_historico1!=historico.end();iterador_historico1++)
                {
                    if(i==*iterador_historico1 && *(*(matriz_ayacencias+*iterador_historico)+i)!=0)
                    {
                        *(*(puntero_matrizU+*iterador_historico)+i) = 10000;
                    }
                }
            }
        }

        //se borran los nodos de la agenda.
        agenda.clear();
        //Se obtiene el costo menor de la matriz U de los nodos adyasentes a los nodos que ya han disparado en la matriz Y.
        //Direrentes de 0 y mayores que el umbral anterior.
        menor = 1000;
        for(iterador_historico = historico.begin();iterador_historico!=historico.end();iterador_historico++)
        {
            for(int i=0 ; i<nodos_grafo->size() ; i++)
            {
                if(*(*(puntero_matrizU+*iterador_historico)+i)!=0 && *(*(puntero_matrizU+*iterador_historico)+i)> E_umbral)
                {
                    if(*(*(puntero_matrizU+*iterador_historico)+i)<=menor)
                    {
                        menor=*(*(puntero_matrizU+*iterador_historico)+i);
                    }
                }
            }
        }
        //Se incrementa E_umbral en delta_E.
        delta_E=menor-E_umbral;
        E_umbral=E_umbral+delta_E;
        //se verifica si alguno de los vecinos del nodo final se ha encendido
        for(int i=0 ; i<nodos_grafo->size() ; i++)
        {
            if(*(*(puntero_matrizY+i)+nodo_g-1)==1)
            {
                pulsofinal=1;
                //cout<<"Nodo alcanzado"<<endl;

            }
        }

        iteracion=iteracion+1;
        //cout<<"iteracion:"<<iteracion<<endl;
        //fin de ciclo
    }while(pulsofinal!=1);
    current=nodo_g-1;
    ruta->push_front(current);
    //cout<<"Nodo final:"<<current+1<<endl;
    //cout<<"Extraccion del conocimiento"<<endl;
    do{
        for(int i=0 ; i<nodos_grafo->size() ; i++)
        {
            if(*(*(puntero_matrizY+i)+current)==1)
            {
                current=i;
                ruta->push_front(current);
            }
        }
    }while(current!=nodo_s-1);

    t1=clock();
    /*
    for(int i=0;i<lineas_cadena;i++)
    {
        for(int j=0;j<lineas_cadena;j++)
        {
           cout<<*(*(puntero_matrizY+i)+j)<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    */
    /*
    list<int>::iterator iterador_ruta;
    for(iterador_ruta = ruta->begin();iterador_ruta!=ruta->end();iterador_ruta++)
    {
        cout<<*iterador_ruta+1<<"-";
    }
    cout<<endl;
    */
    double time = (double(t1-t0)/CLOCKS_PER_SEC);
    resultados_ruta->insert("Costo", E_umbral-delta_E);
    resultados_ruta->insert("Tiempo", time);
    //cout << "Tiempo de ejecucion: " << time << " segundos" << endl;
    resultados_ruta->insert("Iteraciones", iteracion);
    //cout<<"Iteraciones:"<<iteracion<<endl;
    //Libera memoria
    for(int i=0 ; i<nodos_grafo->size() ; i++)
    {
        delete[] puntero_matrizU[i];
        delete[] puntero_matrizY[i];

    }
    delete puntero_matrizU;
    delete puntero_matrizY;
}
/*
    Extraer los nodos
    Lista de nodos pertenecientes a la ruta
    Bloque
        Elemento
            Variante
        Elemento
            Variante
    Bloque
        Elemento
            Variante
        Elemento
            Variante
*/

void MainWindow::generar_reporte(QList<nodo> *nodos_grafo, list<int> *ruta, QMap<QString, int> *resultados_ruta, QJsonObject obj)
{
    QJsonObject modelo_optimizado;
    QList<nodo> nodos_reporte;
    nodo nodo;
    ruta->pop_front();
    ruta->pop_back();
    list<int>::iterator iterador_ruta;

    for(iterador_ruta= ruta->begin() ; iterador_ruta != ruta->end() ; iterador_ruta++)
    {
        for(int i=0 ; i<nodos_grafo->size() ; i++)
        {
            if(*iterador_ruta == i)
            {
                //nodos_reporte.append(nodos_grafo->at(*iterador_ruta));
                nodo = nodos_grafo->at(*iterador_ruta);
                nodos_reporte.append(nodo);
            }
        }
    }
    cout << "nodos:" << nodos_reporte.size() << endl;
    modelo_optimizado = generar_modelo_json(obj["nombre"].toString(), obj["descripcion"].toString());


}
