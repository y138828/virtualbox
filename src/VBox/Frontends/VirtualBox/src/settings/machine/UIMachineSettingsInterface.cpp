/* $Id$ */
/** @file
 * VBox Qt GUI - UIMachineSettingsInterface class implementation.
 */

/*
 * Copyright (C) 2008-2017 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */

#ifdef VBOX_WITH_PRECOMPILED_HEADERS
# include <precomp.h>
#else  /* !VBOX_WITH_PRECOMPILED_HEADERS */

/* GUI includes: */
# include "UIActionPool.h"
# include "UIExtraDataManager.h"
# include "UIMachineSettingsInterface.h"

#endif /* !VBOX_WITH_PRECOMPILED_HEADERS */


/** Machine settings: User Interface page data structure. */
struct UIDataSettingsMachineInterface
{
    /** Constructs data. */
    UIDataSettingsMachineInterface()
        : m_fStatusBarEnabled(false)
#ifndef VBOX_WS_MAC
        , m_fMenuBarEnabled(false)
#endif /* !VBOX_WS_MAC */
        , m_restrictionsOfMenuBar(UIExtraDataMetaDefs::MenuType_Invalid)
        , m_restrictionsOfMenuApplication(UIExtraDataMetaDefs::MenuApplicationActionType_Invalid)
        , m_restrictionsOfMenuMachine(UIExtraDataMetaDefs::RuntimeMenuMachineActionType_Invalid)
        , m_restrictionsOfMenuView(UIExtraDataMetaDefs::RuntimeMenuViewActionType_Invalid)
        , m_restrictionsOfMenuInput(UIExtraDataMetaDefs::RuntimeMenuInputActionType_Invalid)
        , m_restrictionsOfMenuDevices(UIExtraDataMetaDefs::RuntimeMenuDevicesActionType_Invalid)
#ifdef VBOX_WITH_DEBUGGER_GUI
        , m_restrictionsOfMenuDebug(UIExtraDataMetaDefs::RuntimeMenuDebuggerActionType_Invalid)
#endif /* VBOX_WITH_DEBUGGER_GUI */
#ifdef VBOX_WS_MAC
        , m_restrictionsOfMenuWindow(UIExtraDataMetaDefs::MenuWindowActionType_Invalid)
#endif /* VBOX_WS_MAC */
        , m_restrictionsOfMenuHelp(UIExtraDataMetaDefs::MenuHelpActionType_Invalid)
#ifndef VBOX_WS_MAC
        , m_fShowMiniToolBar(false)
        , m_fMiniToolBarAtTop(false)
#endif /* !VBOX_WS_MAC */
    {}

    /** Returns whether the @a other passed data is equal to this one. */
    bool equal(const UIDataSettingsMachineInterface &other) const
    {
        return true
               && (m_fStatusBarEnabled == other.m_fStatusBarEnabled)
               && (m_statusBarRestrictions == other.m_statusBarRestrictions)
               && (m_statusBarOrder == other.m_statusBarOrder)
#ifndef VBOX_WS_MAC
               && (m_fMenuBarEnabled == other.m_fMenuBarEnabled)
#endif /* !VBOX_WS_MAC */
               && (m_restrictionsOfMenuBar == other.m_restrictionsOfMenuBar)
               && (m_restrictionsOfMenuApplication == other.m_restrictionsOfMenuApplication)
               && (m_restrictionsOfMenuMachine == other.m_restrictionsOfMenuMachine)
               && (m_restrictionsOfMenuView == other.m_restrictionsOfMenuView)
               && (m_restrictionsOfMenuInput == other.m_restrictionsOfMenuInput)
               && (m_restrictionsOfMenuDevices == other.m_restrictionsOfMenuDevices)
#ifdef VBOX_WITH_DEBUGGER_GUI
               && (m_restrictionsOfMenuDebug == other.m_restrictionsOfMenuDebug)
#endif /* VBOX_WITH_DEBUGGER_GUI */
#ifdef VBOX_WS_MAC
               && (m_restrictionsOfMenuWindow == other.m_restrictionsOfMenuWindow)
#endif /* VBOX_WS_MAC */
               && (m_restrictionsOfMenuHelp == other.m_restrictionsOfMenuHelp)
#ifndef VBOX_WS_MAC
               && (m_fShowMiniToolBar == other.m_fShowMiniToolBar)
               && (m_fMiniToolBarAtTop == other.m_fMiniToolBarAtTop)
#endif /* !VBOX_WS_MAC */
               ;
    }

    /** Returns whether the @a other passed data is equal to this one. */
    bool operator==(const UIDataSettingsMachineInterface &other) const { return equal(other); }
    /** Returns whether the @a other passed data is different from this one. */
    bool operator!=(const UIDataSettingsMachineInterface &other) const { return !equal(other); }

    /** Holds whether the status-bar is enabled. */
    bool                  m_fStatusBarEnabled;
    /** Holds the status-bar indicator restrictions. */
    QList<IndicatorType>  m_statusBarRestrictions;
    /** Holds the status-bar indicator order. */
    QList<IndicatorType>  m_statusBarOrder;

#ifndef VBOX_WS_MAC
    /** Holds whether the menu-bar is enabled. */
    bool                                                m_fMenuBarEnabled;
#endif /* !VBOX_WS_MAC */
    /** Holds the menu-bar menu restrictions. */
    UIExtraDataMetaDefs::MenuType                       m_restrictionsOfMenuBar;
    /** Holds the Application menu restrictions. */
    UIExtraDataMetaDefs::MenuApplicationActionType      m_restrictionsOfMenuApplication;
    /** Holds the Machine menu restrictions. */
    UIExtraDataMetaDefs::RuntimeMenuMachineActionType   m_restrictionsOfMenuMachine;
    /** Holds the View menu restrictions. */
    UIExtraDataMetaDefs::RuntimeMenuViewActionType      m_restrictionsOfMenuView;
    /** Holds the Input menu restrictions. */
    UIExtraDataMetaDefs::RuntimeMenuInputActionType     m_restrictionsOfMenuInput;
    /** Holds the Devices menu restrictions. */
    UIExtraDataMetaDefs::RuntimeMenuDevicesActionType   m_restrictionsOfMenuDevices;
#ifdef VBOX_WITH_DEBUGGER_GUI
    /** Holds the Debug menu restrictions. */
    UIExtraDataMetaDefs::RuntimeMenuDebuggerActionType  m_restrictionsOfMenuDebug;
#endif /* VBOX_WITH_DEBUGGER_GUI */
#ifdef VBOX_WS_MAC
    /** Holds the Window menu restrictions. */
    UIExtraDataMetaDefs::MenuWindowActionType           m_restrictionsOfMenuWindow;
#endif /* VBOX_WS_MAC */
    /** Holds the Help menu restrictions. */
    UIExtraDataMetaDefs::MenuHelpActionType             m_restrictionsOfMenuHelp;

#ifndef VBOX_WS_MAC
    /** Holds whether the mini-toolbar is enabled. */
    bool  m_fShowMiniToolBar;
    /** Holds whether the mini-toolbar should be aligned at top of screen. */
    bool  m_fMiniToolBarAtTop;
#endif /* !VBOX_WS_MAC */
};


UIMachineSettingsInterface::UIMachineSettingsInterface(const QString strMachineId)
    : m_strMachineId(strMachineId)
    , m_pActionPool(0)
    , m_pCache(0)
{
    /* Prepare: */
    prepare();
}

UIMachineSettingsInterface::~UIMachineSettingsInterface()
{
    /* Cleanup: */
    cleanup();
}

bool UIMachineSettingsInterface::changed() const
{
    return m_pCache->wasChanged();
}

void UIMachineSettingsInterface::loadToCacheFrom(QVariant &data)
{
    /* Fetch data to machine: */
    UISettingsPageMachine::fetchData(data);

    /* Clear cache initially: */
    m_pCache->clear();

    /* Prepare old interface data: */
    UIDataSettingsMachineInterface oldInterfaceData;

    /* Gather old interface data: */
    oldInterfaceData.m_fStatusBarEnabled = gEDataManager->statusBarEnabled(m_machine.GetId());
    oldInterfaceData.m_statusBarRestrictions = gEDataManager->restrictedStatusBarIndicators(m_machine.GetId());
    oldInterfaceData.m_statusBarOrder = gEDataManager->statusBarIndicatorOrder(m_machine.GetId());
#ifndef VBOX_WS_MAC
    oldInterfaceData.m_fMenuBarEnabled = gEDataManager->menuBarEnabled(m_machine.GetId());
#endif
    oldInterfaceData.m_restrictionsOfMenuBar = gEDataManager->restrictedRuntimeMenuTypes(m_machine.GetId());
    oldInterfaceData.m_restrictionsOfMenuApplication = gEDataManager->restrictedRuntimeMenuApplicationActionTypes(m_machine.GetId());
    oldInterfaceData.m_restrictionsOfMenuMachine = gEDataManager->restrictedRuntimeMenuMachineActionTypes(m_machine.GetId());
    oldInterfaceData.m_restrictionsOfMenuView = gEDataManager->restrictedRuntimeMenuViewActionTypes(m_machine.GetId());
    oldInterfaceData.m_restrictionsOfMenuInput = gEDataManager->restrictedRuntimeMenuInputActionTypes(m_machine.GetId());
    oldInterfaceData.m_restrictionsOfMenuDevices = gEDataManager->restrictedRuntimeMenuDevicesActionTypes(m_machine.GetId());
#ifdef VBOX_WITH_DEBUGGER_GUI
    oldInterfaceData.m_restrictionsOfMenuDebug = gEDataManager->restrictedRuntimeMenuDebuggerActionTypes(m_machine.GetId());
#endif
#ifdef VBOX_WS_MAC
    oldInterfaceData.m_restrictionsOfMenuWindow = gEDataManager->restrictedRuntimeMenuWindowActionTypes(m_machine.GetId());
#endif
    oldInterfaceData.m_restrictionsOfMenuHelp = gEDataManager->restrictedRuntimeMenuHelpActionTypes(m_machine.GetId());
#ifndef VBOX_WS_MAC
    oldInterfaceData.m_fShowMiniToolBar = gEDataManager->miniToolbarEnabled(m_machine.GetId());
    oldInterfaceData.m_fMiniToolBarAtTop = gEDataManager->miniToolbarAlignment(m_machine.GetId()) == Qt::AlignTop;
#endif

    /* Cache old interface data: */
    m_pCache->cacheInitialData(oldInterfaceData);

    /* Upload machine to data: */
    UISettingsPageMachine::uploadData(data);
}

void UIMachineSettingsInterface::getFromCache()
{
    /* Get old interface data from the cache: */
    const UIDataSettingsMachineInterface &oldInterfaceData = m_pCache->base();

    /* Load old interface data to the page: */
    m_pStatusBarEditor->setStatusBarEnabled(oldInterfaceData.m_fStatusBarEnabled);
    m_pStatusBarEditor->setStatusBarConfiguration(oldInterfaceData.m_statusBarRestrictions,
                                                  oldInterfaceData.m_statusBarOrder);
#ifndef VBOX_WS_MAC
    m_pMenuBarEditor->setMenuBarEnabled(oldInterfaceData.m_fMenuBarEnabled);
#endif
    m_pMenuBarEditor->setRestrictionsOfMenuBar(oldInterfaceData.m_restrictionsOfMenuBar);
    m_pMenuBarEditor->setRestrictionsOfMenuApplication(oldInterfaceData.m_restrictionsOfMenuApplication);
    m_pMenuBarEditor->setRestrictionsOfMenuMachine(oldInterfaceData.m_restrictionsOfMenuMachine);
    m_pMenuBarEditor->setRestrictionsOfMenuView(oldInterfaceData.m_restrictionsOfMenuView);
    m_pMenuBarEditor->setRestrictionsOfMenuInput(oldInterfaceData.m_restrictionsOfMenuInput);
    m_pMenuBarEditor->setRestrictionsOfMenuDevices(oldInterfaceData.m_restrictionsOfMenuDevices);
#ifdef VBOX_WITH_DEBUGGER_GUI
    m_pMenuBarEditor->setRestrictionsOfMenuDebug(oldInterfaceData.m_restrictionsOfMenuDebug);
#endif
#ifdef VBOX_WS_MAC
    m_pMenuBarEditor->setRestrictionsOfMenuWindow(oldInterfaceData.m_restrictionsOfMenuWindow);
#endif
    m_pMenuBarEditor->setRestrictionsOfMenuHelp(oldInterfaceData.m_restrictionsOfMenuHelp);
#ifndef VBOX_WS_MAC
    m_pCheckBoxShowMiniToolBar->setChecked(oldInterfaceData.m_fShowMiniToolBar);
    m_pComboToolBarAlignment->setChecked(oldInterfaceData.m_fMiniToolBarAtTop);
#endif

    /* Polish page finally: */
    polishPage();

    /* Revalidate: */
    revalidate();
}

void UIMachineSettingsInterface::putToCache()
{
    /* Prepare new interface data: */
    UIDataSettingsMachineInterface newInterfaceData = m_pCache->base();

    /* Gather new interface data from page: */
    newInterfaceData.m_fStatusBarEnabled = m_pStatusBarEditor->isStatusBarEnabled();
    newInterfaceData.m_statusBarRestrictions = m_pStatusBarEditor->statusBarIndicatorRestrictions();
    newInterfaceData.m_statusBarOrder = m_pStatusBarEditor->statusBarIndicatorOrder();
#ifndef VBOX_WS_MAC
    newInterfaceData.m_fMenuBarEnabled = m_pMenuBarEditor->isMenuBarEnabled();
#endif
    newInterfaceData.m_restrictionsOfMenuBar = m_pMenuBarEditor->restrictionsOfMenuBar();
    newInterfaceData.m_restrictionsOfMenuApplication = m_pMenuBarEditor->restrictionsOfMenuApplication();
    newInterfaceData.m_restrictionsOfMenuMachine = m_pMenuBarEditor->restrictionsOfMenuMachine();
    newInterfaceData.m_restrictionsOfMenuView = m_pMenuBarEditor->restrictionsOfMenuView();
    newInterfaceData.m_restrictionsOfMenuInput = m_pMenuBarEditor->restrictionsOfMenuInput();
    newInterfaceData.m_restrictionsOfMenuDevices = m_pMenuBarEditor->restrictionsOfMenuDevices();
#ifdef VBOX_WITH_DEBUGGER_GUI
    newInterfaceData.m_restrictionsOfMenuDebug = m_pMenuBarEditor->restrictionsOfMenuDebug();
#endif
#ifdef VBOX_WS_MAC
    newInterfaceData.m_restrictionsOfMenuWindow = m_pMenuBarEditor->restrictionsOfMenuWindow();
#endif
    newInterfaceData.m_restrictionsOfMenuHelp = m_pMenuBarEditor->restrictionsOfMenuHelp();
#ifndef VBOX_WS_MAC
    newInterfaceData.m_fShowMiniToolBar = m_pCheckBoxShowMiniToolBar->isChecked();
    newInterfaceData.m_fMiniToolBarAtTop = m_pComboToolBarAlignment->isChecked();
#endif

    /* Cache new interface data: */
    m_pCache->cacheCurrentData(newInterfaceData);
}

void UIMachineSettingsInterface::saveFromCacheTo(QVariant &data)
{
    /* Fetch data to machine: */
    UISettingsPageMachine::fetchData(data);

    /* Make sure machine is in valid mode & interface data was changed: */
    if (isMachineInValidMode() && m_pCache->wasChanged())
    {
        /* Get old interface data from the cache: */
        const UIDataSettingsMachineInterface &oldInterfaceData = m_pCache->base();
        /* Get new interface data from the cache: */
        const UIDataSettingsMachineInterface &newInterfaceData = m_pCache->data();

        /* Store whether status-bar is enabled: */
        if (newInterfaceData.m_fStatusBarEnabled != oldInterfaceData.m_fStatusBarEnabled)
            gEDataManager->setStatusBarEnabled(newInterfaceData.m_fStatusBarEnabled, m_machine.GetId());
        /* Store status-bar restrictions: */
        if (newInterfaceData.m_statusBarRestrictions != oldInterfaceData.m_statusBarRestrictions)
            gEDataManager->setRestrictedStatusBarIndicators(newInterfaceData.m_statusBarRestrictions, m_machine.GetId());
        /* Store status-bar order: */
        if (newInterfaceData.m_statusBarOrder != oldInterfaceData.m_statusBarOrder)
            gEDataManager->setStatusBarIndicatorOrder(newInterfaceData.m_statusBarOrder, m_machine.GetId());
#ifndef VBOX_WS_MAC
        /* Store whether menu-bar is enabled: */
        if (newInterfaceData.m_fMenuBarEnabled != oldInterfaceData.m_fMenuBarEnabled)
            gEDataManager->setMenuBarEnabled(newInterfaceData.m_fMenuBarEnabled, m_machine.GetId());
#endif
        /* Store menu-bar restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuBar != oldInterfaceData.m_restrictionsOfMenuBar)
            gEDataManager->setRestrictedRuntimeMenuTypes(newInterfaceData.m_restrictionsOfMenuBar, m_machine.GetId());
        /* Store menu-bar Application menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuApplication != oldInterfaceData.m_restrictionsOfMenuApplication)
            gEDataManager->setRestrictedRuntimeMenuApplicationActionTypes(newInterfaceData.m_restrictionsOfMenuApplication, m_machine.GetId());
        /* Store menu-bar Machine menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuMachine != oldInterfaceData.m_restrictionsOfMenuMachine)
            gEDataManager->setRestrictedRuntimeMenuMachineActionTypes(newInterfaceData.m_restrictionsOfMenuMachine, m_machine.GetId());
        /* Store menu-bar View menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuView != oldInterfaceData.m_restrictionsOfMenuView)
            gEDataManager->setRestrictedRuntimeMenuViewActionTypes(newInterfaceData.m_restrictionsOfMenuView, m_machine.GetId());
        /* Store menu-bar Input menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuInput != oldInterfaceData.m_restrictionsOfMenuInput)
            gEDataManager->setRestrictedRuntimeMenuInputActionTypes(newInterfaceData.m_restrictionsOfMenuInput, m_machine.GetId());
        /* Store menu-bar Devices menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuDevices != oldInterfaceData.m_restrictionsOfMenuDevices)
            gEDataManager->setRestrictedRuntimeMenuDevicesActionTypes(newInterfaceData.m_restrictionsOfMenuDevices, m_machine.GetId());
#ifdef VBOX_WITH_DEBUGGER_GUI
        /* Store menu-bar Debug menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuDebug != oldInterfaceData.m_restrictionsOfMenuDebug)
            gEDataManager->setRestrictedRuntimeMenuDebuggerActionTypes(newInterfaceData.m_restrictionsOfMenuDebug, m_machine.GetId());
#endif
#ifdef VBOX_WS_MAC
        /* Store menu-bar Window menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuWindow != oldInterfaceData.m_restrictionsOfMenuWindow)
            gEDataManager->setRestrictedRuntimeMenuWindowActionTypes(newInterfaceData.m_restrictionsOfMenuWindow, m_machine.GetId());
#endif
        /* Store menu-bar Help menu restrictions: */
        if (newInterfaceData.m_restrictionsOfMenuHelp != oldInterfaceData.m_restrictionsOfMenuHelp)
            gEDataManager->setRestrictedRuntimeMenuHelpActionTypes(newInterfaceData.m_restrictionsOfMenuHelp, m_machine.GetId());
#ifndef VBOX_WS_MAC
        /* Store whether mini-toolbar is enabled: */
        if (newInterfaceData.m_fShowMiniToolBar != oldInterfaceData.m_fShowMiniToolBar)
            gEDataManager->setMiniToolbarEnabled(newInterfaceData.m_fShowMiniToolBar, m_machine.GetId());
        /* Store whether mini-toolbar should be location at top of screen: */
        if (newInterfaceData.m_fMiniToolBarAtTop != oldInterfaceData.m_fMiniToolBarAtTop)
            gEDataManager->setMiniToolbarAlignment(newInterfaceData.m_fMiniToolBarAtTop ? Qt::AlignTop : Qt::AlignBottom, m_machine.GetId());
#endif
    }

    /* Upload machine to data: */
    UISettingsPageMachine::uploadData(data);
}

void UIMachineSettingsInterface::retranslateUi()
{
    /* Translate uic generated strings: */
    Ui::UIMachineSettingsInterface::retranslateUi(this);
}

void UIMachineSettingsInterface::polishPage()
{
    /* Polish interface availability: */
    m_pMenuBarEditor->setEnabled(isMachineInValidMode());
#ifdef VBOX_WS_MAC
    m_pLabelMiniToolBar->hide();
    m_pCheckBoxShowMiniToolBar->hide();
    m_pComboToolBarAlignment->hide();
#else /* !VBOX_WS_MAC */
    m_pLabelMiniToolBar->setEnabled(isMachineInValidMode());
    m_pCheckBoxShowMiniToolBar->setEnabled(isMachineInValidMode());
    m_pComboToolBarAlignment->setEnabled(isMachineInValidMode() && m_pCheckBoxShowMiniToolBar->isChecked());
#endif /* !VBOX_WS_MAC */
    m_pStatusBarEditor->setEnabled(isMachineInValidMode());
}

void UIMachineSettingsInterface::prepare()
{
    /* Apply UI decorations: */
    Ui::UIMachineSettingsInterface::setupUi(this);

    /* Prepare cache: */
    m_pCache = new UISettingsCacheMachineInterface;
    AssertPtrReturnVoid(m_pCache);

    /* Layout created in the .ui file. */
    {
        /* Menu-bar editor created in the .ui file. */
        AssertPtrReturnVoid(m_pMenuBarEditor);
        {
            /* Configure editor: */
            m_pActionPool = UIActionPool::create(UIActionPoolType_Runtime);
            m_pMenuBarEditor->setActionPool(m_pActionPool);
            m_pMenuBarEditor->setMachineID(m_strMachineId);
        }

        /* Status-bar editor created in the .ui file. */
        AssertPtrReturnVoid(m_pStatusBarEditor);
        {
            /* Configure editor: */
            m_pStatusBarEditor->setMachineID(m_strMachineId);
        }
    }

    /* Apply language settings: */
    retranslateUi();
}

void UIMachineSettingsInterface::cleanup()
{
    /* Destroy personal action-pool: */
    UIActionPool::destroy(m_pActionPool);

    /* Cleanup cache: */
    delete m_pCache;
    m_pCache = 0;
}

