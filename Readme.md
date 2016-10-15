GroupedTreeWidget is a custom control QT widget.

What it does?
- It is a tree widget that groups the data based on the unique values in each column
- Sorts the data (in grouped mode too)

What is unique in it?
- QT does not provide a native control to group data after it is displayed
- No hard data value
- Data can be updated at run-time

How to use it?
In .ui file
- Promote the QTreeWidget to GroupedTreeWidget
- Leave the first column blank and enter the column headers

In code
- Add data to the control using void OnNewDataAdded(QList<QTreeWidgetItem*> ptrlistItem)
- Remember that the first column is empty.

What are the APIs?
- Functions:
	- void ResetTree();
		Clears the contents of the tree
	- void SetCheckable(const bool bIsCheckable = false);
		To make items checkable
	- bool IsCheckable() const;
		To know if item is Checkable
	- void GroupByValues(const int iColumnNumber);
		Groups the data as per the unique values in the column selected
- Slots:
	- void ShowGroupingMenu(QPoint objPoint);
		Function to show menu when header of tree widget is right clicked
	- void OnNewDataAdded(QList<QTreeWidgetItem*> ptrlistItem);
		Function called when data is Added
	- void OnDataUpdated(QTreeWidgetItem *ptrItem);
		Function called when data is updated
