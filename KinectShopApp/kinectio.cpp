#include "kinectio.h"

kinectio::kinectio()
{
}

QString kinectio::updateDetectedPidViewer(){
    stringstream stream;

    stream << "<p>Folgendes Produkt wurde erkannt: " << detectedPid << " </p>" << endl;

    string s = stream.str();
    QString htmlOutput = QString::fromStdString(s);

    return htmlOutput;
}

void kinectio::setDetectedPid(int pid){
    detectedPid = pid;
}

int kinectio::getDetectedPid(){
    return detectedPid;
}
