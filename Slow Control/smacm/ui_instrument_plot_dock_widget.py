# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'instrument_plot_dock_widget.ui'
#
# Created: Thu Jan 24 16:22:16 2013
#      by: PyQt4 UI code generator 4.9.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_InstrumentPlotDockWidget(object):
    def setupUi(self, InstrumentPlotDockWidget):
        InstrumentPlotDockWidget.setObjectName(_fromUtf8("InstrumentPlotDockWidget"))
        InstrumentPlotDockWidget.resize(300, 250)
        InstrumentPlotDockWidget.setFeatures(QtGui.QDockWidget.DockWidgetClosable|QtGui.QDockWidget.DockWidgetMovable)
        InstrumentPlotDockWidget.setAllowedAreas(QtCore.Qt.TopDockWidgetArea)
        self.dockWidgetContents = QtGui.QWidget()
        self.dockWidgetContents.setObjectName(_fromUtf8("dockWidgetContents"))
        self.verticalLayout = QtGui.QVBoxLayout(self.dockWidgetContents)
        self.verticalLayout.setMargin(0)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.frame = QtGui.QFrame(self.dockWidgetContents)
        self.frame.setMinimumSize(QtCore.QSize(150, 100))
        self.frame.setFrameShape(QtGui.QFrame.StyledPanel)
        self.frame.setFrameShadow(QtGui.QFrame.Raised)
        self.frame.setObjectName(_fromUtf8("frame"))
        self.verticalLayout_2 = QtGui.QVBoxLayout(self.frame)
        self.verticalLayout_2.setMargin(0)
        self.verticalLayout_2.setObjectName(_fromUtf8("verticalLayout_2"))
        self.plotWidget = DatePlotWidget(self.frame)
        self.plotWidget.setObjectName(_fromUtf8("plotWidget"))
        self.verticalLayout_2.addWidget(self.plotWidget)
        self.verticalLayout.addWidget(self.frame)
        InstrumentPlotDockWidget.setWidget(self.dockWidgetContents)

        self.retranslateUi(InstrumentPlotDockWidget)
        QtCore.QMetaObject.connectSlotsByName(InstrumentPlotDockWidget)

    def retranslateUi(self, InstrumentPlotDockWidget):
        InstrumentPlotDockWidget.setWindowTitle(QtGui.QApplication.translate("InstrumentPlotDockWidget", "DockWidget", None, QtGui.QApplication.UnicodeUTF8))

from DatePlotWidget import DatePlotWidget
