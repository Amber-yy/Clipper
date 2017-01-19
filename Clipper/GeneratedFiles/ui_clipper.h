/********************************************************************************
** Form generated from reading UI file 'clipper.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIPPER_H
#define UI_CLIPPER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClipperClass
{
public:

    void setupUi(QWidget *ClipperClass)
    {
        if (ClipperClass->objectName().isEmpty())
            ClipperClass->setObjectName(QStringLiteral("ClipperClass"));
        ClipperClass->resize(600, 400);

        retranslateUi(ClipperClass);

        QMetaObject::connectSlotsByName(ClipperClass);
    } // setupUi

    void retranslateUi(QWidget *ClipperClass)
    {
        ClipperClass->setWindowTitle(QApplication::translate("ClipperClass", "Clipper", 0));
    } // retranslateUi

};

namespace Ui {
    class ClipperClass: public Ui_ClipperClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIPPER_H
