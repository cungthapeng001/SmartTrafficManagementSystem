import customtkinter as ctk
import os
import sys
import logging

from backend_client import BackendClient

from pages.dashboard import DashboardPage
from pages.normal_vehicles import NormalVehiclesPage
from pages.emergency_vehicles import EmergencyVehiclesPage
from pages.road_management import RoadManagementPage
from pages.road_sensors import RoadSensorsPage
from pages.route_planning import RoutePlanningPage
from pages.traffic_analysis import TrafficAnalysisPage
from pages.signal_control import SignalControlPage
from pages.activity_log import ActivityLogPage
from pages.settings import SettingsPage

from theme import COLORS

BG_DARK    = COLORS["bg_dark"]
BG_CARD    = COLORS["bg_card"]
BG_SIDEBAR = COLORS["bg_dark"]
ACCENT     = COLORS["accent"]
TEXT_MAIN  = COLORS["text_main"]
TEXT_MUTED = COLORS["text_muted"]
GREEN      = COLORS["green"]
YELLOW     = COLORS["yellow"]
RED        = COLORS["red"]
BORDER     = COLORS["border"]

class App(ctk.CTk):

    def __init__(self):
        super().__init__()

        self.title("Smart Traffic Management System")
        self.geometry("1280x820")
        self.minsize(1100, 680)

        ctk.set_appearance_mode("dark")
        ctk.set_default_color_theme("blue")

        self.configure(fg_color=BG_DARK)
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(1, weight=1)

        self.backend = None
        self.activity_logs = []
        self.pages = {}
        self.current_page = None
        self.nav_buttons = {}

        self.init_backend()

        self._build_sidebar()
        self._build_header()
        self._build_content_area()

        self.setup_pages()
        self.show_page("dashboard")

    def _build_sidebar(self):
        self.sidebar_frame = ctk.CTkFrame(
            self,
            width=230,
            corner_radius=0,
            fg_color=BG_SIDEBAR
        )
        self.sidebar_frame.grid(row=0, column=0, rowspan=2, sticky="nsew")
        self.sidebar_frame.grid_propagate(False)

        self.sidebar_frame.grid_columnconfigure(0, weight=1)
        self.sidebar_frame.grid_rowconfigure(3, weight=1)

        logo_label = ctk.CTkLabel(
            self.sidebar_frame,
            text="🚦 Smart Traffic\nManagement System",
            font=ctk.CTkFont(size=15, weight="bold"),
            text_color=TEXT_MAIN,
            justify="center"
        )
        logo_label.grid(row=0, column=0, padx=16, pady=(24, 16), sticky="ew")

        divider = ctk.CTkFrame(self.sidebar_frame, height=1, fg_color=BORDER)
        divider.grid(row=1, column=0, padx=16, pady=(0, 10), sticky="ew")

        top_nav_frame = ctk.CTkFrame(self.sidebar_frame, fg_color="transparent")
        top_nav_frame.grid(row=2, column=0, sticky="new", padx=10)
        top_nav_frame.grid_columnconfigure(0, weight=1)

        main_nav_items = [
            ("Dashboard",           "dashboard"),
            ("Normal Vehicles",     "normal_vehicles"),
            ("Emergency Vehicles",  "emergency_vehicles"),
            ("Road Management",     "road_management"),
            ("Road Sensors",        "road_sensors"),
            ("Route Planning",      "route_planning"),
            ("Traffic Analysis",    "traffic_analysis"),
            ("Signal Control",      "signal_control"),
        ]

        for i, (text, page_name) in enumerate(main_nav_items):
            btn = ctk.CTkButton(
                top_nav_frame,
                text=f"  {text}",
                anchor="w",
                fg_color="transparent",
                text_color=TEXT_MAIN,
                hover_color=BORDER,
                corner_radius=8,
                height=36,
                font=ctk.CTkFont(size=13),
                command=lambda p=page_name: self.show_page(p)
            )
            btn.grid(row=i, column=0, pady=2, sticky="ew")
            self.nav_buttons[page_name] = btn

        bottom_divider = ctk.CTkFrame(self.sidebar_frame, height=1, fg_color=BORDER)
        bottom_divider.grid(row=4, column=0, padx=16, pady=(6, 8), sticky="ew")

        bottom_nav_frame = ctk.CTkFrame(self.sidebar_frame, fg_color="transparent")
        bottom_nav_frame.grid(row=5, column=0, sticky="sew", padx=10, pady=(0, 16))
        bottom_nav_frame.grid_columnconfigure(0, weight=1)

        bottom_nav_items = [
            ("Activity Log", "activity_log"),
            ("Settings",     "settings"),
        ]

        for i, (text, page_name) in enumerate(bottom_nav_items):
            btn = ctk.CTkButton(
                bottom_nav_frame,
                text=f"  {text}",
                anchor="w",
                fg_color="transparent",
                text_color=TEXT_MUTED,
                hover_color=BORDER,
                corner_radius=8,
                height=36,
                font=ctk.CTkFont(size=13),
                command=lambda p=page_name: self.show_page(p)
            )
            btn.grid(row=i, column=0, pady=2, sticky="ew")
            self.nav_buttons[page_name] = btn

    def _build_header(self):
        self.header_frame = ctk.CTkFrame(
            self,
            corner_radius=0,
            fg_color=BG_DARK,
            height=70
        )
        self.header_frame.grid(row=0, column=1, sticky="nsew", padx=0, pady=0)
        self.header_frame.grid_columnconfigure(0, weight=1)
        self.header_frame.grid_propagate(False)

        self.header_title = ctk.CTkLabel(
            self.header_frame,
            text="Dashboard",
            font=ctk.CTkFont(size=22, weight="bold"),
            text_color=TEXT_MAIN
        )
        self.header_title.grid(row=0, column=0, sticky="w", padx=24, pady=(16, 0))

        self.header_desc = ctk.CTkLabel(
            self.header_frame,
            text="Multi-Road Traffic Overview",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED
        )
        self.header_desc.grid(row=1, column=0, sticky="w", padx=24, pady=(0, 10))

        self.conn_status_var = ctk.StringVar(value="● Connecting...")
        self.conn_status_label = ctk.CTkLabel(
            self.header_frame,
            textvariable=self.conn_status_var,
            font=ctk.CTkFont(size=12, weight="bold"),
            text_color=YELLOW
        )
        self.conn_status_label.grid(row=0, column=1, rowspan=2, sticky="e", padx=24)

        self.update_connection_status()

    def _build_content_area(self):
        self.content_frame = ctk.CTkFrame(
            self,
            corner_radius=12,
            fg_color=BG_DARK
        )
        self.content_frame.grid(row=1, column=1, sticky="nsew", padx=20, pady=(0, 20))
        self.content_frame.grid_columnconfigure(0, weight=1)
        self.content_frame.grid_rowconfigure(0, weight=1)

    def init_backend(self):
        if sys.platform == "win32":
            exe_path = os.path.abspath(
                os.path.join(os.path.dirname(__file__), "..", "SmartTrafficManagementSystem.exe")
            )
        else:
            exe_path = os.path.abspath(
                os.path.join(os.path.dirname(__file__), "..", "SmartTrafficManagementSystem")
            )

        self.backend = BackendClient(exe_path)
        try:
            self.backend.start()
            self.log_activity("Backend connected successfully (Multi-Road Engine).", True)
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
            self.conn_status_label.configure(text_color=GREEN)
        else:
            self.conn_status_var.set("● Disconnected")
            self.conn_status_label.configure(text_color=RED)

    def setup_pages(self):
        self.pages["dashboard"]          = DashboardPage(self.content_frame, self)
        self.pages["normal_vehicles"]    = NormalVehiclesPage(self.content_frame, self)
        self.pages["emergency_vehicles"] = EmergencyVehiclesPage(self.content_frame, self)
        self.pages["road_management"]    = RoadManagementPage(self.content_frame, self)
        self.pages["road_sensors"]       = RoadSensorsPage(self.content_frame, self)
        self.pages["route_planning"]     = RoutePlanningPage(self.content_frame, self)
        self.pages["traffic_analysis"]   = TrafficAnalysisPage(self.content_frame, self)
        self.pages["signal_control"]     = SignalControlPage(self.content_frame, self)
        self.pages["activity_log"]       = ActivityLogPage(self.content_frame, self)
        self.pages["settings"]           = SettingsPage(self.content_frame, self)

    def show_page(self, page_name):
        for name, btn in self.nav_buttons.items():
            if name == page_name:
                btn.configure(fg_color=ACCENT, text_color=TEXT_MAIN)
            else:
                btn.configure(fg_color="transparent")
                if name in ("activity_log", "settings"):
                    btn.configure(text_color=TEXT_MUTED)
                else:
                    btn.configure(text_color=TEXT_MAIN)

        if self.current_page:
            self.current_page.grid_forget()

        self.current_page = self.pages[page_name]
        self.current_page.grid(row=0, column=0, sticky="nsew")

        titles = {
            "dashboard":          ("Dashboard",                "Multi-Road Traffic Overview"),
            "normal_vehicles":    ("Normal Vehicles",          "Manage Standard FIFO Traffic Queues"),
            "emergency_vehicles": ("Emergency Vehicles",       "Manage Priority Emergency Queues"),
            "road_management":    ("Road Management",          "Configure Roads and Junction Topology"),
            "road_sensors":       ("Road Sensors",             "Simulated Sensor Data and Traffic Readings"),
            "route_planning":     ("Route Planning",           "Dijkstra Shortest Path Routing"),
            "traffic_analysis":   ("Traffic Analysis",         "Adaptive Road Traffic Weights"),
            "signal_control":     ("Signal Control",           "Adaptive Traffic Signal Simulation"),
            "activity_log":       ("Activity Log",             "System Audit Log and Actions"),
            "settings":           ("Settings & About",         "System Information and Backend Control"),
        }
        title_text, desc_text = titles.get(page_name, (page_name, ""))
        self.header_title.configure(text=title_text)
        self.header_desc.configure(text=desc_text)

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
