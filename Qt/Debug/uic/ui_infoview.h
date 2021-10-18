/********************************************************************************
** Form generated from reading UI file 'infoview.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOVIEW_H
#define UI_INFOVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InfoView
{
public:
    QLabel *API_Title;
    QLabel *submain_Title;
    QLabel *main_Title;
    QWidget *space_line;
    QLabel *api_line_1;
    QLabel *api_line_2;
    QLabel *api_line_3;
    QLabel *api_line_4;
    QLabel *v1_img;
    QLabel *v1_mainLabel;
    QLabel *v1_subLabel;
    QLabel *v2_subLabel;
    QLabel *v2_mainLabel;
    QLabel *v2_img;
    QLabel *v3_subLabel;
    QLabel *v3_mainLabel;
    QLabel *v3_img;

    void setupUi(QWidget *InfoView)
    {
        if (InfoView->objectName().isEmpty())
            InfoView->setObjectName(QString::fromUtf8("InfoView"));
        InfoView->resize(460, 650);
        API_Title = new QLabel(InfoView);
        API_Title->setObjectName(QString::fromUtf8("API_Title"));
        API_Title->setGeometry(QRect(80, 430, 150, 30));
        QFont font;
        font.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font.setPointSize(12);
        API_Title->setFont(font);
        submain_Title = new QLabel(InfoView);
        submain_Title->setObjectName(QString::fromUtf8("submain_Title"));
        submain_Title->setGeometry(QRect(82, 100, 300, 20));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font1.setPointSize(9);
        submain_Title->setFont(font1);
        main_Title = new QLabel(InfoView);
        main_Title->setObjectName(QString::fromUtf8("main_Title"));
        main_Title->setGeometry(QRect(80, 40, 120, 50));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font2.setPointSize(22);
        main_Title->setFont(font2);
        space_line = new QWidget(InfoView);
        space_line->setObjectName(QString::fromUtf8("space_line"));
        space_line->setGeometry(QRect(70, 410, 340, 1));
        api_line_1 = new QLabel(InfoView);
        api_line_1->setObjectName(QString::fromUtf8("api_line_1"));
        api_line_1->setGeometry(QRect(80, 470, 300, 22));
        QFont font3;
        font3.setFamily(QString::fromUtf8("Calibri"));
        font3.setPointSize(12);
        api_line_1->setFont(font3);
        api_line_2 = new QLabel(InfoView);
        api_line_2->setObjectName(QString::fromUtf8("api_line_2"));
        api_line_2->setGeometry(QRect(80, 500, 300, 22));
        api_line_2->setFont(font3);
        api_line_3 = new QLabel(InfoView);
        api_line_3->setObjectName(QString::fromUtf8("api_line_3"));
        api_line_3->setGeometry(QRect(80, 530, 300, 22));
        api_line_3->setFont(font3);
        api_line_4 = new QLabel(InfoView);
        api_line_4->setObjectName(QString::fromUtf8("api_line_4"));
        api_line_4->setGeometry(QRect(80, 560, 300, 22));
        api_line_4->setFont(font3);
        v1_img = new QLabel(InfoView);
        v1_img->setObjectName(QString::fromUtf8("v1_img"));
        v1_img->setGeometry(QRect(90, 160, 40, 30));
        v1_mainLabel = new QLabel(InfoView);
        v1_mainLabel->setObjectName(QString::fromUtf8("v1_mainLabel"));
        v1_mainLabel->setGeometry(QRect(160, 150, 130, 16));
        QFont font4;
        font4.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        font4.setPointSize(10);
        v1_mainLabel->setFont(font4);
        v1_subLabel = new QLabel(InfoView);
        v1_subLabel->setObjectName(QString::fromUtf8("v1_subLabel"));
        v1_subLabel->setGeometry(QRect(160, 180, 270, 16));
        QFont font5;
        font5.setFamily(QString::fromUtf8("\351\273\221\344\275\223"));
        v1_subLabel->setFont(font5);
        v2_subLabel = new QLabel(InfoView);
        v2_subLabel->setObjectName(QString::fromUtf8("v2_subLabel"));
        v2_subLabel->setGeometry(QRect(160, 260, 270, 16));
        v2_subLabel->setFont(font5);
        v2_mainLabel = new QLabel(InfoView);
        v2_mainLabel->setObjectName(QString::fromUtf8("v2_mainLabel"));
        v2_mainLabel->setGeometry(QRect(160, 230, 130, 16));
        v2_mainLabel->setFont(font4);
        v2_img = new QLabel(InfoView);
        v2_img->setObjectName(QString::fromUtf8("v2_img"));
        v2_img->setGeometry(QRect(90, 240, 40, 30));
        v3_subLabel = new QLabel(InfoView);
        v3_subLabel->setObjectName(QString::fromUtf8("v3_subLabel"));
        v3_subLabel->setGeometry(QRect(160, 340, 270, 16));
        v3_subLabel->setFont(font5);
        v3_mainLabel = new QLabel(InfoView);
        v3_mainLabel->setObjectName(QString::fromUtf8("v3_mainLabel"));
        v3_mainLabel->setGeometry(QRect(160, 310, 130, 16));
        v3_mainLabel->setFont(font4);
        v3_img = new QLabel(InfoView);
        v3_img->setObjectName(QString::fromUtf8("v3_img"));
        v3_img->setGeometry(QRect(90, 320, 40, 30));

        retranslateUi(InfoView);

        QMetaObject::connectSlotsByName(InfoView);
    } // setupUi

    void retranslateUi(QWidget *InfoView)
    {
        InfoView->setWindowTitle(QCoreApplication::translate("InfoView", "Form", nullptr));
        API_Title->setText(QCoreApplication::translate("InfoView", "\350\260\203\347\224\250\347\232\204\347\233\270\345\205\263API\357\274\232", nullptr));
        submain_Title->setText(QCoreApplication::translate("InfoView", "\345\206\205\345\255\230\346\213\267\350\264\235\347\233\221\346\265\213\344\270\216\345\205\263\350\201\224\345\210\206\346\236\220", nullptr));
        main_Title->setText(QCoreApplication::translate("InfoView", "\346\263\250\345\206\214\350\241\250", nullptr));
        api_line_1->setText(QCoreApplication::translate("InfoView", " \302\267 FileMove", nullptr));
        api_line_2->setText(QCoreApplication::translate("InfoView", " \302\267 FileMove", nullptr));
        api_line_3->setText(QCoreApplication::translate("InfoView", " \302\267 FileMove", nullptr));
        api_line_4->setText(QCoreApplication::translate("InfoView", " \302\267 FileMove", nullptr));
        v1_img->setText(QString());
        v1_mainLabel->setText(QCoreApplication::translate("InfoView", "\346\223\215\344\275\2341", nullptr));
        v1_subLabel->setText(QCoreApplication::translate("InfoView", "\345\216\237\345\233\240\345\210\206\346\236\2201", nullptr));
        v2_subLabel->setText(QCoreApplication::translate("InfoView", "\345\216\237\345\233\240\345\210\206\346\236\2202", nullptr));
        v2_mainLabel->setText(QCoreApplication::translate("InfoView", "\346\223\215\344\275\2342", nullptr));
        v2_img->setText(QString());
        v3_subLabel->setText(QCoreApplication::translate("InfoView", "\345\216\237\345\233\240\345\210\206\346\236\2203", nullptr));
        v3_mainLabel->setText(QCoreApplication::translate("InfoView", "\346\223\215\344\275\2343", nullptr));
        v3_img->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class InfoView: public Ui_InfoView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOVIEW_H
