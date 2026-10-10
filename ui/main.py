import customtkinter as ctk
import os
import sys
import logging
from backend_client import BackendClient

from pages.dashboard import DashboardPage
from pages.normal_vehicles import NormalVehiclesPage
from pages.emergency_vehicles import EmergencyVehiclesPage
from pages.road_sensors import RoadSensorsPage
from pages.activity_log import ActivityLogPage
from pages.settings import SettingsPage

class App(ctk.CTk):
    def __init__(self):
        super().__init__()

        self.title("Smart Traffic Management System")
        self.geometry("1280x800")
        self.minsize(1000, 600)
        
        # Set dark theme
        ctk.set_appearance_mode("dark")
        ctk.set_default_color_theme("blue")

        # Layout
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(1, weight=1)

        # Initialize Application State
        self.backend = None
        self.activity_logs = [] # Memory store for activity logs
        self.pages = {} # Initialize empty pages dictionary first
        
        self.init_backend()

        # Sidebar
        self.sidebar_frame = ctk.CTkFrame(self, width=200, corner_radius=0)
        self.sidebar_frame.grid(row=0, column=0, rowspan=2, sticky="nsew")
        self.sidebar_frame.grid_rowconfigure(7, weight=1)
        
        self.logo_label = ctk.CTkLabel(self.sidebar_frame, text="Smart Traffic\nSystem", font=ctk.CTkFont(size=20, weight="bold"))
        self.logo_label.grid(row=0, column=0, padx=20, pady=(20, 30))

        # Sidebar Buttons
        self.nav_buttons = {}
        nav_items = [
            ("Dashboard", "dashboard"),
            ("Normal Vehicles", "normal_vehicles"),
            ("Emergency Vehicles", "emergency_vehicles"),
            ("Road Sensors", "road_sensors"),
            ("Activity Log", "activity_log"),
            ("Settings", "settings")
        ]

        for i, (text, page_name) in enumerate(nav_items, start=1):
            btn = ctk.CTkButton(self.sidebar_frame, text=text, anchor="w", 
                                fg_color="transparent", text_color=("gray10", "gray90"),
                                command=lambda p=page_name: self.show_page(p))
            btn.grid(row=i, column=0, padx=20, pady=5, sticky="ew")
            self.nav_buttons[page_name] = btn

        # Header Area
        self.header_frame = ctk.CTkFrame(self, corner_radius=0, fg_color="transparent")
        self.header_frame.grid(row=0, column=1, sticky="nsew", padx=20, pady=10)
        self.header_frame.grid_columnconfigure(0, weight=1)

        self.header_title = ctk.CTkLabel(self.header_frame, text="Dashboard", font=ctk.CTkFont(size=24, weight="bold"))
        self.header_title.grid(row=0, column=0, sticky="w")
        
        self.header_desc = ctk.CTkLabel(self.header_frame, text="Overview of traffic system", text_color="gray")
        self.header_desc.grid(row=1, column=0, sticky="w")

        self.conn_status_var = ctk.StringVar(value="Connecting...")
        self.conn_status_label = ctk.CTkLabel(self.header_frame, textvariable=self.conn_status_var)
        self.conn_status_label.grid(row=0, column=1, rowspan=2, sticky="e")
        self.update_connection_status()

        # Content Area
        self.content_frame = ctk.CTkFrame(self, corner_radius=10)
        self.content_frame.grid(row=1, column=1, sticky="nsew", padx=20, pady=(0, 20))

        # Pages
        self.pages = {}
        self.current_page = None

        # Setup pages
        self.setup_pages()
        
        # Show initial page
        self.show_page("dashboard")

    def init_backend(self):
        if sys.platform == "win32":
            exe_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "SmartTrafficManagementSystem.exe"))
        else:
            exe_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "SmartTrafficManagementSystem"))
        
        self.backend = BackendClient(exe_path)
        try:
            self.backend.start()
            self.log_activity("Backend connected successfully.", True)
        except Exception as e:
            self.log_activity(f"Backend connection failed: {str(e)}", False)
            self.update_connection_status()

    def log_activity(self, message, success):
        import datetime
        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        status = "SUCCESS" if success else "ERROR"
        self.activity_logs.insert(0, {"time": now, "status": status, "message": message})
        
        if "activity_log" in self.pages:
            self.pages["activity_log"].refresh_data()

    def update_connection_status(self):
        if self.backend and self.backend.connected:
            self.conn_status_var.set("● Connected")
            self.conn_status_label.configure(text_color="#2ECC71") # Green
        else:
            self.conn_status_var.set("● Disconnected")
            self.conn_status_label.configure(text_color="#E74C3C") # Red

    def setup_pages(self):
        self.pages["dashboard"] = DashboardPage(self.content_frame, self)
        self.pages["normal_vehicles"] = NormalVehiclesPage(self.content_frame, self)
        self.pages["emergency_vehicles"] = EmergencyVehiclesPage(self.content_frame, self)
        self.pages["road_sensors"] = RoadSensorsPage(self.content_frame, self)
        self.pages["activity_log"] = ActivityLogPage(self.content_frame, self)
        self.pages["settings"] = SettingsPage(self.content_frame, self)

    def show_page(self, page_name):
        for name, btn in self.nav_buttons.items():
            if name == page_name:
                btn.configure(fg_color=("gray75", "gray25"))
            else:
                btn.configure(fg_color="transparent")

        if self.current_page:
            self.current_page.grid_forget()

        self.current_page = self.pages[page_name]
        self.current_page.grid(row=0, column=0, sticky="nsew")
        self.content_frame.grid_columnconfigure(0, weight=1)
        self.content_frame.grid_rowconfigure(0, weight=1)

        titles = {
            "dashboard": ("Dashboard", "Overview of current traffic states"),
            "normal_vehicles": ("Normal Vehicles", "Manage standard traffic queues"),
            "emergency_vehicles": ("Emergency Vehicles", "Manage high-priority vehicles"),
            "road_sensors": ("Road & Sensor Management", "Update simulated sensor data"),
            "activity_log": ("Activity Log", "Recent system actions"),
            "settings": ("Settings & About", "System information and backend control")
        }
        self.header_title.configure(text=titles[page_name][0])
        self.header_desc.configure(text=titles[page_name][1])
        
        self.current_page.refresh_data()
        self.update_connection_status()

    def on_closing(self):
        if self.backend:
            self.backend.stop()
        self.destroy()

if __name__ == "__main__":
    app = App()
    app.protocol("WM_DELETE_WINDOW", app.on_closing)
    app.mainloop()
