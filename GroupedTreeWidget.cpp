/*
 * Copyright (C) 2016 Abhiraj Deshpande
 * Github: http://www.github/ahdeshpande
 *
 * This file is part of GroupedTreeWidget.
 * GroupedTreeWidget is free software (QT control): you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GroupedTreeWidget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GroupedTreeWidget.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "GroupedTreeWidget.hpp"

/**
 * @brief GroupedTreeWidget::GroupedTreeWidget : Constructor of the class
 * @param parent : Parent
 */
GroupedTreeWidget::GroupedTreeWidget(QWidget *parent) :
    QTreeWidget(parent),
    mb_isUserMaxCount(false),
    ml_maxSelectedCount(this->topLevelItemCount())
{
    mobjlist_items = QList<QTreeWidgetItem *>();
    mszlistlist_data = QList<QList<QVariant>>();
    mb_isCheckable = false;

    this->header()->setContextMenuPolicy(Qt::CustomContextMenu);

    this->hideColumn(0);

    // Set Row Selection
    this->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Connect to open Context Menu
    connect(this->header(), &QHeaderView::customContextMenuRequested, this, &GroupedTreeWidget::ShowGroupingMenu, Qt::UniqueConnection);

}

/**
 * @brief GroupedTreeWidget::~GroupedTreeWidget : Destructor
 */
GroupedTreeWidget::~GroupedTreeWidget()
{
}

/**
 * @brief GroupedTreeWidget::Data : Retrieves all the data from the tree widget items and returns it in QVariant form
 * @param ptrItem
 * @return
 */
QList<QVariant> GroupedTreeWidget::Data(QTreeWidgetItem* ptrItem)
{
    QList<QVariant> szlistData = QList<QVariant>();

    // First column is always left blank
    szlistData.append("");

    // Data from all columns
    for(int j=1; j<this->columnCount(); j++)
    {
        szlistData.append(ptrItem->data(j, Qt::DisplayRole));
    }
    return szlistData;
}

/**
 * @brief GroupedTreeWidget::SetItemCheckableFlag : Sets the item as checkable/uncheckable
 * @param ptrItem
 */
void GroupedTreeWidget::SetItemCheckableFlag(QTreeWidgetItem *ptrItem)
{
    // If tree has to have selectable items, make new items selectable
    if(true == IsCheckable())
    {
        ptrItem->setFlags(ptrItem->flags() | Qt::ItemIsUserCheckable);
        ptrItem->setCheckState(1, ptrItem->checkState(1));
    }
    else
    {
        ptrItem->setFlags(ptrItem->flags() & ~(Qt::ItemIsUserCheckable));
    }
}

/**
 * @brief GroupedTreeWidget::GroupByValues : Groups the data as per the unique values in the column selected
 * @param iColumnNumber : Column number
 */
void GroupedTreeWidget::GroupByValues(const int iColumnNumber)
{
    int iTopLevelCount = this->topLevelItemCount();

    // Clear tree Data
    for (int i = iTopLevelCount-1 ; i >= 0 ; i--)
    {
        qDeleteAll(this->topLevelItem(i)->takeChildren());
        this->takeTopLevelItem(i);
    }

    // Show all columns
    for(int i=0; i<this->columnCount(); i++)
    {
        this->showColumn(i);
    }

    // If grouping is on column 0, then ungroup the tree widget
    if(iColumnNumber == 0)
    {
        //        qDebug() << "Ungroup Me ";

        mb_isGrouped = false;

        QList<QTreeWidgetItem*> objlistTemp = QList<QTreeWidgetItem*>();
        QList<QList<QVariant>> szlistTemp = QList<QList<QVariant>>(); // Temp List

        // Retrieve all the data from the member variable list and form the tree again
        for(int j=0; j<mszlistlist_data.count(); j++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(this);
            item->setData(0, Qt::DisplayRole, ""); // Unique value

            QList<QVariant> szData = QList<QVariant>(mszlistlist_data.at(j)); //Data
            for(int k=1; k<szData.count(); k++)
            {
                item->setData(k, Qt::DisplayRole, szData.at(k));
            }
            objlistTemp.append(item);
            szlistTemp.append(mszlistlist_data.at(j));

            SetItemCheckableFlag(item);
        }

        // Update Tree widget item list and Data list
        mobjlist_items = QList<QTreeWidgetItem*>(objlistTemp);
        mszlistlist_data = QList<QList<QVariant>>(szlistTemp);

        // Since the tree is ungrouped, all items are added as top level items
        this->addTopLevelItems(mobjlist_items);

        // Hide 0 column
        this->hideColumn(0);

        // Sorts the data as per column 1
        this->sortByColumn(1);

        // Current item is set to 0
        this->setCurrentItem(this->topLevelItem(0));
    }
    // else group the tree as per the column
    else
    {
        //        qDebug() << "Group Me";

        mobjlist_items = QList<QTreeWidgetItem*>();

        mb_isGrouped = true;

        // Read the unique values from the column to by grouped
        QList<QVariant> szlistUnique = QList<QVariant>(); // List of unique values
        for(int i=0; i<mszlistlist_data.count(); i++)
        {
            if(!(szlistUnique.contains(mszlistlist_data.at(i).at(iColumnNumber))))
            {
                szlistUnique.append(mszlistlist_data.at(i).at(iColumnNumber));
            }
            else
            {
                // Nothing...
            }
        }

        // Hide column on which grouping is applied
        this->hideColumn(iColumnNumber);

        // Set header as column selected
        this->headerItem()->setText(0, this->headerItem()->text(iColumnNumber));

        // Add Items back in tree
        // From the tree data,
        // Select each item having the unique vales of the grouped column one by one
        // And add to the list under one toplevel item

        QList<QList<QVariant>> szlistTemp = QList<QList<QVariant>>(); // Temp List

        for(int i=0; i<szlistUnique.count(); i++)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem(this);
            item->setData(0, Qt::DisplayRole, szlistUnique.at(i)); // Unique value at column 0

            for(int j=0; j<mszlistlist_data.count(); j++)
            {
                if(mszlistlist_data.at(j).at(iColumnNumber) == (szlistUnique.at(i))) // Check if the Value matches
                {
                    QTreeWidgetItem* ptrChild = new QTreeWidgetItem();

                    QList<QVariant> szData = QList<QVariant>(mszlistlist_data.at(j)); //Data
                    for(int k=1; k<szData.count(); k++)
                    {
                        ptrChild->setData(k, Qt::DisplayRole, szData.at(k));
                    }
                    item->addChild(ptrChild);

                    mobjlist_items.append(ptrChild);

                    szlistTemp.append(Data(ptrChild));

                    SetItemCheckableFlag(item);
                }
                else
                {
                    // Nothing... Check next value
                }
            }

            // Display data as
            // <unique_value> (count)
            item->setData(0, Qt::DisplayRole, szlistUnique.at(i).toString() + " (" + QString::number(item->childCount()) + ")");
            this->addTopLevelItem(item);
        }

        // Update String list
        mszlistlist_data = QList<QList<QVariant>>(szlistTemp);

        // Expand all items
        this->expandAll();
        this->sortByColumn(0);
        this->setCurrentItem(this->topLevelItem(0)->child(0));
    }

    // Resize column to contents
    for(int i=0; i<columnCount(); i++)
    {
        resizeColumnToContents(i);
    }
}

/**
 * @brief GroupedTreeWidget::ResetTree : Clears the contents
 */
void GroupedTreeWidget::ResetTree(){

    // Reset Grouping
    GroupByValues(0);

    // Delete all top level items
    int iTopLevelCount = this->topLevelItemCount();
    for (int i = iTopLevelCount-1 ; i >= 0 ; i--)
    {
        qDeleteAll(this->topLevelItem(i)->takeChildren());
        this->takeTopLevelItem(i);
    }

    // Initialize lists
    mobjlist_items = QList<QTreeWidgetItem*>();
    mszlistlist_data = QList<QList<QVariant>>();
}

/**
 * @brief GroupedTreeWidget::SetCheckable  : To make items checkable
 * @param bIsCheckable
 */
void GroupedTreeWidget::SetCheckable(const bool bIsCheckable)
{
    mb_isCheckable = bIsCheckable;
}

/**
 * @brief GroupedTreeWidget::IsCheckable : To know if item is Checkable
 * @return
 */
bool GroupedTreeWidget::IsCheckable() const
{
    return mb_isCheckable;
}

/**
 * @brief GroupedTreeWidget::ShowGroupingMenu : Menu that is shown when header of tree widget is right clicked
 * @param objPoint : Point (x,y) of click
 */
void GroupedTreeWidget::ShowGroupingMenu(QPoint objPoint)
{
    // if data is present and items are not selectable then show menu, else nothing
    if(this->topLevelItemCount() != NULL && false == IsCheckable())
    {
        // Get column to be grouped on
        mi_column = this->columnAt(objPoint.x());

        // Show Menu
        QMenu *ptrMenu = new QMenu(this); // Context Menu to be displayed

        QAction *ptrAction;
        QAction *ptrExpandAction;
        QAction *ptrCollapseAction;
        // if 0 then show option to clear grouping
        if(mi_column == 0)
        {
            ptrAction = new QAction("Clear Grouping", this);
            ptrExpandAction = new QAction("Expand All", this);
            connect(ptrExpandAction, &QAction::triggered, this, &GroupedTreeWidget::expandAll, Qt::UniqueConnection);
            ptrCollapseAction = new QAction("Collapse All", this);
            connect(ptrCollapseAction, &QAction::triggered, this, &GroupedTreeWidget::collapseAll, Qt::UniqueConnection);

            ptrMenu->addAction(ptrExpandAction);
            ptrMenu->insertAction(ptrExpandAction, ptrAction);
            ptrMenu->addAction(ptrCollapseAction);
        }
        else
        {
            ptrAction = new QAction("Group By Values", this);
            ptrMenu->addAction(ptrAction);
        }
        connect(ptrAction, &QAction::triggered, this, &GroupedTreeWidget::OnGroupByClicked, Qt::UniqueConnection);
        ptrMenu->popup(QCursor::pos()); // Position the menu at the right-clicked position
    }
    else
    {
        // Nothing.. Dont show menu
    }

}

/**
 * @brief GroupedTreeWidget::OnGroupByClicked : SLOT to handle Context menu click
 */
void GroupedTreeWidget::OnGroupByClicked()
{
    mi_columnGrouped = mi_column;
    GroupByValues(mi_columnGrouped);
}

/**
 * @brief GroupedTreeWidget::OnNewDataAdded : Function called when data is Added
 * @param ptrlistItem : List of data
 */
void GroupedTreeWidget::OnNewDataAdded(QList<QTreeWidgetItem *> ptrlistItem)
{
    mobjlist_items.append(ptrlistItem);

    // Converts data to String
    for(int i=0; i<ptrlistItem.count(); i++)
    {
        mszlistlist_data.append(Data(ptrlistItem.at(i)));
    }

    // Check if tree is grouped or not
    // If yes, then regroup again
    if(mb_isGrouped == true && mi_columnGrouped != 0)
    {
        GroupByValues(mi_columnGrouped);
    }
    else
    {
        // Adds all elements to last
        for(int i=0; i<ptrlistItem.count(); i++)
        {
            SetItemCheckableFlag(ptrlistItem.at(i));

            this->insertTopLevelItem(this->topLevelItemCount(), ptrlistItem.at(i));
        }
    }

    // Resize column to contents
    for(int i=0; i<columnCount(); i++)
    {
        resizeColumnToContents(i);
    }
}

/**
 * @brief GroupedTreeWidget::OnDataUpdated : Function called when data is updated
 * @param ptrItem : Item
 */
void GroupedTreeWidget::OnDataUpdated(QTreeWidgetItem * ptrItem)
{
    // Find the index of item to be updated
    int iIndex = mobjlist_items.indexOf(ptrItem);

    qDebug() << "ptrItem " << ptrItem << " @ " << iIndex;

    // If item doesn't exist add to list
    if(iIndex == -1)
    {
        qDebug() << "I got a new value";
        OnNewDataAdded(QList<QTreeWidgetItem*>() << ptrItem);
    }
    else
    {
        qDebug() << "I got a update";

        mobjlist_items.replace(iIndex, ptrItem);

        QList<QVariant> szlist = QList<QVariant>(); // List of data
        szlist.append("");

        // Data from all columns
        for(int j=1; j<this->columnCount(); j++)
        {
            szlist.append(ptrItem->data(j, Qt::DisplayRole));
        }
        mszlistlist_data.replace(iIndex, szlist);
    }

    // Check if grouped or not
    if(mb_isGrouped == true && mi_columnGrouped != 0)
    {
        GroupByValues(mi_columnGrouped);
    }
    else
    {
        GroupByValues(0);
    }
}
