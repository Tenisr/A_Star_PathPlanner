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
#include <QtWidgets/QDoubleSpinBox>
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
    QLabel *label;
    QComboBox *visualComboBox;
    QDoubleSpinBox *gridSize;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLabel *label_3;
    QDoubleSpinBox *fstPenaltyRad;
    QDoubleSpinBox *sedPenaltyValue;
    QDoubleSpinBox *fstPenaltyValue;
    QDoubleSpinBox *sedPenaltyRad;
    QLabel *label_5;
    QLabel *label_6;
    QPushButton *clearButton;
    QSpacerItem *verticalSpacer_4;
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
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 1, 1, 1);

        visualComboBox = new QComboBox(centralwidget);
        visualComboBox->addItem(QString());
        visualComboBox->addItem(QString());
        visualComboBox->addItem(QString());
        visualComboBox->setObjectName("visualComboBox");
        visualComboBox->setEditable(false);
        visualComboBox->setCurrentText(QString::fromUtf8(""));
        visualComboBox->setMaxVisibleItems(10);
        visualComboBox->setDuplicatesEnabled(false);

        gridLayout->addWidget(visualComboBox, 0, 2, 1, 1);

        gridSize = new QDoubleSpinBox(centralwidget);
        gridSize->setObjectName("gridSize");
        gridSize->setValue(12.000000000000000);

        gridLayout->addWidget(gridSize, 1, 2, 1, 1);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName("gridLayout_2");
        label_4 = new QLabel(centralwidget);
        label_4->setObjectName("label_4");

        gridLayout_2->addWidget(label_4, 1, 0, 1, 1);

        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");

        gridLayout_2->addWidget(label_3, 0, 0, 1, 1);

        fstPenaltyRad = new QDoubleSpinBox(centralwidget);
        fstPenaltyRad->setObjectName("fstPenaltyRad");
        fstPenaltyRad->setDecimals(0);
        fstPenaltyRad->setMaximum(9999.000000000000000);
        fstPenaltyRad->setSingleStep(2.000000000000000);
        fstPenaltyRad->setValue(24.000000000000000);

        gridLayout_2->addWidget(fstPenaltyRad, 0, 1, 1, 1);

        sedPenaltyValue = new QDoubleSpinBox(centralwidget);
        sedPenaltyValue->setObjectName("sedPenaltyValue");
        sedPenaltyValue->setDecimals(1);
        sedPenaltyValue->setMaximum(9999.000000000000000);
        sedPenaltyValue->setValue(10.000000000000000);

        gridLayout_2->addWidget(sedPenaltyValue, 1, 3, 1, 1);

        fstPenaltyValue = new QDoubleSpinBox(centralwidget);
        fstPenaltyValue->setObjectName("fstPenaltyValue");
        fstPenaltyValue->setDecimals(1);
        fstPenaltyValue->setMaximum(9999.000000000000000);
        fstPenaltyValue->setSingleStep(5.000000000000000);
        fstPenaltyValue->setValue(20.000000000000000);

        gridLayout_2->addWidget(fstPenaltyValue, 0, 3, 1, 1);

        sedPenaltyRad = new QDoubleSpinBox(centralwidget);
        sedPenaltyRad->setObjectName("sedPenaltyRad");
        sedPenaltyRad->setDecimals(0);
        sedPenaltyRad->setMaximum(9999.000000000000000);
        sedPenaltyRad->setSingleStep(4.000000000000000);
        sedPenaltyRad->setValue(48.000000000000000);

        gridLayout_2->addWidget(sedPenaltyRad, 1, 1, 1, 1);

        label_5 = new QLabel(centralwidget);
        label_5->setObjectName("label_5");

        gridLayout_2->addWidget(label_5, 0, 2, 1, 1);

        label_6 = new QLabel(centralwidget);
        label_6->setObjectName("label_6");

        gridLayout_2->addWidget(label_6, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        clearButton = new QPushButton(centralwidget);
        clearButton->setObjectName("clearButton");

        verticalLayout->addWidget(clearButton);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

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
        label->setText(QCoreApplication::translate("MainWindow", "\345\217\257\350\247\206\345\214\226\346\226\271\345\274\217", nullptr));
        visualComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "\350\265\267\347\202\271\347\273\210\347\202\271", nullptr));
        visualComboBox->setItemText(1, QCoreApplication::translate("MainWindow", "\347\237\251\345\275\242\351\232\234\347\242\215", nullptr));
        visualComboBox->setItemText(2, QCoreApplication::translate("MainWindow", "\345\234\206\345\275\242\351\232\234\347\242\215", nullptr));

        visualComboBox->setPlaceholderText(QCoreApplication::translate("MainWindow", "\346\234\252\351\200\211\346\213\251", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\240\205\346\240\274\345\244\247\345\260\217", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\344\272\214\351\230\266\346\203\251\347\275\232\345\215\212\345\276\204", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\344\270\200\351\230\266\346\203\251\347\275\232\345\215\212\345\276\204", nullptr));
        sedPenaltyValue->setSuffix(QString());
        label_5->setText(QCoreApplication::translate("MainWindow", "\344\270\200\351\230\266\346\203\251\347\275\232\345\200\274", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\344\272\214\351\230\266\346\203\251\347\275\232\345\200\274", nullptr));
        clearButton->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\234\260\345\233\276", nullptr));
        pathPlanButton->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\350\267\257\345\276\204\350\247\204\345\210\222", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
