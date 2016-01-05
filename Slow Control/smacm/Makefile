all: $(wildcard ui_*.py)

ui_main_window.py: main_window.ui
	pyuic4 main_window.ui -o ui_main_window.py

ui_preferences_dialog.py: preferences_dialog.ui
	pyuic4 preferences_dialog.ui -o ui_preferences_dialog.py

ui_instrument_plot_dock_widget.py: instrument_plot_dock_widget.ui
	pyuic4 instrument_plot_dock_widget.ui -o ui_instrument_plot_dock_widget.py
