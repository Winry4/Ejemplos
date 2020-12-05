#include "AgentXML.h"
#include "Domain/Controller.h"
#include "QPolygonF"
#include "GUI/myglwidget.h"
#include <QStandardPaths>

AgentXML *AgentXML::agent=nullptr;

AgentXML::AgentXML()
{

}

QList<QPolygonF>* AgentXML::getList_Polygons()
{
    return &list_Polygons;
}

void AgentXML::setList_Polygons(const QList<QPolygonF> &value)
{
    list_Polygons = value;
}

QList<QPolygonF> *AgentXML::getList_Polygon()
{
    return &list_Polygon;
}

void AgentXML::setList_Polygon(const QList<QPolygonF> &value)
{
    list_Polygon = value;
}
/**************************************************************************************************************************/
AgentXML *AgentXML::getInstance()
{
    if(agent == nullptr)
    {
        agent = new AgentXML();
    }
    return agent;
}

/**************************************************************************************************************************/
/**
 * @brief AgentXML::abrirXMLtoRead
 * @param location
 * @param filename
 */
void AgentXML::abrirXMLtoRead(QString location, QString filename)
{
    file = new QFile( location + filename + ".xml");
    if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "[abrirXMLtoRead]Error leyendo xml";
    }
    xmlBOM.setContent(file);
    qDebug () << " [abrirXMLtoRead]";

}

/**************************************************************************************************************************/
/**
 * @brief AgentXML::abrirXMLtoReplace. Metodo para abrir un archivo .xml
 * @param filename
 */
void AgentXML::abrirXMLtoReplace(QString location, QString filename)
{
    xml = QDomDocument("");
    root = xml.createElement("XML");
    xml.appendChild(root);

    file = new QFile( location + filename + ".xml");

    if (!file->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        qDebug() << "[abrirXMLtoReplace]Error leyendo xml " << file;
    }
    else
    {
        qDebug () << " [abrirXMLtoReplace] " << file;
    }

}

/**************************************************************************************************************************/
/**
 * @brief AgentXML::closeXML
 */
void AgentXML::closeXML()
{
    QTextStream output(file);
    output << xml.toString();
    file->close();
    qDebug () << " [closeXML]";
}

void AgentXML::removeXML(QString location, QString filename)
{
    QString prueba = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);//Devuelve una ubicación de directorio donde se pueden almacenar datos persistentes de la aplicación. Este es un directorio específico de la aplicación.
    file->remove(prueba +"/"+ location + filename + ".xml");
}

/**************************************************************************************************************************/
/**
 * @brief AgentXML::leerParcelas. Metodo para leer las Parcelas del archivo de Parcela.xml
 * @return
 */
QList<QString> AgentXML::leerParcelas()
{
    QList<QString> list_Parcelas;
    QPolygonF polygonPrueba;
    QString Parcela;
    QDomElement Child;
    QDomElement ChildAtrib;
    QDomElement root        = xmlBOM.documentElement();
    QDomElement Component   = root.firstChild().toElement();
    while(!Component.isNull())
    {
        // Check if the child tag name is Parcela
        if (Component.tagName()=="Parcelas")
        {
            // Get the first child of the component
            Child=Component.firstChild().toElement();
            // Read each child of the component node
            while (!Child.isNull())
            {
                // Read Name and value
                if (Child.tagName()=="Nombre")
                {
                    Parcela = Child.firstChild().toText().data();
                    list_Parcelas.append(Parcela);

                    ChildAtrib = Child.nextSibling().toElement();
                    while(ChildAtrib.tagName()=="Punto")
                    {
                        QString tagPunto = ChildAtrib.firstChild().toText().data();
                        if(ChildAtrib.hasAttributes())
                        {
                            QDomAttr x = ChildAtrib.attributeNode("X").toAttr();
                            QDomAttr y = ChildAtrib.attributeNode("Y").toAttr();
                            polygonPrueba.append(QPointF(x.value().toDouble(),y.value().toDouble()));
                        }
                        ChildAtrib = ChildAtrib.nextSibling().toElement();
                    }
                    list_Polygon.append(polygonPrueba);
                    polygonPrueba.clear();
                }
                // Next child
                Child = Child.nextSibling().toElement();
            }
            // Display component data
        }
        Component = Component.nextSibling().toElement();

    }
    return list_Parcelas;
}

QList<QPolygonF> AgentXML::leerPoligonos()
{
    QList<QString> list_PolygonsImplement;
    QPolygonF polygonPrueba;
    QString Polygon;
    QDomElement Child;
    QDomElement ChildAtrib;
    QDomElement root        = xmlBOM.documentElement();
    QDomElement Component   = root.firstChild().toElement();
    list_Polygons.clear();
    while(!Component.isNull())
    {
        // Check if the child tag name is Parcela
        if (Component.tagName()=="Polygons")
        {
            // Get the first child of the component
            Child=Component.firstChild().toElement();
            // Read each child of the component node
            while (!Child.isNull())
            {
                // Read Name and value
                if (Child.tagName()=="Polygon")
                {
                    Polygon = Child.firstChild().toText().data();
                    list_PolygonsImplement.append(Polygon);

                    ChildAtrib = Child.nextSibling().toElement();
                    while(ChildAtrib.tagName()=="Point")
                    {
                        QString tagPunto = ChildAtrib.firstChild().toText().data();
                        if(ChildAtrib.hasAttributes())
                        {
                            QDomAttr x = ChildAtrib.attributeNode("X").toAttr();
                            QDomAttr y = ChildAtrib.attributeNode("Y").toAttr();
                            polygonPrueba.append(QPointF(x.value().toDouble(),y.value().toDouble()));
                        }
                        ChildAtrib = ChildAtrib.nextSibling().toElement();
                    }
                    list_Polygons.append(polygonPrueba);
                    polygonPrueba.clear();
                }
                // Next child
                Child = Child.nextSibling().toElement();
            }
            // Display component data
        }
        Component = Component.nextSibling().toElement();

    }
    return list_Polygons;
}

/**************************************************************************************************************************/
/**
 * @brief AgentXML::leerUsuarios. Metodo para leer los Usuarios
 * @return
 */
QList<QString> AgentXML::leerUsuarios()
{
    QString Usuario;
    QDomElement Child;
    QList<QString> list_usuarios;
    QDomElement root        = xmlBOM.documentElement();
    QDomElement Component   = root.firstChild().toElement();
    while(!Component.isNull())
    {
        // Check if the child tag name is Usuarios
        if (Component.tagName()=="Usuarios")
        {
            // Get the first child of the component
            Child=Component.firstChild().toElement();

            // Read each child of the component node
            while (!Child.isNull())
            {
                // Read Name and value
                if (Child.tagName()=="Nombre")
                {
                    Usuario = Child.firstChild().toText().data();
                    list_usuarios.append(Usuario);
                }
                // Next child
                Child = Child.nextSibling().toElement();
            }

            // Display component data
        }
        Component = Component.nextSibling().toElement();
    }
    return list_usuarios;
}
/**************************************************************************************************************************/
/**
 * @brief AgentXML::leerXML. Metodo para leer las Tareas
 * @return
 */
QList<Tareas*> AgentXML::leerXML()
{
    QDomElement Child;
    QList<Tareas*> list_tareas;
    QDomElement root        = xmlBOM.documentElement();
    QDomElement Component   = root.firstChild().toElement();
    QDomElement ChildAtrib;

    while(!Component.isNull())
    {
        // Check if the child tag name is Tarea
        if (Component.tagName()=="Tarea")
        {
            // Get the first child of the component
            Child=Component.firstChild().toElement();
            QString Usuario;
            QString Parcela;
            QString Id;
            QString Nombre;
            QString Aperos;
            QString Comentarios;
            QString Fecha;
            QString estado;
            QString cliente;
            QString puntoReferencia;
            float latitud;
            float longitud;

            // Read each child of the component node
            while (!Child.isNull())
            {
                // Read Name and value
                if (Child.tagName()=="Usuario")
                    Usuario =Child.firstChild().toText().data();
                if (Child.tagName()=="Parcela")
                    Parcela =Child.firstChild().toText().data();
                if (Child.tagName()=="ID")
                    Id =Child.firstChild().toText().data();
                if (Child.tagName()=="Nombre")
                    Nombre =Child.firstChild().toText().data();
                if (Child.tagName()=="Aperos")
                    Aperos =Child.firstChild().toText().data();
                if (Child.tagName()== "Comentarios")
                    Comentarios =Child.firstChild().toText().data();
                if (Child.tagName()== "Fecha")
                    Fecha =Child.firstChild().toText().data();
                if (Child.tagName()=="Estado")
                    estado =Child.firstChild().toText().data();
                if (Child.tagName()=="Cliente")
                    cliente =Child.firstChild().toText().data();
                if (Child.tagName()=="PuntoReferencia")
                {
                    if(Child.hasAttributes())
                    {
                        QDomAttr x = Child.attributeNode("Lat").toAttr();
                        QDomAttr y = Child.attributeNode("Long").toAttr();
                        latitud     = x.value().toDouble();
                        longitud    = y.value().toDouble();
                    }

                    Child = Child.nextSibling().toElement();
                // Next child
                }
                Child = Child.nextSibling().toElement();
            }
            Tareas *tarea = new Tareas(Id, Nombre, Parcela, Usuario, Aperos, Comentarios, Fecha, estado, cliente, latitud, longitud);
            list_tareas.append(tarea);
            // Display component data
        }
        Component = Component.nextSibling().toElement();
    }
    return list_tareas;
}
/**************************************************************************************************************************/
/**
 * @brief AgentXML::escribirXML
 * @param tarea
 */
void AgentXML::escribirXML(Tareas *tarea)
{
    QDomElement ta = xml.createElement("Tarea");
    root.appendChild(ta);

    if(tarea->getUsuario() != "")
    {
        QDomElement tagUsuario= xml.createElement("Usuario");
        ta.appendChild(tagUsuario);
        QDomText textUsuario = xml.createTextNode(tarea->getUsuario());
        tagUsuario.appendChild(textUsuario);
    }

    QDomElement tagParcela= xml.createElement("Parcela");
    ta.appendChild(tagParcela);
    QDomText textParcela = xml.createTextNode(tarea->getParcela());
    tagParcela.appendChild(textParcela);

    QDomElement tagId= xml.createElement("ID");
    ta.appendChild(tagId);
    QDomText textId = xml.createTextNode(tarea->getId());
    tagId.appendChild(textId);

    QDomElement tagCultivo = xml.createElement("Nombre");
    ta.appendChild(tagCultivo);
    QDomText textCultivo = xml.createTextNode(tarea->getNombre());
    tagCultivo.appendChild(textCultivo);

    QDomElement tagAperos = xml.createElement("Aperos");
    ta.appendChild(tagAperos);
    QDomText textAperos = xml.createTextNode(tarea->getAperos());
    tagAperos.appendChild(textAperos);

    QDomElement tagComentarios = xml.createElement("Comentarios");
    ta.appendChild(tagComentarios);
    QDomText textComentarios = xml.createTextNode(tarea->getComentarios());
    tagComentarios.appendChild(textComentarios);

    QDomElement tagFecha = xml.createElement("Fecha");
    ta.appendChild(tagFecha);
    QDomText textFecha = xml.createTextNode(tarea->getFecha());
    tagFecha.appendChild(textFecha);

    QDomElement tagEstado = xml.createElement("Estado");
    ta.appendChild(tagEstado);
    QDomText textEstado = xml.createTextNode(tarea->getEstado());
    tagEstado.appendChild(textEstado);

    QDomElement tagCliente = xml.createElement("Cliente");
    ta.appendChild(tagCliente);
    QDomText textCliente = xml.createTextNode(tarea->getCliente());
    tagCliente.appendChild(textCliente);

    //escritura de punto de referencia de la tarea
    QDomElement tagPuntoReferencia = xml.createElement("PuntoReferencia");

    if(tarea->getId().compare(Controller::getInstance()->getTareaActual()) == 0)//Si es la tarea actual, guardar el último pto de referencia
    {
        tagPuntoReferencia.setAttribute("Long", QString::number(Rs232::getInstance()->getLongitud_inicial(),'f',20));//Mantener la precision al pasar de number a QString
        tagPuntoReferencia.setAttribute("Lat", QString::number(Rs232::getInstance()->getLatitud_inicial(),'f',20));//Mantener la precision al pasar de number a QString
    }
    else
    {
        tagPuntoReferencia.setAttribute("Long", QString::number(tarea->getLongitud(),'f',20));//Mantener la precision al pasar de number a QString.
        tagPuntoReferencia.setAttribute("Lat", QString::number(tarea->getLatitud(),'f',20));//Mantener la precision al pasar de number a QString
    }
    ta.appendChild(tagPuntoReferencia);

}
/**************************************************************************************************************************/
/**
 * @brief AgentXML::escribirParcelaXML
 * @param tarea
 */
void AgentXML::escribirParcelaXML()
{
    MyGLWidget *instanceMyGL = MyGLWidget::getInstance();

    QDomElement ta = xml.createElement("Parcelas");
    root.appendChild(ta);

    AgentXML::getInstance()->getList_Polygon()->append(instanceMyGL->getPolygonCoordenadas());
    for(int i = 0; i < Controller::getInstance()->getListParcelas()->size(); i++)
    {
        QDomElement tagCultivo = xml.createElement("Nombre");
        ta.appendChild(tagCultivo);

        QPolygonF polygon = AgentXML::getInstance()->getList_Polygon()->at(i);
        for(int j = 0; j < polygon.size(); j++)
        {
            QDomElement tagPunto = xml.createElement("Punto");
            tagPunto.setAttribute("Y", QString::number(polygon.at(j).y(),'f',20));//Mantener la precision al pasar de number a QString
            tagPunto.setAttribute("X", QString::number(polygon.at(j).x(),'f',20));//Mantener la precision al pasar de number a QString
            ta.appendChild(tagPunto);
        }

        QDomText textCultivo = xml.createTextNode(Controller::getInstance()->getListParcelas()->at(i));
        tagCultivo.appendChild(textCultivo);
    }
}

void AgentXML::escribirPolygonsXML(QList<QPolygonF> listPolygons)
{
    QDomElement ta = xml.createElement("Polygons");
    root.appendChild(ta);

    for(int i  = 0; i < listPolygons.size(); i++)
    {
        QDomElement tagPolygon = xml.createElement("Polygon");
        ta.appendChild(tagPolygon);
        QPolygonF polygon = listPolygons.at(i);
        for(int j = 0; j < polygon.size(); j++)
        {
            QDomElement tagPunto = xml.createElement("Point");
            tagPunto.setAttribute("Y", QString::number(polygon.at(j).y(),'f',20));//Mantener la precision al pasar de number a QString
            tagPunto.setAttribute("X", QString::number(polygon.at(j).x(),'f',20));//Mantener la precision al pasar de number a QString
            ta.appendChild(tagPunto);
        }

        QDomText numberPolygon = xml.createTextNode(QString::number(i));
        tagPolygon.appendChild(numberPolygon);
    }

}
void AgentXML::updateTareas()
{
    abrirXMLtoReplace("Tareas/", "Tareas");
    for(int32_t a=0; a < Controller::getInstance()->getListTareas()->size(); a++)
    {
        escribirXML(Controller::getInstance()->getListTareas()->at(a));
    }
    closeXML();
}

void AgentXML::createDirectory()
{
    QDir dir;
    QString prueba = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);//Devuelve una ubicación de directorio donde se pueden almacenar datos persistentes de la aplicación. Este es un directorio específico de la aplicación.
//    QString prueba = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    dir.setPath(prueba + "/Tareas");
    if(dir.exists())
    {
        qDebug () << "Existe " << dir.path();
    }
    else
    {
        if(dir.mkdir(prueba + "/Tareas"))
        {
            qDebug () << "Directorio Tareas creado " << dir.path();
        }
    }

    dir.setPath(prueba + "/Tareas/Poligonos");
    if(dir.exists())
    {
        qDebug () << "Existe " << dir.path();
    }
    else
    {
        if(dir.mkdir(prueba + "/Tareas/Poligonos"))
        {
            qDebug () << "Directorio Poligonos creado " << dir.path();
        }
    }

}
