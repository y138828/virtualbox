/* $Id$ */
/** @file
 * VBox Qt GUI - QIStyledItemDelegate class declaration.
 */

/*
 * Copyright (C) 2006-2016 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifndef ___QIStyledItemDelegate_h___
#define ___QIStyledItemDelegate_h___

/* Qt includes: */
# include <QStyledItemDelegate>


/** QStyledItemDelegate subclass extending standard functionality. */
class QIStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT;

signals:

    /** Notifies listeners about editor's Enter key triggering. */
    void sigEditorEnterKeyTriggered();

public:

    /** Constructs delegate passing @a pParent to the base-class. */
    QIStyledItemDelegate(QObject *pParent)
        : QStyledItemDelegate(pParent)
        , m_fWatchForEditorEnterKeyTriggering(false)
    {}

    /** Defines whether delegate should watch for the editor's Enter key triggering. */
    void setWatchForEditorEnterKeyTriggering(bool fWatch) { m_fWatchForEditorEnterKeyTriggering = fWatch; }

private:

    /** Returns the widget used to edit the item specified by @a index.
      * The @a pParent widget and style @a option are used to control how the editor widget appears. */
    QWidget* createEditor(QWidget *pParent, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        /* Call to base-class to get actual editor created: */
        QWidget *pEditor = QStyledItemDelegate::createEditor(pParent, option, index);

        connect(pEditor, SIGNAL(sigCommitData(QWidget*)), this, SIGNAL(commitData(QWidget*)));

        /* Watch for editor Enter key triggering, redirect to listeners: */
        if (m_fWatchForEditorEnterKeyTriggering)
            connect(pEditor, SIGNAL(sigEnterKeyTriggered()), this, SIGNAL(sigEditorEnterKeyTriggered()));

        /* Return actual editor: */
        return pEditor;
    }

    /** Holds whether delegate should watch for the editor's Enter key triggering. */
    bool m_fWatchForEditorEnterKeyTriggering;
};

#endif /* !___QIStyledItemDelegate_h___ */

