/********************************************************************************
** Form generated from reading UI file 'multisigentry.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MULTISIGENTRY_H
#define UI_MULTISIGENTRY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include "qt/qvalidatedlineedit.h"

QT_BEGIN_NAMESPACE

class Ui_MultisigEntry
{
public:
    QFrame *MultisigKeyFrame;
    QWidget *layoutWidget;
    QHBoxLayout *payToLayout;
    QLabel *labelKey;
    QValidatedLineEdit *key;
    QToolButton *pasteButton;
    QToolButton *ownKey;
    QFrame *line;

    void setupUi(QWidget *MultisigEntry)
    {
        if (MultisigEntry->objectName().isEmpty())
            MultisigEntry->setObjectName(QStringLiteral("MultisigEntry"));
        MultisigEntry->resize(727, 75);
        MultisigKeyFrame = new QFrame(MultisigEntry);
        MultisigKeyFrame->setObjectName(QStringLiteral("MultisigKeyFrame"));
        MultisigKeyFrame->setGeometry(QRect(0, 0, 721, 71));
        MultisigKeyFrame->setFrameShape(QFrame::StyledPanel);
        MultisigKeyFrame->setFrameShadow(QFrame::Raised);
        layoutWidget = new QWidget(MultisigKeyFrame);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 701, 41));
        payToLayout = new QHBoxLayout(layoutWidget);
        payToLayout->setSpacing(0);
        payToLayout->setObjectName(QStringLiteral("payToLayout"));
        payToLayout->setContentsMargins(0, 0, 0, 0);
        labelKey = new QLabel(layoutWidget);
        labelKey->setObjectName(QStringLiteral("labelKey"));

        payToLayout->addWidget(labelKey);

        key = new QValidatedLineEdit(layoutWidget);
        key->setObjectName(QStringLiteral("key"));
        key->setClearButtonEnabled(true);

        payToLayout->addWidget(key);

        pasteButton = new QToolButton(layoutWidget);
        pasteButton->setObjectName(QStringLiteral("pasteButton"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/icons/editpaste"), QSize(), QIcon::Normal, QIcon::Off);
        pasteButton->setIcon(icon);
        pasteButton->setIconSize(QSize(22, 22));

        payToLayout->addWidget(pasteButton);

        ownKey = new QToolButton(layoutWidget);
        ownKey->setObjectName(QStringLiteral("ownKey"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/icons/key"), QSize(), QIcon::Normal, QIcon::Off);
        ownKey->setIcon(icon1);
        ownKey->setIconSize(QSize(22, 22));

        payToLayout->addWidget(ownKey);

        line = new QFrame(MultisigKeyFrame);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 60, 716, 3));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        retranslateUi(MultisigEntry);

        QMetaObject::connectSlotsByName(MultisigEntry);
    } // setupUi

    void retranslateUi(QWidget *MultisigEntry)
    {
        MultisigEntry->setWindowTitle(QApplication::translate("MultisigEntry", "Form", Q_NULLPTR));
        labelKey->setText(QApplication::translate("MultisigEntry", "Key  ", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        key->setToolTip(QApplication::translate("MultisigEntry", "The Earthcoin address to send the payment to", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        pasteButton->setToolTip(QApplication::translate("MultisigEntry", "Paste address from clipboard", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        pasteButton->setText(QString());
        pasteButton->setShortcut(QApplication::translate("MultisigEntry", "Alt+P", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        ownKey->setToolTip(QApplication::translate("MultisigEntry", "Generate new key", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        ownKey->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MultisigEntry: public Ui_MultisigEntry {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MULTISIGENTRY_H
