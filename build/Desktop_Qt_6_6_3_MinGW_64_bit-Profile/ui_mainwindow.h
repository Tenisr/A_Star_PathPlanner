/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QComboBox *visualComboBox;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QPushButton *pathPlanButton;
    QSpacerItem *verticalSpacer_3;
    QGraphicsView *graphicsView;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(902, 664);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        visualComboBox = new QComboBox(centralwidget);
        visualComboBox->addItem(QString());
        visualComboBox->addItem(QString());
        visualComboBox->addItem(QString());
        visualComboBox->setObjectName("visualComboBox");
        visualComboBox->setEditable(false);
        visualComboBox->setCurrentText(QString::fromUtf8(""));
        visualComboBox->setMaxVisibleItems(10);
        visualComboBox->setDuplicatesEnabled(false);

        gridLayout->addWidget(visualComboBox, 1, 1, 1, 1);

        label = new QLabel(centralwidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 1, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        pathPlanButton = new QPushButton(centralwidget);
        pathPlanButton->setObjectName("pathPlanButton");

        verticalLayout->addWidget(pathPlanButton);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        horizontalLayout_2->addLayout(verticalLayout);

        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setInteractive(true);

        horizontalLayout_2->addWidget(graphicsView);

        horizontalLayout_2->setStretch(1, 3);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 902, 18));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        visualComboBox->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        visualComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271\347\273\210\347\202\271", nullptr));
        visualComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\347\237\251\345\275\242\351\232\234\347\242\215", nullptr));
        visualComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\345\234\206\345\275\242\351\232\234\347\242\215", nullptr));

        visualComboBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "\346\234\252\351\200\211\346\213\251", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\345\217\257\350\247\206\345\214\226\346\226\271\345\274\217", nullptr));
        pathPlanButton->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\350\267\257\345\276\204\350\247\204\345\210\222", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
