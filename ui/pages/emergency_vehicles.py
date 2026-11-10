import customtkinter as ctk
from theme import COLORS

BG_DARK    = COLORS["bg_dark"]
BG_CARD    = COLORS["bg_card"]
BG_ROW_ALT = COLORS["bg_row_alt"]
ACCENT     = COLORS["accent"]
TEXT_MAIN  = COLORS["text_main"]
TEXT_MUTED = COLORS["text_muted"]
GREEN      = COLORS["green"]
YELLOW     = COLORS["yellow"]
ORANGE     = "#E3671A"
RED        = COLORS["red"]
BORDER     = COLORS["border"]

EMERGENCY_ICONS = {
    "Ambulance":      "🚑",
    "Fire Truck":     "🚒",
    "Police Vehicle": "🚔",
}

PRIORITY_COLORS = {
    1: ("P1", RED),
    2: ("P2", ORANGE),
    3: ("P3", YELLOW),
}

ROAD_COLORS = {
    "NORTH":   "#1F6FEB",
    "SOUTH":   "#8957E5",
    "EAST":    "#238636",
    "WEST":    "#D29922",
    "CENTRAL": "#DA3633",
}

def get_road_badge_color(road_id):
    if road_id in ROAD_COLORS:
        return ROAD_COLORS[road_id]
    palette = ["#1F6FEB", "#8957E5", "#238636", "#D29922", "#E3671A", "#388BFD", "#A371F7"]
    idx = abs(hash(road_id)) % len(palette)
    return palette[idx]

class EmergencyVehiclesPage(ctk.CTkFrame):

    def __init__(self, master, app):
        super().__init__(master, fg_color="transparent")
        self.app = app
        self.available_roads = ["NORTH", "SOUTH", "EAST", "WEST", "CENTRAL"]

        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)

        self.id_var     = ctk.StringVar()
        self.type_var   = ctk.StringVar(value="Ambulance")
        self.road_var   = ctk.StringVar(value="NORTH")
        self.wait_var   = ctk.StringVar(value="0")
        self.filter_var = ctk.StringVar(value="ALL ROADS")

        self._build_form_card()
        self._build_table()
        self._build_action_row()

    def _build_form_card(self):
        self.form_frame = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        self.form_frame.grid(row=0, column=0, sticky="ew", padx=16, pady=(16, 8))
        self.form_frame.grid_columnconfigure((1, 3), weight=1)

        ctk.CTkLabel(
            self.form_frame,
            text="🚨  Add Emergency Vehicle",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=RED
        ).grid(row=0, column=0, columnspan=4, sticky="w", padx=16, pady=(14, 8))

        ctk.CTkLabel(self.form_frame, text="Vehicle ID:",
                     font=ctk.CTkFont(size=13), text_color=TEXT_MUTED
                     ).grid(row=1, column=0, padx=(16, 4), pady=10, sticky="e")

        ctk.CTkEntry(
            self.form_frame,
            textvariable=self.id_var,
            fg_color=BG_DARK,
            border_color=BORDER,
            text_color=TEXT_MAIN,
            placeholder_text="e.g. E-001",
            corner_radius=8
        ).grid(row=1, column=1, padx=(4, 20), pady=10, sticky="ew")

        ctk.CTkLabel(self.form_frame, text="Emergency Type:",
                     font=ctk.CTkFont(size=13), text_color=TEXT_MUTED
                     ).grid(row=1, column=2, padx=(0, 4), pady=10, sticky="e")

        ctk.CTkOptionMenu(
            self.form_frame,
            variable=self.type_var,
            values=["Ambulance", "Fire Truck", "Police Vehicle"],
            fg_color=BG_DARK,
            button_color=RED,
            button_hover_color="#a82a28",
            text_color=TEXT_MAIN,
            corner_radius=8
        ).grid(row=1, column=3, padx=(4, 16), pady=10, sticky="ew")

        ctk.CTkLabel(self.form_frame, text="Assigned Road:",
                     font=ctk.CTkFont(size=13), text_color=TEXT_MUTED
                     ).grid(row=2, column=0, padx=(16, 4), pady=10, sticky="e")

        self.road_menu = ctk.CTkOptionMenu(
            self.form_frame,
            variable=self.road_var,
            values=self.available_roads,
            fg_color=BG_DARK,
            button_color=ACCENT,
            button_hover_color="#1558c0",
            text_color=TEXT_MAIN,
            corner_radius=8
        )
        self.road_menu.grid(row=2, column=1, padx=(4, 20), pady=10, sticky="ew")

        ctk.CTkLabel(self.form_frame, text="Wait Time (s):",
                     font=ctk.CTkFont(size=13), text_color=TEXT_MUTED
                     ).grid(row=2, column=2, padx=(0, 4), pady=10, sticky="e")

        ctk.CTkEntry(
            self.form_frame,
            textvariable=self.wait_var,
            fg_color=BG_DARK,
            border_color=BORDER,
            text_color=TEXT_MAIN,
            placeholder_text="e.g. 60",
            corner_radius=8
        ).grid(row=2, column=3, padx=(4, 16), pady=10, sticky="ew")

        ctk.CTkButton(
            self.form_frame,
            text="🚨  Add Emergency Vehicle to Priority Queue",
            font=ctk.CTkFont(size=14, weight="bold"),
            fg_color=RED,
            hover_color="#a82a28",
            corner_radius=8,
            height=40,
            command=self.add_vehicle
        ).grid(row=3, column=0, columnspan=4, padx=16, pady=(8, 16))

    def _build_table(self):
        table_card = ctk.CTkFrame(
            self, fg_color=BG_CARD, corner_radius=12,
            border_width=1, border_color=BORDER
        )
        table_card.grid(row=1, column=0, sticky="nsew", padx=16, pady=8)
        table_card.grid_columnconfigure(0, weight=1)
        table_card.grid_rowconfigure(1, weight=1)

        hdr = ctk.CTkFrame(table_card, fg_color="transparent")
        hdr.grid(row=0, column=0, sticky="ew", padx=16, pady=(14, 4))
        hdr.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(
            hdr,
            text="Emergency Priority Queue (P1 > P2 > P3)",
            font=ctk.CTkFont(size=16, weight="bold"),
            text_color=TEXT_MAIN
        ).grid(row=0, column=0, sticky="w")

        filter_box = ctk.CTkFrame(hdr, fg_color="transparent")
        filter_box.grid(row=0, column=1, sticky="e")

        ctk.CTkLabel(filter_box, text="Filter by Road:", font=ctk.CTkFont(size=12), text_color=TEXT_MUTED).pack(side="left", padx=(0, 6))
        self.filter_menu = ctk.CTkOptionMenu(
            filter_box,
            variable=self.filter_var,
            values=["ALL ROADS"] + self.available_roads,
            fg_color=BG_DARK,
            button_color=RED,
            text_color=TEXT_MAIN,
            width=140,
            corner_radius=8,
            command=lambda _: self.refresh_data()
        )
        self.filter_menu.pack(side="left")

        self.table_frame = ctk.CTkScrollableFrame(
            table_card,
            fg_color="transparent",
            scrollbar_button_color=BORDER,
            scrollbar_button_hover_color=ACCENT
        )
        self.table_frame.grid(row=1, column=0, sticky="nsew", padx=8, pady=(0, 8))
        self.table_frame.grid_columnconfigure((0, 1, 2, 3, 4), weight=1)

        self.setup_table_headers()
        self.row_widgets = []

    def setup_table_headers(self):
        headers = ["Priority", "Vehicle ID", "Emergency Type", "Wait Time (s)", "Assigned Road"]
        for i, h in enumerate(headers):
            lbl = ctk.CTkLabel(
                self.table_frame,
                text=h,
                font=ctk.CTkFont(size=13, weight="bold"),
                text_color=TEXT_MUTED
            )
            lbl.grid(row=0, column=i, padx=12, pady=(8, 4), sticky="w")

    def clear_table(self):
        for widget in self.row_widgets:
            widget.destroy()
        self.row_widgets.clear()

    def add_row(self, row_idx, priority, vid, vtype, wait, road_id, display_name):
        row_bg = BG_CARD if row_idx % 2 == 0 else BG_ROW_ALT
        p_label, p_color = PRIORITY_COLORS.get(priority, (str(priority), TEXT_MUTED))
        road_color = get_road_badge_color(road_id)

        w1 = ctk.CTkLabel(
            self.table_frame,
            text=p_label,
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color=BG_DARK,
            fg_color=p_color,
            corner_radius=6,
            width=40
        )
        w1.grid(row=row_idx, column=0, padx=12, pady=2, sticky="w")

        w2 = ctk.CTkLabel(self.table_frame, text=vid,
                          font=ctk.CTkFont(size=13), text_color=TEXT_MAIN,
                          fg_color=row_bg, anchor="w")
        w2.grid(row=row_idx, column=1, padx=12, pady=2, sticky="ew")

        icon = EMERGENCY_ICONS.get(vtype, "🚨")
        w3 = ctk.CTkLabel(self.table_frame, text=f"{icon} {vtype}",
                          font=ctk.CTkFont(size=13), text_color=TEXT_MAIN,
                          fg_color=row_bg, anchor="w")
        w3.grid(row=row_idx, column=2, padx=12, pady=2, sticky="ew")

        w4 = ctk.CTkLabel(self.table_frame, text=str(wait),
                          font=ctk.CTkFont(size=13), text_color=TEXT_MAIN,
                          fg_color=row_bg, anchor="w")
        w4.grid(row=row_idx, column=3, padx=12, pady=2, sticky="ew")

        w5 = ctk.CTkLabel(
            self.table_frame, text=f"{display_name} ({road_id})",
            font=ctk.CTkFont(size=11, weight="bold"),
            text_color="#FFFFFF",
            fg_color=road_color,
            corner_radius=6,
            width=130
        )
        w5.grid(row=row_idx, column=4, padx=12, pady=2, sticky="w")

        self.row_widgets.extend([w1, w2, w3, w4, w5])

    def _build_action_row(self):
        self.action_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.action_frame.grid(row=2, column=0, sticky="ew", padx=16, pady=(4, 16))

        ctk.CTkButton(
            self.action_frame,
            text="🚨  Process Emergency Vehicle From Selected Road",
            fg_color=RED,
            hover_color="#a82a28",
            corner_radius=8,
            font=ctk.CTkFont(size=13, weight="bold"),
            height=38,
            command=self.process_selected_road
        ).pack(side="left", padx=(0, 8))

        ctk.CTkButton(
            self.action_frame,
            text="🔄  Refresh Priority Queues",
            fg_color=BG_CARD,
            hover_color=BORDER,
            border_color=BORDER,
            border_width=1,
            text_color=TEXT_MUTED,
            corner_radius=8,
            font=ctk.CTkFont(size=13),
            height=38,
            command=self.refresh_data
        ).pack(side="right")

    def update_road_list(self):
        if not self.app.backend or not self.app.backend.connected:
            return
        res = self.app.backend.send_request({"operation": "get_roads"})
        if res and res.get("success"):
            roads = [r["road_id"] for r in res["data"].get("roads", [])]
            if roads:
                self.available_roads = roads
                self.road_menu.configure(values=self.available_roads)
                self.filter_menu.configure(values=["ALL ROADS"] + self.available_roads)
                if self.road_var.get() not in self.available_roads:
                    self.road_var.set(self.available_roads[0])

    def add_vehicle(self):
        if not self.app.backend or not self.app.backend.connected:
            self.app.log_activity("Backend not connected", False)
            return

        try:
            wait = int(self.wait_var.get())
            if wait < 0:
                raise ValueError
        except Exception:
            self.app.log_activity("Invalid waiting time", False)
            return

        vehicle_id = self.id_var.get().strip()
        if not vehicle_id:
            self.app.log_activity("Vehicle ID cannot be empty", False)
            return

        type_val = self.type_var.get()
        priority = 3
        if type_val == "Ambulance":
            priority = 1
        elif type_val == "Fire Truck":
            priority = 2

        req = {
            "operation":    "add_emergency_vehicle",
            "road_id":      self.road_var.get(),
            "vehicle_id":   vehicle_id,
            "vehicle_type": type_val,
            "waiting_time": wait,
            "priority":     priority
        }
        res = self.app.backend.send_request(req)
        self.app.log_activity(res.get("message", "Error"), res.get("success", False))
        if res.get("success"):
            self.id_var.set("")
            self.refresh_data()

    def process_selected_road(self):
        selected_road = self.road_var.get()
        if not self.app.backend or not self.app.backend.connected:
            return
        res = self.app.backend.send_request(
            {"operation": "process_emergency_vehicle", "road_id": selected_road}
        )
        self.app.log_activity(res.get("message", "Error"), res.get("success", False))
        if res.get("success"):
            self.refresh_data()

    def refresh_data(self):
        if not self.app.backend or not self.app.backend.connected:
            return

        self.update_road_list()
        self.clear_table()

        filter_sel = self.filter_var.get()
        target_roads = self.available_roads if filter_sel == "ALL ROADS" else [filter_sel]

        row_idx = 1
        total_found = 0
        for road_id in target_roads:
            res = self.app.backend.send_request(
                {"operation": "get_emergency_vehicles", "road_id": road_id}
            )
            if res and res.get("success"):
                display_name = res["data"].get("display_name", road_id)
                for v in res["data"]["vehicles"]:
                    self.add_row(
                        row_idx,
                        v.get("priority", 3),
                        v["vehicle_id"],
                        v["vehicle_type"],
                        v["wait_time"],
                        road_id,
                        display_name
                    )
                    row_idx += 1
                    total_found += 1

        self.app.log_activity(f"Emergency vehicle queues refreshed ({total_found} active priority vehicles)", True)
