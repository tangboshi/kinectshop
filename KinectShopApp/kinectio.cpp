#include "kinectio.h"

/**
 * @brief Constructor
 */
kinectio::kinectio()
{
}

/**
 * @brief Ausgabe, welches Produkt (von der Kinect) erkannt wurde
 * Eine Nachricht mit der Produkt ID wird in einen Stream geladen, dieser wird in einen String umgewandelt und dann in einem QString gespeichert und ausgegeben.
 * @return QString: Nachricht mit erkannter Produkt ID
 */
QString kinectio::updateDetectedPidViewer(){
    stringstream stream;

    stream << "<p>Folgendes Produkt wurde erkannt: " << detectedPid << " </p>" << endl;

    string s = stream.str();
    QString htmlOutput = QString::fromStdString(s);

    return htmlOutput;
}

/**
 * @brief setter
 * @param pid erkannte Produkt ID
 * @return void
 */
void kinectio::setDetectedPid(int pid){
    detectedPid = pid;
}

/**
 * @brief getter
 * @return int: erkannte Produkt ID
 */
int kinectio::getDetectedPid(){
    return detectedPid;
}
