/********************************************************************************
** Form generated from reading UI file 'animationview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ANIMATIONVIEW_H
#define UI_ANIMATIONVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_animationView
{
public:

    void setupUi(QWidget *animationView)
    {
        if (animationView->objectName().isEmpty())
            animationView->setObjectName(QString::fromUtf8("animationView"));
        animationView->resize(400, 400);

        retranslateUi(animationView);

        QMetaObject::connectSlotsByName(animationView);
    } // setupUi

    void retranslateUi(QWidget *animationView)
    {
        animationView->setWindowTitle(QCoreApplication::translate("animationView", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class animationView: public Ui_animationView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ANIMATIONVIEW_H
