/********************************************************************************
** Form generated from reading UI file 'multisigdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTISIGDIALOG_H
#define UI_MULTISIGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MultisigDialog
{
public:
    QWidget *gridLayoutWidget;
    QGridLayout *entries;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QLabel *labelKeys;
    QSpinBox *spinBoxKeys;
    QSpacerItem *horizontalSpacer;
    QLabel *labelRequired;
    QSpinBox *spinBoxRequired;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *setButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *createButton;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout;
    QLabel *labelAddress;
    QLabel *labelRedeemScript;
    QLineEdit *lineEditAddress;
    QPlainTextEdit *lineEditRedeemScript;
    QToolButton *copyScriptButton;
    QToolButton *copyAddressButton;
    QFrame *line;
    QFrame *line_2;

    void setupUi(QDialog *MultisigDialog)
    {
        if (MultisigDialog->objectName().isEmpty())
            MultisigDialog->setObjectName(QStringLiteral("MultisigDialog"));
        MultisigDialog->resize(805, 577);
        gridLayoutWidget = new QWidget(MultisigDialog);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(10, 60, 781, 371));
        entries = new QGridLayout(gridLayoutWidget);
        entries->setObjectName(QStringLiteral("entries"));
        entries->setContentsMargins(0, 0, 0, 0);
        layoutWidget_2 = new QWidget(MultisigDialog);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 10, 781, 31));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setSpacing(4);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        labelKeys = new QLabel(layoutWidget_2);
        labelKeys->setObjectName(QStringLiteral("labelKeys"));

        horizontalLayout->addWidget(labelKeys);

        spinBoxKeys = new QSpinBox(layoutWidget_2);
        spinBoxKeys->setObjectName(QStringLiteral("spinBoxKeys"));
        spinBoxKeys->setMinimum(2);
        spinBoxKeys->setMaximum(8);
        spinBoxKeys->setValue(3);

        horizontalLayout->addWidget(spinBoxKeys);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        labelRequired = new QLabel(layoutWidget_2);
        labelRequired->setObjectName(QStringLiteral("labelRequired"));

        horizontalLayout->addWidget(labelRequired);

        spinBoxRequired = new QSpinBox(layoutWidget_2);
        spinBoxRequired->setObjectName(QStringLiteral("spinBoxRequired"));
        spinBoxRequired->setMinimum(1);
        spinBoxRequired->setMaximum(8);
        spinBoxRequired->setValue(2);

        horizontalLayout->addWidget(spinBoxRequired);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        setButton = new QPushButton(layoutWidget_2);
        setButton->setObjectName(QStringLiteral("setButton"));

        horizontalLayout->addWidget(setButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        createButton = new QPushButton(layoutWidget_2);
        createButton->setObjectName(QStringLiteral("createButton"));

        horizontalLayout->addWidget(createButton);

        gridLayoutWidget_2 = new QWidget(MultisigDialog);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(10, 450, 781, 118));
        gridLayout = new QGridLayout(gridLayoutWidget_2);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        labelAddress = new QLabel(gridLayoutWidget_2);
        labelAddress->setObjectName(QStringLiteral("labelAddress"));

        gridLayout->addWidget(labelAddress, 1, 0, 1, 1);

        labelRedeemScript = new QLabel(gridLayoutWidget_2);
        labelRedeemScript->setObjectName(QStringLiteral("labelRedeemScript"));

        gridLayout->addWidget(labelRedeemScript, 4, 0, 1, 1);

        lineEditAddress = new QLineEdit(gridLayoutWidget_2);
        lineEditAddress->setObjectName(QStringLiteral("lineEditAddress"));
        lineEditAddress->setReadOnly(true);

        gridLayout->addWidget(lineEditAddress, 1, 1, 1, 1);

        lineEditRedeemScript = new QPlainTextEdit(gridLayoutWidget_2);
        lineEditRedeemScript->setObjectName(QStringLiteral("lineEditRedeemScript"));

        gridLayout->addWidget(lineEditRedeemScript, 4, 1, 1, 1);

        copyScriptButton = new QToolButton(gridLayoutWidget_2);
        copyScriptButton->setObjectName(QStringLiteral("copyScriptButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/editcopy"), QSize(), QIcon::Normal, QIcon::Off);
        copyScriptButton->setIcon(icon);
        copyScriptButton->setIconSize(QSize(22, 22));

        gridLayout->addWidget(copyScriptButton, 4, 2, 1, 1);

        copyAddressButton = new QToolButton(gridLayoutWidget_2);
        copyAddressButton->setObjectName(QStringLiteral("copyAddressButton"));
        copyAddressButton->setIcon(icon);
        copyAddressButton->setIconSize(QSize(22, 22));

        gridLayout->addWidget(copyAddressButton, 1, 2, 1, 1);

        line = new QFrame(MultisigDialog);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 430, 801, 20));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        line_2 = new QFrame(MultisigDialog);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setGeometry(QRect(0, 40, 801, 20));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        retranslateUi(MultisigDialog);

        QMetaObject::connectSlotsByName(MultisigDialog);
    } // setupUi

    void retranslateUi(QDialog *MultisigDialog)
    {
        MultisigDialog->setWindowTitle(QApplication::translate("MultisigDialog", "Multisig Dialog", Q_NULLPTR));
        labelKeys->setText(QApplication::translate("MultisigDialog", "Count of Keys", Q_NULLPTR));
        labelRequired->setText(QApplication::translate("MultisigDialog", "Required Keys", Q_NULLPTR));
        setButton->setText(QApplication::translate("MultisigDialog", "Set Keys / Required", Q_NULLPTR));
        createButton->setText(QApplication::translate("MultisigDialog", "Create Multisig", Q_NULLPTR));
        labelAddress->setText(QApplication::translate("MultisigDialog", "Address", Q_NULLPTR));
        labelRedeemScript->setText(QApplication::translate("MultisigDialog", "Redeem Script", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        copyScriptButton->setToolTip(QApplication::translate("MultisigDialog", "Copy redeem script to clipboard", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        copyScriptButton->setText(QString());
        copyScriptButton->setShortcut(QApplication::translate("MultisigDialog", "Alt+P", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        copyAddressButton->setToolTip(QApplication::translate("MultisigDialog", "Copy address to clipboard", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        copyAddressButton->setText(QString());
        copyAddressButton->setShortcut(QApplication::translate("MultisigDialog", "Alt+P", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MultisigDialog: public Ui_MultisigDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTISIGDIALOG_H
