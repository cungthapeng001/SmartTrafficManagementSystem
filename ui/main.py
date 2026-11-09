import customtkinter as ctk
import os
import sys
import logging

# Import backend communication client
from backend_client import BackendClient

# Import all page modules
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

# ── Color palette aliases from theme ───────────────────────────────────────────
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
    """
    Main application window for the Smart Traffic Management System.
    Hosts the sidebar navigation, top header bar, and swappable content pages.
    """

    def __init__(self):
        super().__init__()

        self.title("Smart Traffic Management System")
        self.geometry("1280x820")
        self.minsize(1100, 680)

        # Apply dark theme globally
        ctk.set_appearance_mode("dark")
        ctk.set_default_color_theme("blue")

        # Configure root window grid
        self.configure(fg_color=BG_DARK)
        self.grid_columnconfigure(1, weight=1)   # Content stretches horizontally
        self.grid_rowconfigure(1, weight=1)       # Content stretches vertically

        # ── Application state ───────────────────────────────────────────────
        self.backend = None
        self.activity_logs = []   # In-memory list of activity log entries
        self.pages = {}           # Holds instantiated page objects keyed by name
        self.current_page = None  # Reference to the page currently displayed
        self.nav_buttons = {}     # Navigation button references

        # Start the C++ backend process
        self.init_backend()

        # ── Build UI sections ───────────────────────────────────────────────
        self._build_sidebar()
        self._build_header()
        self._build_content_area()

        # Instantiate all page objects and show dashboard first
        self.setup_pages()
        self.show_page("dashboard")

    # ── Sidebar ────────────────────────────────────────────────────────────────

    def _build_sidebar(self):
        """
        Build the left-side navigation sidebar.
        Top section holds primary navigation items.
        Bottom section holds Activity Log and Settings, pinned to the bottom.
        """
        self.sidebar_frame = ctk.CTkFrame(
            self,
            width=230,
            corner_radius=0,
            fg_color=BG_SIDEBAR
        )
        self.sidebar_frame.grid(row=0, column=0, rowspan=2, sticky="nsew")
        self.sidebar_frame.grid_propagate(False)

        # Configure grid rows:
        # Row 0: Logo
        # Row 1: Divider
        # Row 2: Main nav container
        # Row 3: Expanding spacer (pushes bottom nav down)
        # Row 4: Divider
        # Row 5: Bottom nav container (Activity Log, Settings)
        self.sidebar_frame.grid_columnconfigure(0, weight=1)
        self.sidebar_frame.grid_rowconfigure(3, weight=1)

        # ── Logo / branding ──
        logo_label = ctk.CTkLabel(
            self.sidebar_frame,
            text="🚦 Smart Traffic\nManagement System",
            font=ctk.CTkFont(size=15, weight="bold"),
            text_color=TEXT_MAIN,
            justify="center"
        )
        logo_label.grid(row=0, column=0, padx=16, pady=(24, 16), sticky="ew")

        # Divider below logo
        divider = ctk.CTkFrame(self.sidebar_frame, height=1, fg_color=BORDER)
        divider.grid(row=1, column=0, padx=16, pady=(0, 10), sticky="ew")

        # ── Top Navigation Frame ──
        top_nav_frame = ctk.CTkFrame(self.sidebar_frame, fg_color="transparent")
        top_nav_frame.grid(row=2, column=0, sticky="new", padx=10)
        top_nav_frame.grid_columnconfigure(0, weight=1)

        # Main navigation items (in exact requested order)
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

        # ── Row 3 is empty expanding space ──

        # ── Bottom Divider ──
        bottom_divider = ctk.CTkFrame(self.sidebar_frame, height=1, fg_color=BORDER)
        bottom_divider.grid(row=4, column=0, padx=16, pady=(6, 8), sticky="ew")

        # ── Bottom Navigation Frame (Pinned to bottom) ──
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

    # ── Header bar ─────────────────────────────────────────────────────────────

    def _build_header(self):
        """Build the top header bar showing page title and connection status."""
        self.header_frame = ctk.CTkFrame(
            self,
            corner_radius=0,
            fg_color=BG_DARK,
            height=70
        )
        self.header_frame.grid(row=0, column=1, sticky="nsew", padx=0, pady=0)
        self.header_frame.grid_columnconfigure(0, weight=1)
        self.header_frame.grid_propagate(False)

        # Page title
        self.header_title = ctk.CTkLabel(
            self.header_frame,
            text="Dashboard",
            font=ctk.CTkFont(size=22, weight="bold"),
            text_color=TEXT_MAIN
        )
        self.header_title.grid(row=0, column=0, sticky="w", padx=24, pady=(16, 0))

        # Page description subtitle
        self.header_desc = ctk.CTkLabel(
            self.header_frame,
            text="Multi-Road Traffic Overview",
            font=ctk.CTkFont(size=11),
            text_color=TEXT_MUTED
        )
        self.header_desc.grid(row=1, column=0, sticky="w", padx=24, pady=(0, 10))

        # Connection status pill (right side)
        self.conn_status_var = ctk.StringVar(value="● Connecting...")
        self.conn_status_label = ctk.CTkLabel(
            self.header_frame,
            textvariable=self.conn_status_var,
            font=ctk.CTkFont(size=12, weight="bold"),
            text_color=YELLOW
        )
        self.conn_status_label.grid(row=0, column=1, rowspan=2, sticky="e", padx=24)

        self.update_connection_status()

    # ── Content area ───────────────────────────────────────────────────────────

    def _build_content_area(self):
        """Build the main content frame where pages are swapped in and out."""
        self.content_frame = ctk.CTkFrame(
            self,
            corner_radius=12,
            fg_color=BG_DARK
        )
        self.content_frame.grid(row=1, column=1, sticky="nsew", padx=20, pady=(0, 20))
        self.content_frame.grid_columnconfigure(0, weight=1)
        self.content_frame.grid_rowconfigure(0, weight=1)

    # ── Backend initialization ─────────────────────────────────────────────────

    def init_backend(self):
        """Locate the C++ backend executable and start it as a subprocess."""
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

    # ── Logging ────────────────────────────────────────────────────────────────

    def log_activity(self, message, success):
        """Prepend a log entry to the in-memory activity list and refresh the Activity Log page."""
        import datetime
        now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        status = "SUCCESS" if success else "ERROR"
        self.activity_logs.insert(0, {"time": now, "status": status, "message": message})

        if "activity_log" in self.pages:
            self.pages["activity_log"].refresh_data()

    # ── Connection status ──────────────────────────────────────────────────────

    def update_connection_status(self):
        """Update header connection pill to reflect current backend state."""
        if self.backend and self.backend.connected:
            self.conn_status_var.set("● Connected")
            self.conn_status_label.configure(text_color=GREEN)
        else:
            self.conn_status_var.set("● Disconnected")
            self.conn_status_label.configure(text_color=RED)

    # ── Page setup ─────────────────────────────────────────────────────────────

    def setup_pages(self):
        """Instantiate all pages and store them in self.pages dict."""
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

    # ── Page navigation ────────────────────────────────────────────────────────

    def show_page(self, page_name):
        """
        Switch the content area to the named page.
        Updates nav button highlight and header text accordingly.
        """
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

    # ── Window close ──────────────────────────────────────────────────────────

    def on_closing(self):
        """Gracefully shut down the backend subprocess before closing the window."""
        if self.backend:
            self.backend.stop()
        self.destroy()


# ── Entry point ────────────────────────────────────────────────────────────────
if __name__ == "__main__":
    app = App()
    app.protocol("WM_DELETE_WINDOW", app.on_closing)
    app.mainloop()
