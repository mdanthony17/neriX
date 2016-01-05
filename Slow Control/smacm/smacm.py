from PyQt4 import QtCore, QtGui
from ui_main_window import Ui_MainWindow
from ui_preferences_dialog import Ui_PreferencesDialog
from ui_instrument_plot_dock_widget import Ui_InstrumentPlotDockWidget
import pyqtgraph
import numpy as np
import MySQLdb, threading

pyqtgraph.setConfigOption('background', 'w')
pyqtgraph.setConfigOption('foreground', 'k')

class DatabaseController():
	def __init__(self, hostname, username, password, database):
		self.hostname = hostname
		self.username = username
		self.password = password
		self.database = database

		# connect to the smac database
		connection = MySQLdb.connect(self.hostname, self.username, self.password, self.database)
		cursor = connection.cursor()

		# get instrument names
		cursor.execute('SHOW TABLES;')
		rows = cursor.fetchall()

		self._instrument_names = map(lambda p: p[0], rows)

		# get channel names
		self._channel_names = {}
		for instrument_name in self._instrument_names:
			cursor.execute('SHOW COLUMNS FROM %s;' % instrument_name)
			rows = cursor.fetchall()

			self._channel_names[instrument_name] = (map(lambda p: p[0], rows))[1:]

		# create and fill data rings for all instrument channels
		self.lookback_size = 200
		self.instrument_data_rings_lock = threading.Lock()
		self.instrument_data_rings_lock.acquire()
		self.instrument_data_rings = {}
		for instrument_name in self._instrument_names:
			self.instrument_data_rings[instrument_name] = {}
			print instrument_name
			for channel_name in self._channel_names[instrument_name]:
				unixtime_data_ring = DataRing(self.lookback_size)
				process_value_data_ring = DataRing(self.lookback_size)

				select_query = 'SELECT * FROM (SELECT %s_id,%s FROM %s ORDER BY %s_id DESC LIMIT %d) AS `table` ORDER BY %s_id;' % (instrument_name, channel_name, instrument_name, instrument_name, self.lookback_size, instrument_name)
				cursor.execute(select_query)
				rows = cursor.fetchall()

				unixtime_data_ring.append([row[0] for row in rows if row[1] != None])
				process_value_data_ring.append([row[1] for row in rows if row[1] != None])

				self.instrument_data_rings[instrument_name][channel_name] = [unixtime_data_ring, process_value_data_ring]
		self.instrument_data_rings_lock.release()

		cursor.close()
		connection.close()

		database_sync_interval = 30
		self.database_sync_thread = Repeater(self.update_instrument_data_rings, database_sync_interval)
		self.database_sync_thread.start()
	
	def instrument_names(self):
		return self._instrument_names

	def channel_names(self, instrument_name):
		return self._channel_names[instrument_name]

	def update_instrument_data_rings(self):
		print '>update_instrument_data_rings()'

		try:
			connection = MySQLdb.connect(self.hostname, self.username, self.password, self.database)
		except:
			print 'unable to connect to mysql server'
			return

		cursor = connection.cursor()

		for instrument_name in self._instrument_names:
#            print instrument_name
			for channel_name in self._channel_names[instrument_name]:
#                print channel_name

				last_reading = self.last_reading(instrument_name, channel_name)

				if last_reading:
					select_query = 'SELECT %s_id,%s FROM %s WHERE %s_id > %d ORDER BY %s_id LIMIT %d;' % (instrument_name, channel_name, instrument_name, instrument_name, last_reading[0], instrument_name, self.lookback_size)
					cursor.execute(select_query)
					rows = cursor.fetchall()

					self.instrument_data_rings_lock.acquire()
					self.instrument_data_rings[instrument_name][channel_name][0].append([row[0] for row in rows if row[1] != None])
					self.instrument_data_rings[instrument_name][channel_name][1].append([row[1] for row in rows if row[1] != None])
					self.instrument_data_rings_lock.release()

		cursor.close()
		connection.close()

	def readings(self, instrument_name, channel_name):
		self.instrument_data_rings_lock.acquire()
		unixtimes = self.instrument_data_rings[instrument_name][channel_name][0].data()
		process_values = self.instrument_data_rings[instrument_name][channel_name][1].data()
		self.instrument_data_rings_lock.release()

		return (unixtimes, process_values)

	def last_reading(self, instrument_name, channel_name):
		try:
			last_reading = tuple(map(lambda p: p[-1], self.readings(instrument_name, channel_name)))
		except IndexError:
			last_reading = None

		return last_reading

	def stop_database_sync(self):
		self.database_sync_thread.stop()

class Repeater(threading.Thread):
	def __init__(self, function, repeat_interval):
		threading.Thread.__init__(self)
		self.function = function
		self.repeat_interval = repeat_interval
		self.done = threading.Event()
	
	def run(self):
		if self.repeat_interval:
			while not self.done.is_set():
				self.function()
				self.done.wait(self.repeat_interval)
	
	def stop(self):
		self.done.set()

class DataRing():
	def __init__(self, capacity):
		self.array = np.zeros(2*capacity)
		self.capacity = capacity
		self.count = 0
		self.head = 0
		self.tail = 0

	def append(self, a):
		a = np.asarray(a)

		if a.size > self.capacity:
			# only append the last elements
			return

		if self.head+a.size <= self.capacity:
			self.array[self.head:self.head+a.size] = a[:]
			self.array[self.head+self.capacity:self.head+self.capacity+a.size] = a[:]
		else:
			first_size = self.capacity-self.head
			last_size = a.size-first_size
			self.array[self.head:self.head+first_size] = a[:first_size]
			self.array[0:last_size] = a[first_size:]
			self.array[self.head+self.capacity:self.head+self.capacity+first_size] = a[:first_size]
			self.array[self.capacity:self.capacity+last_size] = a[first_size:]

		self.count += a.size
		self.head = (self.head+a.size) % self.capacity

		if self.count > self.capacity:
			self.tail = self.head
			self.count = self.capacity

	def data(self):
		return self.array[self.tail:self.tail+self.count]

class InstrumentStatusNode():
	class Type:
		Instrument, Channel = range(2)

	def __init__(self, node_type=None, name='', checked=True, value='', parent=None):
		self.node_type = node_type
		self.columns = []
		self.columns.append(QtCore.QString(name))
		self.columns.append(checked)
		self.columns.append(QtCore.QString(value))

		self.parent = parent
		if parent:
			self.parent.appendChild(self)
		self.children = []

	def setValue(self, value):
		self.columns[2] = QtCore.QString(value)

	def appendChild(self, child):
		self.children.append(child)

	def child(self, row):
		return self.children[row]

	def childCount(self):
		return len(self.children)

	def columnCount(self):
		return len(self.columns)

	def data(self, column):
		if column == 2 and self.node_type == InstrumentStatusNode.Type.Instrument:
			return QtCore.QVariant()

		return QtCore.QVariant(self.columns[column])

	def setData(self, column, value):
		if column != 1:
			return False

		self.columns[1] = value

		return True

	def row(self):
		if self.parent:
			return self.parent.children.index(self)

		return 0

	def parent(self):
		return self.parent

class InstrumentStatusModel(QtCore.QAbstractItemModel):
	def __init__(self, database_controller):
		QtCore.QAbstractItemModel.__init__(self)

		self.database_controller = database_controller

		self.timer = QtCore.QTimer(self)
		model_refresh_interval = 2000

		self.connect(self.timer, QtCore.SIGNAL("timeout()"), self.update)

		self.root = InstrumentStatusNode()
		self.headers = map(QtCore.QString, ['Name', 'Display', 'Value'])
#        self.headers = ['Name']

		# get instrument and channel names from the database controller
		self.instrument_names = self.database_controller.instrument_names()
		
		self.channel_names = {}
		for instrument_name in self.instrument_names:
			self.channel_names[instrument_name] = self.database_controller.channel_names(instrument_name)

		# fill model with instruments
		self.channel_nodes = {}
		for instrument_name in self.instrument_names:
			instrument_node = InstrumentStatusNode(InstrumentStatusNode.Type.Instrument, instrument_name, True, '', self.root)

			self.channel_nodes[instrument_name] = {}
			for channel_name in self.channel_names[instrument_name]:
				last_reading = self.database_controller.last_reading(instrument_name, channel_name)
				if last_reading:
					channel_node = InstrumentStatusNode(InstrumentStatusNode.Type.Channel, channel_name, True, '%f' % last_reading[1], instrument_node)
				else:
					channel_node = InstrumentStatusNode(InstrumentStatusNode.Type.Channel, channel_name, True, '', instrument_node)

				self.channel_nodes[instrument_name][channel_name] = channel_node

		self.timer.start(model_refresh_interval)

	def update(self):
#        print 'update()'

		for instrument_name in self.instrument_names:
			for channel_name in self.channel_names[instrument_name]:
				last_reading = self.database_controller.last_reading(instrument_name, channel_name)
				if last_reading:
					self.channel_nodes[instrument_name][channel_name].setValue('%f' % last_reading[1])
		
		top_left = self.createIndex(0, 2, self.root)
		bottom_right = self.createIndex(self.root.childCount()-1, 2, self.root)
		self.emit(QtCore.SIGNAL('dataChanged(QModelIndex,QModelIndex)'), top_left, bottom_right)
	
	def data(self, index, role):
		if not index.isValid():
			return QtCore.QVariant()

		if index.column() == 1:
			if role == QtCore.Qt.CheckStateRole:
				return QtCore.Qt.Checked if index.internalPointer().data(index.column()).toBool() else QtCore.Qt.Unchecked
			if role == QtCore.Qt.TextAlignmentRole:
				return QtCore.Qt.AlignRight
			if role == QtCore.Qt.DisplayRole:
				return QtCore.QVariant()

		if role != QtCore.Qt.DisplayRole:
			return QtCore.QVariant()

		return index.internalPointer().data(index.column())

	def setData(self, index, value, role = QtCore.Qt.EditRole):
		if not index.isValid():
			return False

		if index.column() != 1 or role != QtCore.Qt.EditRole:
			return False

		result = index.internalPointer().setData(index.column(), value)
		if result:
			self.emit(QtCore.SIGNAL('dataChanged'), index, index)

		return result

	def flags(self, index):
		if not index.isValid():
			return QtCore.Qt.ItemFlags(0)
		
		if index.column() == 1:
			return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable | QtCore.Qt.ItemIsEditable | QtCore.Qt.ItemIsUserCheckable
		else:
			return QtCore.Qt.ItemIsEnabled | QtCore.Qt.ItemIsSelectable

	def headerData(self, section, orientation, role):
		if orientation == QtCore.Qt.Horizontal and role == QtCore.Qt.DisplayRole:
			return self.headers[section]

		return QtCore.QVariant()

	def index(self, row, column, parent):
		if not self.hasIndex(row, column, parent):
			return QtCore.QModelIndex()

		parent_node = parent.internalPointer() if parent.isValid() else self.root

		if parent_node.child(row):
			return self.createIndex(row, column, parent_node.child(row))
		else:
			return QtCore.QModelIndex()

	def parent(self, index):
		if not index.isValid():
			return QtCore.QModelIndex()

		parent_node = index.internalPointer().parent

		if not parent_node or parent_node == self.root:
			return QtCore.QModelIndex()

		return self.createIndex(parent_node.row(), 0, parent_node)

	def rowCount(self, parent):
		if parent.column() > 0:
			return 0

		parent_node = parent.internalPointer() if parent.isValid() else self.root

		return parent_node.childCount()

	def columnCount(self, parent):
		if parent.isValid():
			return parent.internalPointer().columnCount()
		else:
			return self.root.columnCount()

class PreferencesDialog(QtGui.QDialog):
	def __init__(self):
		QtGui.QDialog.__init__(self)
		self.ui = Ui_PreferencesDialog()
		self.ui.setupUi(self)

class InstrumentPlotDockWidget(QtGui.QDockWidget):
	def __init__(self, title, plot_title, axis_title='', units=''):
		QtGui.QDockWidget.__init__(self)
		self.ui = Ui_InstrumentPlotDockWidget()
		self.ui.setupUi(self)

		self.ui.plotWidget.getPlotItem().setTitle(plot_title)
		self.ui.plotWidget.getPlotItem().setLabel(axis='left', text=axis_title, units=units)
		self.ui.plotWidget.showGrid(x=True,y=True, alpha=0.2)

		self.setWindowTitle(title)

		self.plotDataItem = None

class MainWindow(QtGui.QMainWindow):
	def __init__(self, database_controller):
		QtGui.QMainWindow.__init__(self)
		self.setCentralWidget(None)
		self.setWindowTitle('smacm')
		self.setCorner(QtCore.Qt.TopRightCorner, QtCore.Qt.RightDockWidgetArea)
		self.setCorner(QtCore.Qt.BottomRightCorner, QtCore.Qt.RightDockWidgetArea)

		self.ui = Ui_MainWindow()
		self.ui.setupUi(self)

		self.database_controller = database_controller

		self.instrument_plots_refresh_timer = QtCore.QTimer()

		instrument_channel_config = {}
		instrument_channel_config['adc0{P_chamber}'] = {'axis_title':'Pressure', 'units':'bar'}
		instrument_channel_config['adc0{T_top}'] = {'axis_title':'Temperature', 'units':'C'}
		instrument_channel_config['adc0{Q_circulation}'] = {'axis_title':'Flow', 'units':'SLPM'}
		instrument_channel_config['adc0{T_ambient}'] = {'axis_title':'Temperature', 'units':'C'}
		instrument_channel_config['apc0{P_load}'] = {'axis_title':'Power', 'units':'VA'}
		instrument_channel_config['apc0{Q_battery}'] = {'axis_title':'Charge', 'units':'%'}
		instrument_channel_config['apc0{B_ac_online}'] = {'axis_title':'State', 'units':''}
		instrument_channel_config['apc0{B_battery_online}'] = {'axis_title':'State', 'units':''}
		instrument_channel_config['apc0{B_low_battery}'] = {'axis_title':'State', 'units':''}
		instrument_channel_config['lakeshore0{Q_heater}'] = {'axis_title':'Power', 'units':'%'}
		instrument_channel_config['lakeshore0{T_ptr}'] = {'axis_title':'Temperature', 'units':'C'}
		instrument_channel_config['lakeshore0{T_coldfinger}'] = {'axis_title':'Temperature', 'units':'C'}
		instrument_channel_config['pfeiffer0{P_insulation}'] = {'axis_title':'Pressure', 'units':'Torr'}
		instrument_channel_config['pfeiffer0{P_chamber}'] = {'axis_title':'Pressure', 'units':'Torr'}
		instrument_channel_config['uti0{C_lm1}'] = {'axis_title':'Capacitance', 'units':'pF'}
		instrument_channel_config['uti0{C_lm2}'] = {'axis_title':'Capacitance', 'units':'pF'}

		# get instrument and channel names from the database controller
		self.instrument_names = database_controller.instrument_names()
#        self.instrument_names = self.instrument_names[2:3]

		self.channel_names = {}
		for instrument_name in self.instrument_names:
			self.channel_names[instrument_name] = database_controller.channel_names(instrument_name)

		# create an instrument status model
		self.instrument_status_model = InstrumentStatusModel(database_controller)

		self.ui.treeView.setModel(self.instrument_status_model)
		self.ui.treeView.expandAll()
		for column in range(self.instrument_status_model.columnCount(QtCore.QModelIndex())):
			self.ui.treeView.resizeColumnToContents(column)

		self.intrument_plot_widgets = {}
		for instrument_name in self.instrument_names:
			for channel_name in self.channel_names[instrument_name]:
				instrument_plot_dock_widget_title = '%s{%s}' % (instrument_name, channel_name)
				if instrument_plot_dock_widget_title in instrument_channel_config:
					axis_title = instrument_channel_config[instrument_plot_dock_widget_title]['axis_title']
					units = instrument_channel_config[instrument_plot_dock_widget_title]['units']
					instrument_plot_dock_widget = InstrumentPlotDockWidget(instrument_plot_dock_widget_title, channel_name, axis_title, units)
				else:
					instrument_plot_dock_widget = InstrumentPlotDockWidget(instrument_plot_dock_widget_title, channel_name)
					instrument_plot_dock_widget.setObjectName(instrument_plot_dock_widget_title)
				self.addDockWidget(QtCore.Qt.TopDockWidgetArea, instrument_plot_dock_widget)
				self.intrument_plot_widgets[instrument_plot_dock_widget_title] = instrument_plot_dock_widget

		self.update_instrument_plots()

		self.connect(self.instrument_plots_refresh_timer, QtCore.SIGNAL("timeout()"), self.update_instrument_plots)

		self.instrument_plots_refresh_timer.start(10000) # ms

	def saveLayout(self):
		print 'saveLayout()'

		filename = QtGui.QFileDialog.getSaveFileName(self, self.tr('Save layout'))

		if filename.isEmpty():
			return

		layout_file = QtCore.QFile(filename)

		if not layout_file.open(QtCore.QFile.WriteOnly):
			msg = self.tr('Failed to open %1\n%2').arg(filename).arg(layout_file.errorString())
			QtGui.MessageBox.warning(self, self.tr('Error'), msg)
			return

		geometry_data = self.saveGeometry()
		layout_data = self.saveState()

		result = layout_file.putChar(chr(geometry_data.size()))
		if result:
			result = (layout_file.write(geometry_data) == geometry_data.size())
			print geometry_data.size()
			print repr(geometry_data)
		if result:
			result = (layout_file.write(layout_data) == layout_data.size())
			print layout_data.size()
			print repr(layout_data)

		if not result:
			msg = self.tr('Error writing to %1\n%2').arg(filename).arg(layout_file.errorString())
			QtGui.QMessageBox.warning(self, self.tr('Error'), msg)
			return
	
	def loadLayout(self):
		print 'loadLayout()'

		filename = QtGui.QFileDialog.getOpenFileName(self, self.tr('Load layout'))

		if filename.isEmpty():
			return

		layout_file = QtCore.QFile(filename)
		if not layout_file.open(QtCore.QFile.ReadOnly):
			msg = self.tr('Failed to open %1\n%2').arg(filename).arg(layout_file.errorString())
			QtGui.QMessageBox.warning(self, self.tr('Error'), msg)
			return

		result, geometry_size = layout_file.getChar()
		print ord(geometry_size)
		if result:
			geometry_data = QtCore.QByteArray(layout_file.read(ord(geometry_size)))
			result = (geometry_data.size() == ord(geometry_size))
			print repr(geometry_data)
		
		if result:
			layout_data = QtCore.QByteArray(layout_file.readAll())
			result = (layout_data.size() > 0)
			print layout_data.size()
			print repr(layout_data)

		if result:
			result = self.restoreGeometry(geometry_data)
		if result:
			result = self.restoreState(layout_data)

		if not result:
			msg = self.tr('Error reading %1').arg(filename)
			QtGui.QMessageBox.warning(self, self.tr('Error'), msg)
			return

	def quit(self):
		self.database_controller.stop_database_sync()
		self.close()

	def preferences(self):
		self.preferences_dialog = PreferencesDialog()
		self.preferences_dialog.show()
	
	def update_instrument_plots(self):
		print '>update_instrument_plots()'
		for instrument_name in self.instrument_names:
			for channel_name in self.channel_names[instrument_name]:
				instrument_plot_dock_widget_title = '%s{%s}' % (instrument_name, channel_name)
				instrument_plot_dock_widget = self.intrument_plot_widgets[instrument_plot_dock_widget_title]

				readings = self.database_controller.readings(instrument_name, channel_name)
				if readings[0].size:
					if not instrument_plot_dock_widget.plotDataItem:
						instrument_plot_dock_widget.plotDataItem = pyqtgraph.PlotDataItem(readings[0], readings[1], pen=pyqtgraph.mkPen('r', width=1.5))
						instrument_plot_dock_widget.ui.plotWidget.addItem(instrument_plot_dock_widget.plotDataItem)
					else:
						instrument_plot_dock_widget.plotDataItem.setData(readings[0], readings[1])

#        self.connect(self.ui.plotPushButton, QtCore.SIGNAL("clicked()"), self.plotButtonClicked)

#        self.plotDataItem = None

#    def plotButtonClicked(self):
#        if not self.plotDataItem:
#            self.plotDataItem = pyqtgraph.PlotDataItem(np.random.normal(size=200))
#            self.ui.plotWidget.addItem(self.plotDataItem)
#        else:
#            self.plotDataItem.setData(np.random.normal(size=200))

def smacm_start():

	# parse options, etc

	# create a database controller
	database_controller = DatabaseController('astro18.astronevis.columbia.edu', 'smac_reader', 'wq12op09', 'smac')

	app = QtGui.QApplication(sys.argv)
	main_window = MainWindow(database_controller)
	main_window.show()

	QtCore.QObject.connect(app, QtCore.SIGNAL("lastWindowClosed()"), main_window.quit)

	return app.exec_()

if __name__ == "__main__":
	import sys

	sys.exit(smacm_start())

