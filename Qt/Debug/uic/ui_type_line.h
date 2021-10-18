/********************************************************************************
** Form generated from reading UI file 'type_line.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TYPE_LINE_H
#define UI_TYPE_LINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "animatedlabel.h"

QT_BEGIN_NAMESPACE

class Ui_Type_Line
{
public:
    AnimatedLabel *background;
    QLabel *image;
    QLabel *label;
    QLabel *stateView;

    void setupUi(QWidget *Type_Line)
    {
        if (Type_Line->objectName().isEmpty())
            Type_Line->setObjectName(QString::fromUtf8("Type_Line"));
        Type_Line->resize(240, 80);
        background = new AnimatedLabel(Type_Line);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 240, 80));
        image = new QLabel(Type_Line);
        image->setObjectName(QString::fromUtf8("image"));
        image->setGeometry(QRect(30, 10, 60, 60));
        label = new QLabel(Type_Line);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(110, 25, 50, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\271\346\255\243\345\214\227\351\255\217\346\245\267\344\271\246\347\256\200\344\275\223"));
        font.setPointSize(10);
        label->setFont(font);
        stateView = new QLabel(Type_Line);
        stateView->setObjectName(QString::fromUtf8("stateView"));
        stateView->setGeometry(QRect(180, 30, 20, 20));

        retranslateUi(Type_Line);

        QMetaObject::connectSlotsByName(Type_Line);
    } // setupUi

    void retranslateUi(QWidget *Type_Line)
    {
        Type_Line->setWindowTitle(QCoreApplication::translate("Type_Line", "Form", nullptr));
        background->setText(QString());
        image->setText(QString());
        label->setText(QString());
        stateView->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Type_Line: public Ui_Type_Line {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TYPE_LINE_H
