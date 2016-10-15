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

#ifndef GROUPEDTREEWIDGET_HPP
#define GROUPEDTREEWIDGET_HPP

#include <QTreeWidget>
#include <QMenu>
#include <QHeaderView>
#include <QDebug>

class GroupedTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit GroupedTreeWidget(QWidget *parent = 0);
    ~GroupedTreeWidget();
    void ResetTree();

    void SetCheckable(const bool bIsCheckable = false);
    bool IsCheckable() const;

    void GroupByValues(const int iColumnNumber);
private:
    QList<QVariant> Data(QTreeWidgetItem* ptrItem);

    /**
     * @brief mb_isSelectable denotes if the tree widget items can be checked
     */
    bool mb_isCheckable;
    void SetItemCheckableFlag(QTreeWidgetItem* ptrItem);

protected:

    /**
     * @brief mobjlist_items holds all the tree widget items of the tree
     */
    QList<QTreeWidgetItem *> mobjlist_items;

    /**
     * @brief mszlistlist_data holds all the data of the items of the tree
     */
    QList<QList<QVariant>> mszlistlist_data;

    int mi_column, mi_columnGrouped;
    bool mb_isGrouped;
    long ml_maxSelectedCount;
    bool mb_isUserMaxCount;

signals:

public slots:
    void ShowGroupingMenu(QPoint objPoint);
    void OnNewDataAdded(QList<QTreeWidgetItem*> ptrlistItem);
    void OnDataUpdated(QTreeWidgetItem *ptrItem);

private slots:
    void OnGroupByClicked();
};

#endif // GROUPEDTREEWIDGET_HPP
