// Copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "xoptions.h"

XOptions::XOptions(QObject *parent) : QObject(parent)
{

}

void XOptions::setFilePath(QString sFilePath)
{
    this->sFilePath=sFilePath;
}

void XOptions::setVariantNames(QList<ID> listVariantIDs)
{
    this->listVariantIDs=listVariantIDs;
}

QMap<XOptions::ID, QVariant> *XOptions::getVariants()
{
    return &mapVariants;
}

void XOptions::load()
{
    QSettings settings(sFilePath,QSettings::IniFormat);

    int nCount=listVariantIDs.count();

    for(int i=0;i<nCount;i++)
    {
        ID id=listVariantIDs.at(i);
        QString sName=idToString(id);
        mapVariants.insert(id,settings.value(sName));
    }
}

void XOptions::save()
{
    QSettings settings(sFilePath,QSettings::IniFormat);

    int nCount=listVariantIDs.count();

    for(int i=0;i<nCount;i++)
    {
        ID id=listVariantIDs.at(i);
        QString sName=idToString(id);
        settings.setValue(sName,mapVariants.value(id));
    }
}

QString XOptions::idToString(ID id)
{
    QString sResult="Unknown";

    switch(id)
    {
        case ID_STAYONTOP:          sResult=QString("StayOnTop");               break;
    }

    return sResult;
}
