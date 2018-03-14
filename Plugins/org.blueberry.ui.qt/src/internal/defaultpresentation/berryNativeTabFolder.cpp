/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "berryNativeTabFolder.h"
#include "berryNativeTabItem.h"
#include "berryQCTabBar.h"

#include <internal/berryQtControlWidget.h>
#include <berryWorkbenchPlugin.h>

#include <berryIQtStyleManager.h>
#include <berryShell.h>
#include <berryConstants.h>
#include <berryPlatform.h>
#include <berryPlatformUI.h>

#include <berryLog.h>

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

namespace berry
{

AbstractTabItem* NativeTabFolder::GetTab(int index)
{
  return tabControl->getTab(index);
}

void NativeTabFolder::TabSelectionChanged(int index)
{
  this->FireEvent(TabFolderEvent::EVENT_TAB_SELECTED, tabControl->getTab(index));
}

void NativeTabFolder::DragStarted(const QPoint& location)
{
  this->HandleDragStarted(location);
}

void NativeTabFolder::ViewFormDestroyed(QObject*)
{
  viewForm = nullptr;
  content = nullptr;
}

NativeTabFolder::NativeTabFolder(QWidget* parent)
: QObject(parent)
{
  content = nullptr;
  viewForm = new QtControlWidget(parent, nullptr);
  viewForm->setObjectName("ViewForm");
  viewForm->installEventFilter(this);
  auto   layout = new QVBoxLayout(viewForm);
  layout->setContentsMargins(0,0,0,0);
  layout->setSpacing(0);
  viewForm->setLayout(layout);

  connect(viewForm, SIGNAL(destroyed(QObject*)), this, SLOT(ViewFormDestroyed(QObject*)));

  auto   topControls = new QWidget(viewForm);
  //topControls->setMinimumSize(0, 24);
  //topControls->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout->addWidget(topControls);
  auto   topLayout = new QHBoxLayout(topControls);
  topLayout->setContentsMargins(0, 0, 0, 0);
  topLayout->setSpacing(0);

  tabControl = new QCTabBar(topControls);
  tabControl->installEventFilter(this);
  //tabControl->setMinimumSize(0, 25);
  //tabControl->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  topLayout->addWidget(tabControl);

  auto   topRightControls = new QFrame(topControls);
  topRightControls->setObjectName("TabTopRightControls");
  //topRightControls->setMinimumSize(6, 25);
  //topRightControls->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  topLayout->addWidget(topRightControls);

  contentFrame = new QFrame(viewForm);
  contentFrame->setObjectName("ViewFormContentFrame");
  contentFrame->installEventFilter(this);
  contentFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  auto   contentFrameLayout = new QVBoxLayout(contentFrame);
  contentFrameLayout->setContentsMargins(0,0,0,0);
  contentFrameLayout->setSpacing(0);

  layout->addWidget(contentFrame);

  this->connect(tabControl, SIGNAL(currentChanged(int)), this,
      SLOT(TabSelectionChanged(int)));

  this->connect(tabControl, SIGNAL(dragStarted(const QPoint&)), this,
      SLOT(DragStarted(const QPoint&)));

  ctkServiceReference serviceRef = WorkbenchPlugin::GetDefault()->GetPluginContext()->getServiceReference<IQtStyleManager>();
  if (serviceRef)
  {
    skinManager = WorkbenchPlugin::GetDefault()->GetPluginContext()->getService<IQtStyleManager>(serviceRef);
  }
}

NativeTabFolder::~NativeTabFolder()
{
  if (!PlatformUI::GetWorkbench()->IsClosing())
  {
    BERRY_DEBUG << "Deleting viewForm";
    if (content != nullptr)
    {
      content->setParent(nullptr);
    }
    viewForm->deleteLater();
  }
}

bool NativeTabFolder::eventFilter(QObject* watched, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress)
    {
      QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
      this->HandleMousePress(mouseEvent->pos());
    }


  return QObject::eventFilter(watched, event);
}

void NativeTabFolder::SetActive(int activeState)
{
  AbstractTabFolder::SetActive(activeState);
  this->UpdateColors();
}

void NativeTabFolder::UpdateColors()
{
  viewForm->setStyleSheet(1 == this->GetActive()
    ? skinManager->GetActiveTabStylesheet()
    : skinManager->GetTabStylesheet());
}

void NativeTabFolder::CloseButtonClicked(AbstractTabItem* item)
{
  this->FireEvent(TabFolderEvent::EVENT_CLOSE, item);
}

QSize NativeTabFolder::ComputeSize(int  /*widthHint*/, int  /*heightHint*/)
{
  return QSize(50,50);
}

AbstractTabItem* NativeTabFolder::Add(int index, int flags)
{
  auto   item = new NativeTabItem(this, index, flags);
  return item;
}

void NativeTabFolder::Move(int from, int to)
{
  int tabCount = tabControl->count();
  if (to > tabCount) to = tabCount;
  tabControl->moveAbstractTab(from, to);
}

void NativeTabFolder::Layout(bool flushCache)
{
  AbstractTabFolder::Layout(flushCache);
}

QPoint NativeTabFolder::GetPaneMenuLocation()
{
  return AbstractTabFolder::GetPaneMenuLocation();
}

void NativeTabFolder::SetState(int state)
{
  AbstractTabFolder::SetState(state);
}

QRect NativeTabFolder::GetClientArea()
{
  if (content == nullptr)
  {
    return QRect();
  }

  return content->geometry();
}

QList<AbstractTabItem*> NativeTabFolder::GetItems()
{
  return tabControl->getTabs();
}

void NativeTabFolder::SetSelection(AbstractTabItem* toSelect)
{
  if (toSelect == nullptr)
  {
    return;
  }

  tabControl->setCurrentTab(toSelect);
}

void NativeTabFolder::SetSelectedInfo(const PartInfo&)
{
}

QRect NativeTabFolder::GetTabArea()
{
  return tabControl->geometry();
}

QWidget* NativeTabFolder::GetControl()
{
  return viewForm;
}

bool NativeTabFolder::IsOnBorder(const QPoint& )
{
  return false;
}

AbstractTabItem* NativeTabFolder::GetSelection()
{
  return tabControl->getCurrentTab();
}

QWidget* NativeTabFolder::GetContentParent()
{
  return contentFrame;
}

void NativeTabFolder::SetContent(QWidget* newContent)
{
  if (content != nullptr)
  {
    contentFrame->layout()->removeWidget(content);
    disconnect(content);
  }
  content = newContent;
  content->installEventFilter(this);
  contentFrame->layout()->addWidget(content);
}

QCTabBar* NativeTabFolder::GetTabFolder()
{
  return tabControl;
}

void NativeTabFolder::SetSelectedTitle(const QString&)
{
}

void NativeTabFolder::SetSelectedImage(const QPixmap*)
{
}

AbstractTabItem* NativeTabFolder::GetItem(const QPoint& toFind)
{
  QPoint localPoint = tabControl->mapFromGlobal(toFind);
  int index = tabControl->tabAt(localPoint);
  if (index < 0)
    return nullptr;
  return tabControl->getTab(index);
}

void NativeTabFolder::EnablePaneMenu(bool)
{
}

}
