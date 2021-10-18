/********************************************************************************
** Form generated from reading UI file 'exeloadview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXELOADVIEW_H
#define UI_EXELOADVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>
#include "animatedlabel.h"
#include "animationview.h"

QT_BEGIN_NAMESPACE

class Ui_exeLoadView
{
public:
    QWidget *centralwidget;
    QLabel *background;
    QPushButton *quit_Btn;
    QLabel *exe_name;
    QLabel *exe_recipient;
    QPushButton *minimized_Btn;
    AnimatedLabel *start_Btn;
    animationView *animate_view;

    void setupUi(QMainWindow *exeLoadView)
    {
        if (exeLoadView->objectName().isEmpty())
            exeLoadView->setObjectName(QString::fromUtf8("exeLoadView"));
        exeLoadView->resize(1100, 650);
        centralwidget = new QWidget(exeLoadView);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        background = new QLabel(centralwidget);
        background->setObjectName(QString::fromUtf8("background"));
        background->setGeometry(QRect(0, 0, 1100, 650));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\226\271\346\255\243\347\262\227\351\273\221\345\256\213\347\256\200\344\275\223"));
        font.setPointSize(10);
        background->setFont(font);
        quit_Btn = new QPushButton(centralwidget);
        quit_Btn->setObjectName(QString::fromUtf8("quit_Btn"));
        quit_Btn->setGeometry(QRect(10, 10, 16, 16));
        exe_name = new QLabel(centralwidget);
        exe_name->setObjectName(QString::fromUtf8("exe_name"));
        exe_name->setGeometry(QRect(420, 450, 260, 40));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(14);
        exe_name->setFont(font1);
        exe_name->setLayoutDirection(Qt::LeftToRight);
        exe_name->setAlignment(Qt::AlignCenter);
        exe_recipient = new QLabel(centralwidget);
        exe_recipient->setObjectName(QString::fromUtf8("exe_recipient"));
        exe_recipient->setGeometry(QRect(450, 140, 200, 200));
        minimized_Btn = new QPushButton(centralwidget);
        minimized_Btn->setObjectName(QString::fromUtf8("minimized_Btn"));
        minimized_Btn->setGeometry(QRect(30, 10, 16, 16));
        start_Btn = new AnimatedLabel(centralwidget);
        start_Btn->setObjectName(QString::fromUtf8("start_Btn"));
        start_Btn->setGeometry(QRect(510, 520, 80, 80));
        animate_view = new animationView(centralwidget);
        animate_view->setObjectName(QString::fromUtf8("animate_view"));
        animate_view->setGeometry(QRect(350, 40, 400, 400));
        exeLoadView->setCentralWidget(centralwidget);
        background->raise();
        animate_view->raise();
        quit_Btn->raise();
        exe_name->raise();
        exe_recipient->raise();
        minimized_Btn->raise();
        start_Btn->raise();

        retranslateUi(exeLoadView);

        QMetaObject::connectSlotsByName(exeLoadView);
    } // setupUi

    void retranslateUi(QMainWindow *exeLoadView)
    {
        exeLoadView->setWindowTitle(QCoreApplication::translate("exeLoadView", "MainWindow", nullptr));
        background->setText(QString());
        quit_Btn->setText(QString());
        exe_name->setText(QCoreApplication::translate("exeLoadView", "\350\257\267\345\260\206\346\243\200\346\265\213\347\250\213\345\272\217\346\213\226\350\207\263\346\255\244\345\244\204", nullptr));
        exe_recipient->setText(QString());
        minimized_Btn->setText(QString());
        start_Btn->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class exeLoadView: public Ui_exeLoadView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXELOADVIEW_H
