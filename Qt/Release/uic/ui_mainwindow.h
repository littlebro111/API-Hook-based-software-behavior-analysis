/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "animatedlabel.h"
#include "infoview.h"
#include "type_line.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *quit_Btn;
    QPushButton *minimized_Btn;
    QLabel *big_icon;
    QLabel *background_left;
    QLabel *background_right;
    QWidget *layoutWidget;
    QVBoxLayout *typeList_layout;
    Type_Line *heap_line;
    Type_Line *net_line;
    Type_Line *file_line;
    Type_Line *memory_line;
    Type_Line *register_line;
    InfoView *info_view;
    AnimatedLabel *back_Btn;
    QLabel *exe_name;
    QLabel *process_view;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1100, 650);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        quit_Btn = new QPushButton(centralwidget);
        quit_Btn->setObjectName(QString::fromUtf8("quit_Btn"));
        quit_Btn->setGeometry(QRect(10, 10, 16, 16));
        minimized_Btn = new QPushButton(centralwidget);
        minimized_Btn->setObjectName(QString::fromUtf8("minimized_Btn"));
        minimized_Btn->setGeometry(QRect(30, 10, 16, 16));
        big_icon = new QLabel(centralwidget);
        big_icon->setObjectName(QString::fromUtf8("big_icon"));
        big_icon->setGeometry(QRect(800, 215, 220, 220));
        background_left = new QLabel(centralwidget);
        background_left->setObjectName(QString::fromUtf8("background_left"));
        background_left->setGeometry(QRect(0, 0, 320, 650));
        background_right = new QLabel(centralwidget);
        background_right->setObjectName(QString::fromUtf8("background_right"));
        background_right->setGeometry(QRect(320, 0, 780, 650));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 120, 241, 491));
        typeList_layout = new QVBoxLayout(layoutWidget);
        typeList_layout->setObjectName(QString::fromUtf8("typeList_layout"));
        typeList_layout->setSizeConstraint(QLayout::SetDefaultConstraint);
        typeList_layout->setContentsMargins(0, 0, 0, 0);
        heap_line = new Type_Line(layoutWidget);
        heap_line->setObjectName(QString::fromUtf8("heap_line"));

        typeList_layout->addWidget(heap_line);

        net_line = new Type_Line(layoutWidget);
        net_line->setObjectName(QString::fromUtf8("net_line"));

        typeList_layout->addWidget(net_line);

        file_line = new Type_Line(layoutWidget);
        file_line->setObjectName(QString::fromUtf8("file_line"));

        typeList_layout->addWidget(file_line);

        memory_line = new Type_Line(layoutWidget);
        memory_line->setObjectName(QString::fromUtf8("memory_line"));

        typeList_layout->addWidget(memory_line);

        register_line = new Type_Line(layoutWidget);
        register_line->setObjectName(QString::fromUtf8("register_line"));

        typeList_layout->addWidget(register_line);

        info_view = new InfoView(centralwidget);
        info_view->setObjectName(QString::fromUtf8("info_view"));
        info_view->setGeometry(QRect(320, 0, 420, 650));
        back_Btn = new AnimatedLabel(centralwidget);
        back_Btn->setObjectName(QString::fromUtf8("back_Btn"));
        back_Btn->setGeometry(QRect(870, 520, 80, 80));
        exe_name = new QLabel(centralwidget);
        exe_name->setObjectName(QString::fromUtf8("exe_name"));
        exe_name->setGeometry(QRect(40, 70, 100, 20));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(11);
        exe_name->setFont(font);
        process_view = new QLabel(centralwidget);
        process_view->setObjectName(QString::fromUtf8("process_view"));
        process_view->setGeometry(QRect(160, 40, 80, 80));
        MainWindow->setCentralWidget(centralwidget);
        background_left->raise();
        background_right->raise();
        quit_Btn->raise();
        minimized_Btn->raise();
        big_icon->raise();
        layoutWidget->raise();
        info_view->raise();
        back_Btn->raise();
        exe_name->raise();
        process_view->raise();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        quit_Btn->setText(QString());
        minimized_Btn->setText(QString());
        big_icon->setText(QString());
        background_left->setText(QString());
        background_right->setText(QString());
        back_Btn->setText(QString());
        exe_name->setText(QCoreApplication::translate("MainWindow", "test.exe", nullptr));
        process_view->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
